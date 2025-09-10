#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include "Player.h"
#include <string>

class Game{
    // variables
    int pn; // 0 = player1, 1 = player2
    std::vector<std::unique_ptr<Player>> plns;
    // for APNAP style
    Player *active;
    Player *inactive;
    bool inGame;
  public:
    Game();
    ~Game() = default;
    void setDeck(int pn, std::string deck);
    void setup();
    void changePlayer(int &pn);
    void processCommand(std::istream &in, bool testing, bool init);
    Player *getActive () const;
    Player *getInactive() const;
    Player *getPlayer1() const;
    Player *getPlayer2() const;
    void broadcast(EventType evt, Event &e, Target *tgt = nullptr);
    void endTurn();
};

#endif
