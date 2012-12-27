#include "IntelligenceComponent.h"

#include "Scene.h"

//-------------------------------------------------------------------------------------
IntelligenceComponent::IntelligenceComponent(float speed)
  : intermediates(),
    goal(Ogre::Vector3::ZERO),
    hasGoal(false),
    speed(speed)
{
}

//-------------------------------------------------------------------------------------
IntelligenceComponent::~IntelligenceComponent(void)
{
}

//-------------------------------------------------------------------------------------
void IntelligenceComponent::setGoal(Ogre::Vector3 to)
{
  hasGoal = true;
  goal = to;
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 IntelligenceComponent::getNextPosition(double elapsedSeconds)
{
  Ogre::Vector3 result = node->_getDerivedPosition();
  if(hasGoal) 
  {
    if(intermediates.size() == 0)
    {
      intermediates = scene->getPathfindManager()->getPath(node->_getDerivedPosition(), goal);

      //debug generated path
      int count = 0;
      for(std::vector<Ogre::Vector3>::iterator iter = intermediates.begin(); iter < intermediates.end(); ++iter)
      {
        std::cout << count << ": " << iter->x << "," << iter->y << "," << iter->z << std::endl;
        count++;
      }
    }
    if(intermediates.size() == 0) return result;//no points found


    Ogre::Vector3 unitDirection = intermediates.front() - node->_getDerivedPosition();
    Ogre::Real distance = unitDirection.normalise();
    Ogre::Real move = speed * (float) elapsedSeconds;
    distance -= move;

    if (distance <= 0.0f)
    {
      result = intermediates.front();
      intermediates.erase(intermediates.begin());
    }
    else
    {
      result += (unitDirection * move) + Ogre::Vector3(0,1,0);
    }

  }
  //std::cout << result.x << "," << result.y << "," << result.z << std::endl;
  return result;
}

//-------------------------------------------------------------------------------------
void IntelligenceComponent::hasNodeChange()
{


}

//-------------------------------------------------------------------------------------
void IntelligenceComponent::update(double elapsedSeconds)
{
  if(!node) return;
  node->_setDerivedPosition(getNextPosition(elapsedSeconds));
}

