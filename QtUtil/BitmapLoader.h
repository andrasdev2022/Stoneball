#ifndef __BITMAPLOADER_H__
#define __BITMAPLOADER_H__

#include <vector>
#include <QImage>

class BitmapLoader
{
public:
    BitmapLoader();
    void loadDrawableImages();
    void convertImagesToRGB565();

private:
    void loadBitmapsFromAndroidJava();
    bool replaceWithDesktopVersion(std::string&& imageName);
    void loadDesktopBitmaps();
    std::vector<QImage> images;
};

#endif // __BITMAPLOADER_H__
