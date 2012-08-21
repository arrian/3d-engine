#pragma once
#include "nodecomponent.h"
class QueryComponent :
  public NodeComponent
{
public:
  QueryComponent(void);
  virtual ~QueryComponent(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  bool rayQuery();
protected:
  void hasNodeChange();
};

