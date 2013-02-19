#include "IntelligentMovementComponent.h"

#include "Scene.h"
#include "Monster.h"
#include "Goal.h"
#include "NHException.h"

#include "ScenePathfindManager.h"

//-------------------------------------------------------------------------------------
IntelligentMovementComponent::IntelligentMovementComponent(float speed, Vector3 gravity)
  : MovementComponent(gravity),
    goals(),
    nextGoal(NULL),
    speed(speed),
    agent(NULL)
{
}

//-------------------------------------------------------------------------------------
IntelligentMovementComponent::~IntelligentMovementComponent(void)
{
}

//-------------------------------------------------------------------------------------
void IntelligentMovementComponent::setGoal(Goal* goal)
{
  goals.clear();//delete contents?
  goals.push_back(goal);
  checkNextGoal();
}

//-------------------------------------------------------------------------------------
void IntelligentMovementComponent::addGoal(Goal* goal)
{
  goals.push_back(goal);
  checkNextGoal();
}

//-------------------------------------------------------------------------------------
void IntelligentMovementComponent::removeGoal(Goal* goal)
{
  goals.erase(std::find(goals.begin(), goals.end(), goal));
  if(nextGoal == goal)
  {
    nextGoal = NULL;
    checkNextGoal();
  }
}

//-------------------------------------------------------------------------------------
Goal* IntelligentMovementComponent::getNextGoal()
{
  checkNextGoal();
  return nextGoal;
}

//-------------------------------------------------------------------------------------
void IntelligentMovementComponent::checkNextGoal()
{
  if(goals.size() == 0) nextGoal = NULL;

  for(std::vector<Goal*>::iterator iter = goals.begin(); iter != goals.end(); ++iter)
  {
    if(*iter == NULL) continue;
    if(nextGoal)
    {
      if(nextGoal->getPriority() < (*iter)->getPriority()) nextGoal = *iter;//determine next goal by priority
      else
      {
        if(nextGoal->getPosition().squaredDistance(getPosition()) > (*iter)->getPosition().squaredDistance(getPosition())) nextGoal = *iter;//determine next goal by distance to goal
      }
    }
    else
    {
      nextGoal = *iter;
    }
  }
}

//-------------------------------------------------------------------------------------
void IntelligentMovementComponent::setPosition(Vector3 position)
{
  if(!scene || !agent) return;
  removeAgent();
  addAgent();
}

//-------------------------------------------------------------------------------------
Vector3 IntelligentMovementComponent::getPosition()
{
  if(agent) return agent->getPosition();
  else throw NHException("unable to generate an intelligent position because no pathfind agent has been created");
}

//-------------------------------------------------------------------------------------
void IntelligentMovementComponent::hasNodeChange()
{
  if(oldScene && agent) oldScene->getScenePathfindManager()->removeAgent(agent);
  agent = NULL;
  
  if(!scene || !node) return;
  addAgent();
}

//-------------------------------------------------------------------------------------
void IntelligentMovementComponent::update(double elapsedSeconds)
{
  if(!node) return;
  node->_setDerivedPosition(getPosition());
  if(agent && nextGoal) agent->setNewTarget(nextGoal->getPosition());
}

//-------------------------------------------------------------------------------------
void IntelligentMovementComponent::addAgent()
{
  if(!scene || !node) throw NHException("intelligence component must be inside a scene to create the pathfind agent");
  agent = scene->getScenePathfindManager()->createAgent(node->_getDerivedPosition());
  if(!agent) throw NHException("could not create intelligence component pathfind agent");
}

//-------------------------------------------------------------------------------------
void IntelligentMovementComponent::removeAgent()
{
  if(!scene || !agent) throw NHException("intelligence component must be inside a scene to remove the pathfind agent");
  scene->getScenePathfindManager()->removeAgent(agent);
  agent = NULL;
}

