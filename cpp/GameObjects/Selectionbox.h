#ifndef __SELECTIONBOX_H__
#define __SELECTIONBOX_H__

#include "BlueTriangle.h"
#include "TranslateInterface.h"
#include <vector>
#include "ObserverTouchEvent.h"
#include <functional>

class Selectionbox : public BlueTriangle, public  ObserverTouchEvent, public TouchEventListener
{
public:
    struct Item {
        std::wstring label;
        std::wstring itemData;
    };
    Selectionbox(double x, double bottomY) noexcept;

    BlueTriangle* copy() const noexcept override;

    // GameObject
    void draw(Screen* screen) noexcept override;

    void addItem(Item item) {items_.push_back(item);}

    // from BlueTriangle
    Selectionbox* selectionbox() noexcept override {return this;}

    // TouchEventListener
    bool onTouchEvent(int action, double x, double y) noexcept override;
    // ObserverTouchEvent
    bool notify(ObserverTouchEventSubject* subject) noexcept override;

    int selectedLineNr() const noexcept {return selectedLineNr_;}
    void setSelectedLineNr(int lineNr) noexcept {selectedLineNr_ = lineNr;}
    Item selectedItem() const noexcept;

    using SelectionCallback = std::function<void(Selectionbox*, int selectedLineNr)>;
    void addSelectionCallback(SelectionCallback c) noexcept {selectionCallbacks_.push_back(c);}

    using DrawBeginCallback = std::function<void(Selectionbox*)>;
    void addDrawBeginCallback(DrawBeginCallback c) noexcept {drawBeginCallbacks_.push_back(c);}

    using DrawEndCallback = std::function<void(Selectionbox*)>;
    void addDrawEndCallback(DrawEndCallback c) noexcept {drawEndCallbacks_.push_back(c);}

    // ObserverTouchEvent
    uint32_t id() const noexcept override {return GameObject::id();}
    ObserverTouchEvent* observerTouchEvent() noexcept override {return this;}

    void initOffsets(int xOffset, int yOffset) noexcept;

private:
    std::vector<Item> items_;
    int selectedLineNr_{0};
    int pushedButton_{-1};
    std::vector<SelectionCallback> selectionCallbacks_;
    std::vector<DrawBeginCallback> drawBeginCallbacks_;
    std::vector<DrawEndCallback> drawEndCallbacks_;
    bool selectionboxOffsetUpdated_{};
};

#endif // __SELECTIONBOX_H__
