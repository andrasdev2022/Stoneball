#include <gtest/gtest.h>
#include "Maze.h"
#include "TestUtil.h"
#include "MazeStarV2.h"
#include "MazeStarV3.h"
#include "AStar.h"

using namespace TestUtil;

namespace {

}

TEST(BACKWARDS, StepLeft_01_Empty_Tile) {
    mazeArray = convert(
       //0123456789012
        " h      * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);

    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();
    Maze::MazeArray result = convert(
       //0123456789012
        "h       * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze.stepLeftBackward());
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), 'l');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 0);
    ASSERT_EQ(maze.heroY(), 0);
    ASSERT_EQ(pushCounter, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepLeft_02_Blocked_Hero) {
    mazeArray = convert(
       //0123456789012
        "h       * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    ASSERT_FALSE(maze.stepLeftBackward());

    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " oooho*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    maze = mazeArray;
    ASSERT_FALSE(maze.stepLeftBackward());

    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " o o o*  xh" // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    maze = mazeArray;
    ASSERT_FALSE(maze.stepLeftBackward());
}

TEST(BACKWARDS, StepLeft_03_Pull_Stone_Left) {
    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        "  ho o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();

    Maze::MazeArray result = convert(
       //0123456789012
        "        * x" // 0
        " ho  o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze.pullStoneLeftBackward());
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), '5');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 1);
    ASSERT_EQ(maze.heroY(), 1);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepLeft_03_Pull_Card_Required) {
    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " oho o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();

    Maze::MazeArray result = convert(
       //0123456789012
        "        * x" // 0
        "oh o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_FALSE(maze.stepLeftBackward());
    ASSERT_FALSE(maze == result);

    int teleport = 0, push = 0, pull = 1;
    maze.setMaximumCardsAmount(teleport, push, pull);
    ASSERT_TRUE(maze.stepLeftBackward());
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), 'P');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 1);
    ASSERT_EQ(maze.heroY(), 1);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount, maze.pushCardCount());
    ASSERT_EQ(pullCardCount + 1, maze.pullCardCount());
}

