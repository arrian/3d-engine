#include "BoidExtension.h"

#include "Scene.h"
#include "OgreVector3.h"
#include "OgreQuaternion.h"

//-------------------------------------------------------------------------------------
BoidExtension::BoidExtension(Boids::Vector position, Boids::Vector velocity, Boids::Vector dimensions)
  : Boids::Boid(position, velocity, dimensions),
    entity(NULL)
{
}

//-------------------------------------------------------------------------------------
BoidExtension::~BoidExtension(void)
{
}

//-------------------------------------------------------------------------------------
void BoidExtension::update(double elapsedSeconds)
{
  Boids::Boid::update(elapsedSeconds);

  if(node)
  {
    node->setPosition(position.x, position.y, position.z);
  }
}

//-------------------------------------------------------------------------------------
void BoidExtension::hasNodeChange()
{
  //Visual
  if(oldScene && entity) oldScene->getSceneManager()->destroyEntity(entity);
  entity = NULL;
  //entity = scene->getSceneManager()->createEntity("watermelon.mesh");//use temp debug mesh
  //node->attachObject(entity);
  node->setVisible(true);

  //do I have to destroy old billboards?
  billboardSet = scene->getSceneManager()->createBillboardSet(1);
  billboardSet->setMaterialName("Examples/Flare");
  billboard = billboardSet->createBillboard(0, 0, 0);
  billboard->setDimensions(2,2);
  node->attachObject(billboardSet);

  //light = scene->getSceneManager()->createLight();
  //light->setPosition(Ogre::Vector3::ZERO);
  //light->setAttenuation(5, 1.0f, 0.35f, 0.44f);
  //node->attachObject(light);
}