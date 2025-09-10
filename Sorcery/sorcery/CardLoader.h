#ifndef CARDLOADER_H
#define CARDLOADER_H
#include <iostream>
#include <memory>
#include "Player.h"
#include "Card.h"
#include "Deck.h"

class CardLoader {
  public:
    static std::unique_ptr<Card> loadCard(std::string source, Player *owner);
    static std::unique_ptr<Deck> loadDeck(std::string deckFile, Player *owner);
};

#endif
