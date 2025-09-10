#ifndef GRAPHICALDISPLAY_H
#define GRAPHICALDISPLAY_H
#include "Display.h"
#include "Game.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>



class GraphicalDisplay : public Display {
    public:
        void displayGame(const Game& game) override;
        void displayPlayer(const Game& game, int playerIndex) override;
        void displayCard(const Game& game, int playerIndex, int cardIndex) override;

};

#endif // GRAPHICALDISPLAY_H