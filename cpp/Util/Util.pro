TARGET = Util
TEMPLATE = lib

win32-msvc {
    CONFIG = staticlib
    QMAKE_CXXFLAGS += -std:c++20 -nologo -D_USE_MATH_DEFINES
}

unix {
    QMAKE_CXX = g++-10
    QMAKE_CXXFLAGS += -std=c++20
}

SOURCES += StringUtil.cpp stopwatch.cpp Serialize.cpp Rect.cpp Math.cpp ClientTimeSyncer.cpp Bitmap565.cpp
SOURCES += CommonUtil.cpp ButtonEventListener.cpp Memory.cpp NameGenerator.cpp IdGenerator.cpp
SOURCES += MemCache.cpp Zlib.cpp

win32-msvc {
	SOURCES += UtilWinDefs.cpp
}

HEADERS += StringUtil.h stopwatch.h Serialize.h Rect.h Math_.h ClientTimeSyncer.h Bitmap565.h
HEADERS += CommonUtil.h ButtonEventListener.h NameGenerator.h IdGenerator.h

OBJECTS_DIR=obj

INCLUDEPATH += $$PWD/../Interfaces
win32-msvc {
    INCLUDEPATH += $$PWD/zlib-1.2.3-32bit/include
}

win32:message("using win32")
win64:message("using win64")
amd64:message("using amd64")
win32-msvc:message("using win32-msvc")
win32-*:message("using win32-*")
