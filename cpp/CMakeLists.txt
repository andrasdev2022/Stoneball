cmake_minimum_required(VERSION 3.4.1)

#set(CMAKE_C_COMPILER "gcc-4.2")
#set(CMAKE_CXX_COMPILER "g++-10")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -pedantic -Wall -Wextra -Werror -Wno-unused-function -fPIC -funwind-tables -fexceptions -DANDROID_NDK")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pedantic -Wall -Wextra -Werror -std=c++20 -Wno-unused-function -fPIC -funwind-tables -fexceptions -DANDROID_NDK")

# Locate GTest
#set(GOOGLETEST_ROOT ${ANDROID_NDK}/sources/third_party/googletest)
#add_library(gtest STATIC ${GOOGLETEST_ROOT}/src/gtest_main.cc ${GOOGLETEST_ROOT}/src/gtest-all.cc)
#target_include_directories(gtest PRIVATE ${GOOGLETEST_ROOT})
#target_include_directories(gtest PUBLIC ${GOOGLETEST_ROOT}/include)

include_directories(${GTEST_INCLUDE_DIRS} unit_tests)

add_library(stoneball SHARED
            stoneball.c
            SoundEffect.cpp
            MediaData.cpp
            TouchEventHandler.cpp
            StaticObjects.cpp
            Global.cpp
            GameMap.cpp
            Island.cpp
            Screen.cpp
            Observers/MenuTouchEvent.cpp
            Application/Application.cpp
            Application/ApplicationExports.cpp
            Application/UDPMessageHandler.cpp
            Application/Menu/MainMenu.cpp
            Application/Menu/LevelsMenu.cpp
            Application/Menu/MultiplayerMenu.cpp
            Application/Menu/OnlineMenu.cpp
            Application/Menu/MultiplayerCreateGameMenu.cpp
            Application/Menu/MultiplayerFindGameMenu.cpp
            Application/Menu/SettingsMenu.cpp
            Application/Menu/AboutMenu.cpp
            Application/Menu/MultiplayerOneDeviceGameParameters.cpp
            Application/Menu/ApplicationMenu.cpp
            Application/Menu/AnimatedKnightsForMenu.cpp
            Game/Game.cpp
            Game/SingleplayerGame.cpp
            Game/MultiplayerGame.cpp
            Builders/MenuBuilder.cpp
            MoveHistory.cpp
            Tilesets/Tileset.cpp
            Tilesets/TilesetMovable.cpp
            Tilesets/Level1.cpp
            Tilesets/Level2.cpp
            Tilesets/Level3.cpp
            Tilesets/Level4.cpp
            Tilesets/Level5.cpp
            Tilesets/Level6.cpp
            Tilesets/Level7.cpp
            Tilesets/Level8.cpp
            Tilesets/Level9.cpp
            Tilesets/Level10.cpp
            Tilesets/Level11.cpp
            Tilesets/Level12.cpp
            Tilesets/Level13.cpp
            Tilesets/Level14.cpp
            Tilesets/Level15.cpp
            Tilesets/Level16.cpp
            Tilesets/Level17.cpp
            Tilesets/Level18.cpp
            Tilesets/Level19.cpp
            Tilesets/Level20.cpp
            Tilesets/Level21.cpp
            Tilesets/Level22.cpp
            Tilesets/Level23.cpp
            Tilesets/Level24.cpp
            Tilesets/Level25.cpp
            Tilesets/Level26.cpp
            Tilesets/Level27.cpp
            Tilesets/Level28.cpp
            Tilesets/Level29.cpp
            Tilesets/Level30.cpp
            Tilesets/Level31.cpp
            Tilesets/Level32.cpp
            Tilesets/PlayWithSomeone.cpp
            Tilesets/PlayWithFriend1.cpp
            Tilesets/PlayWithFriend2.cpp
            Tilesets/PlayWithFriend3.cpp
            Tilesets/PlayWithFriend4.cpp
            Tilesets/PlayWithFriend5.cpp
            Tilesets/PlayWithFriend6.cpp
            Tilesets/Multiplayer1.cpp
            Tilesets/Multiplayer2.cpp
            Tilesets/Multiplayer3.cpp
            Tilesets/Multiplayer4.cpp
            Tilesets/Multiplayer5.cpp
            Tilesets/Multiplayer6.cpp
            Tilesets/TilesetCardHandler.cpp
            Worlds/SingleplayerWorld.cpp
            Worlds/MultiplayerWorld.cpp
            Worlds/WorldTouchEvent.cpp
            Worlds/SnapshotSyncer.cpp
            Collision/Collision.cpp
            Collision/GameObjectCollider.cpp
            GameObjects/GameObject.cpp
            GameObjects/Knight.cpp
            GameObjectDescriptor.cpp
            GameObjects/Button.cpp
            GameObjects/Particle.cpp
            GameObjects/RollingStone.cpp
            GameObjects/RPGBgElement.cpp
            GameObjects/BlueTriangle.cpp
            GameObjects/BouncingStones.cpp
            GameObjects/Messagebox1.cpp
            GameObjects/MessageboxYesNo.cpp
            GameObjects/MessageboxTermsOfUse.cpp
            GameObjects/Listbox1.cpp
            GameObjects/Selectionbox.cpp
            GameObjects/DeckOfCards.cpp
            GameObjects/Card.cpp
            GameObjects/CardPull.cpp
            GameObjects/CardPushPlayer.cpp
            GameObjects/CardTeleport.cpp
            GameObjects/Questionmark.cpp
            GameObjects/Slider.cpp
            GameObjects/MatchResults.cpp
            GameObjects/Fireworks.cpp
            GameObjects/Trophy.cpp
            GameObjects/EmotesBubble.cpp
            GameObjects/EmoteFigure.cpp
            Util/Rect.cpp
            Util/Bitmap565.cpp
            Util/stopwatch.cpp
            Util/Serialize.cpp
            Util/NTPClient.cpp
            Util/ClientTimeSyncer.cpp
            Util/Math.cpp
            Util/AndroidFont.cpp
            Util/StringUtil.cpp
            Util/CommonUtil.cpp
            Util/Memory.cpp
            Util/NameGenerator.cpp
            Util/IdGenerator.cpp
            Util/MemCache.cpp
            Util/Zlib.cpp
            UDP/UDPMessageQueue.cpp
            UDP/UDPServer.cpp
            UDP/UDP.cpp
            UDP/UDPMessageData.cpp
            UDP/UDPConnectionRequest.cpp
            UDP/UDPReconnectionRequest.cpp
            UDP/UDPLobbyRequest.cpp
            UDP/UDPLobbyData.cpp
            UDP/UDPServerSnapshot.cpp
            UDP/UDPServerDeltaSnapshot.cpp
            UDP/UDPUserCommands.cpp
            UDP/UDPConnection.cpp
            UDP/UDPRoomListRequest.cpp
            UDP/UDPRoomListAnswer.cpp
            UDP/UDPSimpleMessage.cpp
            UDP/UDPPackageCreator.cpp
            States/Knight/StateKnightIdle.cpp
            States/Knight/StateKnightRun.cpp
            States/Knight/StateKnightSit.cpp
            Strategies/Strategy.cpp
            Strategies/HeroWalkStrategy.cpp
            Strategies/HeroStraightWalkStrategy.cpp
            Factories/KnightFactory.cpp
            Factories/LevelFactory.cpp
            Factories/FontFactory.cpp
            Factories/TranslatorFactory.cpp
            Factories/UDPConnectionFactory.cpp
            Factories/UDPMessageFactory.cpp
            Factories/UDPServerFactory.cpp
            Factories/BlueTriangleFactory.cpp
            Factories/UDPMessageQueueFactory.cpp
            Factories/GameFactory.cpp
            Environments/Environment.cpp
            AI/Maze.cpp
            AI/AStar.cpp
            Localization/Translator.cpp
#            ECS/Implementations/Entity.cpp
#            ECS/Factories/EntityFactory.cpp
#            ECS/Implementations/DrawingStrategy.cpp
#            ECS/Implementations/KnightIdleState.cpp
#            ECS/Implementations/UpdateEntity.cpp
#            ECS/Implementations/ChangeEntityState.cpp
)

include_directories(stoneball PRIVATE . States States/Knight Observers Strategies Util GameObjects UDPMessages)
include_directories(GameObjects UDP Worlds Factories Environments Collision Tilesets AI Localization Interfaces Application)
include_directories(Application/Menu Builders Game)
#include_directories(ECS/Interfaces ECS/Factories ECS/Implementations ${GOOGLETEST_ROOT}/include)

# Include libraries needed for stoneball lib
target_link_libraries(stoneball
                      android
                      jnigraphics
                      log
                      m
                      z)
