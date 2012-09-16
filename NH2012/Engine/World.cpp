
//Local
#include "World.h"
#include "Scene.h"
#include "Player.h"
#include "Item.h"
#include "Monster.h"
#include "NHException.h"

//PhysX
#include "extensions/PxExtensionsAPI.h"
#include "extensions/PxVisualDebuggerExt.h"
#include "PxTkStream.h"

//-------------------------------------------------------------------------------------
World::World(Ogre::Root* root)
  : root(root),
    items(),
    scenes(),
    monsters(),
    dataFiles(),
    dataManager(),
    soundManager(),
    errorCallback(),
    allocatorCallback(),
    player(0),
    physicsWorld(0),
    physicsMaterial(0),
    physicsFoundation(0),
    sceneChangeListener(0),
    defaultRestitution(0.2f),
    defaultStaticFriction(0.3f),
    defaultDynamicFriction(0.4f),
    debug(false)
{
}

//-------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------
void World::initialise(std::string iniFile)
{
  //loading initialisation file
  parseIni(iniFile);

  //Adding datafiles collected from ini file to dataManager
  for(std::vector<std::string>::iterator it = dataFiles.begin(); it < dataFiles.end(); ++it) dataManager.addData(*it);

  //creating physics
  physx::PxAllocatorCallback* allocator = &allocatorCallback;
  physicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *allocator, errorCallback);//getErrorCallback());
  physicsFoundation->setErrorLevel(physx::PxErrorCode::eDEBUG_INFO);
  if(!physicsFoundation) throw NHException("Physics foundation could not be created.");

  bool recordMemoryAllocations = true;
  physicsWorld = PxCreatePhysics(PX_PHYSICS_VERSION, *physicsFoundation, physx::PxTolerancesScale(), recordMemoryAllocations);
  if(!physicsWorld) throw NHException("Physics world could not be created.");

  PxInitExtensions(*physicsWorld);

  physicsCooking = PxCreateCooking(PX_PHYSICS_VERSION, physicsWorld->getFoundation(), physx::PxCookingParams());
  if(!physicsCooking) throw NHException("Physics cooker could not be created.");
  //should release cooking during gameplay

  physicsMaterial = physicsWorld->createMaterial(defaultStaticFriction, defaultDynamicFriction, defaultRestitution);
  if(!physicsMaterial) throw NHException("Default physics material could not be created.");

  //creating default scene
  Scene* scene = loadScene(0);
  if(scene == 0) throw NHException("Default scene creation failed.");
  
  //creating player
  player = new Player(this);
  scene->addPlayer(player);

#ifdef _DEBUG
  //Creating connection to the PhysX Visual Debugger
  physx::PxExtensionVisualDebugger::connect(physicsWorld->getPvdConnectionManager(), physXVisualDebuggerIP.c_str(), physXVisualDebuggerPort, physXVisualDebuggerTimeoutMilliseconds);
#endif
}

//-------------------------------------------------------------------------------------
void World::hookWindow(Ogre::RenderWindow* window)
{
  assert(player);
  player->hook(window);
}

//-------------------------------------------------------------------------------------
Player* World::getPlayer()
{
  return player;
}

//-------------------------------------------------------------------------------------
Scene* World::getScene(Ogre::String name)
{
  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    if((*it)->getName() == name) return (*it);
  }
  return 0;
}

//-------------------------------------------------------------------------------------
Scene* World::getScene(int id)
{
  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    if((*it)->getSceneID() == id) return (*it);
  }
  return 0;
}

//-------------------------------------------------------------------------------------
int World::getNumberScenes()
{
  return scenes.size();
}

//-------------------------------------------------------------------------------------
/*WARNING: May be a null pointer.*/
Ogre::Root* World::getRoot()
{
  return root;
}

//-------------------------------------------------------------------------------------
physx::PxPhysics* World::getPhysics()
{
  return physicsWorld;
}

