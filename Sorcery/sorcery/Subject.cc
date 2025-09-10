#include "Subject.h"
#include <vector>
#include <algorithm>
#include "EventType.h"
#include "Event.h"
#include "Observer.h"
#include "Target.h"
#include <iostream>


void Subject::attach(Observer *o, EventType evt) {
    observers[evt].push_back(o);
}


void Subject::detach(Observer *o, EventType evt) {
    auto& vec = observers[evt];
    // Remove the observer from the vector for the specified event type
    vec.erase(std::remove(vec.begin(), vec.end(), o), vec.end());
}


void Subject::notifyObservers(EventType evt, Event &ev, Target *tgt){
    auto it = observers.find(evt);
    if (it == observers.end()) return;

    /* snapshot so detach / attach inside notify() can’t invalidate loop */
    const std::vector<Observer*> snapshot = it->second;

    for (Observer *ob : snapshot) {
        if (ob) {
            ob->notify(evt, ev, tgt);
        }
    }
}
