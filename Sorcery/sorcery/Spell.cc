#include "Spell.h"


Spell::Spell(int cost, const std::string &description, const std::string &name, Player *owner) 
    : Card(cost, description, name, CardType::SPELL, owner) {}
