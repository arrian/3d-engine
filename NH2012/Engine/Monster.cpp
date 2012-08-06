#include "Monster.h"

#include "Scene.h"

//-------------------------------------------------------------------------------------
Monster::Monster(int id)
  : BasicComponent(),
    speed(200),
    node(0),
    inventory(),
    intelligence(),
    visual("actor.mesh"),
    skeleton()
{
  setPosition(Ogre::Vector3::ZERO);
  setTarget(Ogre::Vector3::ZERO);

  /*
  health = Bar(Ogre::Real(difficulty * difficulty));
  magic = Bar(Ogre::Real(difficulty * difficulty));
  level = Bar(Ogre::Real(difficulty));

  //generating name
  if(difficulty < 5) name = "jackal";
  else if(difficulty < 10) name = "troll";
  else if(difficulty < 20) name = "minotaur";
  else if(difficulty < 30) name = "arch-lich";
  else if(difficulty < 40) name = "Wizard of Yendor";
  else if(difficulty < 50) name = "Baalzebub";
  else if(difficulty < 60) name = "Demogorgon";
  */

}

//-------------------------------------------------------------------------------------
Monster::~Monster(void)
{

  //scene->getSceneManager()->destroySceneNode(node);
}

//-------------------------------------------------------------------------------------
void Monster::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  intelligence.frameRenderingQueued(evt);
  skeleton.frameRenderingQueued(evt);
  //if(health.getCurrent() <= 0) awareness = MonsterAttribute::DEAD;

  //moving
  /*
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
  }*/
}

//-------------------------------------------------------------------------------------
bool operator==(const Monster& x, const Monster& y)
{
  return false;//(x.name == y.name);
}

//-------------------------------------------------------------------------------------
void Monster::hasSceneChange()
{
  if(oldScene && node) oldScene->getSceneManager()->destroySceneNode(node);
  node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();
  node->setPosition(position);

  intelligence.setNode(scene, node);
  visual.setNode(scene, node);
  skeleton.setNode(scene, node);
}

//-------------------------------------------------------------------------------------
void Monster::setPosition(Ogre::Vector3 position)
{
  this->position = position;
}

//-------------------------------------------------------------------------------------
void Monster::setTarget(Ogre::Vector3 target)
{
  this->target = target;
}

