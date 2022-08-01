#include <gtest/gtest.h>
#include "Maze.h"
#include "TestUtil.h"
#include "MazeStarV2.h"
#include "MazeStarV3.h"
#include "AStar.h"

using namespace TestUtil;

namespace {

}

TEST(SOLUTIONS, Level21cpp) {
    std::cout << "Positon 00\n";
    mazeArray = convert(
        //012345678901
                "x*H  *" // 0
                "xx H *" // 1
                "oH oh " // 2
                " **  *" // 3
                "**o  *" // 4
    );
    findWithCardIteration(0);

    std::cout << "Positon 10\n";
    mazeArray = convert(
        //012345678901
                "xx   *" // 0
                "*x  **" // 1
                "HH H o" // 2
                "   o  " // 3
                "* **ho" // 4
    );
    findWithCardIteration(0);

    std::cout << "Positon 01\n";
    mazeArray = convert(
        //012345678901
                " ***o*" // 0
                "x xHo*" // 1
                "  H o " // 2
                "x* * h" // 3
                "  H   " // 4
    );
    findWithCardIteration(0);

    std::cout << "Positon 11\n";
    mazeArray = convert(
        //012345678901
                "*** o*" // 0
                " x H  " // 1
                "x HH  " // 2
                "*x   *" // 3
                "*ho  o" // 4
    );
    findWithCardIteration(0);
}

