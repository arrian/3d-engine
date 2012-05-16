#include "CameraComponent.h"

#include "Scene.h"

CameraComponent::CameraComponent()
  : NodeComponent(),
    camera(0),
    window(0),
    viewport(0),
    oldCameraWidth(0),
    oldCameraHeight(0)
{

}


CameraComponent::~CameraComponent(void)
{
  this->scene->getSceneManager()->destroyCamera(camera);
}

void CameraComponent::hookWindow(Ogre::RenderWindow* window)
{
  assert(window && camera);
  unhookWindow();
  this->window = window;//not sure about order here
  viewport = window->addViewport(camera);
  viewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
  assert(viewport);
}

void CameraComponent::unhookWindow()
{
  viewport = 0;
  if(window) window->removeAllViewports();
}

void CameraComponent::hasNodeChange()
{
  if(oldScene == scene) oldNode->detachObject(camera);
  else 
  {
    if(oldScene && camera) oldScene->getSceneManager()->destroyCamera(camera);//cleaning up previous scene
    camera = scene->getSceneManager()->createCamera("CameraComponent");
  }

  camera->setNearClipDistance(1.0f);
  //if(world->wireframeDebug) camera->setPolygonMode(Ogre::PM_WIREFRAME);
  
  node->attachObject(camera);
}

void CameraComponent::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  //Checking aspect ratio for resolution changes... find better way of doing this
  if(viewport->getActualWidth() != oldCameraWidth || viewport->getActualHeight() != oldCameraHeight)
  {
    camera->setAspectRatio(Ogre::Real(viewport->getActualWidth()) / Ogre::Real(viewport->getActualHeight()));
    oldCameraWidth = viewport->getActualWidth();
    oldCameraHeight = viewport->getActualHeight();
  }
}