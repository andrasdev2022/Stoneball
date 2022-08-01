#ifndef __MESSAGEBOXTERMSOFUSE_H__
#define __MESSAGEBOXTERMSOFUSE_H__

#include "BlueTriangle.h"
#include "ObserverTouchEvent.h"
#include <vector>
#include <functional>

enum BitmapId : int;

class MessageboxTermsOfUse  : public BlueTriangle, public  ObserverTouchEvent, public TouchEventListener
{
public:
    enum class Button {
        READ,
        ACCEPT,
        UNDEFINED
    };
    MessageboxTermsOfUse();
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
    MessageboxTermsOfUse* messageboxTermsOfUse() noexcept override {return this;}
    const MessageboxTermsOfUse* messageboxTermsOfUse() const noexcept override {return this;}

    using PushButtonCallback = std::function<void(MessageboxTermsOfUse*, MessageboxTermsOfUse::Button value)>;
    void add(PushButtonCallback c) noexcept {pushButtonCallbacks_.push_back(c);}

    // ObserverTouchEvent
    uint32_t id() const noexcept override {return GameObject::id();}
    ObserverTouchEvent* observerTouchEvent() noexcept override {return this;}

private:
    void drawBackground(Screen* screen, int bitmapId, int x, int y) noexcept;

    std::vector<PushButtonCallback> pushButtonCallbacks_;
    bool pushed_{false};
};

#endif // __MESSAGEBOXTERMSOFUSE_H__
