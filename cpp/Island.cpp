#include "Island.h"
#include "Global.h"
#include "Screen.h"
#include "Bitmap565.h"
#include "MediaData.h"
#include "BitmapIds.h"

using namespace Bitmap565;

Island::Island(Type type, int x, int y) noexcept
: type_(type)
, position_(x, y)
{
    switch(type_) {
    case Type::Wall04:
        tileIds_.push_back({NATIVE_BITMAP_WALL});
        break;
    case Type::Skull:
        tileIds_.push_back({NATIVE_BITMAP_SKULL});
        break;
    case Type::Bush1:
        tileIds_.push_back({NATIVE_BITMAP_BUSH_01});
        break;
    case Type::Bush2:
        tileIds_.push_back({NATIVE_BITMAP_BUSH_02});
        break;
    case Type::Bush3:
        tileIds_.push_back({NATIVE_BITMAP_BUSH_03});
        break;
    case Type::Lake:
        tileIds_.push_back({NATIVE_BITMAP_LAKE_01});
        break;
    case Type::ROAD_VERTICAL_MIDDLE:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_VERTICAL_MIDDLE});
        break;
    case Type::ROAD_RIGHT_END:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_RIGHT_END});
        break;
    case Type::ROAD_HORIZONTAL_MIDDLE:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_HORIZONTAL_MIDDLE});
        break;
    case Type::ROAD_BOTTOM_LEFT_CORNER:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_BOTTOM_LEFT_CORNER});
        break;
    case Type::ROAD_TOP_LEFT_CORNER:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_TOP_LEFT_CORNER});
        break;
    case Type::ROAD_MIDDLE:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_MIDDLE});
        break;
    case Type::ROAD_TOP_LEFT_CORNER_V2:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_TOP_LEFT_CORNER_V2});
        break;
    case Type::ROAD_TOP_RIGHT_CORNER_V2:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_TOP_RIGHT_CORNER_V2});
        break;
    case Type::ROAD_TOP_MIDDLE_V2:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_TOP_MIDDLE_V2});
        break;
    case Type::ROAD_LEFT_MIDDLE_V2:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_LEFT_MIDDLE_V2});
        break;
    case Type::ROAD_RIGHT_MIDDLE_V2:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_RIGHT_MIDDLE_V2});
        break;
    case Type::ROAD_BOTTOM_MIDDLE_V2:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_BOTTOM_MIDDLE_V2});
        break;
    case Type::ROAD_BOTTOM_RIGHT_CORNER_V2:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_BOTTOM_RIGHT_CORNER_V2});
        break;
    case Type::ROAD_BOTTOM_LEFT_CORNER_V2:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_BOTTOM_LEFT_CORNER_V2});
        break;
    case Type::ROAD_INVERTED_BOTTOM_LEFT_CORNER_V2:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_INVERTED_BOTTOM_LEFT_CORNER_V2});
        break;
    case Type::ROAD_INVERTED_BOTTOM_RIGHT_CORNER_V2:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_INVERTED_BOTTOM_RIGHT_CORNER_V2});
        break;
    case Type::ROAD_INVERTED_TOP_LEFT_CORNER_V2:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_INVERTED_TOP_LEFT_CORNER_V2});
        break;
    case Type::ROAD_INVERTED_TOP_RIGHT_CORNER_V2:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_INVERTED_TOP_RIGHT_CORNER_V2});
        break;
    case Type::ROAD_INVERTED_CENTER_CORNER_V2:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_INVERTED_CENTER_CORNER_V2});
        break;
    case Type::ROAD_CENTER_CORNER_V2:
        tileIds_.push_back({NATIVE_BITMAP_ROAD_CENTER_CORNER_V2});
        break;
    case Type::STARTILE:
        tileIds_.push_back({NATIVE_BITMAP_STARTILE});
        break;
    case Type::EMPTYTILE:
        tileIds_.push_back({NATIVE_BITMAP_EMPTYTILE});
        break;
    case Type::COUNT:
        break;
    }
}

// the decoration is not included
std::vector<Rect> Island::rects() const noexcept {
    std::vector<Rect> retRects;
    Rect rect;

    rect.y = position_.y;
    for(size_t y = 0; y < tileIds_.size(); ++y) {
        rect.x = position_.x;
        for(size_t x = 0; x < tileIds_.at(y).size(); ++x) {
            MediaData* data = bitmapData(tileIds_.at(y).at(x), false);
            rect.width  = data->contentRect().width; //bitmapWidth(tileIds_.at(y).at(x));
            rect.height = data->contentRect().height; //bitmapHeight(tileIds_.at(y).at(x));
            retRects.push_back(rect);
            rect.x += rect.width;
        }
        rect.y += rect.height;
    }
    return retRects;
}

void Island::draw(Screen* screen) const noexcept {
    drawIslands(screen);
    drawDecorations(screen);
}

void Island::drawIslands(Screen* screen) const noexcept {
    // Island is under the screen
    if(position_.y >= screen->y() + screen->height()) return;
    // Island is right to the screen
    if(position_.x >= screen->x() + screen->width()) return;

    // The Screen is a box somewhere on the GameMap
    // I have to draw those pixels of the island which position are in that box (on the Screen)
    Rect rect;

    // Every single tile has to have the same height in a row. It can be different in different rows.
    rect.y = position_.y;
    for(size_t y = 0; y < tileIds_.size(); ++y) {
        rect.x = position_.x;
        for(size_t x = 0; x < tileIds_.at(y).size(); ++x) {
            rect.width  = bitmapWidth(tileIds_.at(y).at(x));
            rect.height = bitmapHeight(tileIds_.at(y).at(x));

            // tile is under the screen
            if(rect.y >= screen->y() + screen->height()) break;
            // tile right to the screen
            if(rect.x >= screen->x() + screen->width()) break;
            // tile is above the screen
            if(rect.y + rect.height <= screen->y()) break;
            // tile left to the screen
            if(rect.x + rect.width <= screen->x()) {
                rect.x += rect.width;
                continue;
            }

            copyIntersection(screen->content(), screen->rect(), screen->rect().width, bitmapPtr(tileIds_.at(y).at(x), false), rect, rect.width, false);
            rect.x += rect.width;
        }
        rect.y += rect.height;
    }
}

void Island::drawDecorations(Screen* screen) const noexcept {
    // The Screen is a box somewhere on the GameMap
    // I have to draw those pixels of the decorations which position are in that box (on the Screen)
    Rect rect;

    for(size_t i = 0; i < decorations_.size(); ++i) {
        const decorPair& pair = decorations_.at(i);
        rect.width  = bitmapWidth(pair.first);
        rect.height = bitmapHeight(pair.first);
        rect.x = position_.x + pair.second.x;
        rect.y = position_.y + pair.second.y - rect.height; // coordinates are bottom left coordinates

        // decoration is under the screen
        if(rect.y >= screen->y() + screen->height()) continue;
        // decoration right to the screen
        if(rect.x >= screen->x() + screen->width()) continue;
        // decoration is above the screen
        if(rect.y + rect.height <= screen->y()) continue;
        // decoration left to the screen
        if(rect.x + rect.width <= screen->x()) continue;

        copyIntersection(screen->content(), screen->rect(), screen->rect().width, bitmapPtr(pair.first, false), rect, rect.width, false);
    }

}
