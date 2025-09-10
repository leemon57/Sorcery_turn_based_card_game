#include "Event.h"


Event::Event(Card* subj, Player* own, Player* act, int val)
    : subject{subj}, owner{own}, activePlayer{act}, value{val} {}

    
Card* Event::getSubject() const {
    return subject;
}


Player* Event::getOwner() const {
    return owner;
}


Player* Event::getActivePlayer() const {
    return activePlayer;
}


int Event::getValue() const {
    return value;
}


void Event::setValue(int val) {
    value = val;
}


void Event::setSubject(Card* subj) {
    subject = subj;
}


void Event::setOwner(Player* own) {
    owner = own;
}


void Event::setActivePlayer(Player* act) {
    activePlayer = act;
}

