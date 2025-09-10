#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H
#include "Display.h"
#include "../Game.h"
#include "../Player.h"
#include "../Card.h"
#include <vector>
#include <string>


class TextDisplay : public Display {
    public:
        TextDisplay()  = default;
        ~TextDisplay() override = default;
        std::vector<std::string> renderCard(const Card *c);
        // One completely blank 11‑line slot 
        std::vector<std::string> emptySlot();
        // print the active player’s hand (row of cards) 
        void printHand(const Player &p);
        // print the entire board: top = p1, bottom = p2 
        void printBoard(const Player &top, const Player &bottom);
        // Show a minion and all its enchantments (inspect command)
        void inspectMinion(Minion *m);
};


#endif