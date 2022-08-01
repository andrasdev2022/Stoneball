#include "MazeStarV3.h"
#include "AStar.h"
#include <limits>

StopWatch stopWatch;
std::string_view stopWatchKey("search");

void printWatches() {
    std::cout << "search: " << stopWatch.duration(stopWatchKey.data()) / 1.e6<< " seconds\n"
              << "extend: " << stopWatch.duration("extend") / 1.e6<< " seconds\n"
              << "calcH: " <<  stopWatch.duration("calcH") / 1.e6 << " seconds\n"
              << "updateTreeG: " << stopWatch.duration("updateTreeG") / 1.e6 << " seconds\n"
;
}

template<typename T>
void Unused(T...) {

}

MazeStar& MazeStar::operator=(const MazeArray& maze) {
    Maze::operator=(maze);
    stones_.clear();
    holes_.clear();
    for(size_t y = 0; y < mazeHeight; ++y) {
        for(size_t x = 0; x < mazeWidth; ++x) {
            if(maze_.at(y).at(x) == stone) {
                stones_.emplace_back(x, y);
            } else if(maze_.at(y).at(x) == hole) {
                holes_.emplace_back(x, y);
            }
        }
    }
    return *this;
}

// operators: step left, step right, step up, step down
bool MazeStar::stepLeft() noexcept {
    auto oldPushCardCount = pushCardCount();
    if(Maze::stepLeft()) {
        if(oldPushCardCount > pushCardCount()) {
            // card usage is more expensive
            g_ += cardUsageCost;
        } else {
            calcG();
        }
        calcH();
        return true;
    }
    return false;
}

bool MazeStar::stepRight() noexcept {
    auto oldPushCardCount = pushCardCount();
    if(Maze::stepRight()) {
        if(oldPushCardCount > pushCardCount()) {
            // card usage is more expensive
            g_ += cardUsageCost;
        } else {
            calcG();
        }
        calcH();
        return true;
    }
    return false;
}

bool MazeStar::stepUp() noexcept {
    auto oldPushCardCount = pushCardCount();
    if(Maze::stepUp()) {
        if(oldPushCardCount > pushCardCount()) {
            // card usage is more expensive
            g_ += cardUsageCost;
        } else {
            calcG();
        }
        calcH();
        return true;
    }
    return false;
}

bool MazeStar::stepDown() noexcept {
    auto oldPushCardCount = pushCardCount();
    if(Maze::stepDown()) {
        if(oldPushCardCount > pushCardCount()) {
            // card usage is more expensive
            g_ += cardUsageCost;
        } else {
            calcG();
        }
        calcH();
        return true;
    }
    return false;
}

bool MazeStar::teleport(int x, int y) noexcept {
    if(Maze::teleport(x ,y)) {
        // card usage is more expensive
        g_ += cardUsageCost;
        calcH();
        return true;
    }
    return false;
}

bool MazeStar::pullLeft() noexcept {
    if(Maze::pullLeft()) {
        // card usage is more expensive
        g_ += cardUsageCost;
        calcH();
        return true;
    }
    return false;
}

bool MazeStar::pullRight() noexcept {
    if(Maze::pullRight()) {
        // card usage is more expensive
        g_ += cardUsageCost;
        calcH();
        return true;
    }
    return false;
}

bool MazeStar::pullUp() noexcept {
    if(Maze::pullUp()) {
        // card usage is more expensive
        g_ += cardUsageCost;
        calcH();
        return true;
    }
    return false;
}

bool MazeStar::pullDown() noexcept {
    if(Maze::pullDown()) {
        // card usage is more expensive
        g_ += cardUsageCost;
        calcH();
        return true;
    }
    return false;
}

void MazeStar::calcG() {
    // Hero's step count
    ++g_;
}

