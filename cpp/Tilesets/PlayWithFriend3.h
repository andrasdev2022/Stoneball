#ifndef __PLAYWITHFRIEND3_H__
#define __PLAYWITHFRIEND3_H__

#include <string>
#include "World.h"
#include "Tileset.h"
#include "PlayWithSomeone.h"

class PlayWithFriend3 : public PlayWithSomeone
{
public:
    static constexpr int TEAMCOUNT = 3;

    PlayWithFriend3();

    void init(WorldUPtr& world) noexcept override;
    void update(double now) noexcept override {Tileset::update(now); updateDecks(now); updateCards(now);}
    void updateTexts(double now) noexcept override;
    void updateTeamMarker(double now) noexcept override;
    uint32_t score() const noexcept override;
    void setScore(uint32_t score) noexcept override;
    int teamCount() const noexcept override {return TEAMCOUNT;}

    // dynamic_cast
    virtual PlayWithFriend3* playWithFriend3() noexcept override {return this;}

protected:

private:

    Team lastActiveTeam_{TEAMCOUNT};
    std::wstring scoreText_;
};

using PlayWithFriend3UPtr = std::unique_ptr<PlayWithFriend3>;

#endif // __PLAYWITHFRIEND3_H__
