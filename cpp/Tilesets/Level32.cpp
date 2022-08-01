#include "Level32.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level32::init(WorldUPtr& world) noexcept {
    std::wstring decorationLayer =
        //01234567890123456789
        L"                    \n" // 0
         "                    \n" // 1
         "                    \n" // 2
         "                    \n" // 3
         "                    \n" // 4
         "                    \n" // 5
         "                    \n" // 6
         "                    \n" // 7
         "                    \n" // 8
         "                    \n" // 9
         "                    \n" // 10
         "                    \n" // 11
;
    tileMap_ =
            //01234567890123
        L"--------*****-***---\n" // 0
         "-------** o *-* *---\n" // 1
         "---*****xxoo***x*---\n" // 2
         "---*x        H  *---\n" // 3
         "---*oHH o  H x  *---\n" // 4
         "---*Ho  *  H x* *---\n" // 5
         "---*o****   *H*x*---\n" // 6
         "---* o**xxo *** *---\n" // 7
         "---*  H H oo **x*---\n" // 8
         "---*H oh    H   *---\n" // 9
         "---*   * x  H  x*---\n" // 10
         "---**************---\n" // 11
;
/*
cells from level25cpp
-------------------------------------------------------------------------------
cell 00
-------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 25
                  g(): 316
                  depth: 27
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 4

//0123456789
 "*  * *h*\n" // 0
 "*****xx*\n" // 1
 "*x     *\n" // 2
 "*oHH o *\n" // 3
 "*Ho  * *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: tPDLDRRRqRqULLDPLURRRURURU (26)
lines: 20
-------------------------------------------------------------------------------
cell 10
-------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 25
                  g(): 316
                  depth: 27
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 4

//0123456789
 "*o *** *\n" // 0
 "*oo***x*\n" // 1
 "*   H  *\n" // 2
 "*hH x  *\n" // 3
 "* H x* *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: LDtDDRQRPLDRQDRRLULPRRRRDU (26)
lines: 20
-------------------------------------------------------------------------------
cell 01
-------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 29
                  g(): 320
                  depth: 31
                  Teleport card count: 1
                  Push card count: 4
                  Pull card count: 5

//0123456789
 "*o**** *\n" // 0
 "* o**xx*\n" // 1
 "*  H H *\n" // 2
 "*H oh  *\n" // 3
 "*   * x*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: LURQRQRRRRRDQtDLUDQDPUDRRRRUUU (30)
lines: 20
-------------------------------------------------------------------------------
cell 11
-------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 28
                  g(): 319
                  depth: 30
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 4

//0123456789
 "*  *H*x*\n" // 0
 "*o *** *\n" // 1
 "*oo **x*\n" // 2
 "* hH   *\n" // 3
 "*  H  x*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: RQRQtLLDRRRLDPRDRDRPRRRRURUUU (29)
lines: 20
*/
    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(decorationLayer.begin(), decorationLayer.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    row = 0;
    col = 0;
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
    // pull push teleport
    initDecks(world, 9, 11, 1);
}

Level32::Level32()
: TilesetMovable()
, TilesetCardHandler(1) {

}

void Level32::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level32::drawTexts() noexcept {
    Tileset::drawTexts(30, LevelFactory::minimumPushCount[29]);
}
