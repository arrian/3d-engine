#include "Item.h"


Item::Item(void)
{
  name = "rock";

  attributes = ItemAttributes();

  attributes.buc = ItemAttribute::CURSED;
  attributes.type = ItemAttribute::GEM;
}


Item::~Item(void)
{
}

bool operator==(const Item& x, const Item& y)
{
  return (x.name == y.name);//should also check if all attributes are identical
}