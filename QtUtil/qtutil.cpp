#include "qtutil.h"
#include "Global.h"
#include "Bitmap565.h"
#include <QDir>
#include "Defines.h"
#include "BitmapIds.h"

using namespace Bitmap565;

namespace QtUtil {

    constexpr int transparentColor_ = 0;
    THREAD_LOCAL std::unique_ptr<uint16_t[]> converted_;

    std::unique_ptr<uint16_t[]>& convert(const QImage& img) {
        converted_ = std::make_unique<uint16_t[]>(img.width() * img.height());
        memset(converted_.get(), transparentColor_, img.width() * img.height() * sizeof(uint16_t));
        //std::cout << img.width() << " " << img.height() << "\n";
        for(int y = 0; y < img.height(); ++y) {
            for(int x = 0; x < img.width(); ++x) {
                QRgb rgb = img.pixel(x, y);
                if(qAlpha(rgb) == 255) { // nem átlátszó
                    converted_.get()[img.width() * y + x] = make565(qRed(rgb), qGreen(rgb), qBlue(rgb));
                }
            }
        }
        return converted_;
    }

    QRgb from565(uint16_t pixel) {
        //return (uint16_t)((red & 0x1f) << 11 | (green & 0x3f) << 5 | (blue & 0x1f));
        //unsigned int red = (pixel >> 11) << 3;
        //unsigned int green = ((pixel & 0b1111100000) >> 5) << 2;
        //unsigned int blue = (pixel & 0x1f) << 3;
        //unsigned char* p = (unsigned char*)&pixel;
        //pixel = *(p + 1) + ( (*p) << 8);

        unsigned int red   = pixel >> 11;
        unsigned int green = (pixel & 0b0000011111100000) >> 5;
        unsigned int blue  = pixel & 0b0000000000011111;

        unsigned int R8 = ( red * 527 + 23 ) >> 6;
        unsigned int G8 = ( green * 259 + 33 ) >> 6;
        unsigned int B8 = ( blue * 527 + 23 ) >> 6;

        return qRgb(R8, G8, B8);
      }

    std::ostream& operator<<(std::ostream& out, const Rect rect) {
        out << "Rect(" << rect.x << ", " << rect.y << ", " << rect.width << ", " << rect.height<< ")";
        return out;
    }

    void storeQtBitmap(std::vector<QImage>& images, int bitmapId, int imageIndex, double scale, MOCK::StoreBitmapType storageType) {
        int newWidth = ZRD(images[imageIndex].width()) * scale;
        int newHeight = ZRD(images[imageIndex].height()) * scale;
        //LOGI("DBG: Image imageIndex: %d, new size: (%d, %d), zoomRateWidth(): %f, zoomRateHeight(): %f, scale: %f", imageIndex, newWidth, newHeight,
        //     zoomRateWidth(), zoomRateHeight(), scale);
        if(images[imageIndex].isNull()) {
            LOGI("ERROR: Image is null, imageIndex: %d, LAST_ONE: %d", imageIndex, BitmapId::LAST_ONE);
        }
        QImage scaled = images[imageIndex].scaled(newWidth, newHeight, Qt::IgnoreAspectRatio);
        storeBitmap(convert(scaled).get(), bitmapId, newWidth, newHeight, newWidth * sizeof(uint16_t), storageType);
    }

    void cutSpriteSheet(QImage spriteSheet, int rows, int cols, std::vector<QImage>& storage) {
        int stepX = spriteSheet.width() / cols;
        int stepY = spriteSheet.height() / rows;
        for(int row = 0; row < rows; ++row) {
            for(int col = 0; col < cols; ++col) {
                storage.push_back(spriteSheet.copy(col * stepX, row * stepY, stepX, stepY));
            }
        }
    }

    void cutFontBitmap(QImage font, std::vector<QImage>& storage) {
        QDir dir;
        dir.mkpath("/tmp/drawable");
        //int index = 0;

        int nextCharX = 0;
        for(int i = 0; i <= font.width(); ++i) {
            if(i == font.width() || font.pixel(i, 0) == qRgb(0x7f, 0x7f, 0x7f)) {
                QImage img(i - nextCharX, font.height(), QImage::Format_RGB32);
                for(int y = 0; y < font.height(); ++y) {
                    for(int x = nextCharX; x < i; ++x) {
                        img.setPixelColor(x - nextCharX, y, font.pixel(x, y));
                        /*
                        if(font.pixel(x, y) != qRgb(0x0, 0x0, 0x0)) {
                            img.setPixelColor(x - nextCharX, y, font.pixel(x, y));
                        } else {
                            img.setPixelColor(x - nextCharX, y, QColor::fromRgb(4, 4, 4, 255));
                        }
                        */
                    }
                }
                //img.save(QString::asprintf("/tmp/drawable/%2d.png", ++index), "PNG");
                storage.push_back(img);
                nextCharX = i + 1;
            }
        }
    }

} // namespace QtUtil
