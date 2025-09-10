#ifndef UNSUMMON_H
#define UNSUMMON_H
#include "../Spell.h"

class Unsummon : public Spell {
    public:
        explicit Unsummon(Player* owner);
        ~Unsummon() override;
        bool needTarget() const override { return true; }
        void activate(Player& caster, Target* tgt) override;
        void activate(Player& caster, Ritual* r) override;
};

#endif
