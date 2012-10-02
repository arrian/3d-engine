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
signals:
  void mouseClickEvent();

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

  void init(World* world);
 
protected:
  virtual void initializeGL();
  virtual void resizeGL(int , int );
  virtual void paintGL();

  void mouseReleaseEvent(QMouseEvent* e);
  void mousePressEvent(QMouseEvent* e);	
 
  virtual Ogre::RenderSystem* chooseRenderer(Ogre::RenderSystemList* rsl);
 
  World* world;

  Ogre::Root *root;
  Ogre::RenderWindow *window;

private:
  QPoint lastMousePoint;	
  bool hasMouseClick;
};

#endif // RENDERER_H
