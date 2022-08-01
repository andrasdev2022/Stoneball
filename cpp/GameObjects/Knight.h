#ifndef __KNIGHT_H__
#define __KNIGHT_H__

#include "StateSubject.h"
#include "android.h"
#include "Global.h"
#include "GameObject.h"
#include "Visitor.h"
#include "Strategy.h"

class GameObjectDescriptor;
class StaticObjects;
class RollingStone;

class Knight final : public StateSubject, public GameObject {
public:
    Knight(double x, double y, GameObject::Type heroType) noexcept;
    Knight(const Knight& z) noexcept;
    virtual void changeState() noexcept;

    // GameObject
    double x() const noexcept override;
    double y() const noexcept override;
    void setX(double x) noexcept override;
    void setY(double y) noexcept override;
    int width() const noexcept override;
    int height() const noexcept override;
    bool isIdle() const noexcept;
    bool isRunning() const noexcept;
    void die() noexcept override;
    bool isDead() const noexcept override;
    void yCorrectionByGravity(double newY, bool collision) noexcept override;
    double velocity() const noexcept override;
    bool isSliding() const {return !stopAt_.isNull();}

    void update(double now) noexcept override;
    void draw(AndroidBitmapInfo* info, void* pixels) noexcept override;
    const MediaData* frameData() const noexcept override;
    int frameIndex() const noexcept override;
    void setFrameIndex(int frameIndex) noexcept override;
    int skinMax() const noexcept override;
    int skinMin() const noexcept override;
    void setSkinMax(int skinMax) noexcept override;
    void setSkinMin(int skinMin) noexcept override;
    const GameObjectDescriptor& god() const noexcept override;
    void setHitPoints(int hitPoints, int maxHitPoints) noexcept override;
    bool flipped() const noexcept override;
    void setFlipped(bool f) noexcept override;
    void wakeUp(double now) noexcept override;
    bool respawn() noexcept override;
    double directionX() const noexcept override;
    double directionY() const noexcept override;
    Knight* knight() noexcept override {return this;}

    virtual void setGravity(StaticObjects* gravity) noexcept {gravity_ = gravity;}
    virtual void handleCollision(RollingStone* gameObject) noexcept;

    // ButtonEventListener
    bool onButtonEvent(GameObject* sender, ButtonState buttonState) noexcept override;

    void setMaxHeightOfJump(double height) noexcept {maxHeightOfJump_ = height;}
    double maxHeightOfJump() const noexcept {return maxHeightOfJump_;}

    void forceState(State::Type type) noexcept;

    virtual void setStrategy(Strategy<Knight>* s) noexcept {strategy_.reset(s);}
    virtual std::unique_ptr<Strategy<Knight>>& strategy() noexcept {return strategy_;}
    virtual bool isReady() const;
    virtual void sitDown();
    virtual void getUp();
    // A knight can push another one using a card
    virtual bool push(double directionX, double directionY, double velocity);

    // SerializeInterface
    bool addData(Serialize*) override;
    // copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
    // and MultiplayerWorld
    static const uint8_t* buildFrom(UDPMessageData*, const uint8_t*);

protected:

    int action;
    GameObject* buttonEventSender;
    ButtonEventListener::ButtonState stateChangeButton;
    bool died_;
    double lastAttackTime_;
    StaticObjects* gravity_;
    double maxHeightOfJump_;
    double anyButtonEventFiredTimeStamp_;
    std::unique_ptr<Strategy<Knight>> strategy_;
    VectorD stopAt_;
};

using KnightUPtr = std::unique_ptr<Knight>;
using KnightSPtr = std::shared_ptr<Knight>;

#endif // __KNIGHT_H__
