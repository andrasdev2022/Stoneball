#include "Breadthfirst.h"
#include <gtest/gtest.h>
#include <chrono>
#include "MazeStarV3.h"
#include "AStar.h"
#include <math.h>
#include "stopwatch.h"
#include "TestUtil.h"
#include "MazeStarV2.h"

extern StopWatch stopWatch;
using namespace std::chrono;
using namespace TestUtil;

namespace {
    int usedTeleportCards;
    int usedPullCards;
    int usedPushCards;

    // Level19.cpp
    // findDetails(0, 6, 2);
    [[maybe_unused]]Maze::MazeArray mazeArray_lv19_first_part = convert(
       //0123456789012
        " x *  o *" // 0
        "   *    *" // 1
        "*  HHHH *" // 2
        "   *    *" // 3
        "*  * o  h" // 4
        "*x *    *" // 5
    );

    // findDetails(0, 6, 5);
    [[maybe_unused]]Maze::MazeArray mazeArray_lv19_second_part = convert(
       //012345678901
        "*    x ****" // 0
        "* Ho  o****" // 1
        "*x*** Hh***" // 2
        "****   ****" // 3
        "****  o****" // 4
        "*****x ****" // 5
    );

    [[maybe_unused]]Maze::MazeArray mazeArray_lv19_full = convert(
       //012345678901
        "    x *  o *" // 0
        " Ho   *    *" // 1
        "x***  HHHH *" // 2
        "***   *    *" // 3
        "***   * o  h" // 4
        "****x *    *" // 5
    );

    // Level20.cpp
    // findDetails(1, 6, 5); push count: 19
    [[maybe_unused]]Maze::MazeArray mazeArray_lv20_first_part = convert(
    //0123456789
     "**xxx " // 0
     "* H *h" // 1
     "*oHHo " // 2
     "*H H  " // 3
     " *   o" // 4
     );

    [[maybe_unused]]Maze::MazeArray mazeArray_lv20_second_part = convert(
   //findDetails(1, 3, 4); push count: 25
   //0123456789
    "h  Hx " // 0
    "H    o" // 1
    "  xHHH" // 2
    "   Ho " // 3
    "    xo" // 4
     );

    [[maybe_unused]]Maze::MazeArray mazeArray_lv20_full = convert(
   //findDetails(2, 9, 9);
   //0123456789012
    "**xxx     Hx " // 0
    "* H *hHH    o" // 1
    "*oHHo *  xHHH" // 2
    "*H H  *   Ho " // 3
    " *   o*    xo" // 4
     );


    [[maybe_unused]]Maze::MazeArray big_test = convert(
   //0123456789012
    "****     h***" // 0
    "***  oo   ***" // 1
    "**  o oo  ***" // 2
    "***  o   ****" // 3
    "***      ****" // 4
    "xxx       xxx" // 5
      );

    [[maybe_unused]]Maze::MazeArray sokoban_touch_1 = convert(
   //0123456789012
    "*****   " // 0
    "*xx **  " // 1
    "*    ***" // 2
    "*      *" // 3
    "* o o  *" // 4
    "***h****" // 5
    "* o o * " // 6
    "*     * " // 7
    "***    *" // 8
    "  * xx* " // 9
    "  ***** " // 0
      );

