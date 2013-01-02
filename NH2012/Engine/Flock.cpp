#include "Flock.h"

#include "Scene.h"
#include "BoidComponent.h"

//-------------------------------------------------------------------------------------
Flock::Flock(int numBoids)
  : BasicComponent(),
    boids(),
    worldRadius(20),
    totalElapsed(0.0f)
{
  for(int i = 0; i < numBoids; i++) addBoid();

  Boids::Sphere* boundingSphere = new Boids::Sphere(Boids::Vector(0,0,0), worldRadius);
  boids[0]->obstacles.Add(*boundingSphere);
}

//-------------------------------------------------------------------------------------
Flock::~Flock(void)
{
  for(std::vector<BoidComponent*>::iterator iter = boids.begin(); iter < boids.end(); ++iter)
  {
    if(*iter) delete (*iter);
  }
}

//-------------------------------------------------------------------------------------
void Flock::update(double elapsedSeconds)
{
  totalElapsed += elapsedSeconds;
  for(std::vector<BoidComponent*>::iterator iter = boids.begin(); iter < boids.end(); ++iter)
  {
    ++iter;
    if(iter >= boids.end()) iter = boids.begin();
    (*iter)->update(totalElapsed);
  }
}

//-------------------------------------------------------------------------------------
BoidComponent* Flock::makeBoid(Boids::Vector dimensions, double maximumVelocity, double maxAcceleration, double cruiseDistance)
{
  Boids::Vector position;
  Boids::Vector attitude;//roll, pitch, yaw
  Boids::Vector velocity;

  //Set up attitude, position and velocity.
  attitude = Boids::Vector(std::rand() % int(worldRadius), 0.5, std::rand() % int(worldRadius));
  velocity  = Direction(attitude) * (maximumVelocity) / 4.0;
  position = Boids::Vector(std::rand() % int(worldRadius) - 0.5 * worldRadius - worldRadius * 0.025, 0, std::rand() % int(worldRadius) - 0.5 * worldRadius - worldRadius * 0.025);

  BoidComponent* boid = new BoidComponent(position, velocity, dimensions);

  boid->maxAcceleration = maxAcceleration;
  boid->cruiseDistance = cruiseDistance;
  boid->maxVelocity = maximumVelocity;

  return boid;
}

//-------------------------------------------------------------------------------------
void Flock::hasSceneChange()
{
  if(oldScene && node) oldScene->getSceneManager()->destroySceneNode(node);
  node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();

  for(std::vector<BoidComponent*>::iterator iter = boids.begin(); iter < boids.end(); ++iter)
  {
    (*iter)->setNode(scene, node->createChildSceneNode());
  }
}

//-------------------------------------------------------------------------------------
void Flock::addBoid()
{
  boids.push_back(makeBoid());
}