void MazeStar::calcH() {
    stopWatch.start("calcH");
    h_ = 0;

    // Distance of hero, stone(s) and the hole(s)

/*
    // Idea: h = distance(hero, closestStone0) + distance(closestStone0, closestHole0) + distance(closestHole0, closestStone1) +
    // distance(closestStone1, closestHole1) + ...

    std::vector<decltype(stones_.begin())> stoneIterators;
    std::vector<decltype(holes_.begin())> holeIterators;
    auto closestStonePos = stones_.end();
    auto closestHolePos = holes_.end();

    int minHeroStoneDistance = std::numeric_limits<int>::max();
    for(auto it1 = stones_.begin(); it1 != stones_.end(); ++it1) {
        int d = abs(it1->first - heroX()) + abs(it1->second - heroY());
        if(minHeroStoneDistance > d) {
            minHeroStoneDistance = d;
            closestStonePos = it1;
        }
    }
    if(closestStonePos != stones_.end()) {
        h_ += minHeroStoneDistance;
        stoneIterators.push_back(closestStonePos);
    } else {
        return;
    }

    while(true) {
        int minStoneHoleDistance = std::numeric_limits<int>::max();
        closestHolePos = holes_.end();
        for(auto it2 = holes_.begin(); it2 != holes_.end(); ++it2) {
            if(std::find(holeIterators.begin(), holeIterators.end(), it2) != holeIterators.end()) continue;
            int h = abs(closestStonePos->first - it2->first) + abs(closestStonePos->second - it2->second);
            if(minStoneHoleDistance > h) {
                minStoneHoleDistance = h;
                closestHolePos = it2;
            }
        }
        if(closestHolePos != holes_.end()) {
            h_ += minStoneHoleDistance;
            holeIterators.push_back(closestHolePos);
        } else {
            break;
        }

        minStoneHoleDistance = std::numeric_limits<int>::max();
        closestStonePos = stones_.end();
        for(auto it1 = stones_.begin(); it1 != stones_.end(); ++it1) {
            if(std::find(stoneIterators.begin(), stoneIterators.end(), it1) != stoneIterators.end()) continue;
            int d = abs(it1->first - closestHolePos->first) + abs(it1->second - closestHolePos->second);
            if(minStoneHoleDistance > d) {
                minStoneHoleDistance = d;
                closestStonePos = it1;
            }
        }
        if(closestStonePos != stones_.end()) {
            h_ += minStoneHoleDistance;
            stoneIterators.push_back(closestStonePos);
        } else {
            break;
        }
    }
*/

    int minHeroStoneDistance = std::numeric_limits<int>::max();
    int minStoneHoleDistance = std::numeric_limits<int>::max();
    for(auto it1 = stones_.begin(); it1 != stones_.end(); ++it1) {
        int d = abs(it1->first - heroX()) + abs(it1->second - heroY());
        if(minHeroStoneDistance > d) {
            minHeroStoneDistance = d;
            minStoneHoleDistance = std::numeric_limits<int>::max();
            for(auto it2 = holes_.begin(); it2 != holes_.end(); ++it2) {
                int h = abs(it1->first - it2->first) + abs(it1->second - it2->second);
                if(minStoneHoleDistance > h) {
                    minStoneHoleDistance = h;
                }
            }
        }
    }

    // Holecount should not be included in the heuristic, because it is not sure if the right stone wall pushed
    // into the hole first time, or?

    if(!holes_.size()) {
        h_ = 0;
    } else {
        h_ = minHeroStoneDistance + minStoneHoleDistance + holes_.size();
    }

    stopWatch.saveDuration("calcH");
}

void MazeStar::calcHBackward() {
    calcH();
}

/*
bool MazeStar::operator==(const MazeStar& m) const {
    return maze_ == m.maze_;
}
*/
bool MazeStar::isGoal() const {
    return !(stones_.size() + holes_.size());
}

void MazeStar::printCostAndHeuristic() const noexcept {
    std::cout << "Cost: " << g_ << ", Heuristic: " << h_ << "\n";
}


void MazeStar::addStone(int x, int y) {
    Maze::addStone(x, y);
    stones_.emplace_back(x, y);
}

void MazeStar::removeStone(int x, int y) {
    Maze::removeStone(x, y);
    std::pair<int, int> p(x, y);
    stones_.erase(std::remove(stones_.begin(), stones_.end(), p), stones_.end());
    /*
    for(auto it = stones_.begin(); it != stones_.end(); ++it) {
        if(*it == p) {
            stones_.erase(it);
            return;
        }
    }*/
}

void MazeStar::removeHole(int x, int y) {
    Maze::removeHole(x, y);
    std::pair<int, int> p(x, y);
    holes_.erase(std::remove(holes_.begin(), holes_.end(), p), holes_.end());
    /*
    for(auto it = holes_.begin(); it != holes_.end(); ++it) {
        if(*it == p) {
            holes_.erase(it);
            return;
        }
    }*/
}

void MazeStar::addHole(int x, int y) {
    Maze::addHole(x, y);
    holes_.emplace_back(x, y);
}

void MazeStar::addHero2(int x, int y) noexcept {
    Maze::addHero2(x, y);
}

