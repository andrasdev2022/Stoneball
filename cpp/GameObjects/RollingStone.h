#ifndef __ROLLINGSTONE_H__
#define __ROLLINGSTONE_H__

#include "GameObject.h"
#include "ObserverSubjectVictoryEvent.h"
#include "Rect.h"
#include <vector>
#include "ObserverSubjectVictoryEventVisitor.h"
#include "GameObjectDescriptor.h"
#include "RollingStoneInterface.h"

class StaticObjects;

class RollingStone final : public RollingStoneInterface, public GameObject, public ObserverSubjectVictoryEvent {
public:

    RollingStone(double x, double y, GameObjectInterface::Type type, Color color) noexcept;
    RollingStone(const RollingStone& g) = default;

    // GameObject
    void update(double now) noexcept override;
    const GameObjectDescriptor& god() const noexcept override;
    void die() noexcept override {dead_ = true;}
    bool isDead() const noexcept override {return dead_;}

    virtual void push(double x, double y, double velocity) noexcept;
    virtual void addHoleRect(Rect r) noexcept {holeRects_.push_back(r);}
    virtual Rect collidedRect() const {return collided_;}

    // ObserverSubjectVictoryEvent
    void accept(ObserverSubjectVictoryEventVisitor* v) noexcept override {v->visit(this);}

    [[nodiscard]] Color color() const noexcept {return color_;}
    void setColor(Color c) noexcept {if(color_ != c){color_ = c; changeDescriptor();}}

    // SerializeInterface
    bool addData(Serialize*) override;
    // copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
    // and MultiplayerWorld
    static const uint8_t* buildFrom(UDPMessageData*, const uint8_t*);

    bool isRolling() noexcept {return !stopAt_.isNull();}

private:
    void changeDescriptor() noexcept;
    void interateSkinMin() noexcept;
    void resetSkinMin() noexcept;

    std::vector<Rect> holeRects_;
    bool dead_{false};
    Rect collided_;
    VectorD stopAt_;
    Color color_;
    GameObjectDescriptor descriptor_;
};

using RollingStoneUPtr = std::unique_ptr<RollingStone>;
#endif // __ROLLINGSTONE_H__
