#ifndef __STATEKNIGHTSIT_H__
#define __STATEKNIGHTSIT_H__

#include "State.h"
#include "GameObject.h"
#include "VisitorSubject.h"
#include "Visitor.h"

class StateKnightSit final : public State, public GameObject {
public:
    StateKnightSit(double x, double bottomY, GameObject::Type heroType) noexcept;
    StateKnightSit(const StateKnightSit& z) noexcept = default;

    // GameObject
    void update(double now) noexcept override;
    const GameObjectDescriptor& god() const noexcept override;

    // State
    void change(StateSubject* subject) noexcept override;
    State* copy() const noexcept override;
    std::string stateTypeStr() const noexcept override {return "Sit";}
    State::Type stateType() const noexcept override {return State::Type::ST_SIT;}

    // VisitorSubject
    void accept(Visitor* v) noexcept override {v->visit(this);}

    // SerializeInterface, nothing to do
    bool addData(Serialize*) override {return false;}
    //const uint8_t* buildFrom(UDPMessageData*, const uint8_t*) override {return nullptr;}

private:
};

#endif // __STATEKNIGHTSIT_H__
