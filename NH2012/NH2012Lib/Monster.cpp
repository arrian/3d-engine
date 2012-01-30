#include "Monster.h"


Monster::Monster(Ogre::Vector3 position, int difficulty) : Actor()
{
  health = Bar(difficulty * difficulty);
  magic = Bar(difficulty * difficulty);
  level = Bar(difficulty);

  //generating name
  if(difficulty < 5) name = "jackal";
  if(difficulty < 10) name = "troll";
  if(difficulty < 20) name = "minotaur";
  if(difficulty < 30) name = "arch-lich";
  if(difficulty < 40) name = "Wizard of Yendor";
  if(difficulty < 50) name = "Baalzebub";
  if(difficulty < 60) name = "Demogorgon";

  //filling inventory
  inventory = Inventory(difficulty);

  //filling attributes
  attributes = MonsterAttributes();

  //other
  this->position = position;
  target = Ogre::Vector3(0,0,0);
  speed = 0;
}


Monster::~Monster(void)
{
}

void Monster::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(health.current <= 0) attributes.awareness = MonsterAttribute::DEAD;

  //moving
  if(evt.timeSinceLastFrame == 0) return;//divide by zero... ignore this frame
  position += ((target - position) * speed) * evt.timeSinceLastFrame;
  std::cout << position << std::endl;
}

void Monster::setTarget(Ogre::Vector3 target)
{
  this->target = target;
}

void Monster::animation()
{

}

void Monster::audio()
{

}

void Monster::collision()
{

}
