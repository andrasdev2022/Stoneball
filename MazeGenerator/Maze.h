#ifndef __MAZE_H__
#define __MAZE_H__
#include <iostream>
#include <array>
#include <vector>
#include <set>
#include <iomanip>
#include <assert.h>

constexpr int mazeWidth = 6;
constexpr int mazeHeight = 5;
constexpr int wallCount = 10;
constexpr int heroCount = 1;
constexpr int hero2Count = 0;
constexpr int stoneAndHoleCount = 2;

constexpr int teleportCount = 0;
constexpr int pushCount = 0;
constexpr int pullCount = 0;
constexpr int depthLimit = 80;

constexpr char stone = 'o';
constexpr char hole = 'x';
constexpr char hero = 'h';
constexpr char wall = '*';
constexpr char space = ' ';
constexpr char hero2 = 'H';

class Maze;
class MazeStar;
class MazeStarV2;
class MazeStarV3;

class MazeInterface {
public:
    virtual ~MazeInterface(){}
    virtual const MazeStar* mazeStar() const {return nullptr;}
    virtual const MazeStarV2* mazeStarV2() const {return nullptr;}
    virtual const MazeStarV3* mazeStarV3() const {return nullptr;}

    virtual int h() const {return 0;}
    virtual int g() const {return 0;}
    virtual int f() const {return 0;}

    virtual bool isGoalBackward([[maybe_unused]]const Maze* backwardGoal) const {return false;}
};

class Maze : public MazeInterface
{
    friend std::ostream& operator<<(std::ostream& out, const Maze& maze);
    //friend bool extend(const Maze& maze);
    friend class BreadthFirst;
    template<typename T> friend class AStar;

public:
    using MazeArray = std::array<std::array<char, mazeWidth>, mazeHeight>;

    Maze(){}

    //virtual void setValue(int x, int y, char value);
    virtual bool operator<(const Maze& maze) const;
    virtual bool operator==(const Maze& maze) const;
    virtual bool operator==(const MazeArray& maze) const;
    virtual bool isGoal() const;
    virtual void printStates(int depth = 1) const;

    // operators: step left, step right, step up, step down, teleport, push, pull
    virtual bool stepLeft() noexcept;
    virtual bool stepRight() noexcept;
    virtual bool stepUp() noexcept;
    virtual bool stepDown() noexcept;
    virtual bool teleport(int x, int y) noexcept;
    virtual bool pullLeft() noexcept;
    virtual bool pullRight() noexcept;
    virtual bool pullUp() noexcept;
    virtual bool pullDown() noexcept;

    virtual bool stepLeftBackward() noexcept;
    virtual bool stepRightBackward() noexcept;
    virtual bool stepUpBackward() noexcept;
    virtual bool stepDownBackward() noexcept;
    virtual bool teleportBackward([[maybe_unused]]int x, [[maybe_unused]]int y) noexcept;

    virtual bool pullHero2LeftBackward() noexcept;
    virtual bool pullHero2RightBackward() noexcept;
    virtual bool pullHero2UpBackward() noexcept;
    virtual bool pullHero2DownBackward() noexcept;

    virtual bool pullStoneLeftBackward() noexcept;
    virtual bool pullStoneRightBackward() noexcept;
    virtual bool pullStoneUpBackward() noexcept;
    virtual bool pullStoneDownBackward() noexcept;

    virtual bool pullHero2StoneLeftBackward() noexcept;
    virtual bool pullHero2StoneRightBackward() noexcept;
    virtual bool pullHero2StoneUpBackward() noexcept;
    virtual bool pullHero2StoneDownBackward() noexcept;

    virtual bool stepLeftSeparateHoleAndStone() noexcept; // only for backward
    virtual bool stepRightSeparateHoleAndStone() noexcept; // only for backward
    virtual bool stepUpSeparateHoleAndStone() noexcept; // only for backward
    virtual bool stepDownSeparateHoleAndStone() noexcept; // only for backward

    virtual int8_t maximumTeleportCardCount() const noexcept {return maximumTeleportCardCount_;}
    virtual int8_t maximumPushCardCount() const noexcept {return maximumPushCardCount_;}
    virtual int8_t maximumPullCardCount() const noexcept {return maximumPullCardCount_;}
    virtual int8_t maximumHoleCount() const noexcept {return maximumHoleCount_;}

    virtual Maze& operator=(const MazeArray& maze);

    virtual int heroX() const {return heroX_;}
    virtual int heroY() const {return heroY_;}

