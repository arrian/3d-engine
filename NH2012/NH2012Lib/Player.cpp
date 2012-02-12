#include "Player.h"

/*
/// Initialisation
- Add a capsule

// Next two lines will keep the capsule always up
- capsule setSleepingThresholds(0.0, 0.0);
- capsule setAngularFactor(0.0);

/// Loop
// Then, you move the capsule with LinearVelocity

To make LinearVelocity move the capsule in the direction you are looking, use ths :
// Angle is the angle Y rotation of your camera

lVelocityX = sin( angle * PI / 180 ) * 2;
lVelocityY = capsule getLinearVelocity().y();
lVelocityZ = cos( angle * PI / 180 ) * 2;
*/


Player::Player(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, 
               Flag* flags, Ogre::RenderWindow* window, Ogre::Vector3 position) 
  : Actor(sceneManager, physics, position), 
    velocity(Ogre::Vector3::ZERO),
    gravityVector(Ogre::Vector3::ZERO),
    moveForward(false), 
    moveBack(false), 
    moveLeft(false), 
    moveRight(false), 
    run(false),
    leftHand(false),
    rightHand(false)
{
  this->flags = flags;

  health = Bar(10);
  magic = Bar(10);
  level = Bar(1);

  food = Bar(10);
  water = Bar(10);
  sleep = Bar(10);

  inventory = Inventory();
  attributes = MonsterAttributes();

  speed = 200;//walking speed

  //Create the camera
  cameraNode = node->createChildSceneNode(Ogre::Vector3(0,30,0));
  camera = sceneManager->createCamera("PlayerCamera");
  //camera->setPosition(Ogre::Vector3(0,100,80));
  camera->lookAt(Ogre::Vector3(0,80,-300));
  camera->setNearClipDistance(5);
  cameraNode->attachObject(camera);

  if(flags->wireframeDebug) camera->setPolygonMode(Ogre::PM_WIREFRAME);

  // Create one viewport, entire window
  Ogre::Viewport* vp = window->addViewport(camera);
  vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

  // Alter the camera aspect ratio to match the viewport
  camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

  //physics
  capsule = new OgreBulletCollisions::CapsuleCollisionShape(15.0f, 50.0f, Ogre::Vector3(0,1,0));

  capsuleBody = new OgreBulletDynamics::RigidBody("character", physics);
  capsuleBody->setShape(node, capsule, 0.0f, 0.0f, 10.0f, position);
  capsuleBody->getBulletRigidBody()->setAngularFactor(0.0);
  capsuleBody->getBulletRigidBody()->setSleepingThresholds(0.0, 0.0);

  stop();
}


Player::~Player(void)
{
  if(camera) delete camera;
}

void Player::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(food.current > 0) food.current -= Ogre::Real(0.0001) * evt.timeSinceLastFrame;
  if(water.current > 0) water.current -= Ogre::Real(0.00015) * evt.timeSinceLastFrame;
  if(food.current <= 0 || water.current <= 0) attributes.awareness = MonsterAttribute::DEAD;

  //Camera update 
  Ogre::Vector3 accel = Ogre::Vector3::ZERO;
  if (moveForward) accel += camera->getDirection();
  if (moveBack) accel -= camera->getDirection();
  if (moveRight) accel += camera->getRight();
  if (moveLeft) accel -= camera->getRight();

  // if accelerating, try to reach top speed in a certain time
  Ogre::Real topSpeed = run ? speed * 2 : speed;
  if (accel.squaredLength() != 0)
  {
    accel.normalise();
    velocity += accel * topSpeed * evt.timeSinceLastFrame * 10;
  }
  // if not accelerating, try to stop in a certain time
  else velocity -= velocity * evt.timeSinceLastFrame * 10;

  Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

  // keep camera velocity below top speed and above epsilon
  if (velocity.squaredLength() > topSpeed * topSpeed)
  {
    velocity.normalise();
    velocity *= topSpeed;
  }
  else if (velocity.squaredLength() < tooSmall * tooSmall)
    velocity = Ogre::Vector3::ZERO;

  if (flags->freeCameraDebug) 
  {
    if(velocity != Ogre::Vector3::ZERO) camera->move(velocity * evt.timeSinceLastFrame);
    node->setPosition(camera->getPosition());
  }
  else 
  {
    if(velocity != Ogre::Vector3::ZERO && capsuleBody->getLinearVelocity() != Ogre::Vector3::ZERO) capsuleBody->setLinearVelocity(velocity);
  }

  /*
  //Accurate ray casting for bullet world
  //Ray casting
  btVector3 from = btVector3(node->getPosition().x,node->getPosition().y,node->getPosition().z);
  //std::cout << "testing: " << node->getPosition() << std::endl; 
  btVector3 to = btVector3(0,0,0);
  btCollisionWorld::ClosestRayResultCallback resultCallback(from, to);
  physics->getBulletCollisionWorld()->rayTest(from, to, resultCallback);

  if(resultCallback.hasHit())
  {
    std::cout << "ray hit object" << std::endl;
    // some object was hit
  }
  */

  
}

void Player::animation()
{

}

void Player::audio()
{

}

void Player::collision()
{

}

void Player::injectKeyDown(const OIS::KeyEvent &evt)
{
  if (evt.key == flags->controls.moveForward) moveForward = true;
  else if (evt.key == flags->controls.moveBack) moveBack = true;
  else if (evt.key == flags->controls.moveLeft) moveLeft = true;
  else if (evt.key == flags->controls.moveRight) moveRight = true;
  else if (evt.key == flags->controls.run) run = true;
}

void Player::injectKeyUp(const OIS::KeyEvent &evt)
{
  if (evt.key == flags->controls.moveForward) moveForward = false;
  else if (evt.key == flags->controls.moveBack) moveBack = false;
  else if (evt.key == flags->controls.moveLeft) moveLeft = false;
  else if (evt.key == flags->controls.moveRight) moveRight = false;
  else if (evt.key == flags->controls.run) run = false;
}

void Player::injectMouseMove(const OIS::MouseEvent &evt)
{

  float lookSensitivity = 0.15f;
  if(leftHand || rightHand) lookSensitivity = 0.05f; 
  camera->yaw(Ogre::Degree(-evt.state.X.rel * lookSensitivity));
  camera->pitch(Ogre::Degree(-evt.state.Y.rel * lookSensitivity));
}

void Player::injectMouseDown(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
  if(id == flags->controls.leftHand) leftHand = true;
  if(id == flags->controls.rightHand) rightHand = true;
}

void Player::injectMouseUp(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
  if(id == flags->controls.leftHand) leftHand = false;
  if(id == flags->controls.rightHand) rightHand = false;
}

void Player::stop()
{
  moveForward = false;
  moveBack = false;
  moveLeft = false;
  moveRight = false;
  velocity = Ogre::Vector3::ZERO;
}

Ogre::Vector3 Player::getPosition()
{
  return node->getPosition();
}

