#ifndef __STATEKNIGHTRUN_H__
#define __STATEKNIGHTRUN_H__

#include "State.h"
#include "GameObject.h"
#include "VisitorSubject.h"
#include "Visitor.h"
#include "GameObjectDescriptor.h"

class StateKnightRun final : public State, public GameObject {
public:
    StateKnightRun(double x, double bottomY, GameObject::Type heroType, Direction direction) noexcept;
    StateKnightRun(const StateKnightRun& z) noexcept = default;

    // GameObject
    void update(double now) noexcept override;
    //bool flipped() const noexcept override {return direction.x < 0;}
    //void setFlipped(bool f) noexcept override {direction.x = f ? -1. : 1.;}
    const GameObjectDescriptor& god() const noexcept override;
    bool isReady() const noexcept override {return ready_;}

    // State
    void change(StateSubject* subject) noexcept override;
    State* copy() const noexcept override;
    std::string stateTypeStr() const noexcept override {return "Run";}
    State::Type stateType() const noexcept override {return State::Type::ST_RUN;}

    // VisitorSubject
    void accept(Visitor* v) noexcept override {v->visit(this);}

    // ButtonEventListener
    bool onButtonEvent(GameObject* sender, ButtonState buttonState) noexcept override;

    virtual void keepRunning(bool run);

    // SerializeInterface, nothing to do
    bool addData(Serialize*) override {return false;}
    //const uint8_t* buildFrom(UDPMessageData*, const uint8_t*) override {return nullptr;}

private:

    void changeDescriptor(Direction direction) noexcept;
    void correctCoordinates() noexcept;
    bool stopOnTile() noexcept;
    bool isNextTileValid(double x, double y) noexcept;
    void revetPosition() noexcept;
    bool findKnightOnTheTile(int nextTileX, int nextTileY);
    bool matchButtonAndDirection() noexcept;

    Speed actionParamSpeed_;
    GameObject::Type buttonActionType_;
    GameObjectDescriptor descriptor_;
    bool ready_{false};
    double startX_;
    double startY_;
    bool stopping_;
    bool keepRunning_;
};

#endif // __STATEKNIGHTRUN_H__
