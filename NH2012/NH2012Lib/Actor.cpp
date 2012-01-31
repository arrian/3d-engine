#include "Actor.h"


Actor::Actor(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, 
             Ogre::Vector3 position, int id) 
  : Entity(sceneManager, physics, "actor.mesh", position, id)
{

}


Actor::~Actor(void)
{
}
