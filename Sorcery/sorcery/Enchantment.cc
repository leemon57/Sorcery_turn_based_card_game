#include "Enchantment.h"
using namespace std;

Enchantment::Enchantment(string name, string desc, int cost, Player *owner, 
                         int atkMod, int hpMod, int costMod, int actionMod, bool mult, bool isSilenced, Minion *m):
    Decorator{m, name, desc, cost, owner}, atkMod{atkMod}, hpMod{hpMod}, costMod{costMod}, actionMod{actionMod},
        mult{mult} {
            if (isSilenced){
                setSilenced(true);
            }
        }

int Enchantment::getAttack() const{
    if (mult){
        return atkMod * minion->getAttack();
    } else {
        return atkMod + minion->getAttack();
    }
}
void Enchantment::activate(Target *t){
    t = new Enchantment(getName(), getDescription(), getCost(), getOwner(), 
                        atkMod, hpMod, costMod, actionMod, mult, Minion::isSilence(), dynamic_cast<Minion*>(t));
}

void Enchantment::gainAction(int a){
    if (actionMod != 1){
        minion->gainAction(actionMod);
    } else {
        minion->gainAction();
    }
}

int Enchantment::getHp() const{
    if (mult){
        return hpMod * minion->getHp();
    } else {
        return hpMod + minion->getHp();
    }
}

bool Enchantment::isDead(){
    return Enchantment::getHp() <= 0;
}

bool Enchantment::isSilence(){
    return (Minion::isSilence() || minion->isSilence());
}

int Enchantment::getAbilityCost() const{
    if (mult){
        return costMod * minion->getCost();
    } else {
        return costMod + minion->getCost();
    }
}

void Enchantment::useAbility(Target *t){
    
}

Minion *Enchantment::removeEnchantment(){
    return minion;
}

int Enchantment::getAttackMod() const {
    return atkMod;
}

int Enchantment::getHpMod() const {
    return hpMod;
}

int Enchantment::getCostMod() const {
    return costMod;
}

int Enchantment::getActionMod() const {
    return actionMod;
}

bool Enchantment::getMult() const {
    return mult;
}
