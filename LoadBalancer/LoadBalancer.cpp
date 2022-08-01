#include "LoadBalancer.h"
#include <iostream>
#include "UDPServerFactory.h"
#include "UDPMessageQueueFactory.h"
#include <unistd.h>
#include "UDPMessageInterface.h"
#include "CommonUtil.h"
#include "UDPLobbyRequest.h"
#include "UDPLobbyRejected.h"
#include "UDPLobbyData.h"
#include "UDPMessageFactory.h"
#include "ProcessHandler.h"
#include "UDPRoomListAnswer.h"
#include "UDPRoomListRequest.h"
#include <regex>
#include "BuildNumber.h"
#include <fstream>
#include "ParameterParser.h"
#include "stopwatch.h"
#include "Global.h"

static constexpr double SlaveMessageExpirationTime = 500; // milliseconds

void LoadBalancer::start() {
    udpMessageQueue_ = UDPMessageQueueFactory::create();
    udpServer_ = UDPServerFactory::create(udpMessageQueue_.get());
    udpServer_->start(serverPort_);
    std::cout << "LoadBalancer is running on port " << serverPort_ << "\n";
}

LoadBalancer::~LoadBalancer() {
    udpServer_->stop();
    udpServer_->join();
}

namespace {
StopWatch stopwatch;
struct OutputCheck : public OutputObserver {
    std::string buffer_;
    bool serverStopped_{};
    bool serverRunning_{};
    int clientsNumber_{1};
    std::regex pattern {R"(.*CLIENTS NUMBER: ([0-9]+).*)"};
    bool executionDone_{};

    void operator()(std::string str) override {
        std::cout << "Server log: " << str;
        std::cout.flush();
        buffer_.append(str);
        if(buffer_.find("RunServer OK") != std::string::npos) {
            serverRunning_ = true;
        }
        if(buffer_.find("RunServer stopped") != std::string::npos) {
            serverRunning_ = false;
            serverStopped_ = true;
        }
        std::smatch matches;
        if(std::regex_search(buffer_, matches, pattern) && matches.size() > 1) {
            int clientNum = std::atoi(matches[1].str().c_str());
            if(clientNum > clientsNumber_) {
                clientsNumber_ = clientNum;
                //std::cout << "Server log clientsNumber_: " << clientsNumber_ << "\n";
                //std::cout.flush();
            }
        }

        if(buffer_.length() > 100) {
            buffer_ = buffer_.substr(buffer_.length() - 100, 100);
        }
    }

    bool isAppReady() const noexcept override {
        return serverRunning_;
    }

    bool isStopped() const noexcept override {
        return serverStopped_;
    }

    int clientsNumber() const noexcept override {
        return clientsNumber_;
    }

    void executionDone() noexcept override {
        executionDone_ = true;
    }

    bool isExecutionDone() const noexcept override {
        return executionDone_;
    }
};

struct ErrorCheck : public ErrorObserver {
    void operator()(std::string str) override {
        std::cerr <<"error from Server: " << str << "\n";
        std::cerr.flush();
    }
};
}

