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
    skeleton(desc.gravity)
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

}

//-------------------------------------------------------------------------------------
void Monster::update(double elapsedSeconds)
{
  scene->getPathfindManager()->FindPath(node->getPosition(), target, getInstanceID(), 0);//final argument is the target id which is not used by recast
  skeleton.followPath(scene->getPathfindManager()->getPath(getInstanceID()));

  intelligence.update(elapsedSeconds);
  skeleton.update(elapsedSeconds);
  


  //if(elapsedSeconds == 0 || target == node->getPosition()) return;

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


  //temporary testing
  setPosition(scene->getPathfindManager()->getRandomNavMeshPoint());
  setTarget(scene->getPathfindManager()->getRandomNavMeshPoint());


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

