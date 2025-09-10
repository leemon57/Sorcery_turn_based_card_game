#include "Game.h"
#include "EventType.h"
#include "sorcery-asciiart/Display.h"
#include "sorcery-asciiart/TextDisplay.h"
#include <sstream>
using namespace std;

TextDisplay td;

Game::Game(): pn{0}, inGame{true} {
    setup(); // setup the game
}

void Game::setup(){
    try {
        plns.emplace_back(make_unique<Player>("Player 1", this));
        plns.emplace_back(make_unique<Player>("Player 2", this));
        plns[0]->setOpponent(plns[1].get());
        plns[1]->setOpponent(plns[0].get());
        pn = 0;
        active = plns[pn].get();
        inactive = plns[1 - pn].get();
        inGame = true;
    } catch (logic_error &e){
        cerr << e.what() << endl;
    } catch (...){
        cerr << "Uh oh" << endl;
    }
}

void Game::setDeck(int pn, string deck){
    plns[pn]->setDeck(deck);
}

void Game::changePlayer(int &idx){
    idx = 1 - idx;
    pn  = idx;
    active   = plns[pn].get();
    inactive = plns[1 - pn].get();
}

void Game::processCommand(istream &in, bool testing, bool init){
    string cmd, cmdL;
    while (inGame){
        if (!getline(in, cmdL)){
            return;
        }
        istringstream iss (cmdL);
        iss >> cmd;
        // if (in.fail() || in.eof()) break;
        if (cmd == "help"){
            ifstream f{"helptext.txt"};
            string s;
            while (getline(f, s)){
                cout << s << endl;
            }
        } else if (cmd == "end") {
            try {
                endTurn(); // single authoritative turn‑switcher
                cout << getActive()->getName() << "'s turn" << endl;
            } catch (out_of_range &e) {
                cerr << e.what() << endl;
            }
        } else if (cmd == "quit"){
            inGame = false;
            cout << "Quitting the game..." << endl;
        } else if (testing && cmd == "discard"){
            int i = -1;
            try {
                iss >> i;
                plns[pn]->discard(i-1);
            } catch (out_of_range &e){
                cerr << e.what() << endl;
            } 
            if (i == -1){
                cerr << "Check your inputs!" << endl;
            }
        } else if (cmd == "attack"){
            try {
                int i, j, argLen = 0;
                if (iss >> i){
                    ++argLen;
                }
                if (iss >> j){
                    ++argLen;
                }
                if (argLen == 2){
                    plns[pn]->attack(i-1, j-1);
                } else if (argLen == 1){
                    plns[pn]->attack(i-1);
                    if(plns[pn]->getOpponent()->isDead()){
                        if (plns[pn]->isDead()){
                            cout << "Players tie!" << endl;
                        } else {
                            cout << "Player " << plns[pn]->getName() << " wins!" << endl;
                        }
                        inGame = false;
                    }
                } else {
                    cerr << "Number of follow-up inputs does not match expected" << endl;
                }
            } catch (runtime_error &e){
                cerr << e.what() << endl;
            } catch (out_of_range &e){
                cerr << e.what() << endl;
            } catch (...){
                cerr << "Invalid input" << endl;
            }
        } else if (cmd == "play"){
            try {
                int i, j, t, argLen = 0;
                char tt;
                if (iss >> i){
                    ++argLen;
                }
                if (iss >> j){
                    ++argLen;
                }
                if (iss >> tt){
                    ++argLen;
                    if (tt == 'r'){
                        t = -1;
                    } else {
                        t = (tt - '0') - 1;
                    }
                }
                if (argLen == 3){
                    plns[pn]->play(i-1, j-1, t);
                } else if (argLen == 1){
                    plns[pn]->play(i-1);
                } else {
                    cerr << "Number of follow-up inputs does not match expected" << endl;
                }
            } catch (out_of_range &e){
                cerr << e.what() << endl;
            } catch (runtime_error &e){
                cerr << e.what() << endl;
            } catch (logic_error &e){
                cerr << e.what() << endl;
            } catch (...){
                cerr << "Invalid input" << endl;
            }
        } else if (cmd == "use"){ // play but for minion ability
            try {
                int i, j, t, argLen = 0;
                char tt;
                if (iss >> i){
                    ++argLen;
                }
                if (iss >> j){
                    ++argLen;
                    if (j == pn + 1){
                        j = 0; // this player
                    } else {
                        j = 1; // other player
                    }
                }
                if (iss >> tt){
                    ++argLen;
                    if (tt == 'r'){
                        t = -1;
                    } else {
                        t = tt - '0';
                    }
                }
                if (argLen == 3){
                    plns[pn]->useAbility(i-1, j, t);
                } else if (argLen == 1){
                    plns[pn]->useAbility(i-1);
                } else {
                    cerr << "Number of follow-up inputs does not match expected" << endl;
                }
            } catch (out_of_range &e){
                cerr << e.what() << endl;
            } catch (runtime_error &e){
                cerr << e.what() << endl;
            } catch (...){
                cerr << "Invalid input" << endl;
            }
        } else if (cmd == "inspect") {
            int idx;
            if (!(iss >> idx)) {
                cerr << "Please provide an index (e.g., 'inspect 1')\n";
                continue;
            }
            if (idx < 1 || idx > 5) {
                cerr << "Index out of range (choose 1‑5)\n";
            } else {
                auto &slots = *plns[pn]->getBoard()->getCards();
                if (idx - 1 >= static_cast<int>(slots.size())) {
                    cerr << "No minion in that slot\n";
                } else {
                    td.inspectMinion(dynamic_cast<Minion*>(slots[idx - 1].get()));
                }
            }
        } else if (cmd == "hand"){
            td.printHand(*plns[pn]);
        } else if (cmd == "board"){
            td.printBoard(*plns[pn], *plns[1 - pn]);
        } else if (testing && cmd == "draw"){
            try {
                plns[pn]->draw();
            } catch (out_of_range &e){
                cerr << e.what() << endl;
            }
        } else if (init){ // initialize player1 name, then player 2
            // input format must be: P1Name P2Name and must be first two inputs from file
            string temp;
            while (iss >> temp){
                cmd = cmd + " " + temp;
            }
            plns[pn]->setName(cmd);
            changePlayer(pn);
        } else {
            cerr << "Such command does not exist or testing is not enabled." << endl;
        }
        if (testing){
            cout << "----------------------" << endl;
            cout << plns[pn]->getName() << "'s Turn" << endl;
            cout << "Stats: HP=" << plns[pn]->getHp() << " Magic=" << plns[pn]->getMagic() << endl;
            cout << "Hand: " << endl;
            for (int m = 0; m < plns[pn]->getHand()->getSize(); ++m){
                cout << (*(plns[pn]->getHand()->getCards()))[m]->getName() << endl;
            }
            cout << "Board: " << endl;
            for (int m = 0; m < plns[pn]->getBoard()->getSize(); ++m){
                cout << (*(plns[pn]->getBoard()->getCards()))[m]->getName() << " ";
                cout << static_cast<Minion*>(plns[pn]->getBoard()->getCards()->at(m).get())->getAttack() << " ";
                cout << static_cast<Minion*>(plns[pn]->getBoard()->getCards()->at(m).get())->getHp() << endl;
            }
            cout << "----------------------" << endl;
        }
    }
}


Player* Game::getActive() const {
    return active;
}

Player* Game::getInactive() const {
    return inactive;
}

Player* Game::getPlayer1() const {
    return plns[0].get();
}

Player* Game::getPlayer2() const {
    return plns[1].get();
}


void Game::broadcast(EventType evt, Event &e, Target *tgt) {
    // Active player first
    active ->notifyObservers(evt, e, tgt);
    // Non‑active player second
    inactive->notifyObservers(evt, e, tgt);
}



void Game::endTurn()
{
    active->endTurn();               // already sends END_TURN (with tgt=this)
    changePlayer(pn);
    /* START_TURN for the new active player */
    Event e{nullptr, active, active, 0};
    broadcast(EventType::START_TURN, e, active);

    active->startTurn();             // +1 magic, draw, etc.
}
