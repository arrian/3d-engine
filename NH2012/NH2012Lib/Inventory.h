#pragma once

#include <vector>
#include <iostream>
#include "Item.h"

class Inventory
{
public:
  Inventory(void);
  Inventory(int numberOfItems);
  virtual ~Inventory(void);

  bool contains(Item item);
  void add(Item item);
private:
  std::vector<Item> items;

  void clear();
};

