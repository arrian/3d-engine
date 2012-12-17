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
      int code = scene->getPathfindManager()->FindPath(node->_getDerivedPosition(), goal, 0, 0);
      intermediates = scene->getPathfindManager()->getPath(0);

      switch(code)
      {
        case 0: break;
        case -1: std::cout << "Monster Pathfind: No polygon near start point." << std::endl; break;
        case -2: std::cout << "Monster Pathfind: No polygon near end point." << std::endl; break;
        case -3: std::cout << "Monster Pathfind: Could not create a path." << std::endl; break;
        case -4: std::cout << "Monster Pathfind: Could not find a path." << std::endl; break;
        case -5: std::cout << "Monster Pathfind: Could not create a straight path." << std::endl; break;
        case -6: std::cout << "Monster Pathfind: Could not find a straight path." << std::endl; break;
        default: break;
      }

      //debug generated path
      int count = 0;
      for(std::vector<Ogre::Vector3>::iterator iter = intermediates.begin(); iter < intermediates.end(); ++iter)
      {
        std::cout << count << ": " << iter->x << "," << iter->y << "," << iter->z << std::endl;
        count++;
      }
    }
    if(intermediates.size() == 0) return result;//no points found


    Ogre::Vector3 unitDirection = intermediates.back() - node->_getDerivedPosition();
    Ogre::Real distance = unitDirection.normalise();
    Ogre::Real move = speed * (float) elapsedSeconds;
    distance -= move;

    //if(distance <= 0.0f)
    //{
      //need to remove intermediate
    //  result = intermediates.front();
    //  intermediates.erase(intermediates.begin());
    //}
    //else
    //{
      result += (unitDirection * move);
    //}

  }
  std::cout << result.x << "," << result.y << "," << result.z << std::endl;
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

