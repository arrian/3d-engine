#include "Player.h"

#include "World.h"
#include "Scene.h"

//-------------------------------------------------------------------------------------
Player::Player(World* world) 
  : IdentificationInterface("Local", "Player"),
    world(world),
    scene(NULL),
    camera(world->enableSSAO, world->enableBloom, world->enableMotionBlur),
    visual("actor.mesh"),
    addItem(false),
    placementDistance(3.0f),
    lookResponsiveness(0.15f),
    handMoveScalar(0.1f),
    skeleton(),
    itemGenerationID(353)//temporarily generating a watermelon when the 1 key is pressed
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
    //scene->removePlayer(this);//should be called in addPlayer
    visual.removeNode();
    camera.removeNode();
    skeleton.removeNode();
    
    if(node) this->scene->getSceneManager()->destroySceneNode(node);
    node = NULL;
  }

  if(!scene) return;//no new scene given

  //setting up
  this->scene = scene;
  //scene->addPlayer(this);//set scene usually called from addPlayer

  //entity = scene->getSceneManager()->createEntity("actor.mesh");
  node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();
  node->setPosition(position);
  //node->attachObject(entity);

  skeleton.mapPhysical((IdentificationInterface*) this);//needs to be before skeleton.setNode()

  visual.setNode(scene, node);
  skeleton.setNode(scene, node);
  camera.setNode(scene, skeleton.getHead());

  //if(window) hook(window);//if the player has a render window then connect camera to new scene

  stop();

  if(world->getSceneChangeListener()) world->getSceneChangeListener()->sceneChanged();//notify the scene change listener that the scene has changed
}

//-------------------------------------------------------------------------------------
Scene* Player::getScene()
{
  return scene;
}

//-------------------------------------------------------------------------------------
void Player::update(double elapsedSeconds)
{
  //camera.rayQuery();//testing ray queries

  skeleton.update(elapsedSeconds);
  camera.update(elapsedSeconds);//for aspect ratio changes

  if(addItem) 
  {
    for(int i = 0; i < 10; i++) scene->addItem(itemGenerationID, skeleton.getForwardPosition(placementDistance));
  }
}

//-------------------------------------------------------------------------------------
void Player::injectKeyDown(const OIS::KeyEvent &evt)
{
  if (evt.key == world->controls.jump) skeleton.jump();
  else keyEvent(evt, true);
}

//-------------------------------------------------------------------------------------
void Player::injectKeyUp(const OIS::KeyEvent &evt)
{
  keyEvent(evt, false);
}

//-------------------------------------------------------------------------------------
void Player::keyEvent(const OIS::KeyEvent &evt, bool isDown)
{
  if (evt.key == world->controls.moveForward) skeleton.setMoveForward(isDown);
  else if (evt.key == world->controls.moveBack) skeleton.setMoveBackward(isDown);
  else if (evt.key == world->controls.moveLeft) skeleton.setMoveLeft(isDown);
  else if (evt.key == world->controls.moveRight) skeleton.setMoveRight(isDown);
  else if (evt.key == world->controls.run) skeleton.setRun(isDown);
  else if (evt.key == world->controls.crouch) skeleton.setCrouch(isDown);
  else if (evt.key == OIS::KC_1) addItem = isDown; 
}

//-------------------------------------------------------------------------------------
void Player::injectMouseMove(const OIS::MouseEvent &evt)
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
void Player::injectMouseDown(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
  if(id == world->controls.leftHand) skeleton.setLeftHand(true);
  if(id == world->controls.rightHand) skeleton.setRightHand(true);
}

//-------------------------------------------------------------------------------------
void Player::injectMouseUp(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
  if(id == world->controls.leftHand) skeleton.setLeftHand(false);
  if(id == world->controls.rightHand) skeleton.setRightHand(false);
}

//-------------------------------------------------------------------------------------
void Player::hook(Ogre::RenderWindow* window)
{
  assert(window && scene);
  camera.hookWindow(window);
}

//-------------------------------------------------------------------------------------
void Player::stop()
{
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
  return skeleton.getVelocity();
}

//-------------------------------------------------------------------------------------
void Player::setPosition(Ogre::Vector3 position)
{
  node->setPosition(position);
  skeleton.setNode(scene, node);//do i need to do this?
  camera.setNode(scene, skeleton.getHead());//do i need to do this?
}

//-------------------------------------------------------------------------------------
void Player::setRotation(Ogre::Quaternion rotation)
{
  node->setOrientation(rotation);
}

//-------------------------------------------------------------------------------------
void Player::setGravity(float gravity)
{
  skeleton.setGravity(gravity);
}

//-------------------------------------------------------------------------------------
void Player::setCollisionEnabled(bool enabled)
{
  skeleton.setCollisionEnabled(enabled);

}

//-------------------------------------------------------------------------------------
void Player::setItemGenerationID(int itemGenerationID)
{
  this->itemGenerationID = itemGenerationID;
}

