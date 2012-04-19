#include "editor.h"

Editor::Editor(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
  ui.setupUi(this);

  QFileSystemModel model;
  model.setRootPath("");

  //DataWrapper gameData(0);
  ui.data->setModel(&model);

  ui.data->setAnimated(false);
  ui.data->setIndentation(20);
  ui.data->setSortingEnabled(true);

  ui.data->setWindowTitle(QObject::tr("Data"));
  ui.data->resize(640, 480);

  ui.data->show();
}

Editor::~Editor()
{

}
