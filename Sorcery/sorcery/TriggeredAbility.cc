#include "Observer.h"
#include "TriggeredAbility.h"
#include "Subject.h"
#include "Target.h"
#include "EventType.h"
#include "Event.h"
#include <vector>
#include <iostream>


TriggeredAbility::TriggeredAbility(EventType triggerType, Card *Owner): 
    triggerType{triggerType}, 
    Owner{Owner} {}


void TriggeredAbility::notify(EventType evt, Event &ev, Target *tgt) {
    if (evt != triggerType) return; // ignore other events
    if (!tgt) tgt = dynamic_cast<Target*>(ev.getSubject());
    if (!tgt) {
        std::cerr << "[Trigger] subject is not a Target\n";
        return;
    }
    execute(tgt); // or execute(tgt, ev) if you
}

EventType TriggeredAbility::getTriggerType() const {
    return triggerType;
}


Card* TriggeredAbility::getOwner() const {
    return Owner;
}


void TriggeredAbility::detachFromSub(Subject& sub) {
    sub.detach(this, triggerType);
}

void TriggeredAbility::attachTo(Subject& sub) {
    sub.attach(const_cast<TriggeredAbility*>(this), triggerType);
}
