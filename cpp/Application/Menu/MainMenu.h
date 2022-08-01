#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "ApplicationMenu.h"

class MainMenu : public ApplicationMenu
{
public:
    MainMenu();

    void update(double now) noexcept override;
    bool draw(AndroidBitmapInfo* info, void* pixels) noexcept override;

    // ObserverTouchEvent
    bool notify(ObserverTouchEventSubject* subject) noexcept override;
};

#endif // __MAINMENU_H__
