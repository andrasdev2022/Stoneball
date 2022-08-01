#ifndef __MULTIPLAYERMENU_H__
#define __MULTIPLAYERMENU_H__

#include "ApplicationMenu.h"

class MultiplayerMenu : public ApplicationMenu
{
public:
    MultiplayerMenu();

    bool draw(AndroidBitmapInfo* info, void* pixels) noexcept override;

    // ObserverTouchEvent
    bool notify(ObserverTouchEventSubject* subject) noexcept override;

};

#endif // __MULTIPLAYERMENU_H__
