#include "PlayWithFriend4.h"
#include "Application.h"
#include "EnumClassOp.h"

PlayWithFriend4::PlayWithFriend4()
: PlayWithFriend3()
{
    tileMap_ =
           //0123456789012345678901234
           L"------------------------------\n" // 0
            "------------------------------\n" // 1
            "-------****************-------\n" // 2
            "-------* *X   XX   x* *-------\n" // 3
            "-------* *X        x* *-------\n" // 4
            "-------*K   OKHhOo   h*-------\n" // 5
            "-------*H X o ó  K x H*-------\n" // 6
            "-------*h X K  ó H x K*-------\n" // 7
            "-------*K X H ó  h x h*-------\n" // 8
            "-------*H X h  ó o x H*-------\n" // 9
            "-------*h   oOhKHO   K*-------\n" // 0
            "-------* *X        x* *-------\n" // 1
            "-------* *X   xx   x* *-------\n" // 2
            "-------****************-------\n" // 3
            "------------------------------\n" // 4
            "------------------------------\n" // 5 // for the extreme screen aspect ratios
            "------------------------------\n" // 6
;
}

void PlayWithFriend4::init(WorldUPtr& world) noexcept {
    std::wstring firstLayer =
           L"                              \n" // 5
            "                     A        \n" // 6
            "                              \n" // 0
            "        eqeqeqeqeqeqeq        \n" // 1
            "        aeeeeeeeeeeeed        \n" // 2
            "        eqeqeqeqeqeqeq        \n" // 3
            "      A eeeeeeeeeeeeed        \n" // 4
            "        eqeqeqeqeqeqeq        \n" // 5
            "        eeeeeeeeeeeeee a      \n" // 6
            "        Aqeqeqeqeqeqeq        \n" // 7
            "        eeeeeeeeeeeeee        \n" // 8
            "      a eqeqeqeqeqeqea        \n" // 9
            "        eeeeeeeeeeeeee        \n" // 0
            "                              \n" // 1
            "                              \n" // 2
            "                              \n" // 3 // for the extreme screen aspect ratios
            "                              \n" // 4
           //0123456789012345678901234567
            ;
;

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(firstLayer.begin(), firstLayer.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    row = 0;
    col = 0;
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
    setPlayMode(Application::PlayMode::TURNBASED);

#ifdef __SERVERAPP__
    initDecks(world, 100, 100, 100);
#else
    if(!world->multiplayer()) {
        initDecks(world, 100, 100, 100);
    }
#endif
}
