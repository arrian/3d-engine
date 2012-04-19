#include "Architecture.h"


Architecture::Architecture(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics)
  : physics(physics),
    sceneManager(sceneManager),
    instanceNumber(0),
    geometry(sceneManager->createStaticGeometry("architecture")),
    nodes(),
    entities(),
    bodies(),
    shapes()
{
}


Architecture::~Architecture(void)
{
  for(std::vector<OgreBulletDynamics::RigidBody*>::iterator it = bodies.begin(); it != bodies.end(); ++it) 
  {
    if(*it) delete (*it);
    (*it) = 0;
  }

  for(std::vector<OgreBulletCollisions::CollisionShape*>::iterator it = shapes.begin(); it != shapes.end(); ++it) 
  {
    if(*it) delete (*it);
    (*it) = 0;
  }
  
  for(std::vector<Ogre::SceneNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) 
  {
    if(sceneManager) sceneManager->destroySceneNode(*it);
    (*it) = 0;
  }

  for(std::map<Ogre::String, Ogre::Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) 
  {
    if(sceneManager) sceneManager->destroyEntity(it->second);
    (*it).second = 0;
  }
}

void Architecture::add(Ogre::String meshName, Ogre::Vector3 position, Ogre::Quaternion quaternion)
{
  addStaticTrimesh(meshName, 0.2, 0.7, position, quaternion);
}

OgreBulletDynamics::RigidBody* Architecture::addStaticTrimesh(Ogre::String meshName, Ogre::Real restitution, const Ogre::Real friction, Ogre::Vector3 position, Ogre::Quaternion quaternion)
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
    entity = sceneManager->createEntity(meshName);
    entities.insert(std::pair<Ogre::String,Ogre::Entity*>(meshName, entity));
  }

  OgreBulletCollisions::StaticMeshToShapeConverter* trimeshConverter = new OgreBulletCollisions::StaticMeshToShapeConverter(entity);
  OgreBulletCollisions::TriangleMeshCollisionShape* shape = trimeshConverter->createTrimesh();
  delete trimeshConverter;
  OgreBulletDynamics::RigidBody* body = new OgreBulletDynamics::RigidBody(
      "architecture" + Ogre::StringConverter::toString(instanceNumber),
      physics);
  instanceNumber++;
  body->setStaticShape(shape, restitution, friction, position, quaternion);

  shapes.push_back(shape);
  bodies.push_back(body);

  geometry->addEntity(entity, position, quaternion);

  return body;
}

void Architecture::build()
{
  geometry->build();
}

