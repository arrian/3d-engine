#pragma once

#include "Item.h"

class Weapon : public Item
{
public:
  Weapon(ItemDesc desc);
  virtual ~Weapon(void);
};