void MazeStar::removeHero2(int x, int y) noexcept {
    Maze::removeHero2(x, y);
}

bool MazeStar::isGoalBackward([[maybe_unused]]const MazeStar* backwardGoal) const {
    std::cout << "MazeStar::isGoalBackward\n";
    assert(!mazeStarV2() || !backwardGoal->mazeStarV2());
    assert(!mazeStarV3() || !backwardGoal->mazeStarV3());
    return isGoal();
}

bool MazeStar::stepLeftBackward() noexcept {
    auto oldPushCardCount = pushCardCount();
    if(Maze::stepLeftBackward()) {
        if(oldPushCardCount < pushCardCount()) {
            // card usage is more expensive
            g_ += cardUsageCost;
        } else {
            calcG();
        }
        calcHBackward();
        return true;
    }
    return false;
}

bool MazeStar::stepRightBackward() noexcept {
    auto oldPushCardCount = pushCardCount();
    if(Maze::stepRightBackward()) {
        if(oldPushCardCount < pushCardCount()) {
            // card usage is more expensive
            g_ += cardUsageCost;
        } else {
            calcG();
        }
        calcHBackward();
        return true;
    }
    return false;
}

bool MazeStar::stepUpBackward() noexcept {
    auto oldPushCardCount = pushCardCount();
    if(Maze::stepUpBackward()) {
        if(oldPushCardCount < pushCardCount()) {
            // card usage is more expensive
            g_ += cardUsageCost;
        } else {
            calcG();
        }
        calcHBackward();
        return true;
    }
    return false;
}

bool MazeStar::stepDownBackward() noexcept {
    auto oldPushCardCount = pushCardCount();
    if(Maze::stepDownBackward()) {
        if(oldPushCardCount < pushCardCount()) {
            // card usage is more expensive
            g_ += cardUsageCost;
        } else {
            calcG();
        }
        calcHBackward();
        return true;
    }
    return false;
}

bool MazeStar::teleportBackward([[maybe_unused]]int x, [[maybe_unused]]int y) noexcept {
    if(Maze::teleportBackward(x ,y)) {
        // card usage is more expensive
        g_ += cardUsageCost;
        calcHBackward();
        return true;
    }
    return false;
}


bool MazeStar::pullHero2LeftBackward() noexcept {
    auto oldPushCardCount = pushCardCount();
    if(Maze::pullHero2LeftBackward()) {
        if(oldPushCardCount < pushCardCount()) {
            // card usage is more expensive
            g_ += cardUsageCost;
        } else {
            calcG();
        }
        calcHBackward();
        return true;
    }
    return false;
}

bool MazeStar::pullHero2RightBackward() noexcept {
    auto oldPushCardCount = pushCardCount();
    if(Maze::pullHero2RightBackward()) {
        if(oldPushCardCount < pushCardCount()) {
            // card usage is more expensive
            g_ += cardUsageCost;
        } else {
            calcG();
        }
        calcHBackward();
        return true;
    }
    return false;
}


bool MazeStar::pullHero2UpBackward() noexcept {
    auto oldPushCardCount = pushCardCount();
    if(Maze::pullHero2UpBackward()) {
        if(oldPushCardCount < pushCardCount()) {
            // card usage is more expensive
            g_ += cardUsageCost;
        } else {
            calcG();
        }
        calcHBackward();
        return true;
    }
    return false;
}


bool MazeStar::pullHero2DownBackward() noexcept {
    auto oldPushCardCount = pushCardCount();
    if(Maze::pullHero2DownBackward()) {
        if(oldPushCardCount < pushCardCount()) {
            // card usage is more expensive
            g_ += cardUsageCost;
        } else {
            calcG();
        }
        calcHBackward();
        return true;
    }
    return false;
}

bool MazeStar::pullStoneLeftBackward() noexcept {
    if(Maze::pullStoneLeftBackward()) {
        calcG();
        calcHBackward();
        return true;
    }
    return false;
}

bool MazeStar::pullStoneRightBackward() noexcept {
    if(Maze::pullStoneRightBackward()) {
        calcG();
        calcHBackward();
        return true;
    }
    return false;
}

bool MazeStar::pullStoneUpBackward() noexcept {
    if(Maze::pullStoneUpBackward()) {
        calcG();
        calcHBackward();
        return true;
    }
    return false;
}


bool MazeStar::pullStoneDownBackward() noexcept {
    if(Maze::pullStoneDownBackward()) {
        calcG();
        calcHBackward();
        return true;
    }
    return false;
}

