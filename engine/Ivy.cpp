#include "Ivy.h"

#include <boost/random.hpp>

#include "OgreManualObject.h"
#include "SceneGraphicsManager.h"
#include "ScenePhysicsManager.h"

IvyLeaf::IvyLeaf()
{

}

IvyLeaf::~IvyLeaf()
{

}

void IvyLeaf::update(double elapsedSeconds)
{

}

IvyStem::IvyStem(Ivy* root, Vector3 start, Vector3 direction, Vector3 localUp, bool attached)
  : root(root),
    start(start),
    end(start),
    unitDirection(direction),
    targetLength(root->getSettings()->stemLength),
    climbing(false),
    attached(attached),
    done(false),
    parent(NULL),
    left(NULL),
    right(NULL),
    leaf(NULL),
    attachmentNormal(),
    up(localUp)
{

  
  {//Growth direction raycast
    physx::PxRaycastHit hit;
    if(root->raycast(start, unitDirection, targetLength, hit)) 
    {
      if(hit.distance >= root->getSettings()->minimumStemLength) targetLength = hit.distance;
      else targetLength = root->getSettings()->minimumStemLength;//don't want grow length to be zero
    
      this->attached = true;
      attachmentNormal = Vector3(hit.normal.x, hit.normal.y, hit.normal.z);
      up = Vector3(hit.normal.x, hit.normal.y, hit.normal.z);
    }
    else this->attached = false;
  }

  Vector3 endPoint = start + direction * targetLength;

  if(!attached)//Raycast inversion of local up
  {
    physx::PxRaycastHit hit;
    if(root->raycast(start, -localUp.normalisedCopy(), targetLength, hit)) 
    {
      if(hit.distance >= root->getSettings()->minimumStemLength) targetLength = hit.distance;
      else targetLength = root->getSettings()->minimumStemLength;//don't want grow length to be zero

      this->attached = true;
      attachmentNormal = Vector3(hit.normal.x, hit.normal.y, hit.normal.z);
      up = Vector3(hit.normal.x, hit.normal.y, hit.normal.z);
    }
    else this->attached = false;
  }

  if(!attached)//Apply gravity
  {
    double gravityPotential = root->getSettings()->growthSpeed * root->getSettings()->gravity;

    endPoint.y += 0.3;//temp add upwards search

    physx::PxRaycastHit hit;
    if(root->raycast(endPoint, Vector3(0,-1,0), std::abs(gravityPotential) + 0.3, hit)) 
    {
      endPoint = Vector3(hit.impact.x, hit.impact.y, hit.impact.z);
      this->attached = true;
      attachmentNormal = Vector3(hit.normal.x, hit.normal.y, hit.normal.z);
      up = Vector3(hit.normal.x, hit.normal.y, hit.normal.z);
    }
    else
    {
      endPoint.y += gravityPotential;
    }

    targetLength = start.distance(endPoint);
    unitDirection = (endPoint - start).normalisedCopy();
  }



  

  segmentNumber = root->getNextSegmentNumber();
  root->getMesh()->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
  root->getMesh()->position(start);
  root->getMesh()->position(end);
  root->getMesh()->end();
}

IvyStem::~IvyStem()
{
  delete left;
  delete right;
  delete leaf;
}

void IvyStem::update(double elapsedSeconds)
{
  if(done) return;//should not happen

  end += unitDirection * elapsedSeconds * root->getSettings()->growthSpeed;
  if((end - start).length() >= targetLength)//reached the target length
  {
    end = start + unitDirection * targetLength;//optimise square root

    Vector3 leftDirection = unitDirection.randomDeviant(Ogre::Degree(15));
    Vector3 rightDirection = unitDirection.randomDeviant(Ogre::Degree(15));

    if(attached)//project to attachment plane
    {
      leftDirection = root->project(leftDirection, attachmentNormal);
      rightDirection = root->project(rightDirection, attachmentNormal);
    }

    leftDirection.normalise();
    rightDirection.normalise();

    //if(root->survive(root->getSettings()->stemSurvival)) left = new IvyStem(root, end, leftDirection, attached);//deviate direction
    right = new IvyStem(root, end, rightDirection, up, attached);//deviate direction

    done = true;
  }

  root->getMesh()->beginUpdate(segmentNumber);
  root->getMesh()->position(start);
  root->getMesh()->position(end);
  root->getMesh()->end();
}

