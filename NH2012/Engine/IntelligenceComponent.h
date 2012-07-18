#pragma once

#include <vector>

#include <OgreVector3.h>

#include "NodeComponent.h"


/* Artificial intelligence component.*/
class IntelligenceComponent : public NodeComponent
{
public:
  IntelligenceComponent(void);
  ~IntelligenceComponent(void);
  
  /*! Finds a path within a scene.*/
  void go(Ogre::Vector3 to);

  void patrol(Ogre::Vector3 from, Ogre::Vector3 to);

  void wander(Ogre::Vector3 around, Ogre::Real distance);

  void find();

  void collect();

  void attack();

  void pickup();

  void open();
  
  Ogre::Vector3 getNextPosition();

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

protected:
  std::queue<Ogre::Vector3> points;

  void hasNodeChange();
};

