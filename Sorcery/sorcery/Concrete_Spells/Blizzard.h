#ifndef BLIZZARD_H
#define BLIZZARD_H
#include "../Spell.h"


class Blizzard : public Spell {
    public:
        explicit Blizzard(Player* owner);
        ~Blizzard() override;
        bool needTarget() const override;
        void activate(Player& caster, Target* tgt) override;
        void activate(Player& caster, Ritual* r) override;
};






#endif
