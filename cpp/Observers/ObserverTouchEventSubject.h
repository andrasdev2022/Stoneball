#ifndef __OBSERVERTOUCHEVENTSUBJECT_H__
#define __OBSERVERTOUCHEVENTSUBJECT_H__

#include "ObserverTouchEvent.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "CommonUtil.h"

#if !defined(ANDROID_NDK) && __GNUC__ >= 9
#include <execution>
#endif

class ObserverTouchEventSubject {
public:
    virtual void add(ObserverTouchEvent* observer) noexcept {
        observers_.push_back(observer);
    }
    virtual void insertFirst(ObserverTouchEvent* observer) noexcept {
        observers_.insert(observers_.begin(), observer);
    }

    virtual void remove(ObserverTouchEvent* observer) noexcept {
        auto cond = [observer](const ObserverTouchEvent* o) {return o->id() == observer->id();};
#if !defined(ANDROID_NDK) && __GNUC__ >= 9
        //auto it = std::remove(std::execution::par, observers_.begin(), observers_.end(), observer);
        auto it = std::remove_if(std::execution::par, observers_.begin(), observers_.end(), cond);
#else
        auto it = std::remove_if(observers_.begin(), observers_.end(), cond);
#endif
        //auto r = std::distance(it, observers_.end());
        observers_.erase(it, observers_.end());
    }

    virtual void notify() noexcept {
        for(auto& observer : observers_) {
            if(observer->notify(this)) {
                return;
            }
        }
    }

    virtual void clear() noexcept {
        observers_.clear();
    }

    virtual int observerNumber() const noexcept { return observers_.size(); }
    virtual int action() const = 0;
    virtual double x() const = 0;
    virtual double y() const = 0;
private:
    std::vector<ObserverTouchEvent*> observers_;
};

#endif // __OBSERVERTOUCHEVENTSUBJECT_H__
