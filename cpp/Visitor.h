#ifndef __VISITOR_H__
#define __VISITOR_H__

class StateKnightIdle;
class StateKnightRun;
class StateKnightSit;
class GameObject;
class Knight;

class Visitor {
public:
    virtual ~Visitor() {}
    virtual void visit(StateKnightIdle*){}
    virtual void visit(StateKnightRun*){}
    virtual void visit(StateKnightSit*){}
    virtual void visit(GameObject*){}
};

#endif // __VISITOR_H__