bool LoadBalancer::handleLobbyRequest(UDPLobbyRequest* request, std::wstring gameName, UDPMessageInterfaceUPtr& answer, const UDPConnectionInterfaceSPtr& connection,
                                      const std::string& addrPort) {
    if(int addrIndex = occupyAddress(); addrIndex == -1) {
        // There is no free address
        // if we are the master lobby, let's ask slaves if they have free slots
        if(!slaveLobbyAddresses_.size()) {
            if(requestFromMaster_) {
                answer = UDPMessageFactory::createLobbyRejected(gameName, request->maxPlayers(), request->selectedMapNr(), request->selectedMinutesNr());
            } else {
                answer = UDPMessageFactory::createLobbyRejected(L"MASTER:" + std::wstring(addrPort.begin(), addrPort.end()) + L" " + gameName, request->maxPlayers(), request->selectedMapNr(), request->selectedMinutesNr());
            }
            debug("Lobby rejected");
        } else {
            waitingLobbyRequests_.insert(RequestedGames{gameName, request->selectedMapNr(), request->selectedMinutesNr(), connection, 0, now_ms() + SlaveMessageExpirationTime});
            sendLobbyRequests2ASlave(request, addrPort);
            return false;
        }

    } else {
        debug("DBG We should start a process!!!!!!!!!!!");

        // Let's start a server process and send the data back
        auto ochecker = new OutputCheck();
        OutputObserverPtr checker(ochecker);
        ErrorObserverPtr checker2(new ErrorCheck());
        ProcessHandlerUPtr processHandler = std::make_unique<ProcessHandler>(checker, checker2);

        debug("DBG 1 !!!!!!!!!!!");

        std::string mapParam = " -map " + std::to_string(request->selectedMapNr());
        std::string minutesParam = " -minutes " + std::to_string(request->selectedMinutesNr());

        debug("DBG 2 !!!!!!!!!!!");

        std::cout << std::string("./runServer -port ") + std::to_string(gameServerAddresses_[addrIndex].port)
                     + mapParam + minutesParam << "\n";

        processHandler->executeNonBlocking(std::string("./runServer -port ") + std::to_string(gameServerAddresses_[addrIndex].port)
                                           + mapParam + minutesParam);
        //processHandler->executeNonBlocking(std::string("./runValgrind -port ") + std::to_string(gameServerAddresses_[addrIndex].port)
        //                                   + mapParam + minutesParam);

        stopwatch.start("clientinfo");
        debug("DBG 3 !!!!!!!!!!!");
        std::cout << addrPort << " !!!!!!!!!!!!!!!!!!!!!!!!!\n";

        clients_.emplace(addrPort, ClientInfo{gameName, request->maxPlayers(), gameServerAddresses_[addrIndex].host,
                                              gameServerAddresses_[addrIndex].port, std::move(processHandler), addrIndex,
                                              request->selectedMapNr(), request->selectedMinutesNr(), now_ms()});

        debug("DBG 4 !!!!!!!!!!!: %u", stopwatch.timeMs("clientinfo"));

        answer = UDPMessageFactory::createLobbyData(request->maxPlayers(), gameServerAddresses_[addrIndex].host,
                                                    gameServerAddresses_[addrIndex].port, gameName,
                                                    request->selectedMapNr(), request->selectedMinutesNr());

        debug("Starting process, waiting for the proper output!!!!!!!!!!!!!!");

        while(!ochecker->isAppReady()) {
            usleep(5e3);
        }
        debug("New process has been started!!!!!!!!!!!!!!");
    }
    return true;
}

std::tuple<std::wstring, std::string> LoadBalancer::parseGameName(std::wstring gameName) {
    std::wcout << L"gameName: '" << gameName << L"'\n";
    size_t pos = gameName.find(' ');
    auto temp = gameName.substr(7, pos - 7);
    std::string addrPort = std::string(temp.begin(), temp.end());
    std::cout << "addrPort from MASTER: '" << addrPort << "'\n";
    gameName = gameName.substr(pos + 1);
    std::wcout << L"New gameName: '" << gameName << L"'\n";
    return std::make_tuple(gameName, addrPort);
}

void LoadBalancer::handleLobbyRequest(UDPMessageInterfaceUPtr& msg) {
    auto& connection = msg->clientConnection();
    //auto addrPort = connection->sinAddrPort();
    UDPLobbyRequest* request = msg->lobbyRequest();

    // Let's find out, if a master LoadBalancer sent this message or a mobile client
    requestFromMaster_ = !request->name().find(L"MASTER:");
    std::wstring gameName = request->name();
    std::string addrPort;
    if(requestFromMaster_) {
        std::tie(gameName, addrPort) = parseGameName(gameName);
    } else {
        addrPort = connection->sinAddrPort();
    }

    //debug("DBG clientId: %s", addrPort.c_str());
    debug("DBG maxPlayer: %u", request->maxPlayers());

    UDPMessageInterfaceUPtr answer;
    if(msg->buildNumber() != BuildNumber::n()) {
        //if(requestFromMaster_) {
            answer = UDPMessageFactory::createLobbyRejected(request->name(), request->maxPlayers(), request->selectedMapNr(), request->selectedMinutesNr());
        //} else {
            //answer = UDPMessageFactory::createLobbyRejected(L"MASTER:" + std::wstring(addrPort.begin(), addrPort.end()) + L" " + request->name(), request->maxPlayers(), request->selectedMapNr(), request->selectedMinutesNr());
        //}
        debug("LoadBalancer::handleLobbyRequest LOBBY REJECTED");
    } else {
        if(auto it = clients_.find(addrPort); it != clients_.end()) {
            // how is this possible??????????????
            debug("DBG old client");
            answer = UDPMessageFactory::createLobbyRejected(request->name(), request->maxPlayers(), request->selectedMapNr(), request->selectedMinutesNr());
/*
            if(!requestFromMaster_) {
                // Let's check the server process and send the data back
                answer = UDPMessageFactory::createLobbyData(it->second.maxPlayers, it->second.host, it->second.port, gameName,
                                                            request->selectedMapNr(), request->selectedMinutesNr());
            } else {
                if(!handleLobbyRequest(request, gameName, answer, connection, addrPort)) return;
            }
*/
        } else {
            debug("DBG new client");
            if(!handleLobbyRequest(request, gameName, answer, connection, addrPort)) return;
        }
    }

    answer->setClientConnection(connection);
    UDP udp;
    udp.sendMessage2Client(answer);
    debug("Lobby request has been answered");
}