    //[[maybe_unused]]Maze::MazeArray mazeArray = mazeArray_lv20_second_part;
/*
                             Push count: 13
                              g(): 267
                              Teleport card count: 1
                              Push card count: 5
                              Pull card count: 2

            //0123456789
             "***xx* *\n" // 0
             "** H *h*\n" // 1
             "* oHHo *\n" // 2
             "*   H  *\n" // 3
             "*      *\n" // 4
----------------------------------------------------------
            //0123456789
             "***xx* *\n" // 0
             "** H *h*\n" // 1
             "* oHHo *\n" // 2
             "* H H  *\n" // 3
             "*      *\n" // 4
            Depth: 24
            search: 35.2479 seconds
            extend: 15.9637 seconds
            calcH: 0.025195 seconds
            updateTreeG: 2.87775 seconds
            Extend count: 7118495
                              Push count: 12
                              g(): 284
                              Teleport card count: 1
                              Push card count: 6
                              Pull card count: 2

            ----------------------------------------------------------
            Depth: 46
            search: 2.22641 seconds
            extend: 0.407159 seconds
            calcH: 0.0006 seconds
            updateTreeG: 0.184305 seconds
            Extend count: 781024
                              Push count: 19
                              g(): 393
                              Teleport card count: 1
                              Push card count: 6
                              Pull card count: 5

            //0123456789
             "***xxx *\n" // 0
             "** H *h*\n" // 1
             "**oHHo *\n" // 2
             "**H H  *\n" // 3
             "* *   o*\n" // 4
            ----------------------------------------------------------
            Depth: 92
            search: 232.188 seconds
            extend: 0 seconds
            calcH: 0.078069 seconds
            updateTreeG: 63.3625 seconds
            Extend count: 49277451
                              Push count: 33
                              g(): 546
                              Teleport card count: 0
                              Push card count: 5
                              Pull card count: 2

            //0123456789
             "*** x *  o **\n" // 0
             "***   *    **\n" // 1
             "***   HHHH **\n" // 2
             "***   *    **\n" // 3
             "****  * o  h*\n" // 4
             "****x *    **\n" // 5
             ----------------------------------------------------------
            Depth: 104
            search: 363.403 seconds
            extend: 0 seconds
            calcH: 0.3255 seconds
            updateTreeG: 100.855 seconds
            Extend count: 61994196
                              Push count: 38
                              g(): 623
                              Teleport card count: 0
                              Push card count: 6
                              Pull card count: 2

            //0123456789
             "*** x *  o **\n" // 0
             "***   *    **\n" // 1
             "****  HHHH **\n" // 2
             "***   *    **\n" // 3
             "****  * o  h*\n" // 4
             "****x *    **\n" // 5



            */
}

