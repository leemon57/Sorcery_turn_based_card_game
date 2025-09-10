#ifndef DAMAGE_TO_MINION_H
#define DAMAGE_TO_MINION_H
#include "../ActivatedAbility.h"     // base class
#include "../Minion.h"


class DamageToMinion : public ActivatedAbility {
    Minion* hostMinion;
    int damage;  // Amount of damage to deal

  public:
    explicit DamageToMinion(Minion* hostMinion, int cost, int damage);
    void effect(Target* target) override;
    bool needTarget() const override;
};

#endif