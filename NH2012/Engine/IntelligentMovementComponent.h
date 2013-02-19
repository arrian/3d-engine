#pragma once

#include <vector>

#include "Vector3.h"

#include "MovementComponent.h"

#include "ScenePathfindManager.h"
#include "DataManager.h"
#include "Player.h"
#include "Item.h"
#include "Interactive.h"
#include "Goal.h"

class Monster;

/* Artificial intelligence component.*/
class IntelligentMovementComponent : public MovementComponent
{
public:
  IntelligentMovementComponent(float speed, Vector3 gravity);
  ~IntelligentMovementComponent(void);
  
  void setPosition(Vector3 position);
  Vector3 getPosition();

  //Goals - component attempts to satisfy these goals. if they can not be achieved at the current time other goals will be attempted. 
  //This component handles the destruction of all pointers given to it so it is safe, for example, to call setGoal(new Goal()) without deleting Goal.
  void setGoal(Goal* goal);//removes all other goals adding only the given goal
  void addGoal(Goal* goal);//adds this goal to the set of other goals
  void removeGoal(Goal* goal);//removes the specified goal
  Goal* getNextGoal();
  void checkNextGoal();//set the nextGoal value to the highest priority goal

  void update(double elapsedSeconds);

protected:
  float speed;
  PathfindAgent* agent;

  std::vector<Goal*> goals;
  Goal* nextGoal;

  void hasNodeChange();
  void removeAgent();
  void addAgent();
};

