#include "World.h"

#include "Scene.h"
#include "Player.h"
#include "Item.h"
#include "Monster.h"

#include "extensions/PxVisualDebuggerExt.h"

World::World(Ogre::Root* root)
  : Environment(),
    root(root),
    scenes(),
    player(0),
    monsters(),
    items(),
    physicsFoundation(0),
    physicsWorld(0),
    sceneChangeListener(0),
    physicsMaterial(0),
    allocatorCallback(),
    errorCallback()
{
}

World::~World(void)
{
  if(player) delete player;
  player = 0;

  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    if(*it) delete (*it);
    (*it) = 0;
  }

  physicsWorld->release();
  physicsWorld = 0;

  physicsFoundation->release();
  physicsFoundation = 0;
}

void World::initialise(std::string iniFile)
{
  //loading initialisation file
  parseIni(iniFile);

  //creating physics
  physx::PxAllocatorCallback* allocator = &allocatorCallback;
  physicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *allocator, errorCallback);//getErrorCallback());
  physicsFoundation->setErrorLevel(physx::PxErrorCode::eDEBUG_INFO);
  if(!physicsFoundation) throw NHException("Physics foundation could not be created.");

  bool recordMemoryAllocations = true;
  physicsWorld = PxCreatePhysics(PX_PHYSICS_VERSION, *physicsFoundation, physx::PxTolerancesScale(), recordMemoryAllocations);
  if(!physicsWorld) throw NHException("Physics world could not be created.");

  physicsCooking = PxCreateCooking(PX_PHYSICS_VERSION, physicsWorld->getFoundation(), physx::PxCookingParams());
  if(!physicsCooking) throw NHException("Physics cooker could not be created.");
  //should release cooking during gameplay

  physicsMaterial = physicsWorld->createMaterial(0.5f,0.5f,0.1f);
  if(!physicsMaterial) throw NHException("Default physics material could not be created.");

  //creating initial scene
  loadScene(defaultStartLevel, FILE_CHAR);

  //creating player and placing in initial scene
  player = new Player(this);
  getScene("test")->addPlayer(player);

  physx::PxExtensionVisualDebugger::connect(physicsWorld->getPvdConnectionManager(), "127.0.0.1", 5425, 10000);
}

void World::hookWindow(Ogre::RenderWindow* window)
{
  assert(player);
  player->hook(window);
}

Player* World::getPlayer()
{
  return player;
}

Scene* World::getScene(Ogre::String name)
{
  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    if((*it)->getName() == name) return (*it);
  }
  return 0;
}

Scene* World::getScene(unsigned int index)
{
  if(index < scenes.size()) return scenes[index];
  return 0;
}

int World::getNumberScenes()
{
  return scenes.size();
}

Ogre::Root* World::getRoot()
{
  return root;
}

physx::PxPhysics* World::getPhysics()
{
  return physicsWorld;
}

const physx::PxTolerancesScale& World::getTolerancesScale()
{
  return physicsWorld->getTolerancesScale();
}

void World::movePlayer(Player* player, Scene* target)
{
  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    (*it)->removePlayer(player);
  }
  target->addPlayer(player);
}

void World::getSceneNames(std::vector<Ogre::String> &names)
{
  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    names.push_back((*it)->getName());
  }
}

Scene* World::loadScene(Ogre::String name, SceneType type)
{
  Scene* scene = new Scene(this, name, type);
  scenes.push_back(scene);
  return scene;
}

bool World::destroyScene(Ogre::String name)
{
  //is unsafe

  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    if((*it)->getName() == name) 
    {
      Scene* scene = *it;
      if(scene->isActive()) return false;//can't destroy active scene
      scenes.erase(it);
      delete scene;
      return true;
    }
  }
  return false;
}

bool World::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    if((*it)->isActive()) (*it)->frameRenderingQueued(evt);//only send frame events to the active scenes
  }
  return true;
}

void World::injectKeyDown(const OIS::KeyEvent &arg)
{
  player->injectKeyDown(arg);
}

void World::injectKeyUp(const OIS::KeyEvent &arg)
{
  player->injectKeyUp(arg);
}

void World::injectMouseMove(const OIS::MouseEvent &arg)
{
  player->injectMouseMove(arg);
}

void World::injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  player->injectMouseDown(arg, id);
}

void World::injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  player->injectMouseUp(arg, id);
}

void World::setSceneChangeListener(SceneChangeListener* listener)
{
  sceneChangeListener = listener;
}

