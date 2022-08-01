#include "Level24.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level24::init(WorldUPtr& world) noexcept {
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

    tileMap_ =
    //01234567890123
    L"---**************---\n" // 0
     "---* x o o H  o *---\n" // 1
     "---*  H H   H  H*---\n" // 2
     "---*  **   h**o *---\n" // 3
     "---*H ***   *****---\n" // 4
     "---* ox x*ox  xx*---\n" // 5
     "---*  o*   *    *---\n" // 6
     "---* o  H  HoH x*---\n" // 7
     "---*oH*  x   *  *---\n" // 8
     "---*  ***   ** **---\n" // 9
     "---* x* Hx*ooHxx*---\n" // 10
     "---**************---\n" // 11
;
/*
cell: 00
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 33
                  g(): 353
                  depth: 35
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "* x o o*\n" // 0
 "*  H H *\n" // 1
 "* h**  *\n" // 2
 "*H *** *\n" // 3
 "* ox x**\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: qtUUURLpRLpLRUQULLLDRQLDLDRLDDDRRR (34)
lines: 25
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 35
                  g(): 355
                  depth: 37
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "* o  H *\n" // 0
 "*H  H  *\n" // 1
 "* o**h *\n" // 2
 "*****  *\n" // 3
 "*xx  xo*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: LRtRRRLDLqQLRDPRDDqURDDDUpDDLLLDLLLL (36)
lines: 24
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 31
                  g(): 351
                  depth: 33
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "*  x   *\n" // 0
 "*xH H  *\n" // 1
 "*H **o *\n" // 2
 "*  **ho*\n" // 3
 "*  x**o*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: UULLRUtULRqUpDUqqLLLDUpDULLDDRLL (32)
lines: 23
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 32
                  g(): 351
                  depth: 33
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "*H    **\n" // 0
 "* oh xo*\n" // 1
 "*  **  *\n" // 2
 "*oH*H **\n" // 3
 "*  xx **\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: PtUURPDRURRRURLLpLLDppDLDLDDRDRR (32)
lines: 23

cell: 01
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 37
                  g(): 328
                  depth: 39
                  Teleport card count: 1
                  Push card count: 4
                  Pull card count: 5

//0123456789
 "*h o*  *\n" // 0
 "* o  H *\n" // 1
 "*oH*  x*\n" // 2
 "*  *** *\n" // 3
 "* x* Hx*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: RpRLDQDUULtDLLDDDPDURRRRDqUUPDURRRRDDD (38)
lines: 24
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 32
                  g(): 351
                  depth: 33
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "*    * *\n" // 0
 "*x HoH *\n" // 1
 "*h *   *\n" // 2
 "** **  *\n" // 3
 "*xxHoo**\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: DRPURDUqLqLLDULLDDRLDqqDDtLULLLL (32)
lines: 23
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 30
                  g(): 291
                  depth: 31
                  Teleport card count: 1
                  Push card count: 4
                  Pull card count: 4

//0123456789
 "*    *o*\n" // 0
 "* Hoh  *\n" // 1
 "** *o  *\n" // 2
 "**x**  *\n" // 3
 "*xxH H *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: PRDLLULPLULLQLDDLDpLLDDRLDDDtL (30)
lines: 22
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 32
                  g(): 351
                  depth: 33
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "*  o   *\n" // 0
 "*x H H *\n" // 1
 "*o *o* *\n" // 2
 "*H ***h*\n" // 3
 "*x  x **\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: RqLLRDDLPLLLRRqPDLDDLtLQRULDDDRR (32)
lines: 22
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 30
                  g(): 263
                  depth: 32
                  Teleport card count: 1
                  Push card count: 3
                  Pull card count: 4

//0123456789
 "**    o*\n" // 0
 "*H o h *\n" // 1
 "* o**Hx*\n" // 2
 "*H***  *\n" // 3
 "* xx   *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: pLLtRRRDURqRRQULRDPRDDRDDLLDLLL (31)
lines: 21
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
 "*     o*\n" // 0
 "*oH H H*\n" // 1
 "* h* x *\n" // 2
 "*o **x *\n" // 3
 "*  **x**\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: UULPtRDRRDUpUQLPDRDURRDRDDD (27)
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
    initDecks(world, 11, 16, 2);
}

Level24::Level24()
: TilesetMovable()
, TilesetCardHandler(1) {

}

void Level24::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level24::drawTexts() noexcept {
    Tileset::drawTexts(22, LevelFactory::minimumPushCount[21]);
}
