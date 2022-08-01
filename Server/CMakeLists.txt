cmake_minimum_required(VERSION 3.4.1)
project(Stoneball)

#set(CMAKE_C_COMPILER "gcc-4.2")
set(CMAKE_CXX_COMPILER "g++-10")

# Debug and -rdynamic in order the get debug info in show_backtrace function
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wall -Wextra -Werror -Wno-unused-function -fPIC -rdynamic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -pedantic -Wall -Wextra -Werror -Wno-unused-function -Wno-error=deprecated-copy -Werror=suggest-override -fPIC -rdynamic")

# measure the compilation time
#add_compile_options(-ftime-report)

add_definitions(-D__SERVERAPP__)

# in order to see the compiler command
#set(CMAKE_VERBOSE_MAKEFILE ON)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} . ../cpp ../cpp/Observers ../cpp/States ../cpp/States/Knight)
include_directories(../cpp/Strategies ../cpp/Util ../cpp/GameObjects ../cpp/UDP ../cpp/Worlds ../cpp/Factories)
include_directories(../cpp/Collision ../cpp/Environments ../cpp/Tilesets ../cpp/AI ../cpp/Interfaces ../cpp/Localization ../cpp/Application)
include_directories(../cpp/Application/Menu ../cpp/Builders ../cpp/Game ../QtUtil)
include_directories(/usr/include/x86_64-linux-gnu/qt5 /usr/include/x86_64-linux-gnu/qt5/QtCore /usr/include/x86_64-linux-gnu/qt5/QtGui /usr/include/x86_64-linux-gnu/qt5/QtWidgets)

if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 9.0)
    find_package(TBB REQUIRED)
endif()

link_directories(/usr/lib/x86_64-linux-gnu)

