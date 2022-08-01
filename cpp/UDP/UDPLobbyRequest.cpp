#include "UDPLobbyRequest.h"
#include "Memory.h"
#include "CommonUtil.h"
#include "BuildNumber.h"

UDPLobbyRequest::UDPLobbyRequest(std::wstring name, uint8_t maxPlayers, uint8_t selectedMapNr, uint8_t selectedMinutesNr)
: maxPlayers_(maxPlayers)
, selectedMapNr_(selectedMapNr)
, selectedMinutesNr_(selectedMinutesNr)
, name_(name) {
}

bool UDPLobbyRequest::buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept {
    if(!payload) {
        debug("DBG UDPLobbyRequest::buildFrom payload is null");
    }
    if(!length) {
        debug("DBG UDPLobbyRequest::buildFrom !length");
    }
    if(*payload != static_cast<unsigned char>(typeId())) {
        debug("DBG UDPLobbyRequest::buildFrom *payload != static_cast<unsigned char>(typeId())");
    }

    if(!payload || !length || *payload != static_cast<unsigned char>(typeId())) return false;

    // First character of the payload is the message type
    const unsigned char* ptr = payload + 1;
    ptr = Memory::copy(maxPlayers_, ptr);
    ptr = Memory::copy(selectedMapNr_, ptr);
    ptr = Memory::copy(selectedMinutesNr_, ptr);
    ptr = Memory::copy(name_, ptr);
    ptr = Memory::copy(buildNumber_, ptr);
    clientConnection_ = clientConnection;
    return true;
}

const unsigned char* UDPLobbyRequest::payload() noexcept {
    if(payload_) return payload_.get();

    unsigned char messageType = static_cast<unsigned char>(typeId());
    serialize_.add(messageType);
    serialize_.add(maxPlayers_);
    serialize_.add(selectedMapNr_);
    serialize_.add(selectedMinutesNr_);
    serialize_.addString(name_);
    serialize_.add(BuildNumber::n());
    length_ = serialize_.size();
    payload_ = serialize_.get();
    return payload_.get();

    return nullptr;
}