TEST(SOLUTIONS, Level22cpp) {
    std::cout << "Positon 00\n";
    mazeArray = convert(
        //012345678901
                "* o oo" // 0
                "      " // 1
                "  ** x" // 2
                " H**H " // 3
                " x h x" // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 10\n";
    mazeArray = convert(
        //012345678901
                "*  x  " // 0
                " o  Hx" // 1
                "o **  " // 2
                " o**  " // 3
                "h H  x" // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 01\n";
    mazeArray = convert(
        //012345678901
                "* xx  " // 0
                "h  *H " // 1
                "ox ***" // 2
                "  H o*" // 3
                "*  oH " // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 11\n";
    mazeArray = convert(
        //012345678901
                "H*xx  " // 0
                "* *H  " // 1
                " * *h " // 2
                "*x H o" // 3
                "  o o*" // 4
    );
    findWithCardIteration(1);

}

TEST(SOLUTIONS, Level23cpp) {
    std::cout << "Positon 00\n";
    mazeArray = convert(
        //012345678901
                "*x* o " // 0
                "*  *h " // 1
                "    Ho" // 2
                "   oH*" // 3
                "*x*xH " // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 10\n";
    mazeArray = convert(
        //012345678901
                "*  H *" // 0
                "*oH   " // 1
                "x  H *" // 2
                "*x*o o" // 3
                "x  *h*" // 4
    );
    findWithCardIteration(2);

    std::cout << "Positon 01\n";
    mazeArray = convert(
        //012345678901
                "  o o*" // 0
                "o H x*" // 1
                "  H*H*" // 2
                "  ****" // 3
                " xx  h" // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 11\n";
    mazeArray = convert(
        //012345678901
                "**Ho o" // 0
                "* x*x " // 1
                "*Hx*  " // 2
                "**   h" // 3
                "  o  H" // 4
    );
    findWithCardIteration(1);

}

TEST(SOLUTIONS, Level24cpp) {
    std::cout << "Positon 00\n";
    mazeArray = convert(
        //012345678901
                " x o o" // 0
                "  H H " // 1
                " h**  " // 2
                "H *** " // 3
                " ox x*" // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 10\n";
    mazeArray = convert(
        //012345678901
                " H  o " // 0
                "  H  H" // 1
                " h**o " // 2
                "  ****" // 3
                "ox  xx" // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 01\n";
    mazeArray = convert(
        //012345678901
                "  o* h" // 0
                " o  H " // 1
                "oH*  x" // 2
                "  *** " // 3
                " x* Hx" // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 11\n";
    mazeArray = convert(
        //012345678901
                " *    " // 0
                " HoH x" // 1
                "   * h" // 2
                "  ** *" // 3
                "*ooHxx" // 4
    );
    findWithCardIteration(1);
}

TEST(SOLUTIONS, Level25cpp) {
    std::cout << "Positon 00\n";
    mazeArray = convert(
        //012345678901
                "o  *hx" // 0
                "*  xH " // 1
                "oo x  " // 2
                "  * H " // 3
                "  *H *" // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 10\n";
    mazeArray = convert(
        //012345678901
                "     o" // 0
                "x H o " // 1
                "x**H  " // 2
                "x  *Hh" // 3
                "*  * o" // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 01\n";
    mazeArray = convert(
        //012345678901
                "oH* **" // 0
                " ***  " // 1
                "H  o o" // 2
                "x   hH" // 3
                " x*x  " // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 11\n";
    mazeArray = convert(
        //012345678901
                " * *oo" // 0
                "H***ho" // 1
                "  x*  " // 2
                " H x H" // 3
                "*x    " // 4
    );
    findWithCardIteration(1);
}

TEST(SOLUTIONS, Level26cpp) {
    std::cout << "Positon 00\n";
    mazeArray = convert(
        //012345678901
                "x*oHo " // 0
                "x Ho  " // 1
                "     *" // 2
                "Hh ***" // 3
                " *x***" // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 10\n";
    mazeArray = convert(
        //012345678901
                "*  oh " // 0
                "o* Hxx" // 1
                " Ho H*" // 2
                "    **" // 3
                "*x ***" // 4
    );
    findWithCardIteration(0);

    std::cout << "Positon 01\n";
    mazeArray = convert(
        //012345678901
                "h * o " // 0
                "  *oo*" // 1
                "H H  *" // 2
                "x  x**" // 3
                "x H***" // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 11\n";
    mazeArray = convert(
        //012345678901
                "h * xx" // 0
                "oH HxH" // 1
                "  o o*" // 2
                " * ***" // 3
                "   *  " // 4
    );
    findWithCardIteration(1);
}

TEST(SOLUTIONS, Level27cpp) {

    std::cout << "Positon 00\n";
    mazeArray = convert(
        //012345678901
                "   ooH" // 0
                " o   h" // 1
                "***   " // 2
                " **HHx" // 3
                "*xx  *" // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 10\n";
    mazeArray = convert(
        //012345678901
                "*o x *" // 0
                "* HxH " // 1
                "h     " // 2
                " o*Hx*" // 3
                " o ** " // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 01\n";
    mazeArray = convert(
        //012345678901
                "o  * *" // 0
                " *x  h" // 1
                " H H o" // 2
                " H*  *" // 3
                "*xx o*" // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 11\n";
    mazeArray = convert(
        //012345678901
                " * h *" // 0
                "o x  H" // 1
                "  o***" // 2
                "HxH x*" // 3
                "  o * " // 4
    );
    findWithCardIteration(1);
}

TEST(SOLUTIONS, Level28cpp) {
    std::cout << "Positon 00\n";
    mazeArray = convert(
        //012345678901
                "* H  *" // 0
                "xx  H*" // 1
                " xH **" // 2
                "   oo*" // 3
                "** oh*" // 4
    );
    findWithCardIteration(0);

    std::cout << "Positon 10\n";
    mazeArray = convert(
        //012345678901
                "xx   *" // 0
                "*x  **" // 1
                "HH H o" // 2
                "   o  " // 3
                "* **ho" // 4
    );
    findWithCardIteration(0);

    std::cout << "Positon 01\n";
    mazeArray = convert(
        //012345678901
                " x****" // 0
                "*o   *" // 1
                "  h *o" // 2
                "o H H " // 3
                "  H xx" // 4
    );
    findWithCardIteration(0);

    std::cout << "Positon 11\n";
    mazeArray = convert(
        //012345678901
                "x**  *" // 0
                "x hoH " // 1
                "   H o" // 2
                "*  *xo" // 3
                "**H   " // 4
    );
    findWithCardIteration(0);
}

TEST(SOLUTIONS, Level29cpp) {
    std::cout << "Positon 00\n";
    mazeArray = convert(
        //012345678901
                " o x  " // 0
                "* HhH*" // 1
                " xHo *" // 2
                "x   **" // 3
                "o ****" // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 10\n";
    mazeArray = convert(
        //012345678901
                "* *x o" // 0
                " * x  " // 1
                " HH H*" // 2
                " ho **" // 3
                "xo ***" // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 01\n";
    mazeArray = convert(
        //012345678901
                "H hx x" // 0
                "   * H" // 1
                " H ***" // 2
                "ox o**" // 3
                "o  ***" // 4
    );
    findWithCardIteration(0);

    std::cout << "Positon 11\n";
    mazeArray = convert(
        //012345678901
                "*o * *" // 0
                "hH  xx" // 1
                "  H  *" // 2
                "Ho  **" // 3
                "o x***" // 4
    );
    findWithCardIteration(0);
}

TEST(SOLUTIONS, Level30cpp) {
    std::cout << "Positon 00\n";
    mazeArray = convert(
        //012345678901
                "*oH   " // 0
                "oxxh o" // 1
                "  **o " // 2
                " H** x" // 3
                "  H x*" // 4
    );
    findWithCardIteration(0);

    std::cout << "Positon 10\n";
    mazeArray = convert(
        //012345678901
                "* *x o" // 0
                " * x  " // 1
                " HH H*" // 2
                " ho **" // 3
                "xo ***" // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 01\n";
    mazeArray = convert(
        //012345678901
                "* H x " // 0
                " oo   " // 1
                "  **  " // 2
                "oH** x" // 3
                " h x  " // 4
    );
    findWithCardIteration(0);

    std::cout << "Positon 11\n";
    mazeArray = convert(
        //012345678901
                "*  H  " // 0
                "      " // 1
                "oo**H " // 2
                "h **ox" // 3
                " x  x " // 4
    );
    findWithCardIteration(1);
}

TEST(SOLUTIONS, Level31cpp) {

    std::cout << "Positon 00\n";
    mazeArray = convert(
        //012345678901
                "*  x**" // 0
                "o*  x*" // 1
                " HxH *" // 2
                "o H **" // 3
                " h  o " // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 10\n";
    mazeArray = convert(
        //012345678901
                "* xxo " // 0
                "*  *  " // 1
                "**H   " // 2
                "**h *x" // 3
                "HHo o " // 4
    );
    findWithCardIteration(0);


    std::cout << "Positon 01\n";
    mazeArray = convert(
        //012345678901
                "  o* *" // 0
                "* o o*" // 1
                "h   H*" // 2
                " xH***" // 3
                "x H  x" // 4
    );
    findWithCardIteration(1);


    std::cout << "Positon 11\n";
    mazeArray = convert(
        //012345678901
                "*o H H" // 0
                "*oh x " // 1
                "**  * " // 2
                "**oHxx" // 3
                " *    " // 4
    );
    findWithCardIteration(0);

}

TEST(SOLUTIONS, Level32cpp) {
    std::cout << "Positon 00\n";
    mazeArray = convert(
        //012345678901
                "    * " // 0
                "****xx" // 1
                "x    h" // 2
                "oHH o " // 3
                "Ho  * " // 4
    );
    findWithCardIteration(0);

    std::cout << "Positon 10\n";
    mazeArray = convert(
        //012345678901
                "o * * " // 0
                "oo***x" // 1
                "h  H  " // 2
                " H x  " // 3
                " H x* " // 4
    );
    findWithCardIteration(0);

    std::cout << "Positon 01\n";
    mazeArray = convert(
        //012345678901
                "o**** " // 0
                " o**xx" // 1
                "  H H " // 2
                "H oh  " // 3
                "   * x" // 4
    );
    findWithCardIteration(1);

    std::cout << "Positon 11\n";
    mazeArray = convert(
        //012345678901
                "h *H*x" // 0
                "o *** " // 1
                "oo **x" // 2
                "  H   " // 3
                "  H  x" // 4
    );
    findWithCardIteration(1);
}

TEST(SOLUTIONS, Level2cpp) {
    mazeArray = convert(
        //0123456789012345
         " ************** "   //0
         "*       x x    *"   //1
         "*    *****     *"   //2
         " *  *     *    *"   //3
         " * * *     **** "   //4
         " *  o           "   //5
         " * * *          "   //6
         " *  o*          "   //7
         " * * *          "   //8
         " *   *          "   //9
         "  ***   *****   "   //10
         "         h      "   //11
    );
    findWithCardIteration(0);
}

TEST(SOLUTIONS, Level4cpp) {
    mazeArray = convert(
        //0123456
         "  ***  "   // 0
         " *   * "   // 1
         " *   * "   // 2
         "*ho  * "   // 3
         "* o* x*"   // 4
         " *   x*"   // 5
         "  **  *"   // 6
         "    ** "   // 7
    );
    findWithCardIteration(0);
}

TEST(SOLUTIONS, Level5cpp) {
    mazeArray = convert(
        //012345
         "  *** "   // 0
         " * xx*"   // 1
         "* h  *"   // 2
         "* oo* "   // 3
         " *   *"   // 4
         " *   *"   // 5
         "  *** "   // 6
    );
    findWithCardIteration(0);
}

TEST(SOLUTIONS, Level6cpp) {
    mazeArray = convert(
        //012345
         "******"   // 0
         "*    *"   // 1
         "* *o *"   // 2
         "*  o *"   // 3
         " *   *"   // 4
         "*xxh* "   // 5
         " ***  "   // 6
    );
    findWithCardIteration(0);
}

TEST(SOLUTIONS, Level7cpp) {
    mazeArray = convert(
        //012345
         "  ****" // 0
         "***  *" // 1
         "*  o *" // 2
         "* *o *" // 3
         "*  o *" // 4
         "*x   *" // 5
         "*xxh**" // 6
    );
    findWithCardIteration(0);
}

TEST(SOLUTIONS, Level8cpp) {
    mazeArray = convert(
        //012345
         "x*    " // 0
         "x*****" // 1
         "x     " // 2
         " *  o " // 3
         " ***o " // 4
         " *  o " // 5
         "   h  " // 6
    );
    findWithCardIteration(0);
}

TEST(SOLUTIONS, Level9cpp) {
    mazeArray = convert(
        //012345
         "xxx **" // 0
         "x   o " // 1
         "x* *o " // 2
         "**  o " // 3
         " ***o " // 4
         " *  o " // 5
         " * h  " // 6
    );
    findWithCardIteration(0);
}

TEST(SOLUTIONS, Level10cpp) {
    mazeArray = convert(
        //012345
         "xxxxx " // 1
         " * *  " // 2
         "**** *" // 3
         "  o   " // 4
         "  o o " // 5
         " *o o*" // 6
         "   h *" // 7
    );
    findWithCardIteration(0);
}

TEST(SOLUTIONS, Level11cpp) {
    mazeArray = convert(
        //012345
         "xxx* *" // 1
         "x    *" // 2
         "x  * *" // 3
         "**  o " // 4
         "  o*o " // 5
         "  o o " // 6
         "* *h  " // 7
    );
    findWithCardIteration(0);
}

TEST(SOLUTIONS, Level12cpp) {
    mazeArray = convert(
        //012345
         "xxx   " // 1
         "x   * " // 2
         "x* o  " // 3
         "x ooo " // 4
         "* oho " // 5
         "*** * " // 6
         "  *   " // 7
    );
    findWithCardIteration(0);
}

TEST(SOLUTIONS, Level13cpp) {
    mazeArray = convert(
        //0123456
         "    ** " // 1
         "*   o  " // 2
         "* ***o " // 3
         "hxxx*  " // 4
         " xxx*o " // 5
         " ****  " // 6
         " o o o " // 7
         "       " // 8
    );
    findWithCardIteration(0);
}

/* program cannot solve it
TEST(SOLUTIONS, Level14cpp) {
    mazeArray = convert(
        //0123456
         "xxxxxxx" // 1
         "  x x  " // 2
         "**    *" // 3
         "  ooo  " // 4
         " oo oo " // 5
         "  o o  " // 6
         "   h * " // 7
         "     * " // 8
    );
    findWithCardIteration(0);
}
*/
