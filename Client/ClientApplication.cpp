#include "MainWindow.h"
#include <QApplication>
#include "Global.h"
#include "Bitmap565.h"
#include "qtutil.h"
#include "mock.h"
#include "BitmapIds.h"
#include "MediaData.h"
#include "GameObject.h"
#include <QDir>
#include "MenuTouchEvent.h"
#include <QMediaPlayer>
#include "TranslatorFromAndroidStrings.h"
#include <QFontDatabase>
#include <QMediaPlaylist>
#include "BitmapLoader.h"

#ifdef WIN32
    #define WIN32_LEAN_AND_MEAN

    #include <winsock2.h>
    #include <Ws2tcpip.h>
    #include <stdio.h>

    // Link with ws2_32.lib
    #pragma comment(lib, "Ws2_32.lib")
#endif

using namespace QtUtil;
using namespace Bitmap565;


namespace ClientApplication {

    std::vector<QImage> images;
    extern const double designScreenWidth = 569.; // 480x270?
    extern const double designScreenHeight = 320.;
    double actualScreenWidth  = 569.;
    double actualScreenHeight = 320.;
    //const double actualScreenWidth  = 1144;
    //const double actualScreenHeight = 816;
    //const double actualScreenWidth  = 1400;
    //const double actualScreenHeight = 500;
    //const double actualScreenWidth  = 854;
    //const double actualScreenHeight = 906;

    // I cannot test with these resolutions because my monitor's resulition is too low
    //const double actualScreenWidth  = 1920;
    //const double actualScreenHeight = 1080;
    //const double actualScreenWidth  = 2300;
    //const double actualScreenHeight = 1500;
    // Nexus 9
    //const double actualScreenWidth = 2048.;
    //const double actualScreenHeight = 1536.;
    // small zoom
    //const double actualScreenWidth  = 1344;
    //const double actualScreenHeight = 1016;
    // Vince
    //const double actualScreenWidth  = 2400.;
    //const double actualScreenHeight = 1080.;

    std::unique_ptr<uint16_t[]> pixels;
    double now;
    AndroidBitmapInfo bitmapInfo{(int)actualScreenWidth, (int)actualScreenHeight, (int)actualScreenWidth * (int)sizeof(uint16_t)};
    std::unique_ptr<TranslatorFromAndroidStrings> translator;

    QImage drawScreen() {
        QImage output(actualScreenWidth, actualScreenHeight, QImage::Format_RGB16);
        for(size_t y = 0; y < actualScreenHeight; ++y) {
            memcpy((void*)output.scanLine(y), &pixels.get()[y * (size_t)actualScreenWidth], (size_t)actualScreenWidth * 2);
        }
        return output;
    }

    struct QObjDeleter {
        void operator()(QObject* obj) {
            obj->deleteLater();
        }
    };

    std::unique_ptr<QMediaPlayer, QObjDeleter> sandPlayer;
    std::unique_ptr<QMediaPlayer, QObjDeleter> goalPlayer;
    std::unique_ptr<QMediaPlayer, QObjDeleter> backgroundPlayer;
    std::unique_ptr<QMediaPlayer, QObjDeleter> popPlayer;
    std::unique_ptr<QMediaPlayer, QObjDeleter> drumPlayer;
    std::unique_ptr<QMediaPlayer, QObjDeleter> gameBallTapPlayer;

    bool playScheduledSoundEffects(const char* name, const char* key, const char* key2, const char* path,
                                   std::unique_ptr<QMediaPlayer, QObjDeleter>& player) {
        bool handled = false;
        if(!strcmp(name, key) && (!player || player->state() == QMediaPlayer::StoppedState)) {
            if(!player) {
                player = std::unique_ptr<QMediaPlayer, QObjDeleter>(new QMediaPlayer());
                QFileInfo file(path);
                //player->setMedia(QUrl::fromLocalFile(file.absoluteFilePath()));
                QMediaPlaylist *playlist = new QMediaPlaylist();
                playlist->addMedia(QUrl::fromLocalFile(file.absoluteFilePath()));
                playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
                player->setPlaylist(playlist);
                player->setVolume(100);
            }
            player->play();
            handled = true;
        } else if(!strcmp(name, key2) && player && player->state() != QMediaPlayer::StoppedState) {
            player->stop();
            handled = true;
        }
        return handled;
    }

