#include "SummonAirElemental.h"
#include "../Minion.h"
#include "../Player.h"
#include "../Board.h"

SummonAirElementalAbility::SummonAirElementalAbility(Minion* host, int cost, int count)
  : ActivatedAbility(cost),
    hostMinion{host},
    count{count} {}

bool SummonAirElementalAbility::needTarget() const {
    return false;
}


void SummonAirElementalAbility::effect(Target* tgt) {
    auto* controller = hostMinion->getOwner();
    auto* board      = controller->getBoard();
    for (int i = 0; i < count && board->hasMinionSpace(); ++i) {
        // manually build a 1/1 Air Elemental token
        auto token = std::make_unique<Minion>(
            /*name*/"Air Elemental",
            /*desc*/"Summoned 1/1 Air Elemental token",
            /*cost*/1,
            /*atk*/1,
            /*def*/1,
            /*owner*/controller
        );
        board->summonMinion(std::move(token));
    }
}
