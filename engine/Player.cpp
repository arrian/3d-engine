#include "Player.h"

#include "World.h"
#include "Scene.h"
#include "Interactive.h"

#include "SceneGraphicsManager.h"
#include "ScenePathfindManager.h"

//-------------------------------------------------------------------------------------
Player::Player(PlayerDesc description, boost::shared_ptr<World> world)
  : Actor(this, "Player", PLAYER_SELF),
    world(boost::weak_ptr<World>(world)),
    camera(world->getGraphicsManager()->isSSAOEnabled(), world->getGraphicsManager()->isBloomEnabled()),
    mesh(description.mesh),
    query(),
    addItem(false),
    addCreature(false),
    placementDistance(3.0f),
    lookResponsiveness(0.15f),
    handMoveScalar(0.1f),
    reachDistance(20.0f),
    skeleton(),
    movement(description.gravity),
    itemGenerationID(3),//temporarily generating a watermelon when the 1 key is pressed
    creatureGenerationID(1),
    currentTarget(NULL),
    interactPressed(false),
    freeCameraNode(NULL),
    localPlayer(true)//TODO: change for multiplayer
{
}

//-------------------------------------------------------------------------------------
Player::~Player(void)
{
  boost::shared_ptr<World> world_ptr = getWorld();
  if(!world_ptr)
  {
#ifdef _DEBUG
    std::cout << "World expired before calling player destructor." << std::endl;
#endif
    return;
  }

  //TODO: Check for scene expiry
  boost::shared_ptr<Scene> scene_ptr = getScene();

  if(scene_ptr && node && scene_ptr->getSceneGraphicsManager()) scene_ptr->getSceneGraphicsManager()->destroySceneNode(node);
  node = NULL;
}

//-------------------------------------------------------------------------------------
void Player::hasSceneChange()
{
  //pulling down
  boost::shared_ptr<Scene> oldScene_ptr = getOldScene();
  if(oldScene_ptr)
  {
    mesh.removeNode();
    camera.removeNode();
    movement.removeNode();
    skeleton.removeNode();
    query.removeNode();
    
    if(node) oldScene_ptr->getSceneGraphicsManager()->destroySceneNode(node);
    node = NULL;

    if(freeCameraNode) oldScene_ptr->getSceneGraphicsManager()->destroySceneNode(freeCameraNode);
    freeCameraNode = NULL;
  }

  //setting up
  boost::shared_ptr<Scene> scene_ptr = getScene();
  if(scene_ptr)
  {
    node = scene_ptr->getSceneGraphicsManager()->createSceneNode();
    
    setPosition(position);

    skeleton.setUserData((Identifiable*) this);
    skeleton.setGroup(PLAYER_SELF);//add ability to have other player groups

    mesh.setNode(scene_ptr, node);
    skeleton.setNode(scene_ptr, node);
    movement.setNode(scene_ptr, node);
    camera.setNode(scene_ptr, skeleton.getHead());
    query.setNode(scene_ptr, skeleton.getHead());

    stop();
  
    camera.rehookWindow();
  }
}

