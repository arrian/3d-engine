#pragma once

#include "Interactive.h"

class Chest : public Interactive
{
public:
  Chest(void);
  ~Chest(void);

  void setOpen(bool open);
  void setLocked(bool locked);

  bool isOpen();
  bool isLocked();

  virtual void interact();

private:
  bool open;
  bool locked;
};

