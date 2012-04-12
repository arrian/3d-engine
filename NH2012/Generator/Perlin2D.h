#pragma once

#include <OgreMath.h>

/**
 * Generates landscape meshes (2D Perlin noise) 
 */
class Perlin2D
{
public:
  Perlin2D(int xSize, int ySize);
  ~Perlin2D(void);

private:
  int xSize;
  int ySize;

  Ogre::Real generatePoint(Ogre::Real x, Ogre::Real y);
  void blur();
};

