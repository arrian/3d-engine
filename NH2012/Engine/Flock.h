#pragma once

#include <vector>
#include <iostream>

#include "Boids/Boid.h"
#include "Boids/Obstacle.h"
#include "Boids/Vector.h"

class Flock
{
public:
  Flock(int numBoids);
  virtual ~Flock(void);

  void update(double elapsedSeconds);

protected:
  std::vector<Boids::Boid*> boids;

};

