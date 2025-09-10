#ifndef ABILITY_H
#define ABILITY_H
#include "Subject.h"
// Forward declaration of the Target class
class Target;
//class Subject;

class Ability {
  public:
    virtual ~Ability() = default;
    virtual void execute(Target* target) = 0;
};

#endif 
