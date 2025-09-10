#ifndef DECORATOR_H
#define DECORATOR_H
#include "Minion.h"
#include <string>

class Decorator: public Minion {
 protected:
  Minion *minion;
 public:
  Decorator(Minion *minion, std::string name, std::string desc, int cost, Player *owner);
  virtual ~Decorator() = 0;
};

#endif
