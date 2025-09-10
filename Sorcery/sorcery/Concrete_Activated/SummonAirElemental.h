#ifndef SUMMON_AIR_ELEMENTAL_H
#define SUMMON_AIR_ELEMENTAL_H

#include "../ActivatedAbility.h"
#include "../Minion.h"


// this class can be used in apprentice summoner and master summoner
// pass the int count to indicate how many air elementals to summon
class SummonAirElementalAbility : public ActivatedAbility {
    Minion* hostMinion;
    int count;
public:
    // Cost 1 magic (plus action) to activate.
    explicit SummonAirElementalAbility(Minion* host, int cost, int count = 1);
    
    // no targer needee
    bool needTarget() const override;
    // Summon the token onto the board.
    void effect(Target* tgt) override;
};


#endif
