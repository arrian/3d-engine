#pragma once
#include ".\Boids\Boid.h"
#include ".\Boids\Vector.h"

#include "NodeComponent.h"

#include "OgreEntity.h"
#include "OgreBillboard.h"
#include "OgreBillboardSet.h"

class Scene;

class BoidExtension : public Boids::Boid, public NodeComponent
{
public:
  BoidExtension(Boids::Vector position, Boids::Vector velocity, Boids::Vector dimensions);
  virtual ~BoidExtension(void);

  void update(double elapsedSeconds);
protected:
  Ogre::Entity* entity;

  void hasNodeChange();

  Ogre::Billboard* billboard;
  Ogre::BillboardSet* billboardSet;
  //Ogre::Light* light;
};

