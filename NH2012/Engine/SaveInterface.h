#pragma once

#include <string>

class SaveInterface
{
public:
  SaveInterface(void);
  virtual ~SaveInterface(void);

  virtual std::string save() = 0;
  virtual void load(std::string state) = 0;
};

