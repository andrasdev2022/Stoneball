#ifndef __UDPCONNECTIONREQUEST_H__
#define __UDPCONNECTIONREQUEST_H__

#include "UDP.h"
#include "GameObjectInterface.h"
#include "Serialize.h"

class UDPConnectionRequest : public UDP::Message {
    UDPConnectionRequest() noexcept;
    friend class UDPMessageFactory;
public:

    // UDP::Message
    bool buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept override;
    TypeId typeId() const noexcept override {return TypeId::CONNECTION_REQUEST;}
    UDPConnectionRequest* connectionRequest() noexcept override {return this;}
    const unsigned char* payload() noexcept override;

    const std::wstring& teamName() const noexcept {return teamName_;}

private:
    Serialize serialize_;
    std::wstring teamName_;
};

#endif // __UDPCONNECTIONREQUEST_H__
