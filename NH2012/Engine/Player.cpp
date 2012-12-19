#include "Player.h"

#include "World.h"
#include "Scene.h"

//-------------------------------------------------------------------------------------
Player::Player(PlayerDesc description, World* world)
  : IdentificationInterface("Local", "Player"),
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
    skeleton(description.gravity),
    itemGenerationID(3),//temporarily generating a watermelon when the 1 key is pressed
    monsterGenerationID(1)
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
    skeleton.removeNode();
    query.removeNode();
    
    if(node) this->scene->getSceneManager()->destroySceneNode(node);
    node = NULL;
  }

  //setting up
  this->scene = scene;

  if(!scene) return;//no new scene given

  node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();
  node->setPosition(position);

  skeleton.mapPhysical((IdentificationInterface*) this);//needs to be before skeleton.setNode()

  mesh.setNode(scene, node);
  skeleton.setNode(scene, node);
  camera.setNode(scene, skeleton.getHead());
  query.setNode(scene, skeleton.getHead());

  stop();
  
  camera.rehookWindow();
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
  skeleton.update(elapsedSeconds);
  camera.update(elapsedSeconds);//for aspect ratio changes

  if(addItem) 
  {
    for(int i = 0; i < 10; i++) scene->addItem(itemGenerationID, skeleton.getForwardPosition(placementDistance));
  }
  if(addMonster) scene->addMonster(monsterGenerationID, scene->getPathfindManager()->getRandomNavMeshPoint() + Ogre::Vector3(0,1,0));//create a monster at an arbitrary location


  query.rayQuery(camera.getDirection(), 200.0f);
}

//-------------------------------------------------------------------------------------
void Player::injectKeyDown(const OIS::KeyEvent &evt)
{
  if (evt.key == world->getControlManager()->jump) skeleton.jump();
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
  if (evt.key == world->getControlManager()->moveForward) skeleton.setMoveForward(isDown);
  else if (evt.key == world->getControlManager()->moveBack) skeleton.setMoveBackward(isDown);
  else if (evt.key == world->getControlManager()->moveLeft) skeleton.setMoveLeft(isDown);
  else if (evt.key == world->getControlManager()->moveRight) skeleton.setMoveRight(isDown);
  else if (evt.key == world->getControlManager()->run) skeleton.setRun(isDown);
  else if (evt.key == world->getControlManager()->crouch) skeleton.setCrouch(isDown);
  else if (evt.key == world->getControlManager()->addItem) addItem = isDown;
  else if (evt.key == world->getControlManager()->addMonster) addMonster = isDown;
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
  if(id == world->getControlManager()->leftHand) skeleton.setLeftHand(true);
  if(id == world->getControlManager()->rightHand) skeleton.setRightHand(true);
}

//-------------------------------------------------------------------------------------
void Player::injectMouseUp(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
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
  query.setNode(scene,skeleton.getHead());
}

//-------------------------------------------------------------------------------------
void Player::setRotation(Ogre::Quaternion rotation)
{
  node->setOrientation(rotation);
}

//-------------------------------------------------------------------------------------
void Player::setGravity(Ogre::Vector3 gravity)
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