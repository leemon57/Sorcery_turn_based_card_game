#ifndef ENCHANTMENT_H
#define ENCHANTMENT_H
#include "Decorator.h"
#include <string>

class Enchantment: public Decorator{
    int atkMod, hpMod, costMod, actionMod;
    bool mult;
  public:
 
    Enchantment(std::string name, std::string desc, int cost, Player *owner, 
                int atkMod, int hpMod, int costMod, int actionMod, bool mult = false, bool isSilenced = false, Minion *m = nullptr);
    void activate(Target *t);
    int getAttack() const override;
    int getHp() const override;
    int getAbilityCost() const override;
    bool isDead() override;
    void gainAction(int a = 1);
    bool isSilence();
    void useAbility(Target *t) override;
    Minion *removeEnchantment();
    int getAttackMod() const;
    int getHpMod() const;
    int getCostMod() const;
    int getActionMod() const;
    bool getMult() const;
};

#endif