bool MazeStar::pullHero2StoneLeftBackward() noexcept {
    auto oldPushCardCount = pushCardCount();
    if(Maze::pullHero2StoneLeftBackward()) {
        if(oldPushCardCount < pushCardCount()) {
            // card usage is more expensive
            g_ += cardUsageCost;
        } else {
            calcG();
        }
        calcHBackward();
        return true;
    }
    return false;
}


bool MazeStar::pullHero2StoneRightBackward() noexcept {
    auto oldPushCardCount = pushCardCount();
    if(Maze::pullHero2StoneRightBackward()) {
        if(oldPushCardCount < pushCardCount()) {
            // card usage is more expensive
            g_ += cardUsageCost;
        } else {
            calcG();
        }
        calcHBackward();
        return true;
    }
    return false;
}


bool MazeStar::pullHero2StoneUpBackward() noexcept {
    auto oldPushCardCount = pushCardCount();
    if(Maze::pullHero2StoneUpBackward()) {
        if(oldPushCardCount < pushCardCount()) {
            // card usage is more expensive
            g_ += cardUsageCost;
        } else {
            calcG();
        }
        calcHBackward();
        return true;
    }
    return false;
}


bool MazeStar::pullHero2StoneDownBackward() noexcept {
    auto oldPushCardCount = pushCardCount();
    if(Maze::pullHero2StoneDownBackward()) {
        if(oldPushCardCount < pushCardCount()) {
            // card usage is more expensive
            g_ += cardUsageCost;
        } else {
            calcG();
        }
        calcHBackward();
        return true;
    }
    return false;
}

bool MazeStar::stepLeftSeparateHoleAndStone(const MazeStar* backwardGoal) noexcept {
    // we can put holes only on those tiles where there are holes on the target state
    if(backwardGoal) {
        if(heroX() + 1 >= mazeWidth || backwardGoal->maze().at(heroY()).at(heroX() + 1) != hole) {
            return false;
        }
    }
    if(!Maze::stepLeftSeparateHoleAndStone()) return false;
    calcG();
    calcHBackward();
    return true;
}

bool MazeStar::stepRightSeparateHoleAndStone(const MazeStar* backwardGoal) noexcept {
    // we can put holes only on those tiles where there are holes on the target state
    if(backwardGoal) {
        if(heroX() == 0 || backwardGoal->maze().at(heroY()).at(heroX() - 1) != hole) {
            return false;
        }
    }
    if(!Maze::stepRightSeparateHoleAndStone()) return false;
    calcG();
    calcHBackward();
    return true;
}

bool MazeStar::stepUpSeparateHoleAndStone(const MazeStar* backwardGoal) noexcept {
    // we can put holes only on those tiles where there are holes on the target state
    if(backwardGoal) {
        if(heroY() + 1 >= mazeHeight || backwardGoal->maze().at(heroY() + 1).at(heroX()) != hole) {
            return false;
        }
    }
    if(!Maze::stepUpSeparateHoleAndStone()) return false;
    calcG();
    calcHBackward();
    return true;

}

bool MazeStar::stepDownSeparateHoleAndStone(const MazeStar* backwardGoal) noexcept {
    // we can put holes only on those tiles where there are holes on the target state
    if(backwardGoal) {
        if(heroY() == 0 || backwardGoal->maze().at(heroY() - 1).at(heroX()) != hole) {
            return false;
        }
    }
    if(!Maze::stepDownSeparateHoleAndStone()) return false;
    calcG();
    calcHBackward();
    return true;
}

void MazeStar::updateHeroPosition(int x, int y) noexcept {
    maze_[heroY()][heroX()] = space;
    setHeroX(x);
    setHeroY(y);
    maze_[heroY()][heroX()] = hero;
}

std::ostream& operator<<(std::ostream& out, const MazeStar& maze) {
    out << *(Maze*)&maze;
    return out;
}

namespace AStarNS {
void saveMaze(std::ofstream& out, const Maze* maze) {
    for(int y = 0; y < mazeHeight; ++y) {
        for(int x = 0; x < mazeWidth; ++x) {
            switch(maze->maze().at(y).at(x)) {
            case space: out << '0'; break;
            case hero: out  << '1'; break;
            case wall: out  << '2'; break;
            case hole: out  << '3'; break;
            case stone: out  << '4'; break;
            case hero2: out  << '5'; break;
            }
        }
    }
}
}
