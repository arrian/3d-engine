#include "renderer.h"

void Renderer::init(std::string plugins_file)
{
  #ifdef _DEBUG
  std::string resources = "resources_d.cfg";
  std::string plugins = "plugins_d.cfg";
#else
  std::string resources = "resources.cfg";
  std::string plugins = "plugins.cfg";
#endif

  mOgreRoot = new Ogre::Root(plugins);

  
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
  

  if(mOgreRoot->showConfigDialog())//mOgreRoot->restoreConfig() || 
  {
    // setup a renderer
    Ogre::RenderSystemList::const_iterator renderers = mOgreRoot->getAvailableRenderers().begin();
    while(renderers != mOgreRoot->getAvailableRenderers().end())
    {
      Ogre::String rName = (*renderers)->getName();
      if (rName == "OpenGL Rendering Subsystem") break;
      renderers++;
    }
 
    //Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);//Set default mipmap level (NB some APIs ignore this)
    //Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    Ogre::RenderSystem *renderSystem = (*renderers);
    mOgreRoot->setRenderSystem(renderSystem);

    mOgreRoot->initialise(false);
  }

  
  //sceneManager = root->createSceneManager(Ogre::ST_INTERIOR);//ST_GENERIC);
  

  //////////////////////////////////////////////////////////////////////

  /*
  // create the main ogre object
  mOgreRoot = new Ogre::Root(plugins_file);
 
  QString dimensions = QString( "%1x%2" )
                    .arg(this->width())
                    .arg(this->height());
 
  renderSystem->setConfigOption( "Video Mode", dimensions.toStdString() );
 
  // initialize without creating window
  mOgreRoot->getRenderSystem()->setConfigOption( "Full Screen", "No" );
  mOgreRoot->saveConfig();
  mOgreRoot->initialise(false); // don't create a window
  */
}
 
void Renderer::initializeGL()
{
  //== Creating and Acquiring Ogre Window ==//
 
  // Get the parameters of the window QT created
  Ogre::String winHandle;
#ifdef WIN32
  // Windows code
  winHandle += Ogre::StringConverter::toString((unsigned long)(this->parentWidget()->winId()));
#elif MACOS
  // Mac code, tested on Mac OSX 10.6 using Qt 4.7.4 and Ogre 1.7.3
  Ogre::String winHandle  = Ogre::StringConverter::toString(winId());
#else
  // Unix code
  QX11Info info = x11Info();
  winHandle  = Ogre::StringConverter::toString((unsigned long)(info.display()));
  winHandle += ":";
  winHandle += Ogre::StringConverter::toString((unsigned int)(info.screen()));
  winHandle += ":";
  winHandle += Ogre::StringConverter::toString((unsigned long)(this->parentWidget()->winId()));
#endif
 
 
  Ogre::NameValuePairList params;
#ifndef MACOS
  // code for Windows and Linux
  params["parentWindowHandle"] = winHandle;
  mOgreWindow = mOgreRoot->createRenderWindow( "QOgreWidget_RenderWindow",
                           this->width(),
                           this->height(),
                           false,
                           &params );
 
  mOgreWindow->setActive(true);
  WId ogreWinId = 0x0;
  mOgreWindow->getCustomAttribute( "WINDOW", &ogreWinId );
 
  assert( ogreWinId );
 
  // bug fix, extract geometry
  QRect geo = this->frameGeometry ( );
 
  // create new window
  this->create( ogreWinId );
 
  // set geometrie infos to new window
  this->setGeometry (geo);
 
#else
  // code for Mac
  params["externalWindowHandle"] = winHandle;
  params["macAPI"] = "cocoa";
  params["macAPICocoaUseNSView"] = "true";
  mOgreWindow = mOgreRoot->createRenderWindow("QOgreWidget_RenderWindow",
      width(), height(), false, &params);
  mOgreWindow->setActive(true);
  makeCurrent();
#endif
 
  setAttribute( Qt::WA_PaintOnScreen, true );
  setAttribute( Qt::WA_NoBackground );
 

  World* world = new World(mOgreRoot, mOgreWindow);

  //== Ogre Initialization ==//
  //Ogre::SceneType scene_manager_type = Ogre::ST_EXTERIOR_CLOSE;
 
  //mSceneMgr = mOgreRoot->createSceneManager( scene_manager_type );
  //mSceneMgr->setAmbientLight( Ogre::ColourValue(1,1,1) );
 
  //mCamera = mSceneMgr->createCamera( "QOgreWidget_Cam" );
  //mCamera->setPosition( Ogre::Vector3(0,1,0) );
  //mCamera->lookAt( Ogre::Vector3(0,0,0) );
  //mCamera->setNearClipDistance( 1.0 );
 
  //Ogre::Viewport *mViewport = mOgreWindow->addViewport(mCamera);
  //mViewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
}
 
void Renderer::paintGL()
{
  assert( mOgreWindow );
  mOgreRoot->renderOneFrame();
}
 
void Renderer::resizeGL( int width, int height )
{
  assert( mOgreWindow );
  mOgreWindow->windowMovedOrResized();
}
 
Ogre::RenderSystem* Renderer::chooseRenderer( Ogre::RenderSystemList *renderers )
{
  // It would probably be wise to do something more friendly 
  // that just use the first available renderer
  return *renderers->begin();
}

