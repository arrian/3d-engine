#include "Flock.h"


Flock::Flock(int numBoids)
  : boids(),
    worldRadius(30)
{
  for(int i = 0; i < numBoids; i++) boids.push_back(makeBoid());
}


Flock::~Flock(void)
{
  for(std::vector<Boids::Boid*>::iterator iter = boids.begin(); iter < boids.end(); ++iter)
  {
    if(*iter) delete (*iter);
  }
}

void Flock::update(double elapsedSeconds)
{
  for(std::vector<Boids::Boid*>::iterator iter = boids.begin(); iter < boids.end(); ++iter)
  {
    (*iter)->update(elapsedSeconds);
    std::cout << "boid: " << (*iter)->getPosition() << std::endl;
  }
}

Boids::Boid* Flock::makeBoid(Boids::Vector dimensions, double maximumVelocity, double maxAcceleration, double cruiseDistance)
{
  Boids::Vector position;
  Boids::Vector attitude;//roll, pitch, yaw
  Boids::Vector velocity;

  //int unrandomTemp = 10;

  

  //Set up attitude, position and velocity.
  attitude = Boids::Vector(std::rand() % int(worldRadius), 0.5, std::rand() % int(worldRadius));
  velocity  = Direction(attitude) * (maximumVelocity)/4.0;
  position = Boids::Vector(std::rand() % int(worldRadius) - 0.5 * worldRadius - worldRadius * 0.025, 0, std::rand() % int(worldRadius) - 0.5 * worldRadius - worldRadius * 0.025);

  Boids::Boid* boid = new Boids::Boid(position, velocity, dimensions);

  boid->maxAcceleration = maxAcceleration;
  boid->cruiseDistance = cruiseDistance;
  boid->maxVelocity = maximumVelocity;

  return boid;
}