SceneChangeListener* World::getSceneChangeListener()
{
  return sceneChangeListener;
}

physx::PxConvexMesh* World::createConvexMesh(Ogre::Entity *e)
{
  unsigned int mVertexCount = 0; 
  unsigned int mIndexCount  = 0; 
  size_t vertex_count; 
  Ogre::Vector3* vertices; 
  size_t index_count; 
  unsigned long* indices; 
  bool added_shared = false; 
  vertex_count = 0; 
  index_count = 0; 
  size_t current_offset = 0; 
  size_t shared_offset = 0; 
  size_t next_offset = 0; 
  size_t index_offset = 0; 
  for (unsigned short i = 0; i < e->getMesh()->getNumSubMeshes(); ++i) 
  { 
    Ogre::SubMesh* submesh = e->getMesh()->getSubMesh(i); 
    if(submesh->useSharedVertices) 
    { 
      if(!added_shared) 
      { 
        mVertexCount += e->getMesh()->sharedVertexData->vertexCount; 
        added_shared = true;
      } 
    } 
    else 
    { 
      mVertexCount += submesh->vertexData->vertexCount; 
    } 
    mIndexCount += submesh->indexData->indexCount; 
  } 

  vertices = new Ogre::Vector3[mVertexCount]; 
  indices = new unsigned long[mIndexCount]; 

  physx::PxVec3* mMeshVertices = new physx::PxVec3[mVertexCount]; 
  physx::PxU32* mMeshFaces = new physx::PxU32[mIndexCount]; 

  added_shared = false; 

  for (unsigned short i = 0; i < e->getMesh()->getNumSubMeshes();i++) 
  {
    Ogre::SubMesh* submesh = e->getMesh()->getSubMesh(i); 
    Ogre::VertexData* vertex_data=submesh->useSharedVertices ? e->getMesh()->sharedVertexData:submesh->vertexData; 

    if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared)) 
    { 
      if(submesh->useSharedVertices) 
      { 
        added_shared = true; 
        shared_offset = current_offset; 
      } 

      const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION); 
      Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource()); 
      unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY)); 

      float* pReal; 

      for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize()) 
      { 
        posElem->baseVertexPointerToElement(vertex, &pReal); 
        mMeshVertices[current_offset + j] = physx::PxVec3(pReal[0],pReal[1],pReal[2]); 
      } 

      vbuf->unlock(); 
      next_offset += vertex_data->vertexCount; 
    } 

    Ogre::IndexData* index_data = submesh->indexData; 

    size_t numTris = index_data->indexCount / 3; 
    Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer; 

    bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT); 

    if (use32bitindexes)   
    {
      unsigned int*  pInt = static_cast<unsigned int*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
      size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset; 

      for (size_t k = 0; k < numTris*3; ++k) mMeshFaces[index_offset] = pInt[k] + static_cast<unsigned int>(offset);
    } 
    else 
    {
      unsigned short* pShort = reinterpret_cast<unsigned short*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY)); 
      size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset; 

      for ( size_t k = 0; k < numTris*3; ++k) mMeshFaces[index_offset] = static_cast<unsigned int>(pShort[k]) + static_cast<unsigned int>(offset);
    } 
    ibuf->unlock(); 
    current_offset = next_offset; 
  } 
  physx::PxConvexMeshDesc convexDesc;

  convexDesc.points.count     = mVertexCount; 
  convexDesc.points.data      = mMeshVertices; 
  convexDesc.points.stride    = sizeof(physx::PxVec3); 

  convexDesc.triangles.count  = mIndexCount / 3; 
  convexDesc.triangles.stride = 3 * sizeof(physx::PxU32); 
  convexDesc.triangles.data   = mMeshFaces; 
  convexDesc.flags            = physx::PxConvexFlag::eCOMPUTE_CONVEX;

  /*
  MemoryWriteBuffer buf;
  bool status = physicsCooking->cookConvexMesh(convexDesc, buf);
  physx::PxConvexMesh* convexMesh = physicsWorld->createConvexMesh(MemoryReadBuffer(buf.data));
  */
  physx::PxConvexMesh* convexMesh = 0;//temp

  //if (!actor2) std::cout << "Could not create a physics actor" << std::endl;//???
  delete []vertices;
  delete []indices;
  delete []mMeshVertices;
  delete []mMeshFaces;
  return convexMesh;
}


