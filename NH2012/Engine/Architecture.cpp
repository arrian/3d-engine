#include "Architecture.h"

#include "Scene.h"

//-------------------------------------------------------------------------------------
Architecture::Architecture(Scene* scene)
  : scene(scene),
    instanceNumber(0),
    geometry(scene->getSceneManager()->createStaticGeometry("architecture")),//potentially unsafe operation... ensure all objects used by getscenemanager have been constructed
    nodes(),
    actors(),
    statics(),
    defaultFriction(0.2f),
    defaultRestitution(0.7f)
{
}

//-------------------------------------------------------------------------------------
Architecture::~Architecture(void)
{
  for(std::vector<physx::PxRigidStatic*>::iterator it = actors.begin(); it != actors.end(); ++it) 
  {
    if(*it) (*it)->release();
    (*it) = 0;
  }
  
  for(std::vector<Ogre::SceneNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) 
  {
    if(*it) scene->getSceneManager()->destroySceneNode(*it);
    (*it) = 0;
  }

  for(std::map<Ogre::String, StaticEntity>::iterator it = statics.begin(); it != statics.end(); ++it) 
  {
    if(it->second.entity) scene->getSceneManager()->destroyEntity(it->second.entity);
    if(it->second.mesh) it->second.mesh->release();
    
    it->second.entity = 0;
    it->second.mesh = 0;
  }
}

//-------------------------------------------------------------------------------------
void Architecture::add(Ogre::String meshName, Ogre::Vector3 position, Ogre::Quaternion quaternion, Ogre::Vector3 scale)
{
  addStaticTrimesh(meshName, defaultRestitution, defaultFriction, position, quaternion);
}

//-------------------------------------------------------------------------------------
void Architecture::addStaticTrimesh(Ogre::String meshName, float restitution, float friction, Ogre::Vector3 position, Ogre::Quaternion quaternion, Ogre::Vector3 scale)
{
  Ogre::Entity* entity = 0;
  physx::PxTriangleMesh* mesh = 0;
  
  //searching for previously created entities
  for(std::map<Ogre::String, StaticEntity>::iterator iter = statics.begin(); iter != statics.end(); ++iter)
  {
    if((*iter).first == meshName) 
    {
      entity = (*iter).second.entity;
      mesh = (*iter).second.mesh;
      break;
    }
  }

  //creating entity if not already created
  if(!entity || !mesh)
  {
    std::cout << "creating " << meshName << std::endl;
    entity = scene->getSceneManager()->createEntity(meshName);
    mesh = scene->getWorld()->createTriangleMeshV2(entity);
    statics.insert(std::pair<Ogre::String,StaticEntity>(meshName, StaticEntity(entity,mesh)));
  }

  /*Construct PhysX triangle mesh here.*/
  physx::PxRigidStatic* actor = scene->getWorld()->getPhysics()->createRigidStatic(physx::PxTransform(physx::PxVec3(position.x, position.y, position.z), physx::PxQuat(quaternion.x,quaternion.y,quaternion.z,quaternion.w)));
  actor->createShape(physx::PxTriangleMeshGeometry(mesh), *scene->getWorld()->getDefaultPhysicsMaterial());

  scene->getPhysicsManager()->addActor(*actor);

  actors.push_back(actor);
  instanceNumber++;

  geometry->addEntity(entity, position, quaternion, scale);
}

//-------------------------------------------------------------------------------------
void Architecture::build()
{
  geometry->build();
}



