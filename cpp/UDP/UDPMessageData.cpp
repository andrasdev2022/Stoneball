#include "UDPMessageData.h"
#include "Knight.h"
#include "RollingStone.h"
#include "RPGBgElement.h"
#include "Particle.h"
#include "Messagebox1.h"
#include "Card.h"
#include "Memory.h"
#include "Questionmark.h"
#include "MatchResults.h"
#include "EmoteFigure.h"

namespace {

    const uint8_t* buildFrom(UDPMessageData* data, const uint8_t* ptr) noexcept {
        switch(static_cast<GameObjectInterface::Type>(data->gameObjectType)) {
        case GameObject::Type::GOT_BLUE_TRIANGLE:
        case GameObject::Type::GOT_RED_BOUNCING_STONE:
        case GameObject::Type::GOT_GRAY_BOUNCING_STONE:
        case GameObject::Type::GOT_YELLOW_BOUNCING_STONE:
        case GameObject::Type::GOT_GREEN_BOUNCING_STONE:
        case GameObject::Type::GOT_LISTBOX:
        case GameObject::Type::GOT_DECK_OF_CARDS:
        case GameObject::Type::GOT_MESSAGEBOXYESNO:
        case GameObject::Type::GOT_FIREWORK_LONG_BLUE:
        case GameObject::Type::GOT_FIREWORK_LONG_ORANGE:
        case GameObject::Type::GOT_FIREWORK_LONG_GREEN:
        case GameObject::Type::GOT_TROPHY_NR1:
        case GameObject::Type::GOT_TROPHY_NR2:
        case GameObject::Type::GOT_TROPHY_NR3:
            ptr = BlueTriangle::buildFrom(data, ptr);
            break;
        case GameObject::Type::GOT_MESSAGEBOX:
            ptr = Messagebox1::buildFrom(data, ptr);
            break;
            break;
        case GameObject::Type::GOT_CARD_PULL:
        case GameObject::Type::GOT_CARD_PUSH_PLAYER:
        case GameObject::Type::GOT_CARD_TELEPORT:
            ptr = Card::buildFrom(data, ptr);
            break;
        case GameObject::Type::GOT_QUESTIONMARK:
            ptr = Questionmark::buildFrom(data, ptr);
            break;
        case GameObject::Type::GOT_MATCH_RESULTS:
            ptr = MatchResults::buildFrom(data, ptr);
            break;
        case GameObject::Type::GOT_EMOTE_FIGURE:
            ptr = EmoteFigure::buildFrom(data, ptr);
            break;
        default:
            debug("buildFrom illegal type");
        }

        return ptr;
    }
}

const uint8_t* UDPMessageData::buildFrom(const uint8_t* ptr) noexcept {
    ptr = GameObject::buildFrom(this, ptr);
    ptr = Memory::copy(type, ptr);
    switch (type) {
    case 'K': ptr = Knight::buildFrom(this, ptr); break;
    case 'S': ptr = RollingStone::buildFrom(this, ptr); break;
    case 'R': ptr = RPGBgElement::buildFrom(this, ptr); break;
    case 'P': ptr = Particle::buildFrom(this, ptr); break;
    case 'O': ptr = ::buildFrom(this, ptr); break;
    default:
        debug("buildFrom illegal character type: %c", (char)type);
        break;
    }
    return ptr;
}
