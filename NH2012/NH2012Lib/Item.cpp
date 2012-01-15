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
