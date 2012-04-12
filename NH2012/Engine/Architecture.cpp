#include "Architecture.h"


Architecture::Architecture(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics)
  : physics(physics),
    sceneManager(sceneManager),
    instanceNumber(0),
    geometry(sceneManager->createStaticGeometry("architecture"))
{

  nodes = std::vector<Ogre::SceneNode*>();
  entities = std::vector<Ogre::Entity*>();
  bodies = std::vector<OgreBulletDynamics::RigidBody*>();
  shapes = std::vector<OgreBulletCollisions::CollisionShape*>();


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
    if(*it) delete (*it);
    (*it) = 0;
  }

  for(std::vector<Ogre::Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) 
  {
    if(*it) delete (*it);
    (*it) = 0;
  }
}

void Architecture::add(Ogre::String meshName, Ogre::Vector3 position, Ogre::Quaternion quaternion)
{
  addStaticTrimesh(meshName, 0.2, 0.7, position, quaternion);
}

OgreBulletDynamics::RigidBody* Architecture::addStaticTrimesh(Ogre::String meshName, Ogre::Real restitution, const Ogre::Real friction, Ogre::Vector3 position, Ogre::Quaternion quaternion)
{
  // todo: implement static geometry
  /*
  int i=0;
  Ogre::Entity *ent = mSceneMgr->createEntity("cube_02.mesh");
  Ogre::StaticGeometry *sg = mSceneMgr->createStaticGeometry("cubes");
  for (i=0;i<5000;i++)
  {
      sg->addEntity(ent, Ogre::Vector3(100+i*100,-100,100));
  }
  sg->build();

  */


  //Ogre::SceneNode* node = sceneManager->getRootSceneNode()->createChildSceneNode();
  //nodes.push_back(node);

  Ogre::Entity* entity = 0;
  
  //searching for previously created entites
  bool found = false;
  for(std::vector<Ogre::Entity*>::iterator iter = entities.begin(); iter < entities.end(); ++iter)
  {
    if(entities.front()->getMesh()->getName() == meshName)
    {
      entity = (*iter);
      found = true;
    }
  }

  //creating entity if not already created
  if(!found)
  {
    entity = sceneManager->createEntity(meshName);
    entities.push_back(entity);
  }

  //node->attachObject(entity);

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

