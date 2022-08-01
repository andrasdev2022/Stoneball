#ifndef __MAZESTAR_H__
#define __MAZESTAR_H__

#include "Maze.h"

template<typename T>
class AStar;

class MazeStar : public Maze {
public:
    template<typename T> friend class AStar;
    virtual MazeStar& operator=(const MazeArray& maze);

    int h() const override {return h_;}
    int g() const override {return g_;};
    int f() const override {return g_ + h_;}
    bool isGoal() const override;
    virtual bool isGoalBackward(const MazeStar* backwardGoal) const;
    virtual bool isGoalBackward([[maybe_unused]]const MazeStarV2* backwardGoal) const {return false;}
    virtual bool isGoalBackward([[maybe_unused]]const MazeStarV3* backwardGoal) const {return false;}

    // operators: step left, step right, step up, step down
    bool stepLeft() noexcept override;
    bool stepRight() noexcept override;
    bool stepUp() noexcept override;
    bool stepDown() noexcept override;
    bool teleport(int x, int y) noexcept override;
    bool pullLeft() noexcept override;
    bool pullRight() noexcept override;
    bool pullUp() noexcept override;
    bool pullDown() noexcept override;

    bool stepLeftBackward() noexcept override;
    bool stepRightBackward() noexcept override;
    bool stepUpBackward() noexcept override;
    bool stepDownBackward() noexcept override;
    bool teleportBackward([[maybe_unused]]int x, [[maybe_unused]]int y) noexcept override;

    bool pullHero2LeftBackward() noexcept override;
    bool pullHero2RightBackward() noexcept override;
    bool pullHero2UpBackward() noexcept override;
    bool pullHero2DownBackward() noexcept override;

    bool pullStoneLeftBackward() noexcept override;
    bool pullStoneRightBackward() noexcept override;
    bool pullStoneUpBackward() noexcept override;
    bool pullStoneDownBackward() noexcept override;

    bool pullHero2StoneLeftBackward() noexcept override;
    bool pullHero2StoneRightBackward() noexcept override;
    bool pullHero2StoneUpBackward() noexcept override;
    bool pullHero2StoneDownBackward() noexcept override;

    virtual bool stepLeftSeparateHoleAndStone(const MazeStar* backwardGoal) noexcept; // only for backward
    virtual bool stepRightSeparateHoleAndStone(const MazeStar* backwardGoal) noexcept; // only for backward
    virtual bool stepUpSeparateHoleAndStone(const MazeStar* backwardGoal) noexcept; // only for backward
    virtual bool stepDownSeparateHoleAndStone(const MazeStar* backwardGoal) noexcept; // only for backward

    //bool operator==(const MazeStar& m) const;

    const MazeStar* mazeStar() const override {return this;}
    void updateHeroPosition(int heroX, int heroY) noexcept;

protected:
    void addStone(int x, int y) override;
    void removeStone(int x, int y) override;
    void removeHole(int x, int y) override;
    void addHole(int x, int y) override;
    void addHero2(int x, int y) noexcept override;
    void removeHero2(int x, int y) noexcept override;
    void printCostAndHeuristic() const noexcept override;
    virtual void calcG();
    virtual void calcH();
    virtual void calcHBackward();

    static constexpr int cardUsageCost = mazeWidth * mazeHeight;

    mutable int g_{}; // cost from start, not part of the set's key
    int h_{}; // heuristic to goal
    // int f_{}; // f(n) = g(n) + h(n)
    using Container = std::vector<std::pair<int, int>>;
    Container stones_;
    Container holes_;
    mutable std::vector<const MazeStar*> children_;
};
/*
namespace std
{
    template<> struct hash<MazeStar>
    {
        std::size_t operator()(MazeStar const& m) const noexcept
        {
            return m.hash();
        }
    };
}
*/
std::ostream& operator<<(std::ostream& out, const MazeStar& maze);

#endif // ___MAZESTAR_H__
