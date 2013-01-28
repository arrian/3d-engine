#include "DataManager.h"

//Standard
#include <iostream>
#include <fstream>

//Boost
#include <boost/lexical_cast.hpp>

//Ogre
#include <OgreResourceGroupManager.h>

//Local
#include "NHException.h"


//-------------------------------------------------------------------------------------
DataManager::DataManager(void)
  : files(std::vector<std::string>()),
    architecture(ArchitectureList()),
    items(ItemList()),
    monsters(MonsterList()),
    scenes(SceneList()),
    sounds(SoundList())
{
}

//-------------------------------------------------------------------------------------
DataManager::~DataManager(void)
{
}

//-------------------------------------------------------------------------------------
void DataManager::addData(std::string file)
{
  for(std::vector<std::string>::iterator iter = files.begin(); iter < files.end(); ++iter)
  {
    if(file == *iter) throw NHException("attempting to load the same data file twice");
  }
  files.push_back(file);//record loading this file

  Ogre::FileInfoListPtr fileListPtr = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceFileInfo("Essential", file);
  if(fileListPtr->size() < 1) throw NHException("could not find the path to the specified data file");

  std::ifstream ifs(fileListPtr->front().archive->getName() + "/" + fileListPtr->front().filename);
  std::string temp;

  std::string type = "";

  int line = 0;

  while(std::getline(ifs, temp))
  {
    line++;

    if(temp == "") continue;
    
    if(temp[0] == '#')//comment
    {
      if(type == "") type = temp;
      continue;
    }

    if(temp[0] == '[') continue;//new group

    std::vector<std::string> words;
    boost::split(words, temp, boost::is_any_of(","), boost::token_compress_on);

    if(words.size() == 0) continue;
    
    int id = -1;
    try
    {
      id = boost::lexical_cast<int>(words[ID_INDEX]);
    }
    catch(boost::bad_lexical_cast ex)
    {
      //we can, in general, safely ignore any badly formatted data entries but we should warn the user
      std::cout << "Incorrectly formatted data entry in " << file << " on line " << line << "." << std::endl;
      continue;
    }

    std::string name;
    std::string mesh;
    
    if(words.size() >= 2) name = words[NAME_INDEX];
    else name = "Name Error";
    
    if(words.size() >= 3) mesh = words[MESH_INDEX];
    else mesh = "error.mesh";

    //Need to extend for other data options
    if(type == ARCHITECTURE_IDENTIFIER) architecture.insert(std::pair<int, ArchitectureDesc>(id, ArchitectureDesc(id, name, mesh)));
    else if(type == MONSTERS_IDENTIFIER) monsters.insert(std::pair<int, MonsterDesc>(id, MonsterDesc(id, name, mesh)));
    else if(type == ITEMS_IDENTIFIER) items.insert(std::pair<int, ItemDesc>(id, ItemDesc(id, name, mesh, mesh)));//change second mesh argument to the simplified mesh
    else if(type == SCENES_IDENTIFIER) scenes.insert(std::pair<int, SceneDesc>(id, SceneDesc(id, name, mesh)));
    else if(type == SOUNDS_IDENTIFIER) sounds.insert(std::pair<int, SoundDesc>(id, SoundDesc(id, name, mesh)));
  }
}

//-------------------------------------------------------------------------------------
ItemDesc DataManager::getItem(int id)
{
  if(items.count(id) == 0) throw NHException("could not find an item with the given id");
  return ((*(items.find(id))).second);
}

//-------------------------------------------------------------------------------------
MonsterDesc DataManager::getMonster(int id)
{
  if(monsters.count(id) == 0) throw NHException("could not find a monster with the given id");
  return ((*(monsters.find(id))).second);
}

//-------------------------------------------------------------------------------------
ArchitectureDesc DataManager::getArchitecture(int id)
{
  if(architecture.count(id) == 0) throw NHException("could not find architecture with the given id");
  return ((*(architecture.find(id))).second);
}

//-------------------------------------------------------------------------------------
SceneDesc DataManager::getScene(int id)
{
  if(scenes.count(id) == 0) throw NHException("could not find a scene with the given id");
  return ((*(scenes.find(id))).second);
}

//-------------------------------------------------------------------------------------
SoundDesc DataManager::getSound(int id)
{
  if(sounds.count(id) == 0) throw NHException("could not find a sound with the given id");
  return ((*(sounds.find(id))).second);
}

//-------------------------------------------------------------------------------------
std::vector<std::string> DataManager::getLoadedDataFiles()
{
  return files;
}


