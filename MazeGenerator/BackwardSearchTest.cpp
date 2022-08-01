#include <gtest/gtest.h>
#include "Maze.h"
#include "TestUtil.h"
#include "MazeStarV2.h"
#include "MazeStarV3.h"
#include "AStar.h"

using namespace TestUtil;

namespace {

}
TEST(BACKWARDS, DISABLED_AStar_Search) {
    std::string invertedSteps;
    mazeArray = convert(
        //0123456789012
         "        * x" // 0
         " o o o*h x " // 1
         "**o* o    x" // 2
         " o o *  *x " // 3
         "  o o*x  x*" // 4
         " o o * x x " // 5
         "     *x  xx" // 6
    );

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
    invertedSteps += doNextPhaseBackward("P1", 12);
    std::cout << invertedSteps << "\n";
    std::string steps = AStar<MazeStarV2>::convertBackwardSteps(invertedSteps);
    std::cout << steps  << "\n";

    //Maze::MazeArray temp = mazeArray;
    //mazeArray = next_phase_test;
    //next_phase_test = temp;
    //std::swap(mazeArray, next_phase_test);

    testSolution(steps);
}

TEST(BOTHDIRECTION, DISABLED_AStar_Search) {
    mazeArray = convert(
        //0123456789012
         "        * x" // 0
         " o o o*h x " // 1
         "**o* o    x" // 2
         " o o *  *x " // 3
         "  o o*x  x*" // 4
         " o o * x x " // 5
         "     *x  xx" // 6
    );

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

/*
    next_phase_test = convert(
        //0123456789012
         "     h  * x" // 0
         " o o o*  x " // 1
         "**o*       " // 2
         " o o *  *x " // 3
         "  o o*x  x*" // 4
         " o o * x x " // 5
         "     *x  xx" // 6
    );
*/
    constexpr int maxDepth = 32;
    if(1){
        stopWatch.reset();
        std::cout << std::string(40, '-') << "AStar forward direction" << std::string(40, '-') << "\n";
        AStarV2 aStar0;
        MazeStarV2 maze0;
        maze0 = mazeArray;
        maze0.setCardsAmount(0, 0, 0);
        aStar0 << maze0;
        aStar0.setMaximumDepth(maxDepth);
        aStar0.setTimeout(180e6);
        EXPECT_TRUE(aStar0.search(true, false));
        auto steps = aStar0.stepString();
        std::cout << "steps: " << steps << ", " << steps.length() << "\n";
        testSolution(steps);
    }
    if(1){
        stopWatch.reset();
        std::cout << std::string(40, '-') << "AStar search backward direction" << std::string(40, '-') << "\n";
        AStarV2 aStar;
        MazeStarV2 mazeEnd;
        mazeEnd = next_phase_test;
        mazeEnd.setMaximumHoleCount(mazeEnd.holeCount() + 2);
        mazeEnd.setCardsAmount(0, 0, 0);
        aStar <<= mazeEnd;
        aStar.setDirection(AICommon::Direction::BACKWARD);
        aStar.setMaximumDepthBackward(maxDepth);
        aStar.setTimeout(180e6);
        EXPECT_TRUE(aStar.search(true, false));
        auto steps = aStar.stepString();
        std::cout << "steps: " << steps << ", " << steps.length() << "\n";
        testSolution(AStarV2::convertBackwardSteps(steps));
    }
    if(1){
        stopWatch.reset();
        std::cout << std::string(40, '-') << "AStar search both directions" << std::string(40, '-') << "\n";
        AStarV2 aStar;
        MazeStarV2 maze;
        maze = mazeArray;
        MazeStarV2 mazeEnd;
        mazeEnd = next_phase_test;
        maze.setCardsAmount(0, 0, 0);
        mazeEnd.setMaximumHoleCount(mazeEnd.holeCount() + 2);
        mazeEnd.setCardsAmount(0, 0, 0);
        aStar << maze;
        aStar <<= mazeEnd;
        aStar.setDirection(AICommon::Direction::BOTH);
        aStar.setMaximumDepth(18); // IT'S NOT FAIR, THIS PARAMETER IS DIFFERENT
        aStar.setMaximumDepthBackward(18); // IT'S NOT FAIR, THIS PARAMETER IS DIFFERENT
        aStar.setTimeout(180e6);
        EXPECT_TRUE(aStar.search(true, false));
        auto steps = aStar.stepString();
        std::cout << "steps: " << steps << ", " << steps.length() << "\n";
        testSolution(steps);
    }
}

TEST(BACKWARDS, AStar_PushAlteration) {
    mazeArray = convert(
                //0123456789
                 "x*  hx" // 0
                 " *  H " // 1
                 " oH o*" // 2
                 " o    " // 3
                 "H *  x" // 4
                );
    {
        AStar<MazeStar> aStar;
        MazeStar maze;
        maze = mazeArray;
        maze.setCardsAmount(0, 2, 2);

        aStar << maze;
        aStar.setMaximumDepth(depthLimit);
        aStar.setDirection(AICommon::Direction::FORWARD);
        aStar.setTimeout(1000e6);
        ASSERT_TRUE(aStar.search(true, true));
        //aStar.printStates();
    }
    std::cout << std::string(80, '-') << "\n";
    {
        AStar<MazeStar> aStar;
        MazeStar maze;
        maze = mazeArray;
        maze.setCardsAmount(0, 2, 2);

        aStar << maze;
        aStar.setMaximumDepth(depthLimit);
        aStar.setDirection(AICommon::Direction::FORWARD);
        aStar.setTimeout(1000e6);
        aStar.enablePushAlteration();
        ASSERT_TRUE(aStar.search(true, true));
        //aStar.printStates();
    }
}
