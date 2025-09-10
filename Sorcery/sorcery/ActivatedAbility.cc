#include "ActivatedAbility.h"
#include "Player.h"
#include "Minion.h"


ActivatedAbility::ActivatedAbility(int cost) : cost{cost} {}


bool ActivatedAbility::canUse(Player& owner, Minion& host) const {
    if (host.isSilence() || host.getActions() == 0) {
        return false;
    }
    int realCost = host.getAbilityCost(); 
    return owner.getMagic() >= realCost;
}

int ActivatedAbility::getCost() const{
    return cost;
}

bool ActivatedAbility::canPay(Player& owner, Minion& host) const {
    int realCost = host.getAbilityCost();
    return owner.payMagic(realCost);
}

void ActivatedAbility::execute(Target* tgt) {
    effect(tgt);
}

