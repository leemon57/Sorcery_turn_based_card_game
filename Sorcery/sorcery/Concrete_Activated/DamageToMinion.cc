#include "DamageToMinion.h"
#include "../Minion.h"


DamageToMinion::DamageToMinion(Minion* host, int cost, int damage)
    : ActivatedAbility(cost), hostMinion{host}, damage{damage} {}


void DamageToMinion::effect(Target* target) {
    if (!target) {
        return;
    }
    if (auto* m = dynamic_cast<Minion*>(target)) {
        m->takeDamage(damage);
    }
}


bool DamageToMinion::needTarget() const { 
    return true; 
}
