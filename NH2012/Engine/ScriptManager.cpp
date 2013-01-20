#include "ScriptManager.h"

#include "Console.h"
#include "World.h"
#include "Player.h"
#include "Scene.h"


ScriptManager::ScriptManager(void)
  : world(NULL),
    done(false),
    outputs()
{
  addCommand("help", "", "shows the help list", &ScriptManager::help);
  //addCommand("clear", "", "clears the console", &ScriptManager::clear);
  //addCommand("refresh", "", "re-hooks the console to the render window", &ScriptManager::refresh);
  addCommand("about", "", "show game info", &ScriptManager::about);
  addCommand("reset", "", "resets the scene", &ScriptManager::reset);
  addCommand("exit", "", "exits the game", &ScriptManager::exit);
  addCommand("screenshot", "", "takes a screenshot and outputs to the executable directory", &ScriptManager::screenshot);
  addCommand("setPhysicsEnabled", "(true | false)", "enables and disables collision", &ScriptManager::setPhysicsEnabled);
  addCommand("setCameraFree", "(true | false)", "frees or attaches the camera from/to the player", &ScriptManager::setCameraFree);
  //addCommand("setConsoleVisible", "(true | false)", "shows or hides the console", &ScriptManager::setConsoleVisible);
  addCommand("setFullscreen", "width height", "sets the display to fullscreen", &ScriptManager::setFullscreen);
  addCommand("setWindowed", "width height", "set the display to windowed", &ScriptManager::setWindowed);
  addCommand("setPlayerScene", "id", "sets the player scene", &ScriptManager::setPlayerScene);
  addCommand("setSceneAmbientLight", "red green blue alpha", "sets the ambient light of the current scene", &ScriptManager::setSceneAmbientLight);
  addCommand("setPlayerPosition", "x y z", "sets the player position", &ScriptManager::setPlayerPosition);
  addCommand("setPlayerItemGenerationID", "id", "sets the id of the item generated by the player", &ScriptManager::setPlayerItemGenerationID);
  addCommand("getItemData", "id", "gets the item associated with the given id", &ScriptManager::getItemData);
  addCommand("getMonsterData", "id", "gets the monster associated with the given id", &ScriptManager::getMonsterData);
  addCommand("getArchitectureData", "id", "gets the architecture associated with the given id", &ScriptManager::getArchitectureData);
  addCommand("getSoundData", "id", "gets the sound associated with the given id", &ScriptManager::getSoundData);
  addCommand("getSceneData", "id", "gets the scene associated with the given id", &ScriptManager::getSceneData);
  addCommand("getDataFiles", "", "gets a list of the active data files", &ScriptManager::getDataFiles);
  addCommand("getPhysicsInfo", "", "gets the current state of the physics", &ScriptManager::getPhysicsInfo);
  addCommand("getPlayerPosition", "", "gets the player position", &ScriptManager::getPlayerPosition);
  addCommand("getGameInfo", "", "gets the current state of the game", &ScriptManager::getGameInfo);
  addCommand("getSceneInfo", "", "gets the current state of the scene", &ScriptManager::getSceneInfo);
  addCommand("getWorldInfo", "", "gets the current state of the world", &ScriptManager::getWorldInfo);
  addCommand("addItem", "id (@player | (x y z)) [number]", "adds the given item to the scene", &ScriptManager::addItem);
  addCommand("addMonster", "id x y z", "adds the given monster to the scene", &ScriptManager::addMonster);
  addCommand("setSceneLoaded", "id (true | false)", "loads or unloads the given scene", &ScriptManager::setSceneLoaded);
  addCommand("setSceneDrawDebugNavMesh", "(true | false)", "displays or hides the scene navigation mesh", &ScriptManager::setSceneDrawDebugNavMesh);
  addCommand("setSceneShadowsEnabled", "(true | false)", "shows or hides shadows", &ScriptManager::setSceneShadowsEnabled);
  addCommand("setSceneGravity", "x y z", "sets the scene gravity", &ScriptManager::setSceneGravity);
}

//-------------------------------------------------------------------------------------
ScriptManager::~ScriptManager(void)
{
  for(std::map<std::string, Command<ScriptManager>* >::iterator iter = commands.begin(); iter != commands.end(); ++iter)
  {
    delete iter->second;
  }
}

//-------------------------------------------------------------------------------------
void ScriptManager::addCommand(std::string name, std::string arguments, std::string help, void (ScriptManager::*run) (Options))
{
  commands.insert(std::pair<std::string, Command<ScriptManager>* >(name, new Command<ScriptManager>(arguments, help, run, this)));
}

