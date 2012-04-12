#pragma once

#include <OgreMath.h>

/**
 * Generates cave meshes (3D Perlin noise) 
 */
class Perlin3D
{
public:
  Perlin3D(int xSize, int ySize, int zSize);
  ~Perlin3D(void);

private:
  int xSize;
  int ySize;
  int zSize;

  Ogre::Real generatePoint(Ogre::Real x, Ogre::Real y, Ogre::Real z);
  void blur();
};

