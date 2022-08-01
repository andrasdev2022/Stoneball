#ifndef __LEVELSMENU_H__
#define __LEVELSMENU_H__

#include "ApplicationMenu.h"
#include <vector>

class RPGBgElement;
using RPGBgElementSPtr = std::shared_ptr<RPGBgElement>;

class LevelsMenu : public ApplicationMenu
{
public:
    LevelsMenu();

    void update(double now) noexcept override;
    bool draw(AndroidBitmapInfo* info, void* pixels) noexcept override;

    // ObserverTouchEvent
    bool notify(ObserverTouchEventSubject* subject) noexcept override;

private:
    virtual void initHoles() noexcept;
    virtual void initHolesOffsets() noexcept;
    virtual void drawArrows(AndroidBitmapInfo* info, void* pixels) noexcept;
    virtual void drawArrow(AndroidBitmapInfo* info, void* pixels, bool left) noexcept;
    virtual bool arrowClick(ObserverTouchEventSubject* subject) noexcept;

    std::vector<RPGBgElementSPtr> holesForLevels_;
    bool offsetsInitialized_{};
    int currentPage_{};
};

#endif // __LEVELSMENU_H__
