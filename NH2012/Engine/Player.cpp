#include "Player.h"

Player::Player(Environment* environment, Ogre::RenderWindow* window) 
  : velocity(Ogre::Vector3::ZERO),
    gravityVector(Ogre::Vector3::ZERO),
    moveForward(false), 
    moveBack(false),
    moveLeft(false), 
    moveRight(false), 
    run(false),
    leftHand(false),
    rightHand(false),
    window(window),
    environment(environment),
    inventory(),
    speed(200),
    cell(0),
    oldCameraWidth(0),
    oldCameraHeight(0),
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
    this->cell->getSceneManager()->destroySceneNode(node);
    this->cell->getSceneManager()->destroyEntity(entity);
  }

  this->cell = cell;

  entity = cell->getSceneManager()->createEntity("actor.mesh");
  node = cell->getSceneManager()->getRootSceneNode()->createChildSceneNode();
  node->setPosition(position);
  node->attachObject(entity);

  //camera
  cameraNode = node->createChildSceneNode(Ogre::Vector3(0,30,0));
  camera = cell->getSceneManager()->createCamera("PlayerCamera");
  
  camera->lookAt(lookAt);
  camera->setNearClipDistance(5);
  cameraNode->attachObject(camera);

  if(environment->wireframeDebug) camera->setPolygonMode(Ogre::PM_WIREFRAME);

  // Create one viewport, entire window
  //window->removeAllViewports();//ensure no viewports remaining
  //vp = window->addViewport(camera);
  //vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

  //physics
  physics = cell->getPhysicsWorld();
  capsule = new OgreBulletCollisions::CapsuleCollisionShape(15.0f, 50.0f, Ogre::Vector3(0,1,0));
  capsuleBody = new OgreBulletDynamics::RigidBody("character", physics);
  capsuleBody->setShape(node, capsule, 0.0f, 0.0f, 10.0f, getPosition());
  capsuleBody->getBulletRigidBody()->setAngularFactor(0.0);
  capsuleBody->getBulletRigidBody()->setSleepingThresholds(0.0, 0.0);

  hook(window);//hooks the render window to the new camera

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

  if (environment->freeCameraDebug) 
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

void Player::injectKeyDown(const OIS::KeyEvent &evt)
{
  if (evt.key == environment->controls.moveForward) moveForward = true;
  else if (evt.key == environment->controls.moveBack) moveBack = true;
  else if (evt.key == environment->controls.moveLeft) moveLeft = true;
  else if (evt.key == environment->controls.moveRight) moveRight = true;
  else if (evt.key == environment->controls.run) run = true;
}

void Player::injectKeyUp(const OIS::KeyEvent &evt)
{
  if (evt.key == environment->controls.moveForward) moveForward = false;
  else if (evt.key == environment->controls.moveBack) moveBack = false;
  else if (evt.key == environment->controls.moveLeft) moveLeft = false;
  else if (evt.key == environment->controls.moveRight) moveRight = false;
  else if (evt.key == environment->controls.run) run = false;
}

void Player::injectMouseMove(const OIS::MouseEvent &evt)
{
  float lookResponsiveness = 0.15f;
  if(leftHand || rightHand) lookResponsiveness = 0.05f; 
  camera->yaw(Ogre::Degree(-evt.state.X.rel * lookResponsiveness));
  camera->pitch(Ogre::Degree(-evt.state.Y.rel * lookResponsiveness));
}

void Player::injectMouseDown(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
  if(id == environment->controls.leftHand) leftHand = true;
  if(id == environment->controls.rightHand) rightHand = true;
}

void Player::injectMouseUp(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
  if(id == environment->controls.leftHand) leftHand = false;
  if(id == environment->controls.rightHand) rightHand = false;
}

void Player::hook(Ogre::RenderWindow* window)
{
  window->removeAllViewports();
  //vp has old pointer for a moment here. not thread safe.
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

void Player::moveLeftHand(const OIS::MouseEvent& evt)
{
  float handResponsiveness = 0.15f;
  leftHandTarget += Ogre::Vector3(evt.state.X.rel * handResponsiveness, 0, evt.state.Y.rel * handResponsiveness);
}

void Player::moveRightHand(const OIS::MouseEvent& evt)
{
  float handResponsiveness = 0.15f;
  rightHandTarget += Ogre::Vector3(evt.state.X.rel * handResponsiveness, 0, evt.state.Y.rel * handResponsiveness);
}


