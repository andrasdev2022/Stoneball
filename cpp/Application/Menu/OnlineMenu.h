#ifndef __ONLINEMENU_H__
#define __ONLINEMENU_H__

#include "ApplicationMenu.h"

class OnlineMenu : public ApplicationMenu
{
public:
    OnlineMenu();

    bool draw(AndroidBitmapInfo* info, void* pixels) noexcept override;

    // ObserverTouchEvent
    bool notify(ObserverTouchEventSubject* subject) noexcept override;

};

#endif // __ONLINEMENU_H__
