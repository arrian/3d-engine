#include "ItemData.h"


ItemData::ItemData(void)
{
  value = 0;

  attributes = ItemAttributes();
  attributes.buc = ItemAttribute::UNCURSED;
  attributes.type = ItemAttribute::GEM;
}


ItemData::~ItemData(void)
{
}

bool operator==(const ItemData& x, const ItemData& y)
{
  return (x.itemName == y.itemName && x.attributes.buc == y.attributes.buc);//should also check if all attributes are identical
}