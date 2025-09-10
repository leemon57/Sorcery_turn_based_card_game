#ifndef DISPLAY_H
#define DISPLAY_H
#include <iostream>
#include <string>
#include "ascii_graphics.h"



class Game;

class Display {
    public:
        virtual ~Display() = default;
        // Draw the current game state (called after every command). 
};

#endif
