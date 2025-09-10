#include "TextDisplay.h"
#include "ascii_graphics.h"
#include "../Game.h"
#include "../Player.h"
#include "../Board.h"
#include "../Card.h"
#include "../Minion.h"
#include "../Ritual.h"
#include "../Enchantment.h"
#include <iostream>
using namespace std;

static const size_t CARD_HEIGHT = CARD_TEMPLATE_BORDER.size();

vector<string> TextDisplay::emptySlot() { 
    return CARD_TEMPLATE_BORDER; 
}

static pair<string, string> buffStrings(const Enchantment *e) {
    int atk = e->getAttackMod();   
    int def = e->getHpMod();
    bool isMult = e->getMult();

    // convert the mods into string so it can be used in the template
    string a, d;
    if (atk) {
      if (isMult) {
        a = "*" + to_string(atk);
      } else {
        a = (atk > 0 ? "+" : "") + to_string(atk);
      }
    }
    if (def) {
      if (isMult) {
        a = "*" + to_string(def);
      } else {
        a = (def > 0 ? "+" : "") + to_string(def);
      }
    }
    return {a,d};
}


vector<string> TextDisplay::renderCard(const Card *c) {
    switch (c->getCardType()) {
        case CardType::MINION: {
            auto *m = dynamic_cast<const Minion*>(c);
            if (m-> getActivatedAbility())
                return display_minion_activated_ability(
                    c->getName(),c->getCost(),
                    m->getAttack(),m->getDefence(),
                    m->getAbilityCost(),m->getDescription());
            else if (m->getTriggeredAbility())
                return display_minion_triggered_ability(
                    c->getName(),c->getCost(),
                    m->getAttack(),m->getDefence(),
                    m->getDescription());
            else
                return display_minion_no_ability(
                    c->getName(),c->getCost(),
                    m->getAttack(),m->getDefence());
        }

        case CardType::SPELL:
            return display_spell(c->getName(),c->getCost(),
                                 c->getDescription());
        case CardType::ENCHANTMENT: {
            auto *e = dynamic_cast<const Enchantment*>(c);
            auto [atkStr,defStr] = buffStrings(e);
            if (!atkStr.empty() || !defStr.empty())
                return display_enchantment_attack_defence(
                    c->getName(),c->getCost(),
                    c->getDescription(),atkStr,defStr);
            else
                return display_enchantment(
                    c->getName(),c->getCost(),
                    c->getDescription());
        }
        case CardType::RITUAL: {
            auto *r = dynamic_cast<const Ritual*>(c);
            return display_ritual(
                c->getName(),c->getCost(),
                r->getActivationCost(),
                c->getDescription(),
                r->getCharges());
        }
        default: break;
    }
    return emptySlot();
}


static void printRow(const vector<vector<string>> &row) {
    for (size_t line=0; line < CARD_HEIGHT; ++line) {
        std::cout << "│"; 
        for (auto &frame : row) {
          cout<<frame[line];
        }
        std::cout << "│" << '\n'; 
    }
}


void TextDisplay::printHand(const Player &p) {
    vector<vector<string>> frames;
    for (auto &u : *p.getHand()->getCards()){
      frames.push_back(renderCard(u.get()));
    }
      
    printRow(frames);
}


void TextDisplay::printBoard(const Player &top,const Player &bot) {
    static const string OUTER = "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------";

     cout << OUTER << '\n';

    // top row: ritual – hero – graveyard
    std::vector<std::vector<std::string>> heroTop(5, CARD_TEMPLATE_BORDER);
    heroTop[0] = top.getBoard()->getRitual() ?
                 renderCard(top.getBoard()->getRitual().get()) : emptySlot();
    heroTop[2] = display_player_card(1, top.getName(),
                                     top.getHp(), top.getMagic());
    heroTop[4] = top.getGraveyard()->top() ?
                 renderCard(top.getGraveyard()->top()) : emptySlot();
    printRow(heroTop);

    // top minions 
    vector<vector<string>> rowMinTop;
    for (auto &u:*top.getBoard()->getCards()) {
      rowMinTop.push_back(renderCard(u.get()));
    }
    while (rowMinTop.size() < 5) {
      rowMinTop.push_back(emptySlot());
    }
    printRow(rowMinTop);

    // separator 
    for (auto &ln : CENTRE_GRAPHIC)
        std::cout << ln << '\n';

    // bottom minions 
    vector<vector<string>> rowMinBot;
    for (auto &u:*bot.getBoard()->getCards()) {
      rowMinBot.push_back(renderCard(u.get()));
    }
    while(rowMinBot.size() < 5) {
      rowMinBot.push_back(emptySlot());
    }
    printRow(rowMinBot);

    // bottom row: ritual – hero – graveyard 
    vector<vector<string>> row2{
        bot.getBoard()->getRitual() ?
            renderCard(bot.getBoard()->getRitual().get()) : emptySlot(),
        display_player_card(2,bot.getName(),bot.getHp(),bot.getMagic()),
        bot.getGraveyard()->top() ?
            renderCard(bot.getGraveyard()->top()) : emptySlot()
    };
    std::vector<std::vector<std::string>> heroBot(5, CARD_TEMPLATE_BORDER);
    heroBot[0] = bot.getBoard()->getRitual().get() ?
                 renderCard(bot.getBoard()->getRitual().get()) : emptySlot();
    heroBot[2] = display_player_card(2, bot.getName(),
                                     bot.getHp(), bot.getMagic());
    heroBot[4] = bot.getGraveyard()->top() ?
                 renderCard(bot.getGraveyard()->top()) : emptySlot();
    printRow(heroBot);

     cout << OUTER << '\n';
}


void TextDisplay::inspectMinion(Minion *m) {
    if (!m) { 
      cerr << "Not a minion\n"; 
      return; 
    }
    // print the minion frame itself
    for (auto &line : renderCard(m)) {
      cout << line << '\n';
    }

    // walk decorator chain: oldest → newest (inner to outer) 
    Minion *cur = m;
    vector<const Decorator*> stack;
    while (auto *dec = dynamic_cast<Enchantment*>(cur)) {
        stack.push_back(dec);
        cur = dec->removeEnchantment();
    }
    reverse(stack.begin(), stack.end());

    vector<vector<string>> row;
    int col = 0;
    for (auto *dec : stack) {
        row.push_back(renderCard(dec));
        if (++col == 5) { 
          printRow(row); row.clear(); col = 0; 
        }
    }
    if (!row.empty()) {
      printRow(row);
    }
}