void LoadBalancer::handleRoomListRequest(UDPMessageInterfaceUPtr& msg) {
    UDPRoomListRequest* roomListRequest = msg->roomListRequest();
    requestFromMaster_ = !roomListRequest->messageData().find(L"MASTER:");
    UDPMessageInterfaceUPtr answer;

    if(msg->buildNumber() != BuildNumber::n()) {
        if(requestFromMaster_) {
            answer = UDPMessageFactory::createLobbyRejected(roomListRequest->messageData(), 0, 0, 0);
        } else {
            answer = UDPMessageFactory::createLobbyRejected(L"", 0, 0, 0);
        }
    } else {
        if(!slaveLobbyAddresses_.size()) {
            answer = UDPMessageFactory::createRoomListAnswer();
            for(const auto& item : clients_) {
                UDPRoomListAnswer::MessageData data{
                    item.second.processHandler->outputObserver()->clientsNumber(),
                    item.second.gameName,
                    gameServerAddresses_[item.second.lobbiAddressesIndex].host,
                    gameServerAddresses_[item.second.lobbiAddressesIndex].port,
                    item.second.selectedMapNr,
                    item.second.selectedMinutesNr,
                    item.second.registrationTime
                };
                answer->roomListAnswer()->addData(data);
            }
            if(!requestFromMaster_) {
                // simple: send the answer as it is, nothing to do
                std::wcout << L"LoadBalancer::handleRoomListRequest no slaves, not from master\n";
            } else {
                std::wcout << L"LoadBalancer::handleRoomListRequest no slaves, from master\n";
                std::wcout << L"roomListRequest->messageData: " << roomListRequest->messageData() << "\n";
                // mark the answer
                //auto [gameName, addr] = parseGameName(roomListRequest->messageData());
                //answer->setLobbyCommInfo(L"MASTER:" + std::wstring(addr.begin(), addr.end()) + L" ");
                answer->roomListAnswer()->setLobbyCommInfo(roomListRequest->messageData());
            }
        } else {
            // forward the request
            if(!requestFromMaster_) {
                std::wcout << L"LoadBalancer::handleRoomListRequest there are slaves, not from master\n";
                auto addr = msg->clientConnection()->sinAddrPort();

                waitingRoomListRequests_.insert(RequestedRoomLists{std::wstring(addr.begin(), addr.end()), msg->clientConnection(), 0, now_ms() + SlaveMessageExpirationTime, {}, requestFromMaster_});
                sendRoomListRequest2AllSlaves(roomListRequest, addr);
            } else {
                std::wcout << L"LoadBalancer::handleRoomListRequest there slaves, from master\n";
                auto [gameName, addr] = parseGameName(roomListRequest->messageData());
                sendRoomListRequest2AllSlaves(roomListRequest, "");
                waitingRoomListRequests_.insert(RequestedRoomLists{std::wstring(addr.begin(), addr.end()), msg->clientConnection(), 0, now_ms() + SlaveMessageExpirationTime, {}, requestFromMaster_});
            }
            return;
        }
    }
    answer->setClientConnection(msg->clientConnection());
    UDP udp;
    //UDPMessageInterfaceUPtr convertedPtr = std::move(answer);
    //udp.sendMessage2Client(convertedPtr);
    udp.sendMessage2Client(answer);
    //debug("LoadBalancer::handleRoomListRequest %p", (void*)convertedPtr->payload());
    debug("LoadBalancer::handleRoomListRequest %p", (void*)answer->payload());
}

