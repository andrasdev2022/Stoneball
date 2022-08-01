#ifndef __MAZESTARV2_H__
#define __MAZESTARV2_H__

#include "MazeStar.h"

class MazeStarV2 : public MazeStar {
public:
    bool isGoal() const override {
        return operator==(TestUtil::next_phase_test);
    }

    bool isGoalBackward([[maybe_unused]]const MazeStarV2* backwardGoal) const override {
        return operator==(TestUtil::mazeArray);
    }

    MazeStarV2& operator=(const MazeArray& maze) override {
        MazeStar::operator=(maze);
        return *this;
    }
    const MazeStarV2* mazeStarV2() const override {return this;}

    MazeStarV2() : MazeStar() {}

    MazeStarV2(const MazeStar& o) {
        *this = *o.mazeStarV2();
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
};

#endif // __MAZESTARV2_H__
