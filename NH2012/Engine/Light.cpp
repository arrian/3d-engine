#include "Light.h"


Light::Light(Scene* scene, Ogre::Vector3 position, bool castShadows, Ogre::Real range, Ogre::ColourValue colour)
  : light(scene->getSceneManager()->createLight())
{
  lightNode = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode(position);
  lightNode->attachObject(light);
  light->setAttenuation(range, 0.95f, 0.05f, 0.0f);
  light->setDiffuseColour(colour);
  light->setSpecularColour(Ogre::ColourValue::White);
  light->setCastShadows(castShadows);
}

//-------------------------------------------------------------------------------------
Light::~Light(void)
{
  for(std::vector<LightPlugin*>::iterator iter = plugins.begin(); iter != plugins.end(); ++iter)
  {
    if(*iter) delete (*iter);
    (*iter) = NULL;
  }
}

//-------------------------------------------------------------------------------------
void Light::addPlugin(LightPlugin* plugin)
{
  plugins.push_back(plugin);
}

//-------------------------------------------------------------------------------------
void Light::setOn(bool on)
{
  light->setVisible(on);
}

//-------------------------------------------------------------------------------------
void Light::update(double elapsedSeconds)
{
  if(!light->isVisible()) return;
  for(std::vector<LightPlugin*>::iterator iter = plugins.begin(); iter != plugins.end(); ++iter)
  {
    (*iter)->update(elapsedSeconds);
  }
}



