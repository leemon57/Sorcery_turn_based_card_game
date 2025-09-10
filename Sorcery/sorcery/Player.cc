#include "Player.h"
#include "CardLoader.h"
#include "Enchantment.h"
#include "./Concrete_Spells/Banish.h"
#include "Spell.h"
#include <iostream>
#include "Game.h"

Player::Player(std::string nameArg, Game *g, std::string deckFileArg)
  : Subject()
  , game{g}
  , opponent{nullptr}
  , hand{std::make_unique<Hand>()}
  , deck{std::make_unique<Deck>()}
  , board{std::make_unique<Board>()}
  , graveyard{std::make_unique<Graveyard>()}
  , deckFile{std::move(deckFileArg)}
  , name{std::move(nameArg)}
  , magic{3}
  , hp{20}
  {
     Player::loadDeck(); // initialize deck
     std::cout << getName() << std::endl;
     for (int i = 0; i < 5; ++i){
        draw();
        //std::cout << (*(hand->getCards()))[i]->getName() << std::endl;
     }
  }

Player::~Player(){
    opponent = nullptr;
};

void Player::setDeck(std::string deckFileArg) {
    deckFile = deckFileArg;
}

void Player::setName(std::string nameArg) {
    name = nameArg;
}

std::string Player::getName() const {
    return name;
}

int Player::getHp() const {
    return hp;
}

int Player::getMagic() const {
    return magic;
}

void Player::draw() {
    if (!hand->isFull()){
        auto cardPtr = deck->remove(0);
        hand->add(std::move(cardPtr));
    } else {
        throw std::out_of_range("Hand is full");
    }
}

void Player::loadDeck() {
    CardLoader loader;
    // loads deck from given deckFile
    deck = move(loader.loadDeck(deckFile, this));
    //std::cout << deck->getSize() << std::endl;
    deck->shuffle();
}

void Player::discard(int i) {
    hand->remove(i);
    // for (int m = 0; m < hand->getSize(); ++m){
    //     std::cout << (*(hand->getCards()))[m]->getName() << std::endl;
    // }
}

void Player::attack(int i) {
    auto& attackerPtr = board->getCards()->at(i);
    // dynamic cast enforces type
    Minion* attacker = dynamic_cast<Minion*>(attackerPtr.get());
    if (!attacker) { // if dynamic cast fails attacker becomes nullptr
        throw std::runtime_error("Selected Card is not a minion");
    }

    attacker->attack(opponent);
}

void Player::attack(int i, int j) {
    auto& attackerPtr = board->getCards()->at(i);
    // dynamic cast enforces type
    Minion* attacker = dynamic_cast<Minion*>(attackerPtr.get());
    if (!attacker) { // if dynamic cast fails attacker becomes nullptr
        throw std::runtime_error("Selected Card is not a minion");
    }
    auto& defenderPtr = opponent->board->getCards()->at(j);
    Minion* defender = dynamic_cast<Minion*>(defenderPtr.get());    
    attacker->attack(defender);
    if (attacker->isDead()){
        board->destroyMinion(*attacker);
    }
    if (defender->isDead()){
        opponent->getBoard()->destroyMinion(*defender);
    }
}

void Player::useAbility(int i) {
    // bounds check
    if (i < 0 || i >= board->getSize()) {
        throw std::out_of_range("Ability index out of range");
    }

    auto& cardPtr = board->getCards()->at(i);
    Minion* m = dynamic_cast<Minion*>(cardPtr.get());
    if (!m) {
        throw std::runtime_error("Selected card is not a minion");
    }

    m->useAbility();
}

