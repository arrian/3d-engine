#pragma once

#include "Gorilla.h"
#include "OIS.h"

class World;

class Screen
{
public:
  Screen(World* world);
  ~Screen(void);

  //Override
  virtual void update(double elapsedSeconds) {}
  virtual void keyPressed(const OIS::KeyEvent &arg) {}
  virtual void keyReleased(const OIS::KeyEvent &arg) {}
  virtual void mouseMoved(const OIS::MouseEvent &arg) {}
  virtual void mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {}
  virtual void mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {}


  void setWorld(World* world);
  void setLayer(Gorilla::Layer* layer);
  void setSize(Ogre::Vector2 dimensions);
  void setVisible(bool visible);

  bool isVisible();

  Ogre::Vector2 getSize();

protected:
  Gorilla::Layer* layer;
  World* world;

  //Override
  virtual void hasLayerChange() {}
  virtual void hasSizeChange() {}

private:
  bool visible;
  Ogre::Vector2 dimensions;

};

