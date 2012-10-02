#include "Console.h"

#include "Scene.h"
#include "Player.h"
#include "World.h"


//-------------------------------------------------------------------------------------
Console::Console()//World* world, OIS::Keyboard* keyboard)
  : overlay(new Gorilla::Silverback),
    //keyboard(keyboard),
    isShift(false),
    isControl(false),
    //world(world),
    window(NULL),
    screen(NULL),
    layer(NULL),
    view(NULL),
    //text(""),
    command(""),
    history(),
    lines(),
    displayLines(27),
    displayOffset(0),
    historyIndex(0),
    firstKeyPressHoldWait(1.0),
    consecutiveKeyPressHoldWait(0.1),
    holdAccumulator(0.0),
    cursorFlashRate(1.0),
    cursorAccumulator(0.0),
    showCursor(true),
    hasDoneFirstHold(false),
    previousKey(OIS::KC_UNASSIGNED),
    keyIsDown(false)

{
  overlay->loadAtlas("dejavu");

  commands.push_back(new Command<Console>("clear", "clears the console", &Console::clear, this));
  commands.push_back(new Command<Console>("refresh", "re-hooks the console to the render window", &Console::refresh, this));
  commands.push_back(new Command<Console>("about", "show game info", &Console::about, this));
  commands.push_back(new Command<Console>("help", "shows the help list", &Console::help, this));
  commands.push_back(new Command<Console>("screenshot", "takes a screenshot and outputs to the executable directory", &Console::screenshot, this));
  commands.push_back(new Command<Console>("setPhysicsEnabled", "enables and disables collision", &Console::setPhysicsEnabled, this));
  commands.push_back(new Command<Console>("setCameraFree", "frees or attaches the camera from/to the player", &Console::setCameraFree, this));
  commands.push_back(new Command<Console>("setConsoleVisible", "shows or hides the console", &Console::setConsoleVisible, this));
  commands.push_back(new Command<Console>("setFullscreen", "sets the display to fullscreen", &Console::setFullscreen, this));
  commands.push_back(new Command<Console>("setWindowed", "set the display to windowed", &Console::setWindowed, this));
  commands.push_back(new Command<Console>("setPlayerScene", "sets the player scene", &Console::setPlayerScene, this));
  commands.push_back(new Command<Console>("setSceneAmbientLight", "sets the ambient light of the current scene", &Console::setSceneAmbientLight, this));
  commands.push_back(new Command<Console>("setPlayerPosition", "sets the player position", &Console::setPlayerPosition, this));
  commands.push_back(new Command<Console>("setPlayerItemGenerationID", "sets the id of the item generated by the player", &Console::setPlayerItemGenerationID, this));
  commands.push_back(new Command<Console>("getItemData", "gets the item associated with the given id", &Console::getItemData, this));
  commands.push_back(new Command<Console>("getMonsterData", "gets the monster associated with the given id", &Console::getMonsterData, this));
  commands.push_back(new Command<Console>("getArchitectureData", "gets the architecture associated with the given id", &Console::getArchitectureData, this));
  commands.push_back(new Command<Console>("getSoundData", "gets the sound associated with the given id", &Console::getSoundData, this));
  commands.push_back(new Command<Console>("getSceneData", "gets the scene associated with the given id", &Console::getSceneData, this));
  commands.push_back(new Command<Console>("getDataFiles", "gets a list of the active data files", &Console::getDataFiles, this));
  commands.push_back(new Command<Console>("getPhysicsInfo", "gets the current state of the physics", &Console::getPhysicsInfo, this));
  commands.push_back(new Command<Console>("getGameInfo", "gets the current state of the game", &Console::getGameInfo, this));
  commands.push_back(new Command<Console>("getSceneInfo", "gets the current state of the scene", &Console::getSceneInfo, this));
  commands.push_back(new Command<Console>("getWorldInfo", "gets the current state of the world", &Console::getWorldInfo, this));
  commands.push_back(new Command<Console>("addItem", "adds the given item id to the scene", &Console::addItem, this));
  commands.push_back(new Command<Console>("addMonster", "adds the given monster id to the scene", &Console::addMonster, this));
  commands.push_back(new Command<Console>("addSound", "adds the given sound id to the scene", &Console::addSound, this));
  commands.push_back(new Command<Console>("setSceneLoaded", "either loads or unloads the given scene", &Console::setSceneLoaded, this));
}

