#pragma once

#include <vector>

#include <OgreColourValue.h>
#include <OgreLight.h>
#include <OgreSceneNode.h>

#include "DataManager.h"
#include "Scene.h"
#include "Effect.h"

#include "Vector3.h"
#include "Quaternion.h"

class LightPlugin
{
public:
  LightPlugin(Light* light)
    : light(light)
  {
  }

  virtual ~LightPlugin() {}

  virtual void update(double elapsedSeconds) = 0;

protected:
  Light* light;
};

class IntensityVariationDesc : public LightPlugin
{
public:
  IntensityVariationDesc(Light* light)
    : LightPlugin(light),
      randomness(0.5f),
      interval(2.0f),
      minIntensity(0.5f),
      maxIntensity(1.0f)
  {
  }

  void update(double elapsedSeconds)
  {
    //implement here
  }

protected:
  float randomness;
  float interval;
  float minIntensity;
  float maxIntensity;
};

class ColourVariationDesc : public LightPlugin
{
  ColourVariationDesc(Light* light)
    : LightPlugin(light),
      colourSet(),
      randomiseColourSet(false),
      randomness(0.5f),
      interval(2.0f)
  {
    //temp variate on primaries
    colourSet.push_back(Ogre::ColourValue::Red);
    colourSet.push_back(Ogre::ColourValue::Green);
    colourSet.push_back(Ogre::ColourValue::Blue);
  }

  void update(double elapsedSeconds)
  {
    //implement here
  }

protected:
  std::vector<Ogre::ColourValue> colourSet;
  bool randomiseColourSet;
  float randomness;
  float interval;
};

class Light : public Effect
{
public:
  Light(LightDesc desc);
  virtual ~Light(void);

  void setPosition(Vector3 position) {lightNode->setPosition(position);}
  Vector3 getPosition() {return lightNode->getPosition();}

  void setRotation(Quaternion rotation) {lightNode->_setDerivedOrientation(rotation);}
  Quaternion getRotation() {return lightNode->_getDerivedOrientation();}

  void addPlugin(LightPlugin* plugin);

  void setOn(bool on);

  virtual void update(double elapsedSeconds);

protected:
  virtual void hasSceneChange();

private:
  LightDesc desc;
  Ogre::Light* light;
  std::vector<LightPlugin*> plugins;

  Ogre::SceneNode* lightNode;
};

