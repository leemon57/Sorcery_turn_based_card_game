#include "Minion.h"
#include "Enchantment.h"
#include <iostream>
using namespace std;

Minion::Minion(std::string name, std::string description, int cost, Player *owner):
    Card(cost, description, name, CardType::ENCHANTMENT, owner), silenced{false} {}

Minion::Minion(std::string name, std::string description, int cost, int atk, int def, Player *owner):
    Card(cost, description, name, CardType::MINION, owner), atk{atk}, def{def}, actions{1}, silenced{false} {}

void Minion::setActive(unique_ptr<ActivatedAbility> aab){
    activeAbility = move(aab);
}
// check notes for pass as ref or ptr
void Minion::setTrigger(unique_ptr<TriggeredAbility> tab){
    triggerAbility = move(tab);
}

void Minion::gainAction(int a){
    actions += a;
}

void Minion::setSilenced(bool tf){
    silenced = tf;
}

bool Minion::isSilence(){
    return silenced;
}

void Minion::attack(Target *t){
    if (getActions() > 0){
        if (t->getType() == Type::MINION){
            Minion *m = static_cast<Minion*>(t);
            m->takeDamage(getAttack());
            takeDamage(m->getAttack());
        } else {
            Player *m = static_cast<Player*>(t);
            m->takeDamage(getAttack());
        }
        --actions;
    } else {
        throw runtime_error("Not enough actions");
    }

}

int Minion::getAttack() const{
    return atk;
}

void Minion::takeDamage(int dmg){
    def -= dmg;
    cout << "Current HP: " << getHp() << endl;
    if (def <= 0) {
        // Remove from board, broadcast MINION_LEAVE, etc.
        getOwner()->getBoard()->destroyMinion(*this);
    }
} // apply damage to the target


Player* Minion::getOwner() const{
    return Card::getOwner();
} // return the owner of the target


void Minion::gainHp(int hp){
    def += hp;
} // apply healing to the target

void Minion::gainAttack(int attk){
    atk += attk;
}

int Minion::getActions() const{
    return actions;
}

bool Minion::isDead(){
    return getHp() <= 0;
}

int Minion::getAbilityCost() const {
    return activeAbility ? activeAbility->getCost() : 0;
}


void Minion::useAbility(Target *t) {
    if (!activeAbility)
        throw std::runtime_error("Minion has no activated ability");

    if (actions <= 0 || isSilence() ||
        !activeAbility->canUse(*getOwner(), *this) ||
        !activeAbility->canPay(*getOwner(), *this))
        throw std::runtime_error("Not enough actions, silenced, or not enough magic");

    activeAbility->execute(t);
    --actions;
}


Type Minion::getType(){
    return Type::MINION;
}

int Minion::getHp() const{
    return def;
}


Minion::~Minion() {
    // Detach abilities from the subject when minion is destroyed
    if (triggerAbility) {
        triggerAbility->detachFromSub(*getOwner());
    }
}


void Minion::setHp(int hp) {
    def = hp;
}


const std::unique_ptr<ActivatedAbility>& Minion::getActivatedAbility() const {
    return activeAbility;
}


const std::unique_ptr<TriggeredAbility>& Minion::getTriggeredAbility() const {
    return triggerAbility;
}


int Minion::getDefence() const {
    return def;
}

