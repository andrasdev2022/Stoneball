#include "Maze.h"
#include "Algorithms.h"

template<typename T>
void Unused(T...) {

}

using namespace Algorithms;
/*
void Maze::setValue(int x, int y, char value) {
    maze_[y][x] = value;
    if(value == hero) {
        heroX_ = x;
        heroY_ = y;
        //std::cout << "Hero position: " << x << ", " << y << "\n";
    }
}
*/
bool Maze::operator<(const Maze& maze) const {
    for(size_t y = 0; y < mazeHeight; ++y) {
        for(size_t x = 0; x < mazeWidth; ++x) {
            if(maze_.at(y).at(x) != maze.maze_.at(y).at(x)) {
                return maze_.at(y).at(x) < maze.maze_.at(y).at(x);
            }
        }
    }
    if(teleportCardCount_ > maze.teleportCardCount_) return true;
    if(pushCardCount_ > maze.pushCardCount_) return true;
    if(pullCardCount_ > maze.pullCardCount_) return true;
    return false;
}

bool Maze::operator==(const Maze& maze) const {
    for(size_t y = 0; y < mazeHeight; ++y) {
        for(size_t x = 0; x < mazeWidth; ++x) {
            if(maze_.at(y).at(x) != maze.maze_.at(y).at(x)) {
                return false;
            }
        }
    }
    if(teleportCardCount_ != maze.teleportCardCount_) return false;
    if(pushCardCount_ != maze.pushCardCount_) return false;
    if(pullCardCount_ != maze.pullCardCount_) return false;
    return true;
}

bool Maze::operator==(const MazeArray& maze) const {
    for(size_t y = 0; y < mazeHeight; ++y) {
        for(size_t x = 0; x < mazeWidth; ++x) {
            if(maze_.at(y).at(x) != maze.at(y).at(x)) {
                return false;
            }
        }
    }
    return true;
}
bool Maze::isGoal() const {
    //Unused(maze);
    return !(stoneCount_ + holeCount_);
/*
    for(size_t y = 0; y < mazeHeight; ++y) {
        for(size_t x = 0; x < mazeWidth; ++x) {
            if(maze_.at(y).at(x) == stone || maze_.at(y).at(x) == hole) return false;
        }
    }
    return true;
*/
}

void Maze::printStates(int depth) const {
    std::cout << *this;
    printCostAndHeuristic();
    if(parent_) {
        parent_->printStates(depth + 1);
    } else {
        std::cout << "Depth: " << depth << "\n";
    }
}

// operators: step left, step right, step up, step down
bool Maze::stepLeft()  noexcept {
    if(heroX_ == 0 || anyOf(maze_[heroY_][heroX_ - 1], wall, hole)) return false;

    // we do not step back to the original position
    // TODO: this check is simpler with op_
    if(parent_ && parent_->parent_ && parent_->parent_->pushCounter_ == pushCounter_ &&
    parent_->parent_->heroX_ == heroX_ - 1 && parent_->parent_->heroY_ == heroY_) {
        return false;
    }

    char c = maze_[heroY_][heroX_ - 1];
    if(c == space) {
        maze_[heroY_][heroX_ - 1] = hero;
        maze_[heroY_][heroX_] = space;
        heroX_ -= 1;
        op_ = 'l';
    } else if(c == stone) {
        if(heroX_ <= 1) return false;
        if(anyOf(maze_[heroY_][heroX_ - 2], stone, wall, hero2)) return false;
        char c2 = maze_[heroY_][heroX_ - 2];
        if(c2 == hole) {
            maze_[heroY_][heroX_ - 2] = space;
            maze_[heroY_][heroX_ - 1] = hero;
            maze_[heroY_][heroX_] = space;

            removeHole(heroX_ - 2, heroY_);
            removeStone(heroX_ - 1, heroY_);
            heroX_ -= 1;
            ++pushCounter_;
            op_ = 'L';
        } else if(c2 == space){
            // we do not push back a ball what we pulled in the previous turn
            // TODO: this check is simpler with op_
            if(parent_ && parent_->parent_ && parent_->parent_->pushCounter_ + 1 == pushCounter_ &&
            parent_->parent_->heroX_ == heroX_ - 1 && parent_->parent_->heroY_ == heroY_ &&
            parent_->parent_->pullCardCount_ - 1 == pullCardCount_) {
                return false;
            }

            maze_[heroY_][heroX_ - 2] = stone;
            maze_[heroY_][heroX_ - 1] = hero;
            maze_[heroY_][heroX_] = space;

            removeStone(heroX_ - 1, heroY_);
            addStone(heroX_ - 2, heroY_);
            heroX_ -= 1;
            ++pushCounter_;
            op_ = 'L';
        }
    } else if(c == hero2) { // using push card
        if(!pushCardCount_ || heroX_ <= 1) return false;
        if(anyOf(maze_[heroY_][heroX_ - 2], wall, hero2, hole)) return false;
        char c2 = maze_[heroY_][heroX_ - 2];
        if(c2 == space){
            maze_[heroY_][heroX_ - 2] = hero2;
            maze_[heroY_][heroX_ - 1] = hero;
            maze_[heroY_][heroX_] = space;

            removeHero2(heroX_ - 1, heroY_);
            addHero2(heroX_ - 2, heroY_);
            heroX_ -= 1;
            --pushCardCount_;
            ++pushCounter_;
            op_ = 'L';
        } else if(c2 == stone) {
            if(heroX_ <= 2 || maze_[heroY_][heroX_ - 3] != space) return false;
            maze_[heroY_][heroX_ - 3] = stone;
            maze_[heroY_][heroX_ - 2] = hero2;
            maze_[heroY_][heroX_ - 1] = hero;
            maze_[heroY_][heroX_] = space;

            removeStone(heroX_ - 2, heroY_);
            addStone(heroX_ - 3, heroY_);
            removeHero2(heroX_ - 1, heroY_);
            addHero2(heroX_ - 2, heroY_);
            heroX_ -= 1;
            --pushCardCount_;
            pushCounter_ += 2;
            op_ = 'L';
        }
    } else {
        std::cerr << "c: " << c << ", heroX_: " << (int)heroX_ << ", heroY_: " << (int)heroY_ << std::endl;
        std::cerr << *this << std::endl;
        assert(0);
    }
    return true;
}

