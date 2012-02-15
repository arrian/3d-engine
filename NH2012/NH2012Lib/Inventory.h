#pragma once

#include <vector>
#include <iostream>
#include "ItemData.h"

class Inventory
{
public:
  /*! Construct an empty inventory.*/
  Inventory(void);

  /*! Construct an inventory with a given number of random items.*/
  Inventory(int numberOfItems);
  
  virtual ~Inventory(void);

  /*! Checks if inventory contains particular item.*/
  bool contains(ItemData item);

  /*! Adds an item to the inventory.*/
  void add(ItemData item);
private:
  std::vector<ItemData> items;

  /*! Clears the inventory.*/
  void clear();
};