//-------------------------------------------------------------------------------------
const physx::PxTolerancesScale& World::getTolerancesScale()
{
  return physicsWorld->getTolerancesScale();
}

//-------------------------------------------------------------------------------------
void World::movePlayer(Player* player, Scene* target)
{
  if(!player) return;
  Scene* old = player->getScene();
  if(old) old->removePlayer(player);
  if(target) target->addPlayer(player);
}

//-------------------------------------------------------------------------------------
void World::getSceneNames(std::vector<Ogre::String> &names)
{
  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    names.push_back((*it)->getName());
  }
}

//-------------------------------------------------------------------------------------
Scene* World::loadScene(int id)
{
  if(hasScene(id)) return getScene(id);//check that the scene has not been loaded already
  Scene* scene = new Scene(this, id);
  scenes.push_back(scene);
  return scene;
}

//-------------------------------------------------------------------------------------
bool World::hasScene(int id)
{
  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it)
  {
    if((*it)->getSceneID() == id) return true;
  }
  return false;
}

//-------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------
bool World::update(double elapsedSeconds)
{
  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    if((*it)->isActive()) (*it)->update(elapsedSeconds);//only send frame events to the active scenes
  }
  return true;
}

//-------------------------------------------------------------------------------------
void World::injectKeyDown(const OIS::KeyEvent &arg)
{
  player->injectKeyDown(arg);
}

//-------------------------------------------------------------------------------------
void World::injectKeyUp(const OIS::KeyEvent &arg)
{
  player->injectKeyUp(arg);
}

//-------------------------------------------------------------------------------------
void World::injectMouseMove(const OIS::MouseEvent &arg)
{
  player->injectMouseMove(arg);
}

//-------------------------------------------------------------------------------------
void World::injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  player->injectMouseDown(arg, id);
}

//-------------------------------------------------------------------------------------
void World::injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  player->injectMouseUp(arg, id);
}

//-------------------------------------------------------------------------------------
void World::setSceneChangeListener(SceneChangeListener* listener)
{
  sceneChangeListener = listener;
}

//-------------------------------------------------------------------------------------
SceneChangeListener* World::getSceneChangeListener()
{
  return sceneChangeListener;
}

//-------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------
physx::PxTriangleMesh* World::createTriangleMesh(Ogre::Entity* e) 
{
  std::cout << "Got triangle mesh cooking request." << std::endl;

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

        std::cout << "x:" << pReal[0] << " z:"<< pReal[1] << " y:" << pReal[2] << std::endl;
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

  meshDesc.points.count     = mVertexCount; 
  meshDesc.points.data      = mMeshVertices; 
  meshDesc.points.stride    = 4 * 3; 

  meshDesc.triangles.count  = mIndexCount / 3; 
  meshDesc.triangles.stride = 4 * 3; 
  meshDesc.triangles.data   = mMeshFaces;

  
  PxToolkit::MemoryOutputStream buf;
  bool status = physicsCooking->cookTriangleMesh(meshDesc, buf);
  
  physx::PxTriangleMesh* triangleMesh = physicsWorld->createTriangleMesh(PxToolkit::MemoryInputData(buf.getData(), buf.getSize()));//MemoryReadBuffer(buf.data));

  delete []vertices;
  delete []indices;
  delete []mMeshVertices;
  delete []mMeshFaces;
  return triangleMesh;
}

