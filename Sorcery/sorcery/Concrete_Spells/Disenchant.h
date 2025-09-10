#ifndef DISENCHANT_H
#define DISENCHANT_H

#include "../Spell.h"


class Disenchant : public Spell {
    public:
        explicit Disenchant(Player *owner);
        ~Disenchant() override;
        bool needTarget() const override;
        void activate(Player& caster, Target* tgt) override;
        void activate(Player& caster, Ritual* r) override;
};


#endif