void Player::useAbility(int i, int pl, int t) {
    // index check
    if (i < 0 || i >= board->getSize()) {
        throw std::out_of_range("Ability source index out of range");
    }
    // cast to minion
    auto& srcPtr = board->getCards()->at(i);
    Minion* srcMinion = dynamic_cast<Minion*>(srcPtr.get());
    if (!srcMinion) {
        throw std::runtime_error("Selected card is not a minion");
    }

    // select player
    Player* targetPlayer = nullptr;
    if (pl == 0) {
        targetPlayer = this;
    } else if (pl == 1) {
        if (!opponent) throw std::runtime_error("Opponent not set");
        targetPlayer = opponent;
    } else {
        throw std::out_of_range("Invalid player index");
    }

    // target bounds
    if (t == -1){
        auto& tgtPtr = targetPlayer->board->getRitual();
        Ritual* tgt = dynamic_cast<Ritual*>(tgtPtr.get());
        if (!tgt) {
            throw std::runtime_error("No ritual is in play");
        }
        throw std::runtime_error("Sorry we got lazy to change function parameters since no minion aab targets rituals currently");
    }
    else if (t < 0 || t >= targetPlayer->board->getSize()) {
        throw std::out_of_range("Target index out of range");
    } else {
        auto& tgtPtr = targetPlayer->board->getCards()->at(t);
        Target* tgt = dynamic_cast<Target*>(tgtPtr.get());
        if (!tgt) {
            throw std::runtime_error("Selected target is not a minion");
        }
        srcMinion->useAbility(tgt);
    }
}

void Player::setOpponent(Player *opponentArgs) {
    opponent = opponentArgs;
}

Type Player::getType() {
    return Type::PLAYER;
}

void Player::gainHp(int i) {
    hp += i;
}

void Player::gainMagic(int i) {
    magic += i;
}

void Player::startTurn() {
    gainMagic(1);
    for (int m = 0; m < board->getSize(); ++m){
       static_cast<Minion*>(board->getCards()->at(m).get())->gainAction(1);
    }
    try {
        draw();
    } catch (std::out_of_range &e){
        std::cerr << e.what() << std::endl;
    }
    Event e(nullptr, this, this, 0);
    notifyObservers(EventType::START_TURN, e);
}

void Player::endTurn() {
    Event e(nullptr, this, this, 0);
    notifyObservers(EventType::END_TURN, e, this);
}

void Player::play(int i) {
    // bounds check
    if (i < 0 || i >= hand->getSize()) {
        throw std::out_of_range("Invalid index");
    }


    Card* check_card = hand->get(i);
    // check cost
    if (check_card->getCost() > magic) {
        throw std::runtime_error{"Not enough magic"};
    }

    // check if it requires a target
    // enchantments
    if (dynamic_cast<Enchantment*>(check_card)) {
        throw std::runtime_error("Enchantment requires a minion target. Syntax: play <handIdx> <player#> <minionIdx>");
    }

    // check spells
    if (Spell *s = dynamic_cast<Spell*>(check_card)) {
        if (s->needTarget()) {
            throw std::runtime_error("This spell needs a target");
        }
    }

    // now remove and do the rest
    std::unique_ptr<Card> cardPtr = hand->remove(i);
    Card* card = cardPtr.get();
    magic -= card->getCost();

    CardType ct = card->getCardType();
    if (ct == CardType::MINION) {
        if (!board->hasMinionSpace())
        throw std::runtime_error("No space to add minion");

        // move the card into the board first 
        board->add(std::move(cardPtr));

        // fetch the *real* object now stored on the board
        Minion *justPlayed = dynamic_cast<Minion*>( board->getCards()->back().get() );

        // attach its trigger (if any) to *this player*
        if (justPlayed && justPlayed->getTriggeredAbility()) {
            justPlayed->getTriggeredAbility()->attachTo(*this);
        }
            
        // notify MINION_ENTER observers 
        Event e{justPlayed, this, this, 0};
        game->broadcast(EventType::MINION_ENTER, e);
    } else if (ct == CardType::RITUAL) {
        std::unique_ptr<Ritual> up(static_cast<Ritual*>(cardPtr.release()));

        if (board->getRitual() && board->getRitual()->getTriggeredAbility())
            board->getRitual()->getTriggeredAbility()->detachFromSub(*this);

        board->setRitual(std::move(up));                // move onto board
        Ritual *rit = board->getRitual().get();         // ← get live pointer

        if (rit && rit->getTriggeredAbility())          // attach *now*
            rit->getTriggeredAbility()->attachTo(*this);
    } else if (ct == CardType::SPELL) {
        auto* sp = dynamic_cast<Spell*>(cardPtr.get());
        if (!sp) {
            throw std::logic_error("Not a spell");
        }

        //If it needs a target, we can't play it here
        if (sp->needTarget()) {
            throw std::logic_error("This spell requires a target");
        }
        // activate with no target
        sp->activate(*this);

    } else if (ct == CardType::ENCHANTMENT) {
        // shouldnt be used with no target
        throw std::logic_error("Enchantment cards require a target");
    } else {
        // unlisted card type
        throw std::logic_error("Invalid Card Type");
    }
}

