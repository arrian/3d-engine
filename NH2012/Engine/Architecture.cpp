#include "Architecture.h"

#include "Scene.h"
#include "World.h"

#include "OgreStaticGeometry.h"

//-------------------------------------------------------------------------------------
Architecture::Architecture(Scene* scene, PathfindManager* pathfinder)
  : IdentificationInterface("Main", "Architecture"),
    scene(scene),
    rootNode(scene->getSceneManager()->getRootSceneNode()->createChildSceneNode()),
    instanceNumber(0),
    geometry(scene->getSceneManager()->createStaticGeometry("architecture")),//potentially unsafe operation... ensure all objects used by getscenemanager have been constructed
    nodes(),
    actors(),
    statics(),
    pathfinder(pathfinder),
    isBuilt(false)
{
  //geometry->setCastShadows(true);//produces no shadows at all?
}

//-------------------------------------------------------------------------------------
Architecture::~Architecture(void)
{
  for(std::vector<physx::PxRigidStatic*>::iterator it = actors.begin(); it != actors.end(); ++it) 
  {
    if(*it) (*it)->release();
    (*it) = NULL;
  }
  
  for(std::vector<Ogre::SceneNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) 
  {
    if(*it) scene->getSceneManager()->destroySceneNode(*it);
    (*it) = NULL;
  }

  //do i need this
  /*
  for(std::map<Ogre::String, StaticEntity>::iterator it = statics.begin(); it != statics.end(); ++it) 
  {
    if(it->second.entity) scene->getSceneManager()->destroyEntity(it->second.entity);
    if(it->second.mesh) it->second.mesh->release();
    
    it->second.entity = NULL;
    it->second.mesh = NULL;
  }*/
}

//-------------------------------------------------------------------------------------
void Architecture::add(ArchitectureDesc description, Ogre::Vector3 position, Ogre::Quaternion quaternion, Ogre::Vector3 scale)
{
  addStaticTrimesh(description.mesh, description.restitution, description.friction, position, quaternion);
}

//-------------------------------------------------------------------------------------
void Architecture::addStaticTrimesh(Ogre::String meshName, float restitution, float friction, Ogre::Vector3 position, Ogre::Quaternion quaternion, Ogre::Vector3 scale)
{
  Ogre::Entity* entity = NULL;
  physx::PxTriangleMesh* mesh = NULL;
  
  //searching for previously created entities//change to use find
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
    std::cout << "Building " << meshName << "... ";
    entity = scene->getSceneManager()->createEntity(meshName);
    mesh = scene->getWorld()->getFabricationManager()->createTriangleMeshV2(entity);
    statics.insert(std::pair<Ogre::String,StaticEntity>(meshName, StaticEntity(entity,mesh)));
    std::cout << "done." << std::endl;
  }

  /*Construct PhysX triangle mesh here.*/
  physx::PxRigidStatic* actor = scene->getWorld()->getPhysics()->createRigidStatic(physx::PxTransform(physx::PxVec3(position.x, position.y, position.z), physx::PxQuat(quaternion.x,quaternion.y,quaternion.z,quaternion.w)));
  actor->createShape(physx::PxTriangleMeshGeometry(mesh), *scene->getWorld()->getDefaultPhysicsMaterial());
  actor->userData = this;

  scene->getPhysicsManager()->addActor(*actor);

  actors.push_back(actor);
  instanceNumber++;


  Ogre::SceneNode* child = rootNode->createChildSceneNode();
  child->setPosition(position);
  child->setOrientation(quaternion);
  child->setScale(scale);
  child->attachObject(entity);

  if(pathfinder) pathfinder->addEntity(entity);
  //geometry->addEntity(entity, position, quaternion, scale);
}

//-------------------------------------------------------------------------------------
void Architecture::build()
{
  if(isBuilt) throw NHException("Attempting to build the architecture multiple times.");
  isBuilt = true;

  geometry->addSceneNode(rootNode);
  geometry->build();

  scene->destroySceneNode(rootNode);
  //is there anything leftover here?
}

//-------------------------------------------------------------------------------------
void Architecture::update(double elapsedSeconds)
{
  //if(pathfinder) pathfinder->update(elapsedSeconds);
}

