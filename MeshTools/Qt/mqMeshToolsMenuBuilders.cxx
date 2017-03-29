/*=========================================================================

   Program: MeshTools
   Module:    mqMeshToolsMenuBuilders.cxx

========================================================================*/
#include "mqMeshToolsCore.h"
#include "mqAboutDialogReaction.h"
#include "mqChangeNodeReaction.h"
#include "mqEditAllFLGDialogReaction.h"
#include "mqSavePLYDialogReaction.h"
#include "mqSaveVTPDialogReaction.h"
#include "mqSaveSTLDialogReaction.h"
#include "mqColorDialogReaction.h"
#include "mqLandmarkDialogReaction.h"
#include "mqMeshToolsMenuBuilders.h"
#include "mqMainControlsToolbar.h"
#include "mqLightControlsToolbar.h"
#include "mqDesktopServicesReaction.h"
#include "mqOpenDataReaction.h"
#include "mqSaveDataReaction.h"
#include "mqSaveLandmarksDialogReaction.h"
#include "mqSaveFlagsDialogReaction.h"
#include "mqSaveCURDialogReaction.h"
#include "mqSaveCURasVERDialogReaction.h"
#include "mqSaveSTVDialogReaction.h"
#include "mqSaveNTWDialogReaction.h"
#include "mqSelectLandmarkRangeDialogReaction.h"
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
  QMenu* submenuProject = menu.addMenu("Project");
  QMenu* submenuSurface = menu.addMenu("Surface");

  QMenu* submenuPosition = menu.addMenu("Position");
  QMenu* submenuLandmark = menu.addMenu("Landmarks");
  QMenu* submenuCurves = menu.addMenu("Curves");
  
  QMenu* submenuTagsAndFlags = menu.addMenu("Tags and Flags");
  //QMenu* submenuFileInfos = menu.addMenu("Save infos (surface, area, volume)");
  QMenu* submenuOrientationLabels = menu.addMenu("Orientation helper labels");
  new mqOpenDataReaction(submenuProject->addAction("Open Project") << mqSetName("actionOpenNTW"), 1);
  new mqSaveNTWDialogReaction(submenuProject->addAction("Save Project") << mqSetName("actionSaveNTW"));
  
  new mqOpenDataReaction(submenuSurface->addAction("Open surface") << mqSetName("actionOpenMesh"), 2);
  new mqSavePLYDialogReaction(submenuSurface->addAction("Save selected surfaces in one single .PLY file") << mqSetName("actionSavePLY"));
  new mqSaveVTPDialogReaction(submenuSurface->addAction("Save selected surfaces in one single VTK PolyData (.VTP) file") << mqSetName("actionSaveVTP"));
  new mqSaveSTLDialogReaction(submenuSurface->addAction("Save selected surfaces in one single .STL file") << mqSetName("actionSaveSTL"));
  
  
  new mqOpenDataReaction(submenuLandmark->addAction("Open MeshTools Landmark/Curve file (STV)") << mqSetName("actionOpenSTV"), 16);
  new mqOpenDataReaction(submenuLandmark->addAction("Open Landmarks") << mqSetName("actionOpenNormalLMK"), 3);
  new mqOpenDataReaction(submenuLandmark->addAction("Open Target Landmarks") << mqSetName("actionOpenNormalLMK"), 4);
  
  new mqSaveSTVDialogReaction(submenuLandmark->addAction("Save MeshTools Landmark/Curve file (STV)") << mqSetName("actionSaveSTV"));
  new mqSaveLandmarksDialogReaction(submenuLandmark->addAction("Save Normal Landmarks") << mqSetName("actionSaveNormalLMK"), 0);
  new mqSaveLandmarksDialogReaction(submenuLandmark->addAction("Save Target Landmarks") << mqSetName("actionSaveTargetLMK"), 1);
  
  
  
  new mqOpenDataReaction(submenuCurves->addAction("Open Curve (.CUR)") << mqSetName("actionOpenCUR"), 5);
  new mqOpenDataReaction(submenuCurves->addAction("Open MeshTools Landmark/Curve file (STV)") << mqSetName("actionOpenSTV2"), 16);
  new mqOpenDataReaction(submenuCurves->addAction("Open Curve Node Landmarks") << mqSetName("actionOpenNodeLMK"), 14);
  new mqOpenDataReaction(submenuCurves->addAction("Open Curve Handle Landmarks") << mqSetName("actionOpenHandleLMK"), 15);
  new mqSaveCURDialogReaction(submenuCurves->addAction("Save .CUR File ") << mqSetName("actionSaveCURLMK"));
  new mqSaveSTVDialogReaction(submenuCurves->addAction("Save MeshTools Landmark/Curve file (STV)") << mqSetName("actionSaveSTV2"));
  new mqSaveLandmarksDialogReaction(submenuCurves->addAction("Save Curve Nodes Landmarks") << mqSetName("actionSaveNodeLMK"), 2);
  new mqSaveLandmarksDialogReaction(submenuCurves->addAction("Save Curve Handle Landmarks") << mqSetName("actionSaveHandleLMK"), 3);
  new mqSaveCURasVERDialogReaction(submenuCurves->addAction("Export curve segments as landmark file") << mqSetName("actionExportCUR"));
  new mqSaveDataReaction(submenuCurves->addAction("Save curve infos (length per curve segment)") << mqSetName("actionSaveCURInfos"), 17);
  

  new mqOpenDataReaction(submenuPosition->addAction("Open position for selected surfaces") << mqSetName("actionOpenPOS"), 8);
  new mqOpenDataReaction(submenuPosition->addAction("Open transposed position for selected surfaces") << mqSetName("actionOpenPOS2"), 10);
  new mqOpenDataReaction(submenuPosition->addAction("Open position for selected landmarks") << mqSetName("actionOpenPOS3"), 11);
  new mqOpenDataReaction(submenuPosition->addAction("Open transposed position for selected landmarks") << mqSetName("actionOpenPOS4"), 12);
  new mqSaveDataReaction(submenuPosition->addAction("Save position for selected surface") << mqSetName("actionSavePOS"), 8);

  new mqOpenDataReaction(submenuTagsAndFlags->addAction("Open Flag") << mqSetName("actionOpenFLG"), 6);
  new mqSaveFlagsDialogReaction(submenuTagsAndFlags->addAction("SaveFlags") << mqSetName("actionSaveFLG"));
  new mqOpenDataReaction(submenuOrientationLabels->addAction("Open Orientation Labels") << mqSetName("actionOpenORI"), 7);
  new mqSaveDataReaction(submenuOrientationLabels->addAction("Save Orientation Labels") << mqSetName("actionSaveORI"), 7);
  new mqOpenDataReaction(submenuTagsAndFlags->addAction("Open Tag") << mqSetName("actionOpenTAG"), 9);

  
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

