#ifndef OBSERVER_H
#define OBSERVER_H
#include <string>
#include "EventType.h"
#include "Event.h"
#include "Target.h"

class Event;
class Subject;

class Observer {
  public:
    virtual void notify(EventType evt, Event& curEvt, Target *tgt) = 0;
    virtual void detachFromSub(Subject& sub) = 0;
    virtual void attachTo(Subject& sub) = 0;
    virtual ~Observer() = default;
};

#endif