void LoadBalancer::handleMessages() {
    while(udpMessageQueue_->size()) {
        UDPMessageInterfaceUPtr msg = udpMessageQueue_->pop();
        if(!msg) {
            // TODO: log an error or something
            debug("ERROR: LoadBalancer::handleMessages: nullptr from UDP message queue");
            continue;
        }

        switch(msg->typeId()) {
        case UDPMessageInterface::TypeId::LOBBY_REQUEST: {
            debug("handleMessages: UDPMessageInterface::TypeId::LOBBY_REQUEST");
            if(msg->lobbyRequest()) {
                handleLobbyRequest(msg);
            }
        }
        break;
        case UDPMessageInterface::TypeId::ROOM_LIST_REQUEST: {
            debug("handleMessages: UDPMessageInterface::TypeId::ROOM_LIST_REQUEST");
            if(msg->roomListRequest()) {
                handleRoomListRequest(msg);
            }
            break;
        }
        case UDPMessageInterface::TypeId::EXIT_GAME:
            debug("handleMessages: UDPMessageInterface::TypeId::EXIT_GAME");
            if(msg->exitMessage()) {
                debug("ERROR: UDPMessageInterface::TypeId::EXIT_GAME");
            }
            break;
        case UDPMessageInterface::TypeId::ROOM_LIST_ANSWER:
        case UDPMessageInterface::TypeId::CONNECTION_REQUEST:
        case UDPMessageInterface::TypeId::RECONNECTION_REQUEST:
        case UDPMessageInterface::TypeId::SERVER_SNAPSHOT:
        case UDPMessageInterface::TypeId::SERVER_DELTA_SNAPSHOT:
        case UDPMessageInterface::TypeId::USER_COMMANDS:
        case UDPMessageInterface::TypeId::KEEP_ALIVE:
        case UDPMessageInterface::TypeId::LOBBY_DATA:
        case UDPMessageInterface::TypeId::LOBBY_REJECTED:
        case UDPMessageInterface::TypeId::LAST_ONE:
            debug("LoadBalancer::handleMessages %d", (int)msg->typeId());
            break;
        }
    }
    processSlaveMessages();
}

void LoadBalancer::handleLobbyRequestRejection(const UDPLobbyRejected* msg) {
    // A slave lobby server rejected the LOBBY_REQUEST. What to do? Try it again, max 3 times.
    requestFromMaster_ = !msg->name().find(L"MASTER:");

    auto clientIt = findAWaitingLobbyRequestClient(msg);
    if(clientIt == waitingLobbyRequests_.end()) return;
    if(incRejectionCount(clientIt)) {
        sendLobbyRequests2ASlave(msg, "");
    } else {
        forwardRejection2AClient(msg, clientIt);
        waitingLobbyRequests_.erase(clientIt);
    }
}

void LoadBalancer::handleRoomListAnswer(const UDPRoomListAnswer* msg) {
    std::wcout << "LoadBalancer::handleRoomListAnswer: " << msg->lobbyCommInfo() << "\n";
    requestFromMaster_ = !msg->lobbyCommInfo().find(L"MASTER:");

    auto clientIt = findAWaitingRoomListRequestClient(msg);
    if(clientIt == waitingRoomListRequests_.end()) return;
    ++clientIt->receivedAnswerCount;
    mergeRoomListAnswer2List(msg, clientIt);
    if(clientIt->receivedAnswerCount >= slaveLobbyAddresses_.size()) {
        forwardRoomListAnswer2TheClient(msg, clientIt);
        waitingRoomListRequests_.erase(clientIt);
    }
}

