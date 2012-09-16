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

  int worldRadius;//bounding radius of the world

  Boids::Boid* makeBoid(Boids::Vector dimensions = Boids::Vector(1,.2,.75), double maximumVelocity = 25, double maxAcceleration = 0.65, double cruiseDistance = 0.75);

};

