#ifndef __OBSERVERSUBJECTVICTORYEVENTVISITOR_H__
#define __OBSERVERSUBJECTVICTORYEVENTVISITOR_H__

class RollingStone;

class ObserverSubjectVictoryEventVisitor {
public:
    virtual void visit(RollingStone*) noexcept {}
};

#endif // __OBSERVERSUBJECTVICTORYEVENTVISITOR_H__
