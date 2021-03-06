#include "CameraComponent.h"

//Ogre
#include "OgreCompositorManager.h"

//Local
#include "Scene.h"
#include "NHException.h"
#include "Identifiable.h"

#include "SceneGraphicsManager.h"

//-------------------------------------------------------------------------------------
CameraComponent::CameraComponent(bool enableSSAO, bool enableBloom)
  : NodeComponent(),
    camera(NULL),
    window(NULL),
    viewport(NULL),
    oldCameraWidth(0),
    oldCameraHeight(0),
    nearClipDefault(0.4f),
    farClipDefault(400.0f),//recommended to keep ratio of far to near at or below 1000:1
    rayCastDistance(10.0f)//,
    //ssao(NULL),
    //enableSSAO(enableSSAO),
    //enableBloom(enableBloom)
{

}

//-------------------------------------------------------------------------------------
CameraComponent::~CameraComponent(void)
{
  //delete ssao;
  boost::shared_ptr<Scene> scene = getScene();
  if(scene) scene->getSceneGraphicsManager()->destroyCamera(camera);
}

//-------------------------------------------------------------------------------------
void CameraComponent::hookWindow(Ogre::RenderWindow* window)
{
  if(!window || !camera) return;
  
  unhookWindow();
  this->window = window;//not sure about order here
  viewport = window->addViewport(camera);
  viewport->setBackgroundColour(Ogre::ColourValue(0,0,0));

  /*
  if(ssao)//remove previously created ssao
  {
    delete ssao;
    ssao = NULL;
  }
  if(enableSSAO) ssao = new PFXSSAO(window, camera);//enables screen space ambient occlusion
  */
  /*
  if(enableMotionBlur)//not working??
  {
    Ogre::CompositorManager::getSingleton().addCompositor(viewport, "Motion Blur");
    Ogre::CompositorManager::getSingleton().setCompositorEnabled(viewport, "Motion Blur", true);
  }
  */
  /*
  if(enableBloom)
  {
    Ogre::CompositorManager::getSingleton().addCompositor(viewport, "Bloom");
    Ogre::CompositorManager::getSingleton().setCompositorEnabled(viewport, "Bloom", true);
  }
  */
  //Ogre::CompositorManager::getSingleton().addCompositor(viewport, "B&W");
  //Ogre::CompositorManager::getSingleton().setCompositorEnabled(viewport, "B&W", true);

  //Console::getInstance().hookWindow(window);
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
  boost::shared_ptr<Scene> scene = getScene();
  boost::shared_ptr<Scene> oldScene = getOldScene();

  if(oldScene.get() == scene.get())
  {
    if(oldNode) oldNode->detachObject(camera);
  }
  else
  {
    if(oldScene && camera) oldScene->getSceneGraphicsManager()->destroyCamera(camera);//cleaning up previous scene
    if(!scene) return;
    camera = scene->getSceneGraphicsManager()->createCamera("CameraComponent");
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
Vector3 CameraComponent::getDirection()
{
  return camera->getRealDirection();//am i using the correct direction function?
}

