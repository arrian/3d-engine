#include "Inventory.h"

Inventory::Inventory(void)
{
  items = std::vector<ItemData>();
}

Inventory::Inventory(int numberOfItems)
{
  items = std::vector<ItemData>();

  for(int i = 0; i <= numberOfItems; i++)
  {
    items.push_back(ItemData("rock"));
  }
}

Inventory::~Inventory(void)
{
}

bool Inventory::contains(ItemData item)
{
  for(std::vector<ItemData>::iterator it = items.begin(); it != items.end(); ++it)
  {
    if((*it) == item) return true;
  }
  return false;
}

void Inventory::add(ItemData item)
{
  items.push_back(item);
}

void Inventory::clear()
{
  items = std::vector<ItemData>();
}