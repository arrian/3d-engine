#include "Player.h"

#include "World.h"
#include "Scene.h"

//-------------------------------------------------------------------------------------
Player::Player(World* world) 
  : PhysicalInterface(0,"Local", "Player"),
    world(world),
    scene(0),
    camera(),
    visual("actor.mesh"),
    addItem(false),
    placementDistance(3.0f),
    lookResponsiveness(0.15f),
    handMoveScalar(0.1f),
    skeleton()
{
  skeleton.mapPhysical((PhysicalInterface*) this);
}

//-------------------------------------------------------------------------------------
Player::~Player(void)
{
  if(node) scene->getSceneManager()->destroySceneNode(node);
  node = 0;
}

//-------------------------------------------------------------------------------------
void Player::setScene(Scene* scene, Ogre::Vector3 position, Ogre::Vector3 lookAt)
{
  assert(scene);

  //pulling down
  if(this->scene)
  {
    visual.removeNode();
    camera.removeNode();
    skeleton.removeNode();
    
    if(node) this->scene->getSceneManager()->destroySceneNode(node);
    node = 0;
  }

  //setting up
  this->scene = scene;

  //entity = scene->getSceneManager()->createEntity("actor.mesh");
  node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();
  node->setPosition(position);
  //node->attachObject(entity);

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
void Player::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  //std::cout << "x:" << node->getPosition().x << " z:" <<  node->getPosition().z << " y:" <<  node->getPosition().y << std::endl;
  camera.rayQuery();//testing ray queries

  skeleton.frameRenderingQueued(evt);
  camera.frameRenderingQueued(evt);//for aspect ratio changes

  if(addItem) 
  {
    scene->addItem(340, skeleton.getForwardPosition(placementDistance));//adding crate if the key was pressed
  }
}

//-------------------------------------------------------------------------------------
void Player::injectKeyDown(const OIS::KeyEvent &evt)
{
  if (evt.key == world->controls.moveForward) skeleton.setMoveForward(true);
  else if (evt.key == world->controls.moveBack) skeleton.setMoveBackward(true);
  else if (evt.key == world->controls.moveLeft) skeleton.setMoveLeft(true);
  else if (evt.key == world->controls.moveRight) skeleton.setMoveRight(true);
  else if (evt.key == world->controls.run) skeleton.setRun(true);
  else if (evt.key == world->controls.jump) skeleton.jump();
  else if (evt.key == OIS::KC_1) addItem = true;
}

//-------------------------------------------------------------------------------------
void Player::injectKeyUp(const OIS::KeyEvent &evt)
{
  if (evt.key == world->controls.moveForward) skeleton.setMoveForward(false);
  else if (evt.key == world->controls.moveBack) skeleton.setMoveBackward(false);
  else if (evt.key == world->controls.moveLeft) skeleton.setMoveLeft(false);
  else if (evt.key == world->controls.moveRight) skeleton.setMoveRight(false);
  else if (evt.key == world->controls.run) skeleton.setRun(false);
  else if (evt.key == OIS::KC_1) addItem = false;
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


