#include "MultiplayerCommon.h"
#include "GameObject.h"
#include "MultiplayerWorld.h"
#include "Card.h"
#include "EmoteFigure.h"
#include "MediaData.h"
#include "Algorithms.h"

template<typename T>
MultiplayerCommon<T>::MultiplayerCommon()
: T()
{

}

template<typename T>
void MultiplayerCommon<T>::recalcEveryXY(MultiplayerWorld* world) noexcept {
    recalcKnightsXY(world);
    recalcRollingStonesXY(world);
    recalcHolesXY(world);
    recalcOtherObjectsXY(world);
}

template<typename T>
void MultiplayerCommon<T>::recalcKnightX(GameObject* knight) noexcept {
    double x = ZRDI(knight->x());
    double col = x / 32.;
    double newX = this->position_.x + col * this->tileSize_.width + this->knightV_.x;
    knight->setX(newX);
}

template<typename T>
void MultiplayerCommon<T>::recalcKnightY(GameObject* knight) noexcept {
    double y = ZRDI(knight->y());
    double row = (y + 10.) / 24.;
    double newY = this->position_.y + row * this->tileSize_.height + this->knightV_.y;
    knight->setY(newY);
}

template<typename T>
void MultiplayerCommon<T>::recalcKnightsXY(MultiplayerWorld* world) noexcept {
    auto& knights = world->knightList();
    for(auto& knightData : knights) {
        auto& knight = knightData.second;
        // based on Tilesethis->createKnight
        // position_.x + col * tileSize_.width + knightV_.x
        // position_.y + row * tileSize_.height + knightV_.y
        // server knightV_.x = 0, knightV_.y = -10
        // server tileSize_.width = 32, tileSize_.height = 24
        // server position.x, position.y = 0
        recalcKnightX(knight.get());
        recalcKnightY(knight.get());
    }
}

template<typename T>
void MultiplayerCommon<T>::recalcRollingStoneX(GameObject* stone) noexcept {
    double x = ZRDI(stone->x());
    double col = x / 32.;
    double newX = this->position_.x + col * this->tileSize_.width + this->stoneV_.x;
    stone->setX(newX);
}

template<typename T>
void MultiplayerCommon<T>::recalcRollingStoneY(GameObject* stone) noexcept {
    double y = ZRDI(stone->y());
    double row = (y + 9.) / 24.;
    double newY = this->position_.y + row * this->tileSize_.height + this->stoneV_.y;
    stone->setY(newY);
}

template<typename T>
void MultiplayerCommon<T>::recalcRollingStonesXY(MultiplayerWorld* world) noexcept {
    auto& stones = world->rollingStoneList();
    for(auto& stoneData : stones) {
        auto& stone = stoneData.second;

        // based on Tilesethis->createRollingStone
        // position_.x + col * tileSize_.width + stoneV_.x
        // position_.y + row * tileSize_.height + stoneV_.y
        // server stoneV_.x = 0, stoneV_.y = -9
        // server tileSize_.width = 32, tileSize_.height = 24
        // server position.x, position.y = 0
        recalcRollingStoneX(stone.get());
        recalcRollingStoneY(stone.get());
    }
}

template<typename T>
void MultiplayerCommon<T>::recalcHoleX(GameObject* hole) noexcept {
    double x = ZRDI(hole->x());
    double col = x / 32.;
    double newX = this->position_.x + col * this->tileSize_.width;
    hole->setX(newX);
}

template<typename T>
void MultiplayerCommon<T>::recalcHoleY(GameObject* hole) noexcept {
    double y = ZRDI(hole->y());
    double row = (y - 10.) / 24.;
    double newY = this->position_.y + row * this->tileSize_.height + ZRD(10.);
    hole->setY(newY);
}

template<typename T>
void MultiplayerCommon<T>::recalcHolesXY(MultiplayerWorld* world) noexcept {
    auto& holes = world->rpgBgElementList();
    for(auto& holeData : holes) {
        auto& hole = holeData.second;
        // based on Tilesethis->createHole
        // position_.x + col * tileSize_.width
        // position_.y + row * tileSize_.height + 10_ZRD
        // server tileSize_.width = 32, tileSize_.height = 24
        // server position.x, position.y = 0
        recalcHoleX(hole.get());
        recalcHoleY(hole.get());
    }
}

