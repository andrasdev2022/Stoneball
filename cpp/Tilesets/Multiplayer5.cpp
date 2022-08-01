#include "Multiplayer5.h"
#include "MultiplayerWorld.h"
#include "Algorithms.h"
#include "MediaData.h"
#include "BitmapIds.h"
#include "Card.h"
#include "EmoteFigure.h"

Multiplayer5::Multiplayer5()
: MultiplayerCommon()
{

#ifndef __SERVERAPP__
    tileMap_ =
           //01234567890123456789012345678
           L"                             \n" // 0
            "                             \n" // 1
            "        **************       \n" // 2
            "        *            *       \n" // 3
            "        *            *       \n" // 4
            "        *            *       \n" // 5
            "        *            *       \n" // 6
            "        *            *       \n" // 7
            "        *            *       \n" // 8
            "        *            *       \n" // 9
            "        *            *       \n" // 0
            "        *            *       \n" // 1
            "        *            *       \n" // 2
            "        **************       \n" // 3
            "                             \n" // 4
            "                             \n" // 5
            "                             \n" // 6
;
#endif

}

