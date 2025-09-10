#ifndef STAND_STILL_ABILITY_H
#define STAND_STILL_ABILITY_H
#include "../TriggeredAbility.h"


class StandstillAbility : public TriggeredAbility {
    public:
        explicit StandstillAbility(Card* host);
        void execute(Target* target) override;
};


#endif