bool Maze::stepRight() noexcept {
    if(heroX_ == mazeWidth - 1 || anyOf(maze_[heroY_][heroX_ + 1], wall, hole)) return false;

    // we do not step back to the original position
    if(parent_ && parent_->parent_ && parent_->parent_->pushCounter_ == pushCounter_ &&
    parent_->parent_->heroX_ == heroX_ + 1 && parent_->parent_->heroY_ == heroY_) {
        return false;
    }

    char c = maze_[heroY_][heroX_ + 1];
    if(c == space) {
        maze_[heroY_][heroX_ + 1] = hero;
        maze_[heroY_][heroX_] = space;
        heroX_ += 1;
        op_ = 'r';
    } else if(c == stone) {
        if(heroX_ >= mazeWidth - 2) return false;
        if(anyOf(maze_[heroY_][heroX_ + 2], stone, wall, hero2)) return false;
        char c2 = maze_[heroY_][heroX_ + 2];
        if(c2 == hole) {
            maze_[heroY_][heroX_ + 2] = space;
            maze_[heroY_][heroX_ + 1] = hero;
            maze_[heroY_][heroX_] = space;

            removeHole(heroX_ + 2, heroY_);
            removeStone(heroX_ + 1, heroY_);
            heroX_ += 1;
            ++pushCounter_;
            op_ = 'R';
        } else if(c2 == space){
            // TODO: we do not push back a ball what we pulled in the previous turn
            if(parent_ && parent_->parent_ && parent_->parent_->pushCounter_ + 1 == pushCounter_ &&
            parent_->parent_->heroX_ == heroX_ + 1 && parent_->parent_->heroY_ == heroY_ &&
            parent_->parent_->pullCardCount_ - 1 == pullCardCount_) {
                return false;
            }

            maze_[heroY_][heroX_ + 2] = stone;
            maze_[heroY_][heroX_ + 1] = hero;
            maze_[heroY_][heroX_] = space;

            removeStone(heroX_ + 1, heroY_);
            addStone(heroX_ + 2, heroY_);
            heroX_ += 1;
            ++pushCounter_;
            op_ = 'R';
        }
    } else if(c == hero2) { // using push card
        if(!pushCardCount_ || heroX_ >= mazeWidth - 2) return false;
        if(anyOf(maze_[heroY_][heroX_ + 2], wall, hero2, hole)) return false;
        char c2 = maze_[heroY_][heroX_ + 2];
        if(c2 == space){
            maze_[heroY_][heroX_ + 2] = hero2;
            maze_[heroY_][heroX_ + 1] = hero;
            maze_[heroY_][heroX_] = space;

            removeHero2(heroX_ + 1, heroY_);
            addHero2(heroX_ + 2, heroY_);
            heroX_ += 1;
            --pushCardCount_;
            ++pushCounter_;
            op_ = 'R';
        } else if(c2 == stone) {
            if(heroX_ >= mazeWidth - 3 || maze_[heroY_][heroX_ + 3] != space) return false;
            maze_[heroY_][heroX_ + 3] = stone;
            maze_[heroY_][heroX_ + 2] = hero2;
            maze_[heroY_][heroX_ + 1] = hero;
            maze_[heroY_][heroX_] = space;

            removeStone(heroX_ + 2, heroY_);
            addStone(heroX_ + 3, heroY_);
            removeHero2(heroX_ + 1, heroY_);
            addHero2(heroX_ + 2, heroY_);
            heroX_ += 1;
            --pushCardCount_;
            pushCounter_ += 2;
            op_ = 'R';
        }
    } else {
        std::cerr << "c: " << c << ", heroX_: " << (int)heroX_ << ", heroY_: " << (int)heroY_ << std::endl;
        assert(0);
    }
    return true;
}

bool Maze::stepUp() noexcept {
    if(heroY_ == 0 || anyOf(maze_[heroY_ - 1][heroX_], wall, hole)) return false;

    // we do not step back to the original position
    if(parent_ && parent_->parent_ && parent_->parent_->pushCounter_ == pushCounter_ &&
    parent_->parent_->heroX_ == heroX_ && parent_->parent_->heroY_ == heroY_ - 1) {
        return false;
    }

    char c = maze_[heroY_ - 1][heroX_];
    if(c == space) {
        maze_[heroY_ - 1][heroX_] = hero;
        maze_[heroY_][heroX_] = space;
        heroY_ -= 1;
        op_ = 'u';
    } else if(c == stone) {
        if(heroY_ <= 1 ) return false;
        if(anyOf(maze_[heroY_ - 2][heroX_], stone, wall, hero2)) return false;
        char c2 = maze_[heroY_ - 2][heroX_];
        if(c2 == hole) {
            maze_[heroY_ - 2][heroX_] = space;
            maze_[heroY_ - 1][heroX_] = hero;
            maze_[heroY_][heroX_] = space;

            removeHole(heroX_, heroY_ - 2);
            removeStone(heroX_, heroY_ - 1);
            heroY_ -= 1;
            ++pushCounter_;
            op_ = 'U';
        } else if(c2 == space){
            // TODO: we do not push back a ball what we pulled in the previous turn
            if(parent_ && parent_->parent_ && parent_->parent_->pushCounter_ + 1 == pushCounter_ &&
            parent_->parent_->heroX_ == heroX_ && parent_->parent_->heroY_ == heroY_ - 1 &&
            parent_->parent_->pullCardCount_ - 1 == pullCardCount_) {
                return false;
            }

            maze_[heroY_ - 2][heroX_] = stone;
            maze_[heroY_ - 1][heroX_] = hero;
            maze_[heroY_][heroX_] = space;

            removeStone(heroX_, heroY_ - 1);
            addStone(heroX_, heroY_ - 2);
            heroY_ -= 1;
            ++pushCounter_;
            op_ = 'U';
        }
    } else if(c == hero2) { // using push card
        if(!pushCardCount_ || heroY_ <= 1) return false;
        if(anyOf(maze_[heroY_ - 2][heroX_], wall, hero2, hole)) return false;
        char c2 = maze_[heroY_ - 2][heroX_];
        if(c2 == space){
            maze_[heroY_ - 2][heroX_] = hero2;
            maze_[heroY_ - 1][heroX_] = hero;
            maze_[heroY_][heroX_] = space;

            removeHero2(heroX_, heroY_ - 1);
            addHero2(heroX_, heroY_ - 2);
            heroY_ -= 1;
            --pushCardCount_;
            ++pushCounter_;
            op_ = 'U';
        } else if(c2 == stone){
            if(heroY_ <= 2 || maze_[heroY_ - 3][heroX_] != space) return false;
            maze_[heroY_ - 3][heroX_] = stone;
            maze_[heroY_ - 2][heroX_] = hero2;
            maze_[heroY_ - 1][heroX_] = hero;
            maze_[heroY_][heroX_] = space;

            removeStone(heroX_, heroY_ - 2);
            addStone(heroX_, heroY_ - 3);
            removeHero2(heroX_, heroY_ - 1);
            addHero2(heroX_, heroY_ - 2);
            heroY_ -= 1;
            --pushCardCount_;
            pushCounter_ += 2;
            op_ = 'U';
        }
    } else {
        std::cerr << "c: " << c << ", heroX_: " << (int)heroX_ << ", heroY_: " << (int)heroY_ << std::endl;
        assert(0);
    }
    return true;
}

