#pragma once

#include <vector>
#include <Ogre.h>
#include "FlockBody.h"

class Flock
{
public:
  Flock(Ogre::SceneManager* manager);
  ~Flock(void);

  void frameRenderingQueued(Ogre::FrameEvent& evt);
  bool addBody(FlockBody *body);
  std::vector<FlockBody*> getBodiesInRadius(FlockBody* body, float radius);

private:
  Ogre::SceneNode* node;
  std::vector<FlockBody*> flock;
};