template<typename T>
void MultiplayerCommon<T>::recalcOtherOX(GameObject* obj) noexcept {
    //LOGI("MultiplayerCommon<T>::recalcOtherOX");
    switch(obj->skinMin()) {
    case NATIVE_BITMAP_BLUE_TRIANGLE_1: break;
    case NATIVE_BITMAP_RED_BOUNCING_STONE_1:
        this->setBouncingStoneId(obj->id());
        obj->setX(this->textXPos(0) - 27_ZRD);
        //LOGI("MultiplayerCommon<T>::recalcOtherOX NATIVE_BITMAP_RED_BOUNCING_STONE_1 x: %d %f", this->textXPos(), obj->x());
        break;
    case NATIVE_BITMAP_GRAY_BOUNCING_STONE_1:
        //LOGI("MultiplayerCommon<T>::recalcOtherOX NATIVE_BITMAP_GRAY_BOUNCING_STONE_1");
        this->setBouncingStoneId(obj->id());
        obj->setX(this->textXPos(1) - 27_ZRD);
        break;
    case NATIVE_BITMAP_YELLOW_BOUNCING_STONE_1:
        //LOGI("MultiplayerCommon<T>::recalcOtherOX NATIVE_BITMAP_GRAY_BOUNCING_STONE_1");
        this->setBouncingStoneId(obj->id());
        obj->setX(this->textXPos(2) - 27_ZRD);
        break;
    case NATIVE_BITMAP_GREEN_BOUNCING_STONE_1:
        //LOGI("MultiplayerCommon<T>::recalcOtherOX NATIVE_BITMAP_GRAY_BOUNCING_STONE_1");
        this->setBouncingStoneId(obj->id());
        obj->setX(this->textXPos(3) - 27_ZRD);
        break;
    case NATIVE_BITMAP_MESSAGEBOX1:
        obj->setX(screenPosX() + (screenWidth() - bitmapWidth(NATIVE_BITMAP_MESSAGEBOX1)) / 2);
        break;
    case NATIVE_BITMAP_DECK_OF_CARDS:
        if(this->deckId(0) == 0xffffffff) {
            this->setDeckId(0, obj->id());
        } else if(this->deckId(0) != obj->id() && this->deckId(1) == 0xffffffff) {
            this->setDeckId(1, obj->id());
        } else if(this->teamCount() > 2 && this->deckId(1) != obj->id() && this->deckId(2) == 0xffffffff) {
            this->setDeckId(2, obj->id());
        } else if(this->teamCount() > 3 && this->deckId(2) != obj->id() && this->deckId(3) == 0xffffffff) {
            this->setDeckId(3, obj->id());
        }
        if(obj->id() == this->deckId(0)) {
            obj->setX(this->textXPos(0) + 30_ZRD);
        } else if(obj->id() == this->deckId(1)) {
            obj->setX(this->textXPos(1) + 30_ZRD);
        } else if(this->teamCount() > 2 && obj->id() == this->deckId(2)) {
            obj->setX(this->textXPos(2) + 30_ZRD);
        } else if(this->teamCount() > 3 && obj->id() == this->deckId(3)) {
            obj->setX(this->textXPos(3) + 30_ZRD);
        }
        break;
    case NATIVE_BITMAP_CARD_PULL:
    case NATIVE_BITMAP_CARD_PULL_ACTIVE:
    case NATIVE_BITMAP_CARD_PULL_DISABLED:
    case NATIVE_BITMAP_CARD_PUSH_PLAYER:
    case NATIVE_BITMAP_CARD_PUSH_PLAYER_ACTIVE:
    case NATIVE_BITMAP_CARD_PUSH_PLAYER_DISABLED:
    case NATIVE_BITMAP_CARD_TELEPORT:
    case NATIVE_BITMAP_CARD_TELEPORT_ACTIVE:
    case NATIVE_BITMAP_CARD_TELEPORT_DISABLED: {
            auto card = obj->blueTriangle()->card();
            //debug("NATIVE_BITMAP_CARD_PULL, teamIndex: %u, id: %u", card->teamIndex(), card->id());
            if(card->teamIndex() == 0 && firstRedCardId_ == 0xffffffff) {
                firstRedCardId_ = card->id();
            } else if(card->teamIndex() == 1 && firstGrayCardId_ == 0xffffffff) {
                firstGrayCardId_ = card->id();
            } else if(card->teamIndex() == 2 && firstYellowCardId_ == 0xffffffff) {
                firstYellowCardId_ = card->id();
            } else if(card->teamIndex() == 3 && firstGreenCardId_ == 0xffffffff) {
                firstGreenCardId_ = card->id();
            }
            if(card->teamIndex() == 0) {
                card->setX(this->textXPos(0) - 26_ZRD + 56_ZRD * (card->id() - firstRedCardId_));
            } else if(card->teamIndex() == 1) {
                card->setX(this->textXPos(1) - 26_ZRD + 56_ZRD * (card->id() - firstGrayCardId_));
            } else if(card->teamIndex() == 2) {
                card->setX(this->textXPos(2) - 26_ZRD + 56_ZRD * (card->id() - firstYellowCardId_));
            } else if(card->teamIndex() == 3) {
                card->setX(this->textXPos(3) - 26_ZRD + 56_ZRD * (card->id() - firstGreenCardId_));
            }
        }
        break;
    case NATIVE_BITMAP_EMOTE_200IQ:
    case NATIVE_BITMAP_EMOTE_CRY:
    case NATIVE_BITMAP_EMOTE_GG:
    case NATIVE_BITMAP_EMOTE_GUN:
    case NATIVE_BITMAP_EMOTE_HI:
    case NATIVE_BITMAP_EMOTE_LOST: {
            auto emoteFigure = obj->blueTriangle()->emoteFigure();
            if(emoteFigure->avatar() != 255) {
                emoteFigure->setX(this->textXPos(emoteFigure->avatar()));
            }
        }
        break;
    }
}

