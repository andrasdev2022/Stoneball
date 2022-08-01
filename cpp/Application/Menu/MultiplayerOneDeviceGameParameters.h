#ifndef __MULTIPLAYERONEDEVICEGAMEPARAMETERS_H__
#define __MULTIPLAYERONEDEVICEGAMEPARAMETERS_H__

#include "ApplicationMenu.h"

class AnimatedKnightsForMenu;
using AnimatedKnightsForMenuUPtr = std::unique_ptr<AnimatedKnightsForMenu>;

class MultiplayerOneDeviceGameParameters : public ApplicationMenu
{
public:
    MultiplayerOneDeviceGameParameters();

    void update(double now) noexcept override;
    bool draw(AndroidBitmapInfo* info, void* pixels) noexcept override;

    AnimatedKnightsForMenu* animatedKnightsForMenu() noexcept override {return animatedKnightsForMenu_.get();}
    MultiplayerOneDeviceGameParameters* multiplayerOneDeviceGameParameters() noexcept override {return this;}

    // ObserverTouchEvent
    bool notify(ObserverTouchEventSubject* subject) noexcept override;

private:
    AnimatedKnightsForMenuUPtr animatedKnightsForMenu_;
    bool offsetsInitialized_{};
};

#endif // __MULTIPLAYERONEDEVICEGAMEPARAMETERS_H__
