#pragma once

#include <iostream>
#include "Item.h"

class Inventory
{
public:
  Inventory(void);
  Inventory(int numberOfItems);
  virtual ~Inventory(void);

  bool contains(Item item);
private:
  void clear();
};

