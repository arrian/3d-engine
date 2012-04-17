#include "editor.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Editor w;
    w.show();
    return a.exec();
}
