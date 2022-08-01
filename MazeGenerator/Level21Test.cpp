#include <gtest/gtest.h>
#include <chrono>
#include <math.h>
#include "TestUtil.h"
#include "MazeStar.h"
#include "MazeStarV2.h"
#include "MazeStarV3.h"
#include "AStar.h"

using namespace TestUtil;

namespace {
    [[maybe_unused]]Maze::MazeArray mazeArray_lv21_first_version = convert(
       //0123456789012
        "        * x" // 0
        "ho o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    [[maybe_unused]]Maze::MazeArray mazeArray_lv21_second_version = convert(
       //0123456789012
        "*    *  xx x" // 0
        "*    *    x " // 1
        "*  *o*o** * " // 2
        "* o oh      " // 3
        "* o o*o * * " // 4
        "   o *    x " // 5
        "     * x xx " // 6
    );
    [[maybe_unused]]Maze::MazeArray mazeArray_lv21_third_version = convert(
       //0123456789012
        "       *****" // 0
        "       *   *" // 1
        "       *   x" // 2
        "       *o* *" // 3
        "      ho    " // 4
        "       * *  " // 5
        "            " // 6
    );
}

TEST(Level21, DISABLED_ThirdDesign) {
    std::string steps;
    mazeArray = mazeArray_lv21_third_version;
    next_phase_test = convert(
       //0123456789012
        "*    *  x  x" // 0
        "*    *   hx " // 1
        "* o*o*o** * " // 2
        "*           " // 3
        "*o   *o * * " // 4
        "   oo*    x " // 5
        "     * x xx " // 6
    );
    steps += doNextPhaseBothDirection("P1", 12, 12);


    mazeArray = mazeArray_lv21_third_version;
    std::cout << "moves: " << steps.length() << "\n";
    std::cout << "pushes: " << AStarV2::pushNumberFromSteps(steps) << "\n";
    testSolution(steps);
}

TEST(Level21, DISABLED_SecondDesign) {
    std::string steps;
    mazeArray = mazeArray_lv21_second_version;
    next_phase_test = convert(
       //0123456789012
        "*    *  x  x" // 0
        "*    *   hx " // 1
        "* o*o*o** * " // 2
        "*           " // 3
        "*o   *o * * " // 4
        "   oo*    x " // 5
        "     * x xx " // 6
    );
    steps += doNextPhaseBothDirection("P1", 12, 12);

    mazeArray = next_phase_test;
    next_phase_test = convert(
       //0123456789012
        "*    *  x  x" // 0
        "*    *    x " // 1
        "*  *o* ** * " // 2
        "*     oho   " // 3
        "*o   *o * * " // 4
        "   oo*    x " // 5
        "     * x xx " // 6
    );
    steps += doNextPhaseBothDirection("P1.5", 25, 25);

    mazeArray = next_phase_test;
    next_phase_test = convert(
       //0123456789012
        "*    *  x  x" // 0
        "*    *   h  " // 1
        "*  *o* ** * " // 2
        "*   o       " // 3
        "*o   *o * * " // 4
        "   oo*    x " // 5
        "     * x xx " // 6
    );
    steps += doNextPhaseBothDirection("P2", 12, 12);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
         "*    *  x  x" // 0
         "*    *      " // 1
         "*  *o* ** * " // 2
         "*           " // 3
         "*o   *o * * " // 4
         "   oo*   hx " // 5
         "     * x  x " // 6
    );
    steps += doNextPhaseBothDirection("P3", 24, 24);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
         "*    *  x  x" // 0
         "*    *      " // 1
         "*  * * ** * " // 2
         "*           " // 3
         "*o   *o * * " // 4
         "   oo*   h  " // 5
         "     * x  x " // 6
    );
    steps += doNextPhaseBothDirection("P4", 23, 23);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
         "*    *  x  x" // 0
         "*    *      " // 1
         "*  * * ** * " // 2
         "*           " // 3
         "*    *o * * " // 4
         "   oo*      " // 5
         "     * x h  " // 6
    );
    steps += doNextPhaseBothDirection("P5", 25, 25);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
         "*    *  x h " // 0
         "*    *      " // 1
         "*  * * ** * " // 2
         "*           " // 3
         "*    *  * * " // 4
         "   oo*      " // 5
         "     * x    " // 6
    );
    steps += doNextPhaseBothDirection("P6", 19, 19);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
         "*    *   h  " // 0
         "*    *      " // 1
         "*  * * ** * " // 2
         "*           " // 3
         "*    *  * * " // 4
         "   o *      " // 5
         "     * x    " // 6
    );
    steps += doNextPhaseBothDirection("P7", 21, 21);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
         "*    *      " // 0
         "*    *      " // 1
         "*  * * ** * " // 2
         "*           " // 3
         "*    *  * * " // 4
         "     * h    " // 5
         "     *      " // 6
    );
    steps += doNextPhaseBothDirection("P8", 22, 22);

    mazeArray = mazeArray_lv21_second_version;
    std::cout << "moves: " << steps.length() << "\n";
    std::cout << "pushes: " << AStarV2::pushNumberFromSteps(steps) << "\n";
    testSolution(steps);
}

