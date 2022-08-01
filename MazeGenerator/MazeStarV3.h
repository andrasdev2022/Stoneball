#ifndef __MAZESTARV3_H__
#define __MAZESTARV3_H__

#include "MazeStar.h"

class MazeStarV3 : public MazeStar {
public:
    bool isGoalBackward(const MazeStarV3* backwardGoal) const override {
        if(backwardGoal) {
            for(int i = 0; i < mazeHeight; ++i)  {
                for(int j = 0; j < mazeWidth; ++j) {
                    if((maze_[i][j] == hole || maze_[i][j] == stone) && backwardGoal->maze().at(i).at(j) != maze_[i][j]) {
                        return false;
                    }
                }
            }
        }
        return /*pushCounter() >= 24 &&*/ holeCount() == maximumHoleCount();
    }

    MazeStarV3& operator=(const Maze::MazeArray& maze) override {
        MazeStar::operator=(maze);
        return *this;
    }
    const MazeStarV3* mazeStarV3() const override {return this;}

    MazeStarV3() : MazeStar() {}

    MazeStarV3(const MazeStar& o) {
        *this = *o.mazeStarV3();
    }

    void calcHBackward() override {
        calcH();
        /*
        h_ = 0;
        for(int i = 0; i < mazeHeight; ++i)  {
            for(int j = 0; j < mazeWidth; ++j) {
                h_ += abs((int)TestUtil::next_phase_test.at(i).at(j) - (int)maze_.at(i).at(j));
            }
        }*/
    }

    void start2EndState(const Maze::MazeArray& maze) noexcept {
        // hero2 is handled by the operators
        setStoneCount(0);
        setHoleCount(0);
        stones_.clear();
        holes_.clear();
        for(int i = 0; i < mazeHeight; ++i)  {
            for(int j = 0; j < mazeWidth; ++j) {
                if(maze.at(i).at(j) == hero2) {
                    std::cerr << "ERROR: start2EndState: conversion does not work with hero2 tiles\n";
                    exit(EXIT_FAILURE);
                }
                if(maze.at(i).at(j) != hole && maze.at(i).at(j) != stone) {
                    if(maze.at(i).at(j) == hero) {
                        setHeroX(j);
                        setHeroY(i);
                    }
                    maze_[i][j] = maze.at(i).at(j);
                } else {
                    maze_[i][j] = space;
                }
            }
        }
    }
};

#endif // __MAZESTARV3_H__