//-------------------------------------------------------------------------------------
physx::PxTriangleMesh* World::createTriangleMeshV2(Ogre::Entity* e, Params &params, AddedMaterials *out_addedMaterials)
{
  Ogre::MeshPtr mesh = e->getMesh();//extracting mesh pointer from entity

  //getting the mesh info
  MeshInfo meshInfo;
  getMeshInfo(mesh, params, meshInfo);
  //mergeVertices(meshInfo);

  //physx desc prep
  physx::PxTriangleMeshDesc desc;
  desc.setToDefault();

  desc.points.count = meshInfo.vertices.size();
  desc.points.stride = 12;
  float *fVertices = new float[meshInfo.vertices.size()*3];
  for (unsigned int i = 0; i < meshInfo.vertices.size(); ++i)
  {
    fVertices[i*3] = meshInfo.vertices[i].x;
    fVertices[i*3+1] = meshInfo.vertices[i].y;
    fVertices[i*3+2] = meshInfo.vertices[i].z;
  }
  desc.points.data = fVertices;

  desc.triangles.count = meshInfo.indices.size() / 3;
  desc.triangles.stride = 12;
  physx::PxU32 *iIndices = new physx::PxU32[meshInfo.indices.size()];
  for (unsigned int i = 0; i < meshInfo.indices.size(); ++i)
    iIndices[i] = meshInfo.indices[i];
  desc.triangles.data = iIndices;

  std::unordered_map<physx::PxMaterial*, physx::PxMaterialTableIndex> materialIndicesMap;
  std::vector<physx::PxMaterial*> orderedMaterials;
  materialIndicesMap.insert(std::make_pair<physx::PxMaterial*, physx::PxMaterialTableIndex>(getDefaultPhysicsMaterial(), 0));//add default material at index 0
  orderedMaterials.push_back(getDefaultPhysicsMaterial());

  physx::PxMaterialTableIndex *materialIndices = nullptr;
  if (!params.mMaterialBindings.empty())
  {
    desc.materialIndices.stride = sizeof(physx::PxMaterialTableIndex);
    materialIndices = new physx::PxMaterialTableIndex[meshInfo.indices.size() / 3];
    for (unsigned int i = 0; i < meshInfo.indices.size() / 3; ++i)
    {
      auto mat = params.mMaterialBindings.find(meshInfo.materials[i]);
      if (mat == params.mMaterialBindings.end())
      {
        materialIndices[i] = 0;		//default material
      }
      else
      {
        auto matIndex = materialIndicesMap.find(mat->second);
        physx::PxMaterialTableIndex index;
        if (matIndex == materialIndicesMap.end())
        {	//create material index if it does not already exist
          index = materialIndicesMap.size();
          materialIndicesMap.insert(std::make_pair<physx::PxMaterial*, physx::PxMaterialTableIndex>(mat->second, index));
          orderedMaterials.push_back(mat->second);
        }
        else index = matIndex->second;
        materialIndices[i] = index;
      }
    }	
  }
  desc.materialIndices.data = materialIndices;

  if (out_addedMaterials)
  {
    out_addedMaterials->materialCount = materialIndicesMap.size();
    out_addedMaterials->materials = new physx::PxMaterial*[out_addedMaterials->materialCount];
    for (unsigned int i = 0; i < orderedMaterials.size(); ++i)
      out_addedMaterials->materials[i] = orderedMaterials[i];
  }


  PxToolkit::MemoryOutputStream buf;
  bool success = physicsCooking->cookTriangleMesh(desc, buf);
  if(!success) return 0;//something went wrong with the cooking
  physx::PxTriangleMesh* triangleMesh = physicsWorld->createTriangleMesh(PxToolkit::MemoryInputData(buf.getData(), buf.getSize()));

  delete[] fVertices;
  delete[] iIndices;
  if (materialIndices) delete[] materialIndices;

  return triangleMesh;
}

