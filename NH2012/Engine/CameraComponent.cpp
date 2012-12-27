#include "CameraComponent.h"

#include "Scene.h"
#include "NHException.h"
#include "IdentificationInterface.h"

#include "Console.h"

#include "OgreCompositorManager.h"

//-------------------------------------------------------------------------------------
CameraComponent::CameraComponent(bool enableSSAO, bool enableBloom, bool enableMotionBlur)
  : NodeComponent(),
    camera(NULL),
    window(NULL),
    viewport(NULL),
    oldCameraWidth(0),
    oldCameraHeight(0),
    nearClipDefault(0.4f),
    farClipDefault(400.0f),//recommended to keep ratio of far to near at or below 1000:1
    rayCastDistance(10.0f),
    ssao(NULL),
    enableSSAO(enableSSAO),
    enableBloom(enableBloom),
    enableMotionBlur(enableMotionBlur)
{

}

//-------------------------------------------------------------------------------------
CameraComponent::~CameraComponent(void)
{
  if(ssao) delete ssao;
  if(scene) scene->getSceneManager()->destroyCamera(camera);
}

//-------------------------------------------------------------------------------------
void CameraComponent::hookWindow(Ogre::RenderWindow* window)
{
  if(!window || !camera) return;
  
  unhookWindow();
  this->window = window;//not sure about order here
  viewport = window->addViewport(camera);
  viewport->setBackgroundColour(Ogre::ColourValue(0,0,0));

  if(ssao)//remove previously created ssao
  {
    delete ssao;
    ssao = NULL;
  }
  if(enableSSAO) ssao = new PFXSSAO(window, camera);//enables screen space ambient occlusion

  if(enableMotionBlur)//not working??
  {
    Ogre::CompositorManager::getSingleton().addCompositor(viewport, "Motion Blur");
    Ogre::CompositorManager::getSingleton().setCompositorEnabled(viewport, "Motion Blur", true);
  }

  if(enableBloom)
  {
    Ogre::CompositorManager::getSingleton().addCompositor(viewport, "Bloom");
    Ogre::CompositorManager::getSingleton().setCompositorEnabled(viewport, "Bloom", true);
  }

  //Ogre::CompositorManager::getSingleton().addCompositor(viewport, "B&W");
  //Ogre::CompositorManager::getSingleton().setCompositorEnabled(viewport, "B&W", true);

  Console::getInstance().hookWindow(window);
}

//-------------------------------------------------------------------------------------
void CameraComponent::unhookWindow()
{
  viewport = NULL;
  if(window) window->removeAllViewports();
}

//-------------------------------------------------------------------------------------
void CameraComponent::rehookWindow()
{
  hookWindow(window);
}

//-------------------------------------------------------------------------------------
Ogre::RenderWindow* CameraComponent::getWindow()
{
  return window;
}

//-------------------------------------------------------------------------------------
void CameraComponent::hasNodeChange()
{
  if(oldScene == scene)
  {
    if(oldNode) oldNode->detachObject(camera);
  }
  else
  {
    if(oldScene && camera) oldScene->getSceneManager()->destroyCamera(camera);//cleaning up previous scene
    if(!scene) return;
    camera = scene->getSceneManager()->createCamera("CameraComponent");
  }

  camera->setNearClipDistance(nearClipDefault);//can see some triangle clipping but no z-fighting
  camera->setFarClipDistance(farClipDefault);//might be able to make this larger if necessary
  //if(world->wireframeDebug) camera->setPolygonMode(Ogre::PM_WIREFRAME);
  
  if(node) node->attachObject(camera);
}

//-------------------------------------------------------------------------------------
void CameraComponent::update(double elapsedSeconds)
{
  if(!window || !viewport) return;//nothing to do
  
  //Checking aspect ratio for resolution changes... find better way of doing this
  if(viewport->getActualWidth() != oldCameraWidth || viewport->getActualHeight() != oldCameraHeight)
  {
    camera->setAspectRatio(Ogre::Real(viewport->getActualWidth()) / Ogre::Real(viewport->getActualHeight()));
    oldCameraWidth = viewport->getActualWidth();
    oldCameraHeight = viewport->getActualHeight();
  }
}

//-------------------------------------------------------------------------------------
Ogre::Camera* CameraComponent::getCamera()
{
  return camera;
}

//-------------------------------------------------------------------------------------
Ogre::Viewport* CameraComponent::getViewport()
{
  return viewport;
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 CameraComponent::getDirection()
{
  return camera->getRealDirection();//am i using the correct direction function?
}