    virtual void setCardsAmount(int teleport, int push, int pull) noexcept;
    virtual void setMaximumCardsAmount(int teleport, int push, int pull) noexcept; // for backward search
    virtual int teleportCardCount() const noexcept {return teleportCardCount_;}
    virtual int pushCardCount() const noexcept {return pushCardCount_;}
    virtual int pullCardCount() const noexcept {return pullCardCount_;}
    virtual void setMaximumHoleCount(int8_t maximumHoleCount) noexcept {maximumHoleCount_ = maximumHoleCount;}

    virtual const MazeArray& maze() const {return maze_;}

    virtual int16_t pushCounter() const noexcept {return pushCounter_;}
    virtual int16_t depth() const noexcept {return depth_;}

    virtual int8_t stoneCount() const noexcept {return stoneCount_;}
    virtual int8_t holeCount() const noexcept {return holeCount_;}

    enum class Neighbour {BOTTOM, TOP, LEFT, RIGHT, ITSELF};

    virtual const char& topLeftCorner() const noexcept {return maze_.at(0).at(0);}
    virtual char& topLeftCorner() noexcept {return maze_[0][0];}
    virtual bool testTopLeftCornerAnyOf(Neighbour neighbour, char value1, char value2) const noexcept;

    virtual const char& topRightCorner() const noexcept {return maze_.at(0).at(mazeWidth - 1);}
    virtual char& topRightCorner() noexcept {return maze_[0][mazeWidth - 1];}
    virtual bool testTopRightCornerAnyOf(Neighbour neighbour, char value1, char value2) const noexcept;

    virtual const char& bottomLeftCorner() const noexcept {return maze_.at(mazeHeight - 1).at(0);}
    virtual char& bottomLeftCorner() noexcept {return maze_[mazeHeight - 1][0];}
    virtual bool testBottomLeftCornerAnyOf(Neighbour neighbour, char value1, char value2) const noexcept;

    virtual const char& bottomRightCorner() const noexcept {return maze_.at(mazeHeight - 1).at(mazeWidth - 1);}
    virtual char& bottomRightCorner() noexcept {return maze_[mazeHeight - 1][mazeWidth - 1];}
    virtual bool testBottomRightCornerAnyOf(Neighbour neighbour, char value1, char value2) const noexcept;

    virtual bool testPositionAnyOf(int x, int y, char value1, char value2, bool returnValueInCaseOfOutOfBorder = false) const noexcept;
    virtual bool testPosition(int x, int y, char value, bool returnValueInCaseOfOutOfBorder = false) const noexcept;

    virtual bool testPositionIsValAndSurroundedBySVal(int x, int y, char value, char surroundingValue) const noexcept;
    virtual bool testPositionIsVal2AndSurroundedBySVal(int x, int y, char value1, char value2, char surroundingValue) const noexcept;
    virtual bool testPositionIsVal2AndSurroundedBySVals(int x, int y, char value1, char value2, char surroundingValue1, char surroundingValue2) const noexcept;

    //std::size_t hash() const noexcept {return hash_;}
    //double hashNumber() const noexcept {return hashNumber_;}

    virtual char op() const noexcept {return op_;}

protected:
    virtual void addStone(int x, int y);
    virtual void removeStone(int x, int y);
    virtual void removeHole(int x, int y);
    virtual void addHole(int x, int y);
    virtual void addHero2(int x, int y) noexcept;
    virtual void removeHero2(int x, int y) noexcept;
    //virtual void makeHashKey() noexcept;
    virtual void printCostAndHeuristic() const noexcept {}
    virtual void setStoneCount(int8_t c) noexcept {stoneCount_ = c;}
    virtual void setHoleCount(int8_t h) noexcept {holeCount_ = h;}
    virtual void setHeroX(int8_t heroX) noexcept {heroX_ = heroX;}
    virtual void setHeroY(int8_t heroY) noexcept {heroY_ = heroY;}

    MazeArray maze_;

    const Maze* parent() const noexcept {return parent_;}

private:
    const mutable Maze* parent_{}; // it's not part of the set's key
    int8_t heroX_{};
    int8_t heroY_{};
    int8_t stoneCount_{};
    int8_t holeCount_{};
    int8_t teleportCardCount_{};
    int8_t pushCardCount_{};
    int8_t pullCardCount_{};

    int8_t maximumTeleportCardCount_{};
    int8_t maximumPushCardCount_{};
    int8_t maximumPullCardCount_{};
    int8_t maximumHoleCount_{};

    mutable int16_t pushCounter_{};
    mutable int16_t depth_{1};
    mutable char op_{}; //lLrRuUdDt,  p: pull left, P: pull right, q: pull up, Q: pull down, backward operator must be inverted
    //std::size_t hash_{};
    //double hashNumber_{};
};
std::ostream& operator<<(std::ostream& out, const Maze& maze);
std::ostream& operator<<(std::ostream& out, const Maze::MazeArray& maze);

#endif // __MAZE_H__
