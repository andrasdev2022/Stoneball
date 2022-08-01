#ifndef __BREADTHFIRST_H__
#define __BREADTHFIRST_H__
#include "Maze.h"
#include <list>
#include "AICommon.h"
#include <unordered_set>
/*
namespace std
{
    template<> struct hash<Maze>
    {
        std::size_t operator()(Maze const& m) const noexcept
        {
            return m.hash();
        }
    };
}
*/
class BreadthFirst : public AICommon
{
public:
    struct MazePCompLess {
        bool operator()(const Maze& m1, const Maze& m2) const {
            return m1 < m2;
        }
    };
/*
    struct MazePCompEq {
        bool operator()(const Maze& m1, const Maze& m2) const {
            return m1.hash() == m2.hash();
        }
    };

    struct MazeHash {
        std::size_t operator () (Maze const &m) const {
            return m.hash();
        }
    };
*/
    BreadthFirst();
    bool extend(const Maze& maze, bool verbose);
    void printMemoryUsage();
    bool search(bool verbose);
    BreadthFirst& operator<<(const Maze& maze);
    int depth() const;
    void printFirstState();
    int pushCounter() const noexcept;
    void setMaximumDepth(int depth) noexcept {maximumDepth_ = depth;}

private:
    void storeNewMaze(Maze& newMaze, bool verbose) noexcept;

    std::list<const Maze*> mazes_;
    std::set<Maze, MazePCompLess> mazeSet_;
    //std::unordered_set<Maze, std::hash<Maze>, MazePCompEq> mazeSet_;
    //std::unordered_set<Maze, std::hash<Maze>> mazeSet_;
    const Maze* solution_;
};

#endif // __BREADTHFIRST_H__
