#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "BlueTriangle.h"
#include "TranslateInterface.h"
#include <vector>
#include "ObserverTouchEvent.h"
#include <functional>

class Slider : public BlueTriangle, public  ObserverTouchEvent, public TouchEventListener
{
public:
    Slider(double x, double bottomY, int minimum, int maximum) noexcept;
    BlueTriangle* copy() const noexcept override;

    // GameObject
    void draw(Screen* screen) noexcept override;

    // from BlueTriangle
    Slider* slider() noexcept override {return this;}

    // TouchEventListener
    bool onTouchEvent(int action, double x, double y) noexcept override;
    // ObserverTouchEvent
    bool notify(ObserverTouchEventSubject* subject) noexcept override;

    int value() const noexcept {return value_;}
    void setValue(int value) noexcept {value_ = value;}

    using ValueChangedCallback = std::function<void(Slider*, int value)>;
    void add(ValueChangedCallback c) noexcept {valueChangedCallbacks_.push_back(c);}

    // ObserverTouchEvent
    uint32_t id() const noexcept override {return GameObject::id();}

private:
    int value_{0};
    int minimum_{0};
    int maximum_{0};
    std::vector<ValueChangedCallback> valueChangedCallbacks_;
    bool grabbed_{};
    double grabbedX_{};
};

#endif // __SLIDER_H__
