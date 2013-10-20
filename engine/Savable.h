#pragma once

#include <string>

class Savable
{
public:
  Savable(void);
  virtual ~Savable(void);

  virtual std::string save() = 0;
  virtual void load(std::string state) = 0;
};