//-------------------------------------------------------------------------------------
Console::~Console(void)
{
  for(std::vector<Command<Console>*>::iterator iter = commands.begin(); iter < commands.end(); ++iter)
  {
    if(*iter) delete (*iter);//deallocate commands
  }
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
  cursorAccumulator += elapsedSeconds;

  if(cursorAccumulator >= cursorFlashRate) 
  {
    showCursor = !showCursor;
    cursorAccumulator = 0.0;
    update();
  }

  if(keyIsDown)
  {
    holdAccumulator += elapsedSeconds;
    if(hasDoneFirstHold)
    {
      if(holdAccumulator >= consecutiveKeyPressHoldWait) 
      {
        keyPressed(OIS::KeyEvent(NULL,previousKey,0));
        holdAccumulator = 0.0;
      }
    }
    else
    {
      if(holdAccumulator >= firstKeyPressHoldWait) 
      {
        keyPressed(OIS::KeyEvent(NULL,previousKey,0));
        holdAccumulator = 0.0;
        hasDoneFirstHold = true;
      }
    }
  }
  else
  {
    holdAccumulator = 0.0;
    hasDoneFirstHold = false;
  }

  //if(!isVisible()) return;
}

//-------------------------------------------------------------------------------------
void Console::injectKeyDown(const OIS::KeyEvent &arg)
{
  injectKey(arg, true);
  keyPressed(arg);
}

//-------------------------------------------------------------------------------------
void Console::injectKeyUp(const OIS::KeyEvent &arg)
{
  injectKey(arg, false);
}

//-------------------------------------------------------------------------------------
void Console::injectKey(const OIS::KeyEvent &arg, bool isDown)
{
  if(!isVisible()) return;
  if(keyboard) 
  {
    if(arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT) isShift = isDown;
    else if(arg.key == OIS::KC_LCONTROL || arg.key == OIS::KC_RCONTROL) isControl = isDown;
    else 
    {
      keyIsDown = isDown;
    }
  }
}

//-------------------------------------------------------------------------------------
void Console::keyPressed(const OIS::KeyEvent &arg)
{
  if(!isVisible()) return;

  if(keyboard) 
  {
    if(arg.key == OIS::KC_SPACE) command += ' ';
    else if(arg.key == OIS::KC_GRAVE) return;
    else if(arg.key == OIS::KC_BACK) backspace();
    else if(arg.key == OIS::KC_RETURN) enter();
    
    else if(arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT || arg.key == OIS::KC_LCONTROL || arg.key == OIS::KC_RCONTROL) {}//ignore... handled elsewhere
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
      else if(arg.key == OIS::KC_UP) displayOffset++;//show previous text
      else if(arg.key == OIS::KC_DOWN) displayOffset--;//show more recent text

      else command += keyboard->getAsString(arg.key);
    }
    else if(arg.key == OIS::KC_UP) up();
    else if(arg.key == OIS::KC_DOWN) down();
    else 
    {
      std::string input = keyboard->getAsString(arg.key);
      input = std::tolower(input[0]);
      command += input;
    }
    cursorAccumulator = 0.0;//we want to be able to see the cursor while typing
    showCursor = true;
  }
  update();
  previousKey = arg.key;
}

//-------------------------------------------------------------------------------------
void Console::enter()
{
  historyIndex = 0;
  history.push_back(command);
  lines.push_back("> " + command);

  Options elements;
  split(command, ' ', elements);

  for(std::vector<Command<Console>*>::iterator iter = commands.begin(); iter < commands.end(); ++iter)
  {
    if((*iter)->name == elements[0]) 
    {
      try
      {
        (*iter)->run(elements);//run the required command
        command = "";
      }
      catch(NHException e)
      {
        error(e.what());
      }
      return;
    }
  }
  noCommand(command);
  command = "";
  /*
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
  
  command = "";*/
}

//-------------------------------------------------------------------------------------
void Console::print(std::string comment)
{
  display(comment);
  update();
}

//-------------------------------------------------------------------------------------
void Console::display(std::string comment)
{
  lines.push_back(comment);
  //text += "\n" + comment;
}

//-------------------------------------------------------------------------------------
void Console::display(std::string highlight, std::string comment)
{
  lines.push_back("%5" + highlight + "%r - " + comment);
  //text += "\n%5" + highlight + "%r - " + comment;
}

//-------------------------------------------------------------------------------------
void Console::error(std::string comment)
{
  lines.push_back("%4error%r - " + comment);
  //text += "\n%4error%r - " + comment;
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
  std::string viewText = "";
  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = lines.end();
  if(displayOffset < 0) displayOffset = 0;//keep offset above the minimum
  if(displayOffset + displayLines > lines.size()) displayOffset = lines.size() - displayLines;//keep offset below the top
  if(lines.size() < displayLines) iter = lines.begin();//fewer lines than the maximum allowable display lines
  else 
  {
    iter = lines.end() - (displayLines + displayOffset);
    end = iter + displayLines;
  }
  for(; iter < end; ++iter)
  {
    viewText += "\n " + *iter;
  }

  if(end == lines.end()) viewText += "\n%3> " + command;
    
  if(showCursor) viewText = viewText + "|";
  if(view) view->text(viewText);
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
  displayOffset = 0;
  if(historyIndex < history.size()) historyIndex++;
  if(historyIndex > 0) command = history[history.size() - historyIndex];
  else command = "";
}

