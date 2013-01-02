#pragma once

#include <vector>
#include <iostream>

#include "OgreEntity.h"
#include "OgreSceneNode.h"

#include "BasicComponent.h"

#include "Boids/Boid.h"
#include "Boids/Obstacle.h"
#include "Boids/Vector.h"

class Scene;
class BoidComponent;

class Flock : public BasicComponent
{
public:
  Flock(int numBoids);
  virtual ~Flock(void);

  void update(double elapsedSeconds);

  void addBoid();

protected:
  Ogre::SceneNode* node;

  std::vector<BoidComponent*> boids;

  int worldRadius;//bounding radius of the world

  void hasSceneChange();

  BoidComponent* makeBoid(Boids::Vector dimensions = Boids::Vector(1,.2,.75), double maximumVelocity = 40, double maxAcceleration = 3, double cruiseDistance = 0.4);//move construction to the BoidComponent
};

