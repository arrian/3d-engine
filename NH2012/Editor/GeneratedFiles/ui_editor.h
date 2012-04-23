/********************************************************************************
** Form generated from reading UI file 'editor.ui'
**
** Created: Mon 23. Apr 15:10:46 2012
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
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>
#include "renderer.h"

QT_BEGIN_NAMESPACE

class Ui_EditorClass
{
public:
    QAction *actionSave;
    QAction *actionData;
    QAction *actionExit;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCut;
    QAction *actionPaste;
    QAction *actionDuplicate;
    QAction *actionDelete;
    QAction *actionSelect_All;
    QAction *actionSelect_None;
    QAction *actionRenderer;
    QAction *actionEnvironment;
    QAction *actionCell_View;
    QAction *actionPreferences;
    QAction *actionData_View;
    QAction *actionAbout;
    QAction *actionCopy;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTreeView *cells;
    QTreeView *data;
    Renderer *renderer;
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
        EditorClass->resize(1000, 750);
        actionSave = new QAction(EditorClass);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionData = new QAction(EditorClass);
        actionData->setObjectName(QString::fromUtf8("actionData"));
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
        actionRenderer = new QAction(EditorClass);
        actionRenderer->setObjectName(QString::fromUtf8("actionRenderer"));
        actionRenderer->setCheckable(true);
        actionRenderer->setChecked(true);
        actionEnvironment = new QAction(EditorClass);
        actionEnvironment->setObjectName(QString::fromUtf8("actionEnvironment"));
        actionCell_View = new QAction(EditorClass);
        actionCell_View->setObjectName(QString::fromUtf8("actionCell_View"));
        actionCell_View->setCheckable(true);
        actionCell_View->setChecked(true);
        actionPreferences = new QAction(EditorClass);
        actionPreferences->setObjectName(QString::fromUtf8("actionPreferences"));
        actionData_View = new QAction(EditorClass);
        actionData_View->setObjectName(QString::fromUtf8("actionData_View"));
        actionData_View->setCheckable(true);
        actionData_View->setChecked(true);
        actionAbout = new QAction(EditorClass);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionCopy = new QAction(EditorClass);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        centralWidget = new QWidget(EditorClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(3);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        cells = new QTreeView(centralWidget);
        cells->setObjectName(QString::fromUtf8("cells"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cells->sizePolicy().hasHeightForWidth());
        cells->setSizePolicy(sizePolicy);

        gridLayout->addWidget(cells, 1, 1, 2, 2);

        data = new QTreeView(centralWidget);
        data->setObjectName(QString::fromUtf8("data"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(data->sizePolicy().hasHeightForWidth());
        data->setSizePolicy(sizePolicy1);
        data->setMinimumSize(QSize(300, 0));

        gridLayout->addWidget(data, 0, 0, 3, 1);

        renderer = new Renderer(centralWidget);
        renderer->setObjectName(QString::fromUtf8("renderer"));
        sizePolicy.setHeightForWidth(renderer->sizePolicy().hasHeightForWidth());
        renderer->setSizePolicy(sizePolicy);
        renderer->setMinimumSize(QSize(650, 450));
        renderer->setAcceptDrops(true);

        gridLayout->addWidget(renderer, 0, 1, 1, 2);

        EditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(EditorClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1000, 26));
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
        QWidget::setTabOrder(data, cells);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionData);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionPaste);
        menuEdit->addAction(actionDuplicate);
        menuEdit->addAction(actionDelete);
        menuEdit->addSeparator();
        menuEdit->addAction(actionSelect_All);
        menuEdit->addAction(actionSelect_None);
        menuEdit->addSeparator();
        menuEdit->addAction(actionEnvironment);
        menuEdit->addAction(actionPreferences);
        menuView->addAction(actionRenderer);
        menuView->addAction(actionCell_View);
        menuView->addAction(actionData_View);
        menuHelp->addAction(actionAbout);

        retranslateUi(EditorClass);

        QMetaObject::connectSlotsByName(EditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *EditorClass)
    {
        EditorClass->setWindowTitle(QApplication::translate("EditorClass", "Editor", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("EditorClass", "Save", 0, QApplication::UnicodeUTF8));
        actionSave->setShortcut(QApplication::translate("EditorClass", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionData->setText(QApplication::translate("EditorClass", "Data", 0, QApplication::UnicodeUTF8));
        actionData->setShortcut(QApplication::translate("EditorClass", "Ctrl+D", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("EditorClass", "Exit", 0, QApplication::UnicodeUTF8));
        actionUndo->setText(QApplication::translate("EditorClass", "Undo", 0, QApplication::UnicodeUTF8));
        actionUndo->setShortcut(QApplication::translate("EditorClass", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        actionRedo->setText(QApplication::translate("EditorClass", "Redo", 0, QApplication::UnicodeUTF8));
        actionRedo->setShortcut(QApplication::translate("EditorClass", "Ctrl+Y", 0, QApplication::UnicodeUTF8));
        actionCut->setText(QApplication::translate("EditorClass", "Cut", 0, QApplication::UnicodeUTF8));
        actionCut->setShortcut(QApplication::translate("EditorClass", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        actionPaste->setText(QApplication::translate("EditorClass", "Paste", 0, QApplication::UnicodeUTF8));
        actionPaste->setShortcut(QApplication::translate("EditorClass", "Ctrl+V", 0, QApplication::UnicodeUTF8));
        actionDuplicate->setText(QApplication::translate("EditorClass", "Instance", 0, QApplication::UnicodeUTF8));
        actionDuplicate->setShortcut(QApplication::translate("EditorClass", "Ctrl+I", 0, QApplication::UnicodeUTF8));
        actionDelete->setText(QApplication::translate("EditorClass", "Delete", 0, QApplication::UnicodeUTF8));
        actionDelete->setShortcut(QApplication::translate("EditorClass", "Del", 0, QApplication::UnicodeUTF8));
        actionSelect_All->setText(QApplication::translate("EditorClass", "Select All", 0, QApplication::UnicodeUTF8));
        actionSelect_All->setShortcut(QApplication::translate("EditorClass", "Ctrl+A", 0, QApplication::UnicodeUTF8));
        actionSelect_None->setText(QApplication::translate("EditorClass", "Select None", 0, QApplication::UnicodeUTF8));
        actionSelect_None->setShortcut(QApplication::translate("EditorClass", "Ctrl+Shift+A", 0, QApplication::UnicodeUTF8));
        actionRenderer->setText(QApplication::translate("EditorClass", "Renderer", 0, QApplication::UnicodeUTF8));
        actionRenderer->setShortcut(QApplication::translate("EditorClass", "Ctrl+1", 0, QApplication::UnicodeUTF8));
        actionEnvironment->setText(QApplication::translate("EditorClass", "Environment", 0, QApplication::UnicodeUTF8));
        actionEnvironment->setShortcut(QApplication::translate("EditorClass", "Ctrl+E", 0, QApplication::UnicodeUTF8));
        actionCell_View->setText(QApplication::translate("EditorClass", "Cell View", 0, QApplication::UnicodeUTF8));
        actionCell_View->setShortcut(QApplication::translate("EditorClass", "Ctrl+2", 0, QApplication::UnicodeUTF8));
        actionPreferences->setText(QApplication::translate("EditorClass", "Preferences", 0, QApplication::UnicodeUTF8));
        actionPreferences->setShortcut(QApplication::translate("EditorClass", "Ctrl+P", 0, QApplication::UnicodeUTF8));
        actionData_View->setText(QApplication::translate("EditorClass", "Data View", 0, QApplication::UnicodeUTF8));
        actionData_View->setShortcut(QApplication::translate("EditorClass", "Ctrl+3", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("EditorClass", "About", 0, QApplication::UnicodeUTF8));
        actionAbout->setShortcut(QApplication::translate("EditorClass", "F1", 0, QApplication::UnicodeUTF8));
        actionCopy->setText(QApplication::translate("EditorClass", "Copy", 0, QApplication::UnicodeUTF8));
        actionCopy->setShortcut(QApplication::translate("EditorClass", "Ctrl+C", 0, QApplication::UnicodeUTF8));
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