    void updateVolume(std::unique_ptr<QMediaPlayer, QObjDeleter>& player, int volume) {
        if(player && player->volume() != volume) {
            player->setVolume(volume);
        }
    }
    void playScheduledSoundEffects() {
        const char* name = getSoundEffect();
        //debug("playScheduledSoundEffects: '%s'", name);
        playScheduledSoundEffects(name, "PLAY_RUN_ON_SAND", "STOP_PLAY_RUN_ON_SAND", "../res/raw/run_sand1.mp3", sandPlayer) ||
        playScheduledSoundEffects(name, "PLAY_GOAL", "STOP_PLAY_GOAL", "../res/raw/goal.mp3", goalPlayer) ||
        playScheduledSoundEffects(name, "PLAY_BACKGROUND", "STOP_PLAY_BACKGROUND", "../res/raw/pixabay.mp3", backgroundPlayer) ||
        playScheduledSoundEffects(name, "PLAY_POP", "STOP_PLAY_POP", "../res/raw/pop1.mp3", popPlayer) ||
        playScheduledSoundEffects(name, "PLAY_DRUMS", "STOP_PLAY_DRUMS", "../res/raw/ominousdrums.mp3", drumPlayer) ||
        playScheduledSoundEffects(name, "PLAY_GAME_BALL_TAP", "STOP_PLAY_GAME_BALL_TAP", "../res/raw/game_ball_tap.wav", gameBallTapPlayer);
        updateVolume(backgroundPlayer, musicVolume());
        updateVolume(sandPlayer, effectVolume());
        updateVolume(goalPlayer, effectVolume());
        updateVolume(popPlayer, effectVolume());
        updateVolume(drumPlayer, effectVolume());
        updateVolume(gameBallTapPlayer, effectVolume());
    }

#ifdef WIN32
    WSADATA wsaData;
    void InitWindowsNetwork() {
        // Initialize Winsock
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != NO_ERROR) {
            //wprintf(L"WSAStartup failed with error %d\n", iResult);
            debug("WSAStartup failed with error %d\n", iResult);
            return;
        }
    }
#endif

    void changeZoomSettings() {
        double zoomCorrection = 1.;
        bitmapInfo.width = (int)actualScreenWidth;
        bitmapInfo.height = (int)actualScreenHeight;
        bitmapInfo.stride = (int)actualScreenWidth * (int)sizeof(uint16_t);
        calcZoomRate(actualScreenWidth * zoomCorrection, actualScreenHeight * zoomCorrection, designScreenWidth, designScreenHeight);
        setScreenSize(actualScreenWidth, actualScreenHeight);
        pixels = std::make_unique<uint16_t[]>(actualScreenWidth * actualScreenHeight);
    }

    int run(int argc, char* argv[]) {
        QApplication app(argc, argv);

        int id = QFontDatabase::addApplicationFont("../assets/fonts/ubuntu_medium.ttf");
        //std::cout << "QFontDatabase::addApplicationFont " << id << std::endl;
        if(id == -1) {
            //for(auto& family : QFontDatabase::applicationFontFamilies(id))
              std::cerr << "ERROR: Font not found: " << std::endl;
        }
        //QFont monospace(family);

        BitmapLoader bitmapLoader;
        bitmapLoader.loadDrawableImages();

        translator = std::make_unique<TranslatorFromAndroidStrings>();
        // planned screen size / actual screen size
        //double zoomCorrection = .87;
        changeZoomSettings();
        bitmapLoader.convertImagesToRGB565();

        playScheduledSoundEffects("PLAY_BACKGROUND", "PLAY_BACKGROUND", "STOP_PLAY_BACKGROUND", "../res/raw/pixabay.mp3", backgroundPlayer);
        backgroundPlayer->playlist()->setPlaybackMode(QMediaPlaylist::Loop);
        //std::cout << "LAST_ONE: " << BitmapId::LAST_ONE << "\n";

#ifdef WIN32
        InitWindowsNetwork();
#endif
        MainWindow mw(actualScreenWidth, actualScreenHeight);
        mw.show();
        return app.exec();
    }

    QImage updateWorld() {
        now = now_ms();
        ::updateWorld(now);
        drawWorld(&bitmapInfo, pixels.get());
        playScheduledSoundEffects();
        return drawScreen();
    }

    void mousePressEvent(int x, int y) {
        now = now_ms();
        onTouchEvent(GameObject::ACTION_DOWN, x, y, now);
    }

    void mouseReleaseEvent(int x, int y){
        now = now_ms();
        onTouchEvent(GameObject::ACTION_UP, x, y, now);
    }

    void mouseMoveEvent(int x, int y){
        now = now_ms();
        onTouchEvent(GameObject::ACTION_MOVE, x, y, now);
    }

}

int main (int argc, char* argv[]) {
    return ClientApplication::run(argc, argv);
}

