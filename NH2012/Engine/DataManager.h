#pragma once

#include <OgreString.h>

#include <boost/algorithm/string.hpp>

struct ArchitectureModel
{
  ArchitectureModel(Ogre::String name, Ogre::String mesh)
    : name(name),
      mesh(mesh)
  {
  }

  Ogre::String mesh;
  Ogre::String name;
};

struct ItemModel
{
  ItemModel(Ogre::String name, Ogre::String mesh)
    : name(name),
      mesh(mesh)
  {
  }

  Ogre::String mesh;
  Ogre::String name;

  int value;
};

struct MonsterModel
{
  MonsterModel(Ogre::String name, Ogre::String mesh)
    : name(name),
    mesh(mesh)
  {
  }

  Ogre::String mesh;
  Ogre::String name;

  Ogre::Real frequency;
  Ogre::Real speed;
  Ogre::Real size;
  Ogre::Real weight;
};

struct SceneDesc
{
  SceneDesc(Ogre::String name, Ogre::String file)
    : name(name),
    file(file)
  {
  }

  Ogre::String file;
  Ogre::String name;
};

struct SoundDesc
{
  SoundDesc(Ogre::String name, Ogre::String file)
    : name(name),
      file(file)
  {
  }

  Ogre::String file;
  Ogre::String name;
};

typedef std::map<int, ArchitectureModel > ArchitectureList;
typedef std::map<Ogre::String, std::vector<ArchitectureModel*> > ArchitectureGroups;

typedef std::map<int, ItemModel > ItemList;
typedef std::map<Ogre::String, std::vector<ItemModel*> > ItemGroups;

typedef std::map<int, MonsterModel > MonsterList;
typedef std::map<Ogre::String, std::vector<MonsterModel*> > MonsterGroups;

typedef std::map<int, SceneDesc > SceneList;
typedef std::map<Ogre::String, std::vector<SceneDesc*> > SceneGroups;

typedef std::map<int, SoundDesc > SoundList;
typedef std::map<Ogre::String, std::vector<SoundDesc*> > SoundGroups;

/* Handles the loading of object mesh names from the data files.*/
class DataManager
{
public:
  DataManager(void);
  ~DataManager(void);

  /* Analyses the given data file.*/
  void addData(Ogre::String file);

  ItemModel* getItem(int id);
  MonsterModel* getMonster(int id);
  ArchitectureModel* getArchitecture(int id);
  SceneDesc* getScene(int id);
  SoundDesc* getSound(int id);
private:
  std::vector<Ogre::String> files;

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

