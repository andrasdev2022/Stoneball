#include "Multiplayer2.h"
#include "Application.h"

Multiplayer2::Multiplayer2()
: Multiplayer1()
{
#ifdef __SERVERAPP__
    tileMap_ =
           //0123456789012345678901234
           L"------------------------------\n" // 0
            "------------------------------\n" // 1
            "-------****************-------\n" // 2
            "-------* *X        x* *-------\n" // 3
            "-------* *X        x* *-------\n" // 4
            "-------*    OoOoOo    *-------\n" // 5
            "-------*h X h ó  H x H*-------\n" // 6
            "-------*H*X h  ó H x*h*-------\n" // 7
            "-------*h*X h ó  H x*H*-------\n" // 8
            "-------*H X h  ó H x h*-------\n" // 9
            "-------*    oOoOoO    *-------\n" // 0
            "-------* *X        x* *-------\n" // 1
            "-------* *X        x* *-------\n" // 2
            "-------****************-------\n" // 3
            "------------------------------\n" // 4
            "------------------------------\n" // 5 // for the extreme screen aspect ratios
            "------------------------------\n" // 6
;

#else
    tileMap_ =
            //0123456789012345678901234
           L"                              \n" // 0
            "                              \n" // 1
            "       ****************       \n" // 2
            "       * *          * *       \n" // 3
            "       * *          * *       \n" // 4
            "       *              *       \n" // 5
            "       *              *       \n" // 6
            "       * *          * *       \n" // 7
            "       * *          * *       \n" // 8
            "       *              *       \n" // 9
            "       *              *       \n" // 0
            "       * *          * *       \n" // 1
            "       * *          * *       \n" // 2
            "       ****************       \n" // 3
            "                              \n" // 4
            "                              \n" // 5
            "                              \n" // 6
;

#endif
}

void Multiplayer2::init(WorldUPtr& world) noexcept {
    std::wstring firstLayer =
           L"                              \n" // 5
            "                     A        \n" // 6
            "                              \n" // 0
            "           qeqeqeqe           \n" // 1
            "        a  eeeeeeee           \n" // 2
            "           qeqeqeqe           \n" // 3
            "      A    eeeeeeee  d        \n" // 4
            "           qeqeqeqe           \n" // 5
            "           eeeeeeee    a      \n" // 6
            "        A  qeqeqeqe           \n" // 7
            "           eeeeeeee           \n" // 8
            "      a    qeqeqeqe  a        \n" // 9
            "                              \n" // 0
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
