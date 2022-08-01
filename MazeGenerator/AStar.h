#ifndef __ASTAR_H__
#define __ASTAR_H__
#include "MazeStar.h"
#include <vector>
#include <list>
//#include <unordered_set>
#include "AICommon.h"
#include "Math.h"
#include <fstream>
#include "stopwatch.h"
#include <algorithm>
#include <string>

extern StopWatch stopWatch;
extern std::string_view stopWatchKey;
extern void printWatches();

template<typename T>
class AStar : virtual public AICommon
{
public:
    struct MazePComp1 {
        bool operator()(const T& m1, const T& m2) const {
            return m1 < m2;
            return false;
        }
    };

    struct MazePComp2 {
        bool operator()(const MazeStar* m1, const MazeStar* m2) const {
            if(m1->f() < m2->f()) return true;
            if(m1->f() == m2->f() && m1->g() > m2->g()) return true;
            return *m1 < *m2;
        }
    };
/*
    struct MazeHash {
        std::size_t operator () (MazeStar const &m) const {
            return m.hash();
        }

        bool operator()(const MazeStar& m1, const MazeStar& m2) const {
            bool equal1 = m1 == m2;
            bool equal2 = Math::abs(m1.hashNumber() - m2.hashNumber()) < 1e-7;
            if(equal1 != equal2) {
                std:: cout << m1 << m2 << "\n";
                std::cout << m1.teleportCardCount() << ", " << m1.pushCardCount() << ", " << m1.pullCardCount() << "\n";
                std::cout << m2.teleportCardCount() << ", " << m2.pushCardCount() << ", " << m2.pullCardCount() << "\n";
                std::cout << std::boolalpha << "equal1: " << equal1 << ", equal2: " << equal2 << ", m1.hashNumber(): " << m1.hashNumber() <<
                             ", m2.hashNumber(): " << m2.hashNumber() << "\n";
            }
            assert(equal1 == equal2);
            return equal1;
            return Math::abs(m1.hashNumber() - m2.hashNumber()) < 1e-7;
        }
    };
*/
    AStar();
    virtual void extend(const MazeStar* maze) noexcept;
    virtual void extendBackward(const MazeStar* maze) noexcept;
    virtual void extendForwardWithPushAlteration(const MazeStar* maze) noexcept;
    virtual void printMemoryUsage();
    virtual bool search(bool verbose, bool findBest) noexcept;
    virtual AStar& operator<<(const T& maze);
    virtual AStar& operator<<=(const T& maze); // for backward
    virtual void printStates() const;
    virtual int depth() const;
    virtual void printFirstState();
    virtual void printFirstStateBackward();
    virtual std::string stepString() const noexcept;
    virtual int pushCounter() const noexcept;
    virtual int pushCounterBackward() const noexcept;
    virtual const Maze* solution() const noexcept {return solution_;}
    virtual const Maze* solutionBackward() const noexcept {return solutionBackward_;}
    virtual void setExportNodesForNN(bool v) noexcept {exportNodesForNN_ =v;}
    virtual void setTimeout(double timeout) noexcept {timeout_ = timeout;}
    virtual bool testSteps(std::string steps) noexcept;
    static std::string convertBackwardSteps(std::string str) noexcept;
    virtual const T& testStepsResult() const noexcept {return testStepsResult_;}
    static int pushNumberFromSteps(std::string steps) noexcept;
    static int lines(std::string steps) noexcept;
    virtual void enablePushAlteration() noexcept {pushAlterationEnabled_ = true;}

protected:
    virtual void updateParentPtr(const MazeStar* node, const MazeStar* newParent);
    virtual void updateChilrenG(const MazeStar* node, int gDiff, int pushCounterDiff, int depthDiff, std::multiset<const MazeStar*, MazePComp2>& mazes);
    virtual void storeNewMaze(const MazeStar* maze, T& newMaze) noexcept;
    virtual void storeNewMazeBackward(const MazeStar* maze, T& newMaze) noexcept;
    virtual void relinkCheaperPath(const MazeStar* oldMaze, T& newMaze, const MazeStar* parent, std::multiset<const MazeStar*, MazePComp2>& mazes) noexcept;
    virtual void exportNodesForNN() noexcept;
    virtual std::string stepString(const Maze* solution) const noexcept;
    virtual bool searchForward() noexcept;
    virtual bool searchBackward() noexcept;
    virtual void extendForwardOneState(const MazeStar* maze, bool (MazeStar::*f)()noexcept, int heroX, int heroY) noexcept;

    std::multiset<const MazeStar*, MazePComp2> mazes_;
    std::set<T, MazePComp1> mazeSet_;
    std::multiset<const MazeStar*, MazePComp2> mazesBackward_;
    std::set<T, MazePComp1> mazeSetBackward_;
    //std::unordered_set<MazeStar, MazeHash, MazeHash> mazeSet_;
    const Maze* solution_{};
    const Maze* solutionBackward_{};
    bool exportNodesForNN_{};
    bool timesUp_{};
    double timeout_{120e6};
    bool verbose_{};
    const T* backwardGoal_{};
    size_t extendCount_{};
    T testStepsResult_;
    bool findBest_{};
    bool pushAlterationEnabled_{};
};

using AStarV2 = AStar<MazeStarV2>;
using AStarV3 = AStar<MazeStarV3>;

#include "AStar.hpp"


#endif // __ASTAR_H__
