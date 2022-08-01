#include "Level22.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level22::init(WorldUPtr& world) noexcept {
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
    L"----*************---\n" // 0
     "---** o oo*  x  *---\n" // 1
     "---*       o  Hx*---\n" // 2
     "---*  ** xo **  *---\n" // 3
     "---* H**H  o**  *---\n" // 4
     "---* x   x  H  x*---\n" // 5
     "---** xx  H*xx  *---\n" // 6
     "---*h  *H * *H  *---\n" // 7
     "---*ox *** * *  *---\n" // 8
     "---*  H o**x H o*---\n" // 9
     "---**  oH   o o**---\n" // 10
     "---*************----\n" // 11
;

/*
cell: 00
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 41
                  g(): 302
                  depth: 42
                  Teleport card count: 1
                  Push card count: 3
                  Pull card count: 5

//0123456789
 "** o oo*\n" // 0
 "*      *\n" // 1
 "*  ** x*\n" // 2
 "* H**H *\n" // 3
 "* x h x*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: tLpLQRRRDLpLQRRRRDLLQRRRRDDDULLURRRDDDLLL (41)
lines: 22
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 34
                  g(): 324
                  depth: 35
                  Teleport card count: 1
                  Push card count: 4
                  Pull card count: 5

//0123456789
 "**  x  *\n" // 0
 "* o  Hx*\n" // 1
 "*o **  *\n" // 2
 "* o** h*\n" // 3
 "*  H  x*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: tDLDPDDDUqqRUqRPRRLLURRRRDDURRRULD (34)
lines: 22


cell: 01
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 22
                  g(): 284
                  depth: 24
                  Teleport card count: 1
                  Push card count: 4
                  Pull card count: 4

//0123456789
 "** xx  *\n" // 0
 "*h  *H *\n" // 1
 "*ox ****\n" // 2
 "*  H o**\n" // 3
 "**  oH *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: DtUPUpLqRLUDRLUURpLUUUR (23)
lines: 19
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 25
                  g(): 229
                  depth: 27
                  Teleport card count: 1
                  Push card count: 2
                  Pull card count: 4

//0123456789
 "*  xx*H*\n" // 0
 "*  H* **\n" // 1
 "* h* * *\n" // 2
 "*o H x**\n" // 3
 "**o o  *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: RqUqRDRULUUURUUPtLLRRLqRRR (26)
lines: 18
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 21
                  g(): 167
                  depth: 23
                  Teleport card count: 1
                  Push card count: 0
                  Pull card count: 4

//0123456789
 "* * *  *\n" // 0
 "*o  * H*\n" // 1
 "*oo  * *\n" // 2
 "*H xH***\n" // 3
 "**h x x*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: UPtDRDUPRDURDQDRQDDRRR (22)
lines: 18
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 20
                  g(): 224
                  depth: 22
                  Teleport card count: 1
                  Push card count: 2
                  Pull card count: 4

//0123456789
 "*oo ** *\n" // 0
 "*H  * h*\n" // 1
 "*  H ***\n" // 2
 "*o x H**\n" // 3
 "**  xx *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: tQRDPDUQDRDURDDRPRDRR (21)
lines: 18
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 20
                  g(): 195
                  depth: 22
                  Teleport card count: 1
                  Push card count: 1
                  Pull card count: 4

//0123456789
 "**o x **\n" // 0
 "* o ** *\n" // 1
 "*    *H*\n" // 2
 "*Hh x **\n" // 3
 "** H xo*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: RQDQDRRDLRDRptLqLUUUR (21)
lines: 17
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 22
                  g(): 168
                  depth: 24
                  Teleport card count: 1
                  Push card count: 1
                  Pull card count: 3

//0123456789
 "*h xx***\n" // 0
 "*  H*H *\n" // 1
 "* o* * *\n" // 2
 "* x H **\n" // 3
 "**  oo *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: DRpLqRLUtLLUURLqRLUUURR (23)
lines: 17
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 21
                  g(): 225
                  depth: 23
                  Teleport card count: 1
                  Push card count: 2
                  Pull card count: 4

//0123456789
 "**h  x *\n" // 0
 "* o *  *\n" // 1
 "*HoH * *\n" // 2
 "** o  **\n" // 3
 "***xx H*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: LtUURDQDPDLUURPDPUDDDR (22)
lines: 17
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 20
                  g(): 224
                  depth: 22
                  Teleport card count: 1
                  Push card count: 3
                  Pull card count: 3

//0123456789
 "*  o*HH*\n" // 0
 "*H *** *\n" // 1
 "* oo * *\n" // 2
 "* h xx**\n" // 3
 "**    x*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: tDRpDQURUQDRLRDRDRRRR (21)
lines: 17

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
    initDecks(world, 16, 12, 4);
}

Level22::Level22()
: TilesetMovable()
, TilesetCardHandler(1) {

}

void Level22::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level22::drawTexts() noexcept {
    Tileset::drawTexts(20, LevelFactory::minimumPushCount[19]);
}
