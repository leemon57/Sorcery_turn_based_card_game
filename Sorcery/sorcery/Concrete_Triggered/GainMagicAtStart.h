#ifndef GAIN_MAGIC_AT_START_H
#define GAIN_MAGIC_AT_START_H
#include "../TriggeredAbility.h"
#include "../Ritual.h"


// for dark ritual

class GainMagicAtStart : public TriggeredAbility {
    int amount;
public:
    explicit GainMagicAtStart(Card* owner, int amount = 1);
    void execute(Target* target) override;
};


#endif
