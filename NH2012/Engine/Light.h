#pragma once

#include <vector>

#include <OgreColourValue.h>
#include <OgreLight.h>

#include "Scene.h"

struct IntensityVariationDesc
{
  IntensityVariationDesc()
    : randomness(0.5f),
      interval(2.0f),
      minIntensity(0.5f),
      maxIntensity(1.0f)
  {
  }

  float randomness;
  float interval;
  float minIntensity;
  float maxIntensity;
};

struct ColourVariationDesc
{
  ColourVariationDesc()
    : colourSet(),
      randomiseColourSet(false),
      randomness(0.5f),
      interval(2.0f)
  {
    //temp variate on primaries
    colourSet.push_back(Ogre::ColourValue::Red);
    colourSet.push_back(Ogre::ColourValue::Green);
    colourSet.push_back(Ogre::ColourValue::Blue);
  }

  std::vector<Ogre::ColourValue> colourSet;
  bool randomiseColourSet;
  float randomness;
  float interval;
};

class Light
{
public:
  Light(Scene* scene, Ogre::Vector3 position, bool castShadows, Ogre::Real range, Ogre::ColourValue colour);
  ~Light(void);

  void addColourVariation(ColourVariationDesc colourVariation);
  void addIntensityVariation(IntensityVariationDesc intensityVariation);

  void on(bool flicker = false, float flickerTime = 0.0f, IntensityVariationDesc flickerParams = IntensityVariationDesc());//turn the light on with optional flickering
  void off();

  void update(double elapsedSeconds);

private:
  Ogre::Light* light;

  bool hasIntensityVariation;//should this light's intensity flicker?
  IntensityVariationDesc intensityVariation;

  bool hasColourVariation;//should this light's colour variate
  ColourVariationDesc colourVariation;

  Ogre::SceneNode* lightNode;
};

