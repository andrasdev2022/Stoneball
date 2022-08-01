QT += core gui widgets multimedia

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets
RESOURCES = Stoneball.qrc

TARGET = Stoneball
TEMPLATE = app

unix {
	CONFIG += link_pkgconfig
	PKGCONFIG += tbb
}

win32-msvc {
    CONFIG += windows
    CONFIG += no_batch
	RC_FILE = Stoneball.rc
}

SOURCES += ClientApplication.cpp MainWindow.cpp ClientCanvas.cpp ProfileWindow.cpp FontFactory.cpp ClientFont.cpp AboutWindow.cpp
SOURCES += ../cpp/Collision/Collision.cpp
SOURCES += ../cpp/Collision/GameObjectCollider.cpp
SOURCES += ../cpp/StaticObjects.cpp
SOURCES += ../cpp/GameObjects/Knight.cpp
SOURCES += ../cpp/GameObjects/Particle.cpp
SOURCES += ../cpp/GameObjects/Button.cpp
SOURCES += ../cpp/GameObjects/RollingStone.cpp
SOURCES += ../cpp/GameObjects/RPGBgElement.cpp
SOURCES += ../cpp/GameObjects/BlueTriangle.cpp
SOURCES += ../cpp/GameObjects/BouncingStones.cpp
SOURCES += ../cpp/GameObjects/Messagebox1.cpp
SOURCES += ../cpp/GameObjects/MessageboxYesNo.cpp
SOURCES += ../cpp/GameObjects/Listbox1.cpp
SOURCES += ../cpp/GameObjects/Selectionbox.cpp
SOURCES += ../cpp/GameObjects/DeckOfCards.cpp
SOURCES += ../cpp/GameObjects/Card.cpp
SOURCES += ../cpp/GameObjects/CardPull.cpp
SOURCES += ../cpp/GameObjects/CardPushPlayer.cpp
SOURCES += ../cpp/GameObjects/CardTeleport.cpp
SOURCES += ../cpp/GameObjects/Questionmark.cpp
SOURCES += ../cpp/GameObjects/Slider.cpp
SOURCES += ../cpp/GameObjects/MatchResults.cpp
SOURCES += ../cpp/GameObjects/GameObject.cpp
SOURCES += ../cpp/GameObjects/Fireworks.cpp
SOURCES += ../cpp/GameObjects/Trophy.cpp
SOURCES += ../cpp/GameObjects/EmotesBubble.cpp
SOURCES += ../cpp/GameObjects/EmoteFigure.cpp
SOURCES += ../cpp/GameObjects/MessageboxTermsOfUse.cpp
SOURCES += ../cpp/Tilesets/Tileset.cpp
SOURCES += ../cpp/Tilesets/TilesetMovable.cpp
SOURCES += ../cpp/Tilesets/Level1.cpp
SOURCES += ../cpp/Tilesets/Level2.cpp
SOURCES += ../cpp/Tilesets/Level3.cpp
SOURCES += ../cpp/Tilesets/Level4.cpp
SOURCES += ../cpp/Tilesets/Level5.cpp
SOURCES += ../cpp/Tilesets/Level6.cpp
SOURCES += ../cpp/Tilesets/Level7.cpp
SOURCES += ../cpp/Tilesets/Level8.cpp
SOURCES += ../cpp/Tilesets/Level9.cpp
SOURCES += ../cpp/Tilesets/Level10.cpp
SOURCES += ../cpp/Tilesets/Level11.cpp
SOURCES += ../cpp/Tilesets/Level12.cpp
SOURCES += ../cpp/Tilesets/Level13.cpp
SOURCES += ../cpp/Tilesets/Level14.cpp
SOURCES += ../cpp/Tilesets/Level15.cpp
SOURCES += ../cpp/Tilesets/Level16.cpp
SOURCES += ../cpp/Tilesets/Level17.cpp
SOURCES += ../cpp/Tilesets/Level18.cpp
SOURCES += ../cpp/Tilesets/Level19.cpp
SOURCES += ../cpp/Tilesets/Level20.cpp
SOURCES += ../cpp/Tilesets/Level21.cpp
SOURCES += ../cpp/Tilesets/Level22.cpp
SOURCES += ../cpp/Tilesets/Level23.cpp
SOURCES += ../cpp/Tilesets/Level24.cpp
SOURCES += ../cpp/Tilesets/Level25.cpp
SOURCES += ../cpp/Tilesets/Level26.cpp
SOURCES += ../cpp/Tilesets/Level27.cpp
SOURCES += ../cpp/Tilesets/Level28.cpp
SOURCES += ../cpp/Tilesets/Level29.cpp
SOURCES += ../cpp/Tilesets/Level30.cpp
SOURCES += ../cpp/Tilesets/Level31.cpp
SOURCES += ../cpp/Tilesets/Level32.cpp
SOURCES += ../cpp/Tilesets/PlayWithSomeone.cpp
SOURCES += ../cpp/Tilesets/PlayWithFriend1.cpp
SOURCES += ../cpp/Tilesets/PlayWithFriend2.cpp
SOURCES += ../cpp/Tilesets/PlayWithFriend3.cpp
SOURCES += ../cpp/Tilesets/PlayWithFriend4.cpp
SOURCES += ../cpp/Tilesets/PlayWithFriend5.cpp
SOURCES += ../cpp/Tilesets/PlayWithFriend6.cpp
SOURCES += ../cpp/Tilesets/Multiplayer1.cpp
SOURCES += ../cpp/Tilesets/Multiplayer2.cpp
SOURCES += ../cpp/Tilesets/Multiplayer3.cpp
SOURCES += ../cpp/Tilesets/Multiplayer4.cpp
SOURCES += ../cpp/Tilesets/Multiplayer5.cpp
SOURCES += ../cpp/Tilesets/Multiplayer6.cpp
SOURCES += ../cpp/Tilesets/TilesetCardHandler.cpp
SOURCES += ../cpp/GameObjectDescriptor.cpp
SOURCES += ../cpp/MediaData.cpp
SOURCES += ../cpp/Global.cpp
SOURCES += ../cpp/GameMap.cpp
SOURCES += ../cpp/Island.cpp
SOURCES += ../cpp/Screen.cpp
SOURCES += ../cpp/SoundEffect.cpp
SOURCES += ../cpp/Application/Application.cpp
SOURCES += ../cpp/Application/ApplicationExports.cpp
SOURCES += ../cpp/Application/UDPMessageHandler.cpp
SOURCES += ../cpp/Application/Menu/MainMenu.cpp
SOURCES += ../cpp/Application/Menu/LevelsMenu.cpp
SOURCES += ../cpp/Application/Menu/MultiplayerMenu.cpp
SOURCES += ../cpp/Application/Menu/OnlineMenu.cpp
SOURCES += ../cpp/Application/Menu/MultiplayerCreateGameMenu.cpp
SOURCES += ../cpp/Application/Menu/MultiplayerFindGameMenu.cpp
SOURCES += SettingsMenu.cpp
SOURCES += ../cpp/Application/Menu/AboutMenu.cpp
SOURCES += ../cpp/Application/Menu/MultiplayerOneDeviceGameParameters.cpp
SOURCES += ../cpp/Application/Menu/ApplicationMenu.cpp
SOURCES += ../cpp/Application/Menu/AnimatedKnightsForMenu.cpp
SOURCES += ../cpp/Game/Game.cpp
SOURCES += ../cpp/Game/SingleplayerGame.cpp
SOURCES += ../cpp/Game/MultiplayerGame.cpp
SOURCES += ../cpp/Builders/MenuBuilder.cpp
SOURCES += ../cpp/TouchEventHandler.cpp
SOURCES += ../cpp/MoveHistory.cpp
SOURCES += ../cpp/Worlds/WorldTouchEvent.cpp
SOURCES += ../cpp/Worlds/SingleplayerWorld.cpp
SOURCES += ../cpp/Worlds/MultiplayerWorld.cpp
SOURCES += ../cpp/Worlds/SnapshotSyncer.cpp
SOURCES += ../cpp/Observers/MenuTouchEvent.cpp
win32-msvc:SOURCES += ../cpp/UDP/UDP_win.cpp
unix:SOURCES += ../cpp/UDP/UDP.cpp
SOURCES += ../cpp/UDP/UDPMessageData.cpp
SOURCES += ../cpp/UDP/UDPConnectionRequest.cpp
SOURCES += ../cpp/UDP/UDPReconnectionRequest.cpp
SOURCES += ../cpp/UDP/UDPServerSnapshot.cpp
SOURCES += ../cpp/UDP/UDPServerDeltaSnapshot.cpp
win32-msvc:SOURCES += ../cpp/UDP/UDPServer_win.cpp
unix:SOURCES += ../cpp/UDP/UDPServer.cpp
SOURCES += ../cpp/UDP/UDPMessageQueue.cpp
SOURCES += ../cpp/UDP/UDPUserCommands.cpp
win32-msvc:SOURCES += ../cpp/UDP/UDPConnection_win.cpp
unix:SOURCES += ../cpp/UDP/UDPConnection.cpp
SOURCES += ../cpp/UDP/UDPLobbyRequest.cpp
SOURCES += ../cpp/UDP/UDPLobbyData.cpp
SOURCES += ../cpp/UDP/UDPRoomListRequest.cpp
SOURCES += ../cpp/UDP/UDPRoomListAnswer.cpp
SOURCES += ../cpp/UDP/UDPSimpleMessage.cpp
SOURCES += ../cpp/UDP/UDPPackageCreator.cpp
SOURCES += ../cpp/States/Knight/StateKnightIdle.cpp
SOURCES += ../cpp/States/Knight/StateKnightRun.cpp
SOURCES += ../cpp/States/Knight/StateKnightSit.cpp
SOURCES += ../cpp/Strategies/Strategy.cpp
SOURCES += ../cpp/Strategies/HeroWalkStrategy.cpp
SOURCES += ../cpp/Strategies/HeroStraightWalkStrategy.cpp
SOURCES += ../cpp/Factories/KnightFactory.cpp
SOURCES += ../cpp/Factories/LevelFactory.cpp
SOURCES += ../cpp/Factories/TranslatorFactory.cpp
SOURCES += ../cpp/Factories/UDPMessageFactory.cpp
SOURCES += ../cpp/Factories/UDPServerFactory.cpp
SOURCES += ../cpp/Factories/UDPConnectionFactory.cpp
SOURCES += ../cpp/Factories/UDPMessageQueueFactory.cpp
SOURCES += ../cpp/Factories/BlueTriangleFactory.cpp
SOURCES += ../cpp/Factories/GameFactory.cpp
SOURCES += ../cpp/Environments/Environment.cpp
SOURCES += ../cpp/AI/Maze.cpp
SOURCES += ../cpp/AI/AStar.cpp
SOURCES += ../cpp/Localization/Translator.cpp
SOURCES += ../QtUtil/qtutil.cpp
SOURCES += ../QtUtil/mock.cpp
SOURCES += ../QtUtil/BitmapLoader.cpp
SOURCES += ../QtUtil/TranslatorFromAndroidStrings.cpp
SOURCES += ../QtUtil/DesktopBitmapIds.cpp

