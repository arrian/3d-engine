#pragma once

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "Vector3.h"
#include "GraphicsManager.h"

class Scene;

/*Interface between ogre scene manager and the application.*/
class SceneGraphicsManager
{
public:
  SceneGraphicsManager(boost::shared_ptr<Scene> scene, Ogre::ColourValue ambientLight, Ogre::ColourValue shadowColour);
  virtual ~SceneGraphicsManager(void);

  void setAmbientLight(Ogre::ColourValue colour) {sceneManager->setAmbientLight(colour);}
  void setShadowsEnabled(bool enabled);
  void setDebugDrawShadows(bool enabled);
  void setDebugDrawBoundingBoxes(bool enabled);

  Ogre::SceneManager* getSceneManager() {return sceneManager;}

  Ogre::SceneNode* createSceneNode(Vector3 position = Vector3::ZERO) {return sceneManager->getRootSceneNode()->createChildSceneNode();}
  Ogre::StaticGeometry* createStaticGeometry(std::string name) {return sceneManager->createStaticGeometry(name);}
  Ogre::Entity* createEntity(std::string name) {return sceneManager->createEntity(name);}
  Ogre::ParticleSystem* createParticleSystem(std::string name, std::string templateName) {return sceneManager->createParticleSystem(name, templateName);}
  Ogre::Light* createLight() {return sceneManager->createLight();}
  Ogre::ManualObject* createManualObject() {return sceneManager->createManualObject();}
  Ogre::Camera* createCamera(std::string name) {return sceneManager->createCamera(name);}
  Ogre::BillboardSet* createBillboardSet(int poolSize) {return sceneManager->createBillboardSet(poolSize);}

  void destroyAllAttachedMoveables(Ogre::SceneNode* node);
  void destroySceneNode(Ogre::SceneNode* node);
  void destroyEntity(Ogre::Entity* entity) {sceneManager->destroyEntity(entity);}
  void destroyManualObject(Ogre::ManualObject* manualObject) {sceneManager->destroyManualObject(manualObject);}
  void destroyCamera(Ogre::Camera* camera) {sceneManager->destroyCamera(camera);}

  boost::shared_ptr<Scene> getScene() 
  {
    try
    {
      return scene.lock();
    }
    catch(boost::bad_weak_ptr b)
    {
#ifdef _DEBUG
      std::cout << "Could not get scene from scene graphics manager. Scene has expired." << std::endl;
#endif
    }
    return boost::shared_ptr<Scene>();
  }

private:
  boost::weak_ptr<Scene> scene;
  //GraphicsManager* graphicsManager;

  Ogre::SceneManager* sceneManager;

  Ogre::ColourValue shadowColour;
};

