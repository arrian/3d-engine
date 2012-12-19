#pragma once

#include "InteractiveInterface.h"

class Door : public InteractiveInterface
{
public:
  Door(void);
  virtual ~Door(void);

  
  void setOpen(bool open);
  void setLocked(bool locked);

  bool isOpen();
  bool isLocked();

  void interact();

private:
  bool open;
  bool locked;

};

