#include "CameraComponent.h"

#include "Scene.h"
#include "NHException.h"
#include "PhysicalInterface.h"

//-------------------------------------------------------------------------------------
CameraComponent::CameraComponent()
  : NodeComponent(),
    camera(NULL),
    window(NULL),
    viewport(NULL),
    oldCameraWidth(0),
    oldCameraHeight(0),
    nearClipDefault(0.4f),
    farClipDefault(1000.0f),
    rayCastDistance(10.0f)
{

}

//-------------------------------------------------------------------------------------
CameraComponent::~CameraComponent(void)
{
  this->scene->getSceneManager()->destroyCamera(camera);
}

//-------------------------------------------------------------------------------------
void CameraComponent::hookWindow(Ogre::RenderWindow* window)
{
  assert(window && camera);
  unhookWindow();
  this->window = window;//not sure about order here
  viewport = window->addViewport(camera);
  viewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
  assert(viewport);
}

//-------------------------------------------------------------------------------------
void CameraComponent::unhookWindow()
{
  viewport = NULL;
  if(window) window->removeAllViewports();
}

//-------------------------------------------------------------------------------------
void CameraComponent::hasNodeChange()
{
  if(oldScene == scene) oldNode->detachObject(camera);
  else 
  {
    if(oldScene && camera) oldScene->getSceneManager()->destroyCamera(camera);//cleaning up previous scene
    camera = scene->getSceneManager()->createCamera("CameraComponent");
  }

  camera->setNearClipDistance(nearClipDefault);//can see some triangle clipping but no z-fighting
  camera->setFarClipDistance(farClipDefault);//might be able to make this larger if necessary
  //if(world->wireframeDebug) camera->setPolygonMode(Ogre::PM_WIREFRAME);
  
  node->attachObject(camera);
}

//-------------------------------------------------------------------------------------
void CameraComponent::update(double elapsedSeconds)
{
  //Checking aspect ratio for resolution changes... find better way of doing this
  if(viewport->getActualWidth() != oldCameraWidth || viewport->getActualHeight() != oldCameraHeight)
  {
    camera->setAspectRatio(Ogre::Real(viewport->getActualWidth()) / Ogre::Real(viewport->getActualHeight()));
    oldCameraWidth = viewport->getActualWidth();
    oldCameraHeight = viewport->getActualHeight();
  }
}

//-------------------------------------------------------------------------------------
void CameraComponent::rayQuery()
{
  throw NHException("the physx character controller is doing something funky with the userData field so I'll have to think of a different way to map the physical shapes to the associated objects");

  Ogre::Vector3 oOrigin = camera->getDerivedPosition();
  Ogre::Vector3 oUnitDir = camera->getDerivedDirection();
  physx::PxVec3 origin = physx::PxVec3(oOrigin.x, oOrigin.y, oOrigin.z);
  physx::PxVec3 unitDir = physx::PxVec3(oUnitDir.x, oUnitDir.y, oUnitDir.z);
  physx::PxSceneQueryFlags outputFlags = physx::PxSceneQueryFlags();
  physx::PxRaycastHit hit;
  if(scene->getPhysicsManager()->raycastSingle(origin, unitDir, rayCastDistance, outputFlags, hit))
  {
    std::cout << "ray hit at a distance of " << hit.distance << " with user data " << hit.shape->userData << std::endl;
    if(hit.shape->userData)
    {
      PhysicalInterface* target = static_cast<PhysicalInterface*>(hit.shape->userData);//Unsafe operation. Have to make certain that the void pointer has been cast to PhysicalInterface initially
      std::cout << target->getType() << ":" << target->getName() << ":" << target->getID() << std::endl;
      
    }
  }
}

