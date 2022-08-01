#ifndef __MENUTOUCHEVENT_H__
#define __MENUTOUCHEVENT_H__

#include "ObserverTouchEvent.h"
#include "Rect.h"
#include <memory>
#include <vector>
#include <memory>
#include "IdGenerator.h"

class Knight;
using KnightWPtr = std::weak_ptr<Knight>;

class RPGBgElement;
using RPGBgElementWPtr = std::weak_ptr<RPGBgElement>;

class Selectionbox;
class Listbox1;

class MenuTouchEvent : public ObserverTouchEvent {
public:
    MenuTouchEvent() : id_(IdGenerator::generateId()) {}
    bool notify(ObserverTouchEventSubject* subject) noexcept override;
    static void sendRoomListRequest() noexcept;

    // ObserverTouchEvent
    uint32_t id() const noexcept override {return id_;}

private:
    uint32_t id_;
};

using MenuTouchEventUPtr = std::unique_ptr<MenuTouchEvent>;

#endif // __MENUTOUCHEVENT_H__
