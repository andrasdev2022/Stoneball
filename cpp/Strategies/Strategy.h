#ifndef __STRATEGY_H__
#define __STRATEGY_H__

#include <memory>
#include "stoneball.h"
#include "Global.h"

class GameObject;

template<typename T>
class Strategy {
public:
    Strategy(GameObject* attackButton1) noexcept : heroXPos(-1.), heroYPos(-1.), collisionTimestamp_(0.), attackButton1_(attackButton1) {}
    Strategy(const Strategy& s) noexcept = default;
    virtual ~Strategy() noexcept = default;

    virtual void doActions(T*) = 0;
    virtual Strategy<T>* copy() const = 0;
    virtual void updateHeroCenterPosition(double x, double y) noexcept {heroXPos = x; heroYPos = y;}
    virtual void setHeroCollision(bool c) noexcept;
    //virtual bool heroCollision() const {return heroCollisionCounter_ > 0;}
    virtual GameObject* attackButton() const noexcept {return attackButton1_;}
    virtual void wakeUp([[maybe_unused]]double now) noexcept {}
    virtual void finishStrategy() noexcept {}

protected:
    double heroXPos;
    double heroYPos;
    double collisionTimestamp_;
    GameObject* attackButton1_;
};

template<typename T>
void Strategy<T>::setHeroCollision(bool c) noexcept {
    extern double appNow();
    if(c) collisionTimestamp_ = appNow();
}

//using StrategyUPtr = std::unique_ptr<Strategy>;
//using StrategySPtr = std::shared_ptr<Strategy>;

#endif // __STRATEGY_H__
