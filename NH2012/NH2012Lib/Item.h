#pragma once

#include <OgreString.h>

#include "Entity.h"
#include "Attributes.h"

class Item : public Entity
{
public:
  Item(void);
  virtual ~Item(void);

  friend bool operator==(const Item& x, const Item& y);
private:
  int value;

  ItemAttributes attributes;
};

