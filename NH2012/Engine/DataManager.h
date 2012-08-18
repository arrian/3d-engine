#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <boost/algorithm/string.hpp>

struct ArchitectureModel
{
  ArchitectureModel(std::string name, std::string mesh)
    : name(name),
      mesh(mesh)
  {
  }

  std::string mesh;
  std::string name;
};

struct ItemModel
{
  ItemModel(std::string name, std::string mesh)
    : name(name),
      mesh(mesh)
  {
  }

  std::string mesh;
  std::string name;

  int value;
};

struct MonsterModel
{
  MonsterModel(std::string name, std::string mesh)
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

typedef std::map<int, ArchitectureModel > ArchitectureList;
typedef std::map<std::string, std::vector<ArchitectureModel*> > ArchitectureGroups;

typedef std::map<int, ItemModel > ItemList;
typedef std::map<std::string, std::vector<ItemModel*> > ItemGroups;

typedef std::map<int, MonsterModel > MonsterList;
typedef std::map<std::string, std::vector<MonsterModel*> > MonsterGroups;

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

  ItemModel* getItem(int id);
  MonsterModel* getMonster(int id);
  ArchitectureModel* getArchitecture(int id);
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

