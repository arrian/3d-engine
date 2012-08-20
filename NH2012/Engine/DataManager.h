#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <boost/algorithm/string.hpp>

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
  ItemDesc(std::string name, std::string mesh)
    : name(name),
      mesh(mesh)
  {
  }

  std::string mesh;
  std::string name;

  int value;
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
typedef std::map<std::string, std::vector<ArchitectureDesc*> > ArchitectureGroups;

typedef std::map<int, ItemDesc > ItemList;
typedef std::map<std::string, std::vector<ItemDesc*> > ItemGroups;

typedef std::map<int, MonsterDesc > MonsterList;
typedef std::map<std::string, std::vector<MonsterDesc*> > MonsterGroups;

typedef std::map<int, SceneDesc > SceneList;
typedef std::map<std::string, std::vector<SceneDesc*> > SceneGroups;

typedef std::map<int, SoundDesc > SoundList;
typedef std::map<std::string, std::vector<SoundDesc*> > SoundGroups;

/* Handles the loading of object mesh names from the data files.*/
class DataManager
{
public:
  DataManager(void);
  ~DataManager(void);

  /* Analyses the given data file.*/
  void addData(std::string file);

  ItemDesc* getItem(int id);
  MonsterDesc* getMonster(int id);
  ArchitectureDesc* getArchitecture(int id);
  SceneDesc* getScene(int id);
  SoundDesc* getSound(int id);

  ArchitectureGroups* getArchitectureGroups();
  ItemGroups* getItemGroups();
  MonsterGroups* getMonsterGroups();
  SceneGroups* getSceneGroups();
  SoundGroups* getSoundGroups();
private:
  std::vector<std::string> files;

  ArchitectureList architecture;
  ArchitectureGroups architectureGroups;

  ItemList items;
  ItemGroups itemGroups;

  MonsterList monsters;
  MonsterGroups monsterGroups;

  SceneList scenes;
  SceneGroups sceneGroups;

  SoundList sounds;
  SoundGroups soundGroups;

  std::string SOUNDS_IDENTIFIER;
  std::string ARCHITECTURE_IDENTIFIER;
  std::string ITEMS_IDENTIFIER;
  std::string MONSTERS_IDENTIFIER;
  std::string SCENES_IDENTIFIER;
  
};

