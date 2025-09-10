#include "FireElementalAbility.h"
#include "../Minion.h"


FireElementalAbility::FireElementalAbility(Card* host)
  : TriggeredAbility(EventType::MINION_ENTER, host) {}


// We override notify only to filter out self‐summons if you ever need to;
// otherwise you could rely on the base notify + execute only.
void FireElementalAbility::notify(EventType evt, Event& curEvt, Target *tgt) {
    if (evt != getTriggerType()) {
        return;
    }

    // Only trigger on opponent's minions
    // curEvt.getOwner() is the player whose minion just entered.
    Card* hostCard = getOwner();
    if (curEvt.getOwner() == hostCard->getOwner()) {
        return;
    }
    // Now run execute on the subject (the entering card)
    if (auto* t = dynamic_cast<Target*>(curEvt.getSubject())) {
        execute(t);
    }
}


void FireElementalAbility::execute(Target* target) {
    // Only care about actual minions
    if (auto* m = dynamic_cast<Minion*>(target)) {
        m->takeDamage(1);
    }
}
