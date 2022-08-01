cmake_minimum_required(VERSION 3.4.1)
project(MazeGenerator)

# Debug and -rdynamic in order the get debug info in show_backtrace function
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++17 -pedantic -Wall -Wextra -Werror -Wno-unused-function -fPIC -rdynamic")
#add_definitions(-DGTEST_HAS_SEH=0)
add_definitions(-D__TEST__)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} . ../cpp/Util ../Server ../cpp ../cpp/Application)
#include_directories(../cpp/Strategies  ../cpp/GameObjects ../cpp/States/Vulture ../cpp/UDP ../cpp/Worlds ../cpp/Factories)
#include_directories(../cpp/Collision ../cpp/Environments ../cpp/Tilesets ../Server)
#include_directories(/usr/include/x86_64-linux-gnu/qt5 /usr/include/x86_64-linux-gnu/qt5/QtCore /usr/include/x86_64-linux-gnu/qt5/QtGui)

# Link runTests with what we want to test and the GTest and pthread library
add_executable(generator main.cpp Maze.cpp Breadthfirst.cpp MazeStar.cpp Generator.cpp BreadthfirstTest.cpp AICommon.cpp TestUtil.cpp BackwardTest.cpp BackwardSearchTest.cpp Level21Test.cpp Level2xSolutionGenerators.cpp ../cpp/Util/stopwatch.cpp ../cpp/Util/CommonUtil.cpp ../Server/ParameterParser.cpp ../cpp/Global.cpp)

#add_library(botutil SHARED IMPORTED)
#set_target_properties(botutil PROPERTIES IMPORTED_LOCATION "../../libbotutil.so")
link_directories(/usr/lib/x86_64-linux-gnu)
target_link_libraries(generator ${GTEST_LIBRARIES} pthread)
