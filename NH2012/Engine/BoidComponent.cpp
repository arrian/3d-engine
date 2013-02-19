#include "BoidComponent.h"

#include "Scene.h"
#include "OgreVector3.h"
#include "OgreQuaternion.h"

#include "SceneGraphicsManager.h"

//-------------------------------------------------------------------------------------
BoidComponent::BoidComponent(Boids::Vector position, Boids::Vector velocity, Boids::Vector dimensions)
  : Boids::Boid(position, velocity, dimensions),
    entity(NULL)
{
}

//-------------------------------------------------------------------------------------
BoidComponent::~BoidComponent(void)
{
}

//-------------------------------------------------------------------------------------
void BoidComponent::update(double elapsedSeconds)
{
  Boids::Boid::update(elapsedSeconds);

  if(node)
  {
    node->setPosition((float) position.x, (float) position.y, (float) position.z);
  }
}

//-------------------------------------------------------------------------------------
void BoidComponent::hasNodeChange()
{
  //Visual
  if(oldScene && entity) oldScene->getSceneGraphicsManager()->destroyEntity(entity);
  entity = NULL;
  //entity = scene->getGraphicsManager()->createEntity("watermelon.mesh");//use temp debug mesh
  //node->attachObject(entity);
  node->setVisible(true);

  //do I have to destroy old billboards?
  billboardSet = scene->getSceneGraphicsManager()->createBillboardSet(1);
  billboardSet->setMaterialName("Examples/Flare");
  billboard = billboardSet->createBillboard(0, 0, 0);
  billboard->setDimensions(2,2);
  node->attachObject(billboardSet);

  //light = scene->getGraphicsManager()->createLight();
  //light->setPosition(Ogre::Vector3::ZERO);
  //light->setAttenuation(5, 1.0f, 0.35f, 0.44f);
  //node->attachObject(light);
}