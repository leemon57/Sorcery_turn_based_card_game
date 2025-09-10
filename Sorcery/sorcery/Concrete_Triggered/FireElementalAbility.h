#ifndef DEALDAMAGEWHENMINIONENTERS_H
#define DEALDAMAGEWHENMINIONENTERS_H
#include "../TriggeredAbility.h"


class FireElementalAbility : public TriggeredAbility {
    public:
        explicit FireElementalAbility(Card* host);
        void notify(EventType evt, Event &curEvent, Target *tgt = nullptr) override;
        void execute(Target* target) override;
};



#endif
