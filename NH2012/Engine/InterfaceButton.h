#pragma once

#include <OgreVector2.h>
#include "OIS.h"
#include "Gorilla.h"

template<typename CallbackObject>
class InterfaceButton
{
public:
  InterfaceButton(std::string text, Ogre::Vector2 position, Ogre::Vector2 dimensions, CallbackObject* target, void(CallbackObject::*callback)(void))
    : position(position),
      dimensions(dimensions),
      target(target),
      callback(callback),
      isOverButton(false),
      text(text),
      layer(NULL),
      surface(NULL),
      background(Ogre::ColourValue(0.0f, 0.0f, 0.3f, 0.5f)),
      hover(Ogre::ColourValue(0.0f, 0.0f, 0.3f, 1.0f))
  {

  }

  virtual ~InterfaceButton(void) {}

  void setLayer(Gorilla::Layer* layer)
  {
    this->layer = layer;

    surface = layer->createRectangle(position, dimensions);
    surface->background_colour(background);
  }

  void mouseMoved(const OIS::MouseEvent &arg)
  {
    if(arg.state.X.abs < position.x || arg.state.X.abs > position.x + dimensions.x || arg.state.Y.abs < position.y || arg.state.Y.abs > position.y + dimensions.y) 
    {
      if(isOverButton) 
      {
        surface->background_colour(background);
        isOverButton = false;
      }
    }
    else 
    {
      if(!isOverButton) 
      {
        surface->background_colour(hover);
        isOverButton = true;
      }
    }
  }

  void mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
  {

  }

  void mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
  {
    if(isOverButton) (target->callback)();
  }

private:
  std::string text;

  Ogre::Vector2 position;
  Ogre::Vector2 dimensions;

  bool isOverButton;

  CallbackObject* target;
  void(CallbackObject::*callback)(void);//callback function

  Gorilla::Layer* layer;
  Gorilla::Rectangle* surface;

  Ogre::ColourValue background;
  Ogre::ColourValue hover;
};

