#ifndef __UDPLOBBYREJECTED_H__
#define __UDPLOBBYREJECTED_H__

#include "UDPLobbyRequest.h"

class UDPLobbyRejected : public UDPLobbyRequest {
    using UDPLobbyRequest::UDPLobbyRequest;
    friend class UDPMessageFactory;
public:
    TypeId typeId() const noexcept override {return TypeId::LOBBY_REJECTED;}
    UDPLobbyRequest* lobbyRequest() noexcept override {return nullptr;}
    const UDPLobbyRejected* lobbyRejected() const noexcept override {return this;}
};

using UDPLobbyRejectedUPtr = std::unique_ptr<UDPLobbyRejected>;

#endif // __UDPLOBBYREJECTED_H__
