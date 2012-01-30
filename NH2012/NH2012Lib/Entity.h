#pragma once

#include <string>

#include <OgreString.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>

class Entity
{
public:
  Entity(Ogre::SceneManager* sceneManager, Ogre::String meshName = Ogre::String("default.mesh"), Ogre::Vector3 position = Ogre::Vector3(0,0,0), int id = 0, Ogre::String name = Ogre::String("DefaultEntity"));
  ~Entity(void);
protected:
  int id;
  Ogre::String name;

  Ogre::Entity* entity;
  Ogre::SceneNode* node;
};

