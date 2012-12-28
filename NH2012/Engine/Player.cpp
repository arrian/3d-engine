#include "Player.h"

#include "World.h"
#include "Scene.h"
#include "Interactive.h"

//-------------------------------------------------------------------------------------
Player::Player(PlayerDesc description, World* world)
  : IdentificationInterface(this, "Self", PLAYER_SELF),
    world(world),
    scene(NULL),
    camera(world->enableSSAO, world->enableBloom, world->enableMotionBlur),
    mesh(description.mesh),
    query(),
    addItem(false),
    addMonster(false),
    placementDistance(3.0f),
    lookResponsiveness(0.15f),
    handMoveScalar(0.1f),
    skeleton(),
    movement(description.gravity),
    itemGenerationID(3),//temporarily generating a watermelon when the 1 key is pressed
    monsterGenerationID(1),
    currentTarget(NULL),
    interactPressed(false),
    node(NULL),
    freeCameraNode(NULL)
{
  
}

//-------------------------------------------------------------------------------------
Player::~Player(void)
{
  if(node) scene->getSceneManager()->destroySceneNode(node);
  node = NULL;
}

//-------------------------------------------------------------------------------------
void Player::setScene(Scene* scene, Ogre::Vector3 position, Ogre::Vector3 lookAt)
{
  //pulling down
  if(this->scene)
  {
    mesh.removeNode();
    camera.removeNode();
    movement.removeNode();
    skeleton.removeNode();
    query.removeNode();
    
    if(node) this->scene->getSceneManager()->destroySceneNode(node);
    node = NULL;

    if(freeCameraNode) this->scene->getSceneManager()->destroySceneNode(freeCameraNode);
    freeCameraNode = NULL;
  }

  //setting up
  this->scene = scene;

  if(!scene) return;//no new scene given

  node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();
  node->setPosition(position);

  skeleton.setUserData((IdentificationInterface*) this);
  skeleton.setGroup(PLAYER_SELF);//add ability to have other player groups

  mesh.setNode(scene, node);
  skeleton.setNode(scene, node);
  movement.setNode(scene, node);
  camera.setNode(scene, skeleton.getHead());
  query.setNode(scene, skeleton.getHead());

  stop();
  
  camera.rehookWindow();
}

//-------------------------------------------------------------------------------------
Scene* Player::getScene()
{
  return scene;
}

//-------------------------------------------------------------------------------------
void Player::update(double elapsedSeconds)
{
  camera.update(elapsedSeconds);
  movement.update(elapsedSeconds);
  skeleton.update(elapsedSeconds);
  if(skeleton.isOnGround()) movement.hitGround();


  if(addItem)
  {
    for(int i = 0; i < 10; i++) scene->addItem(itemGenerationID, skeleton.getForwardPosition(placementDistance));
  }
  if(addMonster) scene->addMonster(monsterGenerationID, scene->getPathfindManager()->getRandomNavigablePoint() + Ogre::Vector3(0,1,0));//create a monster at an arbitrary location

  currentTarget = query.rayQuery(camera.getDirection(), 20.0f, EXCLUDE_SELF);
  
  if(currentTarget)
  {
    std::cout << currentTarget->getName() << "::" << currentTarget->getInstanceID() << std::endl;

    if(interactPressed)
    {
      if(currentTarget->isInGroup(ITEM))
      {
        Item* item = static_cast<Item*>(currentTarget->getInstancePointer());
        item->getDescription();
        scene->removeItem(item);
      }
      else if(currentTarget->isInGroup(INTERACTIVE))
      {
        Interactive* interactive = static_cast<Interactive*>(currentTarget->getInstancePointer());
        interactive->interact();
      }
      interactPressed = false;
    }
  }
}

//-------------------------------------------------------------------------------------
void Player::keyPressed(const OIS::KeyEvent &evt)
{
  if (evt.key == world->getControlManager()->jump) movement.jump();
  else if(evt.key == world->getControlManager()->interact) interactPressed = true;
  else keyEvent(evt, true);
}

//-------------------------------------------------------------------------------------
void Player::keyReleased(const OIS::KeyEvent &evt)
{
  keyEvent(evt, false);
}

