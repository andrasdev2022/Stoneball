#ifndef __UDPRECONNECTIONREQUEST_H__
#define __UDPRECONNECTIONREQUEST_H__

#include "UDP.h"
#include "GameObjectInterface.h"
#include "Serialize.h"

class UDPReconnectionRequest : public UDP::Message {
    UDPReconnectionRequest(uint32_t clientId) noexcept;
    friend class UDPMessageFactory;
public:

    // UDP::Message
    bool buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept override;
    TypeId typeId() const noexcept override {return TypeId::RECONNECTION_REQUEST;}
    UDPReconnectionRequest* reconnectionRequest() noexcept override {return this;}
    const unsigned char* payload() noexcept override;

    const std::wstring& teamName() const noexcept {return teamName_;}
    uint32_t clientId() const noexcept {return clientId_;}

private:
    Serialize serialize_;
    std::wstring teamName_;
    uint32_t clientId_{};
};

#endif // __UDPRECONNECTIONREQUEST_H__
