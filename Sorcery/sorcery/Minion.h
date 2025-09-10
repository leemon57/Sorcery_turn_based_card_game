#ifndef MINION_H
#define MINION_H
#include "Card.h"
#include "Subject.h"
#include <memory>
#include "ActivatedAbility.h"
#include "TriggeredAbility.h"

class Player; // forward declaration
class ActivatedAbility;
class TriggeredAbility;

class Minion: public Card, public Target{
    int atk;
    int def;
    int actions;
    bool silenced;
    std::unique_ptr<ActivatedAbility> activeAbility;
    std::unique_ptr<TriggeredAbility> triggerAbility;
  public:
    Minion(std::string name, std::string description, int cost, Player *owner); // enchantment const
    Minion(std::string name, std::string description, int cost, int atk, int def, Player *owner);
    ~Minion();
    void setActive(std::unique_ptr<ActivatedAbility> aab);
    void setTrigger(std::unique_ptr<TriggeredAbility> tab);
    void attack(Target *t);
    int getAttack() const override;
    int getDefence() const;
    void takeDamage(int dmg) override; // apply damage to the target
    Player* getOwner() const override; // return the owner of the target
    void gainHp(int hp) override; // apply healing to the target
    void gainAttack(int atk) override;
    Type getType() override;
    bool isDead() override;
    int getActions() const;
    void setSilenced(bool tf);
    void useAbility(Target *t = nullptr) override;
    void gainAction(int a = 1);
    int getHp() const override;
    virtual int getAbilityCost() const;
    bool isSilence();
    void setHp(int hp); // set the HP of the minion
    const std::unique_ptr<ActivatedAbility>& getActivatedAbility() const;
    const std::unique_ptr<TriggeredAbility>& getTriggeredAbility() const;
};

#endif
