#ifndef RENDERER_H
#define RENDERER_H

#include <Ogre.h>
#include <QGLWidget>
#include <QX11Info>

#include "../../Engine/World.h"
 
class Renderer : public QGLWidget
{
  //Q_OBJECT;
 
 public:
  Renderer( QWidget *parent=0 ):
    QGLWidget( parent ),
    mOgreWindow(NULL)
    {
      init("plugins_d.cfg");
    }
 
  virtual ~Renderer()
    {
      mOgreRoot->shutdown();
      delete mOgreRoot;
      destroy();
    }
 
 protected:
  virtual void initializeGL();
  virtual void resizeGL(int , int );
  virtual void paintGL();
 
  void init(std::string plugins);
 
  virtual Ogre::RenderSystem* chooseRenderer(Ogre::RenderSystemList* rsl);
 
  Ogre::Root *mOgreRoot;
  Ogre::RenderWindow *mOgreWindow;
  Ogre::Camera *mCamera;
  Ogre::Viewport *mViewport;
  Ogre::SceneManager *mSceneMgr;
};

#endif // RENDERER_H
