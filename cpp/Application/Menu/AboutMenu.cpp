#include "AboutMenu.h"
#include "BitmapIds.h"
#include "MediaData.h"
#include "Application.h"
#include "Screen.h"
#include "Global.h"

AboutMenu::AboutMenu()
: ApplicationMenu(MenuType::ABOUT) {

}


bool AboutMenu::draw(AndroidBitmapInfo* info, void* pixels) noexcept {
    if(!drawBackground(bitmapData(NATIVE_BITMAP_MENU_BACKGROUND, false), info, pixels)) {return false;}

    if(Application::language() == TranslateInterface::Language::ENGLISH) {
        drawMenuItem(NATIVE_BITMAP_ABOUT_MENU_ENG_TITLE, info, pixels, 245_ZRD, 72_ZRD);
    } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
        drawMenuItem(NATIVE_BITMAP_ABOUT_MENU_HUN_TITLE, info, pixels, 205_ZRD, 73_ZRD);
    } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
        drawMenuItem(NATIVE_BITMAP_ABOUT_MENU_BUL_TITLE, info, pixels, 214_ZRD, 73_ZRD);
    }
/*
    Screen screenL(0, 0, screenWidth(), screenHeight());
    screenL.setContentPtr((uint16_t*)pixels);

    auto& font30px = Application::font30px();
    font30px->setColor(0xdd, 0xc6, 0x9f);
    font30px->drawHCentered(&screenL, 78_ZRD, (*Application::translator())(TranslateInterface::Key::TRA__ABOUT));
*/
    return true;
}

bool AboutMenu::notify([[maybe_unused]]ObserverTouchEventSubject* subject) noexcept {
    return false;
}
