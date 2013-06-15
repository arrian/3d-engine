#include "Game.h"


//-------------------------------------------------------------------------------------
Game::Game(void)
: inputManager(NULL),
  keyboard(NULL),  
  window(NULL),
  mouse(NULL),
  root(NULL),
  world(NULL),
  done(false)
{
}

//-------------------------------------------------------------------------------------
Game::~Game(void)
{
  Ogre::WindowEventUtilities::removeWindowEventListener(window, this);
  windowClosed(window);
  delete userInterface;
  delete world;
  delete root;
}

//-------------------------------------------------------------------------------------
bool Game::run()
{
  std::string resources;
  std::string plugins;

#ifdef _DEBUG
  resources = "resources_d.cfg";
  plugins = "plugins_d.cfg";
#else
  resources = "resources.cfg";
  plugins = "plugins.cfg";
#endif

  root = new Ogre::Root(plugins);

  Ogre::ConfigFile cf;
  cf.load(resources);
  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

  Ogre::String secName, typeName, archName;
  while (seci.hasMoreElements())
  {
    secName = seci.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i)
    {
      typeName = i->first;
      archName = i->second;
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
    }
  }

  if(!root->restoreConfig() && !root->showConfigDialog()) return false;

  window = root->initialise(true, "Game");

  //Window icon
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  HWND hwnd;
  window->getCustomAttribute("WINDOW", (void*)&hwnd);
  LONG iconID = (LONG)LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_APPICON));
  SetClassLong(hwnd, GCL_HICON, iconID);
#endif
  
  //Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);//Set default mipmap level (NB some APIs ignore this)//causes errors with d3d11
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  ::Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(::Ogre::TFO_ANISOTROPIC);

  OIS::ParamList parameters;
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;

  window->getCustomAttribute("WINDOW", &windowHnd);
  windowHndStr << windowHnd;
  parameters.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

  inputManager = OIS::InputManager::createInputSystem(parameters);

  keyboard = static_cast<OIS::Keyboard*>(inputManager->createInputObject(OIS::OISKeyboard, true));
  mouse = static_cast<OIS::Mouse*>(inputManager->createInputObject(OIS::OISMouse, true));

  world = new World(root, window);
  world->initialise("game.ini");

  userInterface = new Interface(world);
  userInterface->hookWindow(window);

  mouse->setEventCallback(this);
  keyboard->setEventCallback(this);

  windowResized(window);//Set initial mouse clipping size
  Ogre::WindowEventUtilities::addWindowEventListener(window, this);//Register as a Window listener

  root->addFrameListener(this);
  root->startRendering();
  return true;
}

//-------------------------------------------------------------------------------------
bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(window->isClosed()) return false;

  keyboard->capture();
  mouse->capture();

  if(done) return false;

  userInterface->update(evt.timeSinceLastFrame);
  if(!world->update(evt.timeSinceLastFrame)) return false;

  return !done;//should update is not of done
}

//-------------------------------------------------------------------------------------
bool Game::keyPressed(const OIS::KeyEvent &arg)
{
  if(arg.key == OIS::KC_ESCAPE) done = true;//temp fast exit
  if(arg.key == OIS::KC_F1) ;

  userInterface->keyPressed(arg);
  if(userInterface->capturedInput()) return true;
  world->keyPressed(arg);

  return true;
}

//-------------------------------------------------------------------------------------
bool Game::keyReleased(const OIS::KeyEvent &arg)
{
  userInterface->keyReleased(arg);
  if(userInterface->capturedInput()) return true;
  world->keyReleased(arg);

  return true;
}

//-------------------------------------------------------------------------------------
bool Game::mouseMoved(const OIS::MouseEvent &arg)
{
  userInterface->mouseMoved(arg);
  if(userInterface->capturedInput()) return true;
  world->mouseMoved(arg);

  return true;
}

//-------------------------------------------------------------------------------------
bool Game::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  userInterface->mousePressed(arg, id);
  if(userInterface->capturedInput()) return true;
  world->mousePressed(arg,id);

  return true;
}

//-------------------------------------------------------------------------------------
bool Game::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  userInterface->mouseReleased(arg, id);
  if(userInterface->capturedInput()) return true;
  world->mouseReleased(arg,id);
  
  return true;
}

//-------------------------------------------------------------------------------------
void Game::windowResized(Ogre::RenderWindow* rw)//Adjust mouse clipping area
{
  unsigned int width, height, depth;
  int left, top;
  rw->getMetrics(width, height, depth, left, top);

  const OIS::MouseState &ms = mouse->getMouseState();
  ms.width = width;
  ms.height = height;
}

//-------------------------------------------------------------------------------------
void Game::windowClosed(Ogre::RenderWindow* rw)//Unattach OIS before window shutdown (very important under Linux)
{
  if(rw == window && inputManager)
  {
    inputManager->destroyInputObject(mouse);
    inputManager->destroyInputObject(keyboard);

    OIS::InputManager::destroyInputSystem(inputManager);
    inputManager = NULL;
  }
}

//-------------------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" 
{
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
  int main(int argc, char *argv[])
#endif
  {

//Console
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#ifdef _DEBUG
    AllocConsole();
    freopen("CONOUT$","wb",stdout);
#endif
#endif

    try 
    {
      Game game;
      game.run();
    } 
    catch(Ogre::Exception& e) 
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
      std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
#endif
    }
    catch(NHException& e)
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      MessageBox(NULL, e.what(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
      std::cerr << "An exception has occured: " << e.what() << std::endl;
#endif
    }

    return 0;
  }

#ifdef __cplusplus
}
#endif
