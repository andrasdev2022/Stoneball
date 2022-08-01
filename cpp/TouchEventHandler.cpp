#include "TouchEventHandler.h"

bool TouchEventHandler::onTouchEvent(int action, double x, double y) noexcept {
    action_ = action;
    x_ = x;
    y_ = y;
    notify();
    return true;
}
