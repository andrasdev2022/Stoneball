#ifndef __MAZE_H__
#define __MAZE_H__
#include <iostream>
#include <array>
#include <vector>
#include <set>
#include <iomanip>
#include <assert.h>
/*
constexpr int mazeWidth = 7;
constexpr int mazeHeight = 8;
constexpr int stoneCount = 2;
constexpr int wallCount = 5;
*/

//constexpr char stone = 'o';
//constexpr char hole = 'x';
//constexpr char hero = 'h';
//constexpr char wall = '*';
//constexpr char space = ' ';

class MazeStar;

class Maze
{
    friend std::ostream& operator<<(std::ostream& out, const Maze& maze);
    friend bool extend(const Maze& maze);
    friend class BreadthFirst;
    friend class AStar;
public:
    using MazeVector = std::vector<std::vector<char>>;

    Maze();
    virtual ~Maze(){}

    virtual void setValue(int x, int y, char value);
    virtual bool operator<(const Maze& maze) const;
    virtual bool isGoal() const;
    virtual void printTheSteps(int depth = 1) const;

    // operators: step left, step right, step up, step down
    virtual bool stepLeft();
    virtual bool stepRight();
    virtual bool stepUp();
    virtual bool stepDown();

    virtual Maze& operator=(const MazeVector& maze);

    virtual int h() const {return 0;}
    virtual int g() const {return 0;}
    virtual int f() const {return 0;}

    virtual int heroX() const {return heroX_;}
    virtual int heroY() const {return heroY_;}

    virtual const MazeVector& maze() const {return maze_;}

    virtual const MazeStar* getMazeStar() const {return nullptr;}

    MazeVector maze_;
    [[nodiscard]] const Maze* parent() const {return parent_;}

private:
    const mutable Maze* parent_{}; // it's not part of the set's key
    int heroX_{};
    int heroY_{};
};

std::ostream& operator<<(std::ostream& out, const Maze& maze);

#endif // __MAZE_H__
