/********************************************************************************
** Form generated from reading UI file 'editor.ui'
**
** Created: Tue 17. Apr 14:02:39 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITOR_H
#define UI_EDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditorClass
{
public:
    QAction *actionOPen;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionData_File;
    QAction *actionCell;
    QAction *actionCell_2;
    QAction *actionData;
    QAction *actionSave_2;
    QAction *actionExit;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCut;
    QAction *actionPaste;
    QAction *actionDuplicate;
    QAction *actionDelete;
    QAction *actionSelect_All;
    QAction *actionSelect_None;
    QAction *actionData_2;
    QAction *actionCell_3;
    QAction *actionRenderer;
    QAction *actionEnvironment;
    QAction *actionNew_Cell;
    QAction *actionNew_Actor;
    QAction *actionEdit_Cell;
    QAction *actionAbout_Editor;
    QAction *actionEnvironment_2;
    QAction *actionCell_4;
    QAction *actionActor;
    QAction *actionNew_3;
    QAction *actionPreferences;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTreeView *cells;
    QTreeView *data;
    QFrame *renderer;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EditorClass)
    {
        if (EditorClass->objectName().isEmpty())
            EditorClass->setObjectName(QString::fromUtf8("EditorClass"));
        EditorClass->resize(974, 755);
        actionOPen = new QAction(EditorClass);
        actionOPen->setObjectName(QString::fromUtf8("actionOPen"));
        actionSave = new QAction(EditorClass);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave_As = new QAction(EditorClass);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        actionData_File = new QAction(EditorClass);
        actionData_File->setObjectName(QString::fromUtf8("actionData_File"));
        actionCell = new QAction(EditorClass);
        actionCell->setObjectName(QString::fromUtf8("actionCell"));
        actionCell_2 = new QAction(EditorClass);
        actionCell_2->setObjectName(QString::fromUtf8("actionCell_2"));
        actionData = new QAction(EditorClass);
        actionData->setObjectName(QString::fromUtf8("actionData"));
        actionSave_2 = new QAction(EditorClass);
        actionSave_2->setObjectName(QString::fromUtf8("actionSave_2"));
        actionExit = new QAction(EditorClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionUndo = new QAction(EditorClass);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        actionRedo = new QAction(EditorClass);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        actionCut = new QAction(EditorClass);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        actionPaste = new QAction(EditorClass);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        actionDuplicate = new QAction(EditorClass);
        actionDuplicate->setObjectName(QString::fromUtf8("actionDuplicate"));
        actionDelete = new QAction(EditorClass);
        actionDelete->setObjectName(QString::fromUtf8("actionDelete"));
        actionSelect_All = new QAction(EditorClass);
        actionSelect_All->setObjectName(QString::fromUtf8("actionSelect_All"));
        actionSelect_None = new QAction(EditorClass);
        actionSelect_None->setObjectName(QString::fromUtf8("actionSelect_None"));
        actionData_2 = new QAction(EditorClass);
        actionData_2->setObjectName(QString::fromUtf8("actionData_2"));
        actionCell_3 = new QAction(EditorClass);
        actionCell_3->setObjectName(QString::fromUtf8("actionCell_3"));
        actionRenderer = new QAction(EditorClass);
        actionRenderer->setObjectName(QString::fromUtf8("actionRenderer"));
        actionEnvironment = new QAction(EditorClass);
        actionEnvironment->setObjectName(QString::fromUtf8("actionEnvironment"));
        actionNew_Cell = new QAction(EditorClass);
        actionNew_Cell->setObjectName(QString::fromUtf8("actionNew_Cell"));
        actionNew_Actor = new QAction(EditorClass);
        actionNew_Actor->setObjectName(QString::fromUtf8("actionNew_Actor"));
        actionEdit_Cell = new QAction(EditorClass);
        actionEdit_Cell->setObjectName(QString::fromUtf8("actionEdit_Cell"));
        actionAbout_Editor = new QAction(EditorClass);
        actionAbout_Editor->setObjectName(QString::fromUtf8("actionAbout_Editor"));
        actionEnvironment_2 = new QAction(EditorClass);
        actionEnvironment_2->setObjectName(QString::fromUtf8("actionEnvironment_2"));
        actionCell_4 = new QAction(EditorClass);
        actionCell_4->setObjectName(QString::fromUtf8("actionCell_4"));
        actionActor = new QAction(EditorClass);
        actionActor->setObjectName(QString::fromUtf8("actionActor"));
        actionNew_3 = new QAction(EditorClass);
        actionNew_3->setObjectName(QString::fromUtf8("actionNew_3"));
        actionPreferences = new QAction(EditorClass);
        actionPreferences->setObjectName(QString::fromUtf8("actionPreferences"));
        centralWidget = new QWidget(EditorClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        cells = new QTreeView(centralWidget);
        cells->setObjectName(QString::fromUtf8("cells"));

        gridLayout->addWidget(cells, 1, 1, 2, 2);

        data = new QTreeView(centralWidget);
        data->setObjectName(QString::fromUtf8("data"));
        data->setMinimumSize(QSize(300, 0));

        gridLayout->addWidget(data, 0, 0, 3, 1);

        renderer = new QFrame(centralWidget);
        renderer->setObjectName(QString::fromUtf8("renderer"));
        renderer->setMinimumSize(QSize(650, 450));
        renderer->setFrameShape(QFrame::StyledPanel);
        renderer->setFrameShadow(QFrame::Raised);

        gridLayout->addWidget(renderer, 0, 1, 1, 2);

        EditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(EditorClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 974, 26));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        EditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(EditorClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        EditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(EditorClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        EditorClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionData);
        menuFile->addAction(actionSave_2);
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionPaste);
        menuEdit->addAction(actionDuplicate);
        menuEdit->addAction(actionDelete);
        menuEdit->addSeparator();
        menuEdit->addAction(actionSelect_All);
        menuEdit->addAction(actionSelect_None);
        menuEdit->addSeparator();
        menuEdit->addAction(actionEnvironment_2);
        menuEdit->addAction(actionPreferences);
        menuView->addAction(actionData_2);
        menuView->addAction(actionCell_3);
        menuView->addAction(actionRenderer);
        menuHelp->addAction(actionAbout_Editor);

        retranslateUi(EditorClass);

        QMetaObject::connectSlotsByName(EditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *EditorClass)
    {
        EditorClass->setWindowTitle(QApplication::translate("EditorClass", "Editor", 0, QApplication::UnicodeUTF8));
        actionOPen->setText(QApplication::translate("EditorClass", "Open", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("EditorClass", "Save", 0, QApplication::UnicodeUTF8));
        actionSave_As->setText(QApplication::translate("EditorClass", "Save As", 0, QApplication::UnicodeUTF8));
        actionData_File->setText(QApplication::translate("EditorClass", "Data File", 0, QApplication::UnicodeUTF8));
        actionCell->setText(QApplication::translate("EditorClass", "World", 0, QApplication::UnicodeUTF8));
        actionCell_2->setText(QApplication::translate("EditorClass", "Cell", 0, QApplication::UnicodeUTF8));
        actionData->setText(QApplication::translate("EditorClass", "Data", 0, QApplication::UnicodeUTF8));
        actionSave_2->setText(QApplication::translate("EditorClass", "Save", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("EditorClass", "Exit", 0, QApplication::UnicodeUTF8));
        actionUndo->setText(QApplication::translate("EditorClass", "Undo", 0, QApplication::UnicodeUTF8));
        actionRedo->setText(QApplication::translate("EditorClass", "Redo", 0, QApplication::UnicodeUTF8));
        actionCut->setText(QApplication::translate("EditorClass", "Cut", 0, QApplication::UnicodeUTF8));
        actionPaste->setText(QApplication::translate("EditorClass", "Paste", 0, QApplication::UnicodeUTF8));
        actionDuplicate->setText(QApplication::translate("EditorClass", "Instance", 0, QApplication::UnicodeUTF8));
        actionDelete->setText(QApplication::translate("EditorClass", "Delete", 0, QApplication::UnicodeUTF8));
        actionSelect_All->setText(QApplication::translate("EditorClass", "Select All", 0, QApplication::UnicodeUTF8));
        actionSelect_None->setText(QApplication::translate("EditorClass", "Select None", 0, QApplication::UnicodeUTF8));
        actionData_2->setText(QApplication::translate("EditorClass", "Data", 0, QApplication::UnicodeUTF8));
        actionCell_3->setText(QApplication::translate("EditorClass", "Cells", 0, QApplication::UnicodeUTF8));
        actionRenderer->setText(QApplication::translate("EditorClass", "Renderer", 0, QApplication::UnicodeUTF8));
        actionEnvironment->setText(QApplication::translate("EditorClass", "Environment", 0, QApplication::UnicodeUTF8));
        actionNew_Cell->setText(QApplication::translate("EditorClass", "New Cell", 0, QApplication::UnicodeUTF8));
        actionNew_Actor->setText(QApplication::translate("EditorClass", "New Actor", 0, QApplication::UnicodeUTF8));
        actionEdit_Cell->setText(QApplication::translate("EditorClass", "Edit Cell", 0, QApplication::UnicodeUTF8));
        actionAbout_Editor->setText(QApplication::translate("EditorClass", "About", 0, QApplication::UnicodeUTF8));
        actionEnvironment_2->setText(QApplication::translate("EditorClass", "Environment", 0, QApplication::UnicodeUTF8));
        actionCell_4->setText(QApplication::translate("EditorClass", "Cell", 0, QApplication::UnicodeUTF8));
        actionActor->setText(QApplication::translate("EditorClass", "Actor", 0, QApplication::UnicodeUTF8));
        actionNew_3->setText(QApplication::translate("EditorClass", "New", 0, QApplication::UnicodeUTF8));
        actionPreferences->setText(QApplication::translate("EditorClass", "Preferences", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("EditorClass", "File", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("EditorClass", "Edit", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("EditorClass", "View", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("EditorClass", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EditorClass: public Ui_EditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITOR_H
