#ifndef __ASTAR_HPP__
#define __ASTAR_HPP__

#include <queue>
#include "Global.h"

template<typename T>
AStar<T>::AStar()
{

}

template<typename T>
void AStar<T>::updateParentPtr(const MazeStar* node, const MazeStar* newParent) {
    //std::cout << "AStar::updateParentPtr\n";
    if(node->parent_ && node->parent_ != newParent) {
        const MazeStar* oldParent = node->parent_->mazeStar();
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

template<typename T>
void AStar<T>::updateChilrenG(const MazeStar* node, int gDiff, int pushCounterDiff, int depthDiff, std::multiset<const MazeStar*, MazePComp2>& mazes) {
    //std::cout << "AStar::updateChilrenG\n";
    mazes.erase(node);
    node->g_ -= gDiff;
    node->pushCounter_ -= pushCounterDiff;
    node->depth_ -= depthDiff;
    mazes.insert(node);
    for(auto& child : node->children_) {
        updateChilrenG(child, gDiff, pushCounterDiff, depthDiff, mazes);
    }
}

template<typename T>
void AStar<T>::extendForwardOneState(const MazeStar* maze, bool (MazeStar::*f)()noexcept, int heroX, int heroY) noexcept {
    T newMaze = *maze;
    newMaze.updateHeroPosition(heroX, heroY);
    newMaze.children_.clear();
    newMaze.parent_  = maze;
    ++newMaze.depth_;
    newMaze.op_ = 0;
    assert(newMaze.parent_ == maze);
    if((newMaze.*f)()) {
        assert(newMaze.parent_ == maze);
        storeNewMaze(maze, newMaze);
    }
}

template<typename T>
void AStar<T>::extendForwardWithPushAlteration(const MazeStar* maze) noexcept {
    // flood-fill
    std::queue<std::pair<int, int>> queue;
    queue.emplace(maze->heroX(), maze->heroY());
    Maze::MazeArray markerMaze = maze->maze();

    if(maze->heroX() >= mazeWidth) {
        std::cerr << maze->heroX() << ", " << mazeWidth << *maze;
    }
    assert(maze->heroX() >= 0);
    assert(maze->heroX() < mazeWidth);
    assert(maze->heroY() >= 0);
    assert(maze->heroY() < mazeHeight);

    while(queue.size()) {
        auto item = queue.front();
        queue.pop();
        if(item.first > 0) {
            if(markerMaze[item.second][item.first - 1] == space) {
                queue.emplace(item.first - 1, item.second);
                markerMaze[item.second][item.first - 1] = 0; // mark
            } else if(markerMaze[item.second][item.first - 1] == stone) {
                for(auto f : {&MazeStar::stepLeft, &MazeStar::pullRight}) {
                    // hero position must be updated in the new maze
                    extendForwardOneState(maze, f, item.first, item.second);
                }
            } else if(markerMaze[item.second][item.first - 1] == hero2) {
                for(auto f : {&MazeStar::stepLeft}) {
                    extendForwardOneState(maze, f, item.first, item.second);
                }
            }
        }
        if(item.first < mazeWidth - 1) {
            if(markerMaze[item.second][item.first + 1] == space) {
                queue.emplace(item.first + 1, item.second);
                markerMaze[item.second][item.first + 1] = 0; // mark
            } else if(markerMaze[item.second][item.first + 1] == stone) {
                for(auto f : {&MazeStar::stepRight, &MazeStar::pullLeft}) {
                    // hero position must be updated in the new maze
                    extendForwardOneState(maze, f, item.first, item.second);
                }
            } else if(markerMaze[item.second][item.first + 1] == hero2) {
                for(auto f : {&MazeStar::stepRight}) {
                    extendForwardOneState(maze, f, item.first, item.second);
                }
            }

        }
        if(item.second > 0) {
            if(markerMaze[item.second - 1][item.first] == space) {
                queue.emplace(item.first, item.second - 1);
                markerMaze[item.second - 1][item.first] = 0; // mark
            } else if(markerMaze[item.second - 1][item.first] == stone) {
                for(auto f : {&MazeStar::stepUp, &MazeStar::pullDown}) {
                    // hero position must be updated in the new maze
                    extendForwardOneState(maze, f, item.first, item.second);
                }
            } else if(markerMaze[item.second - 1][item.first] == hero2) {
                for(auto f : {&MazeStar::stepUp}) {
                    extendForwardOneState(maze, f, item.first, item.second);
                }
            }
        }
        if(item.second < mazeHeight - 1) {
            if(markerMaze[item.second + 1][item.first] == space) {
                queue.emplace(item.first, item.second + 1);
                markerMaze[item.second + 1][item.first] = 0; // mark
            } else if(markerMaze[item.second + 1][item.first] == stone) {
                for(auto f : {&MazeStar::stepDown, &MazeStar::pullUp}) {
                    // hero position must be updated in the new maze
                    extendForwardOneState(maze, f, item.first, item.second);
                }
            } else if(markerMaze[item.second + 1][item.first] == hero2) {
                for(auto f : {&MazeStar::stepDown}) {
                    extendForwardOneState(maze, f, item.first, item.second);
                }
            }
        }
    }
    // let's try to teleport to the unmarked tiles
    if(!maze->teleportCardCount()) return;

    for(size_t y = 0; y < mazeHeight; ++y) {
        for(size_t x = 0; x < mazeWidth; ++x) {
            if(markerMaze[y][x] == space) {
                T newMaze = *maze;
                newMaze.children_.clear();
                newMaze.parent_  = maze;
                ++newMaze.depth_;
                newMaze.op_ = 0;
                if(newMaze.teleport(x, y)) {
                    storeNewMaze(maze, newMaze);
                }
            }
        }
    }
}

template<typename T>
void AStar<T>::extend(const MazeStar* maze) noexcept {
    if(pushAlterationEnabled_) {
        stopWatch.start("extend");
        extendForwardWithPushAlteration(maze);
        stopWatch.saveDuration("extend");
        return;
    }

    static const std::array<decltype(&MazeStar::stepDown), 8> steps{&MazeStar::stepLeft, &MazeStar::stepRight, &MazeStar::stepUp, &MazeStar::stepDown,
                &Maze::pullLeft,&Maze::pullRight, &Maze::pullUp, &Maze::pullDown};
    stopWatch.start("extend");
    for(auto f : steps) {
        T newMaze = *maze;
        newMaze.children_.clear();
        newMaze.parent_  = maze;
        ++newMaze.depth_;
        newMaze.op_ = 0;
        //std::cout << "Stones: " << newMaze.stones_.size() << "\n";
        //std::cout << "Holes: " << newMaze.holes_.size() << "\n";
        if((newMaze.*f)()) {
            storeNewMaze(maze, newMaze);
        }
    }

    if(!maze->teleportCardCount()) {
        stopWatch.saveDuration("extend");
        return;
    }

    for(size_t y = 0; y < mazeHeight; ++y) {
        for(size_t x = 0; x < mazeWidth; ++x) {
            T newMaze = *maze;
            newMaze.children_.clear();
            newMaze.parent_  = maze;
            ++newMaze.depth_;
            newMaze.op_ = 0;
            if(newMaze.teleport(x, y)/* && isSolvable(&newMaze)*/) {
                storeNewMaze(maze, newMaze);
            }
        }
    }

    stopWatch.saveDuration("extend");
}

template<typename T>
void AStar<T>::extendBackward(const MazeStar* maze) noexcept {
    //static size_t extendCounter = 0;
    //std::cout << "extendBackward: " << *maze;
    static const std::array<decltype(&MazeStar::stepDownBackward), 16> steps{&MazeStar::stepLeftBackward, &MazeStar::stepRightBackward, &MazeStar::stepUpBackward,
                &MazeStar::stepDownBackward,

                &MazeStar::pullHero2LeftBackward,
                &MazeStar::pullHero2RightBackward,
                &MazeStar::pullHero2UpBackward,
                &MazeStar::pullHero2DownBackward,
                &MazeStar::pullStoneLeftBackward,
                &MazeStar::pullStoneRightBackward,
                &MazeStar::pullStoneUpBackward,
                &MazeStar::pullStoneDownBackward,
                &MazeStar::pullHero2StoneLeftBackward,
                &MazeStar::pullHero2StoneRightBackward,
                &MazeStar::pullHero2StoneUpBackward,
                &MazeStar::pullHero2StoneDownBackward
                };
    stopWatch.start("extendBackward");
    //int operatorIndex = 0;
    for(auto f : steps) {
        //std::cout << "extendBackward operatorIndex: " << operatorIndex++ << "\n";
        T newMaze = *maze;
        newMaze.children_.clear();
        newMaze.parent_  = maze;
        ++newMaze.depth_;
        newMaze.op_ = 0;
        //std::cout << "Stones: " << newMaze.stones_.size() << "\n";
        //std::cout << "Holes: " << newMaze.holes_.size() << "\n";
        if((newMaze.*f)()) {
            storeNewMazeBackward(maze, newMaze);
        }
    }

    static const std::array<decltype(&MazeStar::stepLeftSeparateHoleAndStone), 4> steps2{
        &MazeStar::stepLeftSeparateHoleAndStone, &MazeStar::stepRightSeparateHoleAndStone,
        &MazeStar::stepUpSeparateHoleAndStone, &MazeStar::stepDownSeparateHoleAndStone
    };

    for(auto f : steps2) {
        //std::cout << "extendBackward operatorIndex: " << operatorIndex++ << "\n";
        T newMaze = *maze;
        newMaze.children_.clear();
        newMaze.parent_  = maze;
        ++newMaze.depth_;
        newMaze.op_ = 0;
        //std::cout << "Stones: " << newMaze.stones_.size() << "\n";
        //std::cout << "Holes: " << newMaze.holes_.size() << "\n";
        if((newMaze.*f)(backwardGoal_)) {
            storeNewMazeBackward(maze, newMaze);
        }
    }

    if(maze->teleportCardCount() >= maze->maximumTeleportCardCount()) return;

    for(size_t y = 0; y < mazeHeight; ++y) {
        for(size_t x = 0; x < mazeWidth; ++x) {
            T newMaze = *maze;
            newMaze.children_.clear();
            newMaze.parent_  = maze;
            ++newMaze.depth_;
            newMaze.op_ = 0;
            if(newMaze.teleportBackward(x, y)/* && isSolvable(&newMaze)*/) {
                storeNewMazeBackward(maze, newMaze);
            }
        }
    }

    stopWatch.saveDuration("extendBackward");
}

template<typename T>
void AStar<T>::storeNewMaze(const MazeStar* maze, T& newMaze) noexcept {
    // we will not extend the non-goal ones with maximum depth
    if(!newMaze.isGoal() && newMaze.depth() == maximumDepth_) return;

    bool commonSolutionFound = false;
    if(!findBest_ && direction_ == Direction::BOTH && mazeSetBackward_.find(newMaze) != mazeSetBackward_.end()) {
        if(verbose_) {
            std::cout << "storeNewMaze: Common solution found!\n";
            std::cout << newMaze;
        }
        commonSolutionFound = true;
        solutionBackward_ =  &*mazeSetBackward_.find(newMaze);
    }

    if(newMaze.parent_ != maze) {
        std::cout << "DBG 3 " << newMaze.parent_ << ", " << maze << std::endl;
        show_backtrace();
        assert(newMaze.parent_ == maze);
    }

    auto retv = mazeSet_.insert(newMaze);
    if(commonSolutionFound) {
        solution_ = &*retv.first;
    }
    if(retv.second) {
        assert(retv.first->parent_ == maze);
        mazes_.insert(&*retv.first);
        maze->children_.push_back(&*retv.first);
        if(retv.first->isGoal()) {
            if(!solution_ || solution_->g() > retv.first->g()) {
                solution_ = &*retv.first;
                if(verbose_) {
                    std::cout << "Goal found, cost is less!\n";
                    std::cout << "Depth: " << solution_->depth() << "\n";
                    printFirstState();
                }
            }
        }
    } else {
        if(retv.first->g_ > newMaze.g_) {
            // We came here on a more expensive way before thus we have to update g_, h_ and parent if g_ is smaller
            // Update g_ on each nodes coming from newNode
            stopWatch.start("updateTreeG");
            relinkCheaperPath(retv.first->mazeStar(), newMaze, maze, mazes_);
            stopWatch.saveDuration("updateTreeG");
        }
    }
}

template<typename T>
void AStar<T>::storeNewMazeBackward(const MazeStar* maze, T& newMaze) noexcept {
    // we will not extend the non-goal ones with maximum depth
    if(!newMaze.isGoalBackward(backwardGoal_) && newMaze.depth() == maximumDepthBackward_) return;

    bool commonSolutionFound = false;
    if(!findBest_ && direction_ == Direction::BOTH && mazeSet_.find(newMaze) != mazeSet_.end()) {
        if(verbose_) {
            std::cout << "storeNewMazeBackward: Common solution found!\n";
            std::cout << newMaze;
        }
        commonSolutionFound = true;
        solution_ =  &*mazeSet_.find(newMaze);
    }

    assert(newMaze.parent_ == maze);

    auto retv = mazeSetBackward_.insert(newMaze);
    if(commonSolutionFound) {
        solutionBackward_ = &*retv.first;
    }
    if(retv.second) {
        //std::cout << "storeNewMazeBackward: " << *retv.first;
        assert(retv.first->parent_ == maze);
        mazesBackward_.insert(&*retv.first);
        maze->children_.push_back(&*retv.first);
        if(retv.first->isGoalBackward(backwardGoal_)) {
            if(!solutionBackward_ || solutionBackward_->g() > retv.first->g()) {
                solutionBackward_ = &*retv.first;
                if(verbose_) {
                    std::cout << "Backward goal found, cost is less!\n";
                    std::cout << "Depth: " << solutionBackward_->depth() << "\n";
                    printFirstStateBackward();
                }
            }
        }
    } else {
        if(retv.first->g_ > newMaze.g_) {
            // We came here on a more expensive way before thus we have to update g_, h_ and parent if g_ is smaller
            // Update g_ on each nodes coming from newNode
            stopWatch.start("updateTreeG");
            relinkCheaperPath(retv.first->mazeStar(), newMaze, maze, mazesBackward_);
            stopWatch.saveDuration("updateTreeG");
        }
    }
}

template<typename T>
void AStar<T>::relinkCheaperPath(const MazeStar* oldMaze, T& newMaze, const MazeStar* parent, std::multiset<const MazeStar*, MazePComp2>& mazes) noexcept {
    updateParentPtr(oldMaze, parent);
    oldMaze->op_ = newMaze.op_;
    assert(oldMaze->teleportCardCount() == newMaze.teleportCardCount());
    assert(oldMaze->pushCardCount() == newMaze.pushCardCount());
    assert(oldMaze->pullCardCount() == newMaze.pullCardCount());

    updateChilrenG(oldMaze->mazeStar(),
                   oldMaze->g_ - newMaze.g_,
                   oldMaze->pushCounter() - newMaze.pushCounter(),
                   oldMaze->depth() - newMaze.depth(),
                   mazes);
}

template<typename T>
void AStar<T>::printMemoryUsage() {
    std::cout << "Memory usage: " << std::fixed << sizeof(T) * mazeSet_.size() / (double)(1ULL << 30) << " GB\r";
}

template<typename T>
bool AStar<T>::searchForward() noexcept {
    const MazeStar* current = *mazes_.begin();
    if(!extendCount_ && !isSolvable(current)) return true;
    if(!extendCount_ && direction_ == Direction::BOTH) {backwardGoal_ = &*mazeSet_.begin();}

    if(/*(extendCount % 3000000) == 0 && */stopWatch.timeMs(stopWatchKey.data()) > timeout_) {
        std::cerr << "\nTime's up, break. extendCount: " << extendCount_ << "\n";
        timesUp_ = true;
        return true;
    }
    if(current->isGoal()) {
        if(solution_) {
            if(current->g() <= solution_->g()) {
                solution_ = current;
                if(verbose_) {
                    std::cout << "Goal found, cost is less!\n";
                    std::cout << "Depth: " << solution_->depth() << "\n";
                    printFirstState();
                }
            }
        } else {
            // we are looking for the first solution
            solution_ = current;
            if(!findBest_) return true;
        }
    }
    mazes_.erase(mazes_.begin());
    if(current->depth() >= maximumDepth_) return false;
    if(solution_ && current->g() >= solution_->g()) return false;
    ++extendCount_;
    extend(current);
    if(solution_ && !findBest_) return true;
    return false;
}

template<typename T>
bool AStar<T>::searchBackward() noexcept {
    if(verbose_ && !(extendCount_ % 10000)) std::cout << "mazeSetBackward_: " << mazeSetBackward_.size() << ", mazesBackward_: " << mazesBackward_.size() << "\r";
    const MazeStar* current = *mazesBackward_.begin();

    if(/*(extendCount % 3000000) == 0 && */stopWatch.timeMs(stopWatchKey.data()) > timeout_) {
        std::cerr << "\nTime's up, break. extendCount: " << extendCount_ << "\n";
        timesUp_ = true;
        return true;
    }

    if(current->isGoalBackward(backwardGoal_)) {
        if(solutionBackward_) {
            if(current->g() <= solutionBackward_->g()) {
                solutionBackward_ = current;
                if(verbose_) {
                    std::cout << "Backward goal found, cost is less!\n";
                    std::cout << "Depth: " << solutionBackward_->depth() << "\n";
                    printFirstStateBackward();
                }
            }
        } else {
            // we are looking for the first solution
            solutionBackward_ = current;
            if(!findBest_) return true;
        }
    }
    mazesBackward_.erase(mazesBackward_.begin());
    if(current->depth() >= maximumDepthBackward_) return false;
    if(solutionBackward_ && current->g() >= solutionBackward_->g()) return false;
    ++extendCount_;
    extendBackward(current);
    if(solutionBackward_ && !findBest_) return true;
    return false;
}

template<typename T>
bool AStar<T>::search(bool verbose, bool findBest) noexcept {
    findBest_ = findBest;
    stopWatch.reset();
    verbose_ = verbose;
    stopWatch.start(stopWatchKey.data());
    solution_ = nullptr;
    //std::cout << "mazes_.size(): " << mazes_.size() << ", mazesBackward_.size(): " << mazesBackward_.size() << "\n";
    while(true) {
        bool searchForwardEnabled = mazes_.size() && (direction_ == Direction::FORWARD || direction_ == Direction::BOTH);
        bool searchBackwardEnabled = mazesBackward_.size() && (direction_ == Direction::BACKWARD || direction_ == Direction::BOTH);
        if(!searchForwardEnabled && !searchBackwardEnabled) break;

        if(searchForwardEnabled) {
            if(searchForward()) break;
        }
        if(searchBackwardEnabled) {
            if(searchBackward()) break;
        }
        //if(direction_ == Direction::BACKWARD)
        //    std::cout << "mazes_.size(): " << mazes_.size() << ", mazesBackward_.size(): " << mazesBackward_.size() << "\n";
        //if(mazes_.size()) exit(0);
    }

    if(solution_ && solutionBackward_) {
        if(verbose_) {
            std::cout << "\nFound it\n";
            //solution_->printStates();
            stopWatch.saveDuration(stopWatchKey.data());
            printWatches();
            std::cout << "Extend count: " << extendCount_ << "\n";
            std::cout << "mazeSet_.size:  " << mazeSet_.size() << "\n";
            std::cout << "mazes_.size:  " << mazes_.size() << "\n";
            std::cout << "mazeSetBackward_.size:  " << mazeSetBackward_.size() << "\n";
            std::cout << "mazesBackward_.size:  " << mazesBackward_.size() << "\n";
        }
        return true;
    }

    if(solution_) {
        if(verbose_) {
            std::cout << "\nForward search Found it\n";
            //solution_->printStates();
            stopWatch.saveDuration(stopWatchKey.data());
            printWatches();
            std::cout << "Extend count: " << extendCount_ << "\n";
            std::cout << "mazeSet_.size:  " << mazeSet_.size() << "\n";
            std::cout << "mazes_.size:  " << mazes_.size() << "\n";
            if(direction_ == Direction::BOTH) {
                std::cout << "mazeSetBackward_.size:  " << mazeSetBackward_.size() << "\n";
                std::cout << "mazesBackward_.size:  " << mazesBackward_.size() << "\n";
            }
        }
        if(exportNodesForNN_ /*&& !timesUp_*/) exportNodesForNN();
        return true;
    }

    if(solutionBackward_) {
        if(verbose_) {
            std::cout << "\nBackward search Found it\n";
            solutionBackward_->printStates();
            stopWatch.saveDuration(stopWatchKey.data());
            printWatches();
            std::cout << "Extend count: " << extendCount_ << "\n";
            if(direction_ == Direction::BOTH) {
                std::cout << "mazeSet_.size:  " << mazeSet_.size() << "\n";
                std::cout << "mazes_.size:  " << mazes_.size() << "\n";
            }
            std::cout << "mazeSetBackward_.size:  " << mazeSetBackward_.size() << "\n";
            std::cout << "mazesBackward_.size:  " << mazesBackward_.size() << "\n";
        }
        //if(exportNodesForNN_ /*&& !timesUp_*/) exportNodesForNN();
        return true;
    }

    if(verbose_) {
        std::cout << "\nThere is no solution\n";
        stopWatch.saveDuration(stopWatchKey.data());
        printWatches();
        std::cout << "Extend count: " << extendCount_ << "\n";
        std::cout << "mazeSet_.size:  " << mazeSet_.size() << "\n";
        std::cout << "mazes_.size:  " << mazes_.size() << "\n";
        std::cout << "mazeSetBackward_.size:  " << mazeSetBackward_.size() << "\n";
        std::cout << "mazesBackward_.size:  " << mazesBackward_.size() << "\n";
    }
    if(exportNodesForNN_ && !timesUp_) exportNodesForNN();
    return false;
}

template<typename T>
AStar<T>& AStar<T>::operator<<(const T& maze) {
    auto retv = mazeSet_.insert(maze);
    if(retv.second) {
        mazes_.insert(&*retv.first);
    }
    return *this;
}

template<typename T>
AStar<T>& AStar<T>::operator<<=(const T& maze) {
    auto retv = mazeSetBackward_.insert(maze);
    if(retv.second) {
        mazesBackward_.insert(&*retv.first);
    }
    return *this;
}

template<typename T>
void AStar<T>::printStates() const {
    if(solution_) {
        solution_->printStates();
    }
}

template<typename T>
int AStar<T>::depth() const {
    int d = 0;
    if(solution_) {
        d = solution_->depth();
    }
    return d;
}

template<typename T>
void AStar<T>::printFirstState() {
    if(solution_) {
        const Maze* maze = solution_;
        std::cout << "                  First State:" << std::endl;
        std::cout << "                  Push count: " << maze->pushCounter() << std::endl;
        std::cout << "                  g(): " << maze->g() << std::endl;
        std::cout << "                  depth: " << maze->depth() << std::endl;
        int teleportCount = maze->teleportCardCount();
        int pushCount = maze->pushCardCount();
        int pullCount = maze->pullCardCount();
        while(maze->parent_) {
            maze = maze->parent_;
        }
        std::cout << "                  Teleport card count: " << maze->teleportCardCount() - teleportCount << std::endl;
        std::cout << "                  Push card count: " << maze->pushCardCount() - pushCount << std::endl;
        std::cout << "                  Pull card count: " << maze->pullCardCount() - pullCount << std::endl;
        maze->printStates();
    }
}

template<typename T>
void AStar<T>::printFirstStateBackward() {
    if(solutionBackward_) {
        const Maze* maze = solutionBackward_;
        std::cout << "                  First State Backward:" << std::endl;
        std::cout << "                  Push count: " << maze->pushCounter() << std::endl;
        std::cout << "                  g(): " << maze->g() << std::endl;
        std::cout << "                  depth: " << maze->depth() << std::endl;
        std::cout << "                  Teleport card count: " << maze->teleportCardCount() << std::endl;
        std::cout << "                  Push card count: " << maze->pushCardCount() << std::endl;
        std::cout << "                  Pull card count: " << maze->pullCardCount() << std::endl;
        std::cout << *maze;
    }
}

template<typename T>
int AStar<T>::pushCounter() const noexcept {
    if(solution_) {
        return solution_->pushCounter();
    }
    return 0;
}

template<typename T>
int AStar<T>::pushCounterBackward() const noexcept {
    if(solutionBackward_) {
        return solutionBackward_->pushCounter();
    }
    return 0;
}

namespace AStarNS {
void saveMaze(std::ofstream& out, const Maze* maze);
}

template<typename T>
void AStar<T>::exportNodesForNN() noexcept {
    std::ofstream file("./dataset.txt", std::ios_base::out | std::ios_base::app);

    if(solution_) {
        auto node = solution_;
        auto g = node->g();
        while(node) {
            AStarNS::saveMaze(file, node);
            file << " " << g - node->g() << "\n";
            node = node->parent_;
        }
    } else {
        for(auto& node : mazes_) {
            if(!node->parent_) {
                AStarNS::saveMaze(file, node);
                file << " 1000\n";
            }
        }
    }
    file.flush();
    file.close();
}

template<typename T>
std::string AStar<T>::stepString() const noexcept {
    switch(direction_) {
    case Direction::FORWARD: return stepString(solution_); break;
    case Direction::BACKWARD: return stepString(solutionBackward_); break;
    case Direction::BOTH:
        if(findBest_) {
            return stepString(solution_) + " " + convertBackwardSteps(stepString(solutionBackward_));
        } else {
            return stepString(solution_) + convertBackwardSteps(stepString(solutionBackward_));
        }
        break;
    }
    return "";
}

template<typename T>
std::string AStar<T>::stepString(const Maze* solution) const noexcept {
    if(!solution) return "";
    std::stringstream stringStream;
    std::list<char> steps;
    const Maze* node = solution;
    while(node) {
        if(node->op_) steps.push_front(node->op_);
        else if(node->parent_) {std::cout << "Is operator at fault?\n";break;}
        node = node->parent_;
    }
    for(auto& e : steps) stringStream << e;
    //std::cout << stringStream.str() << "\n";
    return stringStream.str();
}

template<typename T>
std::string AStar<T>::convertBackwardSteps(std::string str) noexcept {
    std::string&& retV = std::move(str);
    for(size_t i = 0; i < retV.length(); ++i) {
        switch(retV[i]) {
        case 'l': retV[i] = 'r'; break;
        case 'r': retV[i] = 'l'; break;
        case 'u': retV[i] = 'd'; break;
        case 'd': retV[i] = 'u'; break;
        case 't': break;
        case 'p': break; // original move: pull a stone left
        case 'P': break; // original move: pull a stone right
        case 'q': break; // original move: pull a stone up
        case 'Q': break; // original move: pull a stone down
        case '1': retV[i] = 'R'; break; // original move: push a hero2 right
        case '2': retV[i] = 'L'; break; // original move: push a hero2 left
        case '3': retV[i] = 'D'; break; // original move: push a hero2 down
        case '4': retV[i] = 'U'; break; // original move: push a hero2 up
        case '5': retV[i] = 'R'; break; // original move: push a stone right
        case '6': retV[i] = 'L'; break; // original move: push a stone left
        case '7': retV[i] = 'D'; break; // original move: push a stone down
        case '8': retV[i] = 'U'; break; // original move: push a stone up
        case 'a': retV[i] = 'R'; break; // original move: push a hero2 and a stone right
        case 'b': retV[i] = 'L'; break; // original move: push a hero2 and a stone left
        case 'c': retV[i] = 'D'; break; // original move: push a hero2 and a stone down
        case 'e': retV[i] = 'U'; break; // original move: push a hero2 and a stone up
        case 'f': retV[i] = 'R'; break; // original move: push a stone right into a hole
        case 'g': retV[i] = 'L'; break; // original move: push a stone left into a hole
        case 'h': retV[i] = 'D'; break; // original move: push a stone down into a hole
        case 'i': retV[i] = 'U'; break; // original move: push a stone up into a hole
        }
    }
    std::reverse(retV.begin(), retV.end());
    return retV;
}

template<typename T>
bool AStar<T>::testSteps(std::string steps) noexcept {
    if(!mazes_.size()) return false;
    auto mazeIt = mazes_.begin();
    const MazeStar* current = *mazeIt;
    testStepsResult_ = *current;
    T mazeParent = *current;

    for(const char s : steps) {
        testStepsResult_.children_.clear();
        testStepsResult_.parent_  = current;
        ++testStepsResult_.depth_;
        testStepsResult_.op_ = 0;

        switch(s) {
        case 'l':
            if(testStepsResult_.stepLeft() && testStepsResult_.pushCounter() == mazeParent.pushCounter()) {
                mazeParent = testStepsResult_;
                continue;
            }
            std::cout << "DBG 1\n";
            return false;
        case 'L':
            if(testStepsResult_.stepLeft() && testStepsResult_.pushCounter() > mazeParent.pushCounter()) {
                mazeParent = testStepsResult_;
                continue;
            }
            std::cout << "DBG 2\n";
            return false;
        case 'r':
            if(testStepsResult_.stepRight() && testStepsResult_.pushCounter() == mazeParent.pushCounter()) {
                mazeParent = testStepsResult_;
                continue;
            }
            std::cout << "DBG 3\n";
            return false;
        case 'R':
            if(testStepsResult_.stepRight() && testStepsResult_.pushCounter() > mazeParent.pushCounter()) {
                mazeParent = testStepsResult_;
                continue;
            }
            return false;
        case 'u':
            if(testStepsResult_.stepUp() && testStepsResult_.pushCounter() == mazeParent.pushCounter()) {
                mazeParent = testStepsResult_;
                continue;
            }
            std::cout << "DBG 5\n";
            return false;
        case 'U':
            if(testStepsResult_.stepUp() && testStepsResult_.pushCounter() > mazeParent.pushCounter()) {
                mazeParent = testStepsResult_;
                continue;
            }
            std::cout << "DBG 6\n";
            return false;
        case 'd':
            if(testStepsResult_.stepDown() && testStepsResult_.pushCounter() == mazeParent.pushCounter()) {
                mazeParent = testStepsResult_;
                continue;
            }
            std::cout << "DBG 7\n";
            return false;
        case 'D':
            if(testStepsResult_.stepDown() && testStepsResult_.pushCounter() > mazeParent.pushCounter()) {
                mazeParent = testStepsResult_;
                continue;
            }
            std::cout << "DBG 8\n";
            return false;
        case 't': assert(0); break; // TODO
        case 'p':
            if(testStepsResult_.pullLeft() && testStepsResult_.pushCounter() > mazeParent.pushCounter()) {
                mazeParent = testStepsResult_;
                continue;
            }
            std::cout << "DBG 9\n";
            return false;
        case 'P':
            if(testStepsResult_.pullRight() && testStepsResult_.pushCounter() > mazeParent.pushCounter()) {
                mazeParent = testStepsResult_;
                continue;
            }
            std::cout << "DBG 10\n";
            return false;
        case 'q':
            if(testStepsResult_.pullUp() && testStepsResult_.pushCounter() > mazeParent.pushCounter()) {
                mazeParent = testStepsResult_;
                continue;
            }
            std::cout << "DBG 11\n";
            return false;
        case 'Q':
            if(testStepsResult_.pullDown() && testStepsResult_.pushCounter() > mazeParent.pushCounter()) {
                mazeParent = testStepsResult_;
                continue;
            }
            std::cout << "DBG 12\n";
            return false;
        }
    }
    return testStepsResult_.isGoal();
}

template<typename T>
int AStar<T>::pushNumberFromSteps(std::string steps) noexcept {
    int retV = 0;
    for(const char s : steps) {
        switch(s) {
        case 'L': case 'R': case 'U': case 'D': case 'p': case 'P': case 'q': case 'Q': ++retV; break;
        }
    }
    return retV;
}

template<typename T>
int AStar<T>::lines(std::string steps) noexcept {
    int retV = 0;
    for(size_t index = 1; index < steps.length(); ++index) {
        if(steps[index - 1] != steps[index]) ++retV;
    }
    return retV;
}

#endif // __ASTAR_HPP__
