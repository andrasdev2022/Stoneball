#include "MainMenu.h"
#include "BitmapIds.h"
#include "MediaData.h"
#include "Screen.h"
#include "Application.h"
#include "Global.h"
#include "ObserverTouchEventSubject.h"
#include "MessageboxTermsOfUse.h"
#include "TouchEventHandler.h"

namespace {
    MessageboxTermsOfUse* termsOfUseMessagebox{};
    void buttonCallback(MessageboxTermsOfUse* msgbox, MessageboxTermsOfUse::Button value) noexcept {
        if(value == MessageboxTermsOfUse::Button::ACCEPT) {
            Application::touchEventHandler()->remove(msgbox);
            msgbox->die();
            Application::privacyPolicyAccepted() = true;
            Application::saveSettings();
        } else if(value == MessageboxTermsOfUse::Button::READ) {
            openPrivacyPolicy();
        }
    }
}

MainMenu::MainMenu()
: ApplicationMenu(MenuType::MAIN) {
}

void MainMenu::update(double now) noexcept {
    ApplicationMenu::update(now);
    if(!Application::privacyPolicyAccepted()) {
        if(!termsOfUseMessagebox) {
            termsOfUseMessagebox = new MessageboxTermsOfUse;
            termsOfUseMessagebox->add(buttonCallback);
            Application::showWidget(termsOfUseMessagebox);
            Application::touchEventHandler()->insertFirst(termsOfUseMessagebox);
        }
    }
}

bool MainMenu::draw(AndroidBitmapInfo* info, void* pixels) noexcept {
    if(!Application::privacyPolicyAccepted() || !drawBackground(bitmapData(NATIVE_BITMAP_MENU_BACKGROUND, false), info, pixels)) {return false;}

    if(Application::language() == TranslateInterface::Language::ENGLISH) {
        drawMenuItem(NATIVE_BITMAP_MAIN_MENU_ENG_LEVELS, info, pixels, 237_ZRD, 82_ZRD);
        drawMenuItem(NATIVE_BITMAP_MAIN_MENU_ENG_MULTIPLAYER, info, pixels, 197_ZRD, 124_ZRD);
        drawMenuItem(NATIVE_BITMAP_MAIN_MENU_ENG_SETTINGS, info, pixels, 222_ZRD, 166_ZRD);
        drawMenuItem(NATIVE_BITMAP_MAIN_MENU_ENG_ABOUT, info, pixels, 245_ZRD, 209_ZRD);
    } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
        drawMenuItem(NATIVE_BITMAP_MAIN_MENU_HUN_LEVELS, info, pixels, 229_ZRD, 82_ZRD);
        drawMenuItem(NATIVE_BITMAP_MAIN_MENU_HUN_MULTIPLAYER, info, pixels, 165_ZRD, 124_ZRD);
        drawMenuItem(NATIVE_BITMAP_MAIN_MENU_HUN_SETTINGS, info, pixels, 197_ZRD, 166_ZRD);
        drawMenuItem(NATIVE_BITMAP_MAIN_MENU_HUN_ABOUT, info, pixels, 205_ZRD, 208_ZRD);
    } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
        drawMenuItem(NATIVE_BITMAP_MAIN_MENU_BUL_LEVELS, info, pixels, 254_ZRD, 82_ZRD);
        drawMenuItem(NATIVE_BITMAP_MAIN_MENU_BUL_MULTIPLAYER, info, pixels, 197_ZRD, 124_ZRD);
        drawMenuItem(NATIVE_BITMAP_MAIN_MENU_BUL_SETTINGS, info, pixels, 214_ZRD, 167_ZRD);
        drawMenuItem(NATIVE_BITMAP_MAIN_MENU_BUL_ABOUT, info, pixels, 213_ZRD, 208_ZRD);
    }

/*
    auto& font30px = Application::font30px();
    font30px->setColor(0xdd, 0xc6, 0x9f);
    Screen screenL(0, 0, screenWidth(), screenHeight());
    screenL.setContentPtr((uint16_t*)pixels);
    font30px->drawHCentered(&screenL, 88_ZRH, (*Application::translator())(TranslateInterface::Key::TRA__LEVELS));
    font30px->drawHCentered(&screenL, 130_ZRH, (*Application::translator())(TranslateInterface::Key::TRA__MULTIPLAYER));
    font30px->drawHCentered(&screenL, 172_ZRH, (*Application::translator())(TranslateInterface::Key::TRA__SETTINGS));
    font30px->drawHCentered(&screenL, 211_ZRH, (*Application::translator())(TranslateInterface::Key::TRA__ABOUT));
*/
    return true;
}

