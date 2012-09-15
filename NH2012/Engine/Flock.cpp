#include "Flock.h"


Flock::Flock(int numBoids)
  : boids()
{
  for(int i = 0; i < numBoids; i++)
  {
    boids.push_back(new Boids::Boid(Boids::Vector(0,0,0),Boids::Vector(0,0,0),Boids::Vector(1,1,1)));
  }
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
