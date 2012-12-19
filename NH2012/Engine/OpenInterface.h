#pragma once
class OpenInterface
{
public:
  virtual ~OpenInterface(void) {};

  virtual void setOpen(bool open) = 0;
  virtual bool isOpen() = 0;

  virtual void setLocked(bool locked) = 0;
  virtual void isLocked() = 0;
};

