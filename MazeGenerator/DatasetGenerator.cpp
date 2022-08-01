#include <fstream>
#include <unordered_map>
#include <iostream>
#include <assert.h>

constexpr int mazeWidth = 13;
constexpr int mazeHeight = 5;

std::unordered_map<std::string, int> dataSet;
int duplication = 0;
int classes[1024]{};
int classIndex = 0;

template <typename T>
bool insert(T&& maze, int distance) {
    auto ret = dataSet.emplace(maze, distance);
    if(!ret.second && distance < ret.first->second) {
        std::cerr << maze << " is duplicated (" << duplication++ << ")\n";
    } else {
        if(ret.second && classes[distance] == -1) {
            classes[distance] = classIndex++;
        }
    }
    //std::cout << "'" << maze << "'\n";
    return ret.second;
}

template <typename T>
std::string rotate(T&& maze, int degree) {
    std::string retV(mazeWidth * mazeHeight, ' ');

    int posOrig, posNew;
    for(int y = 0; y < mazeHeight; ++y) {
        for(int x = 0; x < mazeWidth; ++x) {
            posOrig = y * mazeWidth + x;
            switch(degree) {
            case 90: {
                int x2 = mazeHeight - y - 1;
                int y2 = x;
                posNew = y2 * mazeHeight + x2;
                retV[posNew] = maze[posOrig];
                break;
            }
            case 180: {
                int x2 = mazeWidth - x - 1;
                int y2 = mazeHeight - y - 1;
                posNew = y2 * mazeWidth + x2;
                retV[posNew] = maze[posOrig];
                break;
            }
            case 270: {
                int x2 = y;
                int y2 = mazeWidth - x - 1;
                posNew = y2 * mazeHeight + x2;
                retV[posNew] = maze[posOrig];
                break;
            }
            }
        }
    }

    assert(retV.length() == maze.length());
    return retV;
}

template <typename T>
std::string flip(T&& maze, bool horizontal) {
    std::string retV(mazeWidth * mazeHeight, ' ');

    int posOrig, posNew;
    for(int y = 0; y < mazeHeight; ++y) {
        for(int x = 0; x < mazeWidth; ++x) {
            posOrig = y * mazeWidth + x;
            if(horizontal) {
                int x2 = mazeWidth - x - 1;
                int y2 = y;
                posNew = y2 * mazeWidth + x2;
                retV[posNew] = maze[posOrig];
            } else {
                int x2 = x;
                int y2 = mazeHeight - y - 1;
                posNew = y2 * mazeWidth + x2;
                retV[posNew] = maze[posOrig];
            }
        }
    }

    assert(retV.length() == maze.length());
    return retV;
}

bool loadRawDataset() {
    std::ifstream file("./dataset.txt", std::ios_base::in);
    if(!file.is_open()) {
        std::cerr << "Cannot open file\n";
        return false;
    }
    std::string line;
    while(!file.eof()) {
        getline(file, line);
        if(line.length() == 0) continue;
        size_t pos = line.find(' ');
        std::string maze = line.substr(0, pos);
        int distance = std::stoi(line.substr(pos + 1));

        assert(maze.length() == mazeWidth * mazeHeight);

        insert(maze, distance);
        insert(rotate(maze, 90), distance);
        insert(rotate(maze, 180), distance);
        insert(rotate(maze, 270), distance);
        std::string flippedH = flip(maze, true);
        insert(flippedH, distance);
        std::string flippedV = flip(maze, false);
        insert(flippedV, distance);

        insert(rotate(flippedH, 90), distance);
        //insert(rotate(flippedH, 180), distance);
        insert(rotate(flippedH, 270), distance);

        //insert(rotate(flippedV, 90), distance);
        //insert(rotate(flippedV, 180), distance);
        //insert(rotate(flippedV, 270), distance);

    }
    file.close();
    std::cout << "dataSet.size: " << dataSet.size() << "\n";
    return true;
}

void saveConvertedDataset() {
    // we will save only the unsolvable and solved categories
    std::fstream file("./converted_dataset.txt", std::ios_base::out | std::ios_base::trunc);
    if(!file.is_open()) return;
    for(auto& item : dataSet) {
        if(item.second == 1000 || item.second <= 30) {
            file << item.first << " " << item.second << "\n";
        }
    }
    file.flush();
    file.close();

    std::fstream file2("./converted_dataset_classes.txt", std::ios_base::out | std::ios_base::trunc);
    if(!file2.is_open()) return;
    for(size_t j = 0; j < classIndex; ++j) {
        for(size_t i = 0; i < 1024; ++i) {
            if(classes[i] == j) {
                file2 << "C" << i;
                if(j + 1 != classIndex) file2 << "\n";
                break;
            }
        }
    }
    file2.flush();
    file2.close();
}

void rotateTests() {
    std::string testMaze("             "
                         "          *  "
                         "             "
                         "             "
                         "             ");
    std::string rotated1 = rotate(testMaze, 90);

    std::string expected1("     "
                          "     "
                          "     "
                          "     "
                          "     "
                          "     "
                          "     "
                          "     "
                          "     "
                          "     "
                          "   * "
                          "     "
                          "     ");

    assert(!rotated1.compare(expected1));
    std::string rotated2 = rotate(testMaze, 180);
    std::string expected2("             "
                          "             "
                          "             "
                          "  *          "
                          "             ");
    assert(!rotated2.compare(expected2));

    std::string rotated3 = rotate(testMaze, 270);
    std::string expected3("     "
                          "     "
                          " *   "
                          "     "
                          "     "
                          "     "
                          "     "
                          "     "
                          "     "
                          "     "
                          "     "
                          "     "
                          "     ");

    assert(!rotated3.compare(expected3));
    std::cout << "rotateTests passed\n";
}

void flipTests() {
    std::string testMaze("             "
                         "          *  "
                         "             "
                         "             "
                         "             ");
    std::string flipped1 = flip(testMaze, true);
    std::string expected1("             "
                          "  *          "
                          "             "
                          "             "
                          "             ");
    assert(!flipped1.compare(expected1));

    std::string flipped2 = flip(testMaze, false);
    std::string expected2("             "
                          "             "
                          "             "
                          "          *  "
                          "             ");
    assert(!flipped2.compare(expected2));
    std::cout << "flipTests passed\n";
}

void printClasses() {
    int classcounter[1024];
    std::fill(classcounter, classcounter + 1024, 0);

    for(auto& item : dataSet) {++classcounter[item.second];}
    for(size_t i = 0; i < classIndex; ++i) {
        for(int j = 0; j < 1024; ++j) {
            if(classes[j] == i) {
                std::cout << "Class: " << i << ", count: " << classcounter[j] << ", h(): " << j << "\n";
                break;
            }
        }
    }
}

int main() {
    rotateTests();
    flipTests();

    for(size_t i = 0; i < 1024; ++i) classes[i] = -1;
    if(loadRawDataset()) {
        saveConvertedDataset();
        printClasses();
    }

    return EXIT_SUCCESS;
}
