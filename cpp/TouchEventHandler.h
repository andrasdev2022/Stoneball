#ifndef __TOUCHEVENTHANDLER_H__
#define __TOUCHEVENTHANDLER_H__

#include "TouchEventListener.h"
#include "ObserverTouchEventSubject.h"

class TouchEventHandler : public TouchEventListener, public ObserverTouchEventSubject {
public:
    bool onTouchEvent(int action, double x, double y) noexcept override;
    int action() const noexcept override {return action_;}
    double x() const noexcept override {return x_;}
    double y() const noexcept override {return y_;}

private:
    int action_;
    double x_;
    double y_;
};

#endif // __TOUCHEVENTHANDLER_H__
