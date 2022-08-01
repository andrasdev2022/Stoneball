#include "Level26.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level26::init(WorldUPtr& world) noexcept {
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
         "---*x*oHo *  o  *---\n" // 1
         "---*x Ho  o* Hxx*---\n" // 2
         "---*     * Ho H**---\n" // 3
         "---*Hh ***    **----\n" // 4
         "---* *x****x ****---\n" // 5
         "---*  * o   * xx*---\n" // 6
         "---*  *oo*oH HxH*---\n" // 7
         "---*H H  *  o o**---\n" // 8
         "---*x  x** * ***----\n" // 9
         "---*x H***   *------\n" // 10
         "---*****-*****------\n" // 11
;

/*
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 29
                  g(): 319
                  depth: 30
                  Teleport card count: 1
                  Push card count: 4
                  Pull card count: 5

//0123456789
 "*x*oHo *\n" // 0
 "*x Ho  *\n" // 1
 "*     **\n" // 2
 "*Hh ****\n" // 3
 "* *x****\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: PtRQLpQDLDLLLUpLDLLRDRDLLDLUU (29)
lines: 23
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 25
                  g(): 344
                  depth: 26
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "**  o  *\n" // 0
 "*o* Hxx*\n" // 1
 "* Ho H**\n" // 2
 "*  h ***\n" // 3
 "**x ****\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: RtLLDDpURPRURQDQRURRPRDLD (25)
lines: 21
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
 "*h * o *\n" // 0
 "*  *oo**\n" // 1
 "*H H  **\n" // 2
 "*x  x***\n" // 3
 "*x H****\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: tDDLUURDpUDDpLLUUpQDpUDLLDLD (28)
lines: 21
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 31
                  g(): 322
                  depth: 33
                  Teleport card count: 1
                  Push card count: 4
                  Pull card count: 5

//0123456789
 "*h * xx*\n" // 0
 "*oH HxH*\n" // 1
 "*  o o**\n" // 2
 "* * ****\n" // 3
 "*   ****\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: DDRUUpUUpptDLDURRRLLUURQPRRURRUR (32)
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
 "** o  o*\n" // 0
 "* HohxH*\n" // 1
 "*  * ***\n" // 2
 "*  H ***\n" // 3
 "*x x****\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: ptDRRLRDQRLDpLLRDQRDLLQDDR (26)
lines: 21
--------------------------------------------------------------------------------





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
    initDecks(world, 20, 18, 4);
}

Level26::Level26()
: TilesetMovable()
, TilesetCardHandler(1) {

}

void Level26::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level26::drawTexts() noexcept {
    Tileset::drawTexts(24, LevelFactory::minimumPushCount[23]);
}
