#include "Creature.h"

#include "Scene.h"
#include "NHException.h"

#include "SceneGraphicsManager.h"

//-------------------------------------------------------------------------------------
Creature::Creature(CreatureDesc desc)
  : Actor(this, desc.name, CREATURE),
    description(desc),
    intelligence(desc.speed, desc.gravity),
    mesh(desc.mesh),
    skeleton()
{
  skeleton.setUserData((Identifiable*) this);
  skeleton.setGroup(CREATURE);
}

//-------------------------------------------------------------------------------------
Creature::~Creature(void)
{

}

//-------------------------------------------------------------------------------------
void Creature::update(double elapsedSeconds)
{
  intelligence.update(elapsedSeconds);
  skeleton.update(elapsedSeconds);
}

//-------------------------------------------------------------------------------------
void Creature::hasSceneChange()
{
  //pulling down
  if(oldScene)
  {
    if(node) oldScene->getSceneGraphicsManager()->destroySceneNode(node);
    node = NULL;
  }
  
  //setting up
  if(scene)
  {
    node = scene->getSceneGraphicsManager()->createSceneNode();

    setPosition(position);

    intelligence.setNode(scene, node);
    mesh.setNode(scene, node);
    skeleton.setNode(scene, node);
  }
}

//-------------------------------------------------------------------------------------
void Creature::setPosition(Vector3 position)
{
  this->position = position;
  if(node) 
  {
    node->setPosition(position);
    intelligence.setPosition(position);
  }
}

//-------------------------------------------------------------------------------------
void Creature::setRotation(Quaternion rotation)
{
  //set node horizontal direction here
  throw NHException("setting the rotation of a creature has not been implemented");
}

//-------------------------------------------------------------------------------------
void Creature::setGoal(Goal* goal)
{
  intelligence.setGoal(goal);
}

//-------------------------------------------------------------------------------------
void Creature::stop()
{
  skeleton.stop();
  intelligence.setGoal(NULL);
}

//-------------------------------------------------------------------------------------
void Creature::stagger(Vector3 direction)
{
  throw NHException("not implemented");
}

//-------------------------------------------------------------------------------------
void Creature::damage(double amount)
{
  throw NHException("not implemented");
}

//-------------------------------------------------------------------------------------
void Creature::heal(double amount)
{
  throw NHException("not implemented");
}

//-------------------------------------------------------------------------------------
void Creature::setLookAt(Vector3 lookAt)
{
  skeleton.lookAt(lookAt);
}

//-------------------------------------------------------------------------------------
void Creature::setRunning(bool running)
{
  intelligence.setRun(running);
}

//-------------------------------------------------------------------------------------
void Creature::setCrouching(bool crouching)
{
  skeleton.setCrouch(crouching);
}

//-------------------------------------------------------------------------------------
void Creature::setGravity(Vector3 gravity)
{
  intelligence.setGravity(gravity);
}

//-------------------------------------------------------------------------------------
Vector3 Creature::getPosition()
{
  return node->getPosition();
}

//-------------------------------------------------------------------------------------
Quaternion Creature::getRotation()
{
  return node->getOrientation();
}

//-------------------------------------------------------------------------------------
bool Creature::getCrouching()
{
  return skeleton.isCrouched();
}

//-------------------------------------------------------------------------------------
bool Creature::getRunning()
{
  return intelligence.isRunning();
}

//-------------------------------------------------------------------------------------
Vector3 Creature::getGravity()
{
  return intelligence.getGravity();
}

//-------------------------------------------------------------------------------------
void Creature::integratePacket(CreaturePacket packet)
{
  setPosition(packet.position);//TODO: interpolate with old position
}

//-------------------------------------------------------------------------------------
CreaturePacket Creature::extractPacket()
{
  CreaturePacket packet = CreaturePacket();
  packet.position = getPosition();
  return packet;
}
