#ifndef __ABOUTMENU_H__
#define __ABOUTMENU_H__

#include "ApplicationMenu.h"

class AboutMenu : public ApplicationMenu
{
public:
    AboutMenu();

    bool draw(AndroidBitmapInfo* info, void* pixels) noexcept override;

    // ObserverTouchEvent
    bool notify(ObserverTouchEventSubject* subject) noexcept override;

};

#endif // __ABOUTMENU_H__
