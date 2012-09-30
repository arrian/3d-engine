#include "FabricationManager.h"

//PhysX
#include "extensions/PxExtensionsAPI.h"
#include "PxTkStream.h"

FabricationManager::FabricationManager()
  : material(NULL),
    cooking(NULL),
    physics(NULL)
{
}


FabricationManager::~FabricationManager(void)
{
}

//-------------------------------------------------------------------------------------
physx::PxConvexMesh* FabricationManager::createConvexMesh(const Ogre::MeshPtr& mesh)//Ogre::Entity *e)
{
  unsigned int mVertexCount = 0; 
  unsigned int mIndexCount  = 0; 
  //size_t vertex_count; 
  Ogre::Vector3* vertices; 
  //size_t index_count; 
  unsigned long* indices; 
  bool added_shared = false; 
  //vertex_count = 0; 
  //index_count = 0; 
  size_t current_offset = 0; 
  size_t shared_offset = 0; 
  size_t next_offset = 0; 
  size_t index_offset = 0; 
  for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) 
  { 
    Ogre::SubMesh* submesh = mesh->getSubMesh(i); 
    if(submesh->useSharedVertices) 
    { 
      if(!added_shared) 
      { 
        mVertexCount += mesh->sharedVertexData->vertexCount; 
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

  for (unsigned short i = 0; i < mesh->getNumSubMeshes();i++) 
  {
    Ogre::SubMesh* submesh = mesh->getSubMesh(i); 
    Ogre::VertexData* vertex_data=submesh->useSharedVertices ? mesh->sharedVertexData:submesh->vertexData; 

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

  PxToolkit::MemoryOutputStream buf;
  bool status = cooking->cookConvexMesh(convexDesc, buf);
  physx::PxConvexMesh* convexMesh = physics->createConvexMesh(PxToolkit::MemoryInputData(buf.getData(), buf.getSize()));

  delete []vertices;
  delete []indices;
  delete []mMeshVertices;
  delete []mMeshFaces;
  return convexMesh;
}

//-------------------------------------------------------------------------------------
physx::PxTriangleMesh* FabricationManager::createTriangleMesh(Ogre::Entity* e) 
{
  std::cout << "Got triangle mesh cooking request." << std::endl;

  unsigned int mVertexCount = 0; 
  unsigned long mIndexCount = 0; 
  //size_t vertex_count; 
  Ogre::Vector3* vertices; 
  //size_t index_count; 
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
  bool status = cooking->cookTriangleMesh(meshDesc, buf);
  
  physx::PxTriangleMesh* triangleMesh = physics->createTriangleMesh(PxToolkit::MemoryInputData(buf.getData(), buf.getSize()));//MemoryReadBuffer(buf.data));

  delete []vertices;
  delete []indices;
  delete []mMeshVertices;
  delete []mMeshFaces;
  return triangleMesh;
}

//-------------------------------------------------------------------------------------
physx::PxTriangleMesh* FabricationManager::createTriangleMeshV2(Ogre::Entity* e, Params &params, AddedMaterials *out_addedMaterials)
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
  materialIndicesMap.insert(std::make_pair<physx::PxMaterial*, physx::PxMaterialTableIndex>(material, 0));//add default material at index 0
  orderedMaterials.push_back(material);

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
  bool success = cooking->cookTriangleMesh(desc, buf);
  if(!success) return 0;//something went wrong with the cooking
  physx::PxTriangleMesh* triangleMesh = physics->createTriangleMesh(PxToolkit::MemoryInputData(buf.getData(), buf.getSize()));

  delete[] fVertices;
  delete[] iIndices;
  if (materialIndices) delete[] materialIndices;

  return triangleMesh;
}

//-------------------------------------------------------------------------------------
void FabricationManager::getMeshInfo(Ogre::MeshPtr mesh, Params &params, MeshInfo &outInfo)
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
void FabricationManager::setDefaultPhysicsMaterial(physx::PxMaterial* defaultPhysicsMaterial)
{
  this->material = defaultPhysicsMaterial;
}

//-------------------------------------------------------------------------------------
void FabricationManager::setCooking(physx::PxCooking* cooking)
{
  this->cooking = cooking;
}

//-------------------------------------------------------------------------------------
void FabricationManager::setPhysics(physx::PxPhysics* physics)
{
  this->physics = physics;
}

