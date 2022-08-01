#ifndef __HEROSTRAIGHTWALKSTRATEGY_H__
#define __HEROSTRAIGHTWALKSTRATEGY_H__

#include "Strategy.h"
#include <vector>
#include "Button.h"

class Knight;
class Tileset;

class HeroStraightWalkStrategy final : public Strategy<Knight>
{
public:
    enum class Option {
        SIT_DOWN_AT_THE_END,
        ACTIVATE_NEXT_TURN,
        STOP_AT_THE_END,
    };
    HeroStraightWalkStrategy(double targetX, double targetY);
    HeroStraightWalkStrategy(const HeroStraightWalkStrategy&) = default;

    void doActions(Knight*) noexcept override;
    Strategy* copy() const noexcept override;
    void wakeUp(double now) noexcept override {lastShootTime_ = now;}
    void addOption(Option o) noexcept;

private:
    void quit() noexcept;

    double lastShootTime_;
    double targetX_;
    double targetY_;
    Button btnUp_;
    Button btnDown_;
    Button btnLeft_;
    Button btnRight_;
    Button* btnLast_{};
    Knight* knight_{};
    std::vector<Option> options_;
};

#endif // __HEROSTRAIGHTWALKSTRATEGY_H__
