#include "MultiplayerOneDeviceGameParameters.h"
#include "BitmapIds.h"
#include "MediaData.h"
#include "Screen.h"
#include "Application.h"
#include "Global.h"
#include "AnimatedKnightsForMenu.h"
#include "ObserverTouchEventSubject.h"
#include "Selectionbox.h"
#include "Messagebox1.h"


MultiplayerOneDeviceGameParameters::MultiplayerOneDeviceGameParameters()
: ApplicationMenu(MenuType::MULTIPLAYER_ONE_DEVICE_GAME_PARAMETERS)
, animatedKnightsForMenu_(new AnimatedKnightsForMenu()) {

}

void MultiplayerOneDeviceGameParameters::update(double now) noexcept {
    if(Application::widgets().size() && animatedKnightsForMenu_->selectionboxForMaps()) {
        int selectedMapNr = animatedKnightsForMenu_->selectionboxForMaps()->selectedLineNr();
        if(selectedMapNr < 2) {
            animatedKnightsForMenu()->init(2);
        } else if(selectedMapNr < 4) {
                animatedKnightsForMenu()->init(3);
        } else if(selectedMapNr < 6) {
                animatedKnightsForMenu()->init(4);
        }
    } else {
        animatedKnightsForMenu()->init(2);
    }
    if(!Application::widgets().size()) {
        animatedKnightsForMenu()->initSelectionboxForMaps();
        applicationMenu()->initSelectionboxForTime();
    }
    animatedKnightsForMenu_->update(now);
}

bool MultiplayerOneDeviceGameParameters::draw(AndroidBitmapInfo* info, void* pixels) noexcept {
    if(!drawBackground(bitmapData(NATIVE_BITMAP_MENU_BACKGROUND, false), info, pixels)) {return false;}

    if(Application::language() == TranslateInterface::Language::ENGLISH) {
        drawMenuItem(NATIVE_BITMAP_ONE_DEVICE_GAME_PARAMETERS_MENU_ENG_TITLE, info, pixels, 181_ZRD, 65_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_ENG_FIELD, info, pixels, 141_ZRD, 108_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_ENG_TIME, info, pixels, 141_ZRD, 154_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_ENG_TEAM, info, pixels, 141_ZRD, 200_ZRD);
        drawMenuItem(NATIVE_BITMAP_ONE_DEVICE_GAME_PARAMETERS_MENU_ENG_START, info, pixels, 252_ZRD, 235_ZRD);
    } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
        drawMenuItem(NATIVE_BITMAP_ONE_DEVICE_GAME_PARAMETERS_MENU_HUN_TITLE, info, pixels, 189_ZRD, 65_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_HUN_FIELD, info, pixels, 141_ZRD, 108_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_HUN_TIME, info, pixels, 141_ZRD, 154_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_HUN_TEAM, info, pixels, 141_ZRD, 200_ZRD);
        drawMenuItem(NATIVE_BITMAP_ONE_DEVICE_GAME_PARAMETERS_MENU_HUN_START, info, pixels, 239_ZRD, 235_ZRD);
    } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
        drawMenuItem(NATIVE_BITMAP_ONE_DEVICE_GAME_PARAMETERS_MENU_BUL_TITLE, info, pixels, 141_ZRD, 65_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_BUL_FIELD, info, pixels, 141_ZRD, 108_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_BUL_TIME, info, pixels, 141_ZRD, 154_ZRD);
        drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_BUL_TEAM, info, pixels, 141_ZRD, 200_ZRD);
        drawMenuItem(NATIVE_BITMAP_ONE_DEVICE_GAME_PARAMETERS_MENU_BUL_START, info, pixels, 246_ZRD, 235_ZRD);
    }
    drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_ARROWS1, info, pixels, 221_ZRD, 111_ZRD);
    drawMenuItem(NATIVE_BITMAP_CREATE_GAME_MENU_ARROWS2, info, pixels, 221_ZRD, 157_ZRD);
/*
    Screen screenL(0, 0, screenWidth(), screenHeight());
    screenL.setContentPtr((uint16_t*)pixels);

    auto& font30px = Application::font30px();
    font30px->setColor(0xdd, 0xc6, 0x9f);
    font30px->drawHCentered(&screenL, 71_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__ON_ONE_DEVICE));

    auto& font24px = Application::font24px();
    font24px->setColor(0xdd, 0xc6, 0x9f);
    font24px->draw(&screenL, 141_ZRD, 200_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__YOUR_TEAM) + L":");
    font24px->draw(&screenL, 141_ZRD, 111_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__FIELD) + L":");
    font24px->draw(&screenL, 141_ZRD, 158_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__TIME) + L":");
    font24px->drawHCentered(&screenL, 235_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__START));
*/
    auto standingAvatar = static_cast<Application::Avatar>(animatedKnightsForMenu_->standingAvatar());
    if(standingAvatar != Application::avatar()) {
        Application::setAvatar(standingAvatar);
    }
    if(!offsetsInitialized_) {
        animatedKnightsForMenu_->initOffsets(xOffset_, yOffset_);
        offsetsInitialized_ = true;
    }

    if(offsetsInitialized_ && selectionboxForTime_ && !selectionboxForTimeOffsetUpdated_) {
        selectionboxForTime_->initOffsets(xOffset_, yOffset_);
        selectionboxForTimeOffsetUpdated_ = true;
    }
    animatedKnightsForMenu_->draw(info, pixels);
    return true;
}

bool MultiplayerOneDeviceGameParameters::notify(ObserverTouchEventSubject* subject) noexcept {
    //debug("MultiplayerOneDeviceGameParameters::notify");
    // Let's check the start button first
    bool buttonClicked = Application::language() == TranslateInterface::Language::ENGLISH &&
                         clickTest(subject, 252_ZRD, 315_ZRD, 235_ZRD, 259_ZRD);
    buttonClicked |= Application::language() == TranslateInterface::Language::HUNGARIAN &&
                     clickTest(subject, 239_ZRD, 328_ZRD, 235_ZRD, 259_ZRD);

    buttonClicked |= Application::language() == TranslateInterface::Language::BULGARIAN &&
                     clickTest(subject, 246_ZRD, 322_ZRD, 235_ZRD, 259_ZRD);
    if(buttonClicked) {
        if(Application::avatar() == Application::Avatar::UNDEFINED) {
            // Let's pop up a messagebox
            TranslateInterface::Key message = TranslateInterface::Key::TRA__SELECT_AN_AVATAR;
            auto messagebox = new Messagebox1(1e3, message);
            Application::showMessageBox(messagebox);
            return false;
        }

        int selectedMapNr = animatedKnightsForMenu_->selectionboxForMaps()->selectedLineNr();
        int selectedMinutesNr = selectionboxForTime_->selectedLineNr();

        Application::setActiveMenu(MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING);
        if(selectedMapNr == 0) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_1);
        } else if(selectedMapNr == 1) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_2);
        } else if(selectedMapNr == 2) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_3);
        } else if(selectedMapNr == 3) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_4);
        } else if(selectedMapNr == 4) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_5);
        } else if(selectedMapNr == 5) {
            Application::setGameLevel(LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_6);
        }
        Application::setPlayTime((selectedMinutesNr + 1) * 5);
        Application::destroyWidgets();
        return true;
    }
    return animatedKnightsForMenu_->avatarSelectionCheck(subject);
}
