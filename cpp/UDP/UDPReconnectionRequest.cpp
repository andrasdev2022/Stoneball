#include "UDPReconnectionRequest.h"
#include "Memory.h"
#include "Application.h"

UDPReconnectionRequest::UDPReconnectionRequest(uint32_t clientId) noexcept
: clientId_(clientId) {

}

bool UDPReconnectionRequest::buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept {
    if(!payload || !length || *payload != static_cast<unsigned char>(typeId())) return false;

    const unsigned char* ptr = payload + 1;
    ptr = Memory::copy(teamName_, ptr);
    ptr = Memory::copy(clientId_, ptr);

    clientConnection_ = clientConnection;
    return true;
}

const unsigned char* UDPReconnectionRequest::payload() noexcept {
    if(payload_) return payload_.get();

    unsigned char messageType = static_cast<unsigned char>(typeId());
    serialize_.add(messageType);
    serialize_.addString(Application::profileName());
    serialize_.add(clientId_);

    length_ = serialize_.size();
    payload_ = serialize_.get();
    return payload_.get();
}
