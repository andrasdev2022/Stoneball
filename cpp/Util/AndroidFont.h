#ifndef __ANDROIDFONT_H__
#define __ANDROIDFONT_H__

#include "FontInterface.h"
#include "TranslateInterface.h"

class Screen;

class AndroidFont : public FontInterface
{
    AndroidFont(TranslateInterface::Language lang, FontInterface::Size size);
    friend class FontFactory;
public:
    void draw(Screen* screen, int x, int y, std::wstring text, bool strikethrough = false) noexcept override;
    void drawHCentered(Screen* screen, int y, std::wstring text, bool strikethrough = false) noexcept override;
    int width(std::wstring text) noexcept override;
    int height(std::wstring text) noexcept override;
    void setColor(int r, int g, int b) noexcept override;
    uint16_t color() const noexcept override;
    void setBold(bool bold) noexcept override;

private:
    int size() const noexcept;
    
    TranslateInterface::Language lang_;
    int rgb_[3];
    FontInterface::Size size_;
};

#endif // ANDROIDFONT_H