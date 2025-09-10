#include "Unsummon.h"
#include "../Spell.h"
#include "../Player.h"
#include "../Board.h"
#include "../Minion.h"
#include "../TriggeredAbility.h"
#include <stdexcept>

Unsummon::Unsummon(Player* owner)
  : Spell(1,
          "Return target minion to its owner's hand.",
          "Unsummon",
          owner) {}

Unsummon::~Unsummon() = default;

void Unsummon::activate(Player& caster, Target* tgt){
    if (!tgt) {
        return; 
    }
    // Only minions can be returned
    Minion * m = dynamic_cast<Minion*>(tgt);
    if (!m) {
        throw std::runtime_error("Unsummon target must be a minion");
    }
    Player *owner = m->getOwner();
    auto *bd = caster.getBoard();
    auto *cards = bd->getCards();  // vector<unique_ptr<Card>>*
    int idx = -1; // if not found, idx remains -1
    for (int i = 0; i < caster.getBoard()->getSize(); ++i) {
        if ((*cards)[i].get() == m) {
            idx = i;
            break;
        }
    }
    if (idx < 0) {
        throw std::runtime_error("Unsummon: failed to find minion on board");
    }
   
   if (TriggeredAbility *ta = m->getTriggeredAbility().get()) {
        ta->detachFromSub(*owner);
   }
        
    std::unique_ptr<Card> raw = bd->remove(idx);       // Board::remove returns unique_ptr<Card>
    std::unique_ptr<Minion> mp{ static_cast<Minion *>(raw.release()) };
    owner->getHand()->add(std::move(mp));
}

void Unsummon::activate(Player& caster, Ritual* r) {
    throw std::runtime_error("This spell cannot be played on a ritual");
}