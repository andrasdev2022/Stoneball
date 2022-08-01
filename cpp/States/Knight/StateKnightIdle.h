#ifndef __STATEKNIGHTIDLE_H__
#define __STATEKNIGHTIDLE_H__

#include "State.h"
#include "GameObject.h"
#include "VisitorSubject.h"
#include "Visitor.h"

class GameObjectDescriptor;

class StateKnightIdle final : public State, public GameObject {
public:
    StateKnightIdle(double x, double bottomY, GameObject::Type heroType, double directionX) noexcept;
    StateKnightIdle(const StateKnightIdle& z) noexcept = default;

    // GameObject
    void update(double now) noexcept override;
    bool flipped() const noexcept override {return direction_.x < 0;}
    void setFlipped(bool f) noexcept override {direction_.x = f ? -1. : 1.;}
    const GameObjectDescriptor& god() const noexcept override;

    // State
    void change(StateSubject* subject) noexcept override;
    State* copy() const noexcept override;
    std::string stateTypeStr() const noexcept override {return "Idle";}
    State::Type stateType() const noexcept override {return State::Type::ST_IDLE;}

    // VisitorSubject
    void accept(Visitor* v) noexcept override {v->visit(this);}

    // ButtonEventListener
    bool onButtonEvent(GameObject* sender, ButtonState buttonState) noexcept override;

    // SerializeInterface, nothing to do
    bool addData(Serialize*) override {return false;}
    //const uint8_t* buildFrom(UDPMessageData*, const uint8_t*) override {return nullptr;}

private:
    double firstMoveX;
    double firstMoveY;
    int action;
    GameObject::Type buttonActionType;
};

#endif // __STATEKNIGHTIDLE_H__
