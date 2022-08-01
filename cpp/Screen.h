#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "Rect.h"
#include <memory>

class GameMap;
class GameObject;

class Screen {
public:
    Screen(int x, int y, int width, int height) noexcept;
    virtual ~Screen() noexcept = default;
    virtual int x() const noexcept {return rect_.x;}
    virtual void setX(int x) noexcept {rect_.x = x;}
    virtual int y() const noexcept {return rect_.y;}
    virtual void setY(int y) noexcept {rect_.y = y;}
    virtual int width() const noexcept {return rect_.width;}
    virtual int height() const noexcept {return rect_.height;}
    virtual Rect rect() const noexcept {return rect_;}
    virtual uint16_t pixel(int x, int y) const noexcept;
    virtual void setPixel(int x, int y, uint16_t color) noexcept;
    virtual void draw(GameMap* gameMap) noexcept;
    virtual void drawBackground(GameMap* gameMap) noexcept;
    virtual uint16_t* content() const noexcept {return screenContent;}
    virtual void setContentPtr(uint16_t* c) noexcept {screenContent = c;}
    virtual void follow(GameObject* obj, GameMap* gameMap) noexcept;
    virtual void copyPreparedBackground(GameMap* gameMap) noexcept;

private:

    Rect rect_;
    //std::unique_ptr<uint16_t[]> screenContent;
    uint16_t* screenContent;
};

using ScreenUPtr = std::unique_ptr<Screen>;

#endif // __SCREEN_H__
