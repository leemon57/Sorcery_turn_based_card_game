#ifndef CARDCOLLECTION_H
#define CARDCOLLECTION_H
#include <vector> 
#include <memory>
#include "Card.h"


class CardCollection{
    std::vector<std::unique_ptr<Card>> cards;
  public:
    ~CardCollection();
    int getSize();
    std::vector<std::unique_ptr<Card>> *getCards();
    const std::vector<std::unique_ptr<Card>>* getCards() const; // for read-only access
    std::unique_ptr<Card> remove(int i); // remove card at index i
    void add(std::unique_ptr<Card> c); // add card to cards
    Card* get(int i) const;
};

#endif
