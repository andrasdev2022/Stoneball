#ifndef __ISLAND_H__
#define __ISLAND_H__

#include <vector>
#include <memory>
#include "Position.h"
#include "Rect.h"

class Screen;
enum BitmapId : int;

class Island {
public:
    enum class Type {
        Wall04,
        Skull,
        Bush1,
        Bush2,
        Bush3,
        Lake,
        ROAD_VERTICAL_MIDDLE,
        ROAD_RIGHT_END,
        ROAD_HORIZONTAL_MIDDLE,
        ROAD_BOTTOM_LEFT_CORNER,
        ROAD_TOP_LEFT_CORNER,
        ROAD_MIDDLE,
        ROAD_TOP_LEFT_CORNER_V2,
        ROAD_TOP_RIGHT_CORNER_V2,
        ROAD_TOP_MIDDLE_V2,
        ROAD_LEFT_MIDDLE_V2,
        ROAD_RIGHT_MIDDLE_V2,
        ROAD_BOTTOM_MIDDLE_V2,
        ROAD_BOTTOM_RIGHT_CORNER_V2,
        ROAD_BOTTOM_LEFT_CORNER_V2,
        ROAD_INVERTED_BOTTOM_LEFT_CORNER_V2,
        ROAD_INVERTED_BOTTOM_RIGHT_CORNER_V2,
        ROAD_INVERTED_TOP_LEFT_CORNER_V2,
        ROAD_INVERTED_TOP_RIGHT_CORNER_V2,
        ROAD_INVERTED_CENTER_CORNER_V2,
        ROAD_CENTER_CORNER_V2,
        STARTILE,
        EMPTYTILE,
        COUNT
    };

    Island(Type type, int x, int y) noexcept;
    virtual ~Island() noexcept = default;
    virtual std::vector<Rect> rects() const noexcept; // returns with the islands' rects
    virtual void draw(Screen* screen) const noexcept;

protected:
    virtual void drawIslands(Screen* screen) const noexcept;
    virtual void drawDecorations(Screen* screen) const noexcept;

private:
    Type type_;
    std::vector<std::vector<BitmapId>> tileIds_; // these should be considered in collisions
    using decorPair = std::pair<BitmapId, PositionI>;
    std::vector<decorPair> decorations_; // position is the bottom left position of the bitmap, these are not considered in collisions
    PositionI position_;
};

using IslandUPtr = std::unique_ptr<Island>;
#endif // __ISLAND_H__
