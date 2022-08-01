#include "UDPSimpleMessage.h"
#include "CommonUtil.h"
#include "BuildNumber.h"
#include "Memory.h"

bool UDPSimpleMessage::buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept {
    if(!payload) {
        debug("DBG UDPSimpleMessage::buildFrom payload is null");
    }
    if(!length) {
        debug("DBG UDPSimpleMessage::buildFrom !length");
    }
    if(*payload != static_cast<unsigned char>(typeId())) {
        debug("DBG UDPSimpleMessage::buildFrom *payload != static_cast<unsigned char>(typeId())");
    }

    if(!payload || !length || *payload != static_cast<unsigned char>(typeId())) return false;
    // First character of the payload is the message type
    const unsigned char* ptr = payload + 1;
    ptr = Memory::copy(buildNumber_, ptr);
    clientConnection_ = clientConnection;
    return true;
}

const unsigned char* UDPSimpleMessage::payload() noexcept {
    if(payload_) return payload_.get();

    unsigned char messageType = static_cast<unsigned char>(typeId());
    serialize_.add(messageType);
    serialize_.add(BuildNumber::n());

    length_ = serialize_.size();
    payload_ = serialize_.get();
    return payload_.get();

    return nullptr;
}
