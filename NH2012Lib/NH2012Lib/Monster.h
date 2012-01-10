#pragma once

#include "Vector3.h"

class Monster
{
public:
  Monster(void);
  ~Monster(void);

  void step(double frameTime);

  Vector3 getPosition();
  Vector3 getWaypoints();

  void notify();//notify of event
private:
  Vector3 position;
  Vector3 target;//temp until waypoints implemented

  void generateWaypoints(Vector3 target);
};

