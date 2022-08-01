#include "Breadthfirst.h"

BreadthFirst::BreadthFirst()
{

}

void BreadthFirst::storeNewMaze(Maze& newMaze, bool verbose) noexcept {
    //newMaze.makeHashKey();
    auto retv = mazeSet_.insert(newMaze);
    if(retv.second) {

        // we will not extend the non-goal ones with maximum depth
        if(!newMaze.isGoal() && newMaze.depth() == maximumDepth_) return;

        mazes_.push_back(&(*retv.first));
        if(newMaze.isGoal()) {
            solution_ = mazes_.back();
            if(verbose) {
                std::cout << "\nFound it\n";
                newMaze.printStates();
                std::cout << "Push count: " << solution_->pushCounter() << "\n";
            }
        }
    }
}

bool BreadthFirst::extend(const Maze& maze, bool verbose) {
    //static size_t extendCounter = 0;
    //std::cout << "extend : " << ++extendCounter << "\r";
    static const std::array<decltype(&Maze::stepDown), 8> steps{&Maze::stepLeft, &Maze::stepRight, &Maze::stepUp, &Maze::stepDown, &Maze::pullLeft,
                                                               &Maze::pullRight, &Maze::pullUp, &Maze::pullDown};
    for(auto f : steps) {
        Maze newMaze = maze;
        newMaze.parent_  = &maze;
        ++newMaze.depth_;
        if((newMaze.*f)()/* && isSolvable(&newMaze)*/) {
            storeNewMaze(newMaze, verbose);
            if(solution_) return true;
        }
    }

    if(!maze.teleportCardCount()) return false;

    for(size_t y = 0; y < mazeHeight; ++y) {
        for(size_t x = 0; x < mazeWidth; ++x) {
            Maze newMaze = maze;
            newMaze.parent_  = &maze;
            ++newMaze.depth_;
            if(newMaze.teleport(x, y)/* && isSolvable(&newMaze)*/) {
                storeNewMaze(newMaze, verbose);
                if(solution_) return true;
            }
        }
    }
    return false;
}

void BreadthFirst::printMemoryUsage() {
    std::cout << "Memory usage: " << std::fixed << sizeof(Maze) * mazeSet_.size() / 1024. / 1024. / 1024. << "GB\r";
}

bool BreadthFirst::search(bool verbose) {
    solution_ = nullptr;
    while(mazes_.size()) {
        const Maze* maze = mazes_.front();
        mazes_.pop_front();
        if(!mazes_.size() && !isSolvable(maze)) break;
        if(extend(*maze, verbose)) return true;
        //if(verbose) printMemoryUsage();
        //std::cout << "mazeSet_.size: " << mazeSet_.size() << ", nodes are waiting for extension: "
        //<< std::fixed << mazes_.size() << ", Depth: " << (mazes_.size()?mazes_.back()->depth():0) << "\r";
    }
    if(verbose) std::cout << "\nThere is no solution\n";
    return false;
}

BreadthFirst& BreadthFirst::operator<<(const Maze& maze) {
    auto retv = mazeSet_.insert(maze);
    if(retv.second) {
        mazes_.push_back(&(*retv.first));
    }
    return *this;
}

int BreadthFirst::depth() const {
    int d = 0;
    if(solution_) {
        d = solution_->depth();
    }
    return d;
}

void BreadthFirst::printFirstState() {
    if(solution_) {
        const Maze* maze = solution_;
        std::cout << "                  Push count: " << maze->pushCounter();
        while(maze->parent_) {
            maze = maze->parent_;
        }
        maze->printStates();
    }
}

int BreadthFirst::pushCounter() const noexcept {
    if(solution_) {
        return solution_->pushCounter();
    }
    return 0;
}
