#include "MonsterData.h"


MonsterData::MonsterData(void)
  : health(10),
    magic(10),
    level(1),
    speed(200)
{
}


MonsterData::~MonsterData(void)
{
}

bool operator==(const MonsterData& x, const MonsterData& y)
{
  return (x.name == y.name);
}

