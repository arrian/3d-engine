#pragma once

#include "Vector3.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>


class Scene;

class BasicComponent
{
public:
  BasicComponent(void);
  virtual ~BasicComponent(void);

  virtual void update(double elapsedSeconds) = 0;
    
  void setScene(boost::shared_ptr<Scene> scene);

  boost::shared_ptr<Scene> getScene();
  boost::shared_ptr<Scene> getOldScene();

  bool hasScene();
  bool hasOldScene();

protected:
  virtual void hasSceneChange() = 0;

private:
  boost::weak_ptr<Scene> scene;
  boost::weak_ptr<Scene> oldScene;

  void setOldScene(boost::shared_ptr<Scene> oldScene);
  
};

