#include "Inventory.h"

//no items
Inventory::Inventory(void)
{
  items = std::vector<Item>();
}

//defined number of random items
Inventory::Inventory(int numberOfItems)
{
  items = std::vector<Item>();

  for(int i = 0; i <= numberOfItems; i++)
  {
    //Need internal inventory item representation
    //items.push_back(Item());
  }
}

Inventory::~Inventory(void)
{
}

//check if inventory contains particular item
bool Inventory::contains(Item item)
{
  for(std::vector<Item>::iterator it = items.begin(); it != items.end(); ++it)
  {
    if((*it) == item) return true;
  }
  return false;
}

void Inventory::add(Item item)
{
  items.push_back(item);
}

//clears the inventory
void Inventory::clear()
{
  items = std::vector<Item>();//delete object refs also!
}