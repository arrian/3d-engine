#include "MonsterData.h"


MonsterData::MonsterData(void)
{
}


MonsterData::~MonsterData(void)
{
}

bool operator==(const MonsterData& x, const MonsterData& y)
{
  return (x.name == y.name);
}

