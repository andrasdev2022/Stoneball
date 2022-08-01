#ifndef __TESTUTIL_H__
#define __TESTUTIL_H__

#include "Maze.h"

namespace TestUtil
{
    extern Maze::MazeArray next_phase_test;
    extern Maze::MazeArray mazeArray;
    std::string doNextPhase(std::string_view label, int maxDepth);
    std::string doNextPhaseBackward(std::string_view label, int maxDepth);
    std::string doNextPhaseBothDirection(std::string_view label, int minDepth, int maxDepth);
    Maze::MazeArray convert(std::string str) noexcept;
    void testSolution(std::string steps) noexcept;
    void findWithCardIteration(int teleportCards) noexcept;
}

#endif // TESTUTIL_H