IvyStem* IvyStem::getLeft() 
{
  return left; 
}

IvyStem* IvyStem::getRight() 
{ 
  return right;
}

IvyLeaf* IvyStem::getLeaf()
{
  return leaf;
}

bool IvyStem::isDone()
{
  return done;
}

Ivy::Ivy(boost::shared_ptr<Scene> scene, Vector3 position, Vector3 direction)
  : active(),
    segmentNumber(0),
    scene(scene.get()),
    query(),
    root(NULL)
{

  node = scene->getSceneGraphicsManager()->createSceneNode();
  //node->setPosition(position);

  mesh = scene->getSceneGraphicsManager()->createManualObject();
  node->attachObject(mesh);

  root = new IvyStem(this, position, direction.normalisedCopy(), -direction, false);
  active.push_back(root);

  query.setNode(scene, node);
}

Ivy::~Ivy(void) 
{
  delete root;
  scene->getSceneGraphicsManager()->destroyManualObject(mesh);
  scene->getSceneGraphicsManager()->destroySceneNode(node);
}

void Ivy::update(double elapsedSeconds)
{

  std::vector<IvyStem*> newStems = std::vector<IvyStem*>();

  for(std::vector<IvyStem*>::iterator iter = active.begin(); iter != active.end(); ++iter)
  {
    IvyStem* stem = (*iter);

    if(!stem->isDone())
    {
      stem->update(elapsedSeconds);
    }
    else
    {
      iter = active.erase(iter);
      
      IvyStem* left = stem->getLeft();
      IvyStem* right = stem->getRight();

      if(left) newStems.push_back(left);
      if(right) newStems.push_back(right);

      if(iter == active.end()) break;
    }
  }

  if(newStems.size() > 0) active.insert(active.end(), newStems.begin(), newStems.end());
}

Scene* Ivy::getScene()
{
  return scene;
}

Ogre::ManualObject* Ivy::getMesh()
{
  return mesh;
}

QueryComponent* Ivy::getQuery()
{
  return &query;
}

IvySettings* Ivy::getSettings()
{
  return &settings;
}

int Ivy::getNextSegmentNumber()
{
  segmentNumber++;
  return segmentNumber - 1;
}

bool Ivy::raycast(Vector3 position, Vector3 unitDirection, float distance, physx::PxRaycastHit& hit)
{
  physx::PxSceneQueryFlags outputFlags = physx::PxSceneQueryFlag::eDISTANCE | physx::PxSceneQueryFlag::eNORMAL | physx::PxSceneQueryFlag::eIMPACT;
  physx::PxSceneQueryFilterData filterData = physx::PxSceneQueryFilterData();
  filterData.data.word0 = ALL;
  return scene->getScenePhysicsManager()->getScenePhysics()->raycastSingle(physx::PxVec3(position.x, position.y, position.z), physx::PxVec3(unitDirection.x, unitDirection.y, unitDirection.z), distance, outputFlags, hit, filterData);
}

Vector3 Ivy::project(Vector3 vector, Vector3 normal)
{
  return vector - (vector.dotProduct(normal)) * normal;
  //B = A - (A.dot.N)N
}

bool Ivy::survive(float surviveProbability)
{
  //move statics into required class
  static boost::mt19937 rng;
  static boost::bernoulli_distribution<> dist(surviveProbability);
  static boost::variate_generator<boost::mt19937&, boost::bernoulli_distribution<> > nextValue(rng, dist);
  return nextValue();
}