HEADERS += ClientApplication.h MainWindow.h ClientCanvas.h ProfileWindow.h AboutWindow.h

OBJECTS_DIR=obj


unix {
	QMAKE_CXXFLAGS_RELEASE -= -O2

	QMAKE_LFLAGS_RELEASE -= -O1
	QMAKE_LFLAGS = -g -O1 -export-dynamic -rdynamic

	QMAKE_CFLAGS +=-O3
	QMAKE_CXXFLAGS += -std=c++20 -g -O1 -Wall -Wextra -pedantic -D__LINUX__ -pthread -fPIC -march=native \
			  -I/usr/include/x86_64-linux-gnu/qt5/QtCore \
			  -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I../cpp -I../cpp/Util \
			  -I../cpp/Worlds -I../cpp/GameObjects -I../cpp/States -I../cpp/Strategies \
			  -I../cpp/Observers -I../cpp/UDP -I../cpp/States/Knight -I../cpp/Game \
			  -I../cpp/Factories -I../cpp/Environments -I../cpp/Collision -I../cpp/Tilesets \
			  -I../cpp/AI -I../cpp/Localization -I../cpp/Interfaces -I../cpp/Application \
			  -I../cpp/Builders -I../cpp/Application/Menu -I../QtUtil
	QMAKE_CXX = g++-10
	LIBS += -lz -ldl
	LIBS += "-L../cpp/Util" -lUtil
}

win32-msvc {
	QMAKE_LFLAGS += -NODEFAULTLIB:MSVCRT
	QMAKE_CXXFLAGS += -nologo -std:c++20 -D_USE_MATH_DEFINES -DNOMINMAX -I../cpp -I../cpp/Util \
			  -I../cpp/Worlds -I../cpp/GameObjects -I../cpp/States -I../cpp/Strategies \
			  -I../cpp/Observers -I../cpp/UDP -I../cpp/States/Knight -I../cpp/Game \
			  -I../cpp/Factories -I../cpp/Environments -I../cpp/Collision -I../cpp/Tilesets \
			  -I../cpp/AI -I../cpp/Localization -I../cpp/Interfaces -I../cpp/Application \
			  -I../cpp/Builders -I../cpp/Application/Menu -I../QtUtil

	LIBS += "-L../cpp/Util" -lUtil
	LIBS += "-L../cpp/Util/zlib-1.2.3-32bit/lib" -lzlib
}

#system(./compile_util)


#win32:message("using win32")
win64:message("using win64")
amd64:message("using amd64")
win32-msvc:message("using win32-msvc")
#win32-*:message("using win32-*")