//-------------------------------------------------------------------------------------
void Player::update(double elapsedSeconds)
{
  boost::shared_ptr<Scene> scene_ptr = getScene();
  if(!scene_ptr) throw NHException("attempted to update a player that was not in a scene");

  camera.update(elapsedSeconds);
  movement.update(elapsedSeconds);
  skeleton.update(elapsedSeconds);
  if(skeleton.isOnGround()) movement.hitGround();

  if(addItem)
  {
    for(int i = 0; i < 10; i++) scene_ptr->addItem(itemGenerationID, skeleton.getForwardPosition(placementDistance));
  }
  if(addCreature) scene_ptr->addCreature(creatureGenerationID, scene_ptr->getScenePathfindManager()->getRandomNavigablePoint());//create a creature at an arbitrary location

  currentTarget = query.rayQuery(camera.getDirection(), reachDistance, EXCLUDE_SELF);
  
  if(currentTarget)
  {
    //std::cout << currentTarget->getName() << "::" << currentTarget->getInstanceID() << std::endl;//display current targetted object

    if(interactPressed)
    {
      if(currentTarget->isInGroup(ITEM))
      {
        Item* item = currentTarget->getInstancePointer<Item>();
        inventory.insert(Id<Item>(), scene_ptr->remove<Item>(item));//inventory now becomes responsible for the life of the item
        ;
      }
      else if(currentTarget->isInGroup(INTERACTIVE))
      {
        Interactive* interactive = currentTarget->getInstancePointer<Interactive>();
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
  boost::shared_ptr<World> world_ptr = getWorld();
  if(!world_ptr) throw NHException("Failed to get control manager in player button event. The world has expired.");

  if(isDown)
  {
    if(button == world_ptr->getControlManager()->jump) movement.jump();
    else if(button == world_ptr->getControlManager()->interact) interactPressed = true;
  }

  if(button == world_ptr->getControlManager()->moveForward) movement.setMoveForward(isDown);
  else if(button == world_ptr->getControlManager()->moveBack) movement.setMoveBackward(isDown);
  else if(button == world_ptr->getControlManager()->moveLeft) movement.setMoveLeft(isDown);
  else if(button == world_ptr->getControlManager()->moveRight) movement.setMoveRight(isDown);
  else if(button == world_ptr->getControlManager()->run) movement.setRun(isDown);
  else if(button == world_ptr->getControlManager()->crouch) skeleton.setCrouch(isDown);
  else if(button == world_ptr->getControlManager()->addItem) addItem = isDown;
  else if(button == world_ptr->getControlManager()->addCreature) addCreature = isDown;
  else if(button == world_ptr->getControlManager()->leftHand) skeleton.setLeftHand(isDown);
  else if(button == world_ptr->getControlManager()->rightHand) skeleton.setRightHand(isDown);
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
  boost::shared_ptr<Scene> scene_ptr = getScene();
  if(!scene_ptr) throw NHException("player must be in a scene to enable or disable the free camera");

  if(enabled)
  {
    if(!freeCameraNode) freeCameraNode = scene_ptr->getSceneGraphicsManager()->createSceneNode();
    freeCameraNode->setPosition(skeleton.getHead()->_getDerivedPosition());
    freeCameraNode->_setDerivedOrientation(skeleton.getHead()->_getDerivedOrientation());

    camera.setNode(scene_ptr, freeCameraNode);
    movement.setGravityEnabled(false);
    movement.setNode(scene_ptr, freeCameraNode);
  }
  else
  {
    camera.setNode(scene_ptr, skeleton.getHead());
    movement.setGravityEnabled(true);
    movement.setNode(scene_ptr, skeleton.getHead());

    if(freeCameraNode) scene_ptr->getSceneGraphicsManager()->destroySceneNode(freeCameraNode);
    freeCameraNode = NULL;
  }
}

//-------------------------------------------------------------------------------------
void Player::setItemGenerationID(int itemGenerationID)
{
  this->itemGenerationID = itemGenerationID;
}

//-------------------------------------------------------------------------------------
CameraComponent* Player::getCamera()
{
  return &camera;
}

//-------------------------------------------------------------------------------------
Ogre::Viewport* Player::getViewport()
{
  return camera.getViewport();
}

//-------------------------------------------------------------------------------------
void Player::stagger(Vector3 direction)
{
  movement.push(direction);
}

//-------------------------------------------------------------------------------------
void Player::damage(double amount)
{
  health.remove(amount);
}

//-------------------------------------------------------------------------------------
void Player::heal(double amount)
{
  health.add(amount);
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
  packet.lookAt = camera.getDirection();//TODO: change lookat to direction
  packet.crouching = getCrouching();
  packet.running = getRunning();
  return packet;
}

//-------------------------------------------------------------------------------------
void Player::integratePacket(PlayerPacket packet)
{
  setPosition(packet.position);//should interpolate
  setCrouching(packet.crouching);
  setRunning(packet.running);
  setLookAt(packet.lookAt);
}