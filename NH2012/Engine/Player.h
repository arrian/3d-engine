#pragma once

#include <Ogre.h>
#include <limits>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include "Bar.h"
#include "IdentificationInterface.h"
#include "HumanoidSkeletonComponent.h"
#include "CameraComponent.h"
#include "VisualComponent.h"
#include "QueryComponent.h"

class Scene;
class World;

class Player : public IdentificationInterface
{
public:
  Player(PlayerDesc description, World* world);
  ~Player(void);

  void update(double elapsedSeconds);
  void hook(Ogre::RenderWindow* window);/*! Hooks the player camera to a render window.*/
  void stop();

  //Setters
  void setScene(Scene* scene, Ogre::Vector3 position, Ogre::Vector3 lookAt);
  void setPosition(Ogre::Vector3 position);
  void setRotation(Ogre::Quaternion rotation);
  void setGravity(Ogre::Vector3 gravity);
  void setCollisionEnabled(bool enabled);
  void setItemGenerationID(int id);
  
  //Getters
  Ogre::Vector3 getPosition();
  Ogre::Quaternion getRotation();
  Ogre::Vector3 getVelocity();
  Scene* getScene();//gets the scene the player is currently in
  Ogre::Camera* getCamera();
  Ogre::Viewport* getViewport();

  //Injectors
  void injectKeyDown(const OIS::KeyEvent &evt);
  void injectKeyUp(const OIS::KeyEvent &evt);
  void injectMouseMove(const OIS::MouseEvent &evt);
  void injectMouseDown(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

private:
  World* world;
  Scene* scene;

  Ogre::SceneNode* node;//world location of the player

  CameraComponent camera;//the player camera generally attached to the head node
  HumanoidSkeletonComponent skeleton;
  VisualComponent visual;
  QueryComponent query;

  bool addItem;
  bool addMonster;
  int itemGenerationID;//for debug item generation
  int monsterGenerationID;//for debug monster generation
  float placementDistance;

  float lookResponsiveness;
  float handMoveScalar;

  void keyEvent(const OIS::KeyEvent &evt, bool isDown);
};

