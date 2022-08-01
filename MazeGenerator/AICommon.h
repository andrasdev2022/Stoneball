#ifndef __AICOMMON_H__
#define __AICOMMON_H__

//#include "Maze.h"

class Maze;

class AICommon
{
public:
    enum class Direction {FORWARD, BACKWARD, BOTH};

    AICommon() {}
    virtual ~AICommon() {}

    virtual bool isSolvable(const Maze* maze) noexcept;
    virtual void setMaximumDepth(int depth) noexcept {maximumDepth_ = depth;}
    virtual void setMaximumDepthBackward(int depth) noexcept {maximumDepthBackward_ = depth;}
    virtual void setDirection(Direction d) noexcept {direction_ = d;}

protected:
    int maximumDepth_{10000};
    int maximumDepthBackward_{10000};
    Direction direction_{Direction::FORWARD};
};

#endif // __AICOMMON_H__
