#pragma once

/**
 * Generates landscape (2D Perlin noise) or natural caves(3D Perlin noise) meshes 
 */
class MeshGenerator
{
public:
  MeshGenerator(void);
  MeshGenerator(int xSize, int ySize);//landscapes
  MeshGenerator(int xSize, int ySize, int zSize);//natural caves
  ~MeshGenerator(void);
};

