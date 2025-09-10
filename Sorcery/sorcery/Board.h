#ifndef BOARD_H
#define BOARD_H
#include "Card.h"
#include "CardCollection.h"
#include "Ritual.h"
#include "Minion.h"

class Minion;

class Board: public CardCollection{
    std::unique_ptr<Ritual> ritual;
    const int MAX_CAPACITY = 5;
  public:
    bool hasMinionSpace();
    std::unique_ptr<Ritual> &getRitual();
    void setRitual(std::unique_ptr<Ritual> &&r);
    void clearRitual();
    void destroyCard(Card& c);
    void returnMinionToHand(Minion& m);
    void destroyMinion(Minion& m);
    void destroyRitual(Ritual& r);
    void replaceMinion(Minion* oldM, std::unique_ptr<Minion> newM);
    void summonMinion(std::unique_ptr<Minion> m);
    bool hasRitual() const;
};

#endif
