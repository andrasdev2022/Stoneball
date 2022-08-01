#ifndef __CLIENTFONT_H__
#define __CLIENTFONT_H__

#include "TranslateInterface.h"
#include "FontInterface.h"
#include <QFont>
#include <QColor>

class Screen;

class ClientFont : public FontInterface
{
    ClientFont(TranslateInterface::Language lang, FontInterface::Size size);
    friend class FontFactory;
public:
    void draw(Screen* screen, int x, int y, std::wstring text, bool strikethrough = false) noexcept override;
    void drawHCentered(Screen* screen, int y, std::wstring text, bool strikethrough = false) noexcept override;
    int width(std::wstring text) noexcept override;
    int height(std::wstring text) noexcept override;
    void setColor(int r, int g, int b) noexcept override;
    uint16_t color() const noexcept override;
    void setBold(bool bold) noexcept override;

    //void flush(QImage& screen) noexcept;
/*
    struct TextInfo {
        int x{};
        int y{};
        std::wstring text;
        bool strikethrough{};
        QColor color{};
    };
*/
private:
    TranslateInterface::Language lang_;
    QColor color_{};
    FontInterface::Size size_;
//    std::vector<TextInfo> texts_;
    QFont font_;
};

#endif // __CLIENTFONT_H__
