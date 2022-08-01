#include "Font.h"
#include "EnumClassOp.h"
#include "Screen.h"
#include "Bitmap565.h"
#include "MediaData.h"
#include "BitmapIds.h"
#include "Global.h"
#include <algorithm>
#include <memory.h>
#include "Memory.h"
#include "CommonUtil.h"

Font::Font(TranslateInterface::Language lang, FontInterface::Size size)
: lang_(lang)
, size_(size)
{
    initAlphabets();
}

void Font::drawHCentered(Screen* screen, int y, std::wstring text, bool strikethrough /*= false*/) noexcept {
    int textWidth = width(text);
    int x = (screenWidth() - textWidth) / 2;// + screenPosX();
    draw(screen, x, y, text, strikethrough);
}

void Font::draw(Screen* screen, int x, int y, std::wstring text, bool strikethrough /*= false*/) noexcept {
    int charX = x;

    for(wchar_t c : text) {
        if(c == L' ') {
            charX += 4_ZRD;
            continue;
        }
        TranslateInterface::Language lang = lang_;
        std::wstring::size_type index = char2Index(c);

        // if the character is not found in the current language, a full search must be done
        while(index == std::wstring::npos && lang_ != TranslateInterface::Language::LAST_ONE) {
            for(lang_ = TranslateInterface::Language::ENGLISH; lang_ != TranslateInterface::Language::LAST_ONE;) {
                index = char2Index(c);
                if(index != std::wstring::npos) {break;}
                EnumClassOp langE(lang_);
                lang_ = ++langE;
            }
        }
        if(index == std::wstring::npos || lang_ == TranslateInterface::Language::LAST_ONE) {
            lang_ = lang;
            continue;
        }
        setColor(index);
        Rect rect(charX, y, 0, 0);
        index += charBitmapIndex();
        rect.width  = bitmapWidth(index);
        rect.height = bitmapHeight(index);

        Bitmap565::copyIntersectionRarematrix(screen->content(), screen->rect(), screen->rect().width,
                         bitmapPtr(index, false), rect);
        charX += rect.width + 1_ZRD;
        lang_ = lang;
    }
    if(strikethrough) {drawStrikethrough(screen, x, charX, y);}
}

std::wstring::size_type Font::char2Index(wchar_t c) const noexcept {
    EnumClassOp langE(lang_);
    return alphabets_[langE].find(c);
}

// If you add letters here, modify the related bitmapids too!!!!!!!!!!!!!!!!!!!!!!!!
void Font::initAlphabets() noexcept {
    EnumClassOp i(TranslateInterface::Language::ENGLISH);
    alphabets_[i] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.-,:+'!?0123456789()[]&@%/\\*#\"<>=|";
    letterColors_[i] = std::make_unique<uint16_t[]>(alphabets_[i].length());
    for(size_t j = 0; j < alphabets_[i].length(); ++j) {letterColors_[i][j] = 0;}

    i = TranslateInterface::Language::HUNGARIAN;
    alphabets_[i] = L"AÁBCDEÉFGHIÍJKLMNOÓÖŐPQRSTUÚÜŰVWXYZaábcdeéfghiíjklmnoóöőpqrstuúüűvwxyz.-,:+'!?0123456789()[]&@%/\\*#\"<>=|";
    letterColors_[i] = std::make_unique<uint16_t[]>(alphabets_[i].length());
    for(size_t j = 0; j < alphabets_[i].length(); ++j) {letterColors_[i][j] = 0;}

    i = TranslateInterface::Language::BULGARIAN;
    alphabets_[i] = L"АБВГДЕЖЗИЙКЛМНОПРCТУФХЦЧШЩЪЬЮЯабвгдежзийклмнопрстуфхцчшщъьюя.-,:+'!?0123456789()[]&@%/\\*#\"<>=|";
    letterColors_[i] = std::make_unique<uint16_t[]>(alphabets_[i].length());
    for(size_t j = 0; j < alphabets_[i].length(); ++j) {letterColors_[i][j] = 0;}
}

int Font::width(std::wstring text) noexcept {
    int lineWidth{};
    for(wchar_t c : text) {
        if(c == L' ') {
            lineWidth += 4_ZRD;
            continue;
        }
        TranslateInterface::Language lang = lang_;
        size_t index = char2Index(c);
        if(index == std::wstring::npos && lang_ != TranslateInterface::Language::ENGLISH) {
            lang_ = TranslateInterface::Language::ENGLISH;
            index = char2Index(c);
        }
        if(index == std::wstring::npos) {
            lang_ = lang;
            continue;
        }
        lineWidth += bitmapWidth(index + charBitmapIndex()) + 1_ZRD;
        lang_ = lang;
    }
    if(lineWidth) {
        lineWidth -= 1_ZRD;
    }
    return lineWidth;
}

