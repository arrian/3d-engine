#pragma once

#include <vector>

#include "BasicComponent.h"
#include "NHException.h"

class SceneContainer
{
public:
  SceneContainer()
    : components()
  {
  }

  virtual ~SceneContainer()
  {
    destroyAll();
  }

  void add(BasicComponent* component)
  {
    components.push_back(component);
  }

  void destroyAll()//destroy all objects
  {
    for(std::vector<BasicComponent*>::iterator it = components.begin(); it != components.end(); ++it)
    {
      delete (*it);
    }
    components.clear();
  }

  void destroy(BasicComponent* object)//remove object from set and destroy object
  {
    delete object;
    remove(object);
  }

  void update(double elapsedSeconds)
  {
    for(std::vector<BasicComponent*>::iterator it = components.begin(); it != components.end(); ++it)
    {
      (*it)->update(elapsedSeconds);
    }
  }

  int size()
  {
    return components.size();
  }

private:

  std::vector<BasicComponent*> components;

  void remove(BasicComponent* object)//remove from set without destroying object
  {
    std::vector<BasicComponent*>::iterator iter = std::find(components.begin(), components.end(), object);
    if(iter == components.end()) throw NHException("could not remove the object because it is not in the collection");
    components.erase(iter);
  }
};



