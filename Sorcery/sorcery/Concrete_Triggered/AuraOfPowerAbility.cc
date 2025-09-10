#include "AuraOfPowerAbility.h"
#include "../Minion.h"


AuraOfPowerAbility::AuraOfPowerAbility(Card* host)
  : TriggeredAbility(EventType::MINION_ENTER, host) {}


void AuraOfPowerAbility::notify(EventType evt, Event& curEvt, Target *tgt) {
    if (evt != getTriggerType()) {
        return;
    }

    // Only care about your own minions
    Card* hostCard = getOwner();
    Player* you = hostCard->getOwner();
    // curEvt.getOwner() is the controller of the entering minion
    if (curEvt.getOwner() != you) {
        return;
    }

    if (auto* t = dynamic_cast<Target*>(curEvt.getSubject())) {
        execute(t);
    }
}

void AuraOfPowerAbility::execute(Target* target){
    auto *ritual = dynamic_cast<Ritual*>(getOwner());
    if (!ritual) {
        return;
    }

    auto *minion = dynamic_cast<Minion*>(target);
    if (!minion) {
        return;                 // not a minion => ignore
    }
    if (minion->getOwner() != ritual->getOwner()) {
        return; // buff only friendly minions
    }

    if (ritual->getCharges() < ritual->getActivationCost()) {
        return; // insufficient charges
    }
    ritual->spendCharges(ritual->getActivationCost());
    minion->gainAttack(1);
    minion->gainHp(1);

    if (ritual->getCharges() == 0) {
        if (auto *ta = ritual->getTriggeredAbility()) {
            ta->detachFromSub(*ritual->getOwner());          // disengage observer
        }    
        ritual->getOwner()->getBoard()->clearRitual();      // clear slot
    }
}
