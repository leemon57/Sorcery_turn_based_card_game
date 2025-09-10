#include "Decorator.h"
using namespace std;


Decorator::Decorator(Minion *m, string name, string desc, int cost, Player *owner): 
Minion{name, desc, cost, owner}, minion{m} {}

Decorator::~Decorator() { delete minion; }

