#include "Raisedead.h"
#include "../Player.h"
#include "../Graveyard.h"
#include "../Minion.h"
#include "../Board.h"
#include <memory>
#include <stdexcept>


RaiseDead::RaiseDead(Player *owner)
    : Spell(2, "Return a minion from your graveyard to the battlefield.", "Raise Dead", owner) {}

bool RaiseDead::needTarget() const {
    return false; // This spell does not require a target
}


RaiseDead::~RaiseDead() = default;


void RaiseDead::activate(Player& caster, Target*){
    std::unique_ptr<Minion> minPtr = caster.getGraveyard()->popTopMinion();
    if (!minPtr) return; // No minion to raise

    minPtr->setHp(1); // Resurrected with 1 HP

    // Summon it to the board
    caster.getBoard()->summonMinion(std::move(minPtr));
}


void RaiseDead::activate(Player& caster, Ritual* r) {
    throw std::runtime_error("This spell cannot be played on a ritual");
}
