#include "Multiplayer6.h"
#include "Application.h"
#include "BouncingStones.h"
#include "Screen.h"
#include "Environment.h"
#include "MediaData.h"

Multiplayer6::Multiplayer6()
: Multiplayer5()
{
#ifdef __SERVERAPP__
    tileMap_ =
           //01234567890123456789012345678
           L"---------------------------\n" // 0
            "---------------------------\n" // 1
            "----******************-----\n" // 2
            "----*  X   XXXX   x  *-----\n" // 3
            "----*G X          x G*-----\n" // 4
            "----*K   OoKGhHOo   K*-----\n" // 5
            "----*H   G  ó ó K   H*-----\n" // 6
            "----*h X K   ó  H x h*-----\n" // 7
            "----*G X H  ó   h x G*-----\n" // 8
            "----*K   h ó ó  G   K*-----\n" // 9
            "----*H   oOhGKHoO   H*-----\n" // 0
            "----*h X          x h*-----\n" // 1
            "----*  X   xxxx   x  *-----\n" // 2
            "----******************-----\n" // 3
            "---------------------------\n" // 4
            "---------------------------\n" // 5 // for the extreme screen aspect ratios
            "---------------------------\n" // 6
;

#else
    tileMap_ =
           //01234567890123456789012345678
           L"                           \n" // 0
            "                           \n" // 1
            "    ******************     \n" // 2
            "    *                *     \n" // 3
            "    *                *     \n" // 4
            "    *                *     \n" // 5
            "    *                *     \n" // 6
            "    *                *     \n" // 7
            "    *                *     \n" // 8
            "    *                *     \n" // 9
            "    *                *     \n" // 0
            "    *                *     \n" // 1
            "    *                *     \n" // 2
            "    ******************     \n" // 3
            "                           \n" // 4
            "                           \n" // 5 // for the extreme screen aspect ratios
            "                           \n" // 6
;

#endif
}

void Multiplayer6::init(WorldUPtr& world) noexcept {
    std::wstring firstLayer =
            L"                           \n" // 5
             "                           \n" // 6
             "   d                  A    \n" // 0
             "     eqeqeqeqeqeqeqeq      \n" // 1
             "   a eeeeeeeeeeeeeeee      \n" // 2
             "     eqeqeqeqeqeqeqeq      \n" // 3
             "     eeeeeeeeeeeeeeee A    \n" // 4
             "     eqeqeqeqeqeqeqeq      \n" // 5
             "     eeeeeeeeeeeeeeee      \n" // 6
             "   A eqeqeqeqeqeqeqeq      \n" // 7
             "     eeeeeeeeeeeeeeee      \n" // 8
             "     eqeqeqeqeqeqeqeq a    \n" // 9
             "     eeeeeeeeeeeeeeee      \n" // 0
             "                           \n" // 1
             "                           \n" // 2
             "                           \n" // 3 // for the extreme screen aspect ratios
             "                           \n" // 4
            //01234567890123456789012345678
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
