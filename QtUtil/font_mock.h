#ifndef __FONT_MOCK_H__
#define __FONT_MOCK_H__

#include "FontInterface.h"

class font_mock : public FontInterface
{
public:
    font_mock();
    void draw(Screen* screen, int x, int y, std::wstring text, bool strikethrough = false) noexcept override;
    void drawHCentered(Screen* screen, int y, std::wstring text, bool strikethrough = false) noexcept override;
    int width(std::wstring text) noexcept override;
    int height(std::wstring text) noexcept override;
    void setColor(int r, int g, int b) noexcept override;
    uint16_t color() const noexcept override;
    void setBold(bool bold) noexcept override;
};

#endif // FONT_MOCK_H
