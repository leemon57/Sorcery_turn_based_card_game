#ifndef TRIGGERED_ABILITY_H
#define TRIGGERED_ABILITY_H

#include "Ability.h"
#include "Observer.h"
#include "Target.h"
#include "EventType.h"
#include "Event.h"
#include "Card.h"
#include <memory>
#include <string>

class Target;  // Forward declaration of Target class
class Card;    // Forward declaration of Card class

class TriggeredAbility : public Ability, public Observer {
    EventType triggerType;
    Card *Owner;

  public:
    TriggeredAbility(EventType triggerType, Card *Owner);
    ~TriggeredAbility() override = default;
    // detach this ability from the subject's observer list when it gets destroyed
    void detachFromSub(Subject& sub) override;
    void attachTo(Subject &sub) override;
    
    // we can now overide this method in concrete triggered ability class that 
    // specifies what each ability does
    virtual void execute(Target* target) = 0; 
    void notify(EventType evt, Event &ev, Target *tgt = nullptr);

    EventType getTriggerType() const;
    Card* getOwner() const;
};


#endif
