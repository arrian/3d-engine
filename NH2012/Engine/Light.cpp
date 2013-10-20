#include "Light.h"

#include "SceneGraphicsManager.h"

#include "NHException.h"

Light::Light(LightDesc desc)
  : light(NULL),
    desc(desc)
{

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
  boost::shared_ptr<Scene> scene_ptr = getScene(); 
  light = scene_ptr->getSceneGraphicsManager()->createLight();
  lightNode = scene_ptr->getSceneGraphicsManager()->createSceneNode();
  lightNode->attachObject(light);
  light->setAttenuation(desc.range, 0.95f, 0.05f, 0.0f);
  light->setDiffuseColour(Ogre::ColourValue::White);
  light->setSpecularColour(Ogre::ColourValue::White);
  light->setCastShadows(desc.castShadows);
}

