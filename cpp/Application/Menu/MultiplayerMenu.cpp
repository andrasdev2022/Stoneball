#include "MultiplayerMenu.h"
#include "BitmapIds.h"
#include "MediaData.h"
#include "Screen.h"
#include "Application.h"
#include "Global.h"
#include "ObserverTouchEventSubject.h"
#include "MenuTouchEvent.h"

MultiplayerMenu::MultiplayerMenu()
: ApplicationMenu(MenuType::MULTIPLAYER) {

}


bool MultiplayerMenu::draw(AndroidBitmapInfo* info, void* pixels) noexcept {
    if(!drawBackground(bitmapData(NATIVE_BITMAP_MENU_BACKGROUND, false), info, pixels)) {return false;}

    if(Application::language() == TranslateInterface::Language::ENGLISH) {
        drawMenuItem(NATIVE_BITMAP_MULTIPLAYER_MENU_ENG_TITLE, info, pixels, 197_ZRD, 70_ZRD);
        drawMenuItem(NATIVE_BITMAP_MULTIPLAYER_MENU_ENG_ON_ONE_DEVICE, info, pixels, 181_ZRD, 128_ZRD);
        drawMenuItem(NATIVE_BITMAP_MULTIPLAYER_MENU_ENG_ONLINE, info, pixels, 237_ZRD, 174_ZRD);
    } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
        drawMenuItem(NATIVE_BITMAP_MULTIPLAYER_MENU_HUN_TITLE, info, pixels, 165_ZRD, 70_ZRD);
        drawMenuItem(NATIVE_BITMAP_MULTIPLAYER_MENU_HUN_ON_ONE_DEVICE, info, pixels, 189_ZRD, 128_ZRD);
        drawMenuItem(NATIVE_BITMAP_MULTIPLAYER_MENU_HUN_ONLINE, info, pixels, 206_ZRD, 176_ZRD);
    } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
        drawMenuItem(NATIVE_BITMAP_MULTIPLAYER_MENU_BUL_TITLE, info, pixels, 197_ZRD, 70_ZRD);
        drawMenuItem(NATIVE_BITMAP_MULTIPLAYER_MENU_BUL_ON_ONE_DEVICE, info, pixels, 141_ZRD, 128_ZRD);
        drawMenuItem(NATIVE_BITMAP_MULTIPLAYER_MENU_BUL_ONLINE, info, pixels, 237_ZRD, 175_ZRD);
    }
/*
    Screen screenL(0, 0, screenWidth(), screenHeight());
    screenL.setContentPtr((uint16_t*)pixels);

    auto& font30px = Application::font30px();
    font30px->setColor(0xdd, 0xc6, 0x9f);
    font30px->drawHCentered(&screenL, 77_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__MULTIPLAYER));
    font30px->drawHCentered(&screenL, 134_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__ON_ONE_DEVICE));
    font30px->drawHCentered(&screenL, 176_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__CREATE_GAME));
    font30px->drawHCentered(&screenL, 219_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__FIND_GAME));
*/
    return true;
}

bool MultiplayerMenu::notify(ObserverTouchEventSubject* subject) noexcept {
    if(Application::language() == TranslateInterface::Language::ENGLISH) {
        if(clickTest(subject, 181_ZRD, 388_ZRD, 128_ZRD, 158_ZRD)) {
           Application::setActiveMenu(MenuInterface::MenuType::MULTIPLAYER_ONE_DEVICE_GAME_PARAMETERS);
           return true;
        } else if(clickTest(subject, 237_ZRD, 331_ZRD, 174_ZRD, 195_ZRD)) {
           Application::setActiveMenu(MenuInterface::MenuType::ONLINE);
           return true;
        }
    } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
        if(clickTest(subject, 189_ZRD, 379_ZRD, 128_ZRD, 158_ZRD)) {
            Application::setActiveMenu(MenuInterface::MenuType::MULTIPLAYER_ONE_DEVICE_GAME_PARAMETERS);
           return true;
        } else if(clickTest(subject, 206_ZRD, 363_ZRD, 176_ZRD, 195_ZRD)) {
           Application::setActiveMenu(MenuInterface::MenuType::ONLINE);
           return true;
        }
    } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
        if(clickTest(subject, 141_ZRD, 427_ZRD, 128_ZRD, 158_ZRD)) {
            Application::setActiveMenu(MenuInterface::MenuType::MULTIPLAYER_ONE_DEVICE_GAME_PARAMETERS);
           return true;
        } else if(clickTest(subject, 237_ZRD, 331_ZRD, 175_ZRD, 195_ZRD)) {
           Application::setActiveMenu(MenuInterface::MenuType::ONLINE);
           return true;
        }
    }
    return false;
}
