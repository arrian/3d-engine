#pragma once


#include <string>

class World;

class WorldLoader
{
public:
  static void load(std::string filename, World* world);

private:
  WorldLoader(void) {}
  virtual ~WorldLoader(void) {}
};

