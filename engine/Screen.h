#pragma once

#include "Gorilla.h"
#include "OIS.h"

class World;

class Screen
{
public:
  Screen();
  ~Screen(void);

  //Override
  virtual void update(double elapsedSeconds) {}
  virtual void keyPressed(const OIS::KeyEvent &arg) {}
  virtual void keyReleased(const OIS::KeyEvent &arg) {}
  virtual void mouseMoved(const OIS::MouseEvent &arg) {}
  virtual void mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {}
  virtual void mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {}

  void setLayer(Gorilla::Layer* layer);
  void setSize(Ogre::Vector2 dimensions);
  void setVisible(bool visible);

  bool isVisible();

  Ogre::Vector2 getSize();

protected:
  Gorilla::Layer* layer;

  //Override
  virtual void hasLayerChange() {}
  virtual void hasSizeChange() {}

private:
  bool visible;
  Ogre::Vector2 dimensions;

};

