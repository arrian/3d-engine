#include "DataManager.h"

//-------------------------------------------------------------------------------------
DataManager::DataManager(void)
  : files(std::vector<Ogre::String>()),
    architecture(ArchitectureList()),
    architectureGroups(ArchitectureGroups()),
    items(ItemList()),
    itemGroups(ItemGroups()),
    monsters(MonsterList()),
    monsterGroups(MonsterGroups()),
    scenes(SceneList()),
    sceneGroups(SceneGroups())
{
}

//-------------------------------------------------------------------------------------
DataManager::~DataManager(void)
{
}

//-------------------------------------------------------------------------------------
void DataManager::addData(Ogre::String file)
{
  std::string ARCHITECTURE = "#Architecture";
  std::string MONSTERS = "#Monsters";
  std::string ITEMS = "#Items";
  std::string SCENES = "#Scenes";

  files.push_back(file);

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
    else if(temp[0] == '[')//creating groups
    {
      group = temp;
      if(type == ARCHITECTURE && !architectureGroups.count(group)) architectureGroups.insert(std::pair<Ogre::String, std::vector<ArchitectureModel*> >(group,std::vector<ArchitectureModel*>()));
      if(type == MONSTERS && !monsterGroups.count(group)) monsterGroups.insert(std::pair<Ogre::String, std::vector<MonsterModel*> >(group,std::vector<MonsterModel*>()));
      if(type == ITEMS && !itemGroups.count(group)) itemGroups.insert(std::pair<Ogre::String, std::vector<ItemModel*> >(group,std::vector<ItemModel*>()));
      if(type == SCENES && !sceneGroups.count(group)) sceneGroups.insert(std::pair<Ogre::String, std::vector<SceneDesc*> >(group,std::vector<SceneDesc*>()));
      continue;
    }

    std::vector<std::string> words;
    boost::split(words, temp, boost::is_any_of(","), boost::token_compress_on);

    if(words.size() < 3) continue;
    int id = std::atoi(words[0].c_str());
    Ogre::String name = words[1];
    Ogre::String mesh = words[2];
    
    //TODO implement group pointers
    if(type == ARCHITECTURE)
    {
      ArchitectureModel model(name, mesh);
      architecture.insert(std::pair<int, ArchitectureModel>(id, model));
      architectureGroups.find(group)->second.push_back(&architecture.find(id)->second);//ugly
    }
    else if(type == MONSTERS)
    {
      MonsterModel model(name, mesh);
      monsters.insert(std::pair<int, MonsterModel>(id, model));
      monsterGroups.find(group)->second.push_back(&monsters.find(id)->second);//ugly
    }
    else if(type == ITEMS)
    {
      ItemModel model(name, mesh);
      items.insert(std::pair<int, ItemModel>(id, model));
      itemGroups.find(group)->second.push_back(&items.find(id)->second);//ugly
    }
    else if(type == SCENES)
    {
      SceneDesc desc(name, mesh);
      scenes.insert(std::pair<int, SceneDesc>(id, desc));
      sceneGroups.find(group)->second.push_back(&scenes.find(id)->second);
    }
  }
}

//-------------------------------------------------------------------------------------
ItemModel* DataManager::getItem(int id)
{
  if(items.count(id) == 0) return 0;
  return &((*(items.find(id))).second);
}

//-------------------------------------------------------------------------------------
MonsterModel* DataManager::getMonster(int id)
{
  if(monsters.count(id) == 0) return 0;
  return &((*(monsters.find(id))).second);
}

//-------------------------------------------------------------------------------------
ArchitectureModel* DataManager::getArchitecture(int id)
{
  if(architecture.count(id) == 0) return 0;
  return &((*(architecture.find(id))).second);
}

//-------------------------------------------------------------------------------------
SceneDesc* DataManager::getScene(int id)
{
  if(scenes.count(id) == 0) return 0;
  return &((*(scenes.find(id))).second);
}


