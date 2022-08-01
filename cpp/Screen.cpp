#include "Screen.h"
#include "GameMap.h"
#include "Global.h"
#include "Bitmap565.h"
#include "GameObject.h"
#include "MediaData.h"
#include "BitmapIds.h"

using namespace Bitmap565;

Screen::Screen(int x, int y, int width, int height) noexcept
: rect_(x, y, width, height)
, screenContent(nullptr)
{
}

uint16_t Screen::pixel(int x, int y) const noexcept {
    return *(screenContent + rect_.width * y + x);
}

void Screen::draw(GameMap* gameMap) noexcept {
    // gameMap, knight, etc
    // kikeresem azonak az objektumokat, amelyek részben vagy teljesen látszanak
    // a screenen, és a látható részt kirajzolom

    // 1. Background drawing
    drawBackground(gameMap);

    // 2. islands drawing
    for(size_t i = 0; i < gameMap->islandCount(); ++i) {
        const IslandUPtr& island = gameMap->island(i);
        // absolute position of islands
        island->draw(this);
    }
}

void Screen::drawBackground(GameMap* gameMap) noexcept {
    uint16_t* bg = bitmapPtr(NATIVE_BITMAP_BACKGROUND, false);
    const int bgWidth = bitmapWidth(NATIVE_BITMAP_BACKGROUND);
    const int bgHeight = bitmapHeight(NATIVE_BITMAP_BACKGROUND);
    // What is our drawing strategy?

    // V1. No resize, tiles aligned to the bottom left corner of the map

    // V2.  No resize, tiles aligned to the bottom left corner of the map
    //     The tiles are different from the second row from the bottom
    SizeI mapSize = gameMap->size();
    if(mapSize.height < bgHeight) {
        // The background image is higher than the GameMap
        // TODO
    }
    if(mapSize.width < bgWidth) {
        // The background image is wider than the GameMap
        // TODO
    }

    //LOGI("DBG bgWidth: %d, bgHeight: %d", bgWidth, bgHeight);
    //LOGI("DBG mapSize.width: %d, mapSize.height: %d", mapSize.width, mapSize.height);
    //LOGI("DBG screen width: %d, screen height: %d", rect_.width, rect_.height);

    for(int drawPosY = mapSize.height - bgHeight; drawPosY > -bgHeight; drawPosY -= bgHeight) {
        for(int drawPosX = 0; drawPosX < mapSize.width; drawPosX += bgWidth) {
            // continue, if we are under the screen
            if(drawPosY >= rect_.y + rect_.height) continue;
            // continue, if we are right to the screen
            if(drawPosX >= rect_.x + rect_.width) continue;
            // continue, if we are above the screen
            if(drawPosY + bgHeight <= rect_.y) continue;
            // continue, if we are left to the screen
            if(drawPosX + bgWidth <= rect_.x) continue;

            copyIntersection(screenContent, rect_, rect_.width, bg, Rect(drawPosX, drawPosY, bgWidth, bgHeight), bgWidth, true);
        }
    }
}

void Screen::setPixel(int x, int y, uint16_t color) noexcept {
    screenContent[rect_.width * y + x] = color;
}

void Screen::follow(GameObject* obj, GameMap* gameMap) noexcept {
    SizeI mapSize = gameMap->size();

    // 1. change the screen x,y coordinates in order to place the gameobject into that rect
    int gameObjectCenterX = obj->x() + obj->width() / 2;
    int gameObjectCenterY = obj->y() + obj->height() / 2;
    int screenCenterX = rect_.x + rect_.width / 2;
    int screenCenterY = rect_.y + rect_.height / 2;

    //std::cout << "gameObjectCenterY: " << gameObjectCenterY << ", screenCenterY: " << screenCenterY << "\n";

    rect_.x += gameObjectCenterX - screenCenterX;
    rect_.y += gameObjectCenterY - screenCenterY;

    //std::cout << "Screen::follow: rect_.y: " << rect_.y << ", rect_.height: " << rect_.height << ", mapSize.height: " << mapSize.height << "\n";

    // 2. correct the screen x,y coordinates, if the screen is out of the gamemap
    if(rect_.x < 0) rect_.x = 0;
    if(rect_.y < 0) rect_.y = 0;

    if(rect_.x + rect_.width > mapSize.width) rect_.x = mapSize.width - rect_.width;
    if(rect_.y + rect_.height > mapSize.height) rect_.y = mapSize.height - rect_.height;
}

void Screen::copyPreparedBackground(GameMap* gameMap) noexcept {
    SizeI mapSize = gameMap->size();
    //std::cout << "Screen::copyPreparedBackground: " << rect_ << ", " << mapSize << "\n";
    copyIntersection(screenContent, rect_, rect_.width, gameMap->background(), Rect(0, 0, mapSize.width, mapSize.height), mapSize.width, true);
    //copyIntersection(screenContent, rect_, rect_.width, gameMap->background(), Rect(rect_.x, rect_.y, mapSize.width, mapSize.height), mapSize.width, true);
}
