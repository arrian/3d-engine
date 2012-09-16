#pragma once

#include <OgreFrameListener.h>
#include <OgreString.h>
#include <OgreVector3.h>

#include "BasicComponent.h"
#include "HumanoidSkeletonComponent.h"
#include "IntelligenceComponent.h"
#include "VisualComponent.h"
#include "Bar.h"

class Scene;

class Monster : public BasicComponent, public PhysicalInterface
{
public:
  Monster(int id);
  virtual ~Monster(void);

  void update(double elapsedSeconds);
  //void frameRenderingQueued(const Ogre::FrameEvent& evt);

  void setPosition(Ogre::Vector3 position);
  void setTarget(Ogre::Vector3 target);//Sets the monsters target position.

  friend bool operator==(const Monster& x, const Monster& y);

protected:
  Ogre::SceneNode* node;
  Ogre::Vector3 position;
  Ogre::Vector3 target;
  float speed;//Number of units to move per second ~100 = 1 metre per second.
  float height;//Height of the actor in units.

  IntelligenceComponent intelligence;
  HumanoidSkeletonComponent skeleton;
  VisualComponent visual;

  void hasSceneChange();
};

