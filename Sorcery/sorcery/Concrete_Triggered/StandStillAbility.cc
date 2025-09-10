#include "StandStillAbility.h"
#include "../Minion.h"
#include "../Player.h"
#include "../Board.h"
#include "../Event.h"


StandstillAbility::StandstillAbility(Card* host)
  : TriggeredAbility(EventType::MINION_ENTER, host) {}


void StandstillAbility::execute(Target* target) {
  auto *ritual = dynamic_cast<Ritual*>(getOwner());
  if (!ritual) return;

  auto *minion = dynamic_cast<Minion*>(target);
  if (!minion) return; // triggered on non‑minion? ignore

  if (ritual->getCharges() < ritual->getActivationCost()) {
      return; // no charges
  }
  ritual->spendCharges(ritual->getActivationCost());

  // Destroy the entering minion
  minion->getOwner()->getBoard()->destroyMinion(*minion);

  if (ritual->getCharges() == 0) {
      if (auto *ta = ritual->getTriggeredAbility()) {
          ta->detachFromSub(*ritual->getOwner());          // disengage observer
      }    
      ritual->getOwner()->getBoard()->clearRitual();      // clear slot
  }
}
