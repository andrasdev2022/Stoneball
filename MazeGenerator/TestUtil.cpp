#include "TestUtil.h"
#include <gtest/gtest.h>
#include "MazeStarV2.h"
#include "MazeStarV3.h"
#include "AStar.h"

namespace TestUtil
{
    [[maybe_unused]]Maze::MazeArray next_phase_test;
    [[maybe_unused]]Maze::MazeArray mazeArray;

    std::string doNextPhase(std::string_view label, int maxDepth) {
        std::cout << std::string(40, '-') << label << std::string(40, '-') << "\n";
        AStarV2 aStar;
        MazeStarV2 maze;
        maze = mazeArray;
        maze.setCardsAmount(0, 0, 0);
        aStar << maze;
        aStar.setMaximumDepth(maxDepth);
        aStar.setTimeout(180e6);
        EXPECT_TRUE(aStar.search(false, false));
        std::cout << "Depth <= " << aStar.depth() << "\n";
        return aStar.stepString();
    }

    std::string doNextPhaseBackward(std::string_view label, int maxDepth) {
        std::cout << std::string(40, '-') << label << std::string(40, '-') << "\n";
        AStarV2 aStar;
        MazeStarV2 maze;
        maze = TestUtil::next_phase_test;
        maze.setCardsAmount(0, 0, 0);
        maze.setMaximumHoleCount(maze.holeCount() + 1);
        aStar <<= maze;
        aStar.setDirection(AICommon::Direction::BACKWARD);
        aStar.setMaximumDepth(maxDepth);
        aStar.setTimeout(180e6);
        EXPECT_TRUE(aStar.search(false, false));
        std::cout << "Depth <= " << aStar.depth() << "\n";
        return aStar.stepString();
    }

    std::string doNextPhaseBothDirection(std::string_view label, int minDepth, int maxDepth) {
        std::cout << std::string(40, '-') << label << std::string(40, '-') << "\n";
        for(int i = minDepth; i <= maxDepth; ++i) {
            std::cout << "i: " << i << "\n";
            AStarV2 aStar;
            MazeStarV2 maze;
            maze = mazeArray;
            maze.setCardsAmount(0, 0, 0);

            MazeStarV2 mazeEnd;
            mazeEnd = next_phase_test;
            mazeEnd.setMaximumHoleCount(maze.holeCount());
            mazeEnd.setCardsAmount(0, 0, 0);

            aStar << maze;
            aStar <<= mazeEnd;
            aStar.setMaximumDepth(i);
            aStar.setMaximumDepthBackward(i);
            aStar.setDirection(AICommon::Direction::BOTH);
            aStar.setTimeout(180e6);
            if(minDepth == maxDepth) {
                EXPECT_TRUE(aStar.search(false, false));
                std::cout << "Depth <= " << aStar.depth() << "\n";
                return aStar.stepString();
            } else {
                if(aStar.search(true, false)) {
                    std::cout << "Depth <= " << aStar.depth() << "\n";
                    return aStar.stepString();
                }
            }
        }
        return "";
    }

    void testSolution(std::string steps) noexcept {
        stopWatch.reset();
        AStar<MazeStar> aStar;
        MazeStar maze;
        maze = mazeArray;
        aStar << maze;
        aStar.testSteps(steps);
        EXPECT_EQ(aStar.testStepsResult(), next_phase_test);
    }

    Maze::MazeArray convert(std::string str) noexcept {
        Maze::MazeArray mazeArray;
        if(str.length() != mazeWidth * mazeHeight) {
            std::cerr << "convert: string length does not match maze size\n";
            return mazeArray;
        }
        for(size_t y = 0; y < mazeHeight; ++y) {
            for(size_t x = 0; x < mazeWidth; ++x) {
                mazeArray[y][x] = str[y * mazeWidth + x];
            }
        }
        return mazeArray;
    }

    bool isThereHero2Tile() {
        for(int y = 0; y < mazeHeight; ++y) {
            for(int x = 0; x < mazeWidth; ++ x) {
                if(mazeArray[y][x] == hero2) return true;
            }
        }
        return false;
    }

    bool searchWithParams(int teleportCardCount, int pushCardCount, int pullCardCount, bool findBest) noexcept {
        AStarV3 aStar;
        MazeStarV3 maze;
        maze = mazeArray;
        maze.setCardsAmount(teleportCardCount, pushCardCount, pullCardCount);
        aStar << maze;
        aStar.setMaximumDepth(depthLimit);

        MazeStarV3 mazeEnd;

        if(1/*isThereHero2Tile()*/) {
            // we cannot use backward search
            aStar.setDirection(AICommon::Direction::FORWARD);
            aStar.enablePushAlteration();
        } else {
            aStar.setDirection(AICommon::Direction::BOTH);
            mazeEnd.start2EndState(mazeArray);
            mazeEnd.setMaximumHoleCount(maze.holeCount());
            mazeEnd.setCardsAmount(0, 0, 0);
            mazeEnd.setMaximumCardsAmount(teleportCardCount, pushCardCount, pullCardCount);
            aStar <<= mazeEnd;
            aStar.setMaximumDepthBackward(depthLimit);
        }

        aStar.setTimeout(6000e6);

        if(aStar.search(false, findBest)) {
            if(!findBest) return true;
            auto steps = aStar.stepString();
            std::cout << std::string(80, '-') << "\n";
            std::cout << "                  Solution(s)\n";
            aStar.printFirstState();
            //aStar.printFirstStateBackward();
            //aStar.printStates();
            std::cout << "steps: " << steps << " (" << steps.length() << ")\n";
            std::cout << "lines: " << aStar.lines(steps) << "\n";
            std::cout << std::string(80, '-') << "\n";
            return true;
        }
        assert(!findBest);
        return false;
    }

    void findWithCardIteration(int teleportCards) noexcept {
        for(int cards = 0; cards <= 0; ++cards) {
            std::cerr << "card: " << cards << "\r";
            if(searchWithParams(teleportCards, cards, cards, false) &&
            searchWithParams(teleportCards, cards, cards, true)) {
                break;
            }
        }
    }
}
