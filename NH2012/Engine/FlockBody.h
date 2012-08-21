#pragma once

#include <vector>
#include "Flock.h"

class Flock;

class FlockBody
{
public:
  FlockBody(Ogre::SceneNode* node, Flock *flock);
  virtual ~FlockBody();

  void frameRenderingQueued(Ogre::FrameEvent& evt);

  //Getters
  Ogre::Vector3 getPosition();
  Ogre::Vector3 getVelocity();
  Ogre::Vector3 getDirection();
  float getSize();

private:
  Flock *flock;
  float size;
  Ogre::SceneNode* node;
  Ogre::Vector3 velocity;
  Ogre::Vector3 direction;

};

