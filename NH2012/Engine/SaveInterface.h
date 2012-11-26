#pragma once
class SaveInterface
{
public:
  SaveInterface(void);
  virtual ~SaveInterface(void);

  std::string save() = 0;
  void load(std::string state) = 0;
};