int Font::height(std::wstring text) noexcept {
    int lineHeight{};
    for(wchar_t c : text) {
        if(c == L' ') {
            continue;
        }
        TranslateInterface::Language lang = lang_;
        size_t index = char2Index(c);
        if(index == std::wstring::npos && lang_ != TranslateInterface::Language::ENGLISH) {
            lang_ = TranslateInterface::Language::ENGLISH;
            index = char2Index(c);
        }
        if(index == std::wstring::npos) {
            lang_ = lang;
            continue;
        }
        lineHeight = std::max(bitmapHeight(index + charBitmapIndex()), lineHeight);
        lang_ = lang;
    }
    return lineHeight;
}

int Font::charBitmapIndex() const {
    if(size_ == FontInterface::Size::PIXEL_8) {
            switch(lang_) {
            case TranslateInterface::Language::ENGLISH:   return NATIVE_BITMAP_ENGLISH_ABC_8PX; break;
            case TranslateInterface::Language::BULGARIAN: return NATIVE_BITMAP_BULGARIAN_ABC_8PX; break;
            case TranslateInterface::Language::HUNGARIAN: return NATIVE_BITMAP_HUNGARIAN_ABC_8PX; break;
            //case TranslateInterface::Language::RUSSIAN:   return NATIVE_BITMAP_ENGLISH_ABC; break;
            //case TranslateInterface::Language::GERMAN:    return NATIVE_BITMAP_ENGLISH_ABC; break;
            //case TranslateInterface::Language::FRENCH:    return NATIVE_BITMAP_ENGLISH_ABC; break;
            case TranslateInterface::Language::LAST_ONE: return 0;
            }
    } else if(size_ == FontInterface::Size::PIXEL_12) {
        switch(lang_) {
        case TranslateInterface::Language::ENGLISH:   return NATIVE_BITMAP_ENGLISH_ABC_12PX; break;
        case TranslateInterface::Language::BULGARIAN: return NATIVE_BITMAP_BULGARIAN_ABC_12PX; break;
        case TranslateInterface::Language::HUNGARIAN: return NATIVE_BITMAP_HUNGARIAN_ABC_12PX; break;
        //case TranslateInterface::Language::RUSSIAN:   return NATIVE_BITMAP_ENGLISH_ABC; break;
        //case TranslateInterface::Language::GERMAN:    return NATIVE_BITMAP_ENGLISH_ABC; break;
        //case TranslateInterface::Language::FRENCH:    return NATIVE_BITMAP_ENGLISH_ABC; break;
        case TranslateInterface::Language::LAST_ONE: return 0;
        }
    } else if(size_ == FontInterface::Size::PIXEL_14) {
        switch(lang_) {
        // TODO: 14px font images?
        case TranslateInterface::Language::ENGLISH:   return NATIVE_BITMAP_ENGLISH_ABC_12PX; break;
        case TranslateInterface::Language::BULGARIAN: return NATIVE_BITMAP_BULGARIAN_ABC_12PX; break;
        case TranslateInterface::Language::HUNGARIAN: return NATIVE_BITMAP_HUNGARIAN_ABC_12PX; break;
        //case TranslateInterface::Language::RUSSIAN:   return NATIVE_BITMAP_ENGLISH_ABC; break;
        //case TranslateInterface::Language::GERMAN:    return NATIVE_BITMAP_ENGLISH_ABC; break;
        //case TranslateInterface::Language::FRENCH:    return NATIVE_BITMAP_ENGLISH_ABC; break;
        case TranslateInterface::Language::LAST_ONE: return 0;
        }
    } else if(size_ == FontInterface::Size::PIXEL_16) {
        switch(lang_) {
        case TranslateInterface::Language::ENGLISH:   return NATIVE_BITMAP_ENGLISH_ABC; break;
        case TranslateInterface::Language::BULGARIAN: return NATIVE_BITMAP_BULGARIAN_ABC; break;
        case TranslateInterface::Language::HUNGARIAN: return NATIVE_BITMAP_HUNGARIAN_ABC; break;
        //case TranslateInterface::Language::RUSSIAN:   return NATIVE_BITMAP_ENGLISH_ABC; break;
        //case TranslateInterface::Language::GERMAN:    return NATIVE_BITMAP_ENGLISH_ABC; break;
        //case TranslateInterface::Language::FRENCH:    return NATIVE_BITMAP_ENGLISH_ABC; break;
        case TranslateInterface::Language::LAST_ONE: return 0;
        }
    } else if(size_ == FontInterface::Size::PIXEL_24) {
        switch(lang_) {
        case TranslateInterface::Language::ENGLISH:   return NATIVE_BITMAP_ENGLISH_ABC_24PX; break;
        case TranslateInterface::Language::BULGARIAN: return NATIVE_BITMAP_BULGARIAN_ABC_24PX; break;
        case TranslateInterface::Language::HUNGARIAN: return NATIVE_BITMAP_HUNGARIAN_ABC_24PX; break;
        //case TranslateInterface::Language::RUSSIAN:   return NATIVE_BITMAP_ENGLISH_ABC; break;
        //case TranslateInterface::Language::GERMAN:    return NATIVE_BITMAP_ENGLISH_ABC; break;
        //case TranslateInterface::Language::FRENCH:    return NATIVE_BITMAP_ENGLISH_ABC; break;
        case TranslateInterface::Language::LAST_ONE: return 0;
        }
    } else if(size_ == FontInterface::Size::PIXEL_30) {
        switch(lang_) {
        case TranslateInterface::Language::ENGLISH:   return NATIVE_BITMAP_ENGLISH_ABC_30PX; break;
        case TranslateInterface::Language::BULGARIAN: return NATIVE_BITMAP_BULGARIAN_ABC_30PX; break;
        case TranslateInterface::Language::HUNGARIAN: return NATIVE_BITMAP_HUNGARIAN_ABC_30PX; break;
        //case TranslateInterface::Language::RUSSIAN:   return NATIVE_BITMAP_ENGLISH_ABC; break;
        //case TranslateInterface::Language::GERMAN:    return NATIVE_BITMAP_ENGLISH_ABC; break;
        //case TranslateInterface::Language::FRENCH:    return NATIVE_BITMAP_ENGLISH_ABC; break;
        case TranslateInterface::Language::LAST_ONE: return 0;
        }
    }
    return 0;
}

