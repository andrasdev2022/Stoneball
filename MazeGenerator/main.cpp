#include <iostream>
#include <array>
#include <vector>
#include <set>
#include <iomanip>
#include "Algorithms.h"
#include <assert.h>
#include "Breadthfirst.h"
#include "MazeStarV3.h"
#include "AStar.h"
#include <string.h>
#include <chrono>
#include "Generator.h"
#include "ParameterParser.h"
#include <gtest/gtest.h>

using namespace std::chrono;

void printUsage() noexcept {
    std::cout << "Usage: ./generator [-a] [-b] [-g]\n"
                 "-a      AStart search\n"
                 "-b      Breadth-first search\n"
                 "-ga     Maze A* search with the implemented prarameters\n"
                 "-gb     Maze Breadthfirst search with the implemented prarameters\n"
                 "-ex     Experiment"
                 "-t      Breadth-first tests\n";
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printUsage();
        return EXIT_SUCCESS;
    }

    ParameterParser paramParser(argc, (const char**)argv);
    MazeStar maze;
/*
    Maze::MazeArray mazeArray
          {{
            {' ', 'x', ' ', ' ', ' '},
            {'*', 'o', ' ', ' ', 'o'},
            {'x', '*', '*', ' ', 'H'},
            {'x', ' ', 'H', 'o', 'h'},
            {' ', ' ', '*', ' ', ' '},
          }};
*/
    Maze::MazeArray mazeArray;
    maze = mazeArray;
    maze.setCardsAmount(teleportCount, pushCount, pullCount);

    auto now1 = high_resolution_clock::now();
    if(paramParser.find("-b") != paramParser.size()) {
        BreadthFirst breadthFirst;
        breadthFirst << maze;
        breadthFirst.search(true);
    } else if(paramParser.find("-a") != paramParser.size()) {
        AStar<MazeStar> aStar;
        aStar << maze;
        aStar.search(true, false);
    } else if(paramParser.find("-ga") != paramParser.size()) {
        Generator::start(true);
    } else if(paramParser.find("-gb") != paramParser.size()) {
        Generator::start(false);
    } else if(paramParser.find("-ex") != paramParser.size()) {
        Generator::experiment(true);
    } else if(paramParser.find("-NN") != paramParser.size()) {
        Generator::generateNNDataset();
    } else if(paramParser.find("-t") != paramParser.size()) {
        testing::InitGoogleTest(&argc, (char**)argv);
        size_t p = paramParser.find("-filter");
        if(p + 1 < paramParser.size()) {
            testing::GTEST_FLAG(filter)=paramParser.at(p + 1);
        }
        return RUN_ALL_TESTS();
    } else {
        printUsage();
        return EXIT_SUCCESS;
    }
    auto now2 = high_resolution_clock::now();
    std::cout << "Elapsed time: " << duration_cast<milliseconds>(now2 - now1).count() << " millis\n";
    return EXIT_SUCCESS;
}
