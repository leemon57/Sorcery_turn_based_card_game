#include "Disenchant.h"
#include "../Enchantment.h"
#include <stdexcept>



Disenchant::Disenchant(Player *owner) : 
    Spell(1, "Destroy the top enchantment on target minion.", "Disenchant", owner) {}


bool Disenchant::needTarget() const {
    return true; // Disenchant requires a target
}


Disenchant::~Disenchant() = default;


void Disenchant::activate(Player& caster, Target* tgt) {
   auto* ench = dynamic_cast<Enchantment*>(tgt);
    if (ench->isSilence()) {
        throw std::runtime_error("Disenchant: target must be an enchanted minion");
    }

    // Remove the enchantment from the minion
    Minion* newHead = ench->removeEnchantment();
    if (!newHead) {
        return; // no more decorators
    }

    // Replace the old pointer on the board with the new minion with no enchantments
    caster.getBoard()->replaceMinion(
        // old decorated pointer
        dynamic_cast<Minion*>(tgt),
        // ownership of the raw pointer
        std::unique_ptr<Minion>(newHead)
    );
    
}

void Disenchant::activate(Player& caster, Ritual* r) {
    throw std::runtime_error("This spell cannot be played on a ritual");
}

