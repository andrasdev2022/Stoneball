#include "Level21.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level21::init(WorldUPtr& world) noexcept {
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
    L"---*************----\n" // 0
     "---*x*H  *xx   *----\n" // 1
     "---*xx H **x  ***---\n" // 2
     "---*oH o  HH H o*---\n" // 3
     "---* **  *   o  *---\n" // 4
     "---***o  ** ** o*---\n" // 5
     "---* ***o**** o**---\n" // 6
     "---*x xHo* x H  *---\n" // 7
     "---*  H o x HH  *---\n" // 8
     "---*x* *  *x   **---\n" // 9
     "---*  H   *ho  o*---\n" // 10
     "---**************---\n" // 11
;

/*
position: 00
                  Solution(s)
                  First State:
                  Push count: 27
                  g(): 316
                  depth: 27
                  Teleport card count: 0
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "*x*H  **\n" // 0
 "*xx H **\n" // 1
 "*oH oh *\n" // 2
 "* **  **\n" // 3
 "***o  **\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: PLUPUqLLUULLLDRRqPRDRULLLU (26)
lines: 18

position: 10
                  Solution(s)
                  First State:
                  Push count: 29
                  g(): 261
                  depth: 30
                  Teleport card count: 0
                  Push card count: 4
                  Pull card count: 4

//0123456789
 "*xx   **\n" // 0
 "**x  ***\n" // 1
 "*HH H o*\n" // 2
 "*   o  *\n" // 3
 "**h** o*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: RUUDpLLULDqpLqDULLULURULLLUUL (29)
lines: 22

position: 01

                  Solution(s)
                  First State:
                  Push count: 25
                  g(): 315
                  depth: 26
                  Teleport card count: 0
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "* ***o**\n" // 0
 "*x xHo**\n" // 1
 "*  H o *\n" // 2
 "*x* * h*\n" // 3
 "*  H   *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: QLDLQLQLQLqURLULLRULULLLD (25)
lines: 21

position: 11
                  Solution(s)
                  First State:
                  Push count: 25
                  g(): 286
                  depth: 26
                  Teleport card count: 0
                  Push card count: 4
                  Pull card count: 5

//0123456789
 "**** o**\n" // 0
 "* x H  *\n" // 1
 "*x HH  *\n" // 2
 "**x   **\n" // 3
 "**ho  o*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: RQULqLLpULQDLqLUUULDLRLUL (25)
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
    initDecks(world, 19, 17, 0);
}

Level21::Level21()
: TilesetMovable()
, TilesetCardHandler(1) {

}

void Level21::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level21::drawTexts() noexcept {
    Tileset::drawTexts(19, LevelFactory::minimumPushCount[18]);
}