LoadBalancer::waitingLRit2 LoadBalancer::findAWaitingRoomListRequestClient(const UDPRoomListAnswer* msg) {
    if(!waitingRoomListRequests_.size()) return waitingRoomListRequests_.end();
    //if(requestFromMaster_) {
        auto [gameName, addr] = parseGameName(msg->lobbyCommInfo());
        RequestedRoomLists r{std::wstring(addr.begin(), addr.end()), 0, 0, 0, {}, false};
        return waitingRoomListRequests_.find(r);
    //}
    //RequestedRoomLists r{msg->lobbyCommInfo(), 0, 0, 0, {}};
    //return waitingRoomListRequests_.find(r);
}

void LoadBalancer::forwardRoomListAnswer2TheClient(const UDPRoomListAnswer* msg, LoadBalancer::waitingLRit2 clientIt) {
    UDPMessageInterfaceUPtr newMsg;
    newMsg = UDPMessageFactory::createRoomListAnswer();
    if(requestFromMaster_) {
        newMsg->roomListAnswer()->setLobbyCommInfo(msg->lobbyCommInfo());
    } else if(clientIt->requestFromMaster) {
        newMsg->roomListAnswer()->setLobbyCommInfo(clientIt->messageData);
    }

    for(auto& x : clientIt->roomList) {
        newMsg->roomListAnswer()->addData(x);
    }

    newMsg->setClientConnection(clientIt->connection);
    UDP udp;
    udp.sendMessage2Client(newMsg);
}

void LoadBalancer::mergeRoomListAnswer2List(const UDPRoomListAnswer* msg, LoadBalancer::waitingLRit2 clientIt) {
    if(msg) {
        for(auto& x : msg->roomList()) {
            clientIt->roomList.push_back(x);
        }
    }

    for(const auto& item : clients_) {
        UDPRoomListAnswer::MessageData data{
            item.second.processHandler->outputObserver()->clientsNumber(),
            item.second.gameName,
            gameServerAddresses_[item.second.lobbiAddressesIndex].host,
            gameServerAddresses_[item.second.lobbiAddressesIndex].port,
            item.second.selectedMapNr,
            item.second.selectedMinutesNr,
            item.second.registrationTime
        };
        clientIt->roomList.push_back(data);
    }
}

LoadBalancer::waitingLRit LoadBalancer::findAWaitingLobbyRequestClient(const UDPLobbyRejected* msg) {
    if(!waitingLobbyRequests_.size()) return waitingLobbyRequests_.end();
    // What are the request's parameters?
    if(requestFromMaster_) {
        auto [gameName, addr] = parseGameName(msg->name());
        RequestedGames r{gameName, msg->selectedMapNr(), msg->selectedMinutesNr(), 0, 0, 0};
        return waitingLobbyRequests_.find(r);
    }
    RequestedGames r{msg->name(), msg->selectedMapNr(), msg->selectedMinutesNr(), 0, 0, 0};
    return waitingLobbyRequests_.find(r);
}

bool LoadBalancer::incRejectionCount(waitingLRit clientIt) {
    return ++clientIt->rejectionCount < 4;
}

void LoadBalancer::forwardRejection2AClient(const UDPLobbyRejected* msg, waitingLRit clientIt) {
    UDPMessageInterfaceUPtr newMsg;
    if(requestFromMaster_) {
        auto [gameName, addr] = parseGameName(msg->name());
        newMsg = UDPMessageFactory::createLobbyRejected(gameName, msg->maxPlayers(), msg->selectedMapNr(), msg->selectedMinutesNr());
    } else {
        if(msg) {
            newMsg = UDPMessageFactory::createLobbyRejected(msg->name(), msg->maxPlayers(), msg->selectedMapNr(), msg->selectedMinutesNr());
        } else {
            newMsg = UDPMessageFactory::createLobbyRejected(clientIt->gameName, 0, clientIt->selectedMapNr, clientIt->selectedMinutesNr);
        }
    }

    newMsg->setClientConnection(clientIt->connection);
    UDP udp;
    udp.sendMessage2Client(newMsg);
}

