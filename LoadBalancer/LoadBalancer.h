#ifndef __LOADBALANCER_H__
#define __LOADBALANCER_H__

#include "UDPServerInterface.h"
#include "UDPMessageQueueInterface.h"
#include <unordered_map>
#include "ProcessHandler.h"
#include <thread>
#include <vector>
#include <set>
#include <tuple>
#include "UDPRoomListAnswer.h"

class LoadBalancer {
public:
    LoadBalancer(std::string configFile, int serverPort);
    void start();
    ~LoadBalancer();

    struct ClientInfo {
        //int numPlayers; // actually the running process knows it
        std::wstring gameName; // unique generated string
        int maxPlayers;
        std::string host;
        uint16_t port;
        ProcessHandlerUPtr processHandler;
        int lobbiAddressesIndex;
        uint8_t selectedMapNr;
        uint8_t selectedMinutesNr;
        double registrationTime;
    };

    struct GameServerInfo {
        std::string host;
        uint16_t port;
        bool occupied;
    };

    struct SlaveInfo {
        std::string host;
        uint16_t port;
        UDPConnectionInterfaceSPtr connection;
    };

    struct RequestedGames {
        std::wstring gameName;
        uint8_t selectedMapNr;
        uint8_t selectedMinutesNr;
        UDPConnectionInterfaceSPtr connection;
        mutable uint8_t rejectionCount;
        double expirationTime;
        bool operator()(const RequestedGames& a, const RequestedGames& b) const {
            auto r = a.gameName.compare(b.gameName);
            if(r < 0) return true;
            if(r > 0) return false;
            if(a.selectedMapNr < b.selectedMapNr) return true;
            if(a.selectedMapNr > b.selectedMapNr) return false;
            if(a.selectedMinutesNr < b.selectedMinutesNr) return true;
            if(a.selectedMinutesNr > b.selectedMinutesNr) return false;
            return false;
        }
    };

    struct RequestedRoomLists {
        std::wstring messageData;
        UDPConnectionInterfaceSPtr connection;
        mutable uint8_t receivedAnswerCount;
        double expirationTime;
        mutable std::vector<UDPRoomListAnswer::MessageData> roomList;
        bool requestFromMaster;
        bool operator()(const RequestedRoomLists& a, const RequestedRoomLists& b) const {
            return a.messageData.compare(b.messageData);
        }

    };

    using waitingLRit = std::set<RequestedGames, RequestedGames>::iterator;
    using waitingLRit2 = std::set<RequestedRoomLists, RequestedRoomLists>::iterator;

    void handleMessages();
    void deleteStoppedProcesses() noexcept;
    void sendAnswerToExpiredRoomListRequests();
    void sendRejectionToExpiredGameRequests();

private:
    int occupyAddress() noexcept;
    void handleLobbyRequest(UDPMessageInterfaceUPtr& msg);
    bool handleLobbyRequest(UDPLobbyRequest* request, std::wstring gameName, UDPMessageInterfaceUPtr& answer, const UDPConnectionInterfaceSPtr& connection, const std::string& addrPort);
    void handleRoomListRequest(UDPMessageInterfaceUPtr& msg);
    std::wstring generateUniqueName() noexcept;
    void loadConfigFile(std::string configFile);
    void sendLobbyRequests2ASlave(const UDPLobbyRequest* msg, const std::string& addrPort);
    void forwardLobbyData2AClient(const UDPLobbyData* msg);
    bool incRejectionCount(waitingLRit clientIt);
    void forwardRejection2AClient(const UDPLobbyRejected* msg, waitingLRit clientIt);
    void handleLobbyRequestRejection(const UDPLobbyRejected* msg);
    void handleRoomListAnswer(const UDPRoomListAnswer* msg);
    void processSlaveMessages();
    void processSlaveMessages(UDPConnectionInterfaceSPtr& connection);
    std::tuple<std::wstring, std::string> parseGameName(std::wstring gameName);
    void sendRoomListRequest2AllSlaves(UDPRoomListRequest* msg, std::string addrPort);
    void mergeRoomListAnswer2List(const UDPRoomListAnswer* msg, LoadBalancer::waitingLRit2);
    waitingLRit findAWaitingLobbyRequestClient(const UDPLobbyRejected* msg);
    waitingLRit2 findAWaitingRoomListRequestClient(const UDPRoomListAnswer* msg);
    void forwardRoomListAnswer2TheClient(const UDPRoomListAnswer* msg, LoadBalancer::waitingLRit2 clientIt);

    UDPServerInterfaceUPtr udpServer_;
    UDPServerInterfaceUPtr udpSlaveServer_;
    UDPMessageQueueInterfaceUPtr udpMessageQueue_;
    UDPMessageQueueInterfaceUPtr udpSlaveMessageQueue_;
    std::unordered_map<std::string, ClientInfo> clients_; // room: these clients have created games and they are waiting for opponents
    std::unordered_map<std::string, std::string> config_;
    std::vector<GameServerInfo> gameServerAddresses_;
    std::vector<SlaveInfo> slaveLobbyAddresses_;
    std::set<RequestedGames, RequestedGames> waitingLobbyRequests_;
    std::set<RequestedRoomLists, RequestedRoomLists> waitingRoomListRequests_;

    int serverPort_{44444};
    bool requestFromMaster_{};
};

#endif // __LOADBALANCER_H__