TEST(Level21, DISABLED_FirstDesign) {

    std::string steps;
    mazeArray = mazeArray_lv21_first_version;
    next_phase_test = convert(
        //0123456789012
         "        * x" // 0
         " o o o*  x " // 1
         "**o*     h " // 2
         " o o *  *x " // 3
         "  o o*x  x*" // 4
         " o o * x x " // 5
         "     *x  xx" // 6
    );
    //steps += doNextPhase("P1", 13);
    steps += doNextPhaseBothDirection("P1", 8, 8);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
        "        * x" // 0
        " o o  *  x " // 1
        "**o*       " // 2
        " o o *  *x " // 3
        "  o o*xh x*" // 4
        " o o *   x " // 5
        "     *x  xx" // 6
    );
    //steps += doNextPhase("P2", 21);
    steps += doNextPhaseBothDirection("P2", 12, 12);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
        "        * x" // 0
        " o    *  x " // 1
        "**o*       " // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o *   x " // 5
        "     * h xx" // 6
    );
    //steps += doNextPhase("P3", 39);
    steps += doNextPhaseBothDirection("P3", 18, 18);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
        "        * x" // 0
        "      *  x " // 1
        "**o*       " // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o *   x " // 5
        "     *  h x" // 6
    );
    //steps += doNextPhase("P4", 46);
    steps += doNextPhaseBothDirection("P4", 22, 22);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
        "        * x" // 0
        "  o o *  x " // 1
        "** *h      " // 2
        " o   *  *x " // 3
        " o   *x  x*" // 4
        " o oo*   x " // 5
        "     *    x" // 6
    );
    steps += doNextPhaseBothDirection("P5", 12, 12);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
        "        * x" // 0
        "  o   *  x " // 1
        "** *       " // 2
        " o   * h*x " // 3
        " o   *xo x*" // 4
        " o oo*   x " // 5
        "     *    x" // 6
    );
    steps += doNextPhaseBothDirection("P6", 13, 13);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
        "        * x" // 0
        " ho   *  x " // 1
        "** *       " // 2
        " o   *  *x " // 3
        " o   *x  x*" // 4
        " o oo*   x " // 5
        "     *     " // 6
    );
    steps += doNextPhaseBothDirection("P7", 13, 13);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
        "        * x" // 0
        "      *  x " // 1
        "** *       " // 2
        " o   *  *x " // 3
        " o   *   x*" // 4
        " o oo*h  x " // 5
        "     *     " // 6
    );
    steps += doNextPhaseBothDirection("P8", 15, 15);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
        "        * x" // 0
        "      *  x " // 1
        "** * ho    " // 2
        " o   *  *x " // 3
        " o   *   x*" // 4
        " o o *   x " // 5
        "     *     " // 6
    );
    steps += doNextPhaseBothDirection("P9", 17, 17);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
        "        * x" // 0
        "      *  x " // 1
        "** *       " // 2
        " o   *  *x " // 3
        " o   *   x*" // 4
        " o o *  h  " // 5
        "     *     " // 6
    );
    steps += doNextPhaseBothDirection("P10", 9, 9);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
        "        * x" // 0
        "    ho*  x " // 1
        "** *       " // 2
        " o   *  *x " // 3
        " o   *   x*" // 4
        " o   *     " // 5
        "     *     " // 6
    );
    steps += doNextPhaseBothDirection("P11", 15, 15);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
        "        * x" // 0
        "      *  x " // 1
        "** *       " // 2
        " o   *  *x " // 3
        " o   *  h *" // 4
        " o   *     " // 5
        "     *     " // 6
    );
    steps += doNextPhaseBothDirection("P12", 12, 12);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
        "        * x" // 0
        "      *  x " // 1
        "** *       " // 2
        "     *  *  " // 3
        " o   *   h*" // 4
        " o   *     " // 5
        "     *     " // 6
    );
    steps += doNextPhaseBothDirection("P13", 29, 29);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
        "        * x" // 0
        "      *    " // 1
        "** *     h " // 2
        "     *  *  " // 3
        "     *    *" // 4
        " o   *     " // 5
        "     *     " // 6
    );
    steps += doNextPhaseBothDirection("P14", 24, 24);

    mazeArray = next_phase_test;
    next_phase_test = convert(
        //0123456789012
        "        *  " // 0
        "      *   h" // 1
        "** *       " // 2
        "     *  *  " // 3
        "     *    *" // 4
        "     *     " // 5
        "     *     " // 6
    );
    steps += doNextPhaseBothDirection("P15", 23, 23);

    std::cout << std::string(40, '-') << "solution test" << std::string(40, '-') << "\n";
    mazeArray = mazeArray_lv21_first_version;
    std::cout << "Steps: " << steps.length() << "\n";
    testSolution(steps);
}
