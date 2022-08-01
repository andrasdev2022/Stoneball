#ifndef __STATE_H__
#define __STATE_H__

#include "../VisitorSubject.h"
#include <string>

class StateSubject;

class State : public VisitorSubject {
public:
    enum class Type {
        ST_IDLE,
        ST_WALK,
        ST_RUN,
        ST_JUMP,
        ST_FALL,
        ST_MOVE,
        ST_ROLL,
        ST_ATTACK1,
        ST_ATTACK2,
        ST_ATTACK3,
        ST_DEAD,
        ST_HURT,
        ST_BLOCK1,
        ST_FLY,
        ST_SIT,
        COUNT
    };
    virtual ~State() noexcept {}
    virtual bool isReady() const noexcept { return false; }
    virtual void change(StateSubject* subject) = 0;
    virtual State* copy() const = 0;
    virtual std::string stateTypeStr() const = 0;
    virtual State::Type stateType() const = 0;
};

#endif // __STATE_H__
