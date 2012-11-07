
#pragma once

#include "OgreVector3.h"
#include "OgreManualObject.h"
#include "OgreSceneManager.h"
#include "OgreRenderOperation.h"
#include "PxScene.h"


//typedef Ogre::Vector3 Vector3;//change this if not using Ogre3D and have an equivalent 3 dimensional vector class.

//class PlantSegment;
//typedef std::vector<PlantSegment*> Segments;//a set of plant segments

/**
 * Defines a basic quadrilateral.
 */
/*
struct Quad
{
  Quad(Vector3 first, Vector3 second, Vector3 third, Vector3 fourth)
    : first(first),
      second(second),
      third(third),
      fourth(fourth)
  {
  }

  Vector3 first;
  Vector3 second;
  Vector3 third;
  Vector3 fourth; 
};

typedef std::vector<Quad> Mesh;//a simple triangle mesh
*/
/**
 * Generates the default plant settings. On creating an instance, 
 * values can be changed as required before passing to Plant.
 */
/*struct PlantDesc
{
  PlantDesc()
    :  //worldScale(1.0f),
       plantSegments(100),
       plantGravity(-9.81f),
       plantStrength(3.0f),
       plantGrowSpeed(0.1f),
       leafSize(0.1f),
       leafWeight(0.05f),
       leafProbability(0.5f),
       branchLength(0.4f),
       branchProbability(0.7f),
       branchWeight(0.4f),
       branchInitialDiameter(0.1f),
       branchFinalDiameter(0.01f),
       seed(0)
  {
  }

  int plantSegments;//the maximum number of branch segments from root to tip
  float plantGravity;
  float plantStrength;//opposes gravity
  float plantGrowSpeed;//growth speed in world units per second

  float leafSize;
  float leafWeight;
  float leafProbability;

  float branchLength;
  float branchProbability;
  float branchWeight;
  float branchInitialDiameter;//defines the thickness of the trunk
  float branchFinalDiameter;//defines the thickness at the extremities of the branches

  int seed;//the random seed of this plant
};

class PlantSegment
{
public:
  PlantSegment(int level, PlantDesc* desc, Vector3 start, Vector3 direction)
    : level(level),
      desc(desc),
      start(start),
      direction(direction),
      mesh(),
      isGrowing(true),
      length(0)
  {
  }

  virtual ~PlantSegment(){}

  virtual void grow(double elapsedSeconds) = 0;

  void setUserData(void* userData)
  {
    this->userData = userData;
  }

  void* getUserData()
  {
    return userData;
  }

  Mesh* getMesh()
  {
    return &mesh;
  }

protected:
  int level;
  bool isGrowing;
  PlantDesc* desc;
  Vector3 start;
  Vector3 end;
  Vector3 direction;
  double length;
  void* userData;
  Mesh mesh;
};

class PlantLeaf : PlantSegment
{
public:
  PlantLeaf(int level, PlantDesc* desc, Vector3 start, Vector3 direction)
    : PlantSegment(level, desc, start, direction),
      size(0)
  {
    mesh.push_back(Quad(start, start + Vector3(0.4f,0.0f,0.0f), start + Vector3(0.4f,0.0f,0.4f), start + Vector3(0.0f,0.0f,0.4f)));//testing leaf location
  }

  ~PlantLeaf()
  {

  }

  virtual void grow(double elapsedSeconds)
  {
    if(isGrowing)
    {
      if(size < desc->leafSize)
      {
        size += elapsedSeconds;
        if(size > desc->leafSize) size = desc->leafSize;
      }
      else
      {
        isGrowing = false;
      }
    }
  }

private:
  float size;//specifies the leaf's current size. maximum size defined in the desc
  Ogre::Quaternion leafRotation;
};


class PlantBranch : PlantSegment
{
public:
  PlantBranch(int level, PlantDesc* desc, Vector3 start, Vector3 direction)
    : PlantSegment(level, desc, start, direction)
  {
    end = start + Vector3(0,0,desc->branchLength);//change to random later
  }

  virtual ~PlantBranch()
  {
    //delete all branches and leaves here
  }

  Vector3 end;
  Segments segments;//child segments

  virtual void grow(double elapsedSeconds)//returns the active segments
  {
    if(desc->branchLength*desc->branchLength > start.squaredDistance(end))
    {
      //grow more in the required direction
    }
    else
    {
      if(level < desc->plantSegments)
      {
        float genBranch = (float)rand() / (float)RAND_MAX;
        float genLeaf = (float)rand() / (float)RAND_MAX;
        if(desc->leafProbability || desc->branchProbability > genBranch) createLeaf();//randomly create leaves along branches or if this is the end of the branch, create a leaf
        if(desc->branchProbability <= genBranch) createBranch();
      }
      else
      {
        createLeaf();
      }
    }
    
    growChildren(elapsedSeconds);
  }

  void createLeaf()
  {
    std::cout << "Generating leaf..." << std::endl;
    segments.push_back((PlantSegment*) new PlantLeaf(level + 1, desc, end, direction.perpendicular()));
  }

  void createBranch()
  {
    std::cout << "Generating branch..." << std::endl;
    segments.push_back((PlantSegment*) new PlantBranch(level + 1, desc, end, direction));
  }

  void growChildren(double elapsedSeconds)
  {
    for(Segments::iterator iter = segments.begin(); iter < segments.end(); ++iter) 
    {
      (*iter)->grow(elapsedSeconds);
    }
  }
};

*/

