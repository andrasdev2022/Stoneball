#ifndef __MULTIPLAYERCREATEGAMEMENU_H__
#define __MULTIPLAYERCREATEGAMEMENU_H__

#include "ApplicationMenu.h"

class AnimatedKnightsForMenu;
using AnimatedKnightsForMenuUPtr = std::unique_ptr<AnimatedKnightsForMenu>;
class Selectionbox;

class MultiplayerCreateGameMenu : public ApplicationMenu
{
public:
    MultiplayerCreateGameMenu();

    void update(double now) noexcept override;
    bool draw(AndroidBitmapInfo* info, void* pixels) noexcept override;

    MultiplayerCreateGameMenu* multiplayerCreateGameMenu() noexcept override {return this;}
    AnimatedKnightsForMenu* animatedKnightsForMenu() noexcept override {return animatedKnightsForMenu_.get();}

    // ObserverTouchEvent
    bool notify(ObserverTouchEventSubject* subject) noexcept override;

private:
    AnimatedKnightsForMenuUPtr animatedKnightsForMenu_;

    bool offsetsInitialized_{};
};

#endif // __MULTIPLAYERCREATEGAMEMENU_H__
