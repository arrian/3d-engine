#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <boost/algorithm/string.hpp>

#include <OgreColourValue.h>
#include "Vector3.h"

#include "NHException.h"

#define GRAVITY 0.0f, -9.81f, 0.0f

struct ArchitectureDesc
{
  ArchitectureDesc(int id, std::string name, std::string mesh)
    : name(name),
      mesh(mesh),
      id(id),
      friction(0.2f),
      restitution(0.5f),
      isNavigable(true),
      isPhysical(true),
      isStatic(true)
  {
  }

  std::string mesh;
  std::string name;

  float friction;
  float restitution;

  bool isNavigable;//enable ai navigation
  bool isStatic;//enable static mesh generation
  bool isPhysical;//enable physics fabrication

  int id;
};

struct ItemDesc
{
  ItemDesc(int id, std::string name, std::string mesh, std::string simplifiedMesh)
    : name(name),
      mesh(mesh),
      simplifiedMesh(simplifiedMesh),
      dynamicFriction(0.7f),
      staticFriction(0.7f),
      restitution(0.5f),
      density(0.1f),
      id(id),
      isDynamic(true),
      isPhysical(true)
  {
  }

  std::string mesh;
  std::string simplifiedMesh;//mesh suitable for collison
  std::string name;

  float dynamicFriction;
  float staticFriction;
  float restitution;
  float density;

  bool isDynamic;//movement applies eg. gravity and forces
  bool isPhysical;//can interact with the world

  int id;
};

struct MonsterDesc
{
  MonsterDesc(int id, std::string name, std::string mesh)
    : name(name),
      mesh(mesh),
      height(1.75f),
      speed(1.0f),
      health(100.0f),
      id(id),
      gravity(0,0,0)//GRAVITY)//temp disable gravity while fixing artificial intelligence
  {
  }

  std::string mesh;
  std::string name;

  float speed;
  float height;
  float health;
  Vector3 gravity;

  int id;
};

struct SceneDesc
{
  SceneDesc(int id, std::string name, std::string file)
    : name(name),
      file(file),
      id(id),
      ambientLight(0.1f,0.1f,0.1f),
      shadowColour(1.0f,1.0f,1.0f),
      gravity(GRAVITY),
      north(0.0f, 0.0f, 1.0f),
      fogStart(100.0f),
      fogEnd(200.0f),
      fogColour(0.5f,0.5f,0.5f)
  {
  }

  std::string file;
  std::string name;

  int id;

  Ogre::ColourValue ambientLight;
  Ogre::ColourValue shadowColour;
  Vector3 gravity;
  Vector3 north;
  float fogStart;
  float fogEnd;
  Ogre::ColourValue fogColour;
};

struct SoundDesc
{
  SoundDesc(int id, std::string name, std::string file)
    : name(name),
      file(file),
      id(id)
  {
  }

  std::string file;
  std::string name;

  int id;
};

struct PlayerDesc
{
  PlayerDesc()
    : mesh("actor.mesh"),
      name("Default Name"),
      gravity(GRAVITY)
  {
  }

  std::string mesh;
  std::string name;

  Vector3 gravity;

};

typedef std::map<int, ArchitectureDesc > ArchitectureList;
typedef std::map<int, MonsterDesc > MonsterList;
typedef std::map<int, SceneDesc > SceneList;
typedef std::map<int, SoundDesc > SoundList;
typedef std::map<int, ItemDesc > ItemList;

static const char ARCHITECTURE_IDENTIFIER[] = "#Architecture";
static const char MONSTERS_IDENTIFIER[] = "#Monsters";
static const char SCENES_IDENTIFIER[] = "#Scenes";
static const char SOUNDS_IDENTIFIER[] = "#Sounds";
static const char ITEMS_IDENTIFIER[] = "#Items";

static const int ID_INDEX = 0;
static const int NAME_INDEX = 1;
static const int MESH_INDEX = 2;

/**
 * Handles the loading of all object information from the data files.
 */
class DataManager
{
public:
  DataManager(void);
  ~DataManager(void);

  void addData(std::string file);

  //Data getters
  ArchitectureDesc getArchitecture(int id);
  MonsterDesc getMonster(int id);
  SceneDesc getScene(int id);
  SoundDesc getSound(int id);
  ItemDesc getItem(int id);

  std::vector<std::string> getLoadedDataFiles();

private:
  std::vector<std::string> files;

  ArchitectureList architecture;
  MonsterList monsters;
  SceneList scenes;
  SoundList sounds;
  ItemList items;
};

