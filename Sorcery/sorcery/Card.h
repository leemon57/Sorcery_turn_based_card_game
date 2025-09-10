#ifndef CARD_H
#define CARD_H
#include "Target.h"
#include <string>

class Player;

enum class CardType {MINION, ENCHANTMENT, RITUAL, SPELL};

class Player; // forward declaration

class Card{
    int cost;
    std::string description;
    std::string name;
    CardType ct;
    Player *owner;
  public:
    Card(int cost, const std::string &description, const std::string &name, CardType ct, Player *owner);
    Player* getOwner() const;
    std::string getDescription() const;
    std::string getName() const;
    int getCost() const;
    CardType getCardType() const;
    virtual ~Card();
    virtual bool modifiesStats() const;
};

#endif
