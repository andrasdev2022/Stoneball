#ifndef __GAMEMAP_H__
#define __GAMEMAP_H__

#include "Size.h"
#include "Island.h"
#include <vector>
#include <iostream>

class GameMap {
public:
    GameMap() noexcept;
    virtual ~GameMap() noexcept = default;
    virtual void setSize(int width, int height) noexcept;
    virtual void setSize(SizeI size) noexcept;
    virtual void addBackgroundBitmapId(BitmapId id) noexcept {backgroundBitmapIds_.push_back(id);}

    virtual int createIsland(Island::Type type, int x, int y) noexcept;
    virtual const IslandUPtr& island(int index) const noexcept {
        const IslandUPtr& retv = islands_.at(index);
        return retv;
    }
    virtual size_t islandCount() const noexcept {return islands_.size();}

    virtual SizeI size() const noexcept {return size_;}
    virtual void cacheBackground() noexcept;
    virtual void drawBackground() noexcept;
    virtual uint16_t* background() noexcept {return screenContent.get();}

    virtual void deleteIsland(int index) noexcept {islands_.erase(islands_.begin() + index);}

private:
    SizeI size_;
    std::vector<IslandUPtr> islands_;
    std::unique_ptr<uint16_t[]> screenContent;
    std::vector<BitmapId> backgroundBitmapIds_;
};

using GameMapUPtr = std::unique_ptr<GameMap>;

#endif // __GAMEMAP_H__
