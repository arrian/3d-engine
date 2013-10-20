#pragma once
#include ".\Boids\Boid.h"
#include ".\Boids\Vector.h"

#include "NodeComponent.h"

#include "OgreEntity.h"
#include "OgreBillboard.h"
#include "OgreBillboardSet.h"

class Scene;

class BoidComponent : public Boids::Boid, public NodeComponent
{
public:
  BoidComponent(Boids::Vector position, Boids::Vector velocity, Boids::Vector dimensions);
  virtual ~BoidComponent(void);

  void update(double elapsedSeconds);
protected:
  Ogre::Entity* entity;

  void hasNodeChange();

  Ogre::Billboard* billboard;
  Ogre::BillboardSet* billboardSet;
  //Ogre::Light* light;
};

