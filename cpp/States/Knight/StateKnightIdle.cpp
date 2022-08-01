#include "StateKnightIdle.h"
#include "StateKnightRun.h"
#include "stoneball.h"
#include "StateSubject.h"
#include "GameObjectDescriptor.h"
#include "Knight.h"
#include "Application.h"
#include "Tileset.h"
#include "Level1.h"

#include <math.h>

StateKnightIdle::StateKnightIdle(double x, double bottomY, GameObject::Type heroType, [[maybe_unused]]double directionX) noexcept
 : GameObject(x, bottomY, heroType) {
    direction_ = Vector<double>(1, 0); // we won't go anywhere with this state
    init();
    firstMoveX = -1.;
    firstMoveY = -1.;
    action = -1;
    buttonActionType = GameObject::Type::COUNT;
    //LOGI("DBG NEW KNIGHT STATE: IDLE");

}

State* StateKnightIdle::copy() const noexcept {
    return new StateKnightIdle(*this);
}

void StateKnightIdle::change(StateSubject* subject) noexcept {
    //LOGI("DBG StateKnightIdle::change");
    if(buttonActionType == GameObject::Type::GOT_ARROW_UP) {
        subject->setState(new StateKnightRun(x_, contentBottom(), type_, Direction::NORTH));
    } else if(buttonActionType == GameObject::Type::GOT_ARROW_DOWN) {
        subject->setState(new StateKnightRun(x_, contentBottom(), type_, Direction::SOUTH));
    } else if(buttonActionType == GameObject::Type::GOT_ARROW_RIGHT) {
        subject->setState(new StateKnightRun(x_, contentBottom(), type_, Direction::EAST));
    } else if(buttonActionType == GameObject::Type::GOT_ARROW_LEFT) {
        subject->setState(new StateKnightRun(x_, contentBottom(), type_, Direction::WEST));
    }
}

void StateKnightIdle::update(double now) noexcept {
    updateFrame(now, skinMin_, skinMax_);
}

const GameObjectDescriptor& StateKnightIdle::god() const noexcept {
    return GameObjectDescriptor::find(type_, State::Type::ST_IDLE);
}

// ButtonEventListener
bool StateKnightIdle::onButtonEvent(GameObject* sender, ButtonState buttonState) noexcept {
    buttonActionType = GameObject::Type::COUNT;
    Tileset* tileset = Application::tileset();

    // Let's ask the tileset about the first blocking coords in the given direction
    int tileX = tileset->convertKnightX2TileX(x_ + tileset->tileWidth() / 2.);
    int tileY = tileset->convertKnightY2TileY(y_ + tileset->tileHeight() / 2.);
    //std:: cout << "Current tileX: " << tileX << ", tileY: " << tileY << "\n";

    //std::cout << "Left tile content: " << tileset->tileContent(tileX, tileY) << "\n";
    if(buttonState == ButtonState::BS_DOWN) {
        if(sender->type() == GameObject::Type::GOT_ARROW_UP && !tileset->isBlockingTile(tileX, tileY, {0., -1.})) {
            buttonActionType = sender->type();
            return true;
        }
        if(sender->type() == GameObject::Type::GOT_ARROW_DOWN && !tileset->isBlockingTile(tileX, tileY, {0., 1.})) {
            buttonActionType = sender->type();
            return true;
        }
        if(sender->type() == GameObject::Type::GOT_ARROW_RIGHT && !tileset->isBlockingTile(tileX, tileY, {1., 0.})) {
            buttonActionType = sender->type();
            return true;
        }
        if(sender->type() == GameObject::Type::GOT_ARROW_LEFT && !tileset->isBlockingTile(tileX, tileY, {-1., 0.})) {
            buttonActionType = sender->type();
            return true;
        }
    }
    return false;
}
