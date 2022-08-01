#include "GameMap.h"
#include "Global.h"
#include "Bitmap565.h"
#include "Screen.h"

using namespace Bitmap565;

GameMap::GameMap() noexcept
: size_(-1, -1)
{
}

// It must be as big as the screen or bigger
void GameMap::setSize(int width, int height) noexcept {
    size_.width = width;
    size_.height = height;
}

// It must be as big as the screen or bigger
void GameMap::setSize(SizeI size) noexcept {
    size_ = size;
}

int GameMap::createIsland(Island::Type type, int x, int y) noexcept {
    islands_.push_back(std::make_unique<Island>(type, x, y));
    return islands_.size() - 1;
}

void GameMap::cacheBackground() noexcept {
    drawBackground();
    Screen screen(0, 0, size_.width, size_.height);
    screen.setContentPtr(screenContent.get());
    for(size_t i = 0; i < islands_.size(); ++i) {
        islands_[i]->draw(&screen);
    }
}

void GameMap::drawBackground() noexcept {
    screenContent = std::make_unique<uint16_t[]>(size_.width * size_.height);
    if(backgroundBitmapIds_.size() == 0) return;
    uint16_t* bg = bitmapPtr(backgroundBitmapIds_[0], false);
    int bgWidth = bitmapWidth(backgroundBitmapIds_[0]);
    int bgHeight = bitmapHeight(backgroundBitmapIds_[0]);
    // What is our drawing strategy?

    // V1. No resize, tiles aligned to the bottom left corner of the map

    // V2.  No resize, tiles aligned to the bottom left corner of the map
    //     The tiles are different from the second row from the bottom
    if(size_.height < bgHeight) {
        // The background image is higher than the GameMap
        // TODO
    }
    if(size_.width < bgWidth) {
        // The background image is wider than the GameMap
        // TODO
    }

    //LOGI("DBG bgWidth: %d, bgHeight: %d", bgWidth, bgHeight);
    //LOGI("DBG size_.width: %d, size_.height: %d", size_.width, size_.height);
    //LOGI("DBG screen width: %d, screen height: %d", rect_.width, rect_.height);

    Rect rect(0, 0, size_.width, size_.height);
    size_t bgBitmapIndex = 0;
    for(int drawPosY = size_.height - bgHeight; drawPosY > -bgHeight; drawPosY -= bgHeight) {
        for(int drawPosX = 0; drawPosX < size_.width; drawPosX += bgWidth) {
            //debug("drawPosX: %d, drawPosY: %d, size_.width: %d, size_.height: %d", drawPosX, drawPosY, size_.width, size_.height);
            copyIntersection(screenContent.get(), rect, rect.width, bg, Rect(drawPosX, drawPosY, bgWidth, bgHeight), bgWidth, true);
        }
        if(bgBitmapIndex < backgroundBitmapIds_.size() - 1) {
            ++bgBitmapIndex;
            bg = bitmapPtr(backgroundBitmapIds_[bgBitmapIndex], false);
            bgWidth = bitmapWidth(backgroundBitmapIds_[bgBitmapIndex]);
            bgHeight = bitmapHeight(backgroundBitmapIds_[bgBitmapIndex]);
        }
    }
}
