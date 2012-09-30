#include "Console.h"

#include "Scene.h"
#include "Player.h"
#include "World.h"


//-------------------------------------------------------------------------------------
Console::Console(World* world, OIS::Keyboard* keyboard)
  : overlay(new Gorilla::Silverback),
    keyboard(keyboard),
    isShift(false),
    isControl(false),
    world(world),
    window(NULL),
    screen(NULL),
    layer(NULL),
    view(NULL),
    text(""),
    command(""),
    history(),
    historyIndex(0)
{
  overlay->loadAtlas("dejavu");
}

//-------------------------------------------------------------------------------------
Console::~Console(void)
{
}

//-------------------------------------------------------------------------------------
void Console::hookWindow(Ogre::RenderWindow* window)
{
  assert(window);
  bool visibleTemp = isVisible();
  if(screen) overlay->destroyScreen(screen);
  this->window = window;
  screen = overlay->createScreen(window->getViewport(0), "dejavu");
  layer = screen->createLayer();
  view = layer->createMarkupText(9, 0, 0, "");

  update();
  setVisible(visibleTemp);
}

//-------------------------------------------------------------------------------------
void Console::setVisible(bool visible)
{
  if(layer) layer->setVisible(visible);
}

//-------------------------------------------------------------------------------------
bool Console::isVisible()
{
  if(layer) return layer->isVisible();
  return false;
}

//-------------------------------------------------------------------------------------
void Console::update(double elapsedSeconds)
{
  //if(!isVisible()) return;
}

//-------------------------------------------------------------------------------------
void Console::injectKeyDown(const OIS::KeyEvent &arg)
{
  if(!isVisible()) return;
  if(keyboard)
  {
    if(arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT) isShift = true;
    else if(arg.key == OIS::KC_LCONTROL || arg.key == OIS::KC_RCONTROL) isControl = true;
  }
}

//-------------------------------------------------------------------------------------
void Console::injectKeyUp(const OIS::KeyEvent &arg)
{
  if(!isVisible()) return;
  if(keyboard) 
  {
    if(arg.key == OIS::KC_SPACE) command += ' ';
    else if(arg.key == OIS::KC_GRAVE) return;
    else if(arg.key == OIS::KC_BACK) backspace();
    else if(arg.key == OIS::KC_RETURN) enter();
    else if(arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT) isShift = false;
    else if(arg.key == OIS::KC_LCONTROL || arg.key == OIS::KC_RCONTROL) isControl = false;
    else if(arg.key == OIS::KC_UP) up();
    else if(arg.key == OIS::KC_DOWN) down();
    else if(isShift) 
    {
      if(arg.key == OIS::KC_0) command += ')';
      else if(arg.key == OIS::KC_1) command += '!';
      else if(arg.key == OIS::KC_2) command += '@';
      else if(arg.key == OIS::KC_3) command += '#';
      else if(arg.key == OIS::KC_4) command += '$';
      else if(arg.key == OIS::KC_5) command += "%%";//used as escape sequence in gorilla markup
      else if(arg.key == OIS::KC_6) command += '^';
      else if(arg.key == OIS::KC_7) command += '&';
      else if(arg.key == OIS::KC_8) command += '*';
      else if(arg.key == OIS::KC_9) command += '(';

      else if(arg.key == OIS::KC_PERIOD) command += '>';
      else if(arg.key == OIS::KC_COMMA) command += '<';
      else if(arg.key == OIS::KC_SEMICOLON) command += ':';
      else if(arg.key == OIS::KC_APOSTROPHE) command += '"';
      else if(arg.key == OIS::KC_SLASH) command += '?';
      else if(arg.key == OIS::KC_MINUS) command += '_';
      else if(arg.key == OIS::KC_EQUALS) command += '+';
      else if(arg.key == OIS::KC_BACKSLASH) command += '|';

      else command += keyboard->getAsString(arg.key);
    }
    else 
    {
      std::string input = keyboard->getAsString(arg.key);
      input = std::tolower(input[0]);
      command += input;
    }
  }
  update();
}

