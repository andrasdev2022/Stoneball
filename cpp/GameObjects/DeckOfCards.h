#ifndef __DECKOFCARDS_H__
#define __DECKOFCARDS_H__

#include "BlueTriangle.h"
#include <vector>
#include <functional>

class DeckOfCards : public BlueTriangle, public TouchEventListener
{
public:
    struct Item {
        std::wstring label;
        std::wstring itemData;
    };
    DeckOfCards(double x, double bottomY) noexcept;
    BlueTriangle* copy() const noexcept override;

    // GameObject
    void draw(Screen* screen) noexcept override;
    void update(double) noexcept override{};

    void addItem(Item item) {items_.push_back(item);}

    // from BlueTriangle
    DeckOfCards* deckOfCards() noexcept override {return this;}

    // TouchEventListener
    bool onTouchEvent(int action, double x, double y) noexcept override;

    bool isTouchValid(int action, double x, double y) noexcept;

    bool onClicked() noexcept;
    int selectedLineNr() const noexcept {return selectedLineNr_;}
    Item selectedItem() const noexcept;

    using SelectionCallback = std::function<void(DeckOfCards*, int selectedLineNr)>;
    void add(SelectionCallback c) noexcept {selectionCallbacks_.push_back(c);}

private:

    std::vector<Item> items_;
    int selectedLineNr_{-1};
    std::vector<SelectionCallback> selectionCallbacks_;
};

#endif // __DECKOFCARDS_H__
