#pragma once

#include "Vector3.h"
#include "BasicComponent.h"
#include "QueryComponent.h"
#include "Scene.h"
#include <OgreSceneManager.h>

class IvyStem;
class Ivy;

struct IvySettings
{

  IvySettings()
    : stemLength(0.5),
      growthSpeed(0.1),
      cohesionDistance(1.0),
      gravity(-9.81),
      minimumStemLength(0.1),
      stemSurvival(0.9)
  {
  }

  double stemSurvival;
  double stemLength;
  double minimumStemLength;
  double growthSpeed;
  double cohesionDistance;

  double gravity;
};

class IvyLeaf
{
public:
  IvyLeaf();
  virtual ~IvyLeaf();

  void update(double elapsedSeconds);

private:
  IvyStem* parent;

  double length;
  double width;
  Vector3 normal;
  Vector3 direction;

  int segmentNumber;
};

class IvyStem
{
public:
  IvyStem(Ivy* root, Vector3 start, Vector3 direction, Vector3 localUp, bool attached);

  virtual ~IvyStem();

  void update(double elapsedSeconds);

  IvyStem* getLeft();
  IvyStem* getRight();
  IvyLeaf* getLeaf();

  bool isDone();

private:
  Ivy* root;

  IvyStem* parent;
  IvyStem* left;
  IvyStem* right;
  IvyLeaf* leaf;

  Vector3 start;
  Vector3 end;
  Vector3 unitDirection;
  Vector3 up;

  bool done;
  bool climbing;
  bool attached;//true if this stem has found an attachment point
  Vector3 attachmentNormal;//the normal of the face at the attachment point - the next segment will run perpendicular to this

  int segmentNumber;

  double targetLength;
};

class Ivy
{
public:
  Ivy(Scene* scene, Vector3 position, Vector3 direction);

  virtual ~Ivy(void);

  void update(double elapsedSeconds);

  Scene* getScene();
  QueryComponent* getQuery();
  IvySettings* getSettings();
  Ogre::ManualObject* getMesh();

  int getNextSegmentNumber();

  bool raycast(Vector3 position, Vector3 unitDirection, float distance, physx::PxRaycastHit& hit);
  Vector3 project(Vector3 vector, Vector3 normal);
  bool survive(float surviveProbability);

private:
  IvyStem* root;

  Scene* scene;
  Ogre::SceneNode* node;

  QueryComponent query;

  std::vector<IvyStem*> active;

  Ogre::ManualObject* mesh;
  int segmentNumber;

  IvySettings settings;
};