//-------------------------------------------------------------------------------------
void Console::down()
{
  displayOffset = 0;
  if(historyIndex > 0) historyIndex--;
  if(historyIndex > 0) command = history[history.size() - historyIndex];
  else command = "";
}

//-------------------------------------------------------------------------------------
void Console::clear(Options argv)
{
  displayOffset = 0;
  lines.empty();
  //for(int i = 0; i < displayLines; i++) display("");//fill visible area with spaces
  //text = "";
  command = "";
}

//-------------------------------------------------------------------------------------
void Console::refresh(Options argv)
{
  hookWindow(window);
}

//-------------------------------------------------------------------------------------
void Console::about(Options argv)
{
  display("Copyright Arrian Purcell 2012.");
}

//-------------------------------------------------------------------------------------
void Console::help(Options argv)
{
  for(std::vector<Command<Console>*>::iterator iter = commands.begin(); iter < commands.end(); ++iter)
  {
    if(argv.size() > 1)//only show help for the specified
    {
      for(int i = 1; i < argv.size(); i++)
      {
        if(argv[i] == (*iter)->name) display((*iter)->name, (*iter)->help);
      }
    }
    else display((*iter)->name, (*iter)->help);//show all help
  }
}

//-------------------------------------------------------------------------------------
void Console::screenshot(Options argv)
{
  setVisible(false);
  display("Screenshot saved to '" + window->writeContentsToTimestampedFile("screenshot", ".jpg") + "'.");
  setVisible(true);
}

//-------------------------------------------------------------------------------------
void Console::setPhysicsEnabled(Options argv)
{
  bool isEnabled = boost::lexical_cast<bool>(argv[1]);
  world->freezeCollisionDebug = !isEnabled;
}

//-------------------------------------------------------------------------------------
void Console::setCameraFree(Options argv)
{
  bool isFree = boost::lexical_cast<bool>(argv[1]);
  world->getPlayer()->setCollisionEnabled(!isFree);//freeCameraDebug = !world->freeCameraDebug;
}

//-------------------------------------------------------------------------------------
void Console::setConsoleVisible(Options argv)
{
  bool visible = boost::lexical_cast<bool>(argv[1]);
  setVisible(!visible);
}

//-------------------------------------------------------------------------------------
void Console::setFullscreen(Options argv)
{
  int widthNum = boost::lexical_cast<int>(argv[1]);
  int heightNum = boost::lexical_cast<int>(argv[2]);

  if(widthNum <= 0 || heightNum <= 0) error("bad screen dimensions");
  else window->setFullscreen(true, widthNum, heightNum);
}

//-------------------------------------------------------------------------------------
void Console::setWindowed(Options argv)
{
  int widthNum = boost::lexical_cast<int>(argv[1]);
  int heightNum = boost::lexical_cast<int>(argv[2]);

  if(widthNum <= 0 || heightNum <= 0) error("bad screen dimensions");
  else window->setFullscreen(false, widthNum, heightNum);
}

//-------------------------------------------------------------------------------------
void Console::setPlayerScene(Options argv)
{
  std::string sceneName = argv[1];
  Scene* target = world->getScene(sceneName);
  if(target) world->movePlayer(world->getPlayer(), target);
  else error("no scene loaded named '" + sceneName + "'");
}

//-------------------------------------------------------------------------------------
void Console::setSceneAmbientLight(Options argv)
{
  Scene* target = world->getPlayer()->getScene();
  if(target) target->getSceneManager()->setAmbientLight(Ogre::ColourValue(boost::lexical_cast<float>(argv[1]), boost::lexical_cast<float>(argv[2]), boost::lexical_cast<float>(argv[3]), boost::lexical_cast<float>(argv[4])));
  else error("player needs to be located within a scene to be able to change the ambient light");
}

//-------------------------------------------------------------------------------------
void Console::setPlayerPosition(Options argv)
{
  Ogre::Vector3 position = Ogre::Vector3(Ogre::Real(boost::lexical_cast<float>(argv[1])), Ogre::Real(boost::lexical_cast<float>(argv[2])), Ogre::Real(boost::lexical_cast<float>(argv[3])));
  world->getPlayer()->setPosition(position);
}

//-------------------------------------------------------------------------------------
void Console::setPlayerItemGenerationID(Options argv)
{
  world->getPlayer()->setItemGenerationID(boost::lexical_cast<int>(argv[1]));
}

