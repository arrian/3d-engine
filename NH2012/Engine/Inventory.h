#pragma once

#include <vector>
#include <iostream>

class Item;

class Inventory
{
public:
  /*! Construct an empty inventory.*/
  Inventory(void);
  
  virtual ~Inventory(void);

  /*! Checks if inventory contains particular item.*/
  bool contains(Item* item);

  /*! Adds an item to the inventory.*/
  void add(Item* item);
private:
  std::vector<Item*> items;

  /*! Clears the inventory.*/
  void clear();
};

