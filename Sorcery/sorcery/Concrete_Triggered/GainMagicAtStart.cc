#include "../TriggeredAbility.h"
#include "../Ritual.h"
#include "../Card.h"
#include "../Player.h"
#include "GainMagicAtStart.h"


GainMagicAtStart::GainMagicAtStart(Card* owner, int amount)
    : TriggeredAbility(EventType::START_TURN, owner), amount{amount} {}


void GainMagicAtStart::execute(Target* target) {
    auto *ritual = dynamic_cast<Ritual*>(getOwner());
    if (!ritual) {
        return;
    }

    if (ritual->getCharges() < ritual->getActivationCost()) {
        return;
    }
    ritual->spendCharges(ritual->getActivationCost());
    ritual->getOwner()->gainMagic(amount);
    if (ritual->getCharges() == 0) {
        if (auto *ta = ritual->getTriggeredAbility()) {
            ta->detachFromSub(*ritual->getOwner());          // disengage observer
        }    
        ritual->getOwner()->getBoard()->clearRitual();      // clear slot
    }
}
