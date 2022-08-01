#ifndef __STATESUBJECT_H__
#define __STATESUBJECT_H__

#include <memory>

#include "State.h"
#include "VisitorSubject.h"

class StateSubject : public VisitorSubject {
public:
    virtual ~StateSubject() noexcept {}
    void setState(State* state) noexcept {state_.reset(state);}
    const std::unique_ptr<State>& state() const noexcept {return state_;}

private:
    std::unique_ptr<State> state_;
};

#endif // __STATESUBJECT_H__
