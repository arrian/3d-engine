#pragma once

#include <vector>

#include <OgreColourValue.h>
#include <OgreLight.h>

#include "Scene.h"

class LightPlugin
{
public:
  LightPlugin(Light* light)
    : light(light)
  {
  }

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

  }

protected:
  std::vector<Ogre::ColourValue> colourSet;
  bool randomiseColourSet;
  float randomness;
  float interval;
};

class Light
{
public:
  Light(Scene* scene, Vector3 position, bool castShadows, Ogre::Real range, Ogre::ColourValue colour);
  ~Light(void);

  void addPlugin(LightPlugin* plugin);

  void setOn(bool on);

  void update(double elapsedSeconds);

private:
  Ogre::Light* light;
  std::vector<LightPlugin*> plugins;

  Ogre::SceneNode* lightNode;
};

