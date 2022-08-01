#ifndef __LISTBOX1_H__
#define __LISTBOX1_H__

#include "BlueTriangle.h"
#include "TranslateInterface.h"
#include <vector>
#include "ObserverTouchEvent.h"
#include <functional>

class Listbox1 : public BlueTriangle, public  ObserverTouchEvent, public TouchEventListener
{
public:
    struct Item {
        std::wstring label;
        std::wstring itemData;
    };
    Listbox1(double x, double bottomY, int maxHeight) noexcept;
    BlueTriangle* copy() const noexcept override;

    // GameObject
    void draw(Screen* screen) noexcept override;
    void update([[maybe_unused]]double now) noexcept override {}

    void addItem(Item item) {items_.push_back(item);}

    // from BlueTriangle
    Listbox1* listbox1() noexcept override {return this;}

    // TouchEventListener
    bool onTouchEvent(int action, double x, double y) noexcept override;
    // ObserverTouchEvent
    bool notify(ObserverTouchEventSubject* subject) noexcept override;

    int selectedLineNr() const noexcept {return selectedLineNr_;}
    Item selectedItem() const noexcept;

    using SelectionCallback = std::function<void(Listbox1*, int selectedLineNr)>;
    void add(SelectionCallback c) noexcept {selectionCallbacks_.push_back(c);}

    // ObserverTouchEvent
    uint32_t id() const noexcept override {return GameObject::id();}
    ObserverTouchEvent* observerTouchEvent() noexcept override {return this;}

private:
    void toggleSelection(double x, double y) noexcept;
    void calcSizes() noexcept;

    int maxHeight_;
    std::vector<Item> items_;
    bool pushed_{};
    bool scrolled_{};
    int totalHeight_{};
    int visibleLineCount_{};
    double scrollY_{};
    double grabY_;
    int lineHeight_{};
    int selectedLineNr_{-1};
    std::vector<SelectionCallback> selectionCallbacks_;
};

#endif // __LISTBOX1_H__
