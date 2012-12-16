#include "Player.h"

#include "World.h"
#include "Scene.h"

//-------------------------------------------------------------------------------------
Player::Player(PlayerDesc description, World* world)
  : IdentificationInterface("Local", "Player"),
    world(world),
    scene(NULL),
    camera(world->enableSSAO, world->enableBloom, world->enableMotionBlur),
    visual(description.mesh),
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
    visual.removeNode();
    camera.removeNode();
    skeleton.removeNode();
    query.removeNode();
    
    if(node) this->scene->getSceneManager()->destroySceneNode(node);
    node = NULL;
  }

  if(!scene) return;//no new scene given

  //setting up
  this->scene = scene;

  node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();
  node->setPosition(position);

  skeleton.mapPhysical((IdentificationInterface*) this);//needs to be before skeleton.setNode()

  visual.setNode(scene, node);
  skeleton.setNode(scene, node);
  camera.setNode(scene, skeleton.getHead());
  query.setNode(scene, skeleton.getHead());

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
  skeleton.update(elapsedSeconds);

  ////////////////////////////////Implement later
  /*
  if(node)
  {
    //Calculating the acceleration vector
    Ogre::Vector3 accel = Ogre::Vector3::ZERO;
    if (moveForward) accel -= node->getOrientation().zAxis();//equivalent to camera->getDirection()
    if (moveBack) accel += node->getOrientation().zAxis();
    if (moveRight) accel += node->getOrientation().xAxis();//equivalent to camera->getRight()
    if (moveLeft) accel -= node->getOrientation().xAxis();
  
    float oldY;
    if(collisionEnabled)//ignore up/down directional movement from camera when collision enabled
    {
      oldY = velocity.y;
      velocity.y = 0;
    }
  
    float topSpeed = run ? speed * runScalar : speed;
    if (accel.squaredLength() != 0)
    {
      accel.normalise();
      velocity += accel * topSpeed * (float) elapsedSeconds * moveScalar;
    }
    else 
    {
      Ogre::Vector3 reduce = velocity * (float) elapsedSeconds * moveScalar;
      if(velocity.squaredLength() > reduce.squaredLength()) velocity -= reduce;//comparing with reduction length reduces jitter at low frame rates
      else velocity = Ogre::Vector3::ZERO;
    }

    Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

    if (velocity.squaredLength() > topSpeed * topSpeed)//don't go over the top speed
    {
      velocity.normalise();
      velocity *= topSpeed;
    }
    else if (velocity.squaredLength() < tooSmall * tooSmall) velocity = Ogre::Vector3::ZERO;

    if(collisionEnabled)
    {
      velocity.y = oldY;//restoring saved up/down speed
      velocity.y += gravity.y * (float) elapsedSeconds;//only apply gravity when collision enabled

      physx::PxU32 collisionFlags = controller->move(physx::PxVec3(velocity.x * (float) elapsedSeconds * moveScalar, velocity.y * (float) elapsedSeconds * moveScalar, velocity.z * (float) elapsedSeconds * moveScalar), minimumMoveDistance, (float) elapsedSeconds, physx::PxControllerFilters());//moving character controller
      if((collisionFlags & physx::PxControllerFlag::eCOLLISION_DOWN) != 0)
      {
        onGround = true;
        velocity.y = 0.0f;//stop falling when collision at the base of the skeleton occurs
      }
      physx::PxExtendedVec3 cPosition = controller->getPosition();
      node->setPosition(Ogre::Real(cPosition.x), Ogre::Real(cPosition.y), Ogre::Real(cPosition.z));//updating the body's visual position from the physics world calculated position
    }
    else //just move the controller ignoring all collisions
    {
      controller->setPosition(controller->getPosition() + physx::PxExtendedVec3(velocity.x * (float) elapsedSeconds * moveScalar, velocity.y * (float) elapsedSeconds * moveScalar, velocity.z * (float) elapsedSeconds * moveScalar));
      physx::PxExtendedVec3 cPosition = controller->getPosition();
      node->setPosition(Ogre::Real(cPosition.x), Ogre::Real(cPosition.y), Ogre::Real(cPosition.z));//updating the body's visual position
    }
  }
  */
  ////////////////////////////////

  camera.update(elapsedSeconds);//for aspect ratio changes



  if(addItem) 
  {
    for(int i = 0; i < 10; i++) scene->addItem(itemGenerationID, skeleton.getForwardPosition(placementDistance));
  }
  if(addMonster) scene->addMonster(monsterGenerationID);//create a monster at an arbitrary location


  query.rayQuery(camera.getDirection(), 200.0f);
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
  else if (evt.key == world->controls.addItem) addItem = isDown;
  else if (evt.key == world->controls.addMonster) addMonster = isDown;
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