physx::PxTriangleMesh* World::createTriangleMesh(Ogre::Entity* e) 
{
  unsigned int mVertexCount = 0; 
  unsigned long mIndexCount = 0; 
  size_t vertex_count; 
  Ogre::Vector3* vertices; 
  size_t index_count; 
  unsigned long* indices; 
  bool added_shared = false; 
  //vertex_count = 0; 
  //index_count = 0; 
  size_t current_offset = 0; 
  size_t shared_offset = 0; 
  size_t next_offset = 0; 
  size_t index_offset = 0; 

  for ( unsigned short i = 0; i < e->getMesh()->getNumSubMeshes(); ++i) 
  { 
    Ogre::SubMesh* submesh = e->getMesh()->getSubMesh( i ); 

    if(submesh->useSharedVertices) 
    { 
      if(!added_shared) 
      { 
        mVertexCount += e->getMesh()->sharedVertexData->vertexCount; 
        added_shared = true; 
      } 
    } 
    else mVertexCount += submesh->vertexData->vertexCount;

    mIndexCount += submesh->indexData->indexCount; 
  } 

  vertices = new Ogre::Vector3[mVertexCount]; 
  indices = new unsigned long[mIndexCount]; 

  physx::PxVec3* mMeshVertices = new physx::PxVec3[mVertexCount]; 
  physx::PxU32* mMeshFaces = new physx::PxU32[mIndexCount]; 

  added_shared = false; 

  for (unsigned short i = 0; i < e->getMesh()->getNumSubMeshes();i++) 
  {
    Ogre::SubMesh* submesh = e->getMesh()->getSubMesh(i); 
    Ogre::VertexData* vertex_data=submesh->useSharedVertices ? e->getMesh()->sharedVertexData:submesh->vertexData; 

    if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared)) 
    { 
      if(submesh->useSharedVertices) 
      { 
        added_shared = true; 
        shared_offset = current_offset; 
      } 

      const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION); 
      Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource()); 
      unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY)); 

      float* pReal; 

      for(size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize()) 
      { 
        posElem->baseVertexPointerToElement(vertex, &pReal); 
        mMeshVertices[current_offset + j] = physx::PxVec3(pReal[0],pReal[1],pReal[2]); 
      }

      vbuf->unlock(); 
      next_offset += vertex_data->vertexCount; 
    }

    Ogre::IndexData* index_data = submesh->indexData; 

    size_t numTris = index_data->indexCount / 3; 
    Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer; 

    bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT); 

    if(use32bitindexes) 
    { 
      unsigned int* pInt = static_cast<unsigned int*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY)); 
      size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset; 

      for (size_t k = 0; k < numTris*3; ++k) mMeshFaces[index_offset] = pInt[k] + static_cast<unsigned int>(offset); 
    } 
    else 
    { 
      unsigned short* pShort = reinterpret_cast<unsigned short*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY)); 
      size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset; 

      for (size_t k = 0; k < numTris * 3; ++k) mMeshFaces[index_offset] = static_cast<unsigned int>(pShort[k]) + static_cast<unsigned int>(offset);
    } 

    ibuf->unlock(); 
    current_offset = next_offset; 
  }

  physx::PxTriangleMeshDesc meshDesc;

  meshDesc.points.count      = mVertexCount; 
  meshDesc.points.data       = mMeshVertices; 
  meshDesc.points.stride     = 4 * 3; 

  meshDesc.triangles.count   = mIndexCount / 3; 
  meshDesc.triangles.stride  = 4 * 3; 
  //meshDesc.triangles.data    = mMeshFaces;

  
  /*
  MemoryWriteBuffer buf;
  physx::PxOutputStream buf2;
  bool status = physicsCooking->cookTriangleMesh(meshDesc, buf);
  
  physx::PxTriangleMesh* triangleMesh = physicsWorld->createTriangleMesh(MemoryReadBuffer(buf.data));
  */
  physx::PxTriangleMesh* triangleMesh = 0;//temp

  delete []vertices;
  delete []indices;
  delete []mMeshVertices;
  delete []mMeshFaces;
  return triangleMesh;
}

physx::PxMaterial* World::getDefaultPhysicsMaterial()
{
  return physicsMaterial;
}

Item* World::createItem()
{
  Item* item = new Item();
  items.push_back(item);
  return item;
}

Monster* World::createMonster()
{
  Monster* monster = new Monster();
  monsters.push_back(monster);
  return monster;
}

void World::releaseItem(Item* item)
{
  //!!! fix to delete from items list
  if(item) delete item;
}

void World::releaseMonster(Monster* monster)
{
  //!!! fix to delete from monsters list
  if(monster) delete monster;
}



