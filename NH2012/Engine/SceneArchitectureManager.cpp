#include "SceneArchitectureManager.h"

#include "Scene.h"
#include "World.h"

#include "SceneGraphicsManager.h"
#include "ScenePhysicsManager.h"
#include "ScenePathfindManager.h"

#include "OgreStaticGeometry.h"

//-------------------------------------------------------------------------------------
SceneArchitectureManager::SceneArchitectureManager(boost::shared_ptr<Scene> scene)
  : Identifiable(this, "Main", ARCHITECTURE),
    scene(boost::weak_ptr<Scene>(scene)),
    rootNode(scene->getSceneGraphicsManager()->createSceneNode()),
    instanceNumber(0),
    geometry(scene->getSceneGraphicsManager()->createStaticGeometry("architecture")),//potentially unsafe operation... ensure all objects used by getGraphicsManager have been constructed
    nodes(),
    actors(),
    shapes(),
    entities(),
    isBuilt(false)
{
  //geometry->setCastShadows(true);//produces no shadows at all?
}

//-------------------------------------------------------------------------------------
SceneArchitectureManager::~SceneArchitectureManager(void)
{
  boost::shared_ptr<Scene> scene_ptr = getScene();
  if(!scene_ptr)
  {
#ifdef _DEBUG
    std::cout << "Scene expired before calling scene architecture manager destructor." << std::endl;
#endif
    return;
  }

  for(std::vector<std::string>::iterator it = entities.begin(); it != entities.end(); ++it) 
  {
    if(getScene() && getScene()->getWorld()) getScene()->getWorld()->getPhysicsManager()->getFabrication()->releaseTriangleMesh(*it);
  }

  for(std::vector<physx::PxShape*>::iterator it = shapes.begin(); it != shapes.end(); ++it) 
  {
    if(*it) (*it)->release();
    (*it) = NULL;
  }

  for(std::vector<physx::PxRigidStatic*>::iterator it = actors.begin(); it != actors.end(); ++it) 
  {
    
    if(*it) (*it)->release();
    (*it) = NULL;
  }
  
  for(std::vector<Ogre::SceneNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) 
  {
    if(*it && getScene() && getScene()->getSceneGraphicsManager()) getScene()->getSceneGraphicsManager()->destroySceneNode(*it);
    (*it) = NULL;
  }
}

//-------------------------------------------------------------------------------------
void SceneArchitectureManager::add(ArchitectureDesc description, Vector3 position, Quaternion quaternion, Vector3 scale)
{
  addStaticTrimesh(description.mesh, description.restitution, description.friction, position, quaternion);
}

//-------------------------------------------------------------------------------------
void SceneArchitectureManager::addStaticTrimesh(std::string meshName, float restitution, float friction, Vector3 position, Quaternion quaternion, Vector3 scale)
{
  boost::shared_ptr<Scene> scene_ptr = getScene();
  if(!scene_ptr) throw NHException("Found expired scene while adding static trimesh in scene architecture manager.");

  boost::shared_ptr<World> world_ptr = scene_ptr->getWorld();
  if(!world_ptr) throw NHException("Found expired world while adding static trimesh in scene architecture manager.");

  Ogre::Entity* entity = scene_ptr->getSceneGraphicsManager()->createEntity(meshName);
  if(!entity) throw NHException("could not create architecture entity");

  physx::PxTriangleMesh* mesh = NULL;

  mesh = world_ptr->getPhysicsManager()->getFabrication()->createTriangleMesh(entity->getMesh()->getName(), entity);

  if(!mesh) throw NHException("could not create architecture triangle mesh");

  //construct architecture actor
  physx::PxRigidStatic* actor = world_ptr->getPhysicsManager()->getPhysics()->createRigidStatic(physx::PxTransform(physx::PxVec3(position.x, position.y, position.z), physx::PxQuat(quaternion.x,quaternion.y,quaternion.z,quaternion.w)));
  actor->userData = (Identifiable*) this;
  physx::PxShape* shape = actor->createShape(physx::PxTriangleMeshGeometry(mesh), *world_ptr->getPhysicsManager()->getDefaultMaterial());
  if(!shape) throw NHException("could not create architecture physics shape");
  physx::PxFilterData filter;
  filter.word0 = ARCHITECTURE;
  shape->setQueryFilterData(filter);

  scene_ptr->getScenePhysicsManager()->addActor(*actor);

  entities.push_back(entity->getMesh()->getName());
  shapes.push_back(shape);
  actors.push_back(actor);
  instanceNumber++;

  Ogre::SceneNode* child = rootNode->createChildSceneNode();
  child->setPosition(position);
  child->setOrientation(quaternion);
  child->setScale(scale);
  child->attachObject(entity);

  if(scene_ptr->getScenePathfindManager()) scene_ptr->getScenePathfindManager()->addGeometry(entity);
}

//-------------------------------------------------------------------------------------
void SceneArchitectureManager::build()
{
  if(isBuilt) throw NHException("attempting to build the architecture multiple times");
  isBuilt = true;

  geometry->addSceneNode(rootNode);
  geometry->build();

  //scene->getSceneGraphicsManager()->destroySceneNode(rootNode);
  //is there anything leftover here?
}



