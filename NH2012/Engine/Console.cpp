#include "Console.h"

#include "Scene.h"

Console::Console(World* world, OIS::Keyboard* keyboard)
  : overlay(new Gorilla::Silverback),
    keyboard(keyboard),
    isShift(false),
    isControl(false),
    world(world),
    window(0),
    screen(0),
    layer(0),
    view(0),
    text(""),
    command("")
{
  overlay->loadAtlas("dejavu");
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
    else if(elements[0] == "freecam") world->getPlayer()->setCollisionEnabled(false);//freeCameraDebug = !world->freeCameraDebug;
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
    if(elements[0] == "go")//moves the player to the specified scene
    {
      Scene* target = world->getScene(elements[1]);
      if(target) world->movePlayer(world->getPlayer(), target);
      else error("no scene named '" + elements[1] + "'");
    }
    else if(elements[0] == "load")
    {
      if(world->loadScene(boost::lexical_cast<int>(elements[1]))) display("scene loaded");
      else display("could not load the scene");
    }
    else if(elements[0] == "player_gravity")
    {
      float gravity = boost::lexical_cast<float>(elements[1]);
      world->getPlayer()->setGravity(gravity);
    }
    else if(elements[0] == "unload")
    {
      if(world->destroyScene(elements[1])) display("scene unloaded");
      else error("could not unload the scene");
    }
    else if(elements[0] == "physics")
    {
      Scene* target = world->getScene(elements[1]);
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
      else error("no scene named '" + elements[1] + "'");
    }
    else noCommand(command);
  }
  else if(elements.size() == 3)
  {
    if(elements[0] == "fullscreen" || elements[0] == "window") 
    {
      int width = boost::lexical_cast<int>(elements[1]);
      int height = boost::lexical_cast<int>(elements[2]);

      if(width == 0 || height == 0) error("bad screen dimensions");
      else window->setFullscreen((elements[0] == "fullscreen"), width, height);
    }
    else if(elements[0] == "data")
    {
      int id = boost::lexical_cast<int>(elements[2]);
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
  else if(elements.size() == 4)
  {
    if(elements[0] == "player_pos")
    {
      float x = boost::lexical_cast<float>(elements[1]);
      float y = boost::lexical_cast<float>(elements[2]);
      float z = boost::lexical_cast<float>(elements[3]);
      Ogre::Vector3 position = Ogre::Vector3(Ogre::Real(x), Ogre::Real(y), Ogre::Real(z));
      world->getPlayer()->setPosition(position);
    }
    else noCommand(command);
  }
  else if(elements.size() == 5)
  {
    if(elements[0] == "ambient")
    {
      float r = boost::lexical_cast<float>(elements[2]);
      float g = boost::lexical_cast<float>(elements[3]);
      float b = boost::lexical_cast<float>(elements[4]);
      
      Scene* target = world->getScene(elements[1]);
      if(target) target->getSceneManager()->setAmbientLight(Ogre::ColourValue(r, g, b));
      else error("no scene named '" + elements[1] + "'");
    }
    else noCommand(command);
  }
  else if(elements.size() == 6)
  {
    if(elements[0] == "add")
    {
      Scene* target = world->getScene(elements[1]);
      if(target)
      {
        float x = boost::lexical_cast<float>(elements[3]);
        float y = boost::lexical_cast<float>(elements[4]);
        float z = boost::lexical_cast<float>(elements[5]);
        Ogre::Vector3 position = Ogre::Vector3(Ogre::Real(x), Ogre::Real(y), Ogre::Real(z));
        if(elements[2] == "item") target->addItem(0, position);
        else if(elements[2] == "monster") target->addMonster(0, position);
        else error("no object type '" + elements[2] + "' exists");
      }
      else error("no scene named '" + elements[1] + "'");
    }
    else noCommand(command);
  }
  else noCommand(command);
  
  command = "";
}

void Console::help()
{
  display("help", "shows this list");
  display("stats", "shows a list of statistics and a list of loaded scenes");
  display("clear", "clears the console");
  display("freeze", "freezes physics");
  display("unfreeze", "unfreezes physics");
  display("freecam", "frees the camera");
  display("hide", "hides the console");
  display("refresh", "temp method to rehook console to viewport");
  display("fullscreen [width] [height]", "sets the window to fullscreen mode");
  display("window [width] [height]", "sets the window to windowed mode");
  display("add [scene name] [item|monster] [x] [y] [z]", "adds the given type of entity to the named scene at the given coordinates");
  display("data [item|monster|architecture] [id]", "displays the data associated with the id of the given type of entity");
  display("screenshot", "takes a screenshot and outputs to timestamped file");
  display("go [scene name]", "moves the player to the specified scene");
  display("load [scene id]", "loads a scene into memory");
  display("unload [scene name]", "unloads a scene from memory");
  display("ambient [scene name] [r] [g] [b]", "sets the ambient light in the given scene");
  display("physics [scene name]", "shows physx physics stats for the given scene");
  display("player_pos [x] [y] [z]", "sets the player position");
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
  display("number of loaded scenes", Ogre::StringConverter::toString(world->getNumberScenes()));

  std::vector<Ogre::String> names;
  world->getSceneNames(names);
  Ogre::String list = "";
  for(std::vector<std::string>::iterator it = names.begin(); it < names.end(); ++it) list += (*it) + " ";
  display("loaded scenes", list);
}

void Console::display(std::string comment)
{
  text += "\n" + comment;
}

void Console::display(std::string highlight, std::string comment)
{
  text += "\n%5" + highlight + "%r - " + comment;
}

void Console::error(std::string comment)
{
  text += "\n%4error%r - " + comment;
}

void Console::noCommand(std::string command)
{
  error("there is no command '" + command + "'");
}

void Console::split(const std::string &s, char delim, std::vector<std::string> &elems) 
{
  std::stringstream ss(s);
  std::string item;
  while(std::getline(ss, item, delim)) elems.push_back(item);
}



