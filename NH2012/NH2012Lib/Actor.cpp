#include "Actor.h"


Actor::Actor(Ogre::SceneManager* sceneManager, Ogre::Vector3 position) : Entity(sceneManager, "actor.mesh", position)
{
}


Actor::~Actor(void)
{
}
