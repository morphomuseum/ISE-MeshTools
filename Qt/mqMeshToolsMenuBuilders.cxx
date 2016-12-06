/*=========================================================================

   Program: MeshTools
   Module:    mqMeshToolsMenuBuilders.cxx

========================================================================*/

#include "mqAboutDialogReaction.h"
#include "mqMeshToolsMenuBuilders.h"
#include "mqDesktopServicesReaction.h"
#include "mqSetName.h"

//#include "ui_mdEditMenuBuilder.h" // no .ui Edit menu file yet
//#include "ui_mqFileMenuBuilder.h" // no .ui File menu yet


//#include "pqAboutDialogReaction.h"


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
  //Ui::mqFileMenuBuilder ui; we do not have a ui file yet for the menu!
  //ui.setupUi(&menu);
  // since the UI file tends to change the name of the menu.
  menu.setObjectName(objectName);
  QAction *openNtw = menu.addAction("Open Network");
  /*QObject::connect(
    ui.actionFileExit, SIGNAL(triggered()), pqApplicationCore::instance(), SLOT(quit()));*/

  // now setup reactions.
  /*new pqLoadDataReaction(ui.actionFileOpen);*/

  //new pqRecentFilesMenu(*ui.menuRecentFiles, ui.menuRecentFiles);

  
}

//-----------------------------------------------------------------------------
//void mqMeshToolsMenuBuilders::buildEditMenu(QMenu& menu, QMainWindow* mainWindow = 0)
void mqMeshToolsMenuBuilders::buildEditMenu(QMenu& menu)
{
  QString objectName = menu.objectName();
 /* Ui::pqEditMenuBuilder ui;
  ui.setupUi(&menu);*/ // no ui file yet

  // since the UI file tends to change the name of the menu.
  menu.setObjectName(objectName);

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

