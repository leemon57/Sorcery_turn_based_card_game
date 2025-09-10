#include "Recharge.h"
#include "../Player.h"
#include "../Board.h"
#include "../Ritual.h"
#include <stdexcept>


Recharge::Recharge(Player* owner) 
    : Spell(1, "Your ritual gains 3 charges.", "Recharge", owner) {}


bool Recharge::needTarget() const {
    return false; // Recharge does not require a target
}

Recharge::~Recharge() = default;

void Recharge::activate(Player& caster, Target*) {
    // find a ritual on the board and increase its charges
    std::unique_ptr<Ritual>& ritPtr = caster.getBoard()->getRitual();
    if (Ritual* r = ritPtr.get()) {
        r->addCharges(3);
    }
}

void Recharge::activate(Player& caster, Ritual* r) {
    throw std::runtime_error("This spell cannot be casted on a ritual");
}
