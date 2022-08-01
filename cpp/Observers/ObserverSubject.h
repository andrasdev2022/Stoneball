#ifndef __OBSERVERSUBJECT_H__
#define __OBSERVERSUBJECT_H__

#include "Observer.h"

class ObserverSubject : public std::enable_shared_from_this<ObserverSubject> {
public:
    virtual void add(const ObserverSPtr& observer) noexcept {
	    observers.insert(observer);
    }

    virtual void remove(const ObserverSPtr& observer) noexcept {
	    observers.erase(observer);
    }

    virtual void notify() noexcept {
        for(auto& observer : observers) {
            observer->notify(shared_from_this());
        }
    }

    virtual int observerNumber() const noexcept { return observers.size(); }

private:
    std::set<ObserverSPtr> observers;
};
#endif // __OBSERVERSUBJECT_H__
