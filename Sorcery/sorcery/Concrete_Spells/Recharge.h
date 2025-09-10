#ifndef RECHARGE_H
#define RECHARGE_H

#include "../Spell.h"

class Recharge : public Spell {
public:
    explicit Recharge(Player* owner);
    ~Recharge() override;
    bool needTarget() const override;
    void activate(Player& caster, Target*) override;
    void activate(Player& caster, Ritual* r) override;
};

#endif
