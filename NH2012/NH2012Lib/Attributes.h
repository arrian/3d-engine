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
    FLAYS_MINDS,
    BITES,
    TELEPORT //quantum mechanic
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

  enum Type
  {
    ANIMAL,
    HUMAMOID,
    WERE_CREATURE,
    UNDEAD
  };

  enum Gender
  {
    MALE,
    FEMALE,
    UNDEFINED
  };

  enum LikesDislikes
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
    TEA,
    PINA_COLADAS
  };

  enum Size
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
    LIGHT,
    SMOKE
  };

  enum Status
  {
    NONE,
    KING,
    PRINCE,
    LORD,
    MINION,
    DEMON,
    CROWNED,
    PLAYER
  };
}

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
}

namespace GeneralAttribute
{
  
  enum Conduct
  {
    OMNIVOR,
    HERBIVOR,
    CARNIVOR,
    METALLIVOR,
    FOODLESS,
    ATHEIST,
    PACIFIST,
    ILLITERATE,
    NO_MELEE,
    NO_GENOCIDE,
    NO_POLYMORPH,
    NO_WISHES
  };

  enum FoodSafety
  {
    SAFE,
    POISONOUS,
    VEGETARIAN,
    VEGAN
  };

  enum Food
  {
    SATIATED,
    SATISFIED,
    HUNGRY,
    WEAK,
    FAINTING
  };

  enum Intrinsics
  {
    AGGRAVATE_MONSTER,
    COLD_RESISTANCE,
    CONFLICT,
    FIRE_RESISTANCE,
    FREE_ACTION,
    HUNGER,
    INVISIBILITY,
    LEVITATION,
    POISON_RESISTANCE,
    POLYMORPH_CONTROL,
    POLYMORPHITIS,
    PROTECTION,
    REGENERATION,
    SEARCHING,
    SEE_INVISIBLE,
    SHOCK_RESISTANCE,
    SLOW_DIGESTION,
    STEALTH,
    SUSTAIN_ABILITY,
    TELEPORT_CONTROL,
    TELEPORITIS,
    WARNING,
    INCREASE_ACCURACY,
    INCREASE_DAMAGE,
    ADORNMENT,
    ASTRAL_VISION,
    BLINDING_RESISTANCE,
    CLAIRVOYANCE,
    DISPLACEMENT,
    FLYING,
    FOOD_APPRAISAL,
    FUMBLING,
    HALLUCINATION_RESISTANCE,
    INFRAVISION,
    INVULNERABILITY,
    LEVEL_DRAIN_RESISTANCE,
    LIFE_SAVING,
    LYCANTHROPY,
    MAGICAL_BREATING,
    MONSTER_DETECTION,
    PHASING,
    REFLECTION,
    SICKNESS_RESISTANCE,
    SLEEP_RESISTANCE,
    SPEED,
    STONING_RESISTANCE,
    SWIMMING,
    TELEPATHY,
    UNBREATHING,
    UNCHANGING,
    WATER_WALKING
  };

  enum Water
  {
    DROWNING,
    QUENCHED,
    THIRSTY,
    PARTCHED
  };
}

struct MonsterAttributes
{
public:
  MonsterAttributes(void);
  ~MonsterAttributes(void);

  std::vector<MonsterAttribute::Ability> abilities;
  MonsterAttribute::Attack attack;
  MonsterAttribute::Awareness awareness;
  MonsterAttribute::Behaviour behaviour;
  std::vector<MonsterAttribute::Body> body;
  std::vector<MonsterAttribute::Type> types;
  std::vector<MonsterAttribute::LikesDislikes> dislikes;
  MonsterAttribute::Gender gender;
  std::vector<MonsterAttribute::LikesDislikes> likes;
  MonsterAttribute::Size size;
  std::vector<MonsterAttribute::Produces> produces;
  std::vector<GeneralAttribute::Intrinsics> intrinsics;
  MonsterAttribute::Status status;
};

struct ItemAttributes
{
public:
  ItemAttributes(void);
  ~ItemAttributes(void);

  ItemAttribute::BUC buc;
  ItemAttribute::Status status;
  ItemAttribute::Type type;
  std::vector<GeneralAttribute::Intrinsics> intrinsics;
};