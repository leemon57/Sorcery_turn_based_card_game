#ifndef AURA_OF_POWER_ABILITY_H
#define AURA_OF_POWER_ABILITY_H
#include "../TriggeredAbility.h"

class AuraOfPowerAbility : public TriggeredAbility {
    public:
        explicit AuraOfPowerAbility(Card* host);
        void notify(EventType evt, Event &curEvt, Target *tgt = nullptr) override;
        void execute(Target* tgt) override;
};

#endif
