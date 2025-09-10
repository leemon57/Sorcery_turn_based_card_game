#ifndef SPELL_H
#define SPELL_H
#include "Card.h"
#include "Ritual.h"

class Game;
class Target;

class Spell: public Card{
  public:
    Spell(int cost, const std::string &description, const std::string &name, Player *owner);
    virtual void activate(Player& caster, Target* tgt = nullptr) = 0;
    virtual void activate(Player& caster, Ritual* r) = 0;
    virtual bool needTarget() const = 0; // check if the spell needs a target
    virtual ~Spell() = default; // virtual destructor for proper cleanup
};

#endif