bool Maze::stepDown() noexcept {
    if(heroY_ == mazeHeight - 1 || anyOf(maze_[heroY_ + 1][heroX_], wall, hole)) return false;

    // we do not step back to the original position
    if(parent_ && parent_->parent_ && parent_->parent_->pushCounter_ == pushCounter_ &&
    parent_->parent_->heroX_ == heroX_ && parent_->parent_->heroY_ == heroY_ + 1) {
        return false;
    }

    char c = maze_[heroY_ + 1][heroX_];
    if(c == space) {
        maze_[heroY_ + 1][heroX_] = hero;
        maze_[heroY_][heroX_] = space;
        heroY_ += 1;
        op_ = 'd';
    } else if(c == stone) {
        if(heroY_ >= mazeHeight - 2 ) return false;
        if(anyOf(maze_[heroY_ + 2][heroX_], stone, wall, hero2)) return false;
        char c2 = maze_[heroY_ + 2][heroX_];
        if(c2 == hole) {
            maze_[heroY_ + 2][heroX_] = space;
            maze_[heroY_ + 1][heroX_] = hero;
            maze_[heroY_][heroX_] = space;

            removeHole(heroX_, heroY_ + 2);
            removeStone(heroX_, heroY_ + 1);
            heroY_ += 1;
            ++pushCounter_;
            op_ = 'D';
        } else if(c2 == space){
            // TODO: we do not push back a ball what we pulled in the previous turn
            if(parent_ && parent_->parent_ && parent_->parent_->pushCounter_ + 1 == pushCounter_ &&
            parent_->parent_->heroX_ == heroX_ && parent_->parent_->heroY_ == heroY_ + 1 &&
            parent_->parent_->pullCardCount_ - 1 == pullCardCount_) {
                return false;
            }

            maze_[heroY_ + 2][heroX_] = stone;
            maze_[heroY_ + 1][heroX_] = hero;
            maze_[heroY_][heroX_] = space;

            removeStone(heroX_, heroY_ + 1);
            addStone(heroX_, heroY_ + 2);
            heroY_ += 1;
            ++pushCounter_;
            op_ = 'D';
        }
    } else if(c == hero2) { // using push card
        if(!pushCardCount_ || heroY_ >= mazeHeight - 2) return false;
        if(anyOf(maze_[heroY_ + 2][heroX_], wall, hero2, hole)) return false;
        char c2 = maze_[heroY_ + 2][heroX_];
        if(c2 == space){
            maze_[heroY_ + 2][heroX_] = hero2;
            maze_[heroY_ + 1][heroX_] = hero;
            maze_[heroY_][heroX_] = space;

            removeHero2(heroX_, heroY_ + 1);
            addHero2(heroX_, heroY_ + 2);
            heroY_ += 1;
            --pushCardCount_;
            ++pushCounter_;
            op_ = 'D';
        } else if(c2 == stone) {
            if(heroY_ >= mazeHeight - 3 || maze_[heroY_ + 3][heroX_] != space) return false;
            maze_[heroY_ + 3][heroX_] = stone;
            maze_[heroY_ + 2][heroX_] = hero2;
            maze_[heroY_ + 1][heroX_] = hero;
            maze_[heroY_][heroX_] = space;

            removeStone(heroX_, heroY_ + 2);
            addStone(heroX_, heroY_ + 3);
            removeHero2(heroX_, heroY_ + 1);
            addHero2(heroX_, heroY_ + 2);
            heroY_ += 1;
            --pushCardCount_;
            pushCounter_ += 2;
            op_ = 'D';
        }
    } else {
        std::cerr << "c: " << c << ", heroX_: " << (int)heroX_ << ", heroY_: " << (int)heroY_ << std::endl;
        assert(0);
    }
    return true;
}

bool Maze::teleport(int x, int y) noexcept {
    if(!teleportCardCount_ || maze_[y][x] != space) return false;

    // we do not teleport to the neighbour fields
    if((abs(x - heroX_) == 1 && y == heroY_) || (x == heroX_ && abs(y - heroY_) <= 1)) return false;

    maze_[heroY_][heroX_] = space;
    maze_[y][x] = hero;
    heroX_ = x;
    heroY_ = y;
    --teleportCardCount_;
    op_ = 't';
    return true;
}

bool Maze::pullLeft() noexcept {
    if(!pullCardCount_ || heroX_ == 0 || heroX_ == mazeWidth - 1 || maze_[heroY_][heroX_ + 1] != stone || maze_[heroY_][heroX_ - 1] != space) {
        return false;
    }

    // TODO: we do not pull back a ball what we pushed in the previous turn
    /*
    if(parent_ && parent_->parent_ && parent_->parent_->pushCounter_ + 1 == pushCounter_ &&
    parent_->parent_->heroX_ == heroX_ - 1 && parent_->parent_->heroY_ == heroY_ &&
    parent_->parent_->pullCardCount_ == pullCardCount_) {
        return false;
    }*/

    maze_[heroY_][heroX_ - 1] = hero;
    maze_[heroY_][heroX_] = stone;
    maze_[heroY_][heroX_ + 1] = space;

    removeStone(heroX_ + 1, heroY_);
    addStone(heroX_, heroY_);
    heroX_ -= 1;
    --pullCardCount_;
    ++pushCounter_;
    op_ = 'p';
    return true;
}

