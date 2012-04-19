#include "ItemData.h"


ItemData::ItemData(void)
{
  value = 0;

  name = "rock";

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
  return (x.name == y.name);//TODO should also check if all attributes are identical
}