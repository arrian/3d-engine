#include "Monster.h"


Monster::Monster(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, 
                 Ogre::Vector3 position, int id, int difficulty)
  : Actor(sceneManager, physics, position, id)
{
  health = Bar(Ogre::Real(difficulty * difficulty));
  magic = Bar(Ogre::Real(difficulty * difficulty));
  level = Bar(Ogre::Real(difficulty));

  //generating name
  if(difficulty < 5) entityName = "jackal";
  if(difficulty < 10) entityName = "troll";
  if(difficulty < 20) entityName = "minotaur";
  if(difficulty < 30) entityName = "arch-lich";
  if(difficulty < 40) entityName = "Wizard of Yendor";
  if(difficulty < 50) entityName = "Baalzebub";
  if(difficulty < 60) entityName = "Demogorgon";
   
  //filling inventory
  inventory = Inventory(difficulty);

  //other
  target = Ogre::Vector3(0,51,0);
  speed = 130;
}


Monster::~Monster(void)
{
}

void Monster::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(health.current <= 0) awareness = MonsterAttribute::DEAD;

  //moving
  if(evt.timeSinceLastFrame == 0 || target == node->getPosition()) return;

  Ogre::Vector3 unitDirection = target - node->getPosition();
  Ogre::Real distance = unitDirection.normalise();
  Ogre::Real move = speed * evt.timeSinceLastFrame;
  distance -= move;
  if (distance <= 0.0f)
  {
    node->setPosition(target);
  }
  else
  {
    node->translate(unitDirection * move);
  }
}

void Monster::setTarget(Ogre::Vector3 target)
{
  this->target = target;
}