bool Maze::pullRight() noexcept {
    if(!pullCardCount_ || heroX_ == 0 || heroX_ == mazeWidth - 1 || maze_[heroY_][heroX_ - 1] != stone || maze_[heroY_][heroX_ + 1] != space) {
        return false;
    }

    // TODO: we do not pull back a ball what we pushed in the previous turn

    maze_[heroY_][heroX_ + 1] = hero;
    maze_[heroY_][heroX_] = stone;
    maze_[heroY_][heroX_ - 1] = space;

    removeStone(heroX_ - 1, heroY_);
    addStone(heroX_, heroY_);
    heroX_ += 1;
    --pullCardCount_;
    ++pushCounter_;
    op_ = 'P';
    return true;
}

bool Maze::pullUp() noexcept {
    if(!pullCardCount_ || heroY_ == 0 || heroY_ == mazeHeight - 1 || maze_[heroY_ + 1][heroX_] != stone || maze_[heroY_ - 1][heroX_] != space) {
        return false;
    }

    // TODO: we do not pull back a ball what we pushed in the previous turn

    maze_[heroY_ - 1][heroX_] = hero;
    maze_[heroY_][heroX_] = stone;
    maze_[heroY_ + 1][heroX_] = space;

    removeStone(heroX_, heroY_ + 1);
    addStone(heroX_, heroY_);
    heroY_ -= 1;
    --pullCardCount_;
    ++pushCounter_;
    op_ = 'q';
    return true;
}

bool Maze::pullDown() noexcept {
    if(!pullCardCount_ || heroY_ == 0 || heroY_ == mazeHeight - 1 || maze_[heroY_ - 1][heroX_] != stone || maze_[heroY_ + 1][heroX_] != space) {
        return false;
    }

    // TODO: we do not pull back a ball what we pushed in the previous turn

    maze_[heroY_ + 1][heroX_] = hero;
    maze_[heroY_][heroX_] = stone;
    maze_[heroY_ - 1][heroX_] = space;

    removeStone(heroX_, heroY_ - 1);
    addStone(heroX_, heroY_);
    heroY_ += 1;
    --pullCardCount_;
    ++pushCounter_;
    op_ = 'Q';
    return true;
}

