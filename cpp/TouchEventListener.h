#ifndef __TOUCHEVENTLISTENER_H__
#define __TOUCHEVENTLISTENER_H__

class TouchEventListener {
public:
    virtual bool onTouchEvent(int action, double x, double y) = 0;
};

#endif // __TOUCHEVENTLISTENER_H__