TEST(BACKWARDS, StepLeft_04_Hero2_Pull) {
    mazeArray = convert(
       //0123456789012
        "  Hh    * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    ASSERT_FALSE(maze.stepLeftBackward());

    int teleport = 0, push = 0, pull = 1;
    maze.setMaximumCardsAmount(teleport, push, pull);
    ASSERT_FALSE(maze.stepLeftBackward());
}

TEST(BACKWARDS, StepLeft_04_Hero2_Push) {
    mazeArray = convert(
       //0123456789012
        "   hH   * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();
    ASSERT_FALSE(maze.pullHero2LeftBackward());

    int teleport = 0, push = 1, pull = 0;
    maze.setMaximumCardsAmount(teleport, push, pull);
    ASSERT_TRUE(maze.pullHero2LeftBackward());

    Maze::MazeArray result = convert(
       //0123456789012
        "  hH    * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), '1');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 2);
    ASSERT_EQ(maze.heroY(), 0);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount + 1, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepLeft_04_Hero2_Stone_Push) {
    mazeArray = convert(
       //0123456789012
        "   hHo  * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();
    ASSERT_FALSE(maze.pullHero2StoneLeftBackward());

    int teleport = 0, push = 1, pull = 0;
    maze.setMaximumCardsAmount(teleport, push, pull);
    ASSERT_TRUE(maze.pullHero2StoneLeftBackward());

    Maze::MazeArray result = convert(
       //0123456789012
        "  hHo   * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), 'a');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 2);
    ASSERT_EQ(maze.heroY(), 0);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount + 1, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepLeft_05_Hole_and_Stone) {
    mazeArray = convert(
       //0123456789012
        " xoh    * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    ASSERT_FALSE(maze.stepLeftBackward());
}

TEST(BACKWARDS, StepLeft_06_Hole) {
    mazeArray = convert(
       //0123456789012
        "  xh    * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    ASSERT_FALSE(maze.stepLeftBackward());
}

TEST(BACKWARDS, StepLeft_07_Separate_Hole_and_Stone) {
    mazeArray = convert(
       //0123456789012
        " h      * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    maze.setMaximumHoleCount(12);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();

    Maze::MazeArray result = convert(
       //0123456789012
        "hox     * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze.stepLeftSeparateHoleAndStone());
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), 'f');
    ASSERT_EQ(stoneCount + 1, maze.stoneCount());
    ASSERT_EQ(holeCount + 1, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 0);
    ASSERT_EQ(maze.heroY(), 0);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}


TEST(BACKWARDS, StepRight_01_Empty_Tile) {
    mazeArray = convert(
       //0123456789012
        " h      * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);

    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();
    Maze::MazeArray result = convert(
       //0123456789012
        "  h     * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze.stepRightBackward());
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), 'r');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 2);
    ASSERT_EQ(maze.heroY(), 0);
    ASSERT_EQ(pushCounter, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepRight_02_Right_Side_of_the_Field) {
    mazeArray = convert(
       //0123456789012
        "        * h" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    ASSERT_FALSE(maze.stepRightBackward());

    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        "hooo o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    maze = mazeArray;
    ASSERT_FALSE(maze.stepRightBackward());

    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * xhx " // 5
        "     *x  xx" // 6
    );
    maze = mazeArray;
    ASSERT_FALSE(maze.stepRightBackward());
}

TEST(BACKWARDS, StepRight_03_Pull_Card_Required) {
    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " oho o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();

    Maze::MazeArray result = convert(
       //0123456789012
        "        * x" // 0
        " o hoo*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_FALSE(maze.stepRightBackward());
    ASSERT_FALSE(maze == result);

    int teleport = 0, push = 0, pull = 1;
    maze.setMaximumCardsAmount(teleport, push, pull);
    ASSERT_TRUE(maze.stepRightBackward());
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), 'p');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 3);
    ASSERT_EQ(maze.heroY(), 1);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount, maze.pushCardCount());
    ASSERT_EQ(pullCardCount + 1, maze.pullCardCount());
}

TEST(BACKWARDS, StepRight_03_Pull_Stone) {
    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " oh  o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();

    Maze::MazeArray result = convert(
       //0123456789012
        "        * x" // 0
        "  oh o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze.pullStoneRightBackward());
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), '6');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 3);
    ASSERT_EQ(maze.heroY(), 1);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepRight_04_Hero2_Pull) {
    mazeArray = convert(
       //0123456789012
        "   hH   * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    ASSERT_FALSE(maze.stepRightBackward());

    int teleport = 0, push = 0, pull = 1;
    maze.setMaximumCardsAmount(teleport, push, pull);
    ASSERT_FALSE(maze.stepRightBackward());
}

TEST(BACKWARDS, StepRight_04_Hero2_Push) {
    mazeArray = convert(
       //0123456789012
        "  Hh    * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();
    ASSERT_FALSE(maze.pullHero2RightBackward());

    int teleport = 0, push = 1, pull = 0;
    maze.setMaximumCardsAmount(teleport, push, pull);
    ASSERT_TRUE(maze.pullHero2RightBackward());

    Maze::MazeArray result = convert(
       //0123456789012
        "   Hh   * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), '2');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 4);
    ASSERT_EQ(maze.heroY(), 0);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount + 1, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepRight_04_Hero2_Stone_Push) {
    mazeArray = convert(
       //0123456789012
        " oHh    * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();
    ASSERT_FALSE(maze.pullHero2StoneRightBackward());

    int teleport = 0, push = 1, pull = 0;
    maze.setMaximumCardsAmount(teleport, push, pull);
    ASSERT_TRUE(maze.pullHero2StoneRightBackward());

    Maze::MazeArray result = convert(
       //0123456789012
        "  oHh   * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), 'b');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 4);
    ASSERT_EQ(maze.heroY(), 0);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount + 1, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepRight_05_Hole_and_Stone) {
    mazeArray = convert(
       //0123456789012
        "   hox  * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    ASSERT_FALSE(maze.stepRightBackward());
}

TEST(BACKWARDS, StepRight_06_Hole) {
    mazeArray = convert(
       //0123456789012
        "   hx   * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    ASSERT_FALSE(maze.stepRightBackward());
}

TEST(BACKWARDS, StepRight_07_Separate_Hole_and_Stone) {
    mazeArray = convert(
       //0123456789012
        " h      * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    maze.setMaximumHoleCount(12);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();

    Maze::MazeArray result = convert(
       //0123456789012
        "xoh     * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze.stepRightSeparateHoleAndStone());
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), 'g');
    ASSERT_EQ(stoneCount + 1, maze.stoneCount());
    ASSERT_EQ(holeCount + 1, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 2);
    ASSERT_EQ(maze.heroY(), 0);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepUp_01_Empty_Tile) {
    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " oho o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);

    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();
    Maze::MazeArray result = convert(
       //0123456789012
        "  h     * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze.stepUpBackward());
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), 'u');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 2);
    ASSERT_EQ(maze.heroY(), 0);
    ASSERT_EQ(pushCounter, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepUp_02_Up_Side_of_the_Field) {
    mazeArray = convert(
       //0123456789012
        "        * h" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    ASSERT_FALSE(maze.stepUpBackward());

    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " ooo o*  x " // 1
        "**o* o    x" // 2
        "ho o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    maze = mazeArray;
    ASSERT_FALSE(maze.stepUpBackward());

    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o *hx x " // 5
        "     *x  xx" // 6
    );
    maze = mazeArray;
    ASSERT_FALSE(maze.stepUpBackward());
}

TEST(BACKWARDS, StepUp_03_Pull_Card_Required) {
    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " o o o*  x " // 1
        "**o* h    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();

    Maze::MazeArray result = convert(
       //0123456789012
        "     o  * x" // 0
        " o o h*  x " // 1
        "**o*      x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_FALSE(maze.stepUpBackward());
    ASSERT_FALSE(maze == result);

    int teleport = 0, push = 0, pull = 1;
    maze.setMaximumCardsAmount(teleport, push, pull);
    ASSERT_TRUE(maze.stepUpBackward());
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), 'Q');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 5);
    ASSERT_EQ(maze.heroY(), 1);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount, maze.pushCardCount());
    ASSERT_EQ(pullCardCount + 1, maze.pullCardCount());
}

TEST(BACKWARDS, StepUp_03_Stone_Pull) {
    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " o o h*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();

    Maze::MazeArray result = convert(
       //0123456789012
        "     h  * x" // 0
        " o o o*  x " // 1
        "**o*      x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze.pullStoneUpBackward());
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), '7');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 5);
    ASSERT_EQ(maze.heroY(), 0);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepUp_04_Hero2_Pull) {
    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " o oHo*  x " // 1
        "**o*ho    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    ASSERT_FALSE(maze.stepUpBackward());

    int teleport = 0, push = 0, pull = 1;
    maze.setMaximumCardsAmount(teleport, push, pull);
    ASSERT_FALSE(maze.stepUpBackward());
}

TEST(BACKWARDS, StepUp_04_Hero2_Push) {
    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " o o o*  x " // 1
        "**o*ho    x" // 2
        " o oH*  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();
    ASSERT_FALSE(maze.pullHero2UpBackward());

    int teleport = 0, push = 1, pull = 0;
    maze.setMaximumCardsAmount(teleport, push, pull);
    ASSERT_TRUE(maze.pullHero2UpBackward());

    Maze::MazeArray result = convert(
       //0123456789012
        "        * x" // 0
        " o oho*  x " // 1
        "**o*Ho    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), '3');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 4);
    ASSERT_EQ(maze.heroY(), 1);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount + 1, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepUp_04_Hero2_Stone_Push) {
    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " o o o*  x " // 1
        "**o*ho    x" // 2
        " o oH*  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();
    ASSERT_FALSE(maze.pullHero2StoneUpBackward());

    int teleport = 0, push = 1, pull = 0;
    maze.setMaximumCardsAmount(teleport, push, pull);
    ASSERT_TRUE(maze.pullHero2StoneUpBackward());

    Maze::MazeArray result = convert(
       //0123456789012
        "        * x" // 0
        " o oho*  x " // 1
        "**o*Ho    x" // 2
        " o oo*  *x " // 3
        "  o  *x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), 'c');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 4);
    ASSERT_EQ(maze.heroY(), 1);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount + 1, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepUp_05_Hole_and_Stone) {
    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " o o o*x x " // 1
        "**o* o o  x" // 2
        " o o * h*x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    ASSERT_FALSE(maze.stepUpBackward());
}

TEST(BACKWARDS, StepUp_06_Hole) {
    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " o o o*  x " // 1
        "**o* o   hx" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    ASSERT_FALSE(maze.stepUpBackward());
}

TEST(BACKWARDS, StepUp_07_Separate_Hole_and_Stone) {
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
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    maze.setMaximumHoleCount(12);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();

    Maze::MazeArray result = convert(
       //0123456789012
        "       h* x" // 0
        " o o o*o x " // 1
        "**o* o x  x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze.stepUpSeparateHoleAndStone());
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), 'h');
    ASSERT_EQ(stoneCount + 1, maze.stoneCount());
    ASSERT_EQ(holeCount + 1, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 7);
    ASSERT_EQ(maze.heroY(), 0);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepDown_01_Empty_Tile) {
    mazeArray = convert(
       //0123456789012
        "  h     * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);

    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();
    Maze::MazeArray result = convert(
       //0123456789012
        "        * x" // 0
        " oho o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze.stepDownBackward());
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), 'd');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 2);
    ASSERT_EQ(maze.heroY(), 1);
    ASSERT_EQ(pushCounter, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepDown_02_Down_Side_of_the_Field) {
    mazeArray = convert(
       //0123456789012
        "        *  " // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "  h  *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    ASSERT_FALSE(maze.stepDownBackward());

    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        "hooo o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    maze = mazeArray;
    ASSERT_FALSE(maze.stepDownBackward());

    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o *hx x " // 5
        "     *x  xx" // 6
    );
    maze = mazeArray;
    ASSERT_FALSE(maze.stepDownBackward());
}

TEST(BACKWARDS, StepDown_03_Pull_Card_Required) {
    mazeArray = convert(
       //0123456789012
        "     h  * x" // 0
        " o o o*  x " // 1
        "**o*      x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();

    Maze::MazeArray result = convert(
       //0123456789012
        "        * x" // 0
        " o o h*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_FALSE(maze.stepDownBackward());
    ASSERT_FALSE(maze == result);

    int teleport = 0, push = 0, pull = 1;
    maze.setMaximumCardsAmount(teleport, push, pull);
    ASSERT_TRUE(maze.stepDownBackward());
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), 'q');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 5);
    ASSERT_EQ(maze.heroY(), 1);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount, maze.pushCardCount());
    ASSERT_EQ(pullCardCount + 1, maze.pullCardCount());
}

TEST(BACKWARDS, StepDown_03_Pull_Stone) {
    mazeArray = convert(
       //0123456789012
        "     o  * x" // 0
        " o o h*  x " // 1
        "**o*      x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();

    Maze::MazeArray result = convert(
       //0123456789012
        "        * x" // 0
        " o o o*  x " // 1
        "**o* h    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze.pullStoneDownBackward());
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), '8');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 5);
    ASSERT_EQ(maze.heroY(), 2);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepDown_04_Hero2_Pull) {
    mazeArray = convert(
       //0123456789012
        "    h   * x" // 0
        " o oHo*  x " // 1
        "**o* o    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    ASSERT_FALSE(maze.stepDownBackward());

    int teleport = 0, push = 0, pull = 1;
    maze.setMaximumCardsAmount(teleport, push, pull);
    ASSERT_FALSE(maze.stepDownBackward());
}

TEST(BACKWARDS, StepDown_04_Hero2_Push) {
    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " o oHo*  x " // 1
        "**o*ho    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();
    ASSERT_FALSE(maze.pullHero2DownBackward());

    int teleport = 0, push = 1, pull = 0;
    maze.setMaximumCardsAmount(teleport, push, pull);
    ASSERT_TRUE(maze.pullHero2DownBackward());

    Maze::MazeArray result = convert(
       //0123456789012
        "        * x" // 0
        " o o o*  x " // 1
        "**o*Ho    x" // 2
        " o oh*  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), '4');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 4);
    ASSERT_EQ(maze.heroY(), 3);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount + 1, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepDown_04_Hero2_Stone_Push) {
    mazeArray = convert(
       //0123456789012
        "    o   * x" // 0
        " o oHo*  x " // 1
        "**o*ho    x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();
    ASSERT_FALSE(maze.pullHero2StoneDownBackward());

    int teleport = 0, push = 1, pull = 0;
    maze.setMaximumCardsAmount(teleport, push, pull);
    ASSERT_TRUE(maze.pullHero2StoneDownBackward());

    Maze::MazeArray result = convert(
       //0123456789012
        "        * x" // 0
        " o ooo*  x " // 1
        "**o*Ho    x" // 2
        " o oh*  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), 'e');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 4);
    ASSERT_EQ(maze.heroY(), 3);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount + 1, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, StepDown_05_Hole_and_Stone) {
    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " o o o*h x " // 1
        "**o* o o  x" // 2
        " o o * x*x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    ASSERT_FALSE(maze.stepDownBackward());
}

TEST(BACKWARDS, StepDown_06_Hole) {
    mazeArray = convert(
       //0123456789012
        "        * x" // 0
        " o o o*  x " // 1
        "**o* o   hx" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    ASSERT_FALSE(maze.stepDownBackward());
}

TEST(BACKWARDS, StepDown_07_Separate_Hole_and_Stone) {
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
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    maze.setMaximumHoleCount(12);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();

    Maze::MazeArray result = convert(
       //0123456789012
        "       x* x" // 0
        " o o o*o x " // 1
        "**o* o h  x" // 2
        " o o *  *x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze.stepDownSeparateHoleAndStone());
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), 'i');
    ASSERT_EQ(stoneCount + 1, maze.stoneCount());
    ASSERT_EQ(holeCount + 1, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 7);
    ASSERT_EQ(maze.heroY(), 2);
    ASSERT_EQ(pushCounter + 1, maze.pushCounter());
    ASSERT_EQ(teleportCardCount, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, Teleport) {
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
    Maze maze;
    maze = mazeArray;
    maze.setCardsAmount(0, 0, 0);
    maze.setMaximumHoleCount(12);
    int teleport = 0, push = 0, pull = 0;
    maze.setMaximumCardsAmount(teleport, push, pull);
    int stoneCount = maze.stoneCount();
    int holeCount = maze.holeCount();
    int pushCounter = maze.pushCounter();
    int teleportCardCount = maze.teleportCardCount();
    int pushCardCount = maze.pushCardCount();
    int pullCardCount = maze.pullCardCount();
    ASSERT_FALSE(maze.teleportBackward(7, 3));

    teleport = 1, push = 0, pull = 0;
    maze.setMaximumCardsAmount(teleport, push, pull);
    ASSERT_FALSE(maze.teleportBackward(7, 2));

    ASSERT_TRUE(maze.teleportBackward(7, 3));
    Maze::MazeArray result = convert(
       //0123456789012
        "        * x" // 0
        " o o o*  x " // 1
        "**o* o    x" // 2
        " o o * h*x " // 3
        "  o o*x  x*" // 4
        " o o * x x " // 5
        "     *x  xx" // 6
    );
    ASSERT_TRUE(maze == result);
    ASSERT_EQ(maze.op(), 't');
    ASSERT_EQ(stoneCount, maze.stoneCount());
    ASSERT_EQ(holeCount, maze.holeCount());
    ASSERT_EQ(maze.heroX(), 7);
    ASSERT_EQ(maze.heroY(), 3);
    ASSERT_EQ(pushCounter, maze.pushCounter());
    ASSERT_EQ(teleportCardCount + 1, maze.teleportCardCount());
    ASSERT_EQ(pushCardCount, maze.pushCardCount());
    ASSERT_EQ(pullCardCount, maze.pullCardCount());
}

TEST(BACKWARDS, DISABLED_AStar_Extend) {
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

    class TestClass : public AStar<MazeStar> {
    public:
        void test() {
            ASSERT_EQ(mazeSetBackward_.size(), 6);
        }
        void init() {
            backwardGoal_ = *mazesBackward_.begin();

        }
    };
    TestClass aStar;
    MazeStar maze;
    maze = mazeArray;
    maze.setMaximumHoleCount(maze.holeCount() + 1);
    aStar <<= maze;
    aStar.init();
    aStar.extendBackward(&maze);
    aStar.test();
}
