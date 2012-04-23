#include "editor.h"

Editor::Editor(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags),
      model(DataModel("dummy data")),
      world(0)
{
  ui.setupUi(this);
  ui.renderer->init(world, "plugins_d.cfg");
  ui.data->setModel(&model);
  ui.data->show();
}

Editor::~Editor()
{

}
