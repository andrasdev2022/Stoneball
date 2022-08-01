#ifndef __FONTINTERFACE_H__
#define __FONTINTERFACE_H__

#include <string>
#include <memory>

class Screen;

class FontInterface {
public:
    enum class Size {
        PIXEL_8,
        PIXEL_12,
        PIXEL_14,
        PIXEL_16,
        PIXEL_24,
        PIXEL_30,
    };
    virtual ~FontInterface() = default;
    virtual void draw(Screen* screen, int x, int y, std::wstring text, bool strikethrough = false) noexcept = 0;
    virtual void drawHCentered(Screen* screen, int y, std::wstring text, bool strikethrough = false) noexcept = 0;
    virtual int width(std::wstring text) noexcept = 0;
    virtual int height(std::wstring text) noexcept = 0;
    virtual void setColor(int r, int g, int b) noexcept = 0;
    virtual uint16_t color() const noexcept = 0;
    virtual void setBold(bool bold) noexcept = 0;
};

using FontInterfaceUPtr = std::unique_ptr<FontInterface>;

#endif // __FONTINTERFACE_H__
