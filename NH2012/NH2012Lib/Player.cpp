#include "Player.h"


Player::Player(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, 
               Ogre::RenderWindow* window, Ogre::Vector3 position) 
  : Actor(sceneManager, physics, position), 
    mVelocity(Ogre::Vector3::ZERO), 
    mGoingForward(false), 
    mGoingBack(false), 
    mGoingLeft(false), 
    mGoingRight(false), 
    mGoingUp(false), 
    mGoingDown(false), 
    mFastMove(false)
{
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
  camera = sceneManager->createCamera("PlayerCamera");
  camera->setPosition(Ogre::Vector3(0,100,80));
  camera->lookAt(Ogre::Vector3(0,100,-300));
  camera->setNearClipDistance(5);

  //cameraMovement = new OgreBites::SdkCameraMan(camera);

  // Create one viewport, entire window
  Ogre::Viewport* vp = window->addViewport(camera);
  vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

  // Alter the camera aspect ratio to match the viewport
  camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
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
  if (mGoingForward) accel += camera->getDirection();
  if (mGoingBack) accel -= camera->getDirection();
  if (mGoingRight) accel += camera->getRight();
  if (mGoingLeft) accel -= camera->getRight();
  if (mGoingUp) accel += camera->getUp();
  if (mGoingDown) accel -= camera->getUp();

  // if accelerating, try to reach top speed in a certain time
  Ogre::Real topSpeed = mFastMove ? speed * 2 : speed;
  if (accel.squaredLength() != 0)
  {
    accel.normalise();
    mVelocity += accel * topSpeed * evt.timeSinceLastFrame * 10;
  }
  // if not accelerating, try to stop in a certain time
  else mVelocity -= mVelocity * evt.timeSinceLastFrame * 10;

  Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

  // keep camera velocity below top speed and above epsilon
  if (mVelocity.squaredLength() > topSpeed * topSpeed)
  {
    mVelocity.normalise();
    mVelocity *= topSpeed;
  }
  else if (mVelocity.squaredLength() < tooSmall * tooSmall)
    mVelocity = Ogre::Vector3::ZERO;

  if (mVelocity != Ogre::Vector3::ZERO) camera->move(mVelocity * evt.timeSinceLastFrame);
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
  if (evt.key == OIS::KC_W || evt.key == OIS::KC_UP) mGoingForward = true;
  else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN) mGoingBack = true;
  else if (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT) mGoingLeft = true;
  else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT) mGoingRight = true;
  else if (evt.key == OIS::KC_PGUP) mGoingUp = true;
  else if (evt.key == OIS::KC_PGDOWN) mGoingDown = true;
  else if (evt.key == OIS::KC_LSHIFT) mFastMove = true;
}

void Player::injectKeyUp(const OIS::KeyEvent &evt)
{
  if (evt.key == OIS::KC_W || evt.key == OIS::KC_UP) mGoingForward = false;
  else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN) mGoingBack = false;
  else if (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT) mGoingLeft = false;
  else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT) mGoingRight = false;
  else if (evt.key == OIS::KC_PGUP) mGoingUp = false;
  else if (evt.key == OIS::KC_PGDOWN) mGoingDown = false;
  else if (evt.key == OIS::KC_LSHIFT) mFastMove = false;
}

void Player::injectMouseMove(const OIS::MouseEvent &evt)
{
  camera->yaw(Ogre::Degree(-evt.state.X.rel * 0.15f));
  camera->pitch(Ogre::Degree(-evt.state.Y.rel * 0.15f));
}

void Player::injectMouseDown(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
  
}

void Player::injectMouseUp(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{

}

void Player::stop()
{
  mGoingForward = false;
  mGoingBack = false;
  mGoingLeft = false;
  mGoingRight = false;
  mGoingUp = false;
  mGoingDown = false;
  mVelocity = Ogre::Vector3::ZERO;
}

Ogre::Vector3 Player::getPosition()
{
  return camera->getPosition();
}