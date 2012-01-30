#include "Player.h"


Player::Player(Dungeon* dungeon, Ogre::Vector3 position) : Actor()
{
  health = Bar(10);
  magic = Bar(10);
  level = Bar(1);

  food = Bar(10);
  water = Bar(10);
  sleep = Bar(10);

  inventory = Inventory();
  attributes = MonsterAttributes();

  this->position = position;
  speed = 1.3;//walking speed

  currentDungeon = dungeon;
}


Player::~Player(void)
{
}

void Player::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(food.current > 0) health.current -= 0.00001 * evt.timeSinceLastFrame;
  if(water.current > 0) water.current -= 0.000015 * evt.timeSinceLastFrame;
  if(sleep.current > 0) sleep.current -= 0.00001 * evt.timeSinceLastFrame;


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

