#include "Level23.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level23::init(WorldUPtr& world) noexcept {
    std::wstring decorationLayer =
    //01234567890123
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
    L"----************----\n" // 0
     "---**x* o *  H **---\n" // 1
     "---**  *  *oH   *---\n" // 2
     "---*    Hox  H **---\n" // 3
     "---*   oH**x*o o*---\n" // 4
     "---**x*xH x  * **---\n" // 5
     "---*  o o***Ho o*---\n" // 6
     "---*o H x** x*x *---\n" // 7
     "---*  H*H**Hx*  *---\n" // 8
     "---*  ******    *---\n" // 9
     "---* xx  h  o  H*---\n" // 10
     "---**************---\n" // 11
;

/*
cell: 00
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 25
                  g(): 345
                  depth: 27
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "**x* o *\n" // 0
 "**  *h *\n" // 1
 "*    Ho*\n" // 2
 "*   oH**\n" // 3
 "**x*xH *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: qLtLDLDRDpUDPDQUpDLLLDLLUU (26)
lines: 21

cell: 10
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 27
                  g(): 347
                  depth: 29
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "**  H **\n" // 0
 "**oH   *\n" // 1
 "*x hH **\n" // 2
 "**x*o o*\n" // 3
 "*x  * **\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: URQDRRRLLqpUqLULpDDULDLDDDtL (28)
lines: 21

cell: 01
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 28
                  g(): 290
                  depth: 30
                  Teleport card count: 1
                  Push card count: 3
                  Pull card count: 5

//0123456789
 "*  o o**\n" // 0
 "*o H x**\n" // 1
 "*  H*H**\n" // 2
 "*  *****\n" // 3
 "* xx  h*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: tDpDQDDLUDULUPDURRRpLLQRLDDDR (29)
lines: 22
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 24
                  g(): 199
                  depth: 26
                  Teleport card count: 1
                  Push card count: 0
                  Pull card count: 5

//0123456789
 "*o oH***\n" // 0
 "* x*x **\n" // 1
 "*  *xH**\n" // 2
 "*h   ***\n" // 3
 "*H  o  *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: QtLLqRLUUDpDPUDRQDRDRURUU (25)
lines: 21

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
    initDecks(world, 13, 8, 4);
}

Level23::Level23()
: TilesetMovable()
, TilesetCardHandler(1) {

}

void Level23::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level23::drawTexts() noexcept {
    Tileset::drawTexts(21, LevelFactory::minimumPushCount[20]);
}
