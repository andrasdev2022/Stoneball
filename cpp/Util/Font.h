#ifndef __FONT_H__
#define __FONT_H__

#include <string>
#include <array>
#include "TranslateInterface.h"
#include "FontInterface.h"

class Screen;

class Font : public FontInterface
{
    Font(TranslateInterface::Language lang, FontInterface::Size size);
    friend class FontFactory;
public:
    void draw(Screen* screen, int x, int y, std::wstring text, bool strikethrough = false) noexcept override;
    void drawHCentered(Screen* screen, int y, std::wstring text, bool strikethrough = false) noexcept override;
    int width(std::wstring text) noexcept override;
    int height(std::wstring text) noexcept override;
    void setColor(int r, int g, int b) noexcept override;
    uint16_t color() const noexcept override {return color_;}

private:
    std::wstring::size_type char2Index(wchar_t c) const noexcept;
    void initAlphabets() noexcept;
    int charBitmapIndex() const;
    void setColor(int charIndex) noexcept;
    void drawStrikethrough(Screen* screen, int xFrom, int xTo, int y) noexcept;

    std::array<std::wstring, static_cast<int>(TranslateInterface::Language::LAST_ONE)> alphabets_;
    std::array<std::unique_ptr<uint16_t[]>, static_cast<int>(TranslateInterface::Language::LAST_ONE)> letterColors_;
    TranslateInterface::Language lang_;
    uint16_t color_{};
    FontInterface::Size size_;
};

#endif // __FONT_H__
