#pragma once

#include <Ogre.h>
#include <limits>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include "Bar.h"
#include "Actor.h"
#include "Identifiable.h"
#include "HumanoidSkeletonComponent.h"
#include "KinematicMovementComponent.h"
#include "CameraComponent.h"
#include "MeshComponent.h"
#include "QueryComponent.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Packetable.h"
#include "Item.h"
#include "Apparel.h"
#include "Button.h"
#include "ComponentContainer.h"

class Scene;
class World;

class Player : public Actor, public Packetable<PlayerPacket>
{
public:
  Player(PlayerDesc description, World* world);
  ~Player(void);

  void update(double elapsedSeconds);
  void hookWindow(Ogre::RenderWindow* window);//hooks the player camera to a render window
  
  //Actions
  virtual void stop();
  virtual void stagger(Vector3 direction);
  virtual void damage(double amount);
  virtual void heal(double amount);

  //Setters
  virtual void setPosition(Vector3 position);
  virtual void setRotation(Quaternion rotation);
  virtual void setLookAt(Vector3 lookAt);
  virtual void setRunning(bool running);
  virtual void setCrouching(bool crouching);
  virtual void setGravity(Vector3 gravity);
  void setFreeCamera(bool enabled);
  void setItemGenerationID(int id);
  
  //Getters
  virtual Vector3 getPosition();
  virtual Quaternion getRotation();
  Vector3 getVelocity();
  CameraComponent* getCamera();
  Ogre::Viewport* getViewport();
  Ogre::RenderWindow* getWindow();
  virtual bool getCrouching();
  virtual bool getRunning();
  virtual Vector3 getGravity();

  //Injectors
  void keyPressed(const OIS::KeyEvent &evt);
  void keyReleased(const OIS::KeyEvent &evt);
  void mouseMoved(const OIS::MouseEvent &evt);
  void mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
  void mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
  
  //Network
  virtual PlayerPacket extractPacket();
  virtual void integratePacket(PlayerPacket packet);

private:
  World* world;

  Ogre::SceneNode* freeCameraNode;//node for free camera
  Identifiable* currentTarget;

  CameraComponent camera;//the player camera generally attached to the head node
  HumanoidSkeletonComponent skeleton;
  KinematicMovementComponent movement;
  MeshComponent mesh;
  QueryComponent query;

  ComponentContainer inventory;//make inventory class
  Item* leftHand;
  Item* rightHand;
  Apparel* clothes;
  Apparel* shoes;
  Apparel* hat;
  ComponentContainer quickSlots;

  Bar life;
  Bar experience;
  int level;

  bool addItem;
  bool addMonster;
  int itemGenerationID;//for debug item generation
  int monsterGenerationID;//for debug monster generation
  float placementDistance;
  bool interactPressed;
  float lookResponsiveness;
  float handMoveScalar;

  void buttonEvent(Button button, bool isDown);
  //void keyEvent(const OIS::KeyEvent &evt, bool isDown);
  virtual void hasSceneChange();
};

