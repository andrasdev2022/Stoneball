#include "UDPMessageFactory.h"
#include "UDPConnectionRequest.h"
#include "UDPReconnectionRequest.h"
#include "UDPServerSnapshot.h"
#include "UDPServerDeltaSnapshot.h"
#include "UDPUserCommands.h"
#include "Application.h"
#include "UDPLobbyRequest.h"
#include "UDPLobbyRejected.h"
#include "UDPLobbyData.h"
#include "UDPRoomListAnswer.h"
#include "UDPSimpleMessage.h"
#include "UDPRoomListRequest.h"

UDPMessageInterfaceUPtr UDPMessageFactory::create(UDPMessageInterface::TypeId typeId) noexcept {
    UDPMessageInterfaceUPtr msg;
    switch(typeId) {
    case UDPMessageInterface::TypeId::CONNECTION_REQUEST:
        msg.reset(new UDPConnectionRequest());
       break;
    case UDPMessageInterface::TypeId::RECONNECTION_REQUEST:
        msg.reset(new UDPReconnectionRequest(0));
       break;
    case UDPMessageInterface::TypeId::SERVER_SNAPSHOT:
        msg.reset(new UDPServerSnapshot());
        break;
    case UDPMessageInterface::TypeId::USER_COMMANDS:
        msg.reset(new UDPUserCommands());
        break;
    case UDPMessageInterface::TypeId::EXIT_GAME:
    case UDPMessageInterface::TypeId::KEEP_ALIVE:
        msg.reset(new UDPSimpleMessage(typeId));
        break;
    case UDPMessageInterface::TypeId::SERVER_DELTA_SNAPSHOT:
        msg.reset(new UDPServerDeltaSnapshot());
        break;
    case UDPMessageInterface::TypeId::LOBBY_REQUEST:
        msg.reset(new UDPLobbyRequest(L"", 0, 0, 0));
        break;
    case UDPMessageInterface::TypeId::LOBBY_REJECTED:
        msg.reset(new UDPLobbyRejected(L"", 0, 0, 0));
        break;
    case UDPMessageInterface::TypeId::LOBBY_DATA:
        msg.reset(new UDPLobbyData(0, "", 0, L"", 0, 0));
        break;
    case UDPMessageInterface::TypeId::ROOM_LIST_REQUEST:
        msg.reset(new UDPRoomListRequest(L""));
        break;
    case UDPMessageInterface::TypeId::ROOM_LIST_ANSWER:
        msg.reset(new UDPRoomListAnswer());
        break;
    case UDPMessageInterface::TypeId::LAST_ONE:
        break;
    }
    return msg;
}

UDPMessageInterfaceUPtr UDPMessageFactory::createConnectionRequest() noexcept {
    return std::unique_ptr<UDPMessageInterface>(new UDPConnectionRequest());
}

UDPMessageInterfaceUPtr UDPMessageFactory::createReconnectionRequest(uint32_t clientId) noexcept {
    return std::unique_ptr<UDPMessageInterface>(new UDPReconnectionRequest(clientId));
}

UDPMessageInterfaceUPtr UDPMessageFactory::createServerDeltaSnapshot(const UDPMessageInterfaceUPtr& prevSnapshot, const UDPMessageInterfaceUPtr& snapshot, double now) {
    return std::unique_ptr<UDPMessageInterface>(new UDPServerDeltaSnapshot(prevSnapshot->serverSnapshot(), snapshot->serverSnapshot(), now));
}

UDPMessageInterfaceUPtr UDPMessageFactory::createLobbyRequest(std::wstring name, uint8_t maxPlayers, uint8_t selectedMapNr, uint8_t selectedMinutesNr) noexcept {
    return std::unique_ptr<UDPMessageInterface>(new UDPLobbyRequest(name, maxPlayers, selectedMapNr, selectedMinutesNr));
}

UDPMessageInterfaceUPtr UDPMessageFactory::createRoomListRequest(std::wstring messageData) noexcept {
    return std::unique_ptr<UDPMessageInterface>(new UDPRoomListRequest(messageData));
}

UDPMessageInterfaceUPtr UDPMessageFactory::createJoinRequest() noexcept {
    return std::unique_ptr<UDPMessageInterface>(new UDPConnectionRequest());
}

UDPMessageInterfaceUPtr UDPMessageFactory::createExitMessage() noexcept {
    return std::unique_ptr<UDPMessageInterface>(new UDPSimpleMessage(UDPMessageInterface::TypeId::EXIT_GAME));
}

UDPMessageInterfaceUPtr UDPMessageFactory::createLobbyRejected(std::wstring name, uint8_t maxPlayers, uint8_t selectedMapNr, uint8_t selectedMinutesNr) noexcept {
    return std::unique_ptr<UDPMessageInterface>(new UDPLobbyRejected(name, maxPlayers, selectedMapNr, selectedMinutesNr));
}

UDPMessageInterfaceUPtr UDPMessageFactory::createKeepAlive() noexcept {
    return std::unique_ptr<UDPMessageInterface>(new UDPSimpleMessage(UDPMessageInterface::TypeId::KEEP_ALIVE));
}

UDPMessageInterfaceUPtr UDPMessageFactory::createRoomListAnswer() noexcept {
    return std::unique_ptr<UDPMessageInterface>(new UDPRoomListAnswer());
}
