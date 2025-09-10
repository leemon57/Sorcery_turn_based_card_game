  #ifndef TARGET_H
  #define TARGET_H
  #include <string>

  enum class Type {PLAYER, MINION};
  class Player; // forward declaration
  
  class Target{
    public:
      virtual Type getType() = 0;
      virtual ~Target() = default;
      virtual void takeDamage(int dmg) = 0; // apply damage to the target
      virtual int getAttack() const = 0; // get damage for minion reverse attack
      virtual int getHp() const = 0;
      virtual Player* getOwner() const = 0; // return the owner of the target
      virtual void gainHp(int hp) = 0; // apply healing to the target
      virtual void gainAttack(int atk) = 0; // apply magic gain to the target
      virtual bool isDead() = 0;
      virtual int getAbilityCost() const = 0;
      virtual void useAbility(Target *t) = 0;
  };

  #endif


