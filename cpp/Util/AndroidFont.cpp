#include "AndroidFont.h"
#include "Bitmap565.h"
#include "Screen.h"
#include "stoneball.h"
#include "Global.h"
#include "MediaData.h"

AndroidFont::AndroidFont(TranslateInterface::Language lang, FontInterface::Size size)
: lang_(lang)
, size_(size)
{
    
}

void AndroidFont::draw(Screen* screen, int x, int y, std::wstring text, bool strikethrough /*= false*/) noexcept {
    
    int fontSize = size();
    int textXPos = std::max(x, screen->x());
    int textYPos = std::max(y, screen->y());

    //int textWidth = width(text);
    int textHeight = height(text);

    //int right = std::min(x + textWidth, screen->x() + screen->width());
    int bottom = std::min(y + textHeight, screen->y() + screen->height());

    //int textNewWidth = right - textXPos;
    int textNewHeight = bottom - textYPos;

    textXPos -= screen->x();
    textYPos -= screen->y();

    javaDrawGameText(textXPos, textYPos + textNewHeight - 5_ZRD, text.c_str(), text.length(), strikethrough, fontSize,
                     rgb_[0], rgb_[1], rgb_[2]);
}

void AndroidFont::drawHCentered(Screen* screen, int y, std::wstring text, bool strikethrough /*= false*/) noexcept {
    int textWidth = width(text);
    int x = (screenWidth() - textWidth) / 2;// + screenPosX();
    draw(screen, x, y, text, strikethrough);
}

int AndroidFont::width(std::wstring text) noexcept {
    int fontSize = size();
    int width{}, height{};
    javaGameTextSize(text.c_str(), text.length(), fontSize, &width, &height);
    return width;
}

int AndroidFont::height(std::wstring text) noexcept {
    int fontSize = size();
    int width{}, height{};
    javaGameTextSize(text.c_str(), text.length(), fontSize, &width, &height);
    return height;
}

void AndroidFont::setColor(int r, int g, int b) noexcept {
    rgb_[0] = r;
    rgb_[1] = g;
    rgb_[2] = b;
}

uint16_t AndroidFont::color() const noexcept {
    return Bitmap565::make565(rgb_[0], rgb_[1], rgb_[2]);;
}

int AndroidFont::size() const noexcept {
    switch(size_) {
    case FontInterface::Size::PIXEL_8: return 7;
    case FontInterface::Size::PIXEL_12: return 10;
    case FontInterface::Size::PIXEL_14: return 11;
    case FontInterface::Size::PIXEL_16: return 15;
    case FontInterface::Size::PIXEL_24: return 21;
    case FontInterface::Size::PIXEL_30: return 29;
    }
    return 0;
}

void AndroidFont::setBold(bool bold) noexcept {
    javaSetFontBold(bold);
}
