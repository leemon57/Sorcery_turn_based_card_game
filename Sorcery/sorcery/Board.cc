#include "Board.h"
#include "Enchantment.h"
#include "Card.h"
#include <stdexcept>
#include "Game.h"
using namespace std;



bool Board::hasMinionSpace(){
    return getSize() != MAX_CAPACITY;
}

std::unique_ptr<Ritual> &Board::getRitual(){
    return ritual;
}

bool Board::hasRitual() const {
    return ritual != nullptr;
}

void Board::setRitual(std::unique_ptr<Ritual> &&r){
    ritual = move(r);
}


void Board::clearRitual(){
    ritual.reset();
}


void Board::destroyRitual(Ritual& r) {
    Player* owner = r.getOwner();
    if (!owner) return;
    
    Board* board = owner->getBoard();
    
    // Check if this is the active ritual
    if (board->getRitual().get() == &r) {
        board->clearRitual();
    }
}

void Board::destroyMinion(Minion &m) {
    // Guard & APNAP broadcast BEFORE any board mutations
    Player *owner = m.getOwner();
    if (!owner) return;

    Game *g  = owner->getGame();                // back‑pointer you added to Player
    Event ev{ &m, owner, g->getActive(), 0 };   // active filled in for observers
    g->broadcast(EventType::MINION_LEAVE, ev);   // active first, then non‑active


     // Locate the minion’s index on this board (your code)
    auto cards = getCards();                     // std::vector<unique_ptr<Card>>*
    int idx = -1;
    for (int i = 0, n = getSize(); i < n; ++i) {
        if ((*cards)[i].get() == &m) { 
            idx = i; break; 
        }
    }
    if (idx < 0) {
        throw std::runtime_error("Board::destroyMinion: minion not found on board");
    }

    std::unique_ptr<Card> &slot = (*cards)[idx];
    while (dynamic_cast<Enchantment*>(slot.get())) {
        std::unique_ptr<Enchantment> outer{
            static_cast<Enchantment*>(slot.release())
        };
        std::unique_ptr<Minion> innerPtr{ outer->removeEnchantment() };
        slot = std::move(innerPtr);    // bare Minion back in slot
    }

    Minion *baseMinion = dynamic_cast<Minion*>(slot.get());
    if (baseMinion) {
        if (auto *ta = baseMinion->getTriggeredAbility().get()) {
            ta->detachFromSub(*owner); // unsubscribe from subject
        }
    }
  
    owner->getGraveyard()->add(remove(idx));     // unique_ptr moves to graveyard
}



void Board::destroyCard(Card& c) {
    Player* owner = c.getOwner();
    if (!owner) return; // Safety check
    // Determine what type of card we're destroying and where it is
    if (auto* minion = dynamic_cast<Minion*>(&c)) {
        destroyMinion(*minion);
    } else if (auto* ritual = dynamic_cast<Ritual*>(&c)) {
        destroyRitual(*ritual);
    }
}


void Board::replaceMinion(Minion* oldM, std::unique_ptr<Minion> newM) {
    auto& cards = *getCards();
    for (auto& up : cards) {
        if (up.get() == oldM) {
            up = std::move(newM);  // overwrite the unique_ptr
            return;
        }
    }
}


void Board::summonMinion(std::unique_ptr<Minion> m) {
    if (getSize() >= MAX_CAPACITY) {
        throw std::runtime_error("Board is full");
    }
    add(std::move(m));
}


void Board::returnMinionToHand(Minion &m) {
    // locate the owner 
    Player *owner = m.getOwner();
    if (!owner) return;                         // safety

    // detach triggered ability, if present
    if (auto *ta = m.getTriggeredAbility().get()) {
        ta->detachFromSub(*owner);
    }
        
    //find the slot that holds this (outermost) object
    int idx = -1;
    for (int i = 0; i < getSize(); ++i) {
        if ( (*getCards())[i].get() == &m ) {
            idx = i; break;
        }
    }
    if (idx < 0)  {
        throw std::runtime_error("returnMinionToHand: minion not on board");
    }
        
    // take ownership of the top‑level unique_ptr<Card>
    std::unique_ptr<Card> up = remove(idx);

    // peel all enchantments so we return a bare Minion
    while (dynamic_cast<Enchantment*>(up.get())) {
        std::unique_ptr<Enchantment> outer(
            static_cast<Enchantment*>(up.release()));
        Minion *inner = outer->removeEnchantment();   // gives raw pointer
        up.reset(inner);                              // wrap innermost
        // 'outer' is destroyed here, deleting that enchantment layer
    }

    //push the bare minion into the owner’s hand
    owner->getHand()->add(std::move(up));
}
