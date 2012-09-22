#include "DataManager.h"

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
    if(file == *iter) throw NHException("Attempting to load the same data file twice.");
  }
  files.push_back(file);//record loading this file

  std::ifstream ifs(file);
  std::string temp;

  std::string type = "";
  std::string group = "";

  while(std::getline(ifs, temp))
  {
    if(temp == "") continue;
    
    if(temp[0] == '#') 
    {
      if(type == "") type = temp;
      continue;
    }
    else if(temp[0] == '[')//assuming this line is the start of a new group
    {
      group = temp;
      continue;
    }

    std::vector<std::string> words;
    boost::split(words, temp, boost::is_any_of(","), boost::token_compress_on);

    if(words.size() < 3) continue;
    int id = std::atoi(words[0].c_str());
    std::string name = words[1];
    std::string mesh = words[2];
    
    if(type == ARCHITECTURE_IDENTIFIER) architecture.insert(std::pair<int, ArchitectureDesc>(id, ArchitectureDesc(name, mesh)));
    else if(type == MONSTERS_IDENTIFIER) monsters.insert(std::pair<int, MonsterDesc>(id, MonsterDesc(name, mesh)));
    else if(type == ITEMS_IDENTIFIER) items.insert(std::pair<int, ItemDesc>(id, ItemDesc(name, mesh)));
    else if(type == SCENES_IDENTIFIER) scenes.insert(std::pair<int, SceneDesc>(id, SceneDesc(name, mesh)));
    else if(type == SOUNDS_IDENTIFIER) sounds.insert(std::pair<int, SoundDesc>(id, SoundDesc(name, mesh)));
  }
}

//-------------------------------------------------------------------------------------
ItemDesc DataManager::getItem(int id)
{
  if(items.count(id) == 0) throw NHException("Could not find an item with the given id.");
  return ((*(items.find(id))).second);
}

//-------------------------------------------------------------------------------------
MonsterDesc DataManager::getMonster(int id)
{
  if(monsters.count(id) == 0) throw NHException("Could not find a monster with the given id.");
  return ((*(monsters.find(id))).second);
}

//-------------------------------------------------------------------------------------
ArchitectureDesc DataManager::getArchitecture(int id)
{
  if(architecture.count(id) == 0) throw NHException("Could not find architecture with the given id.");
  return ((*(architecture.find(id))).second);
}

//-------------------------------------------------------------------------------------
SceneDesc DataManager::getScene(int id)
{
  if(scenes.count(id) == 0) throw NHException("Could not find a scene with the given id.");
  return ((*(scenes.find(id))).second);
}

//-------------------------------------------------------------------------------------
SoundDesc DataManager::getSound(int id)
{
  if(sounds.count(id) == 0) throw NHException("Could not find a sound with the given id.");
  return ((*(sounds.find(id))).second);
}

//-------------------------------------------------------------------------------------
std::vector<std::string> DataManager::getLoadedDataFiles()
{
  return files;
}


