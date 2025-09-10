#ifndef EVENT_H
#define EVENT_H


class Player;
class Card;

// this class represents an event in the game.
// It contains information about the subject of the event, the owner of the event,
// the active player, and any additional value associated with the event.
// The subject is the card or entity that triggered the event, the owner is the player
// who owns the subject, the active player is the player whose turn it is,
// value can represent damage, healing etc.

// example of using this class: 
// Event e{ nullptr, currentPlayerPtr, currentPlayerPtr, 0};
// subject.notifyObservers(EventType::StartTurn, e);

class Event {
    Card* subject;
    Player* owner;
    Player* activePlayer;
    int value;

    public:
        Event(Card* subj, Player* own, Player* act, int val);
        Card* getSubject() const;
        Player* getOwner() const;
        Player* getActivePlayer() const;
        int getValue() const;
        void setValue(int val);
        void setSubject(Card* subj);
        void setOwner(Player* own);
        void setActivePlayer(Player* act);
};

#endif
