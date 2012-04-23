#include "Console.h"

Console::Console(World* world, OIS::Keyboard* keyboard)
  : overlay(new Gorilla::Silverback),
    keyboard(keyboard),
    isShift(false),
    isControl(false),
    world(world),
    window(0),
    screen(0),
    layer(0),
    view(0)
{
  overlay->loadAtlas("dejavu");
  text = "";
  command = "";
}

Console::~Console(void)
{
}

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

void Console::setVisible(bool visible)
{
  if(layer) layer->setVisible(visible);
}

bool Console::isVisible()
{
  if(layer) return layer->isVisible();
  return false;
}

void Console::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(!isVisible()) return;
}

void Console::injectKeyDown(const OIS::KeyEvent &arg)
{
  if(!isVisible()) return;
  if(keyboard)
  {
    if(arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT) isShift = true;
    else if(arg.key == OIS::KC_LCONTROL || arg.key == OIS::KC_RCONTROL) isControl = true;
  }
}

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


void Console::enter()
{
  text += "\n> " + command;

  std::vector<std::string> elements;
  split(command, ' ', elements);
  
  if(elements.size() == 0) {}
  else if(elements.size() == 1)
  {
    if(elements[0] == "help") help();
    else if(elements[0] == "about") display("Copyright Arrian Purcell 2012\n");
    else if(elements[0] == "clear") clear();
    else if(elements[0] == "stats") stats();
    else if(elements[0] == "freeze") world->freezeCollisionDebug = true;
    else if(elements[0] == "unfreeze") world->freezeCollisionDebug = false;
    else if(elements[0] == "freecam") world->freeCameraDebug = !world->freeCameraDebug;
    else if(elements[0] == "hide") setVisible(false);
    else if(elements[0] == "refresh") hookWindow(window);
    else if(elements[0] == "screenshot")
    {
      setVisible(false);
      display("Screenshot saved to '" + window->writeContentsToTimestampedFile("screenshot", ".jpg") + "'.");
      setVisible(true);
    }
    else noCommand(command);
  }
  else if(elements.size() == 2)
  {
    if(elements[0] == "go")//moves the player to the specified cell
    {
      Cell* target = world->getCell(elements[1]);
      if(target) world->movePlayer(world->getPlayer(), target);
      else error("no cell named '" + elements[1] + "'");
    }
    else if(elements[0] == "unload")
    {
      if(world->destroyCell(elements[1])) display("cell unloaded");
      else error("could not unload the cell");
    }
    else if(elements[0] == "physics")
    {
      Cell* target = world->getCell(elements[1]);
      if(target) 
      {
        physx::PxScene* physics = target->getPhysicsManager();
        if(physics)
        {
          display("gravity", Ogre::StringConverter::toString(physics->getGravity().magnitude()));
          display("current timestamp", Ogre::StringConverter::toString(physics->getTimestamp()));
          display("number of rigid static actors", Ogre::StringConverter::toString(physics->getNbActors(physx::PxActorTypeSelectionFlags(physx::PxActorTypeSelectionFlag::eRIGID_STATIC))));
          display("number of rigid dynamic actors", Ogre::StringConverter::toString(physics->getNbActors(physx::PxActorTypeSelectionFlags(physx::PxActorTypeSelectionFlag::eRIGID_DYNAMIC))));
          display("number of cloth actors", Ogre::StringConverter::toString(physics->getNbActors(physx::PxActorTypeSelectionFlags(physx::PxActorTypeSelectionFlag::eCLOTH))));
          display("number of particle fluid actors", Ogre::StringConverter::toString(physics->getNbActors(physx::PxActorTypeSelectionFlags(physx::PxActorTypeSelectionFlag::ePARTICLE_FLUID))));
          display("number of particle system actors", Ogre::StringConverter::toString(physics->getNbActors(physx::PxActorTypeSelectionFlags(physx::PxActorTypeSelectionFlag::ePARTICLE_SYSTEM))));
          display("number of aggregates", Ogre::StringConverter::toString(physics->getNbAggregates()));
          display("number of articulations", Ogre::StringConverter::toString(physics->getNbArticulations()));
          display("number of contraints", Ogre::StringConverter::toString(physics->getNbConstraints()));
        }
        else error("no physics associated with this cell");
      }
      else error("no cell named '" + elements[1] + "'");
    }
    else noCommand(command);
  }
  else if(elements.size() == 3)
  {
    if(elements[0] == "fullscreen" || elements[0] == "window") 
    {
      int width = std::atoi(elements[1].c_str());
      int height = std::atoi(elements[2].c_str());

      if(width == 0 || height == 0) error("bad screen dimensions");
      else window->setFullscreen((elements[0] == "fullscreen"), width, height);
    }
    else if(elements[0] == "load")
    {
      SceneType type = DUNGEON;
      if(elements[2] == "dungeon") type = DUNGEON;
      else if(elements[2] == "predefined") type = PREDEFINED;
      else if(elements[2] == "file") type = FILE_CHAR;
      
      if(world->loadCell(elements[1], type)) display("cell loaded");
      else display("could not load the cell");
    }
    else if(elements[0] == "data")
    {
      int id = std::atoi(elements[2].c_str());
      if(elements[1] == "architecture")
      {
        ArchitectureModel* model = world->getDataManager()->getArchitecture(id);
        if(model) 
        {
          display("name", model->name);
          display("mesh", model->mesh);
        }
        else error("no architecture item has that id");
      }
      else if(elements[1] == "item")
      {
        ItemModel* model = world->getDataManager()->getItem(id);
        if(model) 
        {
          display("name", model->name);
          display("mesh", model->mesh);
        }
        else error("no item has that id");
      }
      else if(elements[1] == "monster")
      {
        MonsterModel* model = world->getDataManager()->getMonster(id);
        if(model) 
        {
          display("name", model->name);
          display("mesh", model->mesh);
        }
        else error("no monster has that id");
      }
    }
    else noCommand(command);
  }
  else if(elements.size() == 5)
  {
    if(elements[0] == "ambient")
    {
      float r = std::atof(elements[2].c_str());
      float g = std::atof(elements[3].c_str());
      float b = std::atof(elements[4].c_str());
      
      Cell* target = world->getCell(elements[1]);
      if(target) target->getSceneManager()->setAmbientLight(Ogre::ColourValue(r, g, b));
      else error("no cell named '" + elements[1] + "'");
    }
    else noCommand(command);
  }
  else if(elements.size() == 6)
  {
    if(elements[0] == "add")
    {
      Cell* target = world->getCell(elements[1]);
      if(target)
      {
        int x = std::atoi(elements[3].c_str());
        int y = std::atoi(elements[4].c_str());
        int z = std::atoi(elements[5].c_str());
        Ogre::Vector3 position = Ogre::Vector3(x, y, z);
        if(elements[2] == "item") target->addItem(position);
        else if(elements[2] == "monster") target->addMonster(position);
        else error("no object type '" + elements[2] + "' exists");
      }
      else error("no cell named '" + elements[1] + "'");
    }
    else noCommand(command);
  }
  else noCommand(command);
  
  command = "";
}

