#pragma once
class SceneChangeListener
{
public:
  SceneChangeListener(void);
  virtual ~SceneChangeListener(void);

  virtual void sceneChanged() = 0;
};

