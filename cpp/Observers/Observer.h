#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include <iostream>
#include <set>
#include <memory>

class ObserverSubject;
using ObserverSubjectSPtr = std::shared_ptr<ObserverSubject>;

class Observer {
public:
    virtual ~Observer() noexcept = default;
    virtual void notify(ObserverSubjectSPtr subject) = 0;
};

using ObserverSPtr = std::shared_ptr<Observer>;

#endif // __OBSERVER_H__
