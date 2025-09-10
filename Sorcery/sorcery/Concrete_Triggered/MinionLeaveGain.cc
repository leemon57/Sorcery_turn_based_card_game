#include "MinionLeaveGain.h"
#include "../TriggeredAbility.h"
#include "../Ritual.h"
#include "../Card.h"
#include "../Player.h"
#include "../Minion.h"


MinionLeaveGain::MinionLeaveGain(Card* owner, int atk, int def)
    : TriggeredAbility(EventType::MINION_LEAVE, owner), atk{atk}, def{def} {}

void MinionLeaveGain::execute(Target* target) {
    if (!dynamic_cast<Minion*>(target)) {
        return;
    }

    // Buff the hosting minion (the one that owns this ability)
    auto* host = dynamic_cast<Minion*>(getOwner());
    host->gainAttack(atk);
    host->gainHp(def);
}
