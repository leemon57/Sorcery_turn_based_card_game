#include "Card.h"
using namespace std;


Card::Card(int cost, const std::string &description, const std::string &name, CardType ct, Player *owner):
    cost{cost}, description{description}, name{name}, ct{ct}, owner{owner} {}


Player* Card::getOwner() const{
    return owner;
}

std::string Card::getDescription() const{
    return description;
}
std::string Card::getName() const{
    return name;
}
int Card::getCost() const{
    return cost;
}
CardType Card::getCardType() const{
    return ct;
}

bool Card::modifiesStats() const { 
    return false; 
}


Card::~Card() { 
    owner = nullptr;
}

