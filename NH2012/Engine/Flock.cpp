#include "Flock.h"


Flock::Flock(Ogre::SceneManager* manager)
  : flock()
{
  node = manager->getRootSceneNode()->createChildSceneNode();

  //flock testing
  for(int i = 0; i < 10; i++)
  {
    addBody(new FlockBody(node->createChildSceneNode(), this));
  }
}


Flock::~Flock(void)
{
}

void Flock::frameRenderingQueued(Ogre::FrameEvent& evt)
{
  for(std::vector<FlockBody*>::iterator iter = flock.begin(); iter < flock.end(); ++iter)
  {

  }
}

bool Flock::addBody(FlockBody *body)
{
  flock.push_back(body);
}

std::vector<FlockBody*> Flock::getBodiesInRadius(FlockBody* body, float radius)
{
  std::vector<FlockBody*> collection = std::vector<FlockBody*>();
  for(std::vector<FlockBody*>::iterator iter = flock.begin(); iter < flock.end(); ++iter)
  {
    if((*iter)->getPosition().distance(body->getPosition()) < radius) collection.push_back((*iter));
  }

  return collection;
}

