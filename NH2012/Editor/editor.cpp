#include "editor.h"

#include <QtGui/QApplication>

Editor::Editor(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
  ui.setupUi(this);
  ui.renderer->init();
 
  //Gathering Data
  //model = new DataModel(world->getDataManager());
  //ui.data->setModel(model);
  //ui.data->show();
}

Editor::~Editor()
{

}

void Editor::keyPressEvent(QKeyEvent* e) 
{
  //send event to renderer
}

void Editor::keyReleaseEvent(QKeyEvent* e) 
{
  //send event to renderer
}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Editor w;
  w.show();
  return a.exec();
}
