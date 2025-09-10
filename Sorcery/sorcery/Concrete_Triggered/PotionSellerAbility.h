#ifndef POTION_SELLER_ABILITY_H
#define POTION_SELLER_ABILITY_H
#include "../TriggeredAbility.h"


class PotionSellerAbility : public TriggeredAbility {
public:
    explicit PotionSellerAbility(EventType triggerType, Card* owner);
    void notify(EventType evt, Event &ev, Target *tgt = nullptr) override;
    void execute(Target* tgt) override;
};



#endif
