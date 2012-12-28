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
#include "KinematicMovementComponent.h"
#include "CameraComponent.h"
#include "MeshComponent.h"
#include "QueryComponent.h"

class Scene;
class World;

class Player : public IdentificationInterface
{
public:
  Player(PlayerDesc description, World* world);
  ~Player(void);

  void update(double elapsedSeconds);
  void hookWindow(Ogre::RenderWindow* window);/*! Hooks the player camera to a render window.*/
  void stop();

  //Setters
  void setScene(Scene* scene, Ogre::Vector3 position = Ogre::Vector3::ZERO, Ogre::Vector3 lookAt = Ogre::Vector3::ZERO);
  void setPosition(Ogre::Vector3 position);
  void setRotation(Ogre::Quaternion rotation);
  void setGravity(Ogre::Vector3 gravity);
  void setFreeCamera(bool enabled);
  void setItemGenerationID(int id);
  
  //Getters
  Ogre::Vector3 getPosition();
  Ogre::Quaternion getRotation();
  Ogre::Vector3 getVelocity();
  Scene* getScene();//gets the scene the player is currently in
  Ogre::Camera* getCamera();
  Ogre::Viewport* getViewport();
  Ogre::RenderWindow* getWindow();

  //Injectors
  void keyPressed(const OIS::KeyEvent &evt);
  void keyReleased(const OIS::KeyEvent &evt);
  void mouseMoved(const OIS::MouseEvent &evt);
  void mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
  void mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

private:
  World* world;
  Scene* scene;

  Ogre::SceneNode* node;//world location of the player
  Ogre::SceneNode* freeCameraNode;//node for free camera

  CameraComponent camera;//the player camera generally attached to the head node
  HumanoidSkeletonComponent skeleton;
  KinematicMovementComponent movement;
  MeshComponent mesh;
  QueryComponent query;

  IdentificationInterface* currentTarget;

  bool addItem;
  bool addMonster;
  int itemGenerationID;//for debug item generation
  int monsterGenerationID;//for debug monster generation
  float placementDistance;

  bool interactPressed;

  float lookResponsiveness;
  float handMoveScalar;

  void keyEvent(const OIS::KeyEvent &evt, bool isDown);
};

