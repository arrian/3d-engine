#include "Light.h"

#include "SceneGraphicsManager.h"

#include "NHException.h"

Light::Light(Scene* scene, Vector3 position, bool castShadows, Ogre::Real range, Ogre::ColourValue colour)
  : light(scene->getSceneGraphicsManager()->createLight())
{
  lightNode = scene->getSceneGraphicsManager()->createSceneNode(position);
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
    delete (*iter);
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

//-------------------------------------------------------------------------------------
void Light::hasSceneChange()
{
  //TODO change effects to extending NodeComponent
  throw NHException("light basic component base class not implemented");
}

