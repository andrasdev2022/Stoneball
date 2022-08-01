#include "ServerApplication.h"

#include "Global.h"
#include "Bitmap565.h"
#include "qtutil.h"
#include "mock.h"
#include "BitmapIds.h"
#include "MediaData.h"
#include "GameObject.h"
#include "Application.h"
#include "UDPServer.h"
#include "UDPMessageQueue.h"
#include "UDPConnectionRequest.h"
#include "UDPReconnectionRequest.h"
#include "UDPServerSnapshot.h"
#include "UDPServerDeltaSnapshot.h"
#include "UDPUserCommands.h"
#include "UDPServerFactory.h"
#include "UDPMessageFactory.h"
#include "Button.h"

#include <QDir>
#include <gtest/gtest.h>
#include <iostream>
#include <QImage>
#include <QPainter>
#include <unordered_map>
#include "ClientTimeSyncer.h"
#include "Environment.h"
#include "MultiplayerWorld.h"
#include "Defines.h"
#include "Algorithms.h"
#include "BitmapLoader.h"

using namespace Algorithms;
using namespace QtUtil;
using namespace Bitmap565;

namespace ServerApplication {
    const double designScreenWidth  = 569.;
    const double designScreenHeight = 320.;
    const double actualScreenWidth  = designScreenWidth;
    const double actualScreenHeight = designScreenHeight;
    THREAD_LOCAL std::unique_ptr<uint16_t[]> pixels;
    THREAD_LOCAL double now;
    const size_t iterationSleep_ = 2000; // microsec
    const size_t timeStep_ = 15; // tickrate = 66 ticks/sec, timestep = 15 millisec
    const size_t snapshotStep_ = ClientTimeSyncer::snapshotStep_; // snapshotrate = 20 snapshot/sec, snapshotstep = 50 millisec
    THREAD_LOCAL AndroidBitmapInfo bitmapInfo{(int)actualScreenWidth, (int)actualScreenHeight, (int)actualScreenWidth * (int)sizeof(uint16_t)};
    THREAD_LOCAL UDPMessageQueue udpMessageQueue_;
    THREAD_LOCAL UDPServerInterfaceUPtr udpServer_;
    THREAD_LOCAL uint32_t lastPlayerId{};
    THREAD_LOCAL std::vector<uint8_t> quitters; // GameObject::Type items

    struct ClientInfo {
        GameObject::Type gameObjectType;
        UDPConnectionInterfaceSPtr clientConnection;
        size_t fullSnapshotTime{};
        uint32_t playerId;
        bool viewOnly;
        std::wstring teamName;
    };

    THREAD_LOCAL std::unordered_map<std::string, ClientInfo> clients_;
    THREAD_LOCAL std::list<UDPMessageInterfaceUPtr> snapshots_;