//-------------------------------------------------------------------------------------
void Console::getItemData(Options argv)
{
  try
  {
    ItemDesc desc = world->getDataManager()->getItem(boost::lexical_cast<int>(argv[1]));
    display("name", desc.name);
    display("mesh", desc.mesh);
  }
  catch(NHException e)
  {
    error(e.what());
  }
}

//-------------------------------------------------------------------------------------
void Console::getMonsterData(Options argv)
{
  try
  {
    MonsterDesc desc = world->getDataManager()->getMonster(boost::lexical_cast<int>(argv[1]));
    display("name", desc.name);
    display("mesh", desc.mesh);
  }
  catch(NHException e)
  {
    error(e.what());
  }
}

//-------------------------------------------------------------------------------------
void Console::getArchitectureData(Options argv)
{
  try
  {
    ArchitectureDesc desc = world->getDataManager()->getArchitecture(boost::lexical_cast<int>(argv[1]));
    display("name", desc.name);
    display("mesh", desc.mesh);
  }
  catch(NHException e)
  {
    error(e.what());
  }
}
//-------------------------------------------------------------------------------------
void Console::getSoundData(Options argv)
{

}

//-------------------------------------------------------------------------------------
void Console::getSceneData(Options argv)
{

}

//-------------------------------------------------------------------------------------
void Console::getDataFiles(Options argv)
{
  std::vector<std::string> files = world->getDataManager()->getLoadedDataFiles();

  for(std::vector<std::string>::iterator iter = files.begin(); iter < files.end(); ++iter) display(*iter);
}

//-------------------------------------------------------------------------------------
void Console::getPhysicsInfo(Options argv)
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
void Console::getGameInfo(Options argv)
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
void Console::getSceneInfo(Options argv)
{
  display("name", world->getPlayer()->getScene()->getName());
  display("ogre internal name", world->getPlayer()->getScene()->getSceneManager()->getName());
}

//-------------------------------------------------------------------------------------
void Console::getWorldInfo(Options argv)
{

}

//-------------------------------------------------------------------------------------
void Console::getPlayerPosition(Options argv)
{
  Ogre::Vector3 position = world->getPlayer()->getPosition();
  display("x", boost::lexical_cast<std::string>(position.x));
  display("y", boost::lexical_cast<std::string>(position.y));
  display("z", boost::lexical_cast<std::string>(position.z));
}

//-------------------------------------------------------------------------------------
void Console::addItem(Options argv)
{
  try
  {
    Scene* target = world->getPlayer()->getScene();
    if(target)
    {
      int numberToAdd = 1;
      if(argv.size() > 5) numberToAdd = boost::lexical_cast<int>(argv[5]);
      Ogre::Vector3 position = Ogre::Vector3(Ogre::Real(boost::lexical_cast<float>(argv[2])), Ogre::Real(boost::lexical_cast<float>(argv[3])), Ogre::Real(boost::lexical_cast<float>(argv[4])));
      int idNum = boost::lexical_cast<int>(argv[1]);
      for(int i = 0; i < numberToAdd; i++) target->addItem(idNum, position);
    }
    else error("player needs to be located within a scene to add an item to it");
  }
  catch(NHException e)
  {
    error(e.what());
  }
}
//-------------------------------------------------------------------------------------
void Console::addMonster(Options argv)
{
  try
  {
    Scene* target = world->getPlayer()->getScene();
    if(target)
    {
      Ogre::Vector3 position = Ogre::Vector3(Ogre::Real(boost::lexical_cast<float>(argv[2])), Ogre::Real(boost::lexical_cast<float>(argv[3])), Ogre::Real(boost::lexical_cast<float>(argv[4])));
      int idNum = boost::lexical_cast<int>(argv[1]);
      target->addMonster(idNum, position);
    }
    else error("player needs to be located within a scene to add a monster to it");
  }
  catch(NHException e)
  {
    error(e.what());
  }
}
//-------------------------------------------------------------------------------------
void Console::addSound(Options argv)
{

}

//-------------------------------------------------------------------------------------
void Console::setSceneLoaded(Options argv)
{
  bool load = boost::lexical_cast<bool>(argv[2]);
  if(load)
  {
    try
    {
      if(world->loadScene(boost::lexical_cast<int>(argv[1]))) display("scene loaded");
      else display("could not load the scene");
    }
    catch(NHException e)
    {
      error(e.what());
    }
  }
  else
  {
    try
    {
      if(world->destroyScene(argv[1])) display("scene unloaded");
      else error("could not unload the scene");
    }
    catch(NHException e)
    {
      error(e.what());
    }
  }
}

//-------------------------------------------------------------------------------------
void Console::setRequired(World* world, OIS::Keyboard* keyboard)
{
  this->world = world;
  this->keyboard = keyboard;
}