//-------------------------------------------------------------------------------------
bool ScriptManager::update(double elapsedSeconds)
{
  return !done;
}

//-------------------------------------------------------------------------------------
void ScriptManager::execute(std::string command)
{
  std::vector<std::string> commandSet;
  split(command, ';', commandSet);

  if(commandSet.size() == 0) commandSet.push_back(command);//we were only given one command

  for(std::vector<std::string>::iterator commandIter = commandSet.begin(); commandIter < commandSet.end(); ++commandIter)
  {
    Options elements;
    split(*commandIter, ' ', elements);

    if(elements.size() == 0) elements.push_back(*commandIter);//must be a one word command

    if(commands.count(elements[0]) < 1) throw NHException("command not found");

    try
    {
      commands.find(elements[0])->second->run(elements);
    }
    catch(boost::bad_lexical_cast e)//could not interpret the arguments given
    {
      throw NHException("bad arguments given");
    }
  }
}

//-------------------------------------------------------------------------------------
void ScriptManager::run(std::string file)
{
  Ogre::DataStreamPtr ptr = Ogre::ResourceGroupManager::getSingletonPtr()->openResource(file);
  if(ptr.isNull()) throw NHException("script not found");
  execute(ptr->getAsString());
}

//-------------------------------------------------------------------------------------
void ScriptManager::split(const std::string &s, char delim, std::vector<std::string> &elems) 
{
  std::stringstream ss(s);
  std::string item;
  while(std::getline(ss, item, delim)) elems.push_back(item);
}

//-------------------------------------------------------------------------------------
void ScriptManager::setWorld(World* world)
{
  this->world = world;
}

//-------------------------------------------------------------------------------------
bool ScriptManager::stringToBool(std::string string)
{
  return (string == "1" || boost::algorithm::to_lower_copy(string) == "true");
}

/*
//-------------------------------------------------------------------------------------
void ScriptManager::clear(Options argv)
{
  Console::getInstance().clear();
}
*/

/*
//-------------------------------------------------------------------------------------
void ScriptManager::refresh(Options argv)
{
  Console::getInstance().rehookWindow();
}
*/

//-------------------------------------------------------------------------------------
void ScriptManager::about(Options argv)
{
  display("Copyright Arrian Purcell 2012.");
}

//-------------------------------------------------------------------------------------
void ScriptManager::exit(Options argv)
{
  done = true;
}

//-------------------------------------------------------------------------------------
void ScriptManager::help(Options argv)
{
  if(argv.size() > 1)//only show specified command
  {
    for(int i = 1; i < argv.size(); i++)
    {
      if(commands.count(argv[i]) > 0) display(argv[i], commands.find(argv[i])->second->help);
      else throw NHException("command not found");
    }
  }
  else//show all help
  {
    for(std::map<std::string, Command<ScriptManager>* >::iterator iter = commands.begin(); iter != commands.end(); ++iter)
    {
      display(iter->first + " " + iter->second->args, iter->second->help);
    }
  }
}

//-------------------------------------------------------------------------------------
void ScriptManager::screenshot(Options argv)
{
  //Console::getInstance().setVisible(false);
  if(world->getPlayer()->getWindow())display("Screenshot saved to '" + world->getPlayer()->getWindow()->writeContentsToTimestampedFile("screenshot", ".png") + "'.");
  //Console::getInstance().setVisible(true);
}

//-------------------------------------------------------------------------------------
void ScriptManager::setPhysicsEnabled(Options argv)
{
  if(argv.size() < 2) throw NHException("too few arguments");
  world->setPhysicsEnabled(stringToBool(argv[1]));
}

//-------------------------------------------------------------------------------------
void ScriptManager::setCameraFree(Options argv)
{
  if(argv.size() < 2) throw NHException("too few arguments");
  world->getPlayer()->setFreeCamera(stringToBool(argv[1]));
}

/*
//-------------------------------------------------------------------------------------
void ScriptManager::setConsoleVisible(Options argv)
{
  if(argv.size() < 2) throw NHException("too few arguments");
  bool visible = stringToBool(argv[1]);
  Console::getInstance().setVisible(!visible);
}*/

//-------------------------------------------------------------------------------------
void ScriptManager::setFullscreen(Options argv)
{
  if(argv.size() < 3) throw NHException("too few arguments");
  int widthNum = boost::lexical_cast<int>(argv[1]);
  int heightNum = boost::lexical_cast<int>(argv[2]);

  if(widthNum <= 0 || heightNum <= 0) throw NHException("bad screen dimensions");
  else if(world->getPlayer()->getWindow()) world->getPlayer()->getWindow()->setFullscreen(true, widthNum, heightNum);
  else throw NHException("no window to set to fullscreen mode");
}

