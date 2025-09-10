#ifndef GRAVEYARD_H
#define GRAVEYARD_H
#include "CardCollection.h"
#include "Minion.h"
#include "Card.h"

class Minion;

class Graveyard: public CardCollection{
    public:
        Card* top() const; // returns the top card
        Minion* topMinion() const; // returns the top minion, or nullptr if none
        std::unique_ptr<Minion> popTopMinion();
};



#endif
