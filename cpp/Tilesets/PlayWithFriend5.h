#ifndef __PLAYWITHFRIEND5_H__
#define __PLAYWITHFRIEND5_H__

#include <string>
#include "World.h"
#include "Tileset.h"
#include "PlayWithSomeone.h"

class PlayWithFriend5 : public PlayWithSomeone
{
public:
    static constexpr int TEAMCOUNT = 4;

    PlayWithFriend5();

    void init(WorldUPtr& world) noexcept override;
    void update(double now) noexcept override {Tileset::update(now); updateDecks(now); updateCards(now);}
    void updateTexts(double now) noexcept override;
    void updateTeamMarker(double now) noexcept override;
    uint32_t score() const noexcept override;
    void setScore(uint32_t score) noexcept override;
    int teamCount() const noexcept override {return TEAMCOUNT;}

    // dynamic_cast
    virtual PlayWithFriend5* playWithFriend5() noexcept override {return this;}

protected:

private:

    Team lastActiveTeam_{TEAMCOUNT};
    std::wstring scoreText_;
};

using PlayWithFriend5UPtr = std::unique_ptr<PlayWithFriend5>;

#endif // __PlayWithFriend5_H__
