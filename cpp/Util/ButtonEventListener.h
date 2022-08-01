#ifndef __BUTTONEVENTLISTENER_H__
#define __BUTTONEVENTLISTENER_H__

#include <string>

class GameObject;

class ButtonEventListener {
public:
    enum class ButtonState {
        BS_UP,
        BS_DOWN
    };

    virtual bool onButtonEvent(GameObject* sender, ButtonState buttonState) = 0;
    static std::string str(ButtonState b) noexcept;
};

#endif // __BUTTONEVENTLISTENER_H__