//-------------------------------------------------------------------------------------
void World::getMeshInfo(Ogre::MeshPtr mesh, Params &params, MeshInfo &outInfo)
{
	//First, we compute the total number of vertices and indices and init the buffers.
	unsigned int numVertices = 0;
	unsigned int numIndices = 0;

	if (mesh->sharedVertexData) numVertices += mesh->sharedVertexData->vertexCount;
	Ogre::Mesh::SubMeshIterator subMeshIterator = mesh->getSubMeshIterator();
	bool indices32 = true;
	while (subMeshIterator.hasMoreElements())
	{
		Ogre::SubMesh *subMesh = subMeshIterator.getNext();
		if (subMesh->vertexData) numVertices += subMesh->vertexData->vertexCount;
		if (params.mAddBackfaces)
			numIndices += subMesh->indexData->indexCount*2;
		else
			numIndices += subMesh->indexData->indexCount;
				
		//We assume that every submesh uses the same index format
		indices32 = (subMesh->indexData->indexBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
	}

	outInfo.vertices.resize(numVertices);
	outInfo.indices.resize(numIndices);
	outInfo.materials.resize(numIndices / 3);

	unsigned int addedVertices = 0;
	unsigned int addedIndices = 0;
	unsigned int addedMaterialIndices = 0;

	//Read shared vertices
	unsigned int shared_index_offset = 0;
	Ogre::VertexData *shared_vertex_data = mesh->sharedVertexData;
	if (shared_vertex_data)
	{
		const Ogre::VertexElement* posElem =
			shared_vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
		Ogre::HardwareVertexBufferSharedPtr vbuf =
			shared_vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

		shared_index_offset = shared_vertex_data->vertexCount;

		unsigned char* pVertices = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		Ogre::Real* pReal;
		for (size_t i = addedVertices; i < shared_vertex_data->vertexCount; i++)
		{
			posElem->baseVertexPointerToElement(pVertices, &pReal);
			Ogre::Vector3 vec;
			vec.x = (*pReal++) * params.mScale.x;
			vec.y = (*pReal++) * params.mScale.y;
			vec.z = (*pReal++) * params.mScale.z;
			outInfo.vertices[i] = vec;
			pVertices += vbuf->getVertexSize();
		}
		vbuf->unlock();
		addedVertices += shared_vertex_data->vertexCount;

	}

	unsigned int index_offset = 0;

	//Read submeshes
	subMeshIterator = mesh->getSubMeshIterator();
	while (subMeshIterator.hasMoreElements())
	{
		Ogre::SubMesh *subMesh = subMeshIterator.getNext();

		//Read vertex data
		Ogre::VertexData *vertex_data = subMesh->vertexData;
		if (vertex_data)
		{
			const Ogre::VertexElement* posElem =
				vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
			Ogre::HardwareVertexBufferSharedPtr vbuf =
				vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

			unsigned char* pVertices = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			Ogre::Real* pReal;
			for (size_t i = addedVertices; i < addedVertices+vertex_data->vertexCount; i++)
			{
				posElem->baseVertexPointerToElement(pVertices, &pReal);
				Ogre::Vector3 vec;
				vec.x = (*pReal++) * params.mScale.x;
				vec.y = (*pReal++) * params.mScale.y;
				vec.z = (*pReal++) * params.mScale.z;
				outInfo.vertices[i] = vec;
				pVertices += vbuf->getVertexSize();
			}
			addedVertices += vertex_data->vertexCount;

			vbuf->unlock();
		}

		//Read index data
		Ogre::IndexData *index_data = subMesh->indexData;
		if (index_data)
		{
			Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

			physx::PxU32 *pIndices = 0;
			if (indices32)
			{
				pIndices = static_cast<physx::PxU32*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			}
			else
			{
				physx::PxU16 *pShortIndices = static_cast<physx::PxU16*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
				pIndices = new physx::PxU32[index_data->indexCount];
				for (size_t k = 0; k < index_data->indexCount; k++) pIndices[k] = static_cast<physx::PxU32>(pShortIndices[k]);
			}
			unsigned int bufferIndex = 0;
			if (params.mAddBackfaces)
			{
				size_t numTris = index_data->indexCount / 3;
				size_t i = addedIndices;
				for (unsigned int x = 0; x < numTris; x++)
				{
					if (subMesh->useSharedVertices)
					{
						if (pIndices[bufferIndex] > shared_index_offset) outInfo.indices[i] = pIndices[bufferIndex] + index_offset;
						else outInfo.indices[i] = pIndices[bufferIndex];
						bufferIndex++;
						if (pIndices[bufferIndex] > shared_index_offset) outInfo.indices[i+1] = pIndices[bufferIndex] + index_offset;
						else outInfo.indices[i+1] = pIndices[bufferIndex];
						bufferIndex++;
						if (pIndices[bufferIndex] > shared_index_offset) outInfo.indices[i+2] = pIndices[bufferIndex] + index_offset;
						else outInfo.indices[i+2] = pIndices[bufferIndex];
						bufferIndex++;
					}
					else
					{
						outInfo.indices[i] = pIndices[bufferIndex] + index_offset;
						bufferIndex++;
						outInfo.indices[i+1] = pIndices[bufferIndex] + index_offset;
						bufferIndex++;
						outInfo.indices[i+2] = pIndices[bufferIndex] + index_offset;
						bufferIndex++;
					}
					outInfo.indices[i+3] = outInfo.indices[i+2];
					outInfo.indices[i+4] = outInfo.indices[i+1];
					outInfo.indices[i+5] = outInfo.indices[i];
					i += 6;
				}
				addedIndices += index_data->indexCount*2;
			}
			else
			{
				for (size_t i = addedIndices; i < addedIndices+index_data->indexCount; i++)
				{
					if (subMesh->useSharedVertices)
					{
						if (pIndices[bufferIndex] > shared_index_offset) outInfo.indices[i] = pIndices[bufferIndex] + index_offset;
						else outInfo.indices[i] = pIndices[bufferIndex];
					}
					else outInfo.indices[i] = pIndices[bufferIndex] + index_offset;
					bufferIndex++;
				}
				addedIndices += index_data->indexCount;
			}
			if (!indices32) delete[] pIndices;

			ibuf->unlock();

			//All triangles of a submesh have the same material
			unsigned int numTris = index_data->indexCount / 3;
			if (params.mAddBackfaces) numTris *= 2;
			for (size_t i = addedMaterialIndices; i < addedMaterialIndices+numTris; i++)
				outInfo.materials[i] = subMesh->getMaterialName();
			addedMaterialIndices += numTris;
		}

		if (vertex_data) index_offset += vertex_data->vertexCount;
	}
}

//-------------------------------------------------------------------------------------
physx::PxMaterial* World::getDefaultPhysicsMaterial()
{
  return physicsMaterial;
}

//-------------------------------------------------------------------------------------
Item* World::createItem(int id)
{
  Item* item = new Item(dataManager.getItem(id));
  items.push_back(item);
  return item;
}

//-------------------------------------------------------------------------------------
Monster* World::createMonster(int id)
{
  Monster* monster = new Monster(id);
  monsters.push_back(monster);
  return monster;
}

//-------------------------------------------------------------------------------------
void World::releaseItem(Item* item)
{
  //!!! fix to delete from items list
  if(item) delete item;
}

//-------------------------------------------------------------------------------------
void World::releaseMonster(Monster* monster)
{
  //!!! fix to delete from monsters list
  if(monster) delete monster;
}

//-------------------------------------------------------------------------------------
void World::parseIni(std::string filename)
{
  std::cout << "Getting initialisation data..." << std::endl;

  try
  {
    std::ifstream s(filename);

    if(!s) throw NHException("Error opening initialisation file.");

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(s, pt);

    std::string TRUE_STRING = "true";

    //General
    enablePhysics = (pt.get<std::string>("General.EnablePhysics") == TRUE_STRING);
    enableAI = (pt.get<std::string>("General.EnableAI") == TRUE_STRING);
    enableAudio = (pt.get<std::string>("General.EnableAudio") == TRUE_STRING);

    gravity = pt.get<float>("General.Gravity");

    //Controls //temp default values
    controls.moveForward = OIS::KC_W;
    controls.moveLeft = OIS::KC_A;
    controls.moveBack = OIS::KC_S;
    controls.moveRight = OIS::KC_D;

    controls.jump = OIS::KC_SPACE;
    controls.kick = OIS::KC_F;
    controls.run = OIS::KC_LSHIFT;
    controls.leftHand = OIS::MB_Left;
    controls.rightHand = OIS::MB_Right;

    controls.quickslots.push_back(OIS::KC_1);
    controls.quickslots.push_back(OIS::KC_2);
    controls.quickslots.push_back(OIS::KC_3);
    controls.quickslots.push_back(OIS::KC_4);
    controls.quickslots.push_back(OIS::KC_5);
    controls.quickslots.push_back(OIS::KC_6);
    controls.quickslots.push_back(OIS::KC_7);
    controls.quickslots.push_back(OIS::KC_8);
    controls.quickslots.push_back(OIS::KC_9);
    controls.quickslots.push_back(OIS::KC_0);

    controls.exit = OIS::KC_ESCAPE;

    controls.freezeCollision = OIS::KC_Q;
    controls.addItem = OIS::KC_1;
    controls.addMonster = OIS::KC_2;
    controls.reset = OIS::KC_3;

    controls.console = OIS::KC_GRAVE;

    //Environment
    enableHDR = (pt.get<std::string>("Environment.HDR") == TRUE_STRING);
    enableBloom = (pt.get<std::string>("Environment.Bloom") == TRUE_STRING);
    enableShadows = (pt.get<std::string>("Environment.Shadows") == TRUE_STRING);
    enableLights = (pt.get<std::string>("Environment.Lights") == TRUE_STRING);
    enableParticles = (pt.get<std::string>("Environment.Particles") == TRUE_STRING);
    enableDecals = (pt.get<std::string>("Environment.Decals") == TRUE_STRING);
    enableSprites = (pt.get<std::string>("Environment.Sprites") == TRUE_STRING);
    enableWater = (pt.get<std::string>("Environment.Water") == TRUE_STRING);
    enableSky = (pt.get<std::string>("Environment.Sky") == TRUE_STRING);

    //Data File Paths
    dataFiles.push_back(pt.get<std::string>("Scenes.Data"));
    dataFiles.push_back(pt.get<std::string>("Architecture.Data"));
    dataFiles.push_back(pt.get<std::string>("Monsters.Data"));
    dataFiles.push_back(pt.get<std::string>("Items.Data"));
    dataFiles.push_back(pt.get<std::string>("Sounds.Data"));

    //Debug
    debug = (pt.get<std::string>("Debug.DebugMode") == TRUE_STRING);
    verbose = (pt.get<std::string>("Debug.VerboseMode") == TRUE_STRING);
    freeCameraDebug = (pt.get<std::string>("Debug.FreeCamera") == TRUE_STRING);
    wireframeDebug = (pt.get<std::string>("Debug.Wireframe") == TRUE_STRING);
    freezeCollisionDebug = (pt.get<std::string>("Debug.FreezeCollision") == TRUE_STRING);
    showCollisionDebug = (pt.get<std::string>("Debug.ShowCollisionsDebug") == TRUE_STRING);
    showShadowDebug = (pt.get<std::string>("Debug.ShowShadowDebug") == TRUE_STRING);

    physXVisualDebuggerIP = pt.get<std::string>("Debug.PhysXVisualDebuggerIP");
    physXVisualDebuggerPort = pt.get<int>("Debug.PhysXVisualDebuggerPort");
    physXVisualDebuggerTimeoutMilliseconds = pt.get<int>("Debug.PhysXVisualDebuggerTimeoutMilliseconds");
  }
  catch(boost::property_tree::ini_parser::ini_parser_error e)
  {
    std::cout << e.what() << std::endl;
  }
  catch(boost::property_tree::ptree_bad_path e)
  {
    std::cout << e.what() << std::endl;
  }
}

//-------------------------------------------------------------------------------------
DataManager* World::getDataManager()
{
  return &dataManager;
}

//-------------------------------------------------------------------------------------
bool World::isDebug()
{
  return debug;
}

//-------------------------------------------------------------------------------------
void World::setRoot(Ogre::Root* root)
{
  this->root = root;
}


