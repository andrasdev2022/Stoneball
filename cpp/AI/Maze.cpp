#include "Maze.h"
#include "Algorithms.h"
#include "Tileset.h"

template<typename T>
void Unused(T...) {

}

using namespace Algorithms;

Maze::Maze() {

}

void Maze::setValue(int x, int y, char value) {
    maze_[y][x] = value;
    if(value == Tileset::redKnight) {
        heroX_ = x;
        heroY_ = y;
        //std::cout << "Hero position: " << x << ", " << y << "\n";
    }
}

bool Maze::operator<(const Maze& maze) const {
    for(size_t y = 0; y < maze_.size(); ++y) {
        for(size_t x = 0; x < maze_.at(y).size(); ++x) {
            if(maze_.at(y).at(x) != maze.maze_.at(y).at(x)) {
                return maze_.at(y).at(x) < maze.maze_.at(y).at(x);
            }
        }
    }
    return false;
}

bool Maze::isGoal() const {
    //Unused(maze);
    for(size_t y = 0; y < maze_.size(); ++y) {
        for(size_t x = 0; x < maze_.at(y).size(); ++x) {
            if(maze_.at(y).at(x) == Tileset::ball || maze_.at(y).at(x) == Tileset::hole) return false;
        }
    }
    return true;
}

void Maze::printTheSteps(int depth) const {
    std::cout << *this;
    if(parent_) parent_->printTheSteps(depth + 1);
    else std::cout << "Depth: " << depth << "\n";
}

// operators: step left, step right, step up, step down
bool Maze::stepLeft() {
    if(heroX_ == 0 || anyOf(maze_[heroY_][heroX_ - 1], Tileset::wall, Tileset::hole)) return false;
    char c = maze_[heroY_][heroX_ - 1];
    if(c == Tileset::emptyField) {
        maze_[heroY_][heroX_ - 1] = Tileset::redKnight;
        maze_[heroY_][heroX_] = Tileset::emptyField;
        heroX_ -= 1;
    } else {
        return false;
    }
    return true;
}

bool Maze::stepRight() {
    if(heroX_ == (int)maze_.at(heroY_).size() - 1 || anyOf(maze_[heroY_][heroX_ + 1], Tileset::wall, Tileset::hole)) return false;
    char c = maze_[heroY_][heroX_ + 1];
    if(c == Tileset::emptyField) {
        maze_[heroY_][heroX_ + 1] = Tileset::redKnight;
        maze_[heroY_][heroX_] = Tileset::emptyField;
        heroX_ += 1;
    } else {
        return false;
    }
    return true;
}

bool Maze::stepUp() {
    if(heroY_ == 0 || anyOf(maze_[heroY_ - 1][heroX_], Tileset::wall, Tileset::hole)) return false;
    char c = maze_[heroY_ - 1][heroX_];
    if(c == Tileset::emptyField) {
        maze_[heroY_ - 1][heroX_] = Tileset::redKnight;
        maze_[heroY_][heroX_] = Tileset::emptyField;
        heroY_ -= 1;
    } else {
        return false;
    }
    return true;
}

bool Maze::stepDown() {
    if(heroY_ == (int)maze_.size() - 1 || anyOf(maze_[heroY_ + 1][heroX_], Tileset::wall, Tileset::hole)) return false;
    char c = maze_[heroY_ + 1][heroX_];
    if(c == Tileset::emptyField) {
        maze_[heroY_ + 1][heroX_] = Tileset::redKnight;
        maze_[heroY_][heroX_] = Tileset::emptyField;
        heroY_ += 1;
    } else {
        return false;
    }
    return true;
}

Maze& Maze::operator=(const MazeVector& maze) {
    maze_ = maze;
    bool stop = false;
    for(size_t y = 0; y < maze_.size() && !stop; ++y) {
        for(size_t x = 0; x < maze_.at(y).size() && !stop; ++x) {
            if(maze_.at(y).at(x) == Tileset::redKnight) {
                heroX_ = x;
                heroY_ = y;
                stop = true;
            }
        }
    }
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Maze& maze) {
    out << "\n//0123456789\n";
    int rowIndex = -1;
    for(const auto& row : maze.maze_) {
        out  << " \"*";
        for(const auto& col : row) {
            out << col;
        }
        out << "*\\n\" // " << ++rowIndex << "\n";
    }
    return out;
}