//-------------------------------------------------------------------------------------
void Console::enter()
{
  historyIndex = 0;
  history.push_back(command);
  text += "\n> " + command;

  std::vector<std::string> elements;
  split(command, ' ', elements);
  
  if(elements.size() == 0) {}
  else if(elements.size() == 1)
  {
    if(elements[0] == "help") showHelp();
    else if(elements[0] == "about") showAbout();
    else if(elements[0] == "clear") clear();
    else if(elements[0] == "refresh") hookWindow(window);
    else if(elements[0] == "screenshot") screenshot();
    else if(elements[0] == "show_game_info") showGameInfo();
    else if(elements[0] == "set_physics_frozen") setPhysicsFrozen(true);
    else if(elements[0] == "set_physics_unfrozen") setPhysicsFrozen(false);
    else if(elements[0] == "set_free_camera") setFreeCamera(true);
    else if(elements[0] == "set_console_hidden") setVisible(false);
    else if(elements[0] == "show_player_position") showPlayerPosition();
    else if(elements[0] == "show_physics_info") showPhysicsInfo();
    else if(elements[0] == "show_data_files") showDataFiles();
    else if(elements[0] == "show_scene_info") showSceneInfo();
    else if(elements[0] == "show_world_info") showWorldInfo();
    else noCommand(command);
  }
  else if(elements.size() == 2)
  {
    if(elements[0] == "set_player_scene") setPlayerScene(elements[1]);
    else if(elements[0] == "load_scene") loadScene(elements[1]);
    else if(elements[0] == "set_player_gravity") setPlayerGravity(elements[1]);
    else if(elements[0] == "unload_scene") unloadScene(elements[1]);
    else if(elements[0] == "set_player_item_generation_id") setPlayerItemID(elements[1]);
    else noCommand(command);
  }
  else if(elements.size() == 3)
  {
    if(elements[0] == "set_fullscreen") setFullscreen(true, elements[1], elements[2]);
    if(elements[0] == "set_windowed") setFullscreen(false, elements[1], elements[2]);
    else if(elements[0] == "show_data") showData(elements[1], elements[2]);
    else noCommand(command);
  }
  else if(elements.size() == 4)
  {
    if(elements[0] == "set_player_position") setPlayerPosition(elements[1], elements[2], elements[3]);
    else noCommand(command);
  }
  else if(elements.size() == 5)
  {
    if(elements[0] == "set_ambient_light") setAmbientLight(elements[1], elements[2], elements[3], elements[4]);
    else noCommand(command);
  }
  else if(elements.size() == 6)
  {
    if(elements[0] == "add") add(elements[1], elements[2], elements[3], elements[4], elements[5]);
    else noCommand(command);
  }
  else noCommand(command);
  
  command = "";
}

//-------------------------------------------------------------------------------------
void Console::display(std::string comment)
{
  text += "\n" + comment;
}

//-------------------------------------------------------------------------------------
void Console::display(std::string highlight, std::string comment)
{
  text += "\n%5" + highlight + "%r - " + comment;
}

//-------------------------------------------------------------------------------------
void Console::error(std::string comment)
{
  text += "\n%4error%r - " + comment;
}

//-------------------------------------------------------------------------------------
void Console::noCommand(std::string command)
{
  error("there is no command '" + command + "'");
}

//-------------------------------------------------------------------------------------
void Console::split(const std::string &s, char delim, std::vector<std::string> &elems) 
{
  std::stringstream ss(s);
  std::string item;
  while(std::getline(ss, item, delim)) elems.push_back(item);
}

//-------------------------------------------------------------------------------------
void Console::update()
{
  if(view) view->text(text + "\n%3> " + command + "_");
}

//-------------------------------------------------------------------------------------
void Console::backspace()
{
  if(command.size() == 0) return;
  command = command.substr(0, command.size() - 1);
}


//-------------------------------------------------------------------------------------
void Console::up()
{
  if(historyIndex < history.size()) historyIndex++;
  if(historyIndex > 0) command = history[history.size() - historyIndex];
  else command = "";
}

//-------------------------------------------------------------------------------------
void Console::down()
{
  if(historyIndex > 0) historyIndex--;
  if(historyIndex > 0) command = history[history.size() - historyIndex];
  else command = "";
}

//-------------------------------------------------------------------------------------
void Console::clear()
{
  text = "";
  command = "";
}

