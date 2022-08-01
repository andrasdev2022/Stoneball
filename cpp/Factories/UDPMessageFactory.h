#ifndef __UDPMESSAGEFACTORY_H__
#define __UDPMESSAGEFACTORY_H__

#include "UDPMessageInterface.h"
#include "GameObjectInterface.h"

class UDPMessageFactory
{
public:
    static UDPMessageInterfaceUPtr create(UDPMessageInterface::TypeId typeId) noexcept;
    static UDPMessageInterfaceUPtr createConnectionRequest() noexcept;
    static UDPMessageInterfaceUPtr createReconnectionRequest(uint32_t clientId) noexcept;
    static UDPMessageInterfaceUPtr createServerDeltaSnapshot(const UDPMessageInterfaceUPtr& prevSnapshot,
                                                             const UDPMessageInterfaceUPtr& snapshot, double now);
    static UDPMessageInterfaceUPtr createLobbyRequest(std::wstring name, uint8_t maxPlayers, uint8_t selectedMapNr, uint8_t selectedMinutesNr) noexcept;
    static UDPMessageInterfaceUPtr createLobbyData(uint8_t maxPlayers, std::string host, uint16_t port, std::wstring name, uint8_t selectedMapNr, uint8_t selectedMinutesNr) noexcept;
    static UDPMessageInterfaceUPtr createRoomListRequest(std::wstring messageData) noexcept;
    static UDPMessageInterfaceUPtr createJoinRequest() noexcept;
    static UDPMessageInterfaceUPtr createExitMessage() noexcept;
    static UDPMessageInterfaceUPtr createLobbyRejected(std::wstring name, uint8_t maxPlayers, uint8_t selectedMapNr, uint8_t selectedMinutesNr) noexcept;
    static UDPMessageInterfaceUPtr createKeepAlive() noexcept;
    static UDPMessageInterfaceUPtr createRoomListAnswer() noexcept;
};

#endif // __UDPMESSAGEFACTORY_H__
