#include "GraphicsManager.h"


GraphicsManager::GraphicsManager(void)
  : root(NULL),
    camera(NULL),
    ssao(NULL),
    enableBloom(false),
    enableSSAO(false),
    enableShadows(false),
    window(NULL)
{
}

//-------------------------------------------------------------------------------------
GraphicsManager::~GraphicsManager(void)
{
  delete ssao;
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

  if(window && camera) camera->hookWindow(window);

  refresh();
}

//-------------------------------------------------------------------------------------
void GraphicsManager::hookWindow(Ogre::RenderWindow* window)
{
  this->window = window;

  if(window && camera) camera->hookWindow(window);
}

//-------------------------------------------------------------------------------------
void GraphicsManager::setBloomEnabled(bool enabled)
{
  assert(camera);

  if(enabled)
  {
    Ogre::CompositorManager::getSingleton().addCompositor(camera->getViewport(), "Bloom");
    Ogre::CompositorManager::getSingleton().setCompositorEnabled(camera->getViewport(), "Bloom", true);
  }
  else
  {
    if(enableBloom) Ogre::CompositorManager::getSingleton().removeCompositor(camera->getViewport(), "Bloom");//remove compositor if it was previously created
  }

  enableBloom = enabled;
}

//-------------------------------------------------------------------------------------
void GraphicsManager::setSSAOEnabled(bool enabled)
{
  assert(camera);

  enableSSAO = enabled;

  delete ssao;
  ssao = NULL;

  if(enableSSAO) ssao = new PFXSSAO(camera->getWindow(), camera->getCamera());//enables screen space ambient occlusion
}

//-------------------------------------------------------------------------------------
void GraphicsManager::setShadowsEnabled(bool enabled)
{
  enableShadows = enabled;//shadows are per scene
}

//-------------------------------------------------------------------------------------
void GraphicsManager::refresh()
{
  setBloomEnabled(enableBloom);
  setSSAOEnabled(enableSSAO);
  setShadowsEnabled(enableShadows);
}