TEST(BREADTHFIRST, DISABLED_Test) {

    BreadthFirst breadthFirst;
    std::result_of<decltype(&high_resolution_clock::now)()>::type now1_;
    Maze maze;
/*
    Maze::MazeArray mazeArray
          {{
            {'x', '*', ' ', 'x', 'H', '*'},
            {' ', '*', 'h', ' ', ' ', '*'},
            {' ', ' ', 'o', 'H', ' ', '*'},
            {'*', ' ', ' ', 'o', 'o', '*'},
            {' ', '*', ' ', 'x', ' ', '*'},
          }};
*/
/*
 * isSolvable test
    Maze::MazeArray mazeArray
          {{
            {'x', 'H', ' ', 'x', 'x'},
            {'H', ' ', ' ', 'H', ' '},
            {' ', 'H', 'o', ' ', ' '},
            {'H', 'o', ' ', ' ', 'o'},
            {' ', 'h', 'H', ' ', ' '},
          }};
*/
    maze = mazeArray;
    //maze.setCardsAmount(1, 4, 4);
    maze.setCardsAmount(teleportCount, pushCount, pullCount);
    breadthFirst << maze;
    breadthFirst.setMaximumDepth(depthLimit);
    now1_ = high_resolution_clock::now();
    ASSERT_FALSE(breadthFirst.search(false));
    auto now2 = high_resolution_clock::now();

    EXPECT_LE(duration_cast<milliseconds>(now2 - now1_).count(), 4'000);
}
/*
TEST(HASH, DISABLED_Test) {
    MazeStar maze1;
    Maze::MazeArray mazeArray1 = convert(
        "** x *  o *" // 0
        "**   *    *" // 1
        "***  HHHH *" // 2
        "**   *    *" // 3
        "***  *ho   " // 4
        "***x *    *" // 5
    );
    maze1 = mazeArray1;
    maze1.setCardsAmount(0, 6, 0);
    std::cout << maze1.hashNumber() << "\n";

    MazeStar maze2;
    Maze::MazeArray mazeArray2 = convert(
        "** x *  o *" // 0
        "**   *    *" // 1
        "***  HHHH *" // 2
        "**   *    *" // 3
        "***  *ho   " // 4
        "***x *    *" // 5
    );
    maze2 = mazeArray2;
    maze2.setCardsAmount(0, 6, 2);
    std::cout << maze2.hashNumber() << "\n";
    EXPECT_TRUE(Math::abs(maze1.hashNumber() - maze2.hashNumber()) > 1e-7);
    EXPECT_NE(maze1.hash(), maze2.hash());
    EXPECT_TRUE(Math::abs(maze1.hash() - maze2.hash()) > 1e-7);

    constexpr double numericalSystem = 1.6;
    int y = 0;
    int x = 0;
    double powX = pow(numericalSystem, 2. + y * mazeWidth + x);
    double powx = pow(numericalSystem, y * mazeWidth + x);
    double fieldValue = maze1.hashNumber() - floor(maze1.hashNumber() / powX) * powX;
    std::cout << powX << ", " << powx << "\n";
    std::cout << floor(fieldValue / powx) << "\n";
}
*/

double floorToDecimalDigit(double value, int num) {
    return trunc(value * pow(10., num)) / pow(10., num);
}

TEST(DOUBLE, DISABLED_Test) {
    constexpr double numericalSystem = 6;
    double valuePosition = 0.;
    double convertedValue = 0.;
    double numberOfConversion = 21;
    for(valuePosition = 0.; valuePosition < numberOfConversion; valuePosition += 1.) {
        convertedValue += pow(numericalSystem, valuePosition) * (valuePosition - floor(valuePosition / numericalSystem) * numericalSystem);
    }

    std::cout << "stored format: " << convertedValue << "\n";

    for(valuePosition = 0.; valuePosition < numberOfConversion; valuePosition += 1.) {
        double higherPart = pow(numericalSystem, valuePosition + 1.);
        double lowerPart = pow(numericalSystem, valuePosition);
        // we will store < 0 digits, we need a special floor
        int decimalDigit = 0;
        double fieldValue = floorToDecimalDigit((convertedValue - trunc(convertedValue / higherPart) * higherPart) / lowerPart, decimalDigit);
        double originalValue = (valuePosition - floor(valuePosition / numericalSystem) * numericalSystem);
        std::cout << "fieldValue[" << (int)valuePosition << "]: " << fieldValue << ", originalValue: " << (int)originalValue << "\n";
        ASSERT_EQ((int)fieldValue, (int)originalValue);
    }
}

bool findBest(int teleportCards) noexcept {
    for(int cards = 0; cards <= 9; ++cards) {
        AStar<MazeStar> aStar;
        MazeStar maze;
        maze = mazeArray;
        maze.setCardsAmount(teleportCards, cards, cards);
        aStar << maze;
        aStar.setMaximumDepth(depthLimit);
        std::cout << "Test with " <<  cards << " cards each\n";
        if(aStar.search(false, false)) {
            AStar<MazeStar> aStar;
            MazeStar maze;
            maze = mazeArray;
            maze.setCardsAmount(teleportCards, cards, cards);
            aStar << maze;
            aStar.setMaximumDepth(depthLimit);
            aStar.search(true, true);
            usedTeleportCards = teleportCards - aStar.solution()->teleportCardCount();
            usedPullCards = cards - aStar.solution()->pullCardCount();
            usedPushCards = cards - aStar.solution()->pushCardCount();
            aStar.printFirstState();
            return true;
        }
    }
    return false;
}

void findDetails(int teleportCards, int pushCards, int pullCards) noexcept {
    AStar<MazeStar> aStar;
    MazeStar maze;
    maze = mazeArray;
    maze.setCardsAmount(teleportCards, pushCards, pullCards);
    aStar << maze;
    aStar.setMaximumDepth(depthLimit);
    if(aStar.search(true, true)) {
        //aStar.printFirstState();
        aStar.printStates();
    }
}

void find(int teleportCards, int pushCards, int pullCards) noexcept {
    AStar<MazeStar> aStar;
    MazeStar maze;
    maze = mazeArray;
    maze.setCardsAmount(teleportCards, pushCards, pullCards);
    aStar << maze;
    aStar.setMaximumDepth(depthLimit);
    aStar.setTimeout(180e6);
    if(aStar.search(true, false)) {
        aStar.printFirstState();
    }
}

TEST(ASTAR, NextLevelTest) {
    srand(time(0));
    //if(findBest(1)) findDetails(usedTeleportCards, usedPushCards, usedPullCards);
    //findBest(1);

    mazeArray = mazeArray_lv20_first_part;
    findDetails(1, 6, 5);
    //findBest(0);
    //find(0, 0, 0);
}

[[maybe_unused]]Maze::MazeArray next_level_test = convert(
   //0123456789012
    "*****     ***" // 0
    "**xx  o o  **" // 1
    "**x  o o o h*" // 2
    "*xxx  o o  **" // 3
    "**x**     ***" // 4
    "*************" // 5
  );

TEST(ASTART, DISABLED_BruteForce) {
    AStarV2 aStar;
    MazeStarV2 maze;
    maze = next_level_test;
    maze.setCardsAmount(0, 0, 0);
    aStar << maze;
    aStar.setMaximumDepth(108);
    aStar.setTimeout(180e6);
    ASSERT_TRUE(aStar.search(false, false));
}

TEST(ASTART, DISABLED_IterativeDesign) {

    std::string steps;
    mazeArray = next_level_test;
    next_phase_test = convert(
    //0123456789012
     "*****     ***" // 0
     "**xx  o o  **" // 1
     "**x  o o o  *" // 2
     "*xx h   o  **" // 3
     "**x**     ***" // 4
     "*************" // 5
    );
    steps += doNextPhase("P1", 11);

    mazeArray = next_phase_test;
    next_phase_test = convert(
    //0123456789012
     "*****     ***" // 0
     "**x h   o  **" // 1
     "**     o o  *" // 2
     "*xx     o  **" // 3
     "**x**     ***" // 4
     "*************" // 5
    );
    steps += doNextPhase("P2", 19);

    mazeArray = next_phase_test;
    next_phase_test = convert(
    //0123456789012
     "*****     ***" // 0
     "** h       **" // 1
     "**     o o  *" // 2
     "*xx     o  **" // 3
     "**x**     ***" // 4
     "*************" // 5
    );
    steps += doNextPhase("P3", 18);

    mazeArray = next_phase_test;
    next_phase_test = convert(
    //0123456789012
     "*****     ***" // 0
     "**         **" // 1
     "**     o o  *" // 2
     "*x h       **" // 3
     "**x**     ***" // 4
     "*************" // 5
    );
    steps += doNextPhase("P4", 17);

    mazeArray = next_phase_test;
    next_phase_test = convert(
    //0123456789012
     "*****     ***" // 0
     "**         **" // 1
     "**       o  *" // 2
     "* h        **" // 3
     "**x**     ***" // 4
     "*************" // 5
    );
    steps += doNextPhase("P5", 20);

    mazeArray = next_phase_test;
    next_phase_test = convert(
    //0123456789012
    "*****     ***" // 0
    "**         **" // 1
    "**          *" // 2
    "* h        **" // 3
    "** **     ***" // 4
    "*************" // 5
    );
    steps += doNextPhase("P6", 23);

    mazeArray = next_level_test;
    AStar<MazeStar> aStar;
    MazeStar maze;
    maze = mazeArray;
    aStar << maze;
    std::cout << "Steps: " << steps << "\n";
    ASSERT_TRUE(aStar.testSteps(steps));
}
