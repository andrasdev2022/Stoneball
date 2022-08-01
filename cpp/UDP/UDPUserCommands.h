#ifndef __UDPUSERCOMMANDS_H__
#define __UDPUSERCOMMANDS_H__

#include "UDP.h"
#include "Serialize.h"
#include "ButtonEventListener.h"
#include <list>

class UDPUserCommands : public UDP::Message {
    UDPUserCommands() = default;
    friend class UDPMessageFactory;
public:
    struct MessageData {
        uint32_t clientId;
        unsigned char eventType; // ButtonEvent or active GameObject selection
        unsigned char buttonState; // ButtonEventListener::ButtonState
        union {
            uint8_t gameObjectType;
            uint8_t avatar;
            uint8_t deckIndex;
        };
        union { // these variables are not present in the message at the same time
            uint32_t activeKnightId;
            uint32_t cardId;
            uint32_t ballId;
            uint32_t emoteId;
        };
        unsigned char tileX;
        unsigned char tileY;
    };

    bool buildFrom(const unsigned char* payload, size_t length, const UDPConnectionInterfaceSPtr& clientConnection) noexcept override;
    TypeId typeId() const noexcept override {return TypeId::USER_COMMANDS;}
    const unsigned char* payload() noexcept override;

    bool addData(uint32_t clientId, unsigned char gameObjectType, ButtonEventListener::ButtonState buttonState) noexcept;
    bool addData(uint32_t clientId, uint32_t activeKnightId) noexcept;
    bool addData(uint32_t clientId, unsigned char tileX, unsigned char tileY) noexcept;
    bool addData(uint32_t clientId, uint8_t deckIndex) noexcept;
    bool addCardId(uint32_t clientId, uint32_t cardId) noexcept;
    bool hideAllCardsCommand(uint32_t clientId) noexcept;
    bool addBallData(uint32_t clientId, uint32_t ballId) noexcept;
    bool addEmoteId(uint32_t clientId, uint32_t emoteId, uint8_t avatar) noexcept;

    const std::list<MessageData>& userInputs() const noexcept {return userInputs_;}

    UDPUserCommands* userCommands() noexcept override {return this;}
private:
    uint32_t tick_{0};
    std::list<MessageData> userInputs_;
    Serialize serialize_;
};

using UDPUserCommandsUPtr = std::unique_ptr<UDPUserCommands>;

#endif // __UDPUSERCOMMANDS_H__
