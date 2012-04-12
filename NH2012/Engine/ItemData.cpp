#include "ItemData.h"


ItemData::ItemData(void)
{
  value = 0;

  name = "rock";

  buc = ItemAttribute::UNCURSED;
  type = ItemAttribute::GEM;
}

ItemData::ItemData(Ogre::String name)
{

}

ItemData::~ItemData(void)
{
}

bool operator==(const ItemData& x, const ItemData& y)
{
  return (x.name == y.name && x.buc == y.buc);//TODO should also check if all attributes are identical
}