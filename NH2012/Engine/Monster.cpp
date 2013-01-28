#include "Monster.h"

#include "Scene.h"
#include "NHException.h"

//-------------------------------------------------------------------------------------
Monster::Monster(MonsterDesc desc)
  : Actor(this, desc.name, MONSTER),
    description(desc),
    intelligence(desc.speed, desc.gravity),
    mesh(desc.mesh),
    skeleton()
{
  skeleton.setUserData((Identifiable*) this);
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
void Monster::hasSceneChange()
{
  //pulling down
  if(oldScene)
  {
    if(node) oldScene->getGraphicsManager()->destroySceneNode(node);
    node = NULL;
  }
  
  //setting up
  if(scene)
  {
    node = scene->getGraphicsManager()->getRootSceneNode()->createChildSceneNode();

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
  skeleton.lookAt(lookAt);
}

//-------------------------------------------------------------------------------------
void Monster::setRunning(bool running)
{
  intelligence.setRun(running);
}

//-------------------------------------------------------------------------------------
void Monster::setCrouching(bool crouching)
{
  skeleton.setCrouch(crouching);
}

//-------------------------------------------------------------------------------------
void Monster::setGravity(Vector3 gravity)
{
  intelligence.setGravity(gravity);
}

//-------------------------------------------------------------------------------------
Vector3 Monster::getPosition()
{
  return node->getPosition();
}

//-------------------------------------------------------------------------------------
Quaternion Monster::getRotation()
{
  return node->getOrientation();
}

//-------------------------------------------------------------------------------------
bool Monster::getCrouching()
{
  return skeleton.isCrouched();
}

//-------------------------------------------------------------------------------------
bool Monster::getRunning()
{
  return intelligence.isRunning();
}

//-------------------------------------------------------------------------------------
Vector3 Monster::getGravity()
{
  return intelligence.getGravity();
}

//-------------------------------------------------------------------------------------
void Monster::integratePacket(MonsterPacket packet)
{
  setPosition(packet.position);//TODO: interpolate with old position
}

//-------------------------------------------------------------------------------------
MonsterPacket Monster::extractPacket()
{
  MonsterPacket packet = MonsterPacket();
  packet.position = getPosition();
  return packet;
}
