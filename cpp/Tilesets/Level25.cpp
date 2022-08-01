#include "Level25.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level25::init(WorldUPtr& world) noexcept {
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
             "---*o  * x     o*---\n" // 1
             "---**  xH x H o *---\n" // 2
             "---*oo x  x**H  *---\n" // 3
             "---*  * H x  *H *---\n" // 4
             "---*  *H **  * o*---\n" // 5
             "---*oH* ** * *oo*---\n" // 6
             "---* ***  H*** o*---\n" // 7
             "---*H  o o  x*  *---\n" // 8
             "---*x   hH H x H*---\n" // 9
             "---* x*x  *x    *---\n" // 10
             "---**************---\n" // 11
;

/*
cell: 00
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 26
                  g(): 201
                  depth: 28
                  Teleport card count: 1
                  Push card count: 1
                  Pull card count: 4

//0123456789
 "*o  *hx*\n" // 0
 "**  xH *\n" // 1
 "*oo x  *\n" // 2
 "*  * H *\n" // 3
 "*  *H **\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: tDPqURRDPDURQRDRRDUUDURRRLU (27)
lines: 21 *
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
 "*o     *\n" // 0
 "* o H x*\n" // 1
 "*  H**x*\n" // 2
 "*hH*  x*\n" // 3
 "*o *  **\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: RDLDPLLtUqURRRQqRURRPRRDDURRRRDD (32)
lines: 21 *
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
 "*  H oo*\n" // 0
 "*HHo   *\n" // 1
 "*x h*  *\n" // 2
 "* x*  x*\n" // 3
 "* ***  *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: tRDQLQDPDRqRPULLLDLDULLLDD (26)
lines: 20
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 25
                  g(): 287
                  depth: 27
                  Teleport card count: 1
                  Push card count: 3
                  Pull card count: 5

//0123456789
 "* o h x*\n" // 0
 "*o H* **\n" // 1
 "*   H H*\n" // 2
 "*  *  x*\n" // 3
 "*o *x **\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: LQRDPRRqUDRDRPRRRDPRRRtLDR (26)
lines: 20

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
 "*oH* ***\n" // 0
 "* ***  *\n" // 1
 "*H  o o*\n" // 2
 "*x   hH*\n" // 3
 "* x*x  *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: LpRUDQDLLLRDLtDLQLLDDQPRRRDLD (29)
lines: 22 *
-------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 24
                  g(): 257
                  depth: 26
                  Teleport card count: 1
                  Push card count: 2
                  Pull card count: 5

//0123456789
 "*oo* * *\n" // 0
 "*oh***H*\n" // 1
 "*  *x  *\n" // 2
 "*H x H *\n" // 3
 "*    x**\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: QDtDDRQDPUDRQDRDPUDRURRRD (25)
lines: 21 *
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 27
                  g(): 260
                  depth: 29
                  Teleport card count: 1
                  Push card count: 2
                  Pull card count: 5

//0123456789
 "*H** *x*\n" // 0
 "* o**  *\n" // 1
 "*oo  *x*\n" // 2
 "* Hh   *\n" // 3
 "* *xH  *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: PLRPtRDQDRRRRUpDRRRpLDRUURDU (28)
lines: 21
-------------------------------------------------------------------------------
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
    initDecks(world, 19, 10, 4);
}

Level25::Level25()
: TilesetMovable()
, TilesetCardHandler(1) {

}

void Level25::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level25::drawTexts() noexcept {
    Tileset::drawTexts(23, LevelFactory::minimumPushCount[22]);
}
