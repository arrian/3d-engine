#pragma once

#include "Vector3.h"
#include "Priority.h"
#include "NHException.h"


/**
 * Defines an artificial intelligence goal
 **/
class Goal
{
public:
  
  Goal(Priority priority)
    : priority(priority)
  {
  }

  virtual Vector3 getPosition() = 0;

  Priority getPriority() 
  {
    return priority;
  }

protected:
  Priority priority;
};


class Patrol : public Goal
{
public:

  Patrol(Vector3 origin, Vector3 target, Priority priority)
    : Goal(priority),
      origin(origin),
      target(target)
  {
  }

  Vector3 origin;
  Vector3 target;

  virtual Vector3 getPosition()
  {
    throw NHException("not implemented");
    return Vector3::ZERO;
  }
};

class Wander : public Goal
{
public:

  Wander(Vector3 origin, float radius, Priority priority)
    : Goal(priority),
      origin(origin),
      radius(radius)
  {
  }

  Vector3 origin;
  float radius;

  virtual Vector3 getPosition()
  {
    throw NHException("not implemented");
    return Vector3::ZERO;
  }
};

class Search : public Goal
{
public:

  Search(Vector3 origin, float radius, Priority priority)
    : Goal(priority),
      origin(origin),
      radius(radius)
  {
  }

  Vector3 origin;
  float radius;

  virtual Vector3 getPosition()
  {
    throw NHException("not implemented");
    return Vector3::ZERO;
  }
};

class Follow : public Goal
{
public:

  Follow(Actor* actor, float distance, Priority priority)
    : Goal(priority),
      actor(actor),
      distance(distance)
  {
  }

  Actor* actor;
  float distance;

  virtual Vector3 getPosition()
  {
    throw NHException("not implemented");
    return Vector3::ZERO;
  }
};

class Go : public Goal
{
public:

  Go(Vector3 target, Priority priority)
    : Goal(priority),
      target(target)
  {
  }

  Vector3 target;

  virtual Vector3 getPosition()
  {
    return target;
  }
};

class Find : public Goal
{
public:

  Find(ArchitectureDesc architecture, Priority priority)
    : Goal(priority),
      architecture(architecture)
  {
  }
  
  ArchitectureDesc architecture;

  virtual Vector3 getPosition()
  {
    throw NHException("not implemented");
    return Vector3::ZERO;
  }
};

class Collect : public Goal
{
public:

  Collect(ItemDesc item, Priority priority)
    : Goal(priority),
      item(item)
  {
  }

  ItemDesc item;

  virtual Vector3 getPosition()
  {
    throw NHException("not implemented");
    return Vector3::ZERO;
  }
};

class Attack : public Goal
{
public:

  Attack(Actor* actor, Priority priority)
    : Goal(priority),
      actor(actor)
  {
  }

  Actor* actor;

  virtual Vector3 getPosition()
  {
    return actor->getPosition();
  }
};

class Pickup : public Goal
{
public:

  Pickup(Item* item, Priority priority)
    : Goal(priority),
      item(item)
  {
  }

  Item* item;

  virtual Vector3 getPosition()
  {
    return item->getPosition();
  }
};

class Discard : public Goal
{
public:

  Discard(Item* item, Priority priority)
    : Goal(priority),
      item(item)
  {
  }

  Item* item;

  virtual Vector3 getPosition()
  {
    return item->getPosition();
  }
};

class Interact : public Goal
{
public:

  Interact(Interactive* interactive, Priority priority)
    : Goal(priority),
      interactive(interactive)
  {
  }

  Interactive* interactive;

  virtual Vector3 getPosition()
  {
    return interactive->getPosition();
  }
};

class Flee : public Goal
{
public:

  Flee(Vector3 from, Priority priority)
    : Goal(priority),
      from(from)
  {
  }

  Vector3 from;

  virtual Vector3 getPosition()
  {
    throw NHException("not implemented");
  }
};

