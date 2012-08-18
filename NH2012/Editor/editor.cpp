#include "editor.h"

Editor::Editor(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags),
      model(0),
      world(new World())
{
  ui.setupUi(this);
  ui.renderer->init(world);

  //Gathering Data
  model = new DataModel(world->getDataManager());
  ui.data->setModel(model);
  ui.data->show();
}

Editor::~Editor()
{

}
