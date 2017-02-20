/*=========================================================================

   Program: MeshTools
   Module:    mqMeshToolsMenuBuilders.cxx

========================================================================*/
#include "mqMeshToolsCore.h"
#include "mqAboutDialogReaction.h"
#include "mqColorDialogReaction.h"
#include "mqLandmarkDialogReaction.h"
#include "mqMeshToolsMenuBuilders.h"
#include "mqMainControlsToolbar.h"
#include "mqDesktopServicesReaction.h"
#include "mqOpenDataReaction.h"

#include "mqSetName.h"

//#include "ui_mqEditMenuBuilder.h" // no .ui Edit menu file yet
//#include "ui_mqFileMenuBuilder.h" // no .ui File menu yet


//#include "pqAboutDialogReaction.h"

#include <iostream>
#include <QDesktopServices>
#include <QUrl>
#include <QDockWidget>
#include <QFileInfo>
#include <QKeySequence>
#include <QLayout>
#include <QMainWindow>
#include <QMenu>

//-----------------------------------------------------------------------------
//void mqMeshToolsMenuBuilders::buildFileMenu(QMenu& menu, QMainWindow* mainWindow = 0)
void mqMeshToolsMenuBuilders::buildFileMenu(QMenu& menu)
{
  QString objectName = menu.objectName();
  std::cout << "Menu object name" << objectName.toStdString() << std::endl;
  //Ui::mqFileMenuBuilder ui;// we do not have a ui file yet for the menu!
  //ui.setupUi(&menu);
  // since the UI file tends to change the name of the menu.
  menu.setObjectName(objectName);
 // QAction *openNtw = menu.addAction("Open Project");
  //new mqOpenDataReaction(openNtw, 1);//1= open NTW 

  new mqOpenDataReaction(menu.addAction("Open Mesh") << mqSetName("actionOpenMesh"), 2);
  new mqOpenDataReaction(menu.addAction("Open Project") << mqSetName("actionOpenNTW"), 1);
  new mqOpenDataReaction(menu.addAction("Open Landmark") << mqSetName("actionOpenNormalLMK"), 3);
  new mqOpenDataReaction(menu.addAction("Open Target Landmark") << mqSetName("actionOpenNormalLMK"), 4);
  new mqOpenDataReaction(menu.addAction("Open Curve") << mqSetName("actionOpenCUR"), 5);
  new mqOpenDataReaction(menu.addAction("Open Position") << mqSetName("actionOpenPOS"), 8);
  new mqOpenDataReaction(menu.addAction("Open Flag") << mqSetName("actionOpenFLG"), 6);
  new mqOpenDataReaction(menu.addAction("Open Orientation") << mqSetName("actionOpenORI"), 7);
  new mqOpenDataReaction(menu.addAction("Open Tag") << mqSetName("actionOpenTAG"), 9);

  
}

//-----------------------------------------------------------------------------
//void mqMeshToolsMenuBuilders::buildEditMenu(QMenu& menu, QMainWindow* mainWindow = 0)
void mqMeshToolsMenuBuilders::buildEditMenu(QMenu& menu)
{
	// About

  QString objectName = menu.objectName();
 /* Ui::pqEditMenuBuilder ui;
  ui.setupUi(&menu);*/ // no ui file yet

  // since the UI file tends to change the name of the menu.
  menu.setObjectName(objectName);
  new mqColorDialogReaction(menu.addAction("Edit color options") << mqSetName("actionColor"));
  new mqLandmarkDialogReaction(menu.addAction("Edit landmark and flag rendering options") << mqSetName("actionLandmark"));
  //new pqUndoRedoReaction(ui.actionEditUndo, true);
  //new pqUndoRedoReaction(ui.actionEditRedo, false);
  //new pqCameraUndoRedoReaction(ui.actionEditCameraUndo, true);
 // new pqCameraUndoRedoReaction(ui.actionEditCameraRedo, false);
  
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//void mqMeshToolsMenuBuilders::buildHelpMenu(QMenu& menu, QMainWindow* mainWindow = 0)
void mqMeshToolsMenuBuilders::buildHelpMenu(QMenu& menu)
{
	
	// MeshTools Web Site
	new mqDesktopServicesReaction(QUrl("http://www.morphomuseum.com"),
		(menu.addAction("MeshTools Web Site") << mqSetName("actionWebSite")));




	// MeshTools tutorials
	new mqDesktopServicesReaction(QUrl("http://www.morphomuseum.com/tutorialsMeshtools"),
		(menu.addAction("MeshTools Tutorials") << mqSetName("actionMeshToolsTutorial")));



  

  // ParaView Release Notes
  

  // About
  new mqAboutDialogReaction(menu.addAction("About...") << mqSetName("actionAbout"));
}

//-----------------------------------------------------------------------------
void mqMeshToolsMenuBuilders::buildToolbars(QMainWindow& mainWindow)
{
	QToolBar* mainToolBar = new mqMainControlsToolbar(&mainWindow)
		<< mqSetName("MainControlsToolbar");
	mainToolBar->layout()->setSpacing(0);
	mainWindow.addToolBar(Qt::TopToolBarArea, mainToolBar);

	


}

