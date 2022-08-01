#ifndef __ASTAR_H__
#define __ASTAR_H__
#include "Maze.h"
#include <vector>
#include <list>
#include <unordered_set>

class AStar;
class MazeStar : public Maze {
public:
    friend class AStar;
    MazeStar& operator=(const MazeVector& maze) override;

    int h() const override {return h_;}
    int g() const override {return g_;};
    int f() const override {return g_ + h_;}

    using Maze::stepLeft;
    using Maze::stepRight;
    using Maze::stepUp;
    using Maze::stepDown;

    // operators: step left, step right, step up, step down
    virtual bool stepLeft(const int targetX, const int targetY);
    virtual bool stepRight(const int targetX, const int targetY);
    virtual bool stepUp(const int targetX, const int targetY);
    virtual bool stepDown(const int targetX, const int targetY);

    bool operator==(const MazeStar& m) const;

    const MazeStar* getMazeStar() const override {return this;}

private:
    virtual void calcG();
    virtual void calcH(const int targetX, const int targetY);

    mutable int g_{}; // cost from start, not part of the set's key
    int h_{}; // heuristic to goal
    // int f_{}; // f(n) = g(n) + h(n)
    using Container = std::vector<std::pair<int, int>>;
    mutable std::vector<const MazeStar*> children_;
};

std::ostream& operator<<(std::ostream& out, const MazeStar& maze);

class AStar
{
public:
    struct MazePComp1 {
        bool operator()(const MazeStar& m1, const MazeStar& m2) const {
            return m1 < m2;
        }
    };

    struct MazePComp2 {
        bool operator()(const MazeStar* m1, const MazeStar* m2) const {
            if(m1->f() > m2->f()) return false;
            if(m1->f() < m2->f()) return true;
            return m1 < m2;
        }
    };

    AStar(int targetX, int targetY);
    bool extend(const MazeStar& maze);
    bool search(bool verbose);
    AStar& operator<<(const MazeStar& maze);
    int depth() const;
    std::vector<Maze::MazeVector> getResult();
    bool isGoal(const MazeStar* maze) const;

private:
    void updateParentPtr(const MazeStar* node, const MazeStar* newParent);
    void updateChilrenG(const MazeStar* node, int gDiff);

    std::set<const MazeStar*, MazePComp2> mazes_;
    std::set<MazeStar, MazePComp1> mazeSet_;
    const Maze* solution_;
    const int targetX_;
    const int targetY_;
};

#endif // __ASTAR_H__
