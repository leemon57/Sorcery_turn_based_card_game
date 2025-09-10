#ifndef ACTIVATED_ABILITY_H
#define ACTIVATED_ABILITY_H

#include "Ability.h"
#include "Target.h"
#include "Player.h"
#include "Minion.h"

class Target;  // Forward declaration of Target class
class Minion;

class ActivatedAbility : public Ability {
  protected:
    int cost;       // The mana cost of the ability

  public:
    explicit ActivatedAbility(int cost);
    ~ActivatedAbility() override = default;
    // all the other concrete activated abilities will implement these two methods
    virtual void effect(Target* tgt) = 0;
    virtual bool needTarget() const = 0; // Check if the ability requires a target
    
    bool canUse(Player& owner, Minion& host) const;
    bool canPay(Player& owner, Minion& host) const;
    void execute(Target* tgt) override final; // Execute the ability on the target
    int getCost() const;
};

#endif
