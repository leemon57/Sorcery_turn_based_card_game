#ifndef BANISH_H
#define BANISH_H
#include "../Spell.h"


class Banish : public Spell {
    public:
        explicit Banish(Player *owner);
        ~Banish() override;
        bool needTarget() const override;
        void activate(Player& caster, Target* tgt) override;
        void activate(Player& caster, Ritual* r) override;
};
#endif