void LoadBalancer::forwardLobbyData2AClient(const UDPLobbyData* msg) {
    debug("DBG LoadBalancer::forwardLobbyData2AClient 1");
    if(!waitingLobbyRequests_.size()) return;
    debug("DBG LoadBalancer::forwardLobbyData2AClient 2");

    std::wcout << msg->name() << " " << msg->selectedMapNr() << " " << msg->selectedMinutesNr() << "\n";
    // What are the request's parameters?
    RequestedGames r{msg->name(), msg->selectedMapNr(), msg->selectedMinutesNr(), 0, 0, 0};

    auto it = waitingLobbyRequests_.find(r);
    if(it == waitingLobbyRequests_.end()) return;

    debug("DBG LoadBalancer::forwardLobbyData2AClient 11");

    UDPMessageInterfaceUPtr newMsg = UDPMessageFactory::createLobbyData(msg->maxPlayers(), msg->host(), msg->port(), msg->name(), msg->selectedMapNr(), msg->selectedMinutesNr());
    newMsg->setClientConnection(it->connection);
    UDP udp;
    udp.sendMessage2Client(newMsg);
    waitingLobbyRequests_.erase(it);

    debug("DBG LoadBalancer::forwardLobbyData2AClient 111");
}

void LoadBalancer::sendLobbyRequests2ASlave(const UDPLobbyRequest* msg, const std::string& addrPort) {
    // We will send a LOBBY_REQUEST to a random slave
    UDPMessageInterfaceUPtr newMsg;
    if(requestFromMaster_) {
        newMsg = UDPMessageFactory::createLobbyRequest(msg->name(), msg->maxPlayers(), msg->selectedMapNr(), msg->selectedMinutesNr());
    } else {
        newMsg = UDPMessageFactory::createLobbyRequest(L"MASTER:" + std::wstring(addrPort.begin(), addrPort.end()) + L" " + msg->name(), msg->maxPlayers(), msg->selectedMapNr(), msg->selectedMinutesNr());
    }
    size_t lobby = rand() % slaveLobbyAddresses_.size();
    auto& addr = slaveLobbyAddresses_[lobby];
    if(addr.connection) {
        UDP udp;
        newMsg->setClientConnection(addr.connection);
        if(udp.sendMessage2Client(newMsg)) {
            debug("LoadBalancer::sendLobbyRequests2ASlave 1");
            std::wcout << msg->name() << " " << msg->selectedMapNr() << " " << msg->selectedMinutesNr() << "\n";
        }
    } else {
        UDP udp(addr.host, addr.port, true);
        if(udp.sendMessage2Server(newMsg)) {
            debug("LoadBalancer::sendLobbyRequests2ASlave 2");
            std::wcout << msg->name() << " " << msg->selectedMapNr() << " " << msg->selectedMinutesNr() << "\n";
            addr.connection = UDP::clientConnection();
        }
    }
}

void LoadBalancer::sendRoomListRequest2AllSlaves(UDPRoomListRequest* msg, std::string addrPort) {
    UDPMessageInterfaceUPtr newMsg;
    if(requestFromMaster_) {
        newMsg = UDPMessageFactory::createRoomListRequest(msg->messageData());
    } else {
        newMsg = UDPMessageFactory::createRoomListRequest(L"MASTER:" + std::wstring(addrPort.begin(), addrPort.end()) + L" ");
    }

    for(auto& addr : slaveLobbyAddresses_) {
        if(addr.connection) {
            UDP udp;
            newMsg->setClientConnection(addr.connection);
            if(udp.sendMessage2Client(newMsg)) {
                debug("LoadBalancer::sendRoomListRequest2AllSlaves 1");
                std::wcout << msg->messageData() << "\n";
            }
        } else {
            UDP udp(addr.host, addr.port, true);
            if(udp.sendMessage2Server(newMsg)) {
                debug("LoadBalancer::sendRoomListRequest2AllSlaves 2");
                std::wcout << msg->messageData() << "\n";
                addr.connection = UDP::clientConnection();
            }
        }
    }
}

int LoadBalancer::occupyAddress() noexcept {
    for(size_t i = 0; i < gameServerAddresses_.size(); ++i) {
        if(!gameServerAddresses_[i].occupied) {
            gameServerAddresses_[i].occupied = true;
            return i;
        }
    }
    return -1;
}

