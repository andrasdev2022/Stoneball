#ifndef __QTUTIL_H__
#define __QTUTIL_H__

#include <QImage>
#include <memory>
#include <iostream>
#include "Rect.h"
#include "mock.h"

namespace QtUtil {
    std::unique_ptr<uint16_t[]>& convert(const QImage& img);
    QRgb from565(uint16_t pixel);
    std::ostream& operator<<(std::ostream& out, const Rect rect);
    void storeQtBitmap(std::vector<QImage>& images, int bitmapId, int imageIndex, double scale = 1., MOCK::StoreBitmapType storageType = MOCK::NATIVE_BITMAP_STORE_TYPE_STANDARD);
    void cutSpriteSheet(QImage spriteSheet, int rows, int cols, std::vector<QImage>& storage);
    void cutFontBitmap(QImage font, std::vector<QImage>& storage);
}

#endif // __QTUTIL_H__
