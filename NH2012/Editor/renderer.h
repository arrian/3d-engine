#ifndef RENDERER_H
#define RENDERER_H

#include <Ogre.h>
#include <QGLWidget>
#include <QX11Info>

#include <OgreException.h>
#include "../../Engine/World.h"
 
class Renderer : public QGLWidget
{
  //Q_OBJECT;
 
 public:
  Renderer(QWidget *parent = 0)
    : QGLWidget(parent),
      window(NULL),
      world(0)
  {
  }
 
  virtual ~Renderer()
  {
    root->shutdown();
    delete root;
    destroy();
  }

  void init(World* world, std::string plugins);
 
 protected:
  virtual void initializeGL();
  virtual void resizeGL(int , int );
  virtual void paintGL();
 
  virtual Ogre::RenderSystem* chooseRenderer(Ogre::RenderSystemList* rsl);
 
  World* world;

  Ogre::Root *root;
  Ogre::RenderWindow *window;
};

#endif // RENDERER_H
