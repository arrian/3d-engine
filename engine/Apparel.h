#pragma once

#include "Item.h"

class Apparel : public Item
{
public:
  Apparel(ItemDesc desc);
  virtual ~Apparel(void);
};

