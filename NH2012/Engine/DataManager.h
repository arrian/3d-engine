#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <boost/algorithm/string.hpp>

#include "NHException.h"


struct ArchitectureDesc
{
  ArchitectureDesc(std::string name, std::string mesh)
    : name(name),
      mesh(mesh)
  {
  }

  std::string mesh;
  std::string name;
};


struct ItemDesc
{
  ItemDesc(std::string name, std::string mesh, std::string simplifiedMesh)
    : name(name),
      mesh(mesh),
      simplifiedMesh(simplifiedMesh),
      dynamicFriction(0.7f),
      staticFriction(0.7f),
      resititution(0.5f),
      density(0.1f)
  {
  }

  std::string mesh;
  std::string simplifiedMesh;//mesh suitable for collison
  std::string name;

  float dynamicFriction;
  float staticFriction;
  float resititution;
  float density;

  int instanceID;
  
};

struct MonsterDesc
{
  MonsterDesc(std::string name, std::string mesh)
    : name(name),
      mesh(mesh)
  {
  }

  std::string mesh;
  std::string name;

  float frequency;
  float speed;
  float size;
  float weight;

  int instanceID;
};

struct SceneDesc
{
  SceneDesc(std::string name, std::string file)
    : name(name),
      file(file)
  {
  }

  std::string file;
  std::string name;
};

struct SoundDesc
{
  SoundDesc(std::string name, std::string file)
    : name(name),
      file(file)
  {
  }

  std::string file;
  std::string name;
};

typedef std::map<int, ArchitectureDesc > ArchitectureList;
typedef std::map<int, MonsterDesc > MonsterList;
typedef std::map<int, SceneDesc > SceneList;
typedef std::map<int, SoundDesc > SoundList;
typedef std::map<int, ItemDesc > ItemList;

#define ARCHITECTURE_IDENTIFIER "#Architecture"
#define MONSTERS_IDENTIFIER "#Monsters"
#define SCENES_IDENTIFIER "#Scenes"
#define SOUNDS_IDENTIFIER "#Sounds"
#define ITEMS_IDENTIFIER "#Items"

#define ID_INDEX 0
#define NAME_INDEX 1
#define MESH_INDEX 2

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

