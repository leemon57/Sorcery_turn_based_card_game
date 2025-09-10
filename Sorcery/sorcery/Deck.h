#ifndef DECK_H
#define DECK_H
#include "CardCollection.h"

class Deck: public CardCollection{
  public:
    void shuffle();
};

#endif
