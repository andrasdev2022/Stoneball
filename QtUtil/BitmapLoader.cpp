#include "BitmapLoader.h"
#include "Defines.h"
#include "qtutil.h"
#include "BitmapIds.h"
#include <fstream>
#include <regex>
#include <iostream>
#include "Algorithms.h"
#include "DesktopBitmapIds.h"

using namespace QtUtil;
using namespace  Algorithms;

BitmapLoader::BitmapLoader() {
}

void BitmapLoader::loadDrawableImages() {
    loadBitmapsFromAndroidJava();
    loadDesktopBitmaps();
}

void BitmapLoader::convertImagesToRGB565() {
    //std::cout << DesktopBitmapIds::settings_menu_eng_resolution() << ", " << DesktopBitmapIds::settings_menu_hun_resolution() << "\n";
    //std::cout << "images.size(): " << images.size() << ", LAST_ONE: " << LAST_ONE << ", DesktopBitmapIds::last_one(): " << DesktopBitmapIds::last_one() << "\n";
    if(images.size() != (uint32_t)DesktopBitmapIds::last_one()) throw std::runtime_error("BitmapLoader::convertImagesToRGB565: image count mismatch");
    // ONLY the GameObjects' sprites can be NATIVE_BITMAP_STORE_TYPE_RAREMATRIX,
    // the background sprites must be NATIVE_BITMAP_STORE_TYPE_STANDARD
    for(int bitmapIndex = 0; bitmapIndex < LAST_ONE; ++bitmapIndex) {
        if(bitmapIndex > NATIVE_BITMAP_MAIN_MENU_BUL_ABOUT && bitmapIndex < LAST_ONE) {
            storeQtBitmap(images, bitmapIndex, bitmapIndex, 1., MOCK::NATIVE_BITMAP_STORE_TYPE_RAREMATRIX);
        } else {
            storeQtBitmap(images, bitmapIndex, bitmapIndex);
        }
    }

    // Desktop bitmaps
    for(int bitmapIndex = DesktopBitmapIds::settings_menu_eng_resolution(); bitmapIndex < DesktopBitmapIds::last_one(); ++bitmapIndex) {
        storeQtBitmap(images, bitmapIndex, bitmapIndex);
    }
}

void BitmapLoader::loadBitmapsFromAndroidJava() {
    std::string line;
    std::regex pattern {R"x(R\.drawable\.([^\),]+))x"};
    std::regex pattern2 {R"x((loadBackgroundBitmap\(.+?\{)|(loadBackgroundBitmapKeepAspectRatio\(.+?\{)|(loadStandardBitmaps\(.+?\{)|(loadSpriteSheets\(.+?\{)|(loadRarematrixBitmaps\(.+?\{)|(loadFireworksSpriteSheets\(.+?\{)|(loadTrophyAnimationSpriteSheets\(.+?\{))x"};
    std::regex pattern3 {R"x( (rows|cols) = (\d+))x"};

    std::smatch matches;
    std::ifstream file("../java/com/stoneball/BitmapLoader.java", std::ios::in);
    if(!file.is_open()) {
        throw std::runtime_error("File BitmapLoader.java is not found");
    }

    bool skipBitmaps = false;
    bool spritesheets = false;
    int rows = 0;
    int cols = 0;
    while(!file.eof()) {
        getline(file, line);
        if(std::regex_search(line, matches, pattern2) && matches.size() > 0) {
            //std::cout << "function: " << matches[0] << "\n";
            if(matches[0].str().find("loadBackgroundBitmapKeepAspectRatio") != std::string::npos) {
                //std::cout << "Bitmaps will be skipped\n";
                skipBitmaps = true;
                spritesheets = false;
            } else if(matches[0].str().find("loadSpriteSheets") != std::string::npos ||
            matches[0].str().find("loadTrophyAnimationSpriteSheets") != std::string::npos ||
            matches[0].str().find("loadFireworksSpriteSheets") != std::string::npos) {
                skipBitmaps = false;
                spritesheets = true;
            } else {
                skipBitmaps = false;
                spritesheets = false;
            }
        }
        else if(std::regex_search(line, matches, pattern) && matches.size() > 0) {
            if(!skipBitmaps) {
                //std::cout << "Bitmap: ../res/drawable/" << matches[1] << ".png, spritesheets: " << spritesheets << "\n";
                if(spritesheets) {
                    if(anyOf(matches[1].str(), "male01_red_knight", "male164_dark_knight", "female_032", "female_061",
                    "bluetriangle", "redbouncingstone", "graybouncingstone", "yellowbouncingstone", "greenbouncingstone",
                    "explosion_long_blue_sheet", "explosion_long_orange_sheet", "explosion_long_green_sheet",
                    "trophy_nr1", "trophy_nr2", "trophy_nr3")) {
                        cutSpriteSheet(QImage("../res/drawable/" + QString::fromStdString(matches[1]) + ".png"), rows, cols, images);
                    }
                } else {
                    if(!replaceWithDesktopVersion(matches[1])) {
                        images.emplace_back("../res/drawable/" + QString::fromStdString(matches[1]) + ".png");
                    }
                    //std::cout << "OK\n";
                }
            }
        } else if(std::string::const_iterator searchStart(line.cbegin()); std::regex_search(searchStart, line.cend(), matches, pattern3) && matches.size() > 2) {
            do {
                if(matches[1] == "rows") rows = std::atoi(matches[2].str().c_str());
                else if(matches[1] == "cols") cols = std::atoi(matches[2].str().c_str());
                searchStart = matches.suffix().first;
            } while(regex_search(searchStart, line.cend(), matches, pattern3) && matches.size() > 2);
        }
    }
}

bool BitmapLoader::replaceWithDesktopVersion(std::string&& imageName) {
    if(anyOf(imageName,
     "settings_menu_eng_title",
     "settings_menu_eng_language",
     "settings_menu_eng_music",
     "settings_menu_eng_effects",
     "settings_menu_eng_save",
     "settings_menu_hun_title",
     "settings_menu_hun_language",
     "settings_menu_hun_music",
     "settings_menu_hun_effects",
     "settings_menu_hun_save",
     "settings_menu_bul_title",
     "settings_menu_bul_language",
     "settings_menu_bul_music",
     "settings_menu_bul_effects",
     "settings_menu_bul_save",
     "settings_menu_arrows")) {
        images.emplace_back("../Client/bitmaps/" + QString::fromStdString(imageName) + ".png");
        return true;
    }

    return false;
}

void BitmapLoader::loadDesktopBitmaps() {
    images.emplace_back("../Client/bitmaps/" + QString::fromStdString("settings_menu_eng_resolution") + ".png");
    images.emplace_back("../Client/bitmaps/" + QString::fromStdString("settings_menu_hun_resolution") + ".png");
    images.emplace_back("../Client/bitmaps/" + QString::fromStdString("settings_menu_bul_resolution") + ".png");
    images.emplace_back("../Client/bitmaps/" + QString::fromStdString("settings_menu_eng_arrows2") + ".png");
    images.emplace_back("../Client/bitmaps/" + QString::fromStdString("settings_menu_hun_arrows2") + ".png");
    images.emplace_back("../Client/bitmaps/" + QString::fromStdString("settings_menu_bul_arrows2") + ".png");
}