    void InitServer(int port, int mapIndex, int minutes) {
        srand(time(nullptr));

        BitmapLoader bitmapLoader;
        bitmapLoader.loadDrawableImages();

        // planned screen size / actual screen size
        calcZoomRate(actualScreenWidth, actualScreenHeight, designScreenWidth, designScreenHeight);
        setScreenSize(actualScreenWidth, actualScreenHeight);

        bitmapLoader.convertImagesToRGB565();
        pixels = std::make_unique<uint16_t[]>(actualScreenWidth * actualScreenHeight);
        //system("rm -f tmp/*.png");
        udpServer_ = UDPServerFactory::create(&udpMessageQueue_);
        udpServer_->start(port);
        if(mapIndex == 0) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP1);
        } else if(mapIndex == 1) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP2);
        } else if(mapIndex == 2) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP3);
        } else if(mapIndex == 3) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP4);
        } else if(mapIndex == 4) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP5);
        } else if(mapIndex == 5) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP6);
        }
        // The real one will be set by registerClient
        setAvatar(Application::Avatar::UNDEFINED);
        Application::setPlayTime(minutes);
    }

    bool isRegistered(UDP::Message* msg) {
        auto addrPort = msg->clientConnection()->sinAddrPort();
        //LOGI("DBG isRegistered: clientId: %s", addrPort.c_str());
        auto it = clients_.find(addrPort);
        if(it == clients_.end()) {
            return 0;
        }
        return true;
    }

    bool updateConnectionData(UDPReconnectionRequest* msg) noexcept {
        auto it = std::find_if(clients_.begin(), clients_.end(), [&msg](const auto& p){ return p.second.playerId == msg->clientId();});
        if(it != clients_.end()) {
            auto addrPort = msg->clientConnection()->sinAddrPort();
            ClientInfo newInfo{it->second.gameObjectType, msg->clientConnection(), it->second.fullSnapshotTime, it->second.playerId, it->second.viewOnly, it->second.teamName};
            udpServer_->removeClientConnection(it->second.clientConnection);
            clients_.erase(it);
            clients_.emplace(addrPort, newInfo);
            debug("updateConnectionData: Client connection has been updated");
            return true;
        }
        return false;
    }

    GameObjectInterface::Type preparePlayerType(int maxPlayerCount) noexcept {
        if((int)clients_.size() >= maxPlayerCount) {
            return GameObjectInterface::Type::GOT_KNIGHT_RED;
        }
        std::set<GameObjectInterface::Type> choices{GameObjectInterface::Type::GOT_KNIGHT_RED, GameObjectInterface::Type::GOT_KNIGHT_DARK};

        if(maxPlayerCount >= 3) {
            choices.insert(GameObjectInterface::Type::GOT_KNIGHT_BLONDE_FEMALE);
        }

        if(maxPlayerCount >= 4) {
            choices.insert(GameObjectInterface::Type::GOT_KNIGHT_GREEN_FEMALE);
        }

        for(auto& client : clients_) {
            choices.erase(client.second.gameObjectType);
        }
        int choosenIndex = rand() % choices.size();
        debug("preparePlayerType choosenIndex: %d", choosenIndex);
        return *std::next(choices.begin(), choosenIndex);
        //return *choices.begin();
    }

    //std::string registerClient(UDPConnectionRequest* msg) {
    template<typename T>
    std::string registerClient(T* msg) {
        auto addrPort = msg->clientConnection()->sinAddrPort();
        debug("DBG registerClient: clientId: %s", addrPort.c_str());
        //std::wcout << L"client's team name: '" << msg->teamName() << L"'" << std::endl;

        int maxPlayerCount = 2;
        if(anyOf(Application::gameLevel(), LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP3,
                 LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP4)) {
            maxPlayerCount = 3;
        }
        if(anyOf(Application::gameLevel(), LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP5,
                 LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP6)) {
            maxPlayerCount = 4;
        }

        clients_.emplace(addrPort, ClientInfo{preparePlayerType(maxPlayerCount), msg->clientConnection(), 0, ++lastPlayerId,
                         Application::furtherPlayersViewOnly(), msg->teamName()});

        if(clients_.size() == 1) {
            setAvatar(Application::GOT2Avatar(clients_.begin()->second.gameObjectType));
        }
        // The LoadBalancer reads this one
        debug("CLIENTS NUMBER: %u", clients_.size());
        return addrPort;
    }

    void storeSnapshot(UDPMessageInterfaceUPtr& snapshot) {
        snapshots_.push_back(std::move(snapshot));
        while(snapshots_.size() > 3) {
            snapshots_.erase(snapshots_.begin());
        }
    }

    std::vector<std::wstring> collectTeamNames() noexcept {
        std::vector<std::wstring> retv(4);
        for(auto& playerPair : clients_) {
            if(playerPair.second.viewOnly) continue;
            if(anyOf(playerPair.second.gameObjectType, GameObjectInterface::Type::GOT_KNIGHT_RED,
                     GameObjectInterface::Type::GOT_KNIGHT_DARK,
                     GameObjectInterface::Type::GOT_KNIGHT_BLONDE_FEMALE,
                     GameObjectInterface::Type::GOT_KNIGHT_GREEN_FEMALE)) {
                retv[static_cast<int>(playerPair.second.gameObjectType)] = playerPair.second.teamName;
                Application::tileset()->setTeamName(static_cast<int>(playerPair.second.gameObjectType), playerPair.second.teamName);
                //std::wcout << L"collected team name: '" << playerPair.second.teamName << L"'" << std::endl;
            }
        }
        return retv;
    }

    void sendSnapshot(double now) {
        if(!clients_.size()) {
            //debug("sendSnapshot: there are no clients");
            return;
        }

        //std::cout << "Now: " << std::scientific << std::setw(20) << std::fixed << now << "\n";
        //  1. generate a full snapshot (new one)
        //  2. generate a delta snapshot from two full snapshots and send it
        //UDPServerSnapshotUPtr snapshot = std::make_unique<UDPServerSnapshot>();
        UDPMessageInterfaceUPtr msg = UDPMessageFactory::create(UDPMessageInterface::TypeId::SERVER_SNAPSHOT);

        UDP udp;
        auto multiplayerWorld = Application::latestWorld()->multiplayer();
        auto snapshot = msg->serverSnapshot();
        uint16_t elapsedGameTime3{};
        uint16_t elapsedGameTime4{};
        if(anyOf(Application::gameLevel(), LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP3, LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP4,
                 LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP5, LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP6)) {
            elapsedGameTime3 = Application::tileset()->elapsedGameTime(2);
        }
        if(anyOf(Application::gameLevel(), LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP5, LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP6)) {
            elapsedGameTime4 = Application::tileset()->elapsedGameTime(3);
        }
        snapshot->addData(multiplayerWorld, 0, now, (uint16_t)Application::tileset()->historySize(), Application::environment()->activeKnightId(),
                          Application::tileset()->activeTeamId(), Application::tileset()->score(),
                          Application::playTime() * 60 - Application::tileset()->elapsedGameTime(0),
                          Application::playTime() * 60 - Application::tileset()->elapsedGameTime(1),
                          Application::playTime() * 60 - elapsedGameTime3,
                          Application::playTime() * 60 - elapsedGameTime4,
                          Application::playTime(),
                          Application::tileset()->winnerTeam(), 0, 0, quitters, collectTeamNames(), Application::viewerCount());

        //debug("sendSnapshot: %d, %d", Application::playTime() * 60 - Application::tileset()->elapsedGameTime(0),
        //      Application::playTime() * 60 - Application::tileset()->elapsedGameTime(1));

        snapshot->payload(); // The serialized payload must be generated
        snapshot->buildFrom(snapshot->payload(), snapshot->length(), nullptr);

        //debug("sendDeltaSnapshot() full length: %u, delta length: %u", (unsigned)snapshot->length(), (unsigned)deltaSnapshot->length());
        //std::cout << "sendDeltaSnapshot() full length: " << (unsigned)snapshot->length() << " delta length: "  << (unsigned)deltaSnapshot->length() << "\n";
        UDPMessageInterfaceUPtr deltaMsg;
        for(auto& playerPair : clients_) {
            if(playerPair.second.fullSnapshotTime < now - 13e3) {
                snapshot->updateClientId(playerPair.second.playerId); // clientId(std::string), playerId(uint32_t)/worldId/heroId ?
                snapshot->updateViewOnly(playerPair.second.viewOnly);
                snapshot->updateAvatar((uint8_t)playerPair.second.gameObjectType);
                snapshot->setClientConnection(playerPair.second.clientConnection);
                //debug("full snapshot length: %u", msg->length());
                udp.sendMessage2Client(msg);
                playerPair.second.fullSnapshotTime = now;
                //std::cout << "Full snapshot\n";
                //debug("sendSnapshot: Full");
            } else {
                //const UDPServerSnapshotUPtr& prevSnapshot = snapshots_.back();
                if(!deltaMsg) {
                    UDPMessageInterfaceUPtr& prevMsg = snapshots_.back();
                    //UDPServerDeltaSnapshotUPtr deltaSnapshot = std::make_unique<UDPServerDeltaSnapshot>(prevSnapshot, snapshot, now);
                    deltaMsg = UDPMessageFactory::createServerDeltaSnapshot(prevMsg, msg, now);
                }
                auto deltaSnapshot = deltaMsg->serverDeltaSnapshot();
                deltaSnapshot->payload(); // The serialized payload must be generated in order to update the clientId
                deltaSnapshot->updateClientId(playerPair.second.playerId);
                deltaSnapshot->setClientConnection(playerPair.second.clientConnection);
                //deltaSnapshot->setViewOnly(playerPair.second.viewOnly);
                //debug("delta snapshot length: %u", deltaMsg->length());
                udp.sendMessage2Client(deltaMsg);
                //std::cout << "Delta snapshot\n";
                //debug("sendSnapshot: Delta");
            }
        }
        storeSnapshot(msg);
    }

    bool isThereDeadGameObject() {
        return Application::game() &&
               Application::game()->worldList().size() &&
               Application::latestWorld()->multiplayer() &&
               Application::latestWorld()->multiplayer()->isAnyoneDead();
    }

    // This should be createTeam or createTeamInWorld
    // clientId
    // playerId == msgData.id == GameObject::id will come from snapshots
    /* the GameObjects should be created during the snapshot synchronisation,
     * but the server has to initialize the teams somehow
    void createPlayer(std::string clientId) {
        auto it = clients_.find(clientId);
        auto multiplayerWorld = Application::latestWorld()->multiplayer();
        if(!multiplayerWorld->isClientRegistered(it->second.gameObjectType, it->second.playerId)) {
            LOGI("DBG processClientMessages player does not exist");
            multiplayerWorld->registerClient(it->second.gameObjectType, it->second.playerId);
        }
    }
    */

    void execUserCommands(UDPUserCommands* userCommands) {
        Unused(0, userCommands);
        std::string clientId = userCommands->clientConnection()->sinAddrPort();
        //LOGI("execUserCommands: userCommands.clientId: %s", clientId.c_str());
        auto it = clients_.find(clientId);
        for(const auto& msg : userCommands->userInputs()) {
            //debug("execUserCommands DBG 1");
            // Let's ignore the messages from the inactive team and from the viewers
            if(it != clients_.end() && it->second.viewOnly) return;

            auto multiplayerWorld = Application::latestWorld()->multiplayer();
            if(msg.eventType == 'e') { // emote should be shown
                multiplayerWorld->onEmoteBubbleSelectedServer(msg.clientId, msg.emoteId, msg.avatar);
                continue;
            }

            auto teamId = Application::tileset()->activeTeamId();
            //debug("execUserCommands teamId: %u", teamId);
            if(it != clients_.end() && teamId < Application::tileset()->teamCount()) {
                if(teamId == 0 && it->second.gameObjectType != GameObjectInterface::Type::GOT_KNIGHT_RED) {
                    return;
                }
                if(teamId == 1 && it->second.gameObjectType != GameObjectInterface::Type::GOT_KNIGHT_DARK) {
                    return;
                }
                if(teamId == 2 && it->second.gameObjectType != GameObjectInterface::Type::GOT_KNIGHT_BLONDE_FEMALE) {
                    return;
                }
                if(teamId == 3 && it->second.gameObjectType != GameObjectInterface::Type::GOT_KNIGHT_GREEN_FEMALE) {
                    return;
                }
            }

#ifdef __SERVERAPP__
            // We must ignore the client commands until every opponent is joined
            //if((int)clientCount() < Application::tileset()->teamCount()) {
            if(!Application::playersJoined()) {
                debug("ServerApplication::clientCount(): %u, tileset->teamCount(): %d", clientCount(), Application::tileset()->teamCount());
                return;
            }
#endif
            //LOGI("execUserCommands: msg.clientId: %u", msg.clientId);
            debug("execUserCommands: msg.eventType: %c", msg.eventType);

            if(msg.eventType == 'b') { // Button event
                auto gameObjectType = static_cast<GameObject::Type>(msg.gameObjectType);
                auto buttonState = static_cast<ButtonEventListener::ButtonState>(msg.buttonState);
                std::string gameObjectTypeStr = GameObject::str(gameObjectType);
                std::string buttonStateStr = ButtonEventListener::str(buttonState);
                //LOGI("execUserCommands: gameObjectType: %s", gameObjectTypeStr.c_str());
                //LOGI("execUserCommands: buttonState: %s", buttonStateStr.c_str());
                GameObject* button{nullptr};
                switch(gameObjectType) {
                case GameObject::Type::GOT_ARROW_RIGHT: button = Application::game()->arrowButtonRight(); break;
                case GameObject::Type::GOT_ARROW_UP:    button = Application::game()->arrowButtonUp(); break;
                case GameObject::Type::GOT_ARROW_LEFT:  button = Application::game()->arrowButtonLeft(); break;
                case GameObject::Type::GOT_ARROW_DOWN:  button = Application::game()->arrowButtonDown(); break;
                case GameObject::Type::GOT_NEXT_TURN:   button = Application::game()->nextTurnButton(); break;
                case GameObject::Type::GOT_PASS:        button = Application::game()->nextTurnButton(); break;// the same button as next turn
                default:
                    LOGI("Error, unknow button");
                }
                //LOGI("execUserCommands: button: %p", button);
                multiplayerWorld->onButtonEventServer(msg.clientId, button, buttonState);
            } else if(msg.eventType == 'a') { // knight selection event
                multiplayerWorld->onActivateKnightServer(msg.clientId, msg.activeKnightId);
            } else if(msg.eventType == 't') { // click on tile
                multiplayerWorld->onTileClickedServer(msg.clientId, msg.tileX, msg.tileY);
            } else if(msg.eventType == 'D') { // click on decks
                multiplayerWorld->onDeckClickedServer(msg.clientId, msg.deckIndex);
            } else if(msg.eventType == 'c') { // click on card
                multiplayerWorld->onCardClickedServer(msg.clientId, msg.cardId);
            } else if(msg.eventType == 'h') { // hide all cards
                multiplayerWorld->onHideAllCardsServer(msg.clientId);
            } else if(msg.eventType == 'B') { // pull the clicked stone ball
                multiplayerWorld->onBallToPullClickedServer(msg.clientId, msg.ballId);
            } else {
                LOGI("DBG Illegal event type");
            }
        }
    }

    void sendLatestSnapshot(std::string clientId, double now) {
        UDP udp;
        UDPMessageInterfaceUPtr& prevMsg = snapshots_.back();
        auto serverSnapshot = prevMsg->serverSnapshot();
        serverSnapshot->payload(); // The serialized payload must be generated
        serverSnapshot->buildFrom(serverSnapshot->payload(), serverSnapshot->length(), nullptr);
        auto& client = clients_[clientId];
        serverSnapshot->updateClientId(client.playerId); // clientId(std::string), playerId(uint32_t)/worldId/heroId ?
        serverSnapshot->updateViewOnly(client.viewOnly);
        serverSnapshot->updateAvatar((uint8_t)client.gameObjectType);
        serverSnapshot->setClientConnection(client.clientConnection);
        //debug("full snapshot length: %u", msg->length());
        udp.sendMessage2Client(prevMsg);
        client.fullSnapshotTime = now;
    }

    //void answerConnectionRequest(UDPConnectionRequest* msg) {
    template<typename T>
    void answerConnectionRequest(T* msg) {
        auto clientId = registerClient(msg);
        if(snapshots_.size()) {
            sendLatestSnapshot(clientId, now);
        }
    }

    bool processClientMessages(double now) {
        //for(auto& playerPair : players_) {
        //    udpServer.readClientConnection(playerPair.second.clientConnection);
        //}
        while(udpMessageQueue_.size()) {
            UDPMessageInterfaceUPtr msg = udpMessageQueue_.pop();
            if(!msg) {
                // TODO: log an error or something
                debug("ERROR: processClientMessages: nullptr from UDP message queue");
                continue;
            }
            switch(msg->typeId()) {
            case UDPMessageInterface::TypeId::CONNECTION_REQUEST: {
                LOGI("DBG processClientMessages CONNECTION_REQUEST");
                if(msg->connectionRequest()) {
                    if(isRegistered(msg->connectionRequest())) {
                        // why did he send a new request?
                        // Maybe the player died
                    } else {
                        answerConnectionRequest(msg->connectionRequest());
                        return true;
                    }
                }
            }
            break;
            case UDPMessageInterface::TypeId::RECONNECTION_REQUEST:
                if(msg->reconnectionRequest()) {
                    if(updateConnectionData(msg->reconnectionRequest())) {
                        if(snapshots_.size()) {
                            auto clientId = msg->clientConnection()->sinAddrPort();
                            sendLatestSnapshot(clientId, now);
                        }
                    } else {
                        answerConnectionRequest(msg->reconnectionRequest());
                    }
                }
                break;
            case UDPMessageInterface::TypeId::SERVER_SNAPSHOT:
            case UDPMessageInterface::TypeId::SERVER_DELTA_SNAPSHOT:
                 /* client does not send snapshot, nothing to do */
            break;
            case UDPMessageInterface::TypeId::USER_COMMANDS: {
                //LOGI("DBG processClientMessages USER_COMMANDS");
                // we do not accept the view only clients' commands
                if(msg->userCommands()) {
                    if(!isRegistered(msg->userCommands())) {
                        // Why is he not registered?
                        // What TODO?
                        LOGI("DBG processClientMessages client is not registered");
                    } else {
                        execUserCommands(msg->userCommands());
                    }
                }
            }
            break;
            case UDPMessageInterface::TypeId::EXIT_GAME:
                if(msg->exitMessage()) {
                    std::string clientId = msg->clientConnection()->sinAddrPort();
                    LOGI("processClientMessages: exit game clientId: %s", clientId.c_str());
                    auto it = clients_.find(clientId);
                    if(it != clients_.end()) {
                        if(!Application::tileset()->gameOver() && !it->second.viewOnly && Application::playersJoined()) {
                            quitters.push_back((uint8_t)it->second.gameObjectType);
                            Application::tileset()->setQuitters(quitters);
                            Application::tileset()->resetPassCount();
                            // Your opponent might be the winner
                            if(playerCount() > 1) { // Ha az aktív játékos ez a játékos
                                auto teamId = Application::tileset()->activeTeamId();
                                if((teamId == 0 && it->second.gameObjectType == GameObjectInterface::Type::GOT_KNIGHT_RED) ||
                                (teamId == 1 && it->second.gameObjectType == GameObjectInterface::Type::GOT_KNIGHT_DARK) ||
                                (teamId == 2 && it->second.gameObjectType == GameObjectInterface::Type::GOT_KNIGHT_BLONDE_FEMALE) ||
                                (teamId == 3 && it->second.gameObjectType == GameObjectInterface::Type::GOT_KNIGHT_GREEN_FEMALE)) {
                                    Application::tileset()->nextTurn();
                                }
                            }
                            if(playerCount() == 1) {Application::tileset()->setLastTurn();}
                        }
                        if(udpServer_->removeClientConnection(it->second.clientConnection)) {
                            it = clients_.erase(it);
                            Application::setPlayerCount(playerCount());
                            debug("Client connection deleted, new size: %u", clients_.size());
                        }
                    }
                }
                break;
            case UDPMessageInterface::TypeId::LOBBY_REQUEST:
            case UDPMessageInterface::TypeId::LOBBY_DATA:
            case UDPMessageInterface::TypeId::ROOM_LIST_REQUEST:
            case UDPMessageInterface::TypeId::ROOM_LIST_ANSWER:
            case UDPMessageInterface::TypeId::LOBBY_REJECTED:
            case UDPMessageInterface::TypeId::LAST_ONE: break;
            case UDPMessageInterface::TypeId::KEEP_ALIVE:
                debug("processClientMessages: KEEP_ALIVE");
                break;
            }
        }
        return false;
    }

    void dropTheExpiredConnection() {
        bool nextTurnShouldBeCalled = false;
        auto teamId = Application::tileset()->activeTeamId();

        for(auto it = clients_.begin(); it != clients_.end();) {
            // Client sends keep_alive messages every 10th seconds, so we can kill the connection after 30 sec silence
            if(it->second.clientConnection->expired(3.e4)) {
                if(udpServer_->removeClientConnection(it->second.clientConnection)) {
                    // The application has to mark that which client is beeing deleted now
                    if(Application::playersJoined() && !it->second.viewOnly && !Application::tileset()->gameOver()) {
                        // these players' color should be saved and send with snapshot
                        quitters.push_back((uint8_t)it->second.gameObjectType);

                        if((teamId == 0 && it->second.gameObjectType == GameObjectInterface::Type::GOT_KNIGHT_RED) ||
                        (teamId == 1 && it->second.gameObjectType == GameObjectInterface::Type::GOT_KNIGHT_DARK) ||
                        (teamId == 2 && it->second.gameObjectType == GameObjectInterface::Type::GOT_KNIGHT_BLONDE_FEMALE) ||
                        (teamId == 3 && it->second.gameObjectType == GameObjectInterface::Type::GOT_KNIGHT_GREEN_FEMALE)) {
                            nextTurnShouldBeCalled = true;
                        }
                    }
                    it = clients_.erase(it);
                    debug("connection expired, player has been deleted");
                    Application::tileset()->resetPassCount();
                }
            } else {
                ++it;
            }
        }
        const size_t pC = playerCount();
        Application::setPlayerCount(pC);
        Application::setViewerCount(clients_.size() - pC);
        Application::tileset()->setQuitters(quitters);

        if(nextTurnShouldBeCalled && pC > 0) {
            //debug("ServerApplication::dropTheExpiredConnection quitters.size(): %u, teamId: %d", quitters.size(), (int)teamId);
            Application::tileset()->nextTurn();
        }

        if(Application::playersJoined() && pC == 1) {Application::tileset()->setLastTurn();}
    }

    size_t clientCount() {
        return clients_.size();
    }

    size_t playerCount() {
        return std::count_if(clients_.begin(), clients_.end(), [](auto& cInfo)->bool{return !cInfo.second.viewOnly;});
    }

    void RunServer() try {
        MOCK::logon();
        // TODO: Init the multiplayer world
        LOGI("RunServer OK"); // LoadBalance waiting for this
        setPlayerMode(Application::PlayerMode::MULTIPLAYER);
        if(Application::avatar() == Application::Avatar::UNDEFINED) {
            setAvatar(Application::Avatar::RED);
        }
        Application::setActiveMenu(MenuInterface::MenuType::MULTIPLAYER_HOST_RUNNING);
        if(Application::gameLevel() < LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP1) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_MULTIPLAYER_MAP1);
        }

        size_t lastSnapshotTime = 0;
        size_t lastUpdateTime = 0;
        double serverStartedAt = now_ms();

        while(true) {
            update_now();
            now = now_ms();
            if(Application::game() && Application::game()->worldList().size()) {
                processClientMessages(now);
            }
            if(lastUpdateTime + timeStep_ <= now) {
                updateWorld(now);
            }
            //drawScreen();
            if(lastSnapshotTime + snapshotStep_ <= now || isThereDeadGameObject()) {
                sendSnapshot(now);
                lastSnapshotTime = now;
            }
            if(lastUpdateTime + timeStep_ <= now) {
                drawWorld(&bitmapInfo, pixels.get());
                lastUpdateTime = now;
            }
            dropTheExpiredConnection();
            if((now - serverStartedAt > 20e3 && !playerCount()) ||
            (Application::deadLineOfRunning() > 1e-5 && Application::deadLineOfRunning() < now)) {
                Application::stop();
                udpServer_->stop();
                udpServer_->join();
                break;
            }
            usleep(iterationSleep_);
            // snapshotStep_
        }
        LOGI("RunServer stopped");
    } catch(...) {
        LOGI("RunServer exception, something went wrong!");
    }

    UDPMessageQueue* udpMessageQueue() {
        return &udpMessageQueue_;
    }
}
