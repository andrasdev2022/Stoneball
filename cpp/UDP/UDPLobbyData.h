#ifndef __UDPLOBBYDATA_H__
#define __UDPLOBBYDATA_H__

#include "UDP.h"
#include "Serialize.h"
#include "ButtonEventListener.h"
#include <list>

class UDPLobbyData : public UDP::Message {
    UDPLobbyData(uint8_t maxPlayers, std::string host, uint16_t port, std::wstring name, uint8_t selectedMapNr, uint8_t selectedMinutesNr);
    friend class UDPMessageFactory;
public:
    bool buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept override;
    TypeId typeId() const noexcept override {return TypeId::LOBBY_DATA;}
    const unsigned char* payload() noexcept override;

    UDPLobbyData* lobbyData() noexcept override {return this;}
    uint8_t maxPlayers() const noexcept {return maxPlayers_;}
    std::string host() const noexcept {return host_;}
    uint16_t port() const noexcept {return port_;}
    uint8_t selectedMapNr() const noexcept {return selectedMapNr_;}
    uint8_t selectedMinutesNr() const noexcept {return selectedMinutesNr_;}
    std::wstring name() const noexcept {return name_;}

private:
    Serialize serialize_;
    uint8_t maxPlayers_; // TODO: remove?
    std::string host_;
    uint16_t port_;
    uint8_t selectedMapNr_;
    uint8_t selectedMinutesNr_;
    std::wstring name_;
};

using UDPLobbyDataUPtr = std::unique_ptr<UDPLobbyData>;

#endif // __UDPLOBBYDATA_H__
