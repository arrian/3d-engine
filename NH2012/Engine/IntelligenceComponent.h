#pragma once

#include <vector>

#include <OgreVector3.h>

#include "NodeComponent.h"


/* Artificial intelligence component.*/
class IntelligenceComponent : public NodeComponent
{
public:
  IntelligenceComponent(float speed);
  ~IntelligenceComponent(void);
  
  /*! Finds a path within a scene.*/
  void setGoal(Ogre::Vector3 to);

  void patrol(Ogre::Vector3 from, Ogre::Vector3 to);

  void wander(Ogre::Vector3 around, Ogre::Real distance);

  void find();

  void collect();

  void attack();

  void pickup();

  void open();
  
  Ogre::Vector3 getNextPosition(double elapsedSeconds);

  void update(double elapsedSeconds);

protected:
  bool hasGoal;
  Ogre::Vector3 goal;
  std::vector<Ogre::Vector3> intermediates;

  float speed;

  void hasNodeChange();
};