//-------------------------------------------------------------------------------------
void ScriptManager::setWindowed(Options argv)
{
  if(argv.size() < 3) throw NHException("too few arguments");
  int widthNum = boost::lexical_cast<int>(argv[1]);
  int heightNum = boost::lexical_cast<int>(argv[2]);

  if(widthNum <= 0 || heightNum <= 0) throw NHException("bad screen dimensions");
  else if(world->getPlayer()->getWindow()) world->getPlayer()->getWindow()->setFullscreen(false, widthNum, heightNum);
  else throw NHException("no window to set to windowed mode");
}

//-------------------------------------------------------------------------------------
void ScriptManager::setPlayerScene(Options argv)
{
  if(argv.size() < 2) throw NHException("too few arguments");
  Scene* target = world->getScene(boost::lexical_cast<int>(argv[1]));
  if(target) target->addPlayer(world->getPlayer());
  else throw NHException("no scene loaded with the given id");
}

//-------------------------------------------------------------------------------------
void ScriptManager::setSceneAmbientLight(Options argv)
{
  if(argv.size() < 5) throw NHException("too few arguments");
  Scene* target = world->getPlayer()->getScene();
  if(target) target->setAmbientColour(Ogre::ColourValue(boost::lexical_cast<float>(argv[1]), 
    boost::lexical_cast<float>(argv[2]), 
    boost::lexical_cast<float>(argv[3]), 
    boost::lexical_cast<float>(argv[4])));
  else throw NHException("player needs to be located within a scene");
}

//-------------------------------------------------------------------------------------
void ScriptManager::setPlayerPosition(Options argv)
{
  if(argv.size() < 4) throw NHException("too few arguments");
  Vector3 position = Vector3(Ogre::Real(boost::lexical_cast<float>(argv[1])), 
  Ogre::Real(boost::lexical_cast<float>(argv[2])), 
  Ogre::Real(boost::lexical_cast<float>(argv[3])));
  world->getPlayer()->setPosition(position);
}

//-------------------------------------------------------------------------------------
void ScriptManager::setPlayerItemGenerationID(Options argv)
{
  if(argv.size() < 2) throw NHException("too few arguments");
  world->getPlayer()->setItemGenerationID(boost::lexical_cast<int>(argv[1]));
}

//-------------------------------------------------------------------------------------
void ScriptManager::getItemData(Options argv)
{
  if(argv.size() < 2) throw NHException("too few arguments");
  ItemDesc desc = world->getDataManager()->getItem(boost::lexical_cast<int>(argv[1]));
  display("name", desc.name);
  display("mesh", desc.mesh);
  display("simplified mesh", desc.simplifiedMesh);
  display("dynamic friction", boost::lexical_cast<std::string>(desc.dynamicFriction));
  display("static friction", boost::lexical_cast<std::string>(desc.staticFriction));
  display("restitution", boost::lexical_cast<std::string>(desc.restitution));
  display("density", boost::lexical_cast<std::string>(desc.density));
}

//-------------------------------------------------------------------------------------
void ScriptManager::getMonsterData(Options argv)
{
  if(argv.size() < 2) throw NHException("too few arguments");
  MonsterDesc desc = world->getDataManager()->getMonster(boost::lexical_cast<int>(argv[1]));
  display("name", desc.name);
  display("mesh", desc.mesh);
  display("health", boost::lexical_cast<std::string>(desc.health));
  display("height", boost::lexical_cast<std::string>(desc.height));
  display("speed", boost::lexical_cast<std::string>(desc.speed));

}

//-------------------------------------------------------------------------------------
void ScriptManager::getArchitectureData(Options argv)
{
  if(argv.size() < 2) throw NHException("too few arguments");
  ArchitectureDesc desc = world->getDataManager()->getArchitecture(boost::lexical_cast<int>(argv[1]));
  display("name", desc.name);
  display("mesh", desc.mesh);
  display("friction", boost::lexical_cast<std::string>(desc.friction));
  display("restitution", boost::lexical_cast<std::string>(desc.restitution));

}
//-------------------------------------------------------------------------------------
void ScriptManager::getSoundData(Options argv)
{
  if(argv.size() < 2) throw NHException("too few arguments");
  SoundDesc desc = world->getDataManager()->getSound(boost::lexical_cast<int>(argv[1]));
  display("name", desc.name);
  display("file", desc.file);
}

//-------------------------------------------------------------------------------------
void ScriptManager::getSceneData(Options argv)
{
  if(argv.size() < 2) throw NHException("too few arguments");
  SceneDesc desc = world->getDataManager()->getScene(boost::lexical_cast<int>(argv[1]));
  display("name", desc.name);
  display("file", desc.file);
  //add other attributes here
}

//-------------------------------------------------------------------------------------
void ScriptManager::getDataFiles(Options argv)
{
  std::vector<std::string> files = world->getDataManager()->getLoadedDataFiles();

  for(std::vector<std::string>::iterator iter = files.begin(); iter < files.end(); ++iter) display(*iter);
}

//-------------------------------------------------------------------------------------
void ScriptManager::getPhysicsInfo(Options argv)
{
  Scene* target = world->getPlayer()->getScene();
  if(target) 
  {
    physx::PxScene* physics = target->getPhysicsManager();
    if(physics)
    {
      display("gravity", boost::lexical_cast<std::string>(target->getGravity().length()));
      display("current timestamp", boost::lexical_cast<std::string>(physics->getTimestamp()));
      display("number of rigid static actors", boost::lexical_cast<std::string>(physics->getNbActors(physx::PxActorTypeSelectionFlags(physx::PxActorTypeSelectionFlag::eRIGID_STATIC))));
      display("number of rigid dynamic actors", boost::lexical_cast<std::string>(physics->getNbActors(physx::PxActorTypeSelectionFlags(physx::PxActorTypeSelectionFlag::eRIGID_DYNAMIC))));
      display("number of cloth actors", boost::lexical_cast<std::string>(physics->getNbActors(physx::PxActorTypeSelectionFlags(physx::PxActorTypeSelectionFlag::eCLOTH))));
      display("number of particle fluid actors", boost::lexical_cast<std::string>(physics->getNbActors(physx::PxActorTypeSelectionFlags(physx::PxActorTypeSelectionFlag::ePARTICLE_FLUID))));
      display("number of particle system actors", boost::lexical_cast<std::string>(physics->getNbActors(physx::PxActorTypeSelectionFlags(physx::PxActorTypeSelectionFlag::ePARTICLE_SYSTEM))));
      display("number of aggregates", boost::lexical_cast<std::string>(physics->getNbAggregates()));
      display("number of articulations", boost::lexical_cast<std::string>(physics->getNbArticulations()));
      display("number of constraints", boost::lexical_cast<std::string>(physics->getNbConstraints()));
    }
    else throw NHException("no physics associated with this scene");
  }
  else throw NHException("player needs to be located within a scene to be able to query the physics");
}

//-------------------------------------------------------------------------------------
void ScriptManager::getGameInfo(Options argv)
{
  Ogre::RenderWindow* window = world->getPlayer()->getWindow();
  if(!window) throw NHException("no render window found");
  display("average fps", boost::lexical_cast<std::string>(int(window->getAverageFPS())));
  display("best fps", boost::lexical_cast<std::string>(window->getBestFPS()));
  display("batches", boost::lexical_cast<std::string>(window->getBatchCount()));
  display("colour depth", boost::lexical_cast<std::string>(window->getColourDepth()));
  display("number of viewports", boost::lexical_cast<std::string>(window->getNumViewports()));
  display("triangle count", boost::lexical_cast<std::string>(window->getTriangleCount()));
  display("window size", boost::lexical_cast<std::string>(window->getWidth()) + "x" + Ogre::StringConverter::toString(window->getHeight()));
  display("number of loaded scenes", boost::lexical_cast<std::string>(world->getNumberScenes()));

  std::map<int, std::string> names;
  world->getSceneNames(names);
  std::string list = "";
  for(std::map<int, std::string>::iterator it = names.begin(); it != names.end(); ++it) list += boost::lexical_cast<std::string>(it->first) + ": \"" + it->second + "\" ";
  display("loaded scenes", list);
}

//-------------------------------------------------------------------------------------
void ScriptManager::getSceneInfo(Options argv)
{
  display("name", world->getPlayer()->getScene()->getName());
  display("ogre internal name", world->getPlayer()->getScene()->getGraphicsManager()->getName());
}

//-------------------------------------------------------------------------------------
void ScriptManager::getWorldInfo(Options argv)
{

}

//-------------------------------------------------------------------------------------
void ScriptManager::getPlayerPosition(Options argv)
{
  Vector3 position = world->getPlayer()->getPosition();
  display("x,y,z: ", "(" + boost::lexical_cast<std::string>(position.x) + "," 
    + boost::lexical_cast<std::string>(position.y) + "," 
    + boost::lexical_cast<std::string>(position.z) + ")");
}

//-------------------------------------------------------------------------------------
void ScriptManager::addItem(Options argv)
{
  if(argv.size() < 3) throw NHException("too few arguments");
  Scene* target = world->getPlayer()->getScene();
  if(target)
  {
    int numberToAdd = 1;
    if(argv.size() > 5) numberToAdd = boost::lexical_cast<int>(argv[5]);

    Vector3 position;
    if(argv.size() > 1 && argv[2] == "@player")//the user specified to add the item at the current location
    {
      position = world->getPlayer()->getPosition();
      if(argv.size() > 2) numberToAdd = boost::lexical_cast<int>(argv[3]);
    }
    else position = Vector3(Ogre::Real(boost::lexical_cast<float>(argv[2])), 
      Ogre::Real(boost::lexical_cast<float>(argv[3])), 
      Ogre::Real(boost::lexical_cast<float>(argv[4])));
    int idNum = boost::lexical_cast<int>(argv[1]);
    for(int i = 0; i < numberToAdd; i++) target->addItem(idNum, position);
  }
  else throw NHException("player needs to be located within a scene to add an item to it");
}
//-------------------------------------------------------------------------------------
void ScriptManager::addMonster(Options argv)
{
  Scene* target = world->getPlayer()->getScene();
  if(target)
  {
    Vector3 position = Vector3(Ogre::Real(boost::lexical_cast<float>(argv[2])), 
                                           Ogre::Real(boost::lexical_cast<float>(argv[3])), 
                                           Ogre::Real(boost::lexical_cast<float>(argv[4])));
    int idNum = boost::lexical_cast<int>(argv[1]);
    target->addMonster(idNum, position);
  }
  else throw NHException("player needs to be located within a scene to add a monster to it");
}

//-------------------------------------------------------------------------------------
void ScriptManager::setSceneLoaded(Options argv)
{
  if(argv.size() < 3) throw NHException("too few arguments");
  if(stringToBool(argv[2]))
  {
    if(world->loadScene(boost::lexical_cast<int>(argv[1]))) display("scene loaded");
    else display("could not load the scene");
  }
  else
  {
    world->destroyScene(boost::lexical_cast<int>(argv[1]));
    display("scene unloaded");
  }
}

//-------------------------------------------------------------------------------------
void ScriptManager::setSceneDrawDebugNavMesh(Options argv)
{
  if(argv.size() < 2) throw NHException("too few arguments");
  world->getPlayer()->getScene()->setDebugDrawNavigationMesh(stringToBool(argv[1]));
}

//-------------------------------------------------------------------------------------
void ScriptManager::setSceneShadowsEnabled(Options argv)
{
  if(argv.size() < 2) throw NHException("too few arguments");
  world->getPlayer()->getScene()->setShadowsEnabled(stringToBool(argv[1]));
}

//-------------------------------------------------------------------------------------
void ScriptManager::setSceneGravity(Options argv)
{
  if(argv.size() < 4) throw NHException("too few arguments");
  world->getPlayer()->getScene()->setGravity(Vector3(boost::lexical_cast<float>(argv[1]), boost::lexical_cast<float>(argv[2]), boost::lexical_cast<float>(argv[3])));
}

//-------------------------------------------------------------------------------------
void ScriptManager::reset(Options argv)
{
  if(!world) throw NHException("no world found");
  if(!world->getPlayer()) throw NHException("no player found");
  if(!world->getPlayer()->getScene()) throw NHException("no scene found");
  world->getPlayer()->getScene()->reset();
}

//-------------------------------------------------------------------------------------
void ScriptManager::addOutputTarget(OutputCallback* target)
{
  outputs.push_back(target);
}

//-------------------------------------------------------------------------------------
void ScriptManager::display(std::string comment)
{
  for(std::vector<OutputCallback*>::iterator iter = outputs.begin(); iter != outputs.end(); ++iter)
  {
    (*iter)->print(comment);
  }

  //if(outputStandardOut) std::cout << comment << std::endl;
  //if(outputGameConsole) Console::getInstance().print(comment);
}

//-------------------------------------------------------------------------------------
void ScriptManager::display(std::string highlight, std::string comment)
{
  for(std::vector<OutputCallback*>::iterator iter = outputs.begin(); iter != outputs.end(); ++iter)
  {
    (*iter)->print(highlight, comment);
  }

  //if(outputStandardOut) std::cout << highlight << " - " << comment << std::endl;
  //if(outputGameConsole) Console::getInstance().print(highlight, comment);
}


