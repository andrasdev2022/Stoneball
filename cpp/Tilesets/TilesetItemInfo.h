#ifndef __TILESETITEMINFO_H__
#define __TILESETITEMINFO_H__

#include "BitmapIds.h"
#include "Position.h"
#include <stdint.h>

struct TilesetItemInfo {
    uint32_t gameObjectId;
    PositionI tilePosition;
    BitmapId skinMin; // TODO: there is no id, which identify a gameobject type, because the RPG elements can be different
};

#endif // __TILESETITEMINFO_H__
