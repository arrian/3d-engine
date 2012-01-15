#include "Monster.h"


Monster::Monster(int difficulty)
{
  //health
  health = Bar(difficulty * difficulty);
  
  //magic
  magic = Bar(health.getMax());
  
  //level
  level = difficulty;

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
}


Monster::~Monster(void)
{
}

void Monster::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

}