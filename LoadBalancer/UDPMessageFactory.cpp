#include "UDPMessageFactory.h"
#include "UDPLobbyRequest.h"
#include "UDPLobbyRejected.h"
#include "UDPLobbyData.h"
#include "UDPSimpleMessage.h"
#include "UDPRoomListRequest.h"
#include "CommonUtil.h"
#include "UDPRoomListAnswer.h"

// UDPMessageQueue uses it
UDPMessageInterfaceUPtr UDPMessageFactory::create(UDPMessageInterface::TypeId typeId) noexcept {
    UDPMessageInterfaceUPtr msg;
    switch(typeId) {
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
    case UDPMessageInterface::TypeId::KEEP_ALIVE:
    case UDPMessageInterface::TypeId::EXIT_GAME: // it seems the client did not connect to any server, he left the lobby
        msg.reset(new UDPSimpleMessage(typeId));
        break;
    case UDPMessageInterface::TypeId::ROOM_LIST_ANSWER: debug("ROOM_LIST_ANSWER");
        msg.reset(new UDPRoomListAnswer());
        break;
    case UDPMessageInterface::TypeId::CONNECTION_REQUEST: debug("CONNECTION_REQUEST"); break;
    case UDPMessageInterface::TypeId::RECONNECTION_REQUEST: debug("RECONNECTION_REQUEST"); break;
    case UDPMessageInterface::TypeId::SERVER_SNAPSHOT: debug("SERVER_SNAPSHOT"); break;
    case UDPMessageInterface::TypeId::USER_COMMANDS: debug("USER_COMMANDS"); break;
    case UDPMessageInterface::TypeId::SERVER_DELTA_SNAPSHOT: debug("SERVER_DELTA_SNAPSHOT"); break;
    case UDPMessageInterface::TypeId::LAST_ONE: debug("LAST_ONE"); break;
    }
    return msg;
}

UDPMessageInterfaceUPtr UDPMessageFactory::createLobbyData(uint8_t maxPlayers, std::string host, uint16_t port, std::wstring name, uint8_t selectedMapNr, uint8_t selectedMinutesNr) noexcept {
    return std::unique_ptr<UDPMessageInterface>(new UDPLobbyData(maxPlayers, host, port, name, selectedMapNr, selectedMinutesNr));
}

UDPMessageInterfaceUPtr UDPMessageFactory::createLobbyRequest(std::wstring name, uint8_t maxPlayers, uint8_t selectedMapNr, uint8_t selectedMinutesNr) noexcept {
    return std::unique_ptr<UDPMessageInterface>(new UDPLobbyRequest(name, maxPlayers, selectedMapNr, selectedMinutesNr));
}

UDPMessageInterfaceUPtr UDPMessageFactory::createLobbyRejected(std::wstring name, uint8_t maxPlayers, uint8_t selectedMapNr, uint8_t selectedMinutesNr) noexcept {
    return std::unique_ptr<UDPMessageInterface>(new UDPLobbyRejected(name, maxPlayers, selectedMapNr, selectedMinutesNr));
}

UDPMessageInterfaceUPtr UDPMessageFactory::createRoomListRequest(std::wstring messageData) noexcept {
    return std::unique_ptr<UDPMessageInterface>(new UDPRoomListRequest(messageData));
}

UDPMessageInterfaceUPtr UDPMessageFactory::createRoomListAnswer() noexcept {
    return std::unique_ptr<UDPMessageInterface>(new UDPRoomListAnswer());
}
