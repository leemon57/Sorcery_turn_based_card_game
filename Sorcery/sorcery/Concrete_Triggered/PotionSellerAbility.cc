#include "PotionSellerAbility.h"
#include "../Card.h"
#include "../Minion.h"



PotionSellerAbility::PotionSellerAbility(EventType triggerType, Card* owner)
  : TriggeredAbility(triggerType, owner) {}

void PotionSellerAbility::notify(EventType evt, Event& curEvt, Target *tgt) {
    if (evt != getTriggerType()) {
        return;
    }

    // Only trigger on *your* end‑of‑turn
    if (curEvt.getActivePlayer() != getOwner()->getOwner()) {
        return;
    }

    // Now apply the buff
    execute(tgt);
}

void PotionSellerAbility::execute(Target* tgt) {
    auto* endingPlayer = dynamic_cast<Player*>(tgt);
    if (!endingPlayer) {
        return;                       // safety
    }

    //Card -> player
    auto* controller = getOwner()->getOwner();  // Potion Seller’s owner
    if (endingPlayer != controller) {
        return;          // opponent’s turn → ignore
    }

    //Give every friendly minion +0/+1
    for (auto& up : *controller->getBoard()->getCards()) {
        if (auto* m = dynamic_cast<Minion*>(up.get())) {
            m->gainHp(1);
        }
    }
}

