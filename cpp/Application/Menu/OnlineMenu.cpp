#include "OnlineMenu.h"
#include "BitmapIds.h"
#include "MediaData.h"
#include "Screen.h"
#include "Application.h"
#include "Global.h"
#include "ObserverTouchEventSubject.h"
#include "MenuTouchEvent.h"

OnlineMenu::OnlineMenu()
: ApplicationMenu(MenuType::ONLINE) {

}


bool OnlineMenu::draw(AndroidBitmapInfo* info, void* pixels) noexcept {
    if(!drawBackground(bitmapData(NATIVE_BITMAP_MENU_BACKGROUND, false), info, pixels)) {return false;}

    if(Application::language() == TranslateInterface::Language::ENGLISH) {
        drawMenuItem(NATIVE_BITMAP_ONLINE_MENU_ENG_TITLE, info, pixels, 237_ZRD, 74_ZRD);
        drawMenuItem(NATIVE_BITMAP_ONLINE_MENU_ENG_CREATE_GAME, info, pixels, 141_ZRD, 120_ZRD);
        drawMenuItem(NATIVE_BITMAP_ONLINE_MENU_ENG_FIND_GAME, info, pixels, 158_ZRD, 163_ZRD);
    } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
        drawMenuItem(NATIVE_BITMAP_ONLINE_MENU_HUN_TITLE, info, pixels, 206_ZRD, 76_ZRD);
        drawMenuItem(NATIVE_BITMAP_ONLINE_MENU_HUN_CREATE_GAME, info, pixels, 150_ZRD, 120_ZRD);
        drawMenuItem(NATIVE_BITMAP_ONLINE_MENU_HUN_FIND_GAME, info, pixels, 174_ZRD, 163_ZRD);
    } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
        drawMenuItem(NATIVE_BITMAP_ONLINE_MENU_BUL_TITLE, info, pixels, 237_ZRD, 75_ZRD);
        drawMenuItem(NATIVE_BITMAP_ONLINE_MENU_BUL_CREATE_GAME, info, pixels, 197_ZRD, 121_ZRD);
        drawMenuItem(NATIVE_BITMAP_ONLINE_MENU_BUL_FIND_GAME, info, pixels, 198_ZRD, 165_ZRD);
    }
/*
    Screen screenL(0, 0, screenWidth(), screenHeight());
    screenL.setContentPtr((uint16_t*)pixels);

    auto& font30px = Application::font30px();
    font30px->setColor(0xdd, 0xc6, 0x9f);
    font30px->drawHCentered(&screenL, 77_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__ONLINE));
    font30px->drawHCentered(&screenL, 134_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__ON_ONE_DEVICE));
    font30px->drawHCentered(&screenL, 176_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__CREATE_GAME));
    font30px->drawHCentered(&screenL, 219_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__FIND_GAME));
*/
    return true;
}

bool OnlineMenu::notify(ObserverTouchEventSubject* subject) noexcept {
    if(Application::language() == TranslateInterface::Language::ENGLISH) {
        if(clickTest(subject, 141_ZRD, 426_ZRD, 122_ZRD, 146_ZRD)) {
           Application::setActiveMenu(MenuInterface::MenuType::MULTIPLAYER_CREATE_GAME);
           return true;
        } else if(clickTest(subject, 158_ZRD, 417_ZRD, 163_ZRD, 195_ZRD)) {
           Application::setActiveMenu(MenuInterface::MenuType::MULTIPLAYER_FIND_GAME);
           MenuTouchEvent::sendRoomListRequest();
           return true;
        }
    } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
        if(clickTest(subject, 150_ZRD, 419_ZRD, 120_ZRD, 141_ZRD)) {
           Application::setActiveMenu(MenuInterface::MenuType::MULTIPLAYER_CREATE_GAME);
           return true;
        } else if(clickTest(subject, 174_ZRD, 395_ZRD, 163_ZRD, 184_ZRD)) {
           Application::setActiveMenu(MenuInterface::MenuType::MULTIPLAYER_FIND_GAME);
           MenuTouchEvent::sendRoomListRequest();
           return true;
        }
    } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
        if(clickTest(subject, 197_ZRD, 371_ZRD, 121_ZRD, 146_ZRD)) {
           Application::setActiveMenu(MenuInterface::MenuType::MULTIPLAYER_CREATE_GAME);
           return true;
        } else if(clickTest(subject, 198_ZRD, 371_ZRD, 165_ZRD, 189_ZRD)) {
           Application::setActiveMenu(MenuInterface::MenuType::MULTIPLAYER_FIND_GAME);
           MenuTouchEvent::sendRoomListRequest();
           return true;
        }
    }
    return false;
}
