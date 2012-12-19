#pragma once

#include <OgreFrameListener.h>
#include <OgreString.h>
#include <OgreVector3.h>

#include "PathfindManager.h"
#include "BasicComponent.h"
#include "HumanoidSkeletonComponent.h"
#include "IntelligenceComponent.h"
#include "MeshComponent.h"
#include "Bar.h"

class Scene;

class Monster : public BasicComponent, public IdentificationInterface
{
public:
  Monster(MonsterDesc description);
  virtual ~Monster(void);

  void update(double elapsedSeconds);

  void setPosition(Ogre::Vector3 position);
  void setTarget(Ogre::Vector3 target);//Sets the monsters target position.

  friend bool operator==(const Monster& x, const Monster& y);

protected:
  MonsterDesc description;

  Ogre::SceneNode* node;
  Ogre::Vector3 position;
  Ogre::Vector3 target;

  IntelligenceComponent intelligence;
  HumanoidSkeletonComponent skeleton;
  MeshComponent mesh;

  void hasSceneChange();


  
};

