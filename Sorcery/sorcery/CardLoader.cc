#include <fstream>
#include "CardLoader.h"
#include "Minion.h"
#include "ActivatedAbility.h"
#include "EventType.h"
#include "Enchantment.h"
#include "./Concrete_Triggered/MinionLeaveGain.h"
#include "./Concrete_Triggered/FireElementalAbility.h"
#include "./Concrete_Triggered/GainMagicAtStart.h"
#include "./Concrete_Triggered/AuraOfPowerAbility.h"
#include "./Concrete_Triggered/PotionSellerAbility.h"
#include "./Concrete_Triggered/StandStillAbility.h"
#include "./Concrete_Activated/DamageToMinion.h"
#include "./Concrete_Activated/SummonAirElemental.h"
#include "./Concrete_Spells/Banish.h"
#include "./Concrete_Spells/Blizzard.h"
#include "./Concrete_Spells/Disenchant.h"
#include "./Concrete_Spells/Raisedead.h"
#include "./Concrete_Spells/Recharge.h"
#include "./Concrete_Spells/Unsummon.h"
using namespace std;

unique_ptr<Card> CardLoader::loadCard(string name, Player *owner){
    {
        // .card general format: type cost 
        //                       description
        // minion: atk def aab tab (number of active/trigger abilities) 
        //         (aability name) (aability cost) (aab values) (tability name) (tab values)
        // ritual: tability-name charges activation-cost triggerT
        // spell: (values if have)
        // enchantment: operator atkBuff hpBuff costBuff actionBuff isSilenced

        ifstream f{"./card-data/" + name + ".card"};
        string type, description, aability, tability, op;
        int cost, atk, def, charge, aab, tab, aabcost, triggerT;
        f >> type >> cost;
        getline(f, description);
        getline(f, description);
        // load each card by involking corresponding constructors
        if (type == "m"){
            f >> atk >> def >> aab >> tab;
            unique_ptr<Card> p = make_unique<Minion>(name, description, cost, atk, def, owner);
            Minion* minion = static_cast<Minion*>(p.get());
            cout << name << minion->getAttack() << endl;
            if (aab){
                f >> aability >> aabcost;
                // hardcode abilities
                if (aability == "DamageToMinion"){
                    minion->setActive(
                        make_unique<DamageToMinion>(minion, aabcost, 1)
                    );
                } else if (aability == "SummonAirElemental"){
                    if (name == "Apprentice Summoner"){
                        minion->setActive(
                            make_unique<SummonAirElementalAbility>(minion, aabcost, 1)
                        );
                    } else if (name == "Master Summoner"){
                        minion->setActive(
                            make_unique<SummonAirElementalAbility>(minion, aabcost, 3)
                        );
                    }
                }
            } else {
                minion->setActive(nullptr);
            }
            if (tab){
                f >> tability ;
                // hard code which tability
                if (tability == "MinionLeaveGain"){
                    f >> atk >> def;
                    minion->setTrigger(
                        make_unique<MinionLeaveGain>(p.get(), atk, def)
                    );
                } else if (tability == "FireElementalAbility"){
                    minion->setTrigger(
                        make_unique<FireElementalAbility>(p.get())
                    );
                } else if (tability == "PotionSellerAbility"){
                    minion->setTrigger(
                        make_unique<PotionSellerAbility>(EventType::END_TURN, p.get())
                    );
                }
            }
            return p;
        } 
        // ─── Ritual ───────────────────────────────────────────────
        else if (type == "r") {
            f >> tability >> charge >> tab >> triggerT;
            unique_ptr<Card> p = make_unique<Ritual>(
                cost, description, name, CardType::RITUAL, owner,
                /*activationCost=*/tab,
                /*charges=*/charge
            );
            Ritual* rawR = static_cast<Ritual*>(p.get());

            std::unique_ptr<TriggeredAbility> trig;
            if (tability == "GainMagicAtStart") {
                // ctor is GainMagicAtStart(Card* owner, int amount)
                trig = make_unique<GainMagicAtStart>(rawR, 1);
            }
            else if (tability == "StandStillAbility") {
                // ctor is StandstillAbility(Card* owner)
                trig = make_unique<StandstillAbility>(rawR);
            }
            else if (tability == "AuraOfPowerAbility") {
                // ctor is AuraOfPowerAbility(Card* owner)
                trig = make_unique<AuraOfPowerAbility>(rawR);
            }
            rawR->setTrigger(move(trig));
            return p;
        } else if (type == "s"){
            unique_ptr<Card> p;
            if (name == "Banish"){
                p = make_unique<Banish>(owner);
            } else if (name == "Disenchant"){
                p = make_unique<Disenchant>(owner);
            } else if (name == "Unsummon"){
                p = make_unique<Unsummon>(owner);
            } else if (name == "Raise Dead"){
                p = make_unique<RaiseDead>(owner);
            } else if (name == "Recharge"){
                p = make_unique<Recharge>(owner);
            } else if (name == "Blizzard"){
                p = make_unique<Blizzard>(owner);
            } 
            return p;
        } else if (type == "e"){
            int atkBuff, hpBuff, costBuff, actionBuff;
            unique_ptr<Card> p;
            f >> op >> atkBuff >> hpBuff >> costBuff >> actionBuff >> tability;
            if (op == "m" && tability == "t"){
                p = make_unique<Enchantment>(name, description, cost, owner, atkBuff, hpBuff,
                                                      costBuff, actionBuff, true, true);
            } else if (op == "m"){
                p = make_unique<Enchantment>(name, description, cost, owner, atkBuff, hpBuff,
                                                      costBuff, actionBuff, true);
            } else if (tability == "t"){
                p = make_unique<Enchantment>(name, description, cost, owner, atkBuff, hpBuff,
                                                      costBuff, actionBuff, false, true);
            } else {
                p = make_unique<Enchantment>(name, description, cost, owner, atkBuff, hpBuff,
                                                      costBuff, actionBuff);
            }
            return p;
        } else {
            throw logic_error("Not a valid card type");
            return nullptr;
        }
        
    }
}

// loading the whole deck by loading all cards in the given deckFile
unique_ptr<Deck> CardLoader::loadDeck(string deckFile, Player *owner){
    ifstream f {deckFile};
    unique_ptr<Deck> d (new Deck());
    string cardName;
    while (getline(f, cardName)){
        //cout << "loading " << cardName;
        d->add(loadCard(cardName, owner));
        //cout << " " << (*(d.get()->getCards()))[d->getSize() - 1]->getName() << endl;
    }
    f.close();
    return d;
}


