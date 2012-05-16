#include "Inventory.h"

Inventory::Inventory(void)
  : items()
{
}

Inventory::~Inventory(void)
{
}

bool Inventory::contains(Item* item)
{
  for(std::vector<Item*>::iterator it = items.begin(); it != items.end(); ++it) if(*it == item) return true;
  return false;
}

void Inventory::add(Item* item)
{
  items.push_back(item);
}

void Inventory::clear()
{
  items.clear();
}