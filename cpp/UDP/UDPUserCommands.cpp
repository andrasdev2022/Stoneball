#include "UDPUserCommands.h"
#include "Global.h"
#include "Memory.h"

bool UDPUserCommands::buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept {
    if(!payload) {
        LOGI("DBG UDPUserCommands::buildFrom payload is null");
    }
    if(!length) {
        LOGI("DBG UDPUserCommands::buildFrom !length");
    }
    if(*payload != static_cast<unsigned char>(typeId())) {
        LOGI("DBG UDPUserCommands::buildFrom *payload != static_cast<unsigned char>(typeId())");
    }

    if(!payload || !length || *payload != static_cast<unsigned char>(typeId())) return false;

    // First character of the payload is the message type
    const unsigned char* ptr = payload + 1;
    uint16_t size{};
    ptr = Memory::copy(size, ptr);

    for(uint16_t i = 0; i < size; ++i) {
        MessageData msgData;
        ptr = Memory::copy(msgData.clientId, ptr);
        ptr = Memory::copy(msgData.eventType, ptr);
        ptr = Memory::copy(msgData.buttonState, ptr);
        ptr = Memory::copy(msgData.gameObjectType, ptr);
        ptr = Memory::copy(msgData.activeKnightId, ptr);
        ptr = Memory::copy(msgData.tileX, ptr);
        ptr = Memory::copy(msgData.tileY, ptr);
        userInputs_.push_back(msgData);
    }

    ptr = Memory::copy(tick_, ptr);

    clientConnection_ = clientConnection;
    return true;
}

const unsigned char* UDPUserCommands::payload() noexcept {
    if(payload_) return payload_.get();

    unsigned char messageType = static_cast<unsigned char>(typeId());
    serialize_.add(messageType);

    uint16_t size = userInputs_.size();
    serialize_.add(size);
    for(const auto& p : userInputs_) {
        serialize_.add(p.clientId);
        serialize_.add(p.eventType);
        serialize_.add(p.buttonState);
        serialize_.add(p.gameObjectType);
        serialize_.add(p.activeKnightId);
        serialize_.add(p.tileX);
        serialize_.add(p.tileY);
    }
    serialize_.add(++tick_);

    length_ = serialize_.size();
    payload_ = serialize_.get();
    return payload_.get();

    return nullptr;
}

bool UDPUserCommands::addData(uint32_t clientId, unsigned char gameObjectType, ButtonEventListener::ButtonState buttonState) noexcept {
    MessageData msg{};
    msg.clientId = clientId;
    msg.gameObjectType = gameObjectType;
    msg.buttonState = static_cast<unsigned char>(buttonState);
    msg.eventType = 'b';
    userInputs_.push_back(msg);
    return true;
}

bool UDPUserCommands::addData(uint32_t clientId, uint32_t activeKnightId) noexcept {
    MessageData msg{};
    msg.clientId = clientId;
    msg.eventType = 'a';
    msg.activeKnightId = activeKnightId;
    userInputs_.push_back(msg);
    return true;
}

bool UDPUserCommands::addData(uint32_t clientId, unsigned char tileX, unsigned char tileY) noexcept {
    MessageData msg{};
    msg.clientId = clientId;
    msg.eventType = 't';
    msg.tileX = tileX;
    msg.tileY = tileY;
    userInputs_.push_back(msg);
    return true;
}

bool UDPUserCommands::addData(uint32_t clientId, uint8_t deckIndex) noexcept {
    MessageData msg{};
    msg.clientId = clientId;
    msg.eventType = 'D';
    msg.deckIndex = deckIndex;
    userInputs_.push_back(msg);
    return true;
}

bool UDPUserCommands::addCardId(uint32_t clientId, uint32_t cardId) noexcept{
    MessageData msg{};
    msg.clientId = clientId;
    msg.eventType = 'c';
    msg.cardId = cardId;
    userInputs_.push_back(msg);
    return true;
}

bool UDPUserCommands::hideAllCardsCommand(uint32_t clientId) noexcept {
    MessageData msg{};
    msg.clientId = clientId;
    msg.eventType = 'h';
    userInputs_.push_back(msg);
    return true;
}

bool UDPUserCommands::addBallData(uint32_t clientId, uint32_t ballId) noexcept {
    MessageData msg{};
    msg.clientId = clientId;
    msg.eventType = 'B';
    msg.ballId = ballId;
    userInputs_.push_back(msg);
    return true;
}

bool UDPUserCommands::addEmoteId(uint32_t clientId, uint32_t emoteId, uint8_t avatar) noexcept {
    MessageData msg{};
    msg.clientId = clientId;
    msg.eventType = 'e';
    msg.emoteId = emoteId;
    msg.avatar = avatar;
    userInputs_.push_back(msg);
    return true;
}
