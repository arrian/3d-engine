#include "NH2012.h"


//-------------------------------------------------------------------------------------
NH2012::NH2012(void)
: root(0),
  window(0),
  resources(Ogre::StringUtil::BLANK),
  plugins(Ogre::StringUtil::BLANK),
  cursorWasVisible(false),
  shutDown(false),
  inputManager(0),
  mouse(0),
  keyboard(0),
  game(0)
{
}

//-------------------------------------------------------------------------------------
NH2012::~NH2012(void)
{
  //Remove window listener from this
  Ogre::WindowEventUtilities::removeWindowEventListener(window, this);
  windowClosed(window);
  delete root;
}

//-------------------------------------------------------------------------------------
bool NH2012::go(void)
{
#ifdef _DEBUG
  resources = "resources_d.cfg";
  plugins = "plugins_d.cfg";
#else
  mResourcesCfg = "resources.cfg";
  mPluginsCfg = "plugins.cfg";
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
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
          archName, typeName, secName);
    }
  }

  if(root->restoreConfig() || root->showConfigDialog())
  {
    window = root->initialise(true, "NH2012");

    //Window icon
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    HWND hwnd;
    window->getCustomAttribute("WINDOW", (void*)&hwnd);
    LONG iconID   = (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_APPICON) );
    SetClassLong( hwnd, GCL_HICON, iconID );
#endif
  }
  else return false;

  //Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);//Set default mipmap level (NB some APIs ignore this)//causes errors with d3d11
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

  OIS::ParamList parameters;
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;

  window->getCustomAttribute("WINDOW", &windowHnd);
  windowHndStr << windowHnd;
  parameters.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

  inputManager = OIS::InputManager::createInputSystem(parameters);

  keyboard = static_cast<OIS::Keyboard*>(inputManager->createInputObject(OIS::OISKeyboard, true));
  mouse = static_cast<OIS::Mouse*>(inputManager->createInputObject(OIS::OISMouse, true));

  game = new Singleplayer(root, window, keyboard);

  mouse->setEventCallback(this);
  keyboard->setEventCallback(this);

  windowResized(window);//Set initial mouse clipping size
  Ogre::WindowEventUtilities::addWindowEventListener(window, this);//Register as a Window listener

  root->addFrameListener(this);
  root->startRendering();
  return true;
}

bool NH2012::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(window->isClosed()) return false;
  if(shutDown) return false;

  keyboard->capture();
  mouse->capture();

  game->frameRenderingQueued(evt);

  return true;
}

//-------------------------------------------------------------------------------------
bool NH2012::keyPressed( const OIS::KeyEvent &arg )
{
  if (arg.key == OIS::KC_ESCAPE) shutDown = true;

  game->injectKeyDown(arg);
  return true;
}

bool NH2012::keyReleased( const OIS::KeyEvent &arg )
{
  game->injectKeyUp(arg);
  return true;
}

bool NH2012::mouseMoved( const OIS::MouseEvent &arg )
{
  game->injectMouseMove(arg);
  return true;
}

bool NH2012::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  game->injectMouseDown(arg, id);
  return true;
}

bool NH2012::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  game->injectMouseUp(arg, id);
  return true;
}

//Adjust mouse clipping area
void NH2012::windowResized(Ogre::RenderWindow* rw)
{
  unsigned int width, height, depth;
  int left, top;
  rw->getMetrics(width, height, depth, left, top);

  const OIS::MouseState &ms = mouse->getMouseState();
  ms.width = width;
  ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void NH2012::windowClosed(Ogre::RenderWindow* rw)
{
  if(rw == window)
  {
    if(inputManager)
    {
      inputManager->destroyInputObject(mouse);
      inputManager->destroyInputObject(keyboard);

      OIS::InputManager::destroyInputSystem(inputManager);
      inputManager = 0;
    }
  }
}



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

    //Create application
    NH2012 app;

    try 
    {
      app.go();
    } 
    catch( Ogre::Exception& e ) 
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
      std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
#endif
    }

    return 0;
  }

#ifdef __cplusplus
}
#endif
