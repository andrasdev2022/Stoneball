#include "StateKnightIdle.h"
#include "StateKnightSit.h"
#include "StateKnightRun.h"
#include "stoneball.h"
#include "StateSubject.h"
#include "Knight.h"
#include "GameObjectDescriptor.h"

#include <math.h>

StateKnightSit::StateKnightSit(double x, double bottomY, GameObject::Type heroType) noexcept
 : GameObject(x, bottomY, heroType) {
    this->direction_ = Vector<double>(1, 0);
    init();
}

State* StateKnightSit::copy() const noexcept {
    return new StateKnightSit(*this);
}

void StateKnightSit::change(StateSubject*) noexcept {
}

void StateKnightSit::update(double now) noexcept {
    updateFrame(now, skinMin_, skinMax_);
    walkOrMove(now);
}

const GameObjectDescriptor& StateKnightSit::god() const noexcept {
    return GameObjectDescriptor::find(type_, State::Type::ST_SIT);
}
