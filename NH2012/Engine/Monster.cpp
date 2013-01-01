#include "Monster.h"

#include "Scene.h"

//-------------------------------------------------------------------------------------
Monster::Monster(MonsterDesc desc)
  : Actor(this, desc.name, MONSTER),
    description(desc),
    intelligence(desc.speed),
    mesh(desc.mesh),
    //movement(desc.gravity),
    skeleton()
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
  //movement.update(elapsedSeconds);
  skeleton.update(elapsedSeconds);
}

//-------------------------------------------------------------------------------------
void Monster::hasSceneChange()
{
  //pulling down
  if(oldScene)
  {
    if(node) oldScene->getSceneManager()->destroySceneNode(node);
    node = NULL;
  }
  
  //setting up
  if(scene)
  {
    node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();

    setPosition(position);

    intelligence.setNode(scene, node);
    mesh.setNode(scene, node);
    skeleton.setNode(scene, node);
  }
}

//-------------------------------------------------------------------------------------
void Monster::setPosition(Vector3 position)
{
  this->position = position;
  if(node) 
  {
    node->setPosition(position);
    intelligence.setPosition(position);
  }
}

//-------------------------------------------------------------------------------------
void Monster::setRotation(Quaternion rotation)
{
  //set node horizontal direction here
  throw NHException("setting the rotation of a monster has not been implemented");
}

//-------------------------------------------------------------------------------------
void Monster::setGoal(Goal* goal)
{
  intelligence.setGoal(goal);
}

//-------------------------------------------------------------------------------------
void Monster::stop()
{
  skeleton.stop();
  intelligence.setGoal(NULL);
}

//-------------------------------------------------------------------------------------
void Monster::stagger(Vector3 direction)
{
  throw NHException("not implemented");
}

//-------------------------------------------------------------------------------------
void Monster::damage(double amount)
{
  throw NHException("not implemented");
}

//-------------------------------------------------------------------------------------
void Monster::heal(double amount)
{
  throw NHException("not implemented");
}

//-------------------------------------------------------------------------------------
void Monster::setLookAt(Vector3 lookAt)
{
  throw NHException("not implemented");
}

//-------------------------------------------------------------------------------------
void Monster::setRunning(bool running)
{
  throw NHException("not implemented");
}

//-------------------------------------------------------------------------------------
void Monster::setCrouching(bool crouching)
{
  throw NHException("not implemented");
}

//-------------------------------------------------------------------------------------
void Monster::setGravity(Vector3)
{
  throw NHException("not implemented");
}

//-------------------------------------------------------------------------------------
Vector3 Monster::getPosition()
{
  throw NHException("not implemented");
}

//-------------------------------------------------------------------------------------
Quaternion Monster::getRotation()
{
  throw NHException("not implemented");
}

//-------------------------------------------------------------------------------------
bool Monster::getCrouching()
{
  throw NHException("not implemented");
}

//-------------------------------------------------------------------------------------
bool Monster::getRunning()
{
  throw NHException("not implemented");
}

//-------------------------------------------------------------------------------------
Vector3 Monster::getGravity()
{
  throw NHException("not implemented");
}


