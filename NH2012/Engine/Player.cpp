#include "Player.h"

#include "World.h"
#include "Scene.h"
#include "Interactive.h"

//-------------------------------------------------------------------------------------
Player::Player(PlayerDesc description, World* world)
  : Actor(this, "Player", PLAYER_SELF),
    world(world),
    camera(world->isSSAOEnabled(), world->isBloomEnabled()),
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
    freeCameraNode(NULL)
{
}

//-------------------------------------------------------------------------------------
Player::~Player(void)
{
  if(node) scene->getGraphicsManager()->destroySceneNode(node);
  node = NULL;
}

//-------------------------------------------------------------------------------------
void Player::hasSceneChange()
{
  //pulling down
  if(oldScene)
  {
    mesh.removeNode();
    camera.removeNode();
    movement.removeNode();
    skeleton.removeNode();
    query.removeNode();
    
    if(node) oldScene->getGraphicsManager()->destroySceneNode(node);
    node = NULL;

    if(freeCameraNode) oldScene->getGraphicsManager()->destroySceneNode(freeCameraNode);
    freeCameraNode = NULL;
  }

  //setting up
  if(scene)
  {
    node = scene->getGraphicsManager()->getRootSceneNode()->createChildSceneNode();
    
    setPosition(position);

    skeleton.setUserData((Identifiable*) this);
    skeleton.setGroup(PLAYER_SELF);//add ability to have other player groups

    mesh.setNode(scene, node);
    skeleton.setNode(scene, node);
    movement.setNode(scene, node);
    camera.setNode(scene, skeleton.getHead());
    query.setNode(scene, skeleton.getHead());

    stop();
  
    camera.rehookWindow();
  }
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
  if(addMonster) scene->addMonster(monsterGenerationID, scene->getPathfindManager()->getRandomNavigablePoint());//create a monster at an arbitrary location

  currentTarget = query.rayQuery(camera.getDirection(), 20.0f, EXCLUDE_SELF);
  
  if(currentTarget)
  {
    //std::cout << currentTarget->getName() << "::" << currentTarget->getInstanceID() << std::endl;//display current targetted object

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
  buttonEvent(Button(evt.key), true);
}

//-------------------------------------------------------------------------------------
void Player::keyReleased(const OIS::KeyEvent &evt)
{
  buttonEvent(Button(evt.key), false);
}

//-------------------------------------------------------------------------------------
void Player::buttonEvent(Button button, bool isDown)
{
  if(isDown)
  {
    if(button == world->getControlManager()->jump) movement.jump();
    else if(button == world->getControlManager()->interact) interactPressed = true;
  }

  if(button == world->getControlManager()->moveForward) movement.setMoveForward(isDown);
  else if(button == world->getControlManager()->moveBack) movement.setMoveBackward(isDown);
  else if(button == world->getControlManager()->moveLeft) movement.setMoveLeft(isDown);
  else if(button == world->getControlManager()->moveRight) movement.setMoveRight(isDown);
  else if(button == world->getControlManager()->run) movement.setRun(isDown);
  else if(button == world->getControlManager()->crouch) skeleton.setCrouch(isDown);
  else if(button == world->getControlManager()->addItem) addItem = isDown;
  else if(button == world->getControlManager()->addMonster) addMonster = isDown;
  else if(button == world->getControlManager()->leftHand) skeleton.setLeftHand(isDown);
  else if(button == world->getControlManager()->rightHand) skeleton.setRightHand(isDown);
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

  if(freeCameraNode)//TODO: make a flag for free camera enabled rather than relying on freeCameraNode to be NULL
  {
    node->yaw(Ogre::Degree(-evt.state.X.rel * lookResponsiveness * lookScalar));
    freeCameraNode->pitch(Ogre::Degree(-evt.state.Y.rel * lookResponsiveness * lookScalar));
  }
  else skeleton.headRelative(Ogre::Degree(-evt.state.X.rel * lookResponsiveness * lookScalar), Ogre::Degree(-evt.state.Y.rel * lookResponsiveness * lookScalar));
}

//-------------------------------------------------------------------------------------
void Player::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
  buttonEvent(Button(id), true);
}

