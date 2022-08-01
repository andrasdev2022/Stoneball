QT += core

#greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = LoadBalancer
TEMPLATE = app

#CONFIG += link_pkgconfig
#PKGCONFIG += tbb

SOURCES += LoadBalancer.cpp ProcessHandler.cpp Application.cpp UDPMessageFactory.cpp

SOURCES += ../cpp/Factories/UDPConnectionFactory.cpp
SOURCES += ../cpp/Factories/UDPMessageQueueFactory.cpp
SOURCES += ../cpp/UDP/UDP.cpp
SOURCES += ../cpp/UDP/UDPConnection.cpp
SOURCES += ../cpp/UDP/UDPServer.cpp
SOURCES += ../cpp/UDP/UDPMessageQueue.cpp
SOURCES += ../cpp/UDP/UDPLobbyRequest.cpp
SOURCES += ../cpp/UDP/UDPLobbyData.cpp
SOURCES += ../cpp/UDP/UDPRoomListAnswer.cpp
SOURCES += ../cpp/UDP/UDPRoomListRequest.cpp
SOURCES += ../cpp/UDP/UDPSimpleMessage.cpp
SOURCES += ../cpp/UDP/UDPPackageCreator.cpp
SOURCES += ../cpp/Factories/UDPServerFactory.cpp
SOURCES += ../cpp/Global.cpp
SOURCES += ../QtUtil/ParameterParser.cpp
#SOURCES += ../cpp/SoundEffect.cpp

OBJECTS_DIR=obj

QMAKE_CXXFLAGS_RELEASE -= -O2

QMAKE_CXXFLAGS += -std=c++17 -g -O0 -Wall -Wextra -pedantic -D__LINUX__ -D__TEST__ -pthread -fPIC -march=native \
          -I../cpp/Util -I../cpp/UDP -I../cpp/Factories -I../cpp/Interfaces -I../cpp/Application -I../cpp -I../QtUtil \
          -I../cpp/Observers -I../Server
QMAKE_CXX = g++
LIBS += -ldl
LIBS += "-L../cpp/Util" -lUtil -lz

#system(./compile_util)

