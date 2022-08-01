#include "UDPRoomListRequest.h"
#include "CommonUtil.h"
#include "BuildNumber.h"
#include "Memory.h"

bool UDPRoomListRequest::buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept {
    if(!payload) {
        debug("DBG RoomListRequest::buildFrom payload is null");
    }
    if(!length) {
        debug("DBG RoomListRequest::buildFrom !length");
    }
    if(*payload != static_cast<unsigned char>(typeId())) {
        debug("DBG RoomListRequest::buildFrom *payload != static_cast<unsigned char>(typeId())");
    }

    if(!payload || !length || *payload != static_cast<unsigned char>(typeId())) return false;
    // First character of the payload is the message type
    const unsigned char* ptr = payload + 1;
    ptr = Memory::copy(buildNumber_, ptr);
    ptr = Memory::copy(messageData_, ptr);

    clientConnection_ = clientConnection;
    return true;
}

const unsigned char* UDPRoomListRequest::payload() noexcept {
    if(payload_) return payload_.get();

    unsigned char messageType = static_cast<unsigned char>(typeId());
    serialize_.add(messageType);
    serialize_.add(BuildNumber::n());
    serialize_.addString(messageData_);

    length_ = serialize_.size();
    payload_ = serialize_.get();
    return payload_.get();

    return nullptr;
}
