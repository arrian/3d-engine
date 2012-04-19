#ifndef EDITOR_H
#define EDITOR_H

#include <QtGui/QMainWindow>
#include <QFileSystemModel>
#include "ui_editor.h"
#include "datawrapper.h"

class Editor : public QMainWindow
{
    Q_OBJECT

public:
    Editor(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~Editor();

private:
    Ui::EditorClass ui;

    
};

#endif // EDITOR_H
