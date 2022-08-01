#include "Level27.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level27::init(WorldUPtr& world) noexcept {
    std::wstring decorationLayer =
        //0123456789012345678
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
//              0123456789012
    tileMap_ =
            //01234567890123
        L"---**************---\n" // 0
         "---*   ooH*o x **---\n" // 1
         "---* o   h* HxH *---\n" // 2
         "---****         *---\n" // 3
         "---* **HHx o*Hx**---\n" // 4
         "---**xx  * o ** *---\n" // 5
         "---*o  * * *   **---\n" // 6
         "---* *x   o x  H*---\n" // 7
         "---* H H o  o****---\n" // 8
         "---* H*  *HxH x**---\n" // 9
         "---**xx o*  o * *---\n" // 10
         "---**************---\n" // 11
;
/*
cell: 00
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 26
                  g(): 288
                  depth: 28
                  Teleport card count: 1
                  Push card count: 4
                  Pull card count: 4

//0123456789
 "*   ooH*\n" // 0
 "* o   h*\n" // 1
 "****   *\n" // 2
 "* **HHx*\n" // 3
 "**xx  **\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: QLQDRPRDtUUUDLDRRRDLQRDLDLL (27)
lines: 21
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 28
                  g(): 347
                  depth: 29
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "** ohH**\n" // 0
 "* x H  *\n" // 1
 "****H **\n" // 2
 "*o    o*\n" // 3
 "*x x  **\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: LRPRPptLDLRPRUULLRQLURDDDLLL (28)
lines: 21
--------------------------------------------------------------------------------
cell: 01
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 28
                  g(): 347
                  depth: 29
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "*o  * **\n" // 0
 "* *x   *\n" // 1
 "* H Hho*\n" // 2
 "* H*  **\n" // 3
 "**xx o**\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: DLpLqLLqLULPRtRDRRDQDDLRDDLL (28)
lines: 22
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 27
                  g(): 259
                  depth: 28
                  Teleport card count: 1
                  Push card count: 4
                  Pull card count: 3

//0123456789
 "* Hh* x*\n" // 0
 "*** Hxx*\n" // 1
 "* H    *\n" // 2
 "*oo o **\n" // 3
 "*  ** **\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: LqtRDURUUPRqLURRRLURRURRURU (27)
lines: 21
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 25
                  g(): 316
                  depth: 27
                  Teleport card count: 1
                  Push card count: 4
                  Pull card count: 5

//0123456789
 "***x*o *\n" // 0
 "* *  Ho*\n" // 1
 "*      *\n" // 2
 "*  hxH**\n" // 3
 "** Hxo**\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: ULQtDQLDLLRURRDpLDDLpqULUU (26)
lines: 21
--------------------------------------------------------------------------------
cell: 10
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 23
                  g(): 284
                  depth: 24
                  Teleport card count: 1
                  Push card count: 3
                  Pull card count: 5

//0123456789
 "**o x **\n" // 0
 "** HxH *\n" // 1
 "*  h   *\n" // 2
 "* o*Hx**\n" // 3
 "* o ** *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: QURURtDDRDRURqRpqDURqUR (23)
lines: 21
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 24
                  g(): 344
                  depth: 26
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "** o o *\n" // 0
 "**  HH *\n" // 1
 "* hHx o*\n" // 2
 "*  x* **\n" // 3
 "*x *** *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: tRLDDRDpLqLpLDRQRLDDQDDLD (25)
lines: 21
--------------------------------------------------------------------------------
cell: 11
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 25
                  g(): 316
                  depth: 27
                  Teleport card count: 1
                  Push card count: 4
                  Pull card count: 5

//0123456789
 "*h*   **\n" // 0
 "*o x  H*\n" // 1
 "*  o****\n" // 2
 "*HxH x**\n" // 3
 "*  o * *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: tDUpDRUpLqRLUURQPDRRDLDDRR (26)
lines: 21
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 26
                  g(): 346
                  depth: 28
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "*H*oo o*\n" // 0
 "**  x  *\n" // 1
 "** H****\n" // 2
 "*  hH  *\n" // 3
 "*xx *  *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: RRDQRLQRLpDLLRRDQRtLDLLQDDL (27)
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
    initDecks(world, 15, 16, 4);
}

Level27::Level27()
: TilesetMovable()
, TilesetCardHandler(1) {

}

void Level27::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level27::drawTexts() noexcept {
    Tileset::drawTexts(25, LevelFactory::minimumPushCount[24]);
}
