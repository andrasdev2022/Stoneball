#ifndef __OBSERVERVICTORYEVENT_H__
#define __OBSERVERVICTORYEVENT_H__


class ObserverSubjectVictoryEvent;

class ObserverVictoryEvent {
public:
    virtual ~ObserverVictoryEvent() noexcept = default;
    virtual void notify(ObserverSubjectVictoryEvent* subject) = 0;
};

#endif // __OBSERVERVICTORYEVENT_H__
