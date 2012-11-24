#include "Monster.h"

#include "Scene.h"

//-------------------------------------------------------------------------------------
//Monster::Monster(int id)
Monster::Monster(MonsterDesc desc)
  : BasicComponent(),
    IdentificationInterface(desc.name, "Monster"),
    description(desc),
    node(NULL),
    intelligence(),
    visual(desc.mesh),
    skeleton()
{
  setPosition(Ogre::Vector3::ZERO);
  setTarget(Ogre::Vector3::ZERO);
  skeleton.mapPhysical((IdentificationInterface*) this);
  /*
  health = Bar(Ogre::Real(difficulty * difficulty));
  magic = Bar(Ogre::Real(difficulty * difficulty));
  level = Bar(Ogre::Real(difficulty));
  */
}

//-------------------------------------------------------------------------------------
Monster::~Monster(void)
{

  //scene->getSceneManager()->destroySceneNode(node);
}

//-------------------------------------------------------------------------------------
void Monster::update(double elapsedSeconds)
{
  intelligence.update(elapsedSeconds);
  skeleton.update(elapsedSeconds);
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
  throw NHException("Monsters comparison operator not implemented.");
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

