#pragma once

#include <vector>
#include <iostream>
#include "ItemData.h"

class Inventory
{
public:
  Inventory(void);
  Inventory(int numberOfItems);
  virtual ~Inventory(void);

  bool contains(ItemData item);
  void add(ItemData item);
private:
  std::vector<ItemData> items;

  void clear();
};

