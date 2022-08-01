#ifndef __UDPLOBBYREQUEST_H__
#define __UDPLOBBYREQUEST_H__

#include "UDP.h"
#include "Serialize.h"
#include "GameObjectInterface.h"
#include <list>

class UDPLobbyRequest : public UDP::Message {
    friend class UDPMessageFactory;
    UDPLobbyRequest() = default;
public:
    bool buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept override;
    TypeId typeId() const noexcept override {return TypeId::LOBBY_REQUEST;}
    const unsigned char* payload() noexcept override;

    UDPLobbyRequest* lobbyRequest() noexcept override {return this;}
    uint8_t maxPlayers() const noexcept {return maxPlayers_;}
    uint8_t selectedMapNr() const noexcept {return selectedMapNr_;}
    uint8_t selectedMinutesNr() const noexcept {return selectedMinutesNr_;}
    std::wstring name() const noexcept {return name_;}
    uint16_t buildNumber() const override {return buildNumber_;}

protected:
    UDPLobbyRequest(std::wstring name, uint8_t maxPlayers, uint8_t selectedMapNr, uint8_t selectedMinutesNr);

private:
    Serialize serialize_;
    uint8_t maxPlayers_;
    uint8_t selectedMapNr_;
    uint8_t selectedMinutesNr_;
    std::wstring name_;
    uint16_t buildNumber_{};
};

using UDPLobbyRequestUPtr = std::unique_ptr<UDPLobbyRequest>;

#endif // __UDPLOBBYREQUEST_H__
