#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include "CardCollection.h"
#include "Target.h"
#include <memory>
#include "Subject.h"
#include <string>
#include "Hand.h"
#include "Deck.h"
#include "Board.h"
#include "Graveyard.h"

// forward declarations
class Game;
class Card;
class Hand;
class Deck;
class Board;
class Graveyard;

class Player: public Target, public Subject{
    Game *game; //back pointer to game to access broadcast
    Player *opponent;
    std::unique_ptr<Hand> hand;
    std::unique_ptr<Deck> deck;
    std::unique_ptr<Board> board;
    std::unique_ptr<Graveyard> graveyard;
    std::string deckFile;
    std::string name;
    int magic;
    int hp;

  public:
    Player(std::string name, Game *g, std::string deckFile = "default.deck");
    ~Player();

    void setDeck(std::string deckFile);
    void setName(std::string s);
    std::string getName() const; // for graphic/owner purposes
    int getMagic() const;
    Hand* getHand() const;
    Deck* getDeck() const;
    Board* getBoard() const;
    Graveyard* getGraveyard() const;
    void draw(); // draw card, for testing
    void loadDeck(); // loading the deck
    void discard(int i); // remove card at index i of hand
    void attack(int i); // minion i in board attack opponent's minion j
    void attack(int i, int j); // minion i in board attack opponent
    void useAbility(int i); // use minion i's active ability 
    void useAbility(int i, int pl, int t); // use minion i's active ability on card t by player p
    void setOpponent(Player *opponent);
    Type getType() override; // return Type::PLAYER
    void gainMagic(int i); // use magic == gain negative magic
    void startTurn(); // notify observers
    void endTurn(); // notify observers
    void play(int i); // play card i
    void play(int i, int pl, int t); // play card i at minion j
    bool payMagic(int i);
    Player* getOpponent();
    void takeDamage(int dmg) override;
    int getAttack() const override;
    int getHp() const override;
    Player* getOwner() const override;
    void gainHp(int hp) override;
    void gainAttack(int atk) override;
    bool isDead() override;
    int getAbilityCost() const override;
    void useAbility(Target *t) override;
    Game *getGame() const;
    
};

#endif