//-------------------------------------------------------------------------------------
void Player::keyEvent(const OIS::KeyEvent &evt, bool isDown)
{
  if(evt.key == world->getControlManager()->moveForward) movement.setMoveForward(isDown);
  else if(evt.key == world->getControlManager()->moveBack) movement.setMoveBackward(isDown);
  else if(evt.key == world->getControlManager()->moveLeft) movement.setMoveLeft(isDown);
  else if(evt.key == world->getControlManager()->moveRight) movement.setMoveRight(isDown);
  else if(evt.key == world->getControlManager()->run) movement.setRun(isDown);
  else if(evt.key == world->getControlManager()->crouch) skeleton.setCrouch(isDown);
  else if(evt.key == world->getControlManager()->addItem) addItem = isDown;
  else if(evt.key == world->getControlManager()->addMonster) addMonster = isDown;
}

//-------------------------------------------------------------------------------------
void Player::mouseMoved(const OIS::MouseEvent &evt)
{
  float lookScalar = 1.0f;
  if(skeleton.isLeftHand()) 
  {
    skeleton.leftHandRelative(Ogre::Degree(-evt.state.X.rel * lookResponsiveness), Ogre::Degree(-evt.state.Y.rel * lookResponsiveness));
    lookScalar = handMoveScalar;
  }
  if(skeleton.isRightHand()) 
  {
    skeleton.rightHandRelative(Ogre::Degree(-evt.state.X.rel * lookResponsiveness), Ogre::Degree(-evt.state.Y.rel * lookResponsiveness));
    lookScalar = handMoveScalar;
  }
  skeleton.headRelative(Ogre::Degree(-evt.state.X.rel * lookResponsiveness * lookScalar), Ogre::Degree(-evt.state.Y.rel * lookResponsiveness * lookScalar));
}

//-------------------------------------------------------------------------------------
void Player::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
  if(id == world->getControlManager()->leftHand) skeleton.setLeftHand(true);
  if(id == world->getControlManager()->rightHand) skeleton.setRightHand(true);
}

//-------------------------------------------------------------------------------------
void Player::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
  if(id == world->getControlManager()->leftHand) skeleton.setLeftHand(false);
  if(id == world->getControlManager()->rightHand) skeleton.setRightHand(false);
}

//-------------------------------------------------------------------------------------
void Player::hookWindow(Ogre::RenderWindow* window)
{
  if(window) camera.hookWindow(window);
}

//-------------------------------------------------------------------------------------
Ogre::RenderWindow* Player::getWindow()
{
  return camera.getWindow();
}

//-------------------------------------------------------------------------------------
void Player::stop()
{
  movement.stop();
  skeleton.stop();
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 Player::getPosition()
{
  return node->getPosition();
}

//-------------------------------------------------------------------------------------
Ogre::Quaternion Player::getRotation()
{
  return node->getOrientation();
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 Player::getVelocity()
{
  return movement.getVelocity();
}

//-------------------------------------------------------------------------------------
void Player::setPosition(Ogre::Vector3 position)
{
  node->setPosition(position);
}

//-------------------------------------------------------------------------------------
void Player::setRotation(Ogre::Quaternion rotation)
{
  node->setOrientation(rotation);
}

//-------------------------------------------------------------------------------------
void Player::setGravity(Ogre::Vector3 gravity)
{
  movement.setGravity(gravity);
}

//-------------------------------------------------------------------------------------
void Player::setFreeCamera(bool enabled)
{
  if(!scene) throw NHException("player must be in a scene to enable or disable the free camera");

  if(enabled)
  {
    if(!freeCameraNode) freeCameraNode = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    freeCameraNode->setPosition(skeleton.getHead()->_getDerivedPosition());
    camera.setNode(scene, freeCameraNode);
  }
  else
  {
    if(freeCameraNode) scene->getSceneManager()->destroySceneNode(freeCameraNode);
    freeCameraNode = NULL;
    camera.setNode(scene, skeleton.getHead());
  }
}

//-------------------------------------------------------------------------------------
void Player::setItemGenerationID(int itemGenerationID)
{
  this->itemGenerationID = itemGenerationID;
}

//-------------------------------------------------------------------------------------
Ogre::Camera* Player::getCamera()
{
  return camera.getCamera();
}

//-------------------------------------------------------------------------------------
Ogre::Viewport* Player::getViewport()
{
  return camera.getViewport();
}