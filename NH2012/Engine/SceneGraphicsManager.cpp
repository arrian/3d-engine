#include "SceneGraphicsManager.h"

#include "OgreSceneManager.h"


SceneGraphicsManager::SceneGraphicsManager(GraphicsManager* graphicsManager, Ogre::ColourValue ambientLight, Ogre::ColourValue shadowColour)
  : graphicsManager(graphicsManager),
    shadowColour(shadowColour)
{
  sceneManager = graphicsManager->getRoot()->createSceneManager(Ogre::ST_GENERIC);

  setShadowsEnabled(graphicsManager->isShadowsEnabled());
  setAmbientLight(ambientLight);
}

//-------------------------------------------------------------------------------------
SceneGraphicsManager::~SceneGraphicsManager(void)
{
  graphicsManager->getRoot()->destroySceneManager(sceneManager);
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
  if(!node) return;
  destroyAllAttachedMoveables(node);
  node->removeAndDestroyAllChildren();
  node->getCreator()->destroySceneNode(node);
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