// https://github.com/rud/name-generator/blob/master/lib/name_generator/main.rb
// https://github.com/EugeneDanini/nickname-generator/blob/master/nickname_generator/__init__.py
std::wstring LoadBalancer::generateUniqueName() noexcept {
    int index = 0;
    while(true) {
        bool found = false;
        std::wstring name = L"Noname " + std::to_wstring(index);
        for(const auto& item : clients_) {
            if(name == item.second.gameName) {
                found = true;
                break;
            }
        }
        if(!found) return name;
        ++index;
    }
    return L"";
}

void LoadBalancer::deleteStoppedProcesses() noexcept {
    for(auto it = clients_.begin(); it != clients_.end();) {
        if(it->second.processHandler->outputObserver()->isStopped() ||
        it->second.processHandler->outputObserver()->isExecutionDone()) {
            gameServerAddresses_[it->second.lobbiAddressesIndex].occupied = false;
            it = clients_.erase(it);
            debug("Client has been deleted, new client number is %lu", clients_.size());
        } else {
            ++it;
        }
    }
}

void LoadBalancer::loadConfigFile(std::string configFile) {
    if(!configFile.length()) configFile = "./LoadBalancer.cfg";
    std::ifstream file(configFile.data(), std::ios::in);
    if(!file.is_open()) {
        throw std::runtime_error(std::string("Configuration file not found: ") + std::string(configFile));
    }
    std::regex pattern {R"x(^\s*([a-zA-Z0-9_\-]+)\s*=\s*([a-zA-Z0-9_\-\:\.]+))x"};
    std::smatch matches;
    std::string line;

    auto toGameServerAddress = [&](std::string& str, std::vector<GameServerInfo>& addresses) {
        auto sep = str.find(':');
        if(sep != std::string::npos) {
            GameServerInfo info{str.substr(0, sep), (uint16_t)std::stoi(str.substr(sep + 1)), false};
            addresses.push_back(info);
        }
    };

    auto toSlaveAddress = [&](std::string& str, std::vector<SlaveInfo>& addresses) {
        auto sep = str.find(':');
        if(sep != std::string::npos) {
            SlaveInfo info{str.substr(0, sep), (uint16_t)std::stoi(str.substr(sep + 1)), nullptr};
            addresses.push_back(info);
        }
    };

    while(!file.eof()) {
        getline(file, line);
        if(line.length() == 0) continue;
        if(std::regex_search(line, matches, pattern) && matches.size() > 2) {
            //std::cout << "'" << matches[1] << "'  =  '" << matches[2] << "'\n";
            auto str1 = matches[1].str();
            auto str2 = matches[2].str();
            if(str1.find("server") == 0) {
                toGameServerAddress(str2, gameServerAddresses_);
                std::cout << "server: " << gameServerAddresses_.back().host << ", port: " << gameServerAddresses_.back().port << std::endl;
            } else if(str1.find("slave") == 0) {
                toSlaveAddress(str2, slaveLobbyAddresses_);
                std::cout << "slave: " << slaveLobbyAddresses_.back().host << ", port: " << slaveLobbyAddresses_.back().port << std::endl;
            } else {
                std::cout << "Unknown configuration options: " << line << "\n";
                config_.emplace(matches[1].str(), matches[2].str());
            }
        }
    }
    file.close();
}

LoadBalancer::LoadBalancer(std::string configFile, int serverPort) {
    loadConfigFile(configFile);
    serverPort_ = serverPort;
}

