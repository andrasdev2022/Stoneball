#ifndef __MESSAGEBOXYESNO_H__
#define __MESSAGEBOXYESNO_H__

#include "BlueTriangle.h"
#include "ObserverTouchEvent.h"
#include <vector>
#include <functional>

enum BitmapId : int;

class MessageboxYesNo  : public BlueTriangle, public  ObserverTouchEvent, public TouchEventListener
{
public:
    enum class Button {
        YES,
        NO
    };

    enum class Type {
        QUIT_PUZZLE,
        QUIT_MATCH,
        RELOAD_PUZZLE,
        RESTART_MATCH,
        UNDEFINED
    };

    MessageboxYesNo();
    MessageboxYesNo(Type type);
    BlueTriangle* copy() const noexcept override;

    // GameObject
    void update(double now) noexcept override;
    void draw(Screen* screen) noexcept override;
    double z() const noexcept override;

    // TouchEventListener
    bool onTouchEvent(int action, double x, double y) noexcept override;
    // ObserverTouchEvent
    bool notify(ObserverTouchEventSubject* subject) noexcept override;

    // from BlueTriangle
    MessageboxYesNo* messageboxYesNo() noexcept override {return this;}
    const MessageboxYesNo* messageboxYesNo() const noexcept override {return this;}

    using PushButtonCallback = std::function<void(MessageboxYesNo*, MessageboxYesNo::Button value)>;
    void add(PushButtonCallback c) noexcept {pushButtonCallbacks_.push_back(c);}

    // ObserverTouchEvent
    uint32_t id() const noexcept override {return GameObject::id();}
    ObserverTouchEvent* observerTouchEvent() noexcept override {return this;}

private:
    void drawBackground(Screen* screen, BitmapId bitmapId, int x, int y) noexcept;

    std::vector<PushButtonCallback> pushButtonCallbacks_;
    bool pushed_{false};
    Type type_{Type::UNDEFINED};
};

#endif // __MESSAGEBOXYESNO_H__
