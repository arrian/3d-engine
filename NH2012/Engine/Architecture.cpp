#include "Architecture.h"

#include "Scene.h"

Architecture::Architecture(Scene* scene)
  : scene(scene),
    instanceNumber(0),
    geometry(scene->getSceneManager()->createStaticGeometry("architecture")),//potentially unsafe operation... ensure all objects used by getscenemanager have been constructed
    nodes(),
    entities(),
    bodies()
{
}

Architecture::~Architecture(void)
{
  for(std::vector<physx::PxTriangleMesh*>::iterator it = bodies.begin(); it != bodies.end(); ++it) 
  {
    if(*it) (*it)->release();
    (*it) = 0;
  }
  
  for(std::vector<Ogre::SceneNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) 
  {
    if(scene->getSceneManager()) scene->getSceneManager()->destroySceneNode(*it);
    (*it) = 0;
  }

  for(std::map<Ogre::String, Ogre::Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) 
  {
    if(scene->getSceneManager()) scene->getSceneManager()->destroyEntity(it->second);
    (*it).second = 0;
  }
}

void Architecture::add(Ogre::String meshName, Ogre::Vector3 position, Ogre::Quaternion quaternion)
{
  addStaticTrimesh(meshName, 0.2f, 0.7f, position, quaternion);
}

void Architecture::addStaticTrimesh(Ogre::String meshName, Ogre::Real restitution, const Ogre::Real friction, Ogre::Vector3 position, Ogre::Quaternion quaternion)
{
  Ogre::Entity* entity = 0;
  
  //searching for previously created entites
  for(std::map<Ogre::String, Ogre::Entity*>::iterator iter = entities.begin(); iter != entities.end(); ++iter)
  {
    if((*iter).first == meshName) entity = (*iter).second;
  }

  //creating entity if not already created
  if(!entity)
  {
    std::cout << "creating " << meshName << std::endl;
    entity = scene->getSceneManager()->createEntity(meshName);
    entities.insert(std::pair<Ogre::String,Ogre::Entity*>(meshName, entity));
  }

  /*Construct PhysX triangle mesh here.*/

  /*
  OgreBulletCollisions::StaticMeshToShapeConverter* trimeshConverter = new OgreBulletCollisions::StaticMeshToShapeConverter(entity);
  OgreBulletCollisions::TriangleMeshCollisionShape* shape = trimeshConverter->createTrimesh();
  delete trimeshConverter;
  OgreBulletDynamics::RigidBody* body = new OgreBulletDynamics::RigidBody(
      "architecture" + Ogre::StringConverter::toString(instanceNumber),
      physics);
  instanceNumber++;
  body->setStaticShape(shape, restitution, friction, position, quaternion);

  bodies.push_back(body);
  */

  geometry->addEntity(entity, position, quaternion);
}

void Architecture::build()
{
  geometry->build();
}

