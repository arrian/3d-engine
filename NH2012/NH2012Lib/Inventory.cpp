#include "Inventory.h"

//no items
Inventory::Inventory(void)
{
  items = std::vector<ItemData>();
}

//defined number of random items
Inventory::Inventory(int numberOfItems)
{
  items = std::vector<ItemData>();

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

//clears the inventory
void Inventory::clear()
{
  items = std::vector<ItemData>();//delete object refs also!
}