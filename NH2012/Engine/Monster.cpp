#include "Monster.h"

#include "Scene.h"

//-------------------------------------------------------------------------------------
Monster::Monster(MonsterDesc desc)
  : BasicComponent(),
    IdentificationInterface(this, desc.name, MONSTER),
    description(desc),
    node(NULL),
    intelligence(desc.speed),
    mesh(desc.mesh),
    skeleton(desc.gravity),
    position(Ogre::Vector3::ZERO)
{
  skeleton.setUserData((IdentificationInterface*) this);
  skeleton.setGroup(MONSTER);
}

//-------------------------------------------------------------------------------------
Monster::~Monster(void)
{

}

//-------------------------------------------------------------------------------------
void Monster::update(double elapsedSeconds)
{
  intelligence.update(elapsedSeconds);
  skeleton.update(elapsedSeconds);
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
  
  if(!scene) return;

  node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();

  //temporary testing
  //setPosition(scene->getPathfindManager()->getRandomNavMeshPoint() + Ogre::Vector3(0.0f,2.0f,0.0f));
  setTarget(scene->getPathfindManager()->getRandomNavMeshPoint() + Ogre::Vector3(0.0f,2.0f,0.0f));

  node->_setDerivedPosition(position);

  intelligence.setNode(scene, node);
  mesh.setNode(scene, node);
  skeleton.setNode(scene, node);
}

//-------------------------------------------------------------------------------------
void Monster::setPosition(Ogre::Vector3 position)
{
  this->position = position;
  if(node) node->_setDerivedPosition(position);
}

//-------------------------------------------------------------------------------------
void Monster::setTarget(Ogre::Vector3 target)
{
  intelligence.setGoal(target);
  this->target = target;
}

