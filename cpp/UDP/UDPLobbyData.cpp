#include "UDPLobbyData.h"
#include "Memory.h"
#include "CommonUtil.h"

UDPLobbyData::UDPLobbyData(uint8_t maxPlayers, std::string host, uint16_t port, std::wstring name, uint8_t selectedMapNr, uint8_t selectedMinutesNr)
: maxPlayers_(maxPlayers)
, host_(host)
, port_(port)
, selectedMapNr_(selectedMapNr)
, selectedMinutesNr_(selectedMinutesNr)
, name_(name) {
}

bool UDPLobbyData::buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept {
    if(!payload) {
        debug("DBG UDPLobbyData::buildFrom payload is null");
    }
    if(!length) {
        debug("DBG UDPLobbyData::buildFrom !length");
    }
    if(*payload != static_cast<unsigned char>(typeId())) {
        debug("DBG UDPLobbyData::buildFrom *payload != static_cast<unsigned char>(typeId())");
    }

    if(!payload || !length || *payload != static_cast<unsigned char>(typeId())) return false;

    // First character of the payload is the message type
    const unsigned char* ptr = payload + 1;
    ptr = Memory::copy(maxPlayers_, ptr);
    ptr = Memory::copy(host_, ptr);
    ptr = Memory::copy(port_, ptr);
    ptr = Memory::copy(selectedMapNr_, ptr);
    ptr = Memory::copy(selectedMinutesNr_, ptr);
    ptr = Memory::copy(name_, ptr);
    clientConnection_ = clientConnection;
    return true;
}

const unsigned char* UDPLobbyData::payload() noexcept {
    if(payload_) return payload_.get();

    unsigned char messageType = static_cast<unsigned char>(typeId());
    serialize_.add(messageType);
    serialize_.add(maxPlayers_);
    uint16_t length = (uint16_t)host_.length();
    serialize_.add(length);
    serialize_.add3(host_.c_str(), host_.length());
    serialize_.add(port_);
    serialize_.add(selectedMapNr_);
    serialize_.add(selectedMinutesNr_);
    serialize_.addString(name_);

    length_ = serialize_.size();
    payload_ = serialize_.get();
    return payload_.get();
}
