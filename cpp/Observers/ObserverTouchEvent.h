#ifndef __OBSERVERTOUCHEVENT_H__
#define __OBSERVERTOUCHEVENT_H__

#include <inttypes.h>

class ObserverTouchEventSubject;

class ObserverTouchEvent {
public:
    virtual ~ObserverTouchEvent() noexcept = default;
    virtual bool notify(ObserverTouchEventSubject* subject) = 0;
    virtual uint32_t id() const noexcept = 0;
};

#endif // __OBSERVERTOUCHEVENT_H__
