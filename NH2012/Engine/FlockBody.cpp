#include "FlockBody.h"


FlockBody::FlockBody(Ogre::SceneNode* node, Flock* flock)
  : node(node),
    flock(flock),
    velocity(Ogre::Vector3(0, 0, 0)),
    direction(Ogre::Vector3(0, 0, 0)),
    size(0.1f)
{

}


FlockBody::~FlockBody(void)
{

}

Ogre::Vector3 FlockBody::getPosition()
{
  return node->getPosition();
}

Ogre::Vector3 FlockBody::getVelocity()
{
  return velocity;
}

Ogre::Vector3 FlockBody::getDirection()
{
  return direction;

}

float FlockBody::getSize()
{
  return size;

}

void FlockBody::frameRenderingQueued(Ogre::FrameEvent& evt)
{
  Ogre::Vector3 cohesion = Ogre::Vector3(0, 0, 0);
  Ogre::Vector3 alignment = Ogre::Vector3(0, 0, 0);
  Ogre::Vector3 separation = Ogre::Vector3(0, 0, 0);

  int cohesionCount = 0;
  int alignmentCount = 0;
  int separationCount = 0;

  std::vector<FlockBody*>::iterator iter;
  std::vector<FlockBody*> bodiesInRadius = flock->getBodiesInRadius(this, 2.0f);

  for (iter = bodiesInRadius.begin(); iter != bodiesInRadius.end(); ++iter) 
  {
    if ((*iter) != this)
    {

      Ogre::Vector3 p1 = getPosition();
      Ogre::Vector3 p2 = (*iter)->getPosition();
      Ogre::Real distance = p1.distance(p2);

      if (distance <= 10 * size) 
      {
        //cohesion
        cohesionCount++;
        cohesion += (*iter)->getPosition();

        //alignment
        alignmentCount++;
        alignment += (*iter)->getVelocity();
      }

      if (distance <= 2.5 * size) 
      {
        //separation
        separationCount++;

        Ogre::Vector3 away = getPosition() - (*iter)->getPosition();
        away.normalise();
        away *= size;

        away/=(distance/2);
        separation += away;
      }
    }
  }

  //Averaging values
  cohesion /= cohesionCount;
  alignment /= alignmentCount;
  separation /= separationCount;

  //Applying time factor
  float time = evt.timeSinceLastFrame / 10;
  cohesion *= time;
  alignment *= time;
  separation *= time;

  direction += alignment;
  node->setPosition(getPosition() + cohesion);
  node->setPosition(getPosition() - separation);
}