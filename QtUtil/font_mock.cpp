#include "font_mock.h"
#include "CommonUtil.h"

font_mock::font_mock()
{

}

void font_mock::draw(Screen* screen, int x, int y, std::wstring text, bool strikethrough) noexcept {
    Unused(0, screen, x, y, text, strikethrough);
}

void font_mock::drawHCentered(Screen* screen, int y, std::wstring text, bool strikethrough) noexcept {
    Unused(0, screen, y, text, strikethrough);
}

int font_mock::width(std::wstring text) noexcept {
    Unused(0, text);
    return 10;
}

int font_mock::height(std::wstring text) noexcept {
    Unused(0, text);
    return 10;
}

void font_mock::setColor(int r, int g, int b) noexcept {
    Unused(0, r, g, b);
}

uint16_t font_mock::color() const noexcept {
    return 100;
}

void font_mock::setBold(bool) noexcept {}
