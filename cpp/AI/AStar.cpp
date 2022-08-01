#include "AStar.h"
#include <algorithm>
#include <limits>

template<typename T>
void Unused(T...) {

}

MazeStar& MazeStar::operator=(const MazeVector& maze) {
    Maze::operator=(maze);
    return *this;
}

// operators: step left, step right, step up, step down
bool MazeStar::stepLeft(const int targetX, const int targetY) {
    if(Maze::stepLeft()) {
        calcG();
        calcH(targetX, targetY);
        return true;
    }
    return false;
}

bool MazeStar::stepRight(const int targetX, const int targetY) {
    if(Maze::stepRight()) {
        calcG();
        calcH(targetX, targetY);
        return true;
    }
    return false;
}

bool MazeStar::stepUp(const int targetX, const int targetY) {
    if(Maze::stepUp()) {
        calcG();
        calcH(targetX, targetY);
        return true;
    }
    return false;
}

bool MazeStar::stepDown(const int targetX, const int targetY) {
    if(Maze::stepDown()) {
        calcG();
        calcH(targetX, targetY);
        return true;
    }
    return false;
}

void MazeStar::calcG() {
    // Hero's step count
    g_ += 11;
    // the turns should be considered too: the less turn the less weight
    if(parent() && parent()->parent()) {
        if((parent()->parent()->heroX() == parent()->heroX() && parent()->heroX() == heroX())
        || (parent()->parent()->heroY() == parent()->heroY() && parent()->heroY() == heroY())) {
            --g_;
        }
    }
}

void MazeStar::calcH(const int targetX, const int targetY) {
    // Distance of hero, stone(s) and the hole(s)
    h_ = (abs(targetX - heroX()) + abs(targetY - heroY())) * 10;
}

bool MazeStar::operator==(const MazeStar& m) const {
    return maze_ == m.maze_;
}

std::ostream& operator<<(std::ostream& out, const MazeStar& maze) {
    out << *(Maze*)&maze;
    return out;
}

AStar::AStar(int targetX, int targetY)
: targetX_(targetX)
, targetY_(targetY)
{

}

void AStar::updateParentPtr(const MazeStar* node, const MazeStar* newParent) {
    //std::cout << "AStar::updateParentPtr\n";
    if(node->parent_ && node->parent_ != newParent) {
        const MazeStar* oldParent = node->parent_->getMazeStar();
        // remove node pointer from old parent's children
        //std::cout << "children count1 : " << oldParent->children_.size() << "\n";
        oldParent->children_.erase(std::remove(oldParent->children_.begin(), oldParent->children_.end(), node),
                                   oldParent->children_.end());
        //std::cout << "children count2 : " << oldParent->children_.size() << "\n";
        // add parent pointer to new parent's children
        newParent->children_.push_back(node);
    }
    node->parent_ = newParent;
}

void AStar::updateChilrenG(const MazeStar* node, int gDiff) {
    //std::cout << "AStar::updateChilrenG\n";
    mazes_.erase(node);
    node->g_ -= gDiff;
    mazes_.insert(node);
    for(auto& child : node->children_) {
        updateChilrenG(child, gDiff);
    }
}

bool AStar::extend(const MazeStar& maze) {
    //static size_t extendCounter = 0;
    //std::cout << "extend : " << ++extendCounter << "\r";
    static const std::array<bool(MazeStar::*)(const int,const int), 4> steps{&MazeStar::stepLeft, &MazeStar::stepRight, &MazeStar::stepUp, &MazeStar::stepDown};
    for(auto f : steps) {
        MazeStar newMaze = maze;
        newMaze.children_.clear();
        newMaze.parent_  = &maze;
        if((newMaze.*f)(targetX_, targetY_)) {
            auto retv = mazeSet_.insert(newMaze);
            if(retv.second) {
                //printMemoryUsage();
                mazes_.insert(&(*retv.first));
                //std::cout << "children count : " << maze.children_.size() << "\n";
                maze.children_.push_back(&(*retv.first));
            } else {
                // We have this node, we have to update g_, h_ and parent if g_ is smaller
                if(retv.first->g_ > newMaze.g_) {
                    // Update g_ on each nodes coming from newNode
                    updateParentPtr(retv.first->getMazeStar(), &maze);
                    updateChilrenG(retv.first->getMazeStar(), retv.first->g_ - newMaze.g_);
                }
            }
        }
    }
    return false;
}

bool AStar::search(bool verbose) {
    solution_ = nullptr;
    size_t extendCount = 0;
    while(mazes_.size()) {
        auto mazeIt = mazes_.begin();
        if(isGoal(*mazeIt)) {
            if(solution_) {
                if((*mazeIt)->g() <= solution_->g()) {
                    solution_ = *mazeIt;
                } else {
                    // keresem az optimális megoldást
                    if(verbose) {
                        std::cout << "\nFound it\n";
                        solution_->printTheSteps();
                        std::cout << "Extend count: " << extendCount << "\n";
                    }
                    return true;
                }
            } else {
                solution_ = *mazeIt;
            }
        }
        const MazeStar* current = *mazeIt;
        mazes_.erase(mazeIt);
        ++extendCount;
        extend(*current);
    }

    if(solution_) {
        if(verbose) {
            std::cout << "\nFound it\n";
            solution_->printTheSteps();
            std::cout << "Extend count: " << extendCount << "\n";
        }
        return true;
    }

    if(verbose) {
        std::cout << "\nThere is no solution\n";
        std::cout << "Extend count: " << extendCount << "\n";
    }
    return false;
}

AStar& AStar::operator<<(const MazeStar& maze) {
    auto retv = mazeSet_.insert(maze);
    if(retv.second) {
        mazes_.insert(&(*retv.first));
    }
    return *this;
}

int AStar::depth() const {
    int d = 0;
    if(solution_) {
        ++d;
        const Maze* maze = solution_;
        while(maze->parent_) {
            maze = maze->parent_;
            ++d;
        }
    }
    return d;
}

std::vector<Maze::MazeVector> AStar::getResult() {
    std::vector<Maze::MazeVector> result;
    if(solution_) {
        const Maze* maze = solution_;
        result.push_back(maze->maze_);
        while(maze->parent_) {
            maze = maze->parent_;
            result.push_back(maze->maze_);
        }
    }
    return result;
}

bool AStar::isGoal(const MazeStar* maze) const {
    return maze->heroX() == targetX_ && maze->heroY() == targetY_;
}
