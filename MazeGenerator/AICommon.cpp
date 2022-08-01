#include "AICommon.h"
#include "Maze.h"

bool AICommon::isSolvable(const Maze* maze) noexcept {
    if(maze->holeCount() != maze->stoneCount()) {return false;}

    // blocked hole or stone in the top-left corner
    if(maze->testTopLeftCornerAnyOf(Maze::Neighbour::ITSELF, hole, stone) &&
    maze->testTopLeftCornerAnyOf(Maze::Neighbour::RIGHT, hero2, wall) &&
    maze->testTopLeftCornerAnyOf(Maze::Neighbour::BOTTOM, hero2, wall)) {
        return false;
    }

    // blocked hole or stone in the top-right corner
    if(maze->testTopRightCornerAnyOf(Maze::Neighbour::ITSELF, hole, stone) &&
    maze->testTopRightCornerAnyOf(Maze::Neighbour::LEFT, hero2, wall) &&
    maze->testTopRightCornerAnyOf(Maze::Neighbour::BOTTOM, hero2, wall)) {
        return false;
    }

    // blocked hole or stone in the bottom-left corner
    if(maze->testBottomLeftCornerAnyOf(Maze::Neighbour::ITSELF, hole, stone) &&
    maze->testBottomLeftCornerAnyOf(Maze::Neighbour::RIGHT, hero2, wall) &&
    maze->testBottomLeftCornerAnyOf(Maze::Neighbour::TOP, hero2, wall)) {
        return false;
    }

    // blocked hole or stone in the bottom-right corner
    if(maze->testBottomRightCornerAnyOf(Maze::Neighbour::ITSELF, hole, stone) &&
    maze->testBottomRightCornerAnyOf(Maze::Neighbour::LEFT, hero2, wall) &&
    maze->testBottomRightCornerAnyOf(Maze::Neighbour::TOP, hero2, wall)) {
        return false;
    }
/*
    // blocked stone in the top-left corner
    if(maze->testTopLeftCornerAnyOf(Maze::Neighbour::ITSELF, stone, stone) &&
    maze->testTopLeftCornerAnyOf(Maze::Neighbour::RIGHT, hole, hole) &&
    maze->testTopLeftCornerAnyOf(Maze::Neighbour::BOTTOM, hole, hole)) {
        return false;
    }

    // blocked stone in the top-right corner
    if(maze->testTopRightCornerAnyOf(Maze::Neighbour::ITSELF, stone, stone) &&
    maze->testTopRightCornerAnyOf(Maze::Neighbour::LEFT, hole, hole) &&
    maze->testTopRightCornerAnyOf(Maze::Neighbour::BOTTOM, hole, hole)) {
        return false;
    }

    // blocked stone in the bottom-left corner
    if(maze->testBottomLeftCornerAnyOf(Maze::Neighbour::ITSELF, stone, stone) &&
    maze->testBottomLeftCornerAnyOf(Maze::Neighbour::RIGHT, hole, hole) &&
    maze->testBottomLeftCornerAnyOf(Maze::Neighbour::TOP, hole, hole)) {
        return false;
    }

    // blocked stone in the bottom-right corner
    if(maze->testBottomRightCornerAnyOf(Maze::Neighbour::ITSELF, stone, stone) &&
    maze->testBottomRightCornerAnyOf(Maze::Neighbour::LEFT, hole, hole) &&
    maze->testBottomRightCornerAnyOf(Maze::Neighbour::TOP, hole, hole)) {
        return false;
    }
*/
    for(int y = 0; y < mazeHeight; ++y) {
        for(int x = 0; x < mazeWidth; ++x) {
            // if a stone or hole is surrounded by walls
            if(maze->testPositionIsVal2AndSurroundedBySVal(x, y, hole, stone, wall)) {
                return false;
            }
            // if a stone is surrounded by holes: another stone can be pushed into a surrounder hole
            //if(maze->testPositionIsValAndSurroundedBySVal(x, y, stone, hole)) {
            //    return false;
            //}
            // if player has no push card and a hole or stone is surrounded by hero2s and walls
            if(!maze->pushCardCount() && maze->testPositionIsVal2AndSurroundedBySVals(x, y, hole, stone, hero2, wall)) {
                return false;
            }
        }
    }
/*
    if(!maze->teleportCardCount()) {
        // hero is blocked
        if(maze->testPositionAnyOf(maze->heroX() - 1, maze->heroY(), wall, 0) ||
        maze->testPositionAnyOf(maze->heroX() + 1, maze->heroY(), wall, 0) ||
        maze->testPositionAnyOf(maze->heroX(), maze->heroY() - 1, wall, 0) ||
        maze->testPositionAnyOf(maze->heroX(), maze->heroY() + 1, wall, 0)) {
            return false;
        }

        if(!maze->pushCardCount()) {
            if(maze->testPositionAnyOf(maze->heroX() - 1, maze->heroY(), hero2, 0) ||
            maze->testPositionAnyOf(maze->heroX() + 1, maze->heroY(), hero2, 0) ||
            maze->testPositionAnyOf(maze->heroX(), maze->heroY() - 1, hero2, 0) ||
            maze->testPositionAnyOf(maze->heroX(), maze->heroY() + 1, hero2, 0)) {
                return false;
            }
        }
    }
*/
    return true;
}
