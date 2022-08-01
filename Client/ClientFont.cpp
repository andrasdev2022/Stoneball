#include "ClientFont.h"
#include "MediaData.h"
#include <QFontMetrics>
#include "Bitmap565.h"
#include <QPainter>
#include "Global.h"
#include "Application.h"
#include "Environment.h"
#include "Tileset.h"
#include "Algorithms.h"
#include "Screen.h"
#include "CommonUtil.h"

using namespace  Algorithms;

ClientFont::ClientFont(TranslateInterface::Language lang, FontInterface::Size size)
: lang_(lang)
, size_(size)
//, font_("Ubuntu Mono")
, font_("Ubuntu")
{
#ifdef WIN32
    font_.setWeight(QFont::Bold);
#else
    font_.setWeight(QFont::DemiBold);
#endif
    switch(size) {
    case FontInterface::Size::PIXEL_8:  font_.setPixelSize(6._ZRD); break;
    case FontInterface::Size::PIXEL_12: font_.setPixelSize(9._ZRD); break;
    case FontInterface::Size::PIXEL_14: font_.setPixelSize(11._ZRD); break;
    case FontInterface::Size::PIXEL_16: font_.setPixelSize(14._ZRD); break;
    case FontInterface::Size::PIXEL_24: font_.setPixelSize(22._ZRD); break;
    case FontInterface::Size::PIXEL_30: font_.setPixelSize(28._ZRD); break;
/*
    case FontInterface::Size::PIXEL_8:  font_.setPointSize(5._ZRD); break;
    case FontInterface::Size::PIXEL_12: font_.setPointSize(9._ZRD); break;
    case FontInterface::Size::PIXEL_16: font_.setPointSize(13._ZRD); break;
    case FontInterface::Size::PIXEL_24: font_.setPointSize(21._ZRD); break;
    case FontInterface::Size::PIXEL_30: font_.setPointSize(27._ZRD); break;
*/
    }
    font_.setLetterSpacing(QFont::AbsoluteSpacing, .1_ZRD);
    font_.setStyleStrategy((QFont::StyleStrategy)(QFont::NoSubpixelAntialias|QFont::PreferAntialias));
}

void ClientFont::draw([[maybe_unused]]Screen* screen, int x, int y, std::wstring text, bool strikethrough /*= false*/) noexcept {
    //if(x < screen->x() || x >= screen->width()) return;
    //if(x < screen->x() || x >= screen->width()) return;
    //if(y < screen->y() || y >= screen->height()) return;
    //if(y < screen->y() || y >= screen->height()) return;

/*
    if(anyOf(Application::activeMenu(), MenuInterface::MenuType::SINGLEPLAYER_RUNNIG, MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING,
    MenuInterface::MenuType::MULTIPLAYER_HOST_RUNNING, MenuInterface::MenuType::MULTIPLAYER_JOINED_RUNNING)) {
        texts_.push_back(TextInfo{x - screen->rect().x, y - screen->rect().y, text, strikethrough, color_});
    } else {
        texts_.push_back(TextInfo{x, y, text, strikethrough, color_});
    }
    */

    font_.setStrikeOut(strikethrough);
    //y += 8;
    int textXPos = std::max(x, screen->x());
    int textYPos = std::max(y, screen->y());

    int textWidth = width(text) + 5_ZRD;
    int textHeight = height(text);

    int right = std::min(x + textWidth, screen->x() + screen->width());
    int bottom = std::min(y + textHeight, screen->y() + screen->height());

    int textNewWidth = right - textXPos;
    int textNewHeight = bottom - textYPos;

    if(textNewWidth <= 0 || textNewHeight <= 0) return;

    textXPos -= screen->x();
    textYPos -= screen->y();

    //std::wcout << L"text:" << text << L"\n";
    //std::wcout << L"x: " << x << L", y: " << y << L"\n";
    //std::wcout << L"textXPos:" << textXPos << L"\n";
    //std::wcout << L"textYPos:" << textYPos << L"\n";
    //std::wcout << L"screen->x():" << screen->x() << L"\n";
    //std::wcout << L"screen->y():" << screen->y() << L"\n";
    //std::wcout << L"screen->width():" << screen->width() << L"\n";
    //std::wcout << L"screen->height():" << screen->height() << L"\n";
    //debug("textNewWidth: %d, textNewHeight: %d", textNewWidth, textNewHeight);

    QImage output(textNewWidth, textNewHeight, QImage::Format_RGB16);
    for(int line = 0; line < textNewHeight; ++line) {
        assert((void*)output.scanLine(line));
        assert(screen);
        assert(screen->content());
        memcpy((void*)output.scanLine(line), &screen->content()[(textYPos + line) * (size_t)screen->width() + textXPos], (size_t)textNewWidth * 2);
    }
    QPainter p(&output);
    p.setFont(font_);
    p.setPen(color_);
    p.drawText(textNewWidth - textWidth, textNewHeight - 5_ZRD, QString::fromStdWString(text));

    for(int line = 0; line < textNewHeight; ++line) {
        memcpy(&screen->content()[(textYPos + line) * (size_t)screen->width() + textXPos], (void*)output.scanLine(line), (size_t)textNewWidth * 2);
    }
}

void ClientFont::drawHCentered([[maybe_unused]]Screen* screen, int y, std::wstring text, bool strikethrough /*= false*/) noexcept {
    int textWidth = width(text);
    int x = (screenWidth() - textWidth) / 2;// + screenPosX();
    draw(screen, x, y, text, strikethrough);
}

int ClientFont::width(std::wstring text) noexcept {
    QFontMetrics fontMetrics(font_);
    QRect bounds;
    bounds = fontMetrics.boundingRect(QString::fromStdWString(text));
    //std::wcout << L"bounds.width(): " << bounds.width() << L"\n";
    return bounds.width() + 3_ZRD;
}

int ClientFont::height(std::wstring text) noexcept {
    QFontMetrics fontMetrics(font_);
    return fontMetrics.boundingRect(QString::fromStdWString(text)).height() + 3_ZRD;
}

void ClientFont::setColor(int r, int g, int b) noexcept {
    color_ = QColor(r, g, b);
}

uint16_t ClientFont::color() const noexcept {
    return Bitmap565::make565(color_.red(), color_.green(), color_.blue());
}
/*
void ClientFont::flush(QImage& screen) noexcept {
    QPainter p(&screen);
    p.setFont(font_);
//    Rect r = Application::screen()->rect();
    for(auto& info : texts_) {
        p.setPen(info.color);
        font_.setStrikeOut(info.strikethrough);
        if(anyOf(Application::activeMenu(), MenuInterface::MenuType::SINGLEPLAYER_RUNNIG, MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING,
        MenuInterface::MenuType::MULTIPLAYER_HOST_RUNNING, MenuInterface::MenuType::MULTIPLAYER_JOINED_RUNNING)) {
        //if(Application::environment() && Application::environment()->tileset()) {
            p.drawText(info.x - Application::tileset()->position().x - screenPosX(),
                       info.y - Application::tileset()->position().y + height(info.text) - 8_ZRD,
                       QString::fromStdWString(info.text));
        } else {
            p.drawText(info.x, info.y + height(info.text) - 8_ZRD, QString::fromStdWString(info.text));
        }
    }
    texts_.clear();
    font_.setStrikeOut(false);
}
*/

void ClientFont::setBold(bool bold) noexcept {
    font_.setBold(bold);
}