void mqMeshToolsMenuBuilders::buildLandmarksMenu(QMenu& menu)
{
	QString objectName = menu.objectName();
	std::cout << "Menu object name" << objectName.toStdString() << std::endl;
	//Ui::mqFileMenuBuilder ui;// we do not have a ui file yet for the menu!
	//ui.setupUi(&menu);
	// since the UI file tends to change the name of the menu.
	menu.setObjectName(objectName);
	// QAction *openNtw = menu.addAction("Open Project");

	//new mqOpenDataReaction(openNtw, 1);//1= open NTW 
	new mqSelectLandmarkRangeDialogReaction(menu.addAction("Select landmark range") << mqSetName("actionSelectLandmarkRange"));
	QAction *MoveUp = menu.addAction("Selected landmarks : move up (decrease landmark number)");
	QAction *MoveDown = menu.addAction("Selected landmarks move down (increase landmark number)");
	
	QAction::connect(MoveUp, SIGNAL(triggered()), mqMeshToolsCore::instance() , SLOT(slotLandmarkMoveUp()));
	QAction::connect(MoveDown, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotLandmarkMoveDown()));
	//actionLandmarskMoveUp


	QMenu* submenuLandmarksInvolved = menu.addMenu("Selected node and handle landmarks");
	new mqEditAllFLGDialogReaction(menu.addAction("Edit all selected flags") << mqSetName("actionEditAllSelectedFlags"));
	
	new mqChangeNodeReaction(submenuLandmarksInvolved->addAction("Move curve handles semi-automatically") << mqSetName("actionMoveHandles"), 4);
	new mqChangeNodeReaction(submenuLandmarksInvolved->addAction("Normal nodes (red): change as starting nodes (dark red)") << mqSetName("actionStartingNode"), 1);
	new mqChangeNodeReaction(submenuLandmarksInvolved->addAction("Normal nodes (red): connect to preceding starting nodes (cyan)") << mqSetName("actionConnectNode"), 3);
	new mqChangeNodeReaction(submenuLandmarksInvolved->addAction("Normal nodes (red): define as milestone nodes (blue)") << mqSetName("actionMilestoneNode"), 2);
	new mqChangeNodeReaction(submenuLandmarksInvolved->addAction("Reset selected nodes to Normal nodes (red)") << mqSetName("actionNormalNode"), 0);
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

	QToolBar* lightToolBar = new mqLightControlsToolbar(&mainWindow)
		<< mqSetName("LightControlsToolbar");
	lightToolBar->layout()->setSpacing(0);
	mainWindow.addToolBar(Qt::BottomToolBarArea, lightToolBar);



}