/**
 * A plant generator that uses two simple rules to grow:
 * 1) If the branch has no obstacles in the direction it is growing, then attempt to grow towards the ground.
 * 2) If the branch has obstacles in the direction it is growing, then attempt to climb the obstacle.
 */
/*class Plant
{
public:
  Plant(PlantDesc desc, Vector3 position)
    : desc(desc),
      position(position),
      branch(0,&(desc),position,Vector3(0,0,0))//generate the initial branch
  {
    srand(desc.seed);//seeding locally to attempt to get more repeatable results
  }

  virtual ~Plant()
  {
  }
  
  void grow(double elapsedSeconds)//seconds elapsed since the last grow call was made
  {
    branch.grow(elapsedSeconds);
  }

private:
  PlantDesc desc;//plant description
  Vector3 position;
  PlantBranch branch;//initial branch
};*/

/**
 * Generates the plant mesh using the Ogre3D (http://www.ogre3d.org) ManualObject. 
 */
/*
class PlantOgreMesh : Plant
{
public:
  PlantOgreMesh(Ogre::SceneManager* sceneManager, PlantDesc desc, Vector3 position)
    : Plant(desc, position),
      mesh(sceneManager->createManualObject("IvyTest"))
  {
    sceneManager->getRootSceneNode()->attachObject(mesh);
  }

  virtual ~PlantOgreMesh()
  {
  }

  void grow(float elapsedSeconds)
  {
    Plant::grow(elapsedSeconds);
  }

  virtual void updateMesh()
  {
    for(Segments::iterator iter = segments.begin(); iter < segments.end(); ++iter)
    {
      //std::cout << "Updating segment..." << std::endl;
      if((*iter)->getUserData() == NULL)
      { 
        (*iter)->setUserData((void*) mesh->getNumSections());//saving the section index for future updates
        mesh->begin("Leaf1", Ogre::RenderOperation::OT_TRIANGLE_LIST);//start a new section

        int i = 0;
        for(Mesh::iterator tri = (*iter)->getMesh()->begin(); tri < (*iter)->getMesh()->end(); ++tri)//iterating triangles
        {
          mesh->position(tri->first);
          mesh->position(tri->second);
          mesh->position(tri->third);
          mesh->position(tri->fourth);

          int offset = i * 4;
          mesh->triangle(offset, offset + 1, offset + 2);
          mesh->triangle(offset, offset + 2, offset + 3);
          i++;
        }
        mesh->end();
      }
      else 
      {
        //mesh->beginUpdate((unsigned int)(*iter)->getUserData());//update a previously created section
        //mesh->end();
      }
    }
  }

  Ogre::ManualObject* mesh;
};


struct PlantQueryResult
{
  float distance;
  Ogre::Vector3 normal;
};

class PlantQueryInterface
{
public:
  PlantQueryInterface();
  virtual ~PlantQueryInterface();

  virtual PlantQueryResult rayQuery(Vector3 start, Vector3 direction) = 0;
  virtual PlantQueryResult sweepQuery(Vector3 start, Vector3 leftDirection, Vector3 rightDirection) = 0;
  virtual PlantQueryResult sphereQuery(Vector3 start, float radius) = 0;
};*/

/**
 * Queries the world using PhysX. 
 * Leads to a fair amount of indirection but makes swapping the physics manager much easier.
 */
/*class PlantQueryPhysX : PlantQueryInterface
{
public:
  PlantQueryPhysX(physx::PxScene* scene);
  virtual ~PlantQueryPhysX();

  physx::PxScene* scene;

  PlantQueryResult rayQuery(Vector3 start, Vector3 direction, float length)
  {
    physx::PxRaycastHit hit;
    scene->raycastSingle(physx::PxVec3(start.x,start.y,start.z), physx::PxVec3(direction.x, direction.y, direction.z), length, physx::PxSceneQueryFlags(), hit);
    
    PlantQueryResult ogreHit;
    ogreHit.distance = hit.distance;
    ogreHit.normal = Vector3(hit.normal.x, hit.normal.y, hit.normal.z);
  }

  PlantQueryResult sweepQuery(Vector3 start, Vector3 leftDirection, Vector3 rightDirection)
  {

  }

  PlantQueryResult sphereQuery(Vector3 start, float radius)
  {

  }
};

*/