//-------------------------------------------------------------------------------------
void Player::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
  buttonEvent(Button(id), false);
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
Vector3 Player::getPosition()
{
  if(node) return node->getPosition();
  return position;
}

//-------------------------------------------------------------------------------------
Quaternion Player::getRotation()
{
  return node->getOrientation();
}

//-------------------------------------------------------------------------------------
Vector3 Player::getVelocity()
{
  return movement.getVelocity();
}

//-------------------------------------------------------------------------------------
void Player::setPosition(Vector3 position)
{
  this->position = position;
  if(node) node->setPosition(position);
}

//-------------------------------------------------------------------------------------
void Player::setRotation(Quaternion rotation)
{
  node->setOrientation(rotation);
}

//-------------------------------------------------------------------------------------
void Player::setGravity(Vector3 gravity)
{
  movement.setGravity(gravity);
}

//-------------------------------------------------------------------------------------
void Player::setFreeCamera(bool enabled)
{
  if(!scene) throw NHException("player must be in a scene to enable or disable the free camera");

  if(enabled)
  {
    if(!freeCameraNode) freeCameraNode = scene->getGraphicsManager()->getRootSceneNode()->createChildSceneNode();
    freeCameraNode->setPosition(skeleton.getHead()->_getDerivedPosition());
    freeCameraNode->_setDerivedOrientation(skeleton.getHead()->_getDerivedOrientation());

    camera.setNode(scene, freeCameraNode);
    movement.setGravityEnabled(false);
    movement.setNode(scene, freeCameraNode);
  }
  else
  {
    camera.setNode(scene, skeleton.getHead());
    movement.setGravityEnabled(true);
    movement.setNode(scene, skeleton.getHead());

    if(freeCameraNode) scene->getGraphicsManager()->destroySceneNode(freeCameraNode);
    freeCameraNode = NULL;
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

//-------------------------------------------------------------------------------------
void Player::stagger(Vector3 direction)
{
  //check if there is space to stagger
  //stumble backwards
  throw NHException("not implemented");
}

//-------------------------------------------------------------------------------------
void Player::damage(double amount)
{
  throw NHException("not implemented");
}

//-------------------------------------------------------------------------------------
void Player::heal(double amount)
{

  throw NHException("not implemented");
}

//-------------------------------------------------------------------------------------
void Player::setLookAt(Vector3 lookAt)
{
  if(camera.getCamera()->getPosition() == lookAt) camera.getCamera()->lookAt(lookAt - Vector3(0.0f,0.0f,1.0f));//camera glitch if lookAt is set to the same position as its location... attempting to correct
  else camera.getCamera()->lookAt(lookAt);
}

//-------------------------------------------------------------------------------------
void Player::setRunning(bool running)
{
  movement.setRun(running);
}

//-------------------------------------------------------------------------------------
void Player::setCrouching(bool crouching)
{
  skeleton.setCrouch(crouching);
}

//-------------------------------------------------------------------------------------
bool Player::getCrouching()
{
  return skeleton.isCrouched();
}

//-------------------------------------------------------------------------------------
bool Player::getRunning()
{
  return movement.isRunning();
}

//-------------------------------------------------------------------------------------
Vector3 Player::getGravity()
{
  return movement.getGravity();
}

//-------------------------------------------------------------------------------------
PlayerPacket Player::extractPacket()
{
  PlayerPacket packet = PlayerPacket();
  packet.position = getPosition();
  packet.velocity = getVelocity();
  packet.isCrouching = getCrouching();
  packet.isRunning = getRunning();
  return packet;
}

//-------------------------------------------------------------------------------------
void Player::integratePacket(PlayerPacket packet)
{
  setPosition(packet.position);//should interpolate
  setCrouching(packet.isCrouching);
  setRunning(packet.isRunning);
  //set velocity
}