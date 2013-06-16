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
  : files(std::vector<std::string>())
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
    
    int id = 0;
    try
    {
      id = boost::lexical_cast<int>(words[ID_INDEX]);
      if(id == 0)
      {
        std::cout << "Could not find the id of the data entry in " << file << " on line " << line << "." << std::endl;
        continue;
      }
    }
    catch(boost::bad_lexical_cast ex)
    {
      //we can, in general, safely ignore any badly formatted data entries but we should warn the user
      std::cout << "Incorrectly formatted data entry in " << file << " on line " << line << "." << std::endl;
      continue;
    }

    std::string name;
    std::string file;
    
    if(words.size() >= 2) name = words[NAME_INDEX];
    else name = "Name Error";
    
    if(words.size() >= 3) file = words[MESH_INDEX];
    else file = "error.mesh";

    //Need to extend for other data options
    if(type == ARCHITECTURE_IDENTIFIER) architecture.insert(Id<ArchitectureDesc>(id), std::shared_ptr<ArchitectureDesc>(new ArchitectureDesc(id, name, file)));
    else if(type == CREATURES_IDENTIFIER) creatures.insert(Id<CreatureDesc>(id), std::shared_ptr<CreatureDesc>(new CreatureDesc(id, name, file)));
    else if(type == ITEMS_IDENTIFIER) items.insert(Id<ItemDesc>(id), std::shared_ptr<ItemDesc>(new ItemDesc(id, name, file, file)));
    else if(type == SCENES_IDENTIFIER) scenes.insert(Id<SceneDesc>(id), std::shared_ptr<SceneDesc>(new SceneDesc(id, name, file)));
    else if(type == SOUNDS_IDENTIFIER) sounds.insert(Id<SoundDesc>(id), std::shared_ptr<SoundDesc>(new SoundDesc(id, name, file)));
  }
}

//-------------------------------------------------------------------------------------
std::vector<std::string> DataManager::getLoadedDataFiles()
{
  return files;
}


