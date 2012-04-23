#include "Player.h"

Player::Player(World* world) 
  : velocity(Ogre::Vector3::ZERO),
    gravityVector(Ogre::Vector3::ZERO),
    moveForward(false), 
    moveBack(false),
    moveLeft(false), 
    moveRight(false), 
    run(false),
    leftHand(false),
    rightHand(false),
    world(world),
    inventory(),
    speed(200),
    cell(0),
    oldCameraWidth(0),
    oldCameraHeight(0),
    window(0),
    vp(0)
{
}

Player::~Player(void)
{
  if(camera) delete camera;
}

void Player::setCell(Cell* cell, Ogre::Vector3 position, Ogre::Vector3 lookAt)
{
  if(this->cell)
  {
    delete capsuleBody;
    delete capsule;
    this->cell->getSceneManager()->destroyCamera(camera);
    this->cell->getSceneManager()->destroySceneNode(cameraNode);
    this->cell->getSceneManager()->destroySceneNode(leftHandTarget);
    this->cell->getSceneManager()->destroySceneNode(rightHandTarget);
    this->cell->getSceneManager()->destroySceneNode(node);
    this->cell->getSceneManager()->destroyEntity(entity);
  }

  this->cell = cell;
  entity = cell->getSceneManager()->createEntity("actor.mesh");
  node = cell->getSceneManager()->getRootSceneNode()->createChildSceneNode();
  node->setPosition(position);
  node->attachObject(entity);

  node->showBoundingBox(true);

  //hands
  leftHandTarget = node->createChildSceneNode();
  leftHandTarget->setPosition(Ogre::Vector3(10,0,10));
  rightHandTarget = node->createChildSceneNode();
  rightHandTarget->setPosition(Ogre::Vector3(10,0,-10));

  //camera
  cameraNode = node->createChildSceneNode(Ogre::Vector3(0,30,0));
  camera = cell->getSceneManager()->createCamera("PlayerCamera");
  
  camera->lookAt(lookAt);
  camera->setNearClipDistance(5);
  cameraNode->attachObject(camera);

  if(world->wireframeDebug) camera->setPolygonMode(Ogre::PM_WIREFRAME);

  //physics
  physics = cell->getPhysicsWorld();
  capsule = new OgreBulletCollisions::CapsuleCollisionShape(15.0f, 50.0f, Ogre::Vector3(0,1,0));
  capsuleBody = new OgreBulletDynamics::RigidBody("character", physics);
  capsuleBody->setShape(node, capsule, 0.0f, 0.0f, 10.0f, getPosition());
  capsuleBody->getBulletRigidBody()->setAngularFactor(0.0);
  capsuleBody->getBulletRigidBody()->setSleepingThresholds(0.0, 0.0);

  if(window) hook(window);//if the player has a render window then connect camera to new cell

  stop();
}

void Player::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

  //Checking aspect ratio for resolution changes... find better way of doing this
  if(vp->getActualWidth() != oldCameraWidth || vp->getActualHeight() != oldCameraHeight)
  {
    camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
    oldCameraWidth = vp->getActualWidth();
    oldCameraHeight = vp->getActualHeight();
  }
 
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

  if (world->freeCameraDebug) 
  {
    if(velocity != Ogre::Vector3::ZERO) camera->move(velocity * evt.timeSinceLastFrame);
    node->setPosition(camera->getPosition());
  }
  else 
  {
    if(velocity != Ogre::Vector3::ZERO && capsuleBody->getLinearVelocity() != Ogre::Vector3::ZERO) capsuleBody->setLinearVelocity(velocity);
  }
}

void Player::injectKeyDown(const OIS::KeyEvent &evt)
{
  if (evt.key == world->controls.moveForward) moveForward = true;
  else if (evt.key == world->controls.moveBack) moveBack = true;
  else if (evt.key == world->controls.moveLeft) moveLeft = true;
  else if (evt.key == world->controls.moveRight) moveRight = true;
  else if (evt.key == world->controls.run) run = true;
}

void Player::injectKeyUp(const OIS::KeyEvent &evt)
{
  if (evt.key == world->controls.moveForward) moveForward = false;
  else if (evt.key == world->controls.moveBack) moveBack = false;
  else if (evt.key == world->controls.moveLeft) moveLeft = false;
  else if (evt.key == world->controls.moveRight) moveRight = false;
  else if (evt.key == world->controls.run) run = false;
}

void Player::injectMouseMove(const OIS::MouseEvent &evt)
{
  float lookResponsiveness = 0.15f;
  float handResponsiveness = 0.15f;
  if(leftHand || rightHand) lookResponsiveness = 0.05f;

  Ogre::Degree xHand = Ogre::Degree(-evt.state.X.rel * lookResponsiveness);
  Ogre::Degree yHand = Ogre::Degree(-evt.state.Y.rel * lookResponsiveness);

  Ogre::Degree xLook = Ogre::Degree(-evt.state.X.rel * lookResponsiveness);
  Ogre::Degree yLook = Ogre::Degree(-evt.state.Y.rel * lookResponsiveness);

  if(leftHand)
  {
    leftHandTarget->yaw(xHand);
    leftHandTarget->pitch(yHand);
  }
  if(rightHand)
  {
    rightHandTarget->yaw(xHand);
    rightHandTarget->pitch(yHand);
  }

  camera->yaw(xLook);
  camera->pitch(yLook);
  leftHandTarget->yaw(xLook);
  leftHandTarget->pitch(yLook);
  rightHandTarget->yaw(xLook);
  rightHandTarget->pitch(yLook);
}

void Player::injectMouseDown(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
  if(id == world->controls.leftHand) leftHand = true;
  if(id == world->controls.rightHand) rightHand = true;
}

void Player::injectMouseUp(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
  if(id == world->controls.leftHand) leftHand = false;
  if(id == world->controls.rightHand) rightHand = false;
}

void Player::hook(Ogre::RenderWindow* window)
{
  assert(window && cell);
  this->window = window;
  vp = 0;
  window->removeAllViewports();
  vp = window->addViewport(camera);
  vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
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



