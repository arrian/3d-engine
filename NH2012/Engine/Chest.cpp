#include "Chest.h"

#include "NHException.h"


Chest::Chest(void)
  : Interactive(InteractiveDesc(0, "Chest", "invalid.script")),
    open(false),
    locked(false)
{
}

//-------------------------------------------------------------------------------------
Chest::~Chest(void)
{
}

//-------------------------------------------------------------------------------------
void Chest::setOpen(bool open)
{
  this->open = open;
}

//-------------------------------------------------------------------------------------
void Chest::setLocked(bool locked)
{
  this->locked = locked;
}

//-------------------------------------------------------------------------------------
bool Chest::isOpen()
{
  return open;
}

//-------------------------------------------------------------------------------------
bool Chest::isLocked()
{
  return locked;
}

//-------------------------------------------------------------------------------------
void Chest::interact()
{
  setOpen(!isOpen());
}

//-------------------------------------------------------------------------------------
void Chest::update(double elapsedSeconds)
{

}

//-------------------------------------------------------------------------------------
void Chest::hasSceneChange()
{

}

//-------------------------------------------------------------------------------------
void Chest::setPosition(Vector3 position)
{
  if(node) node->setPosition(position);
  else throw NHException("chest must be inside a scene to set its position");
}

//-------------------------------------------------------------------------------------
void Chest::setRotation(Quaternion rotation)
{
  if(node) node->setOrientation(rotation);
  else throw NHException("chest must be inside a scene to set its rotation");
}

//-------------------------------------------------------------------------------------
Vector3 Chest::getPosition()
{
  if(node) return node->getPosition();
  throw NHException("chest must be inside a scene to get its position");
}

//-------------------------------------------------------------------------------------
Quaternion Chest::getRotation()
{
  if(node) node->getOrientation();
  throw NHException("chest must be inside a scene to get its rotation");
}