# Link runServer with what we want to test and the GTest and pthread library
add_executable(runServer
                        ServerApplication.cpp
                        FontFactory.cpp
                        main.cpp
                        ../QtUtil/ParameterParser.cpp
                        ../QtUtil/BitmapLoader.cpp
                        ../QtUtil/mock.cpp
                        ../QtUtil/qtutil.cpp
                        ../QtUtil/font_mock.cpp
                        ../QtUtil/TranslatorFromAndroidStrings.cpp
                        ../QtUtil/DesktopBitmapIds.cpp
                        ../cpp/Collision/Collision.cpp
                        ../cpp/Collision/GameObjectCollider.cpp
                        ../cpp/StaticObjects.cpp
                        ../cpp/GameObjectDescriptor.cpp
                        ../cpp/GameObjects/Knight.cpp
                        ../cpp/GameObjects/Particle.cpp
                        ../cpp/GameObjects/Button.cpp
                        ../cpp/GameObjects/GameObject.cpp
                        ../cpp/GameObjects/RollingStone.cpp
                        ../cpp/GameObjects/RPGBgElement.cpp
                        ../cpp/GameObjects/BlueTriangle.cpp
                        ../cpp/GameObjects/BouncingStones.cpp
                        ../cpp/GameObjects/Messagebox1.cpp
                        ../cpp/GameObjects/MessageboxYesNo.cpp
                        ../cpp/GameObjects/MessageboxTermsOfUse.cpp
                        ../cpp/GameObjects/Listbox1.cpp
                        ../cpp/GameObjects/Selectionbox.cpp
                        ../cpp/GameObjects/DeckOfCards.cpp
                        ../cpp/GameObjects/Card.cpp
                        ../cpp/GameObjects/CardPull.cpp
                        ../cpp/GameObjects/CardPushPlayer.cpp
                        ../cpp/GameObjects/CardTeleport.cpp
                        ../cpp/GameObjects/Questionmark.cpp
                        ../cpp/GameObjects/Slider.cpp
                        ../cpp/GameObjects/MatchResults.cpp
                        ../cpp/GameObjects/Fireworks.cpp
                        ../cpp/GameObjects/Trophy.cpp
                        ../cpp/GameObjects/EmotesBubble.cpp
                        ../cpp/GameObjects/EmoteFigure.cpp
                        ../cpp/Environments/Environment.cpp
                        ../cpp/MediaData.cpp
                        ../cpp/Global.cpp
                        ../cpp/GameMap.cpp
                        ../cpp/Island.cpp
                        ../cpp/Screen.cpp
                        ../cpp/SoundEffect.cpp
                        ../cpp/Application/Application.cpp
                        ../cpp/Application/ApplicationExports.cpp
                        ../cpp/Application/UDPMessageHandler.cpp
                        ../cpp/Application/Menu/MainMenu.cpp
                        ../cpp/Application/Menu/LevelsMenu.cpp
                        ../cpp/Application/Menu/MultiplayerMenu.cpp
                        ../cpp/Application/Menu/OnlineMenu.cpp
                        ../cpp/Application/Menu/MultiplayerCreateGameMenu.cpp
                        ../cpp/Application/Menu/MultiplayerFindGameMenu.cpp
                        ../cpp/Application/Menu/SettingsMenu.cpp
                        ../cpp/Application/Menu/AboutMenu.cpp
                        ../cpp/Application/Menu/MultiplayerOneDeviceGameParameters.cpp
                        ../cpp/Application/Menu/ApplicationMenu.cpp
                        ../cpp/Application/Menu/AnimatedKnightsForMenu.cpp
                        ../cpp/Game/Game.cpp
                        ../cpp/Game/SingleplayerGame.cpp
                        ../cpp/Game/MultiplayerGame.cpp
                        ../cpp/Game/MultiplayerGameServerSide.cpp
                        ../cpp/Builders/MenuBuilder.cpp
                        ../cpp/TouchEventHandler.cpp
                        ../cpp/Tilesets/Tileset.cpp
                        ../cpp/Tilesets/TilesetMovable.cpp
                        ../cpp/Tilesets/PlayWithSomeone.cpp
                        ../cpp/Tilesets/PlayWithFriend1.cpp
                        ../cpp/Tilesets/PlayWithFriend2.cpp
                        ../cpp/Tilesets/PlayWithFriend3.cpp
                        ../cpp/Tilesets/PlayWithFriend4.cpp
                        ../cpp/Tilesets/PlayWithFriend5.cpp
                        ../cpp/Tilesets/PlayWithFriend6.cpp
                        ../cpp/Tilesets/Multiplayer1.cpp
                        ../cpp/Tilesets/Multiplayer2.cpp
                        ../cpp/Tilesets/Multiplayer3.cpp
                        ../cpp/Tilesets/Multiplayer4.cpp
                        ../cpp/Tilesets/Multiplayer5.cpp
                        ../cpp/Tilesets/Multiplayer6.cpp
                        ../cpp/Tilesets/TilesetCardHandler.cpp
                        ../cpp/Worlds/WorldTouchEvent.cpp
                        ../cpp/Worlds/SingleplayerWorld.cpp
                        ../cpp/Worlds/MultiplayerWorld.cpp
                        ../cpp/Worlds/SnapshotSyncer.cpp
                        ../cpp/Observers/MenuTouchEvent.cpp
                        ../cpp/UDP/UDP.cpp
                        ../cpp/UDP/UDPMessageData.cpp
                        ../cpp/UDP/UDPConnectionRequest.cpp
                        ../cpp/UDP/UDPReconnectionRequest.cpp
                        ../cpp/UDP/UDPLobbyRequest.cpp
                        ../cpp/UDP/UDPLobbyData.cpp
                        ../cpp/UDP/UDPRoomListRequest.cpp
                        ../cpp/UDP/UDPRoomListAnswer.cpp
                        ../cpp/UDP/UDPServerSnapshot.cpp
                        ../cpp/UDP/UDPServerDeltaSnapshot.cpp
                        ../cpp/UDP/UDPServer.cpp
                        ../cpp/UDP/UDPMessageQueue.cpp
                        ../cpp/UDP/UDPUserCommands.cpp
                        ../cpp/UDP/UDPConnection.cpp
                        ../cpp/UDP/UDPSimpleMessage.cpp
                        ../cpp/UDP/UDPPackageCreator.cpp
                        ../cpp/States/Knight/StateKnightIdle.cpp
                        ../cpp/States/Knight/StateKnightRun.cpp
                        ../cpp/States/Knight/StateKnightSit.cpp
                        ../cpp/Strategies/Strategy.cpp
                        ../cpp/Strategies/HeroWalkStrategy.cpp
                        ../cpp/Strategies/HeroStraightWalkStrategy.cpp
                        ../cpp/Factories/KnightFactory.cpp
                        ../cpp/Factories/LevelFactory.cpp
                        ../cpp/Factories/TranslatorFactory.cpp
                        ../cpp/Factories/UDPMessageFactory.cpp
                        ../cpp/Factories/UDPServerFactory.cpp
                        ../cpp/Factories/UDPConnectionFactory.cpp
                        ../cpp/Factories/BlueTriangleFactory.cpp
                        ../cpp/Factories/UDPMessageQueueFactory.cpp
                        ../cpp/Factories/GameFactoryServerSide.cpp
                        ../cpp/AI/Maze.cpp
                        ../cpp/AI/AStar.cpp
                        ../cpp/Localization/Translator.cpp
                        ../cpp/MoveHistory.cpp
		)
add_library(Util SHARED IMPORTED)
set_target_properties(Util PROPERTIES IMPORTED_LOCATION "../cpp/Util/libUtil.so")
target_link_libraries(runServer ${GTEST_LIBRARIES} pthread Qt5Core Qt5Gui Qt5Widgets z tbb Util)
