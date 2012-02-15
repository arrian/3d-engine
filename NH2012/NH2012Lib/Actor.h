#pragma once

#include <OgreFrameListener.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>

#include "Entity.h"
#include "MonsterData.h"

class Actor : public Entity, public MonsterData
{
public:
  Actor(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, 
        Ogre::Vector3 position = Ogre::Vector3(0,0,0), int id = 0);
  ~Actor(void);

  virtual void frameRenderingQueued(const Ogre::FrameEvent& evt) = 0;
protected:

};

