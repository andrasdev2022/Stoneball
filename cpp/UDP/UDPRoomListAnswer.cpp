#include "UDPRoomListAnswer.h"
#include "Memory.h"
#include "CommonUtil.h"

bool UDPRoomListAnswer::buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept {
    if(!payload) {
        debug("DBG UDPRoomListAnswer::buildFrom payload is null");
    }
    if(!length) {
        debug("DBG UDPRoomListAnswer::buildFrom !length");
    }
    if(*payload != static_cast<unsigned char>(typeId())) {
        debug("DBG UDPRoomListAnswer::buildFrom *payload != static_cast<unsigned char>(typeId())");
    }

    if(!payload || !length || *payload != static_cast<unsigned char>(typeId())) return false;

    // First character of the payload is the message type
    const unsigned char* ptr = payload + 1;
    uint16_t size{};
    ptr = Memory::copy(size, ptr);

    MessageData msgData;
    for(uint16_t i = 0; i < size; ++i) {
        ptr = Memory::copy(msgData.clientNum, ptr);
        ptr = Memory::copy(msgData.roomName, ptr);
        ptr = Memory::copy(msgData.host, ptr);
        ptr = Memory::copy(msgData.port, ptr);
        ptr = Memory::copy(msgData.selectedMapNr, ptr);
        ptr = Memory::copy(msgData.selectedMinutesNr, ptr);
        roomList_.push_back(msgData);
    }
    ptr = Memory::copy(lobbyCommInfo_, ptr);

    clientConnection_ = clientConnection;
    return true;
}

const unsigned char* UDPRoomListAnswer::payload() noexcept {
    if(payload_) return payload_.get();

    unsigned char messageType = static_cast<unsigned char>(typeId());
    serialize_.add(messageType);

    uint16_t size = roomList_.size();
    serialize_.add(size);

    // client will display in ordered way
    roomList_.sort([](const MessageData& d1, const MessageData& d2){return d1.registrationTime > d2.registrationTime;});

    for(const auto& p : roomList_) {
        serialize_.add(p.clientNum);
        serialize_.addString(p.roomName);
        serialize_.addString(p.host);
        serialize_.add(p.port);
        serialize_.add(p.selectedMapNr);
        serialize_.add(p.selectedMinutesNr);
    }
    serialize_.addString(lobbyCommInfo_);

    length_ = serialize_.size();
    payload_ = serialize_.get();
    return payload_.get();

    return nullptr;
}

bool UDPRoomListAnswer::addData(MessageData msgData) noexcept {
    roomList_.push_back(msgData);
    return true;
}