void Console::help()
{
  display("help", "shows this list");
  display("stats", "shows a list of statistics and a list of loaded cells");
  display("clear", "clears the console");
  display("freeze", "freezes physics");
  display("unfreeze", "unfreezes physics");
  display("freecam", "frees the camera");
  display("hide", "hides the console");
  display("refresh", "temp method to rehook console to viewport");
  display("fullscreen [width] [height]", "sets the window to fullscreen mode");
  display("window [width] [height]", "sets the window to windowed mode");
  display("add [cell name] [item|monster] [x] [y] [z]", "adds the given type of entity to the named cell at the given coordinates");
  display("data [item|monster|architecture] [id]", "displays the data associated with the id of the given type of entity");
  display("screenshot", "takes a screenshot and outputs to timestamped file");
  display("go [cell name]", "moves the player to the specified cell");
  display("load [cell name] [dungeon|predefined|file]", "loads a cell into memory");
  display("unload [cell name]", "unloads a cell from memory");
  display("ambient [cell name] [r] [g] [b]", "sets the ambient light in the given cell");
  display("physics [cell name]", "shows physx physics stats for the given cell");
}

void Console::clear()
{
  text = "";
  command = "";
}

void Console::update()
{
  if(view) view->text(text + "\n%3> " + command + "_");
}

void Console::backspace()
{
  if(command.size() == 0) return;
  command = command.substr(0, command.size() - 1);
}

void Console::stats()
{
  if(!window) 
  {
    error("no render window");
    return;
  }

  display("average fps", Ogre::StringConverter::toString(int(window->getAverageFPS())));
  display("best fps", Ogre::StringConverter::toString(window->getBestFPS()));
  display("batches", Ogre::StringConverter::toString(window->getBatchCount()));
  display("colour depth", Ogre::StringConverter::toString(window->getColourDepth()));
  display("number of viewports", Ogre::StringConverter::toString(window->getNumViewports()));
  display("triangle count", Ogre::StringConverter::toString(window->getTriangleCount()));
  display("window size", Ogre::StringConverter::toString(window->getWidth()) + "x" + Ogre::StringConverter::toString(window->getHeight()));
  display("number of loaded cells", Ogre::StringConverter::toString(world->getNumberCells()));

  std::vector<Ogre::String> names;
  world->getCellNames(names);
  Ogre::String list = "";
  for(std::vector<Ogre::String>::iterator it = names.begin(); it < names.end(); ++it) list += (*it) + " ";
  display("loaded cells", list);
}

void Console::display(Ogre::String comment)
{
  text += "\n" + comment;
}

void Console::display(Ogre::String highlight, Ogre::String comment)
{
  text += "\n%5" + highlight + "%r - " + comment;
}

void Console::error(Ogre::String comment)
{
  text += "\n%4error%r - " + comment;
}

void Console::noCommand(Ogre::String command)
{
  error("there is no command '" + command + "'");
}

void Console::split(const std::string &s, char delim, std::vector<std::string> &elems) 
{
  std::stringstream ss(s);
  std::string item;
  while(std::getline(ss, item, delim)) elems.push_back(item);
}



