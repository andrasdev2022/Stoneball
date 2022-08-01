#ifndef __VISITORSUBJECT_H__
#define __VISITORSUBJECT_H__

class Visitor;

class VisitorSubject {
public:
    virtual ~VisitorSubject() {}
    virtual void accept(Visitor*) {};
};

#endif // __VISITORSUBJECT_H__
