#include "Generator.h"
#include <iostream>
#include <array>
#include <vector>
#include <set>
#include <iomanip>
#include "Algorithms.h"
#include <assert.h>
#include "MazeStarV3.h"
#include "Breadthfirst.h"
#include "AStar.h"
#include <string.h>
#include "stopwatch.h"
#include "TestUtil.h"

template<typename T>
void Unused(T...) {

}

namespace Generator {
StopWatch stopWatch;

struct Trio {
    uint8_t first{}; // x coordinate
    uint8_t second{}; // y coordinate
    uint8_t third{}; // wall, hero2, etc.
    bool operator<(const Trio& o) const {
        if(first < o.first) return true;
        if(first == o.first && second < o.second) return true;
        if(first == o.first && second == o.second && third < o.third) return true;
        return false;
    }
};

std::array<Trio, wallCount + hero2Count + stoneAndHoleCount * 2 + heroCount> walls;
std::set<std::set<Trio>> combinations;

int maximumPush = 0;
int maximumDepth = 0;
void search(const Maze::MazeArray& mazeArray, bool searchA) {
    MazeStar maze;
    maze = mazeArray;
    maze.setCardsAmount(teleportCount, pushCount, pullCount);

    std::cerr << "\nTest case: " << maze;

    stopWatch.start("Generate::search");
    if(searchA) {
        AStar<MazeStar> aStar;
        aStar << maze;
        aStar.setMaximumDepth(depthLimit);
        if(aStar.search(false, false)) {
            if(25 < aStar.pushCounter()/* || maximumDepth < aStar.depth()*/) {
                maximumPush = maximumPush >= aStar.pushCounter() ? maximumPush : aStar.pushCounter();
                maximumDepth = maximumDepth >= aStar.depth() ? maximumDepth : aStar.depth();
                std::cout << "                  Solution's depth: " << aStar.depth() << "\n";
                aStar.printFirstState();
            }
        }
        std::cerr << "Generate::search A* duration: " << stopWatch.timeMs("Generate::search") / 1000. << " ms\n";
    } else {
        BreadthFirst breadthFirst;
        breadthFirst << maze;
        breadthFirst.setMaximumDepth(depthLimit);
        if(breadthFirst.search(false)) {
            if(20 < breadthFirst.pushCounter() || maximumDepth < breadthFirst.depth()) {
                maximumPush = maximumPush >= breadthFirst.pushCounter() ? maximumPush : breadthFirst.pushCounter();
                maximumDepth = maximumDepth >= breadthFirst.depth() ? maximumDepth : breadthFirst.depth();
                std::cout << "                  Solution's depth: " << breadthFirst.depth() << "\n";
                breadthFirst.printFirstState();
            }
        }
        std::cerr << "Generate::search Breadthfirst duration: " << stopWatch.timeMs("Generate::search") / 1000. << " ms" << std::endl;
        std::cerr.flush();
        std::cout.flush();
    }
}

void searchBothDirections(const Maze::MazeArray& mazeArray) {
    std::cerr << "\nTest case: " << mazeArray;
    AStarV3 aStar;
    MazeStarV3 maze;
    maze = mazeArray;
    maze.setCardsAmount(teleportCount, pushCount, pullCount);

    MazeStarV3 mazeEnd;
    mazeEnd.start2EndState(mazeArray);
    mazeEnd.setMaximumHoleCount(maze.holeCount());
    mazeEnd.setCardsAmount(0, 0, 0);
    mazeEnd.setMaximumCardsAmount(teleportCount, pushCount, pullCount);

    aStar << maze;
    aStar <<= mazeEnd;
    aStar.setMaximumDepth(depthLimit);
    aStar.setMaximumDepthBackward(depthLimit);
    aStar.setDirection(AICommon::Direction::BOTH);
    aStar.setTimeout(40e6);
    if(aStar.search(false, false) && 10 < aStar.pushCounter() + aStar.pushCounterBackward()) {
        auto steps = aStar.stepString();
        std::cout << std::string(80, '-') << "\n";
        std::cout << "                  Solution's depth: " << steps.length() << "\n";
        aStar.printFirstState();
        aStar.printFirstStateBackward();
        std::cout << "steps: " << steps << " (" << steps.length() << ")\n";
        std::cout << "Push counters: " << aStar.pushCounter() + aStar.pushCounterBackward() << "\n";
        /*
        AStar<MazeStar> aStarTest;
        MazeStar mazeTest;
        mazeTest = mazeArray;
        aStarTest << mazeTest;
        aStarTest.testSteps(steps);
        */
        std::cout << std::string(80, '-') << "\n";
    }
}

void addRandomPosition(Trio& trio, char element) {
    trio.first = (uint8_t)(rand() % mazeWidth);
    trio.second = (uint8_t)(rand() % mazeHeight);
    trio.third = element;
}

void fillWalls(Maze::MazeArray& m, decltype(walls)& w, int from, int to, char element) {
    for(int s = from; s < to; ++s) {
        do {
            addRandomPosition(w[s], element);
        } while(m[w[s].second][w[s].first] != ' ');
        m[w[s].second][w[s].first] = element;
    }
}

void start(bool searchA) {
    if(searchA) {
        std::cout << "A* search\n";
    } else {
        std::cout << "BTF search\n";
    }
    std::cout << "constexpr int wallCount = " << wallCount << "\n";
    std::cout << "constexpr int hero2Count = " << hero2Count << "\n";
    std::cout << "constexpr int stoneAndHoleCount = " << stoneAndHoleCount << "\n";
    std::cout << "constexpr int teleportCount = " << teleportCount << "\n";
    std::cout << "constexpr int pushCount = " << pushCount << "\n";
    std::cout << "constexpr int pullCount = " << pullCount << "\n";
    std::cout << "constexpr int depthLimit = " << depthLimit << "\n";


    srand(time(0));
    const Maze::MazeArray mazeArray
    {};/*{{
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
           }};*/

    //search(mazeArray, true);
    //search(mazeArray, false);
    walls = {};

    while(true) {
        auto m = mazeArray;
        auto w = walls;

        static size_t comb = 0;
        std::cerr << "Combination number: " << ++comb << ", combinations.size: " << combinations.size() << "     \r";
        if(!(comb % 1000)) {srand(time(0));}

        int begin = 0;
        int end = wallCount;
        fillWalls(m, w, begin, end, wall);

        begin = end;
        end += hero2Count;
        fillWalls(m, w, begin, end, hero2);

        begin = end;
        end += stoneAndHoleCount;
        fillWalls(m, w, begin, end, stone);

        begin = end;
        end += stoneAndHoleCount;
        fillWalls(m, w, begin, end, hole);

        begin = end;
        end += heroCount;
        fillWalls(m, w, begin, end, hero);

        //std::set<Trio> w2Set;
        //for(auto a : w) w2Set.insert(a);

        //auto r = combinations.insert(std::move(w2Set));

        //if(r.second) {
            search(m, searchA);
        //} else {
        //    std::cerr << "Combination was generated before!\n";
        //}
    }
}

void experiment([[maybe_unused]]bool searchA) {
    if(searchA) {
        std::cout << "A* search\n";
    } else {
        std::cout << "BTF search\n";
    }
    std::cout << "constexpr int heroCount = " << heroCount << "\n";
    std::cout << "constexpr int wallCount = " << wallCount << "\n";
    std::cout << "constexpr int hero2Count = " << hero2Count << "\n";
    std::cout << "constexpr int stoneAndHoleCount = " << stoneAndHoleCount << "\n";
    std::cout << "constexpr int teleportCount = " << teleportCount << "\n";
    std::cout << "constexpr int pushCount = " << pushCount << "\n";
    std::cout << "constexpr int pullCount = " << pullCount << "\n";
    std::cout << "constexpr int depthLimit = " << depthLimit << "\n";


    srand(time(0));
/*
                  Solution's depth: 78
                  Push count: 35
//0123456789
 "* x   x*\n" // 0
 "*o  o H*\n" // 1
 "*HHH   *\n" // 2
 "*  H hH*\n" // 3
 "*ox    *\n" // 4
Depth: 1

                  Solution's depth: 116
                  Push count: 31
//0123456789
 "* xH  x*\n" // 0
 "*o  o H*\n" // 1
 "*HHH   *\n" // 2
 "*  Hh  *\n" // 3
 "*ox    *\n" // 4
Depth: 1

*/
    std::cerr << "Generator experiment has started\n";
    const Maze::MazeArray mazeArray = TestUtil::convert(
       //123456
        "      " // 1
        "      " // 2
        "      " // 3
        "      " // 4
        "      " // 5
        "      " // 6
);

    //search(mazeArray, true);
    //searchBothDirections(mazeArray);
    walls = {};

    while(true) {
        auto m = mazeArray;
        auto w = walls;

        static size_t comb = 0;
        std::cerr << "Combination number: " << ++comb << ", combinations.size: " << combinations.size() << "     \n";
        if(!(comb % 1000)) {srand(time(0));}

        int begin = 0;
        int end = wallCount;
        fillWalls(m, w, begin, end, wall);

        begin = end;
        end += hero2Count;
        fillWalls(m, w, begin, end, hero2);

        begin = end;
        end += stoneAndHoleCount;
        fillWalls(m, w, begin, end, stone);

        begin = end;
        end += stoneAndHoleCount;
        fillWalls(m, w, begin, end, hole);

        begin = end;
        end += heroCount;
        fillWalls(m, w, begin, end, hero);

        std::set<Trio> w2Set;
        for(auto a : w) w2Set.insert(a);

        auto r = combinations.insert(std::move(w2Set));
        if(r.second) {
            //searchBothDirections(m);
            //search(m, true);
            TestUtil::mazeArray = m;
            TestUtil::findWithCardIteration(teleportCount);
        } else {
            std::cerr << "Combination was generated before!\n";
        }
    }
}

void generateNNDataset() {
    const Maze::MazeArray mazeArray = TestUtil::convert(
       //1234567890123
        "             " // 1
        "             " // 2
        "             " // 3
        "             " // 4
        "             " // 5
);

    //search(mazeArray, true);
    //search(mazeArray, true);
    walls = {};

    while(true) {
        auto m = mazeArray;
        auto w = walls;

        static size_t comb = 0;
        std::cerr << "Combination number: " << ++comb << ", combinations.size: " << combinations.size() << "     \n";
        if(!(comb % 1000)) {srand(time(0));}

        int begin = 0;
        int end = wallCount;
        fillWalls(m, w, begin, end, wall);

        begin = end;
        end += hero2Count;
        fillWalls(m, w, begin, end, hero2);

        begin = end;
        end += stoneAndHoleCount;
        fillWalls(m, w, begin, end, stone);

        begin = end;
        end += stoneAndHoleCount;
        fillWalls(m, w, begin, end, hole);

        begin = end;
        end += heroCount;
        fillWalls(m, w, begin, end, hero);

        std::set<Trio> w2Set;
        for(auto a : w) w2Set.insert(a);

        auto r = combinations.insert(std::move(w2Set));

        if(r.second) {
            AStar<MazeStar> aStar;
            MazeStar maze;
            maze = m;
            maze.setCardsAmount(0, 0, 0);
            aStar << maze;
            aStar.setMaximumDepth(depthLimit);
            aStar.setExportNodesForNN(true);
            if(aStar.search(false, true)) {

            }
        } else {
            std::cerr << "Combination was generated before!\n";
        }
    }

}

}
