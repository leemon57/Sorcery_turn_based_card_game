#include "Blizzard.h"
#include "../Player.h"     
#include "../Board.h"
#include "../Minion.h"
#include <stdexcept>


Blizzard::Blizzard(Player* owner) 
    : Spell(3, "Deal 2 damage to all enemy minions.", "Blizzard", owner) {}


Blizzard::~Blizzard() = default;

bool Blizzard::needTarget() const { 
    return false; 
}

void Blizzard::activate(Player& caster, Target*) {
    // Damage caster's minions
    auto myCards = caster.getBoard()->getCards();  
    for (auto& cardPtr : *myCards) {
        if (auto m = dynamic_cast<Minion*>(cardPtr.get()))
            m->takeDamage(2);
    }

    // Damage opponent's minions
    if (Player* opp = caster.getOpponent()) {
        auto theirCards = opp->getBoard()->getCards();
        for (auto& cardPtr : *theirCards) {
            if (auto m = dynamic_cast<Minion*>(cardPtr.get()))
                m->takeDamage(2);
        }
    }
}

void Blizzard::activate(Player& caster, Ritual* r) {
    throw std::runtime_error("This spell cannot be played on a ritual");
}
