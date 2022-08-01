#ifndef __SINGLEPLAYERWORLD_H__
#define __SINGLEPLAYERWORLD_H__

#include "TouchEventListener.h"
#include "Knight.h"
#include "Particle.h"
#include "android.h"
#include "World.h"

#include <list>
#include <iostream>

enum BitmapId : int;

class SingleplayerWorld final : public World, public TouchEventListener {
public:
    friend class GameObjectCollider;
    SingleplayerWorld() noexcept = default;
    void init() noexcept override;
    SingleplayerWorld(const SingleplayerWorld& w) noexcept;
    SingleplayerWorld(SingleplayerWorld&& w) = delete;
    void operator=(SingleplayerWorld&& w) = delete;

    // World
    void changeState() noexcept override;
    Status status() const noexcept override {return status_;}
    void update(double now) noexcept override;
    void draw(AndroidBitmapInfo* info, void* pixels) noexcept override;
    void deleteDeadObjects() noexcept override;
    bool isHeroDead() const noexcept override {return status_ == Status::ST_HERO_IS_DEAD;}
    bool areEnemiesDead() const noexcept override {return status_ == Status::ST_ENEMIES_ARE_DEAD;}
    void setDefeat() noexcept override {status_ = Status::ST_DEFEAT;}
    double timeOfDeath() const noexcept override {return timeOfDeath_;}
    void wakeUp(double now) noexcept override;
    World* copy() const noexcept override;
    SingleplayerWorld* singleplayer() noexcept override {return this;}

    virtual bool gameOver() const noexcept {return status_ == Status::ST_DEFEAT || status_ == Status::ST_VICTORY;}
    //virtual void initRollingStones() noexcept;
    virtual void draw2DRPG() noexcept;

    // TouchEventListener
    bool onTouchEvent(int action, double x, double y) noexcept override;

    // ButtonEventListener
    bool onButtonEvent(GameObject* sender, ButtonState buttonState) noexcept override;

    void redrawLevelCompletedPanel() {redrawLevelCompletedPanel_ = true;}

private:
    void drawBitmap(BitmapId id, AndroidBitmapInfo* info, void* pixels) noexcept;
    void drawLevelCompletedPanel(AndroidBitmapInfo* info, void* pixels) noexcept;

    Status status_;
    double timeOfDeath_{0.};
    int waitForResultPanel_{};
    bool redrawLevelCompletedPanel_{};
};

#endif // __SINGLEPLAYERWORLD_H__
