#ifndef __MULTIPLAYERFINDGAMEMENU_H__
#define __MULTIPLAYERFINDGAMEMENU_H__

#include "ApplicationMenu.h"

class AnimatedKnightsForMenu;
using AnimatedKnightsForMenuUPtr = std::unique_ptr<AnimatedKnightsForMenu>;
class Listbox1;

class MultiplayerFindGameMenu : public ApplicationMenu
{
public:
    MultiplayerFindGameMenu();

    void update(double now) noexcept override;
    bool draw(AndroidBitmapInfo* info, void* pixels) noexcept override;
    AnimatedKnightsForMenu* animatedKnightsForMenu() noexcept override {return animatedKnightsForMenu_.get();}
    MultiplayerFindGameMenu* multiplayerFindGameMenu() noexcept override {return this;}

    void listboxSelectionCallback(Listbox1* listbox, int selectedLineNr) noexcept;

    // ObserverTouchEvent
    bool notify(ObserverTouchEventSubject* subject) noexcept override;

    virtual Listbox1* initListbox() noexcept;

private:
    AnimatedKnightsForMenuUPtr animatedKnightsForMenu_;
    uint32_t listbox1Id_{};
    bool listboxOffsetUpdated_{};
};

#endif // __MULTIPLAYERFINDGAMEMENU_H__
