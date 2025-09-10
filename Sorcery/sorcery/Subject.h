#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "EventType.h"
#include "Event.h"
#include "Observer.h"
#include "Target.h"

class Observer;

class Subject {
    std::map<EventType, std::vector<Observer*>> observers;
  public:
    void attach(Observer *o, EventType evt);  
    void detach(Observer *o, EventType evt);
    void notifyObservers(EventType type, Event &e, Target *tgt = nullptr);
};

#endif
