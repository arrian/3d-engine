#ifndef RENDERER_H
#define RENDERER_H

#include <Ogre.h>
#include <QGLWidget>
#include <QX11Info>

#include <OgreException.h>
#include "../../Engine/World.h"

#include <QWidget>
#include <QMouseEvent>
 
class Renderer : public QGLWidget
{
  Q_OBJECT
//signals:
//  void mouseClickEvent();

public:
  Renderer(QWidget *parent = 0)
    : QGLWidget(parent),
      window(NULL),
      world(NULL),
      hasMouseClick(false)
  {
  }
 
  virtual ~Renderer()
  {
    delete world;
    root->shutdown();
    delete root;
    destroy();
  }

  void init();
 
protected:
  virtual void initializeGL();
  virtual void resizeGL(int , int );
  virtual void paintGL();

  virtual void mouseReleaseEvent(QMouseEvent* e);
  virtual void mousePressEvent(QMouseEvent* e);	
  virtual void mouseMoveEvent(QMouseEvent* e);
  virtual void keyPressEvent(QKeyEvent* e);
  virtual void keyReleaseEvent(QKeyEvent* e);
  //virtual void wheelEvent(QWheelEvent* e);
 
  virtual Ogre::RenderSystem* chooseRenderer(Ogre::RenderSystemList* rsl);
 
  World* world;

  Ogre::Root *root;
  Ogre::RenderWindow *window;

private:
  bool hasMouseClick;
};

#endif // RENDERER_H
