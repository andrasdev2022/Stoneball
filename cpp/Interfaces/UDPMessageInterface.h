#ifndef __UDPMESSAGEINTERFACE_H__
#define __UDPMESSAGEINTERFACE_H__

#include <memory>
#include "UDPConnectionInterface.h"

class UDPUserCommands;
class UDPServerSnapshot;
class UDPServerDeltaSnapshot;
class UDPConnectionRequest;
class UDPReconnectionRequest;
class UDPLobbyRequest;
class UDPLobbyData;
class UDPRoomListRequest;
class UDPRoomListAnswer;
class UDPExitMessage;
class UDPLobbyRejected;

class UDPMessageInterface {
public:
    enum class TypeId {
        CONNECTION_REQUEST,       // 0
        SERVER_SNAPSHOT,          // 1
        USER_COMMANDS,            // 2
        EXIT_GAME,                // 3
        SERVER_DELTA_SNAPSHOT,    // 4
        LOBBY_REQUEST,            // 5
        LOBBY_DATA,               // 6
        ROOM_LIST_REQUEST,        // 7
        ROOM_LIST_ANSWER,         // 8
        LOBBY_REJECTED,           // 9
        KEEP_ALIVE,               // 10
        RECONNECTION_REQUEST,     // 11
        LAST_ONE                  // 12
    };
    virtual ~UDPMessageInterface() = default;
    virtual size_t length() const noexcept = 0;
    virtual const unsigned char* payload() noexcept = 0;
    virtual bool buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) = 0;
    virtual TypeId typeId() const = 0;
    virtual const UDPConnectionInterfaceSPtr& clientConnection() const = 0;
    virtual void setClientConnection(const UDPConnectionInterfaceSPtr& clientConnection) = 0;
    virtual uint16_t buildNumber() const {return 0;}

    // 'dynamic_cast'
    virtual UDPUserCommands* userCommands() noexcept {return nullptr;}
    virtual UDPServerSnapshot* serverSnapshot() noexcept {return nullptr;}
    virtual UDPServerDeltaSnapshot* serverDeltaSnapshot() noexcept {return nullptr;}
    virtual UDPConnectionRequest* connectionRequest() noexcept {return nullptr;}
    virtual UDPReconnectionRequest* reconnectionRequest() noexcept {return nullptr;}
    virtual UDPLobbyRequest* lobbyRequest() noexcept {return nullptr;}
    virtual const UDPLobbyRejected* lobbyRejected() const noexcept {return nullptr;}
    virtual UDPLobbyData* lobbyData() noexcept {return nullptr;}
    virtual UDPRoomListAnswer* roomListAnswer() noexcept {return nullptr;}
    virtual UDPRoomListRequest* roomListRequest() noexcept {return nullptr;}
    virtual bool exitMessage() const noexcept {return false;}
    virtual bool keepAlive() const noexcept {return false;}
};

using UDPMessageInterfaceUPtr = std::unique_ptr<UDPMessageInterface>;

#endif // __UDPMESSAGEINTERFACE_H__
