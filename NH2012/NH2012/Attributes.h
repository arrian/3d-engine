#pragma once

#include <vector>

namespace MonsterAttribute
{
  enum Ability
  {
    FLIES,
    FLOATS,
    CLINGS,
    ENGULFS,
    SWIMS,
    HIDES,
    REGENERATES
  };

  enum Attack
  {
    NORMAL,
    PETRIFIES,
    FLAYS_MINDS
  };

  enum Awareness
  {
    DEAD,
    STUNNED,
    HALLUCINATING,
    SLEEPING
  };

  enum Behaviour
  {
    AGGRESSIVE,
    WANDERER,
    MINDLESS,
    PEACEFUL,
    PET
  };

  enum Body
  {
    NO_EYES,
    NO_HANDS,
    NO_LIMBS,
    NO_HEAD
  };

  enum Conduct
  {
    OMNIVOR,
    HERBIVOR,
    CARNIVOR,
    METALLIVOR
  };

  enum Corpse
  {
    SAFE,
    POISONOUS,
    VEGETARIAN,
    VEGAN
  };

  enum Creature
  {
    ANIMAL,
    HUMAMOID,
    WERE_CREATURE,
    UNDEAD
  };

  enum Dislikes
  {

  };

  enum Food
  {
    SATIATED,
    SATISFIED,
    HUNGRY,
    WEAK,
    FAINTING
  };

  enum Gender
  {
    MALE,
    FEMALE,
    UNDEFINED
  };

  enum Likes
  {
    GOLD,
    MAGIC,
    GEMS,
    ITEMS,
    FOOD,
    WATER,
    LAVA,
    ROCK,
    METAL,
    PINA_COLADAS
  };

  enum MonsterSize
  {
    GIANT,
    LARGE,
    AVERAGE,
    SMALL,
    TINY
  };

  enum Produces
  {
    WEB,
    STRUCTURES,
    TUNNELS,
    EGGS,
    LIGHT
  };

  enum Resistance
  {
    SLEEP,
    FIRE,
    COLD,
    LIGHTNING,
    STONING,
    POISON
  };

  enum Status
  {
    NONE,
    KING,
    PRINCE,
    LORD,
    MINION,
    DEMON,
    CROWNED
  };

  enum Water
  {
    DROWNING,
    QUENCHED,
    THIRSTY,
    PARTCHED
  };
}

class MonsterAttributes
{
public:
  MonsterAttributes(void);
  ~MonsterAttributes(void);

  std::vector<MonsterAttribute::Ability> abilities;
  MonsterAttribute::Attack attack;
  MonsterAttribute::Awareness awareness;
  MonsterAttribute::Behaviour behaviour;
  std::vector<MonsterAttribute::Body> body;
  MonsterAttribute::Conduct conduct;
  MonsterAttribute::Corpse corpse;
  std::vector<MonsterAttribute::Creature> types;
  std::vector<MonsterAttribute::Dislikes> dislikes;
  MonsterAttribute::Gender gender;
  std::vector<MonsterAttribute::Likes> likes;
  MonsterAttribute::MonsterSize size;
  std::vector<MonsterAttribute::Produces> produces;
  std::vector<MonsterAttribute::Resistance> resistances;
  MonsterAttribute::Status status;
};

namespace ItemAttribute
{
  enum BUC
  {
    BLESSED,
    UNCURSED,
    CURSED
  };

  enum Status
  {
    NORMAL,
    UNIQUE,
    ARTIFACT
  };

  enum Type
  {
    POTION,
    SCROLL,
    SPELLBOOK,
    MELEE_WEAPON,
    RANGED_WEAPON,
    GEM,
    GOLD,
    PICK,
    HELMET,
    BOOTS,
    ARMOUR,
    KEY,
    LAMP,
    FOOD,
    WATER,
    WAND,
    RING,
    CONTAINER
  };

  enum Intrinsics
  {

  };
}

class ItemAttributes
{
public:
  ItemAttributes(void);
  ~ItemAttributes(void);

  ItemAttribute::BUC buc;
  ItemAttribute::Status status;
  ItemAttribute::Type type;
  std::vector<ItemAttribute::Intrinsics> intrinsics;
};