Maze& Maze::operator=(const MazeArray& maze) {
    maze_ = maze;
    stoneCount_ = 0;
    holeCount_ = 0;
    for(size_t y = 0; y < mazeHeight; ++y) {
        for(size_t x = 0; x < mazeWidth; ++x) {
            char v = maze_.at(y).at(x);
            if(v == hero) {
                heroX_ = x;
                heroY_ = y;
            } else if(v == stone) {
                ++stoneCount_;
            } else if(v == hole) {
                ++holeCount_;
            }
        }
    }
    //makeHashKey();
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Maze& maze) {
    out << "\n//0123456789\n";
    int rowIndex = -1;
    for(const auto& row : maze.maze_) {
        out  << " \"*";
        for(const char& col : row) {
            out << col;
        }
        out << "*\\n\" // " << ++rowIndex << "\n";
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Maze::MazeArray& maze) {
    out << "\n//0123456789\n";
    int rowIndex = -1;
    for(const auto& row : maze) {
        out  << " \"*";
        for(const char& col : row) {
            out << col;
        }
        out << "*\\n\" // " << ++rowIndex << "\n";
    }
    return out;
}

void Maze::addStone(int x, int y) {
    Unused(x, y);
    ++stoneCount_;
}

void Maze::removeStone(int x, int y) {
    Unused(x, y);
    --stoneCount_;
}

void Maze::removeHole(int x, int y) {
    Unused(x, y);
    --holeCount_;
}

void Maze::addHole(int x, int y) {
    Unused(x, y);
    ++holeCount_;
}

void Maze::setCardsAmount(int teleport, int push, int pull) noexcept {
    teleportCardCount_ = teleport;
    pushCardCount_ = push;
    pullCardCount_ = pull;
    //makeHashKey();
}

void Maze::addHero2(int x, int y) noexcept {
    Unused(x, y);
}

void Maze::removeHero2(int x, int y) noexcept {
    Unused(x, y);
}

bool Maze::testTopLeftCornerAnyOf(Maze::Neighbour neighbour, char value1, char value2) const noexcept {
    switch(neighbour) {
    case Neighbour::ITSELF: return maze_.at(0).at(0) == value1 || maze_.at(0).at(0) == value2;
    case Neighbour::LEFT:
    case Neighbour::TOP: return false;
    case Neighbour::RIGHT: return maze_.at(0).at(1) == value1 || maze_.at(0).at(1) == value2;
    case Neighbour::BOTTOM: return maze_.at(1).at(0) == value1 || maze_.at(1).at(0) == value2;
    }
    return false;
}

bool Maze::testTopRightCornerAnyOf(Maze::Neighbour neighbour, char value1, char value2) const noexcept {
    switch(neighbour) {
    case Neighbour::ITSELF: return maze_.at(0).at(mazeWidth - 1) == value1 || maze_.at(0).at(mazeWidth - 1) == value2;
    case Neighbour::RIGHT:
    case Neighbour::TOP: return false;
    case Neighbour::LEFT: return maze_.at(0).at(mazeWidth - 2) == value1 || maze_.at(0).at(mazeWidth - 2) == value2;
    case Neighbour::BOTTOM: return maze_.at(1).at(mazeWidth - 1) == value1 || maze_.at(1).at(mazeWidth - 1) == value2;
    }
    return false;
}

bool Maze::testBottomLeftCornerAnyOf(Maze::Neighbour neighbour, char value1, char value2) const noexcept {
    switch(neighbour) {
    case Neighbour::ITSELF: return maze_.at(mazeHeight - 1).at(0) == value1 || maze_.at(mazeHeight - 1).at(0) == value2;
    case Neighbour::LEFT:
    case Neighbour::BOTTOM: return false;
    case Neighbour::RIGHT: return maze_.at(mazeHeight - 1).at(1) == value1 || maze_.at(mazeHeight - 1).at(1) == value2;
    case Neighbour::TOP: return maze_.at(mazeHeight - 2).at(0) == value1 || maze_.at(mazeHeight - 2).at(0) == value2;
    }
    return false;
}

bool Maze::testBottomRightCornerAnyOf(Maze::Neighbour neighbour, char value1, char value2) const noexcept {
    switch(neighbour) {
    case Neighbour::ITSELF: return maze_.at(mazeHeight - 1).at(mazeWidth - 1) == value1 || maze_.at(mazeHeight - 1).at(mazeWidth - 1) == value2;
    case Neighbour::RIGHT:
    case Neighbour::BOTTOM: return false;
    case Neighbour::LEFT: return maze_.at(mazeHeight - 1).at(mazeWidth - 2) == value1 || maze_.at(mazeHeight - 1).at(mazeWidth - 2) == value2;
    case Neighbour::TOP: return maze_.at(mazeHeight - 2).at(mazeWidth - 1) == value1 || maze_.at(mazeHeight - 2).at(mazeWidth - 1) == value2;
    }
    return false;
}

bool Maze::testPositionAnyOf(int x, int y, char value1, char value2, bool returnValueInCaseOfOutOfBorder /*false*/) const noexcept {
    if(x < 0 || x >= mazeWidth || y < 0 || y >= mazeHeight) return returnValueInCaseOfOutOfBorder;
    return maze_.at(y).at(x) == value1 || maze_.at(y).at(x) == value2;
}

bool Maze::testPosition(int x, int y, char value, bool returnValueInCaseOfOutOfBorder /*false*/) const noexcept {
    if(x < 0 || x >= mazeWidth || y < 0 || y >= mazeHeight) return returnValueInCaseOfOutOfBorder;
    return maze_.at(y).at(x) == value;
}

bool Maze::testPositionIsValAndSurroundedBySVal(int x, int y, char value, char surroundingValue) const noexcept {
    if(testPosition(x, y, value) &&
    testPosition(x - 1, y, surroundingValue, true) && testPosition(x + 1, y, surroundingValue, true) &&
    testPosition(x, y - 1, surroundingValue, true) && testPosition(x, y + 1, surroundingValue, true)) {
        return true;
    }
    return false;
}

bool Maze::testPositionIsVal2AndSurroundedBySVal(int x, int y, char value1, char value2, char surroundingValue) const noexcept {
    if(testPositionAnyOf(x, y, value1, value2) && testPosition(x - 1, y, surroundingValue, true) && testPosition(x + 1, y, surroundingValue, true) &&
    testPosition(x, y - 1, surroundingValue, true) && testPosition(x, y + 1, surroundingValue, true)) {
        return true;
    }
    return false;
}

bool Maze::testPositionIsVal2AndSurroundedBySVals(int x, int y, char value1, char value2, char surroundingValue1, char surroundingValue2) const noexcept {
    if(testPositionAnyOf(x, y, value1, value2) &&
    testPositionAnyOf(x - 1, y, surroundingValue1, surroundingValue2,true) &&
    testPositionAnyOf(x + 1, y, surroundingValue1, surroundingValue2, true) &&
    testPositionAnyOf(x, y - 1, surroundingValue1, surroundingValue2, true) &&
    testPositionAnyOf(x, y + 1, surroundingValue1, surroundingValue2, true)) {
        return true;
    }
    return false;
}

// meaning: hero came from left side, let's restore the original status
// From ___h__ to __h___
// From __oh__ to _oh___
// From __Hh__ to _Hh___ we cannot pull a hero2 (original operator would be pull hero2)
bool Maze::stepLeftBackward() noexcept { // or stepRightBackward?
    // the goal is unknown here thus the stone pushing into a hole and the hole's disappearence have to be examined
    // in the search algorithm
    if(heroX_ == 0 || anyOf(maze_[heroY_][heroX_ - 1], wall, hole, hero2)) return false;

    // we do not step back to the original position
    if(parent_ && parent_->parent_ && parent_->parent_->pushCounter_ == pushCounter_ &&
    parent_->parent_->heroX_ == heroX_ - 1 && parent_->parent_->heroY_ == heroY_) {
        return false;
    }

    char c = maze_[heroY_][heroX_ - 1];
    if(c == space) { // it's OK
        maze_[heroY_][heroX_] = space;
        maze_[heroY_][heroX_ - 1] = hero;
        op_ = 'l';
        heroX_ -= 1;
    } else if(c == stone) {// hero pulled a stone, we restore the situation before the pull
        if(heroX_ <= 1 || maze_[heroY_][heroX_ - 2] != space || pullCardCount() == maximumPullCardCount_) return false;
        // TODO: we do not push back a ball what we pulled in the previous turn
        if(parent_ && parent_->parent_ && parent_->parent_->pushCounter_ + 1 == pushCounter_ &&
        parent_->parent_->heroX_ == heroX_ - 1 && parent_->parent_->heroY_ == heroY_ &&
        parent_->parent_->pullCardCount_ == pullCardCount_) {
            return false;
        }

        maze_[heroY_][heroX_ - 2] = stone;
        maze_[heroY_][heroX_ - 1] = hero;
        maze_[heroY_][heroX_] = space;
        removeStone(heroX_ - 1, heroY_);
        addStone(heroX_ - 2, heroY_);
        heroX_ -= 1;
        ++pushCounter_;
        ++pullCardCount_;
        op_ = 'P'; // original move: pull a stone right
    } else {
        std::cerr << "c: " << c << ", heroX_: " << (int)heroX_ << ", heroY_: " << (int)heroY_ << std::endl;
        assert(0);
    }
    return true;
}

// From __h___ to ___h__
// From _ho___ to __ho__
// From _hH___ to __hH__ we cannot pull a hero2 (original operator would be pull hero2)
bool Maze::stepRightBackward() noexcept {
    if(heroX_ >= mazeWidth - 1 || anyOf(maze_[heroY_][heroX_ + 1], wall, hole, hero2)) return false;

    // we do not step back to the original position
    if(parent_ && parent_->parent_ && parent_->parent_->pushCounter_ == pushCounter_ &&
    parent_->parent_->heroX_ == heroX_ + 1 && parent_->parent_->heroY_ == heroY_) {
        return false;
    }

    char c = maze_[heroY_][heroX_ + 1];
    if(c == space) {
        maze_[heroY_][heroX_] = space;
        maze_[heroY_][heroX_ + 1] = hero;
        op_ = 'r';
        heroX_ += 1;
    } else if(c == stone) {
        if(heroX_ >= mazeWidth - 2 || maze_[heroY_][heroX_ + 2] != space || pullCardCount() == maximumPullCardCount_) return false;
        // TODO: we do not push back a ball what we pulled in the previous turn
        if(parent_ && parent_->parent_ && parent_->parent_->pushCounter_ + 1 == pushCounter_ &&
        parent_->parent_->heroX_ == heroX_ + 1 && parent_->parent_->heroY_ == heroY_ &&
        parent_->parent_->pullCardCount_ == pullCardCount_) {
            return false;
        }

        maze_[heroY_][heroX_ + 2] = stone;
        maze_[heroY_][heroX_ + 1] = hero;
        maze_[heroY_][heroX_] = space;

        removeStone(heroX_ + 1, heroY_);
        addStone(heroX_ + 2, heroY_);
        heroX_ += 1;
        ++pushCounter_;
        ++pullCardCount_;
        op_ = 'p'; // original move: pull a stone left
    } else {
        std::cerr << "c: " << c << ", heroX_: " << (int)heroX_ << ", heroY_: " << (int)heroY_ << std::endl;
        assert(0);
    }
    return true;
}

// From:  to:      From:  to:        From:  to:  we cannot pull a hero2 (original operator would be pull hero2)
//      _    _          _    o            _    H
//      _    h          o    h            H    h
//      h    _          h    _            h    _
//      _    _          _    _            _    _
bool Maze::stepUpBackward() noexcept {
    if(heroY_ == 0 || anyOf(maze_[heroY_ - 1][heroX_], wall, hole, hero2)) return false;

    // we do not step back to the original position
    if(parent_ && parent_->parent_ && parent_->parent_->pushCounter_ == pushCounter_ &&
    parent_->parent_->heroX_ == heroX_ && parent_->parent_->heroY_ == heroY_ - 1) {
        return false;
    }

    char c = maze_[heroY_ - 1][heroX_];
    if(c == space) {
        maze_[heroY_][heroX_] = space;
        maze_[heroY_ - 1][heroX_] = hero;
        heroY_ -= 1;
        op_ = 'u';
    } else if(c == stone) {
        if(heroY_ <= 1 || maze_[heroY_ - 2][heroX_] != space || pullCardCount() == maximumPullCardCount_) return false;
        // TODO: we do not push back a ball what we pulled in the previous turn
        if(parent_ && parent_->parent_ && parent_->parent_->pushCounter_ + 1 == pushCounter_ &&
        parent_->parent_->heroX_ == heroX_ && parent_->parent_->heroY_ == heroY_ - 1 &&
        parent_->parent_->pullCardCount_ == pullCardCount_) {
            return false;
        }

        maze_[heroY_ - 2][heroX_] = stone;
        maze_[heroY_ - 1][heroX_] = hero;
        maze_[heroY_][heroX_] = space;

        removeStone(heroX_, heroY_ - 1);
        addStone(heroX_, heroY_ - 2);
        heroY_ -= 1;
        ++pushCounter_;
        ++pullCardCount_;
        op_ = 'Q'; // original move: pull a stone down
    } else {
        std::cerr << "c: " << c << ", heroX_: " << (int)heroX_ << ", heroY_: " << (int)heroY_ << std::endl;
        assert(0);
    }
    return true;
}

// From:  to:      From:  to:        From:  to:  we cannot pull a hero2 (original operator would be pull hero2)
//      _    _          _    _            _    _
//      h    _          h    _            h    _
//      _    h          o    h            H    h
//      _    _          _    o            _    H
bool Maze::stepDownBackward() noexcept {
    if(heroY_ == mazeHeight - 1 || anyOf(maze_[heroY_ + 1][heroX_], wall, hole, hero2)) return false;

    // we do not step back to the original position
    if(parent_ && parent_->parent_ && parent_->parent_->pushCounter_ == pushCounter_ &&
    parent_->parent_->heroX_ == heroX_ && parent_->parent_->heroY_ == heroY_ + 1) {
        return false;
    }

    char c = maze_[heroY_ + 1][heroX_];
    if(c == space) {
        maze_[heroY_][heroX_] = space;
        maze_[heroY_ + 1][heroX_] = hero;
        op_ = 'd';
        heroY_ += 1;
    } else if(c == stone) {
        if(heroY_ >= mazeHeight - 2 || maze_[heroY_ + 2][heroX_] != space || pullCardCount() == maximumPullCardCount_) return false;
        // TODO: we do not push back a ball what we pulled in the previous turn
        if(parent_ && parent_->parent_ && parent_->parent_->pushCounter_ + 1 == pushCounter_ &&
        parent_->parent_->heroX_ == heroX_ && parent_->parent_->heroY_ == heroY_ + 1 &&
        parent_->parent_->pullCardCount_ == pullCardCount_) {
            return false;
        }

        maze_[heroY_ + 2][heroX_] = stone;
        maze_[heroY_ + 1][heroX_] = hero;
        maze_[heroY_][heroX_] = space;

        removeStone(heroX_, heroY_ + 1);
        addStone(heroX_, heroY_ + 2);
        heroY_ += 1;
        ++pushCounter_;
        ++pullCardCount_;
        op_ = 'q'; // original move: pull a stone up
    } else {
        std::cerr << "c: " << c << ", heroX_: " << (int)heroX_ << ", heroY_: " << (int)heroY_ << std::endl;
        assert(0);
    }
    return true;
}

bool Maze::teleportBackward([[maybe_unused]]int x, [[maybe_unused]]int y) noexcept {
    if(teleportCardCount_ >= maximumTeleportCardCount_ || maze_[y][x] != space) return false;

    // we do not teleport to the neighbour fields
    if((abs(x - heroX_) == 1 && y == heroY_) || (x == heroX_ && abs(y - heroY_) <= 1)) return false;

    maze_[heroY_][heroX_] = space;
    maze_[y][x] = hero;
    heroX_ = x;
    heroY_ = y;
    ++teleportCardCount_;
    op_ = 't';
    return true;
}

// From __hH__ to hH___
bool Maze::pullHero2LeftBackward() noexcept {
    if(heroX_ == 0 || maze_[heroY_][heroX_ - 1] != space || heroX_ + 1 >= mazeWidth || maze_[heroY_][heroX_ + 1] != hero2 ||
    pushCardCount_ == maximumPushCardCount_) return false;

    ++pushCardCount_;
    ++pushCounter_;
    maze_[heroY_][heroX_ + 1] = space;
    maze_[heroY_][heroX_] = hero2;
    maze_[heroY_][heroX_ - 1] = hero;
    heroX_ -= 1;
    op_ = '1'; // original move: push a hero2 right
    return true;
}

// From __Hh__ to ___Hh_
bool Maze::pullHero2RightBackward() noexcept {
    if(heroX_ == 0 || maze_[heroY_][heroX_ - 1] != hero2 || heroX_ + 1 >= mazeWidth || maze_[heroY_][heroX_ + 1] != space ||
    pushCardCount_ == maximumPushCardCount_) return false;

    ++pushCardCount_;
    ++pushCounter_;
    maze_[heroY_][heroX_ - 1] = space;
    maze_[heroY_][heroX_] = hero2;
    maze_[heroY_][heroX_ + 1] = hero;
    heroX_ += 1;
    op_ = '2'; // original move: push a hero2 left
    return true;
}

// From:  to:
//      _    h
//      h    H
//      H    _
//      _    _
bool Maze::pullHero2UpBackward() noexcept {
    if(heroY_ == 0 || maze_[heroY_ - 1][heroX_] != space || heroY_ + 1 >= mazeHeight || maze_[heroY_ + 1][heroX_] != hero2 ||
    pushCardCount_ == maximumPushCardCount_) return false;

    ++pushCardCount_;
    ++pushCounter_;
    maze_[heroY_ + 1][heroX_] = space;
    maze_[heroY_][heroX_] = hero2;
    maze_[heroY_ - 1][heroX_] = hero;
    heroY_ -= 1;
    op_ = '3'; // original move: push a hero2 down
    return true;
}

// From:  to:
//      _    _
//      H    _
//      h    H
//      _    h
bool Maze::pullHero2DownBackward() noexcept {
    if(heroY_ == 0 || maze_[heroY_ - 1][heroX_] != hero2 || heroY_ + 1 >= mazeHeight || maze_[heroY_ + 1][heroX_] != space ||
    pushCardCount_ == maximumPushCardCount_) return false;

    ++pushCardCount_;
    ++pushCounter_;
    maze_[heroY_ - 1][heroX_] = space;
    maze_[heroY_][heroX_] = hero2;
    maze_[heroY_ + 1][heroX_] = hero;
    heroY_ += 1;
    op_ = '4'; // original move: push a hero2 up
    return true;
}

// From __ho__ to ho___
bool Maze::pullStoneLeftBackward() noexcept {
    if(heroX_ == 0 || maze_[heroY_][heroX_ - 1] != space || heroX_ + 1 >= mazeWidth || maze_[heroY_][heroX_ + 1] != stone) return false;

    maze_[heroY_][heroX_ + 1] = space;
    maze_[heroY_][heroX_] = stone;
    maze_[heroY_][heroX_ - 1] = hero;
    removeStone(heroX_ + 1, heroY_);
    addStone(heroX_, heroY_);
    ++pushCounter_;
    op_ = '5'; // original move: push a stone right
    heroX_ -= 1;
    return true;
}

// From __oh__ to ___oh_
bool Maze::pullStoneRightBackward() noexcept {
    if(heroX_ == 0 || maze_[heroY_][heroX_ - 1] != stone || heroX_ + 1 >= mazeWidth || maze_[heroY_][heroX_ + 1] != space) return false;

    maze_[heroY_][heroX_ - 1] = space;
    maze_[heroY_][heroX_] = stone;
    maze_[heroY_][heroX_ + 1] = hero;
    removeStone(heroX_ - 1, heroY_);
    addStone(heroX_, heroY_);
    ++pushCounter_;
    op_ = '6'; // original move: push a stone left
    heroX_ += 1;
    return true;
}

// From:  to:
//      _    h
//      h    o
//      o    _
//      _    _
bool Maze::pullStoneUpBackward() noexcept {
    if(heroY_ == 0 || maze_[heroY_ - 1][heroX_] != space || heroY_ + 1 >= mazeHeight || maze_[heroY_ + 1][heroX_] != stone) return false;

    maze_[heroY_ + 1][heroX_] = space;
    maze_[heroY_][heroX_] = stone;
    maze_[heroY_ - 1][heroX_] = hero;
    removeStone(heroX_, heroY_ + 1);
    addStone(heroX_, heroY_);
    ++pushCounter_;
    op_ = '7'; // original move: push a stone down
    heroY_ -= 1;
    return true;
}

// From:  to:
//      _    _
//      o    _
//      h    o
//      _    h
bool Maze::pullStoneDownBackward() noexcept {
    if(heroY_ == 0 || maze_[heroY_ - 1][heroX_] != stone || heroY_ + 1 >= mazeHeight || maze_[heroY_ + 1][heroX_] != space) return false;

    maze_[heroY_ - 1][heroX_] = space;
    maze_[heroY_][heroX_] = stone;
    maze_[heroY_ + 1][heroX_] = hero;
    removeStone(heroX_, heroY_ - 1);
    addStone(heroX_, heroY_);
    ++pushCounter_;
    op_ = '8'; // original move: push a stone up
    heroY_ += 1;
    return true;
}

// From __hHo_ to hHo__
bool Maze::pullHero2StoneLeftBackward() noexcept {
    if(heroX_ == 0 || maze_[heroY_][heroX_ - 1] != space || heroX_ + 2 >= mazeWidth || maze_[heroY_][heroX_ + 1] != hero2 || maze_[heroY_][heroX_ + 2] != stone ||
    pushCardCount_ == maximumPushCardCount_) return false;

    ++pushCardCount_;
    pushCounter_ += 2;
    maze_[heroY_][heroX_ + 2] = space;
    maze_[heroY_][heroX_ + 1] = stone;
    maze_[heroY_][heroX_] = hero2;
    maze_[heroY_][heroX_ - 1] = hero;
    removeStone(heroX_ + 2, heroY_);
    addStone(heroX_ + 1, heroY_);
    heroX_ -= 1;
    op_ = 'a'; // original move: push a hero2 and a stone right
    return true;
}

// From __oHh_ to ___oHh
bool Maze::pullHero2StoneRightBackward() noexcept {
    if(heroX_ + 1 >= mazeWidth || maze_[heroY_][heroX_ + 1] != space || heroX_ < 2 || maze_[heroY_][heroX_ - 1] != hero2 || maze_[heroY_][heroX_ - 2] != stone ||
    pushCardCount_ == maximumPushCardCount_) return false;

    ++pushCardCount_;
    pushCounter_ += 2;
    maze_[heroY_][heroX_ - 2] = space;
    maze_[heroY_][heroX_ - 1] = stone;
    maze_[heroY_][heroX_] = hero2;
    maze_[heroY_][heroX_ + 1] = hero;
    removeStone(heroX_ - 2, heroY_);
    addStone(heroX_ - 1, heroY_);
    heroX_ += 1;
    op_ = 'b'; // original move: push a hero2 and a stone left
    return true;
}

// From:  to:
//      _    h
//      h    H
//      H    o
//      o    _
bool Maze::pullHero2StoneUpBackward() noexcept {
    if(heroY_ + 2 >= mazeHeight || maze_[heroY_ + 1][heroX_] != hero2 || maze_[heroY_ + 2][heroX_] != stone || heroY_ == 0 || maze_[heroY_ - 1][heroX_] != space ||
    pushCardCount_ == maximumPushCardCount_) return false;

    ++pushCardCount_;
    pushCounter_ += 2;
    maze_[heroY_ + 2][heroX_] = space;
    maze_[heroY_ + 1][heroX_] = stone;
    maze_[heroY_][heroX_] = hero2;
    maze_[heroY_ - 1][heroX_] = hero;
    removeStone(heroX_, heroY_ + 2);
    addStone(heroX_, heroY_ + 1);
    heroY_ -= 1;
    op_ = 'c'; // original move: push a hero2 and a stone down
    return true;
}

// From:  to:
//      o    _
//      H    o
//      h    H
//      _    h
bool Maze::pullHero2StoneDownBackward() noexcept {
    if(heroY_ <= 1 || maze_[heroY_ - 2][heroX_] != stone || maze_[heroY_ - 1][heroX_] != hero2 || heroY_ + 1 >= mazeHeight || maze_[heroY_ + 1][heroX_] != space ||
    pushCardCount_ == maximumPushCardCount_) return false;

    ++pushCardCount_;
    pushCounter_ += 2;
    maze_[heroY_ - 2][heroX_] = space;
    maze_[heroY_ - 1][heroX_] = stone;
    maze_[heroY_][heroX_] = hero2;
    maze_[heroY_ + 1][heroX_] = hero;
    removeStone(heroX_, heroY_ - 2);
    addStone(heroX_, heroY_ - 1);
    heroY_ += 1;
    op_ = 'e'; // original move: push a hero2 and a stone up
    return true;
}

// From __h___ to _hox__
bool Maze::stepLeftSeparateHoleAndStone() noexcept {
    //std::cout << "stepLeftSeparateHoleAndStone DBG 1, heroX_: "
    //          << (int)heroX_ << " heroY_: " << (int)heroY_ << " holeCount_: " << (int)holeCount_ << " maximumHoleCount_ : " << (int)maximumHoleCount_ << "\n";
    if(heroX_ == 0 || maze_[heroY_][heroX_ - 1] != space || heroX_ + 1 >= mazeWidth ||
       maze_[heroY_][heroX_ + 1] != space || holeCount_ >= maximumHoleCount_) return false;

    maze_[heroY_][heroX_ + 1] = hole;
    maze_[heroY_][heroX_] = stone;
    maze_[heroY_][heroX_ - 1] = hero;
    addStone(heroX_, heroY_);
    addHole(heroX_ + 1, heroY_);
    --heroX_;
    ++pushCounter_;
    op_ = 'f'; // original move: push a stone right into a hole
    return true;
}

// From __h___ to _xoh__
bool Maze::stepRightSeparateHoleAndStone() noexcept {
    if(heroX_ == 0 || maze_[heroY_][heroX_ - 1] != space || heroX_ + 1 >= mazeWidth ||
       maze_[heroY_][heroX_ + 1] != space || holeCount_ >= maximumHoleCount_) return false;

    maze_[heroY_][heroX_ - 1] = hole;
    maze_[heroY_][heroX_] = stone;
    maze_[heroY_][heroX_ + 1] = hero;
    addStone(heroX_, heroY_);
    addHole(heroX_ - 1, heroY_);
    ++heroX_;
    ++pushCounter_;
    op_ = 'g'; // original move: push a stone left into a hole
    return true;
}

// From:  to:
//      _    h
//      h    o
//      _    x
//      _    _
bool Maze::stepUpSeparateHoleAndStone() noexcept {
    if(heroY_ == 0 || maze_[heroY_ - 1][heroX_] != space || heroY_ + 1 >= mazeHeight ||
       maze_[heroY_ + 1][heroX_] != space || holeCount_ >= maximumHoleCount_) return false;

    maze_[heroY_ + 1][heroX_] = hole;
    maze_[heroY_][heroX_] = stone;
    maze_[heroY_ - 1][heroX_] = hero;
    addStone(heroX_, heroY_);
    addHole(heroX_, heroY_ + 1);
    --heroY_;
    ++pushCounter_;
    op_ = 'h'; // original move: push a stone down into a hole
    return true;
}

// From:  to:
//      _    _
//      _    x
//      h    o
//      _    h
bool Maze::stepDownSeparateHoleAndStone() noexcept {
    if(heroY_ == 0 || maze_[heroY_ - 1][heroX_] != space || heroY_ + 1 >= mazeHeight ||
       maze_[heroY_ + 1][heroX_] != space || holeCount_ >= maximumHoleCount_) return false;

    maze_[heroY_ - 1][heroX_] = hole;
    maze_[heroY_][heroX_] = stone;
    maze_[heroY_ + 1][heroX_] = hero;
    addStone(heroX_, heroY_);
    addHole(heroX_, heroY_ - 1);
    ++heroY_;
    ++pushCounter_;
    op_ = 'i'; // original move: push a stone up into a hole
    return true;
}

void Maze::setMaximumCardsAmount(int teleport, int push, int pull) noexcept {
    maximumTeleportCardCount_ = teleport;
    maximumPushCardCount_ = push;
    maximumPullCardCount_ = pull;
}

/*
void Maze::makeHashKey() noexcept {
    hashNumber_ = 0.;
    constexpr double numeralSystem = 1.6;
    double index = numeralSystem;
    for(int y = 0; y < mazeHeight; ++y) {
        for(int x = 0; x < mazeWidth; ++x) {
            switch(maze_.at(y).at(x)) {
            case ' ': break;
            case 'o': hashNumber_ += index; break;
            case 'h': hashNumber_ += index * .2; break;
            case 'x': hashNumber_ += index * .3; break;
            case 'H': hashNumber_ += index * .4; break;
            case '*': hashNumber_ += index * .5; break;
            }
            index *= numeralSystem;
        }
    }
    hashNumber_ += index * teleportCardCount();
    index *= numeralSystem;
    hashNumber_ += index * pushCardCount();
    index *= numeralSystem;
    hashNumber_ += index * pullCardCount();
    hash_ = std::hash<double>{}(hashNumber_);
}
*/
