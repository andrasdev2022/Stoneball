#ifndef __HEROWALKSTRATEGY_H__
#define __HEROWALKSTRATEGY_H__

#include "Strategy.h"
#include <vector>
#include "Button.h"

class Knight;
class Tileset;

class HeroWalkStrategy final : public Strategy<Knight>
{
public:
    HeroWalkStrategy(int targetX, int targetY);
    HeroWalkStrategy(const HeroWalkStrategy&) = default;

    void doActions(Knight*) noexcept override;
    Strategy* copy() const noexcept override;
    void wakeUp(double now) noexcept override {lastShootTime_ = now;}
    void finishStrategy() noexcept override;

private:
    void quit();
    bool findNextTileForHero();
    void findPath();
    void followThePath();
    bool waitForHeroWalk();
    void goToTheNextTile();

    double lastShootTime_;
    int targetX_;
    int targetY_;
    std::vector<std::vector<std::vector<char>>> aStarResult_;
    int stepIndex_;
    Button btnUp_;
    Button btnDown_;
    Button btnLeft_;
    Button btnRight_;
    Button* btnLast_{};
    bool waitForWalk_{};
    int nextX_{};
    int nextY_{};

    Knight* knight_{};
    Tileset* tileset_{};
    int heroTileX_{}; // current tile position of the hero on the tileset
    int heroTileY_{};
};

#endif // __HEROWALKSTRATEGY_H__
