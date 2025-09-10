#include "CardCollection.h"
#include <stdexcept>
using namespace std;

CardCollection::~CardCollection(){
    cards.clear();
}

vector<unique_ptr<Card>> *CardCollection::getCards(){
    return &cards;
}


// for read-only access
const vector<std::unique_ptr<Card>>* CardCollection::getCards() const {
    return &cards;
}

int CardCollection::getSize(){
    return static_cast<int>(cards.size());
}

unique_ptr<Card> CardCollection::remove(int i){
    if (cards.empty()){
        throw out_of_range("Deck is empty.");
    }
    unique_ptr<Card> c;
    bool erased = false;
    for (int m = 0; m < getSize(); ++m){
        if (m == i){
            c = move(cards[i]);
            cards.erase(cards.begin() + i);
            erased = true;
            break;
        }
    }
    if (erased){
        return c;
    } else {
        throw out_of_range("Card index out of range.");
    }
}

Card* CardCollection::get(int i) const {
    if (i < 0 || i >= static_cast<int>(cards.size())) {
        throw std::out_of_range("Index out of range.");
    }
    return cards[i].get(); // just give a non-owning pointer
}


void CardCollection::add(unique_ptr<Card> c){
    cards.emplace_back(move(c));
}





