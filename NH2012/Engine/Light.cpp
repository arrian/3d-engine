#include "Light.h"


Light::Light(Scene* scene, Ogre::Vector3 position, bool castShadows, Ogre::Real range, Ogre::ColourValue colour)
  : light(scene->getSceneManager()->createLight()),
    hasIntensityVariation(false),
    hasColourVariation(false),
    up(false)//temp colour change
{
  lightNode = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode(position);
  lightNode->attachObject(light);
  light->setAttenuation(range, 0.95f, 0.05f, 0);
  light->setDiffuseColour(colour);
  light->setSpecularColour(Ogre::ColourValue::White);
  light->setCastShadows(castShadows);

  
  //temp intensity variation
  addIntensityVariation(IntensityVariationDesc());
}


Light::~Light(void)
{
}

void Light::addColourVariation(ColourVariationDesc colourVariation)
{
  hasColourVariation = true;
  this->colourVariation = colourVariation;
}

void Light::addIntensityVariation(IntensityVariationDesc intensityVariation)
{
  hasIntensityVariation = true;
  this->intensityVariation = intensityVariation;
}

void Light::on(bool flicker, float flickerTime, IntensityVariationDesc flickerParams)
{
  light->setVisible(true);
}

void Light::off()
{
  light->setVisible(false);
}

void Light::update(double elapsedSeconds)
{
  //if(up) light->setDiffuseColour(light->getDiffuseColour() + Ogre::ColourValue(0,0,0.01));
  //else light->setDiffuseColour(light->getDiffuseColour() - Ogre::ColourValue(0,0,0.01));

  //if(light->getDiffuseColour().b >= 1.0f) up = false;
  //else if(light->getDiffuseColour().b <= 0.0f) up = true;
}



