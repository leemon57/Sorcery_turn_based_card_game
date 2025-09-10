#ifndef MINIONLEAVEGAINGAIN_H
#define MINIONLEAVEGAINGAIN_H

#include "../TriggeredAbility.h"
#include "../Ritual.h"
#include "../Card.h"
#include "../Player.h"
#include "GainMagicAtStart.h"

// for bone golem
class MinionLeaveGain : public TriggeredAbility {
    int atk;
    int def;

  public:
    explicit MinionLeaveGain(Card* owner, int atk, int def);
    void execute(Target* target) override;
};


#endif