void Font::setColor(int r, int g, int b) noexcept {
    color_ = Bitmap565::make565(r, g, b);
}

void Font::setColor(int charIndex) noexcept {
    EnumClassOp lang(lang_);
    if(letterColors_[lang][charIndex] == color_) return;
    letterColors_[lang][charIndex] = color_;

    int bitmapIndex = charBitmapIndex();
    uint16_t* rareMatrix = bitmapData(bitmapIndex + charIndex, false)->data();
    uint32_t size = ((uint32_t)rareMatrix[0] | ((uint32_t)rareMatrix[1] << 16));
    const uint16_t* rareMatrixEnd = rareMatrix + size;
    ++rareMatrix;++rareMatrix;
    while(rareMatrixEnd > rareMatrix) {
        ++rareMatrix;
        ++rareMatrix;
        int length = *rareMatrix++;
        int jump = length;
        Memory::memset(rareMatrix, color_, length);
        rareMatrix += jump;
    }
}

void Font::drawStrikethrough(Screen* screen, int xFrom, int xTo, int y) noexcept {
    int relX = xFrom - screen->x();
    int relY = y - screen->y();
    uint16_t* dst = screen->content() + relY * screen->width() + relX;
    int thickness = 1_ZRD;
    switch(size_) {
    case FontInterface::Size::PIXEL_8:
        dst += screen->width() * 3_ZRD;
        break;
    case FontInterface::Size::PIXEL_12:
        dst += screen->width() * 5_ZRD;
        thickness = 2_ZRD;
        break;
    case FontInterface::Size::PIXEL_14:
        dst += screen->width() * 6_ZRD;
        thickness = 2_ZRD;
        break;
    case FontInterface::Size::PIXEL_16:
        dst += screen->width() * 7_ZRD;
        thickness = 3_ZRD;
        break;
    case FontInterface::Size::PIXEL_24:
        dst += screen->width() * 11_ZRD;
        thickness = 5_ZRD;
        break;
    case FontInterface::Size::PIXEL_30:
        dst += screen->width() * 15_ZRD;
        thickness = 6_ZRD;
        break;
    }
    for(int i = 0; i < thickness; ++i) {
        Memory::memset(dst, color_, xTo - xFrom);
        dst += screen->width();
    }
}
