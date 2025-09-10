#include "Banish.h"
#include "../Game.h"
#include "../Card.h"
#include "../Player.h"


Banish::Banish(Player *owner) : 
    Spell(2, "Banish a minion from the game.", "Banish", owner) {}


Banish::~Banish() = default;

bool Banish::needTarget() const {
    return true; // Banish requires a target
}


void Banish::activate(Player &caster, Target *tgt) {
    if (!tgt) return;

    if (auto *victim = dynamic_cast<Minion*>(tgt)) {
        /* use the board that actually contains the minion */
        victim->getOwner()->getBoard()->destroyMinion(*victim);
        return;
    }
}

void Banish::activate(Player &caster, Ritual *rit) {
    if (!rit) return;
    if (auto *ta = rit->getTriggeredAbility()) {
        ta->detachFromSub(*rit->getOwner());
    }
    rit->getOwner()->getBoard()->clearRitual();
}
