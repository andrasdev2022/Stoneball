#ifndef __APPLICATIONMENU_H__
#define __APPLICATIONMENU_H__

#include "MenuInterface.h"

class MediaData;
class Selectionbox;
enum BitmapId : int;

class ApplicationMenu : public MenuInterface, public std::enable_shared_from_this<ApplicationMenu>
{
public:
    ApplicationMenu(MenuType menuType);

    // MenuInterface
    void add(MenuInterfaceSPtr subMenu) noexcept override;
    MenuInterfaceSPtr& parent() noexcept override {return parent_;}
    MenuType menuType() const noexcept override {return menuType_;}
    MenuInterfaceSPtr findMenu(MenuType t) noexcept override;
    std::map<MenuType, MenuInterfaceSPtr>& children() noexcept override {return children_;}
    bool draw(AndroidBitmapInfo*, void*) noexcept override{return false;}
    ApplicationMenu* applicationMenu() noexcept override {return this;}
    int drawXOffset() noexcept override {return xOffset_;}
    int drawYOffset() noexcept override {return yOffset_;}

    // ObserverTouchEvent
    bool notify(ObserverTouchEventSubject* subject) noexcept override;

    virtual bool drawBackground(MediaData* data, AndroidBitmapInfo* info, void* pixels) noexcept;
    virtual void initSelectionboxForTime() noexcept;

    virtual void selectionboxCallbackForTime(Selectionbox* selectionbox, int selectedLineNr)  noexcept;

    // ObserverTouchEvent
    uint32_t id() const noexcept override {return id_;}

protected:
    virtual bool findMenu(MenuType t, MenuInterfaceSPtr& item) noexcept;
    virtual void drawMenuItem(BitmapId id, AndroidBitmapInfo* info, void* pixels, int x, int y) noexcept;
    virtual void drawMenuItem(int id, AndroidBitmapInfo* info, void* pixels, int x, int y) noexcept;
    virtual void drawRareMatrix(BitmapId id, AndroidBitmapInfo* info, void* pixels, int x, int y) noexcept;
    virtual bool clickTest(ObserverTouchEventSubject* subject, int x1, int x2, int y1, int y2) const noexcept;

    MenuType menuType_;
    MenuInterfaceSPtr parent_;
    std::map<MenuType, MenuInterfaceSPtr> children_;
    MenuInterfaceSPtr foundItem_;
    Selectionbox* selectionboxForTime_{};
    int lastSelectedLineNr_{};
    int xOffset_{};
    int yOffset_{};
    bool selectionboxForTimeOffsetUpdated_{};

private:
    uint32_t id_{};
};

#endif // __APPLICATIONMENU_H__
