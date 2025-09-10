#ifndef RAISEREAD_H
#define RAISEREAD_H
#include "../Spell.h"


class RaiseDead : public Spell {
    public:
        explicit RaiseDead(Player *owner);
        ~RaiseDead() override;
        bool needTarget() const override;
        void activate(Player& caster, Target* tgt) override;
        void activate(Player& caster, Ritual* r) override;
};






#endif
