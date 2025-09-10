#ifndef RITUAL_H
#define RITUAL_H
#include <memory>
#include "Card.h"
#include "TriggeredAbility.h"
#include "Subject.h"
#include <string>

class Ritual: public Card{
    int activationCost;
    int charges;
    std::unique_ptr<TriggeredAbility> trigger;
  public:
    Ritual(int cost,
           std::string &description,
           std::string &name,
           CardType ct,
           Player *owner,
           int activationCost,
           int charges);
    ~Ritual();
    // accessors
    int getActivationCost() const;
    int getCharges() const;
    void spendCharges(int i);
    // mutators
    void setActivationCost(int cost);
    void setCharges(int c);
    void setTrigger(std::unique_ptr<TriggeredAbility> trig);
    void addCharges(int c);
    TriggeredAbility* getTriggeredAbility() const;
    
};

#endif
