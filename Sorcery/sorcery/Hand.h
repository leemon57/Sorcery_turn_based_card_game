#ifndef HAND_H
#define HAND_H
#include "CardCollection.h"

class Hand: public CardCollection{
    const int MAX_CAPACITY = 5;
  public:
    bool isFull();
};

#endif