template<typename T>
void MultiplayerCommon<T>::recalcOtherOY(GameObject* obj) noexcept {
    switch(obj->skinMin()) {
    case NATIVE_BITMAP_BLUE_TRIANGLE_1: break;
    case NATIVE_BITMAP_RED_BOUNCING_STONE_1:
        obj->setY(this->textYPos() + this->textHeight() - 25_ZRD);
        //LOGI("MultiplayerCommon<T>::recalcOtherOY NATIVE_BITMAP_RED_BOUNCING_STONE_1 y: %d %d %f", this->textYPos(), this->textHeight(), obj->y());
        break;
    case NATIVE_BITMAP_GRAY_BOUNCING_STONE_1:
        obj->setY(this->textYPos() + this->textHeight() - 25_ZRD);
        break;
    case NATIVE_BITMAP_YELLOW_BOUNCING_STONE_1:
        obj->setY(this->textYPos() + this->textHeight() - 25_ZRD);
        break;
    case NATIVE_BITMAP_GREEN_BOUNCING_STONE_1:
        obj->setY(this->textYPos() + this->textHeight() - 25_ZRD);
        break;
    case NATIVE_BITMAP_MESSAGEBOX1:
        obj->setY(screenPosY() + (screenHeight() - bitmapHeight(NATIVE_BITMAP_MESSAGEBOX1)) / 2);
        break;
    case NATIVE_BITMAP_DECK_OF_CARDS:
        if(obj->id() == this->deckId(0) || obj->id() == this->deckId(1) || obj->id() == this->deckId(2) || obj->id() == this->deckId(3)) {
            obj->setY(this->textYPos() + this->textHeight());
        }
        break;
    case NATIVE_BITMAP_CARD_PULL:
    case NATIVE_BITMAP_CARD_PULL_ACTIVE:
    case NATIVE_BITMAP_CARD_PULL_DISABLED:
    case NATIVE_BITMAP_CARD_PUSH_PLAYER:
    case NATIVE_BITMAP_CARD_PUSH_PLAYER_ACTIVE:
    case NATIVE_BITMAP_CARD_PUSH_PLAYER_DISABLED:
    case NATIVE_BITMAP_CARD_TELEPORT:
    case NATIVE_BITMAP_CARD_TELEPORT_ACTIVE:
    case NATIVE_BITMAP_CARD_TELEPORT_DISABLED: {
            auto card = obj->blueTriangle()->card();
            if(card->teamIndex() == 0) {
                card->setY(this->textYPos() + this->textHeight() + 10_ZRD);
            } else if(card->teamIndex() == 1) {
                card->setY(this->textYPos() + this->textHeight() + 10_ZRD);
            } else if(card->teamIndex() == 2) {
                card->setY(this->textYPos() + this->textHeight() + 10_ZRD);
            } else if(card->teamIndex() == 3) {
                card->setY(this->textYPos() + this->textHeight() + 10_ZRD);
            }
        }
        break;
    case NATIVE_BITMAP_EMOTE_200IQ:
    case NATIVE_BITMAP_EMOTE_CRY:
    case NATIVE_BITMAP_EMOTE_GG:
    case NATIVE_BITMAP_EMOTE_GUN:
    case NATIVE_BITMAP_EMOTE_HI:
    case NATIVE_BITMAP_EMOTE_LOST: {
            obj->setY(this->textYPos() + 25_ZRD);
        }
        break;
    }
}

template<typename T>
void MultiplayerCommon<T>::recalcOtherObjectsXY(MultiplayerWorld* world) noexcept {
    //LOGI("MultiplayerCommon<T>::recalcOtherObjectsXY");
    auto& elements = world->otherElementList();
    //LOGI("MultiplayerCommon<T>::recalcOtherObjectsXY elements.size(): %lu", elements.size());
    for(auto& elementData : elements) {
        auto& element = elementData.second;
        // calc of bouncin stone in PlayWithFriend1::updateTeamMarker
        // red x: textXPos_ - 27_ZRD;              y: textYPos_ + textHeight_ + 25_ZRD;
        // gray x: textXPos_ + textWidth_ - 5_ZRD; y: textYPos_ + textHeight_ + 25_ZRD;
        recalcOtherOX(element.get());
        recalcOtherOY(element.get());
    }
}

