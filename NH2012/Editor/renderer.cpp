#include "renderer.h"

void Renderer::init(World* world)
{
  hasMouseClick = false;
  this->world = world;

  #ifdef _DEBUG
  std::string resources = "resources_d.cfg";
  std::string plugins = "plugins_d.cfg";
#else
  std::string resources = "resources.cfg";
  std::string plugins = "plugins.cfg";
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
  
  if(root->restoreConfig()) 
  {
    Ogre::RenderSystemList::const_iterator renderers = root->getAvailableRenderers().begin();
    while(renderers != root->getAvailableRenderers().end())
    {
      Ogre::String rName = (*renderers)->getName();
      if (rName == "OpenGL Rendering Subsystem") break;
      renderers++;
    }
    Ogre::RenderSystem *renderSystem = (*renderers);
    root->setRenderSystem(renderSystem);

    root->initialise(false);

    //Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);//Set default mipmap level (NB some APIs ignore this)
  }
}
 
void Renderer::initializeGL()
{
  //== Creating and Acquiring Ogre Window ==//
 
  // Get the parameters of the window QT created
  Ogre::String winHandle;
#ifdef WIN32
  winHandle += Ogre::StringConverter::toString((unsigned long)(this->parentWidget()->winId()));
#elif MACOS
  // Mac code, tested on Mac OSX 10.6 using Qt 4.7.4 and Ogre 1.7.3
  winHandle  = Ogre::StringConverter::toString(winId());
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
  window = root->createRenderWindow("QOgreWidget_RenderWindow", this->width(), this->height(), false, &params);
 
  window->setActive(true);
  WId ogreWinId = 0x0;
  window->getCustomAttribute( "WINDOW", &ogreWinId );
 
  assert(ogreWinId);
 
  QRect geo = this->frameGeometry();
  this->create(ogreWinId);
  this->setGeometry(geo);
 
#else
  // code for Mac
  params["externalWindowHandle"] = winHandle;
  params["macAPI"] = "cocoa";
  params["macAPICocoaUseNSView"] = "true";
  window = root->createRenderWindow("QOgreWidget_RenderWindow",
      width(), height(), false, &params);
  window->setActive(true);
  makeCurrent();
#endif
 
  setAttribute(Qt::WA_PaintOnScreen, true);
  setAttribute(Qt::WA_NoBackground);

  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();/////////////////////test
  
  //Preparing World
  world->setRoot(root);
  world->initialise("C:\\Dev\\Nethack2012\\NH2012\\Media\\nh2012.ini");
  if(world) world->hookWindow(window);
}
 
void Renderer::paintGL()
{
  assert(window);
  world->update(0.01);//change to variable frame time
  root->renderOneFrame();
  update();
}
 
void Renderer::resizeGL(int width, int height)
{
  assert(window);
  window->resize(width, height);
  window->windowMovedOrResized();
  paintGL();
}
 
Ogre::RenderSystem* Renderer::chooseRenderer(Ogre::RenderSystemList *renderers)
{
  // It would probably be wise to do something more friendly 
  // that just use the first available renderer
  return *renderers->begin();
}

void Renderer::mousePressEvent(QMouseEvent* e)
{
  // store click position
  lastMousePoint = e->pos();
  // set the flag meaning "click begin"
  hasMouseClick = true;
}

void Renderer::mouseReleaseEvent(QMouseEvent* e)
{
  // check if cursor not moved since click beginning
  if ((hasMouseClick) && (e->pos() == lastMousePoint))
  {
    // do something: for example emit Click signal
    emit mouseClickEvent;
  }
}

