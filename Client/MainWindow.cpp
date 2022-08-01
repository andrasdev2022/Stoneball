#include "MainWindow.h"
#include <QGridLayout>
#include <QPushButton>
#include <QScreen>
#include <QApplication>
#include <iostream>
#include <QTextEdit>
#include <sstream>
#include <QPainter>
#include <QMenu>
#include <QMenuBar>
#include <QTimer>
#include <QSettings>
#include <cmath>
#include "ClientCanvas.h"
#include "ClientApplication.h"
#include "Global.h"
#include "ProfileWindow.h"
#include "Application.h"
#include "ClientFont.h"
#include "AboutWindow.h"
#include "BuildNumber.h"
#include "BitmapLoader.h"
#include "MediaData.h"
#include "CommonUtil.h"

extern char callProfileActivity;
extern char callAboutActivity;
extern bool hasSettingsBeenUpdated();

namespace ClientApplication {
    extern const double designScreenWidth;
    extern const double designScreenHeight;
    extern double actualScreenWidth;
    extern double actualScreenHeight;
    extern void changeZoomSettings();
}

MainWindow::MainWindow(double w, double h) {
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    setStyleSheet("QMainWindow {background: 'black';}");
    setWindowTitle("Stoneball v0.0." + QString::number(BuildNumber::n()));
    //setGeometry(2330, 350, (int)w + 30, (int)h + 30);
    setGeometry(330, 350, (int)w + 18, (int)h + 17);

    canvas_ = new ClientCanvas(this);
    QGridLayout* grid = new QGridLayout;
    grid->addWidget(canvas_, 0, 0);

    centralWidget_ = new QWidget;
    setCentralWidget(centralWidget_);
    centralWidget_->setLayout(grid);
    profileWindow_ = new ProfileWindow(w, h);
    aboutWindow_ = new AboutWindow(w, h);
    QTimer::singleShot(30, this, &MainWindow::tick);
    loadSettings();
    profileWindow_->setProfileName(Application::profileName());
    if(Application::resolution()) changeWindowSize();
 }

MainWindow::~MainWindow() {
    int widgetCount = 0;
    if(centralWidget() != profileWindow_) {
        profileWindow_->deleteLater();
        ++widgetCount;
    }
    if(centralWidget() != aboutWindow_) {
        aboutWindow_->deleteLater();
        ++widgetCount;
    }

    if(widgetCount != 2){
        centralWidget_->deleteLater();
    }
}

void MainWindow::changeWindowSize() noexcept {
    using namespace ClientApplication;
    if(actualScreenWidth == designScreenWidth * (Application::resolution() + 2) * 0.5) return;

    actualScreenWidth = designScreenWidth * (Application::resolution() + 2) * 0.5;
    actualScreenHeight = designScreenHeight * (Application::resolution() + 2) * 0.5;

    BitmapLoader bitmapLoader;
    bitmapLoader.loadDrawableImages();
    changeZoomSettings();
    bitmapLoader.convertImagesToRGB565();
    resetFlippedImages();
    setGeometry(330, 350, actualScreenWidth + 18, actualScreenHeight + 17);
    Application::resetMenu();

    profileWindow_ = new ProfileWindow(actualScreenWidth, actualScreenHeight);
    aboutWindow_ = new AboutWindow(actualScreenWidth, actualScreenHeight);
    profileWindow_->setProfileName(Application::profileName());
    setMinimumHeight(actualScreenHeight + 17);
    setMaximumHeight(actualScreenHeight + 17);
    setMinimumWidth(actualScreenWidth + 18);
    setMaximumWidth(actualScreenWidth + 18);
}

void MainWindow::tick() {
    if(callProfileActivity == 'y') {
        if(centralWidget() != profileWindow_) {
            profileWindow_->setProfileName(Application::profileName());
            profileWindow_->updateLabels();
            takeCentralWidget();
            setCentralWidget(profileWindow_);
        }
    } else if(callAboutActivity == 'y') {
        if(centralWidget() != aboutWindow_) {
            aboutWindow_->updateLabels();
            takeCentralWidget();
            setCentralWidget(aboutWindow_);
        }
    } else if(hasSettingsBeenUpdated()) {
        Application::setProfileName(profileWindow_->profileName());
        saveSettings();
        changeWindowSize();
    } else {
        //canvas_->updateImage(drawClientTexts(ClientApplication::updateWorld()));
        canvas_->updateImage(ClientApplication::updateWorld());
        update_now();
        //QTimer::singleShot(40 + (rand() % 60), this, &MainWindow::tick);
        //QTimer::singleShot(66, this, &MainWindow::tick);
    }
    QTimer::singleShot(16, this, &MainWindow::tick);
}

void MainWindow::saveSettings() {
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("Client");

    settings.setValue("profile", QString::fromStdWString(profileWindow_->profileName()));
    settings.setValue("language", languageValue());
    for(int i = 0; i < 30; ++i) {
        settings.setValue(QString::asprintf("level%02d", i), levelStatuses()[i]);
    }
    settings.setValue("musicVolume", musicVolume());
    settings.setValue("effectVolume", effectVolume());
    settings.setValue("resolution", Application::resolution());
    settings.setValue("privacyPolicyAccepted", Application::privacyPolicyAccepted());

    settings.endGroup();
    settings.sync();
}

void MainWindow::loadSettings() {
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("Client");

    int statuses[30]{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    std::wstring profileName;
    int language{};
    int musicVolume{};
    int effectVolume{};

    for(QString key : settings.allKeys()) {
        if(key == "profile") {
            profileName = settings.value(key).toString().toStdWString();
        } else if(key == "language") {
            language = settings.value(key).toInt();
        } else if(key.startsWith("level")) {
            int pos = key.right(key.length() - 5).toInt();
            statuses[pos] = settings.value(key).toInt();
        } else if(key.startsWith("musicVolume")) {
            musicVolume = settings.value(key).toInt();
        } else if(key.startsWith("effectVolume")) {
            effectVolume = settings.value(key).toInt();
        } else if(key == "resolution") {
            Application::setResolution(settings.value(key).toInt());
        } else if(key == "privacyPolicyAccepted") {
            Application::privacyPolicyAccepted() = settings.value(key).toBool();
        }
    }
    syncSettingsValues(language, musicVolume, effectVolume, statuses, profileName.data() - 1, profileName.length() + 1,
                        Application::privacyPolicyAccepted());

    /*
            QString value = settings.value(key).toString();
            //std::cout << key.toStdString() << ": " << sample.toStdString() << "\n";
            if(value.length() > 0) {
                QStringList vals = value.split(",");
                int threshold = vals[1].trimmed().toInt();
                QString sample = vals[0].trimmed();
                detectedObjectSettings.push_back(std::pair<QString,int>{sample, threshold});
            }
    */

    settings.endGroup();
}

void MainWindow::resetCentralWidget() {
    callProfileActivity = 'n';
    callAboutActivity = 'n';
    takeCentralWidget();
    setCentralWidget(centralWidget_);
}
/*
QImage MainWindow::drawClientTexts(QImage&& screen) noexcept {
    Application::font()->clientFont()->flush(screen);
    Application::font8px()->clientFont()->flush(screen);
    Application::font12px()->clientFont()->flush(screen);
    Application::font24px()->clientFont()->flush(screen);
    Application::font30px()->clientFont()->flush(screen);
    return screen;
}
*/
