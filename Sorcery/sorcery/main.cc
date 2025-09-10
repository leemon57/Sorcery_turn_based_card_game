#include <iostream>
#include <fstream>
#include "Game.h"
#include <memory>
#include <string>
using namespace std;


int main(int argc, char *argv[]){
    std::unique_ptr<Game> g (new Game);
    bool testing = false;
    // processing command line arguments
    if (argc >= 2){
        for (int i = 1; i < argc; ++i){
            string c = argv[i];
            if (c == "-deck1"){
                ++i;
                cout << "set deck 1 " << argv[i] << endl;
                g->setDeck(0, argv[i]);
            } else if (c == "-deck2"){  
                ++i;
                cout << "set deck 2 " << argv[i] << endl;
                g->setDeck(1, argv[i]);
            } else if (c == "-init"){
                cout << "init" << endl;
                ++i;
                string initfile = argv[i];
                {
                    // process file inputs
                    try {
                        ifstream f{initfile};
                        g->processCommand(f, testing, true);
                    } catch (...){
                        cerr << "File dne or cannot be opened" << endl;
                    }
                }
            } else if (c == "-testing"){
                cout << "testing enabled" << endl;
                testing = true;
            } else if (c == "-graphics"){
                cout << "no graphics yet" << endl;
            }
        }
    }

    //processing standard input arguments
    g->processCommand(cin, testing, false);
    
}