bool MainMenu::notify(ObserverTouchEventSubject* subject) noexcept {
    if(Application::language() == TranslateInterface::Language::ENGLISH) {
        if(clickTest(subject, 237_ZRD, 332_ZRD, 82_ZRD, 113_ZRD)) {
            Application::setPlayerMode(Application::PlayerMode::SINGLEPLAYER);
            Application::setActiveMenu(MenuInterface::MenuType::LEVELS);
            return true;
        } else if(clickTest(subject, 197_ZRD, 372_ZRD, 124_ZRD, 155_ZRD)) {
            Application::setPlayerMode(Application::PlayerMode::MULTIPLAYER);
            Application::setActiveMenu(MenuInterface::MenuType::MULTIPLAYER);
            return true;
        } else if(clickTest(subject, 221_ZRD, 347_ZRD, 166_ZRD, 196_ZRD)) {
            // settings
            Application::setPlayerMode(Application::PlayerMode::UNDEFINED);
            Application::setActiveMenu(MenuInterface::MenuType::SETTINGS);
            return true;
        } else if(clickTest(subject, 245_ZRD, 323_ZRD, 208_ZRD, 238_ZRD)) {
            // about
            Application::setPlayerMode(Application::PlayerMode::UNDEFINED);
            //Application::setActiveMenu(MenuInterface::MenuType::ABOUT);
            extern char callAboutActivity;
            callAboutActivity = 'y';
            return true;
        }
    } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
        if(clickTest(subject, 229_ZRD, 339_ZRD, 82_ZRD, 112_ZRD)) {
            Application::setPlayerMode(Application::PlayerMode::SINGLEPLAYER);
            Application::setActiveMenu(MenuInterface::MenuType::LEVELS);
            return true;
        } else if(clickTest(subject, 165_ZRD, 403_ZRD, 124_ZRD, 154_ZRD)) {
            Application::setPlayerMode(Application::PlayerMode::MULTIPLAYER);
            Application::setActiveMenu(MenuInterface::MenuType::MULTIPLAYER);
            return true;
        } else if(clickTest(subject, 197_ZRD, 371_ZRD, 166_ZRD, 196_ZRD)) {
            // settings
            Application::setPlayerMode(Application::PlayerMode::UNDEFINED);
            Application::setActiveMenu(MenuInterface::MenuType::SETTINGS);
            return true;
        } else if(clickTest(subject, 205_ZRD, 363_ZRD, 208_ZRD, 238_ZRD)) {
            // about
            Application::setPlayerMode(Application::PlayerMode::UNDEFINED);
            //Application::setActiveMenu(MenuInterface::MenuType::ABOUT);
            extern char callAboutActivity;
            callAboutActivity = 'y';
            return true;
        }
    } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
        if(clickTest(subject, 253_ZRD, 315_ZRD, 82_ZRD, 112_ZRD)) {
            Application::setPlayerMode(Application::PlayerMode::SINGLEPLAYER);
            Application::setActiveMenu(MenuInterface::MenuType::LEVELS);
            return true;
        } else if(clickTest(subject, 197_ZRD, 371_ZRD, 124_ZRD, 154_ZRD)) {
            Application::setPlayerMode(Application::PlayerMode::MULTIPLAYER);
            Application::setActiveMenu(MenuInterface::MenuType::MULTIPLAYER);
            return true;
        } else if(clickTest(subject, 213_ZRD, 355_ZRD, 166_ZRD, 196_ZRD)) {
            // settings
            Application::setPlayerMode(Application::PlayerMode::UNDEFINED);
            Application::setActiveMenu(MenuInterface::MenuType::SETTINGS);
            return true;
        } else if(clickTest(subject, 213_ZRD, 355_ZRD, 208_ZRD, 238_ZRD)) {
            // about
            Application::setPlayerMode(Application::PlayerMode::UNDEFINED);
            //Application::setActiveMenu(MenuInterface::MenuType::ABOUT);
            extern char callAboutActivity;
            callAboutActivity = 'y';
            return true;
        }
    }

    return false;
}