template<typename T>
bool MultiplayerCommon<T>::onGameObjectChange(GameObject* gameObject, UDPMessageData::Fields field) noexcept {
    using namespace Algorithms;

    //LOGI("Multiplayer5::onGameObjectChange: %d", gameObject->type());
    if(noneOf(field, UDPMessageData::Fields::Field_x, UDPMessageData::Fields::Field_contentBottom)) {
        return false;
    }

    if(noneOf(gameObject->type(), GameObject::Type::GOT_KNIGHT_RED, GameObject::Type::GOT_KNIGHT_DARK,
              GameObject::Type::GOT_KNIGHT_BLONDE_FEMALE, GameObject::Type::GOT_KNIGHT_GREEN_FEMALE,
              GameObject::Type::GOT_RPGBGELEMENT, GameObject::Type::GOT_BLUE_TRIANGLE, GameObject::Type::GOT_ROLLINGSTONE,
              GameObject::Type::GOT_RED_BOUNCING_STONE, GameObject::Type::GOT_GRAY_BOUNCING_STONE, GameObject::Type::GOT_MESSAGEBOX,
              GameObject::Type::GOT_LISTBOX, GameObject::Type::GOT_YELLOW_BOUNCING_STONE, GameObject::Type::GOT_GREEN_BOUNCING_STONE,
              GameObject::Type::GOT_DECK_OF_CARDS, GameObject::Type::GOT_CARD_PULL, GameObject::Type::GOT_CARD_PUSH_PLAYER,
              GameObject::Type::GOT_CARD_TELEPORT, GameObject::Type::GOT_EMOTE_FIGURE)) {
        return false;
    }

    switch(gameObject->type()) {
    case GameObject::Type::GOT_KNIGHT_RED:
    case GameObject::Type::GOT_KNIGHT_BLONDE_FEMALE:
    case GameObject::Type::GOT_KNIGHT_DARK:
    case GameObject::Type::GOT_KNIGHT_GREEN_FEMALE: {
        switch(field) {
        case UDPMessageData::Fields::Field_x:
            recalcKnightX(gameObject);
            return true;
            break;
        case UDPMessageData::Fields::Field_contentBottom:
            recalcKnightY(gameObject);
            return true;
            break;
        default:
            break;
        }
    }
    break;
    case GameObject::Type::GOT_RPGBGELEMENT: {
        switch(field) {
        case UDPMessageData::Fields::Field_x:
            recalcHoleX(gameObject);
            return true;
            break;
        case UDPMessageData::Fields::Field_contentBottom:
            recalcHoleY(gameObject);
            return true;
            break;
        default:
            break;
        }
    }
    break;
    case GameObject::Type::GOT_EMOTE_FIGURE:
    case GameObject::Type::GOT_BLUE_TRIANGLE:
    case GameObject::Type::GOT_RED_BOUNCING_STONE:
    case GameObject::Type::GOT_YELLOW_BOUNCING_STONE:
    case GameObject::Type::GOT_GRAY_BOUNCING_STONE:
    case GameObject::Type::GOT_GREEN_BOUNCING_STONE:
    case GameObject::Type::GOT_DECK_OF_CARDS:
    case GameObject::Type::GOT_CARD_PULL:
    case GameObject::Type::GOT_CARD_PUSH_PLAYER:
    case GameObject::Type::GOT_CARD_TELEPORT: {
        switch(field) {
        case UDPMessageData::Fields::Field_x:
            recalcOtherOX(gameObject);
            return true;
            break;
        case UDPMessageData::Fields::Field_contentBottom:
            recalcOtherOY(gameObject);
            return true;
            break;
        default:
            break;
        }
    }
    break;
    case GameObject::Type::GOT_MESSAGEBOX: {
        switch(field) {
        case UDPMessageData::Fields::Field_x:
            gameObject->setX(this->position_.x + (this->widthInPixels() - bitmapWidth(NATIVE_BITMAP_MESSAGEBOX1)) / 2);
            return true;
            break;
        case UDPMessageData::Fields::Field_contentBottom:
            gameObject->setY(this->position_.y + 7 * this->tileHeight());
            return true;
            break;
        default:
            break;
        }
    }
    break;
    case GameObject::Type::GOT_ROLLINGSTONE: {
        switch(field) {
        case UDPMessageData::Fields::Field_x:
            recalcRollingStoneX(gameObject);
            return true;
            break;
        case UDPMessageData::Fields::Field_contentBottom:
            recalcRollingStoneY(gameObject);
            return true;
            break;
        default:
            break;
        }
    }
    break;
    default: break;
    }
    return false;
}