void LoadBalancer::processSlaveMessages(UDPConnectionInterfaceSPtr& connection) {
    udpSlaveServer_->readClientConnection(connection);
    while(udpSlaveMessageQueue_->size()) {
        debug("LoadBalancer::processSlaveMessages");
        UDPMessageInterfaceUPtr msg = udpSlaveMessageQueue_->pop();
        if(!msg) {
            // TODO: log an error or something
            debug("ERROR: processSlaveMessages: nullptr from UDP message queue");
            continue;
        }
        switch(msg->typeId()) {
        case UDPMessageInterface::TypeId::LOBBY_REQUEST: {
            debug("ERROR processSlaveMessages: UDPMessageInterface::TypeId::LOBBY_REQUEST");
        }
        break;
        case UDPMessageInterface::TypeId::ROOM_LIST_REQUEST: {
            debug("ERROR processSlaveMessages: UDPMessageInterface::TypeId::ROOM_LIST_REQUEST");
            break;
        }
        case UDPMessageInterface::TypeId::EXIT_GAME:
            debug("ERROR processSlaveMessages: UDPMessageInterface::TypeId::EXIT_GAME");
            break;
        case UDPMessageInterface::TypeId::CONNECTION_REQUEST:
        case UDPMessageInterface::TypeId::RECONNECTION_REQUEST:
        case UDPMessageInterface::TypeId::SERVER_SNAPSHOT:
        case UDPMessageInterface::TypeId::SERVER_DELTA_SNAPSHOT:
        case UDPMessageInterface::TypeId::USER_COMMANDS:
        case UDPMessageInterface::TypeId::KEEP_ALIVE:
        case UDPMessageInterface::TypeId::LAST_ONE:
            debug("ERROR LoadBalancer::processSlaveMessages %d", (int)msg->typeId());
            break;
        case UDPMessageInterface::TypeId::LOBBY_DATA:
            debug("DBG processSlaveMessages: UDPMessageInterface::TypeId::LOBBY_DATA");
            // A slave lobby server answered and sent the data of the running server. Let's forward it to a client.
            forwardLobbyData2AClient(msg->lobbyData());
            break;
        case UDPMessageInterface::TypeId::LOBBY_REJECTED:
            debug("processSlaveMessages: UDPMessageInterface::TypeId::LOBBY_REJECTED");
            handleLobbyRequestRejection(msg->lobbyRejected());
            break;
        case UDPMessageInterface::TypeId::ROOM_LIST_ANSWER:
            debug("processSlaveMessages: UDPMessageInterface::TypeId::ROOM_LIST_ANSWER");
            handleRoomListAnswer(msg->roomListAnswer());
            break;
        }
    }
}

void LoadBalancer::processSlaveMessages() {
    if(!udpSlaveServer_) {
        udpSlaveMessageQueue_ = UDPMessageQueueFactory::create();
        udpSlaveServer_ = UDPServerFactory::create(udpSlaveMessageQueue_.get());
         // The slaves does NOT need a server thread
    }

    for(auto& slaveInfo : slaveLobbyAddresses_) {
        processSlaveMessages(slaveInfo.connection);
    }
}

void LoadBalancer::sendAnswerToExpiredRoomListRequests() {
    requestFromMaster_ = false;
    for(auto it = waitingRoomListRequests_.begin(); it != waitingRoomListRequests_.end();) {
        if(it->expirationTime > now_ms()) {
            ++it;
            continue;
        }
        mergeRoomListAnswer2List(nullptr, it);
        forwardRoomListAnswer2TheClient(nullptr, it);
        it = waitingRoomListRequests_.erase(it);
    }
}

void LoadBalancer::sendRejectionToExpiredGameRequests() {
    requestFromMaster_ = false;
    for(auto it = waitingLobbyRequests_.begin(); it != waitingLobbyRequests_.end();) {
        if(it->expirationTime > now_ms()) {
            ++it;
            continue;
        }
        std::wcout << L"sendRejectionToExpiredGameRequests@@@@@@@@@@@@\n";
        forwardRejection2AClient(nullptr, it);
        it = waitingLobbyRequests_.erase(it);
    }
}

int main([[maybe_unused]]int argc, [[maybe_unused]]const char* argv[]) {
    std::string configFile;
    int serverPort = 44444;
    ParameterParser parameterParser(argc, argv);

    size_t pos = parameterParser.find("-cfg");
    if(pos + 1 < parameterParser.size()) {
        configFile = parameterParser.at(pos + 1);
    }
    pos = parameterParser.find("-port");
    if(pos + 1 < parameterParser.size()) {
        serverPort = std::stoi(parameterParser.at(pos + 1));
    }

    auto loadBalancer = std::make_unique<LoadBalancer>(configFile, serverPort);
    loadBalancer->start();
    while(true) {
        loadBalancer->handleMessages();
        loadBalancer->deleteStoppedProcesses();
        loadBalancer->sendAnswerToExpiredRoomListRequests();
        loadBalancer->sendRejectionToExpiredGameRequests();
        update_now();
        usleep(1e4);
    }
    return EXIT_SUCCESS;
}
