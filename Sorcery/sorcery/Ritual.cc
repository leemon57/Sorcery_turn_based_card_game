#include "Ritual.h"
#include "Card.h"
#include "Subject.h"
#include "Player.h"



Ritual::Ritual(int cost,
               std::string &description,
               std::string &name,
               CardType ct,
               Player *owner,
               int activationCost,
               int charges)
    : Card(cost, description, name, ct, owner),
      activationCost{activationCost},
      charges{charges} {}

Ritual::~Ritual() {
    if (auto *ta = getTriggeredAbility()) {
        if (Player *own = getOwner())
            ta->detachFromSub(*own);
    }
}

int Ritual::getActivationCost() const {
    return activationCost;
}   

int Ritual::getCharges() const {
    return charges;
}


void Ritual::spendCharges(int i) {
    charges -= i;
    if (charges < 0) {
        charges = 0;
    }
}

void Ritual::setTrigger(std::unique_ptr<TriggeredAbility> trig) {
    trigger = std::move(trig);
}


void Ritual::setActivationCost(int cost) {
    activationCost = cost;
}


void Ritual::setCharges(int c) {
    charges = c;
}


void Ritual::addCharges(int c) {
    charges += c;
}

TriggeredAbility* Ritual::getTriggeredAbility() const {
    return trigger.get();
}
