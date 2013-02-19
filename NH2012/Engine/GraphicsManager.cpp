#include "GraphicsManager.h"


GraphicsManager::GraphicsManager(void)
  : root(NULL),
    camera(NULL)
{
}

//-------------------------------------------------------------------------------------
GraphicsManager::~GraphicsManager(void)
{
}

//-------------------------------------------------------------------------------------
void GraphicsManager::setRoot(Ogre::Root* root)
{
  this->root = root;
}

//-------------------------------------------------------------------------------------
void GraphicsManager::setCamera(CameraComponent* camera)
{
  this->camera = camera;
}

//-------------------------------------------------------------------------------------
void GraphicsManager::hookWindow(Ogre::RenderWindow* window)
{
  assert(camera);
  camera->hookWindow(window);
}