void Player::play(int i, int pl, int t) {
    // bounds check
    if (i < 0 || i >= hand->getSize()) {
        throw std::out_of_range("Invalid index");
    }
    Card* check_card = hand->get(i);
    // check cost
    if (check_card->getCost() > magic) {
        throw std::runtime_error{"Not enough magic"};
    }

    if (Spell *s = dynamic_cast<Spell*>(check_card)) {
        if (!s->needTarget()) {
            throw std::runtime_error("This spell dont need a target");
        }
    }

    //check if there is ritual
    Player* target = (pl == 0) ? this : opponent;
    if (t == -1 && !target->getBoard()->getRitual()) {
        throw std::runtime_error{"No ritual is in play"};
    }

    // get card
    std::unique_ptr<Card> cardPtr = hand->remove(i);
    Card* card = cardPtr.get();

    // check cost
    if (card->getCost() > magic) throw std::runtime_error{"Not enough magic"};
    magic -= card->getCost();

     // select player
    Player* targetPlayer = nullptr;
    if (pl == 0) {
        targetPlayer = this;
    } else if (pl == 1) {
        if (!opponent) throw std::runtime_error("Opponent not set");
        targetPlayer = opponent;
    } else {
        throw std::out_of_range("Invalid player index");
    }

    // target bounds
    if (t == -1) {
        auto &tgtPtr = targetPlayer->board->getRitual();
        if (!tgtPtr){
            throw std::runtime_error("No ritual is in play");
        }
        Ritual *tgt = dynamic_cast<Ritual *>(tgtPtr.get());
        if (!tgt) {
            throw std::runtime_error("Selected target is not a ritual");
        }

        CardType ct = card->getCardType();
        if (ct == CardType::SPELL) {
            auto* sp = dynamic_cast<Spell*>(cardPtr.get());
            if (!sp) {
                throw std::logic_error("Not a spell");
            }
            if (!sp->needTarget()) {
                throw std::logic_error("This spell does not take a target");
            }

            if (auto* banish = dynamic_cast<Banish*>(sp)) {
                banish->activate(*this, tgt); // ✅ Ritual overload
            } else {
                throw std::runtime_error("This spell cannot target a ritual");
            }
        } else {
            throw std::logic_error("Card type cannot be played with a target or on ritual");
        }
    } else if (t < 0 || t >= targetPlayer->board->getSize()) {
        throw std::out_of_range("Target index out of range");
    } else {
        auto& tgtPtr = targetPlayer->board->getCards()->at(t);
        Target* tgt = dynamic_cast<Target*>(tgtPtr.get());
        if (!tgt) {
            throw std::runtime_error("Selected target is not a minion");
        }
        CardType ct = card->getCardType();
        if (ct == CardType::SPELL) {
            auto* sp = dynamic_cast<Spell*>(cardPtr.get());
            if (!sp) {
                throw std::logic_error("Not a spell");
            }
            if (!sp->needTarget()) {
                throw std::logic_error("This spell does not take a target");
            }
            // Activate on that target
            sp->activate(*this, tgt);
        } else if (ct == CardType::ENCHANTMENT) {
            dynamic_cast<Enchantment*>(card)->activate(tgt);
        } else {
            throw std::logic_error("Card type cannot be played with a target");
        }
    }
}


Hand* Player::getHand() const {
    return hand.get();
}


Deck* Player::getDeck() const {
    return deck.get();
}


Board* Player::getBoard() const {
    return board.get();
}


Graveyard* Player::getGraveyard() const {
    return graveyard.get();
}


Player* Player::getOpponent() {
    return opponent;
}


bool Player::payMagic(int i) {
    if (i < 0 || i > magic) {
        return false;  // do nothing
    }
    magic -= i;
    return true;
}

void Player::takeDamage(int dmg){
    hp -= dmg;
    std::cout << "Current HP: " << hp << std::endl;
}
int Player::getAttack() const{
    return 0;
}
Player* Player::getOwner() const {
    return nullptr;
}
void Player::gainAttack(int atk) {
    return;
}
bool Player::isDead() {
    return hp <= 0;
}
int Player::getAbilityCost() const{
    return 0;
}
void Player::useAbility(Target *t){
    return;
}
Game *Player::getGame() const {
    return game;
}

