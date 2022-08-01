#ifndef __OBSERVERSUBJECTVICTORYEVENT_H__
#define __OBSERVERSUBJECTVICTORYEVENT_H__

#include "ObserverVictoryEvent.h"
#include <set>

class ObserverSubjectVictoryEventVisitor;

class ObserverSubjectVictoryEvent {
public:
    virtual void add(ObserverVictoryEvent* observer) noexcept {
        observers.insert(observer);
    }

    virtual void remove(ObserverVictoryEvent* observer) noexcept {
        observers.erase(observer);
    }

    virtual void notify() noexcept {
        for(auto& observer : observers) {
            observer->notify(this);
        }
    }

    virtual int observerNumber() const noexcept { return observers.size(); }
    virtual void accept(ObserverSubjectVictoryEventVisitor*) noexcept {}

private:
    std::set<ObserverVictoryEvent*> observers;
};

#endif // __OBSERVERSUBJECTVICTORYEVENT_H__
