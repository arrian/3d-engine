#ifndef EDITOR_H
#define EDITOR_H

#include <QtGui/QMainWindow>
#include <QFileSystemModel>
#include "ui_editor.h"
#include "datamodel.h"

#include "../../Engine/World.h"

class Editor : public QMainWindow
{
    Q_OBJECT

public:
  Editor(QWidget *parent = 0, Qt::WFlags flags = 0);
  ~Editor();

private:
  World* world;

  Ui::EditorClass ui;

  DataModel model;
};

#endif // EDITOR_H
