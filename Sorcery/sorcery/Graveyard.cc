#include "Graveyard.h"



Card* Graveyard::top() const {
    auto* vec = getCards();
    if (vec->empty()) {
        return nullptr;
    }
    return vec->back().get();
}


Minion* Graveyard::topMinion() const {
    if (Card* c = top()) {
        return dynamic_cast<Minion*>(c);
    }
    return nullptr;
}


std::unique_ptr<Minion> Graveyard::popTopMinion() {
    auto* vec = getCards();
    if (vec->empty()) return nullptr;

    Card* topRaw = vec->back().get();
    if (dynamic_cast<Minion*>(topRaw)) {
        std::unique_ptr<Card> card = std::move(vec->back());
        vec->pop_back();
        return std::unique_ptr<Minion>{static_cast<Minion*>(card.release())};
    }
    return nullptr;
}
