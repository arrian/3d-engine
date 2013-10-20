#include "SceneGraphicsManager.h"

#include "OgreSceneManager.h"
#include "Scene.h"
#include "World.h"


SceneGraphicsManager::SceneGraphicsManager(boost::shared_ptr<Scene> scene, Ogre::ColourValue ambientLight, Ogre::ColourValue shadowColour)
  : scene(boost::weak_ptr<Scene>(scene)),
    shadowColour(shadowColour),
    sceneManager(NULL)
{
  if(!scene) throw NHException("Given scene null in scene graphics manager constructor.");

  boost::shared_ptr<World> world_ptr = scene->getWorld();
  if(!world_ptr) throw NHException("World has expired in scene graphics manager contructor.");

  sceneManager = world_ptr->getGraphicsManager()->createSceneGraphicsManager();
  if(!sceneManager) throw NHException("failed to create SceneGraphicsManager");

  setShadowsEnabled(scene->getWorld()->getGraphicsManager()->isShadowsEnabled());
  setAmbientLight(ambientLight);
}

//-------------------------------------------------------------------------------------
SceneGraphicsManager::~SceneGraphicsManager(void)
{
  boost::shared_ptr<Scene> scene_ptr = getScene();
  if(scene_ptr) 
  {
    boost::shared_ptr<World> world_ptr = scene_ptr->getWorld();
    if(world_ptr && world_ptr->getGraphicsManager() && sceneManager) world_ptr->getGraphicsManager()->destroySceneGraphicsManager(sceneManager);
  }
  sceneManager = NULL;
}

//-------------------------------------------------------------------------------------
void SceneGraphicsManager::destroyAllAttachedMoveables(Ogre::SceneNode* node)
{
  if(!node) return;

  // Destroy all the attached objects
  Ogre::SceneNode::ObjectIterator itObject = node->getAttachedObjectIterator();

  while (itObject.hasMoreElements()) node->getCreator()->destroyMovableObject(itObject.getNext());

  // Recurse to child SceneNodes
  Ogre::SceneNode::ChildNodeIterator itChild = node->getChildIterator();

  while (itChild.hasMoreElements())
  {
    Ogre::SceneNode* pChildNode = static_cast<Ogre::SceneNode*>(itChild.getNext());
    destroyAllAttachedMoveables(pChildNode);
  }
}

//-------------------------------------------------------------------------------------
void SceneGraphicsManager::destroySceneNode(Ogre::SceneNode* node)
{
  boost::shared_ptr<Scene> scene_ptr = getScene();
  if(scene_ptr && node) 
  {
    boost::shared_ptr<World> world_ptr = scene_ptr->getWorld();
    if(world_ptr && world_ptr->getGraphicsManager() && world_ptr->getGraphicsManager()->getRoot())//TODO: fix checks here
    {
      destroyAllAttachedMoveables(node);
      node->removeAndDestroyAllChildren();
      node->getCreator()->destroySceneNode(node);
    }
  }
  
}

//-------------------------------------------------------------------------------------
void SceneGraphicsManager::setShadowsEnabled(bool enabled)
{
  if(enabled)
  {
    sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
    sceneManager->setShadowColour(shadowColour);
    sceneManager->setShadowTextureSize(4096);//1024 seems very pixelated
    sceneManager->setShadowTextureCount(5);
  }
  else
  {
    sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
  }
}

//-------------------------------------------------------------------------------------
void SceneGraphicsManager::setDebugDrawShadows(bool enabled)
{
  sceneManager->setShowDebugShadows(enabled);
}

//-------------------------------------------------------------------------------------
void SceneGraphicsManager::setDebugDrawBoundingBoxes(bool enabled)
{
  sceneManager->showBoundingBoxes(enabled);
}




