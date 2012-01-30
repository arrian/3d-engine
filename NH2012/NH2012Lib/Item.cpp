#include "Item.h"


Item::Item(void) : Entity()
{
  value = 0;
  name = "rock";

  attributes = ItemAttributes();

  attributes.buc = ItemAttribute::UNCURSED;
  attributes.type = ItemAttribute::GEM;
}


Item::~Item(void)
{
}

bool operator==(const Item& x, const Item& y)
{
  return (x.name == y.name);//should also check if all attributes are identical
}