//-------------------------------------------------------------------------------------
void Console::showGameInfo()
{
  display("average fps", Ogre::StringConverter::toString(int(window->getAverageFPS())));
  display("best fps", Ogre::StringConverter::toString(window->getBestFPS()));
  display("batches", Ogre::StringConverter::toString(window->getBatchCount()));
  display("colour depth", Ogre::StringConverter::toString(window->getColourDepth()));
  display("number of viewports", Ogre::StringConverter::toString(window->getNumViewports()));
  display("triangle count", Ogre::StringConverter::toString(window->getTriangleCount()));
  display("window size", Ogre::StringConverter::toString(window->getWidth()) + "x" + Ogre::StringConverter::toString(window->getHeight()));
  display("number of loaded scenes", Ogre::StringConverter::toString(world->getNumberScenes()));

  std::vector<Ogre::String> names;
  world->getSceneNames(names);
  Ogre::String list = "";
  for(std::vector<std::string>::iterator it = names.begin(); it < names.end(); ++it) list += (*it) + " ";
  display("loaded scenes", list);
}

//-------------------------------------------------------------------------------------
void Console::showHelp()
{
  display("help", "shows this list");
  display("clear", "clears the console");
  display("refresh", "temp method to rehook console to viewport");
  display("screenshot", "takes a screenshot and outputs to timestamped file");
  display("set_physics_frozen", "freezes physics");
  display("set_physics_unfrozen", "unfreezes physics");
  display("set_free_camera", "frees the camera");
  display("set_console_hidden", "hides the console");
  display("set_fullscreen [width] [height]", "sets the window to fullscreen mode");
  display("set_windowed [width] [height]", "sets the window to windowed mode");
  display("set_player_scene [scene name]", "moves the player to the specified scene");
  display("set_ambient_light [r] [g] [b]", "sets the ambient light in the given scene");
  display("set_player_position [x] [y] [z]", "sets the player position");
  display("set_player_item_generation_id [id]", "sets the id of the item to generate");
  display("show_data [item|monster|architecture] [id]", "displays the data associated with the id of the given type of entity");
  display("show_data_files", "lists the loaded data files");
  display("show_physics_info", "shows physx physics stats for the current scene");
  display("show_player_position", "gets the player position");
  display("show_game_info", "shows a list of statistics and a list of loaded scenes");
  display("show_scene_info","shows scene statistics");
  display("show_world_info", "shows world statistics");
  display("add [item|monster] [data id] [x] [y] [z]", "adds the given type of entity to the current scene at the given coordinates");
  display("load_scene [scene id]", "loads a scene into memory");
  display("unload_scene [scene name]", "unloads a scene from memory");
}

//-------------------------------------------------------------------------------------
void Console::setPhysicsFrozen(bool isFrozen)
{
  world->freezeCollisionDebug = isFrozen;
}

//-------------------------------------------------------------------------------------
void Console::setFreeCamera(bool isFree)
{
  world->getPlayer()->setCollisionEnabled(!isFree);//freeCameraDebug = !world->freeCameraDebug;
}

//-------------------------------------------------------------------------------------
void Console::setPlayerPosition(std::string x, std::string y, std::string z)
{
  Ogre::Vector3 position = Ogre::Vector3(Ogre::Real(boost::lexical_cast<float>(x)), Ogre::Real(boost::lexical_cast<float>(y)), Ogre::Real(boost::lexical_cast<float>(z)));
  world->getPlayer()->setPosition(position);
}

//-------------------------------------------------------------------------------------
void Console::setPlayerScene(std::string sceneName)
{
  Scene* target = world->getScene(sceneName);
  if(target) world->movePlayer(world->getPlayer(), target);
  else error("no scene loaded named '" + sceneName + "'");
}

//-------------------------------------------------------------------------------------
void Console::setPlayerGravity(std::string gravity)
{
  world->getPlayer()->setGravity(boost::lexical_cast<float>(gravity));
}

//-------------------------------------------------------------------------------------
void Console::setFullscreen(bool isFullscreen, std::string width, std::string height)
{
  int widthNum = boost::lexical_cast<int>(width);
  int heightNum = boost::lexical_cast<int>(height);

  if(widthNum <= 0 || heightNum <= 0) error("bad screen dimensions");
  else window->setFullscreen(isFullscreen, widthNum, heightNum);
}

//-------------------------------------------------------------------------------------
void Console::setAmbientLight(std::string r, std::string g, std::string b, std::string a)
{
  Scene* target = world->getPlayer()->getScene();
  if(target) target->getSceneManager()->setAmbientLight(Ogre::ColourValue(boost::lexical_cast<float>(r), boost::lexical_cast<float>(g), boost::lexical_cast<float>(b), boost::lexical_cast<float>(a)));
  else error("player needs to be located within a scene to be able to change the ambient light");
}

//-------------------------------------------------------------------------------------
void Console::setPlayerItemID(std::string id)
{
  world->getPlayer()->setItemGenerationID(boost::lexical_cast<int>(id));
}

//-------------------------------------------------------------------------------------
void Console::showPlayerPosition()
{
  Ogre::Vector3 position = world->getPlayer()->getPosition();
  display("x", boost::lexical_cast<std::string>(position.x));
  display("y", boost::lexical_cast<std::string>(position.y));
  display("z", boost::lexical_cast<std::string>(position.z));
}

//-------------------------------------------------------------------------------------
void Console::showPhysicsInfo()
{
  Scene* target = world->getPlayer()->getScene();
  if(target) 
  {
    physx::PxScene* physics = target->getPhysicsManager();
    if(physics)
    {
      display("gravity", boost::lexical_cast<std::string>(physics->getGravity().magnitude()));
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
    else error("no physics associated with this scene");
  }
  else error("player needs to be located within a scene to be able to query the physics");
}

//-------------------------------------------------------------------------------------
void Console::showSceneInfo()
{
  display("name", world->getPlayer()->getScene()->getName());
  display("ogre internal name", world->getPlayer()->getScene()->getSceneManager()->getName());
}

//-------------------------------------------------------------------------------------
void Console::showWorldInfo()
{
  
}

//-------------------------------------------------------------------------------------
void Console::showData(std::string type, std::string id)
{
  try
  {
    int idNumber = boost::lexical_cast<int>(id);
    if(type == "architecture")
    {
      ArchitectureDesc desc = world->getDataManager()->getArchitecture(idNumber);
      display("name", desc.name);
      display("mesh", desc.mesh);
    }
    else if(type == "item")
    {
      ItemDesc desc = world->getDataManager()->getItem(idNumber);
      display("name", desc.name);
      display("mesh", desc.mesh);
    }
    else if(type == "monster")
    {
      MonsterDesc desc = world->getDataManager()->getMonster(idNumber);
      display("name", desc.name);
      display("mesh", desc.mesh);
    }
  }
  catch(NHException e)
  {
    error(e.what());
  }
}

//-------------------------------------------------------------------------------------
void Console::showDataFiles()
{
  std::vector<std::string> files = world->getDataManager()->getLoadedDataFiles();

  for(std::vector<std::string>::iterator iter = files.begin(); iter < files.end(); ++iter) display(*iter);
}

//-------------------------------------------------------------------------------------
void Console::showAbout()
{
  display("Copyright Arrian Purcell 2012\n");
}

//-------------------------------------------------------------------------------------
void Console::add(std::string type, std::string id, std::string x, std::string y, std::string z)
{
  try
  {
    Scene* target = world->getPlayer()->getScene();
    if(target)
    {
      Ogre::Vector3 position = Ogre::Vector3(Ogre::Real(boost::lexical_cast<float>(x)), Ogre::Real(boost::lexical_cast<float>(y)), Ogre::Real(boost::lexical_cast<float>(z)));
      int idNum = boost::lexical_cast<int>(id);
      if(type == "item") target->addItem(idNum, position);
      else if(type == "monster") target->addMonster(idNum, position);
      else error("object type '" + type + "' not implemented");
    }
    else error("player needs to be located within a scene to add an item to it");
  }
  catch(NHException e)
  {
    error(e.what());
  }
}

//-------------------------------------------------------------------------------------
void Console::loadScene(std::string sceneId)
{
  try
  {
    if(world->loadScene(boost::lexical_cast<int>(sceneId))) display("scene loaded");
    else display("could not load the scene");
  }
  catch(NHException e)
  {
    error(e.what());
  }
}

//-------------------------------------------------------------------------------------
void Console::unloadScene(std::string sceneName)
{
  try
  {
    if(world->destroyScene(sceneName)) display("scene unloaded");
    else error("could not unload the scene");
  }
  catch(NHException e)
  {
    error(e.what());
  }
}

//-------------------------------------------------------------------------------------
void Console::screenshot()
{
  setVisible(false);
  display("Screenshot saved to '" + window->writeContentsToTimestampedFile("screenshot", ".jpg") + "'.");
  setVisible(true);
}


