/*=========================================================================

   Program: MeshTools
   Module:    mqMeshToolsMenuBuilders.cxx

========================================================================*/
#include "mqMeshToolsCore.h"
#include "mqAboutDialogReaction.h"
#include "mqChangeNodeReaction.h"
#include "mqEditAllFLGLengthDialogReaction.h"
#include "mqEditAllFLGColorDialogReaction.h"
#include "mqSavePLYDialogReaction.h"
#include "mqSaveVTPDialogReaction.h"
#include "mqSaveSTLDialogReaction.h"
#include "mqColorDialogReaction.h"
#include "mqLandmarkDialogReaction.h"
#include "mqMeshToolsMenuBuilders.h"
#include "mqCameraControlsToolbar.h"
#include "mqMainControlsToolbar.h"
#include "mqLightControlsToolbar.h"
#include "mqObjectsControlsToolbar.h"
#include "mqScalarsControlsToolbar.h"
#include "mqDesktopServicesReaction.h"
#include "mqOpenDataReaction.h"
#include "mqSaveDataReaction.h"
#include "mqSaveLandmarksDialogReaction.h"
#include "mqSaveFlagsDialogReaction.h"
#include "mqSaveCURDialogReaction.h"
#include "mqGridSizeDialogReaction.h"
#include "mqEditAlphaDialogReaction.h"
#include "mqSaveCURasVERDialogReaction.h"
#include "mqOrientationLabelsDialogReaction.h"
#include "mqSaveSTVDialogReaction.h"
#include "mqSaveNTWDialogReaction.h"
#include "mqSelectLandmarkRangeDialogReaction.h"
#include "mqSmoothDialogReaction.h"
#include "mqDecimateDialogReaction.h"
#include "mqDensifyDialogReaction.h"
#include "mqFillHolesDialogReaction.h"
#include "mqTPSDialogReaction.h"
#include "mqDecomposeDialogReaction.h"
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
#include <QPalette>
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
  QMenu* submenuMeasurements = menu.addMenu("Measurements");
  new mqOpenDataReaction(submenuProject->addAction("Open Project") << mqSetName("actionOpenNTW"), 1);
  new mqSaveNTWDialogReaction(submenuProject->addAction("Save Project") << mqSetName("actionSaveNTW"));
  
  new mqOpenDataReaction(submenuSurface->addAction("Open surface") << mqSetName("actionOpenMesh"), 2);
  new mqSavePLYDialogReaction(submenuSurface->addAction("Save selected surfaces in one single .PLY file") << mqSetName("actionSavePLY"));
  new mqSaveVTPDialogReaction(submenuSurface->addAction("Save selected surfaces in one single VTK PolyData (.VTK or .VTP) file") << mqSetName("actionSaveVTP"));
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
  
  new mqSaveDataReaction(submenuMeasurements->addAction("Save area and volume of selected surfaces") << mqSetName("actionSaveAV"), 18);
  new mqSaveDataReaction(submenuMeasurements->addAction("Save normalized shape index of selected surfaces") << mqSetName("actionSaveNSI"), 19);  
  new mqSaveDataReaction(submenuMeasurements->addAction("Save convex hull area ratio and normalized shape index of selected surfaces (warning: slow)") << mqSetName("actionSaveCHNSI"), 20);

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
  new mqGridSizeDialogReaction(menu.addAction("Edit size unit and grid spacing") << mqSetName("actionGridSize"));
  new mqLandmarkDialogReaction(menu.addAction("Edit landmark and flag rendering options") << mqSetName("actionLandmark"));
  new mqOrientationLabelsDialogReaction(menu.addAction("Edit orientation labels") << mqSetName("actionOrientationLabels"));
  //new pqUndoRedoReaction(ui.actionEditUndo, true);
  //new pqUndoRedoReaction(ui.actionEditRedo, false);
  //new pqCameraUndoRedoReaction(ui.actionEditCameraUndo, true);
 // new pqCameraUndoRedoReaction(ui.actionEditCameraRedo, false);
  
}

void mqMeshToolsMenuBuilders::buildEditSelectedSurfacesMenu(QMenu& menu)
{
	QString objectName = menu.objectName();
	std::cout << "Menu object name" << objectName.toStdString() << std::endl;
	menu.setObjectName(objectName);
	QMenu* submenuStructureModification = menu.addMenu("Structure modification");

	
	new mqSmoothDialogReaction(submenuStructureModification->addAction("Smooth each selected surface") << mqSetName("actionSmooth"));
	new mqDecimateDialogReaction(submenuStructureModification->addAction("Decimate each selected surface") << mqSetName("actionDecimate"));

	new mqDensifyDialogReaction(submenuStructureModification->addAction("Densify each selected surface") << mqSetName("actionDensify"));
	new mqFillHolesDialogReaction(submenuStructureModification->addAction("Fill holes of each selected surface") << mqSetName("actionFillHoles"));
	new mqTPSDialogReaction(submenuStructureModification->addAction("TPS deformation of each selected surface") << mqSetName("actionTPS"));
	new mqDecomposeDialogReaction(submenuStructureModification->addAction("Connectivity : decompose each selected surface into independent regions") << mqSetName("actionDecompose"));
	QAction *KeepLargest = submenuStructureModification->addAction("Connectivity : keep largest region for each selected surface");

	QAction *Invert = submenuStructureModification->addAction("Invert each selected surface");
	QAction *Mirror = submenuStructureModification->addAction("Mirror each selected surface along Y plane");
	QAction *ConvexHULL = submenuStructureModification->addAction("Create convex hull for each selected surface");

	QMenu* submenuRenderingModification = menu.addMenu("Rendering modification");
	new mqEditAlphaDialogReaction(submenuRenderingModification->addAction("Change transparency") << mqSetName("actionEditAlpha"));

	QMenu* submenuChangeObjectColor = submenuRenderingModification->addMenu("Change object color");

	QAction *Grey = submenuChangeObjectColor->addAction("Grey");
	
	QAction *Yellow = submenuChangeObjectColor->addAction("Yellow");
	QAction *Red = submenuChangeObjectColor->addAction("Red");
	QAction *Pink = submenuChangeObjectColor->addAction("Pink");
	QAction *Blue = submenuChangeObjectColor->addAction("Blue");
	QAction *Violet = submenuChangeObjectColor->addAction("Violet");
	QAction *Bone = submenuChangeObjectColor->addAction("Bone");
	QAction *Green = submenuChangeObjectColor->addAction("Green");
	QAction *Darkred = submenuChangeObjectColor->addAction("Dark red");
	QAction *Darkblue = submenuChangeObjectColor->addAction("Dark blue");
	QAction *Darkgreen = submenuChangeObjectColor->addAction("Dark green");
	QAction *Orange = submenuChangeObjectColor->addAction("Orange");
	QAction *Brown = submenuChangeObjectColor->addAction("Brown");
	
	QAction::connect(KeepLargest, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotKeepLargest()));
	QAction::connect(Invert, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotInvert()));
	QAction::connect(Mirror, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotMirror()));
	QAction::connect(ConvexHULL, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotConvexHULL()));

	
	QAction::connect(Grey, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotGrey()));
	QAction::connect(Yellow, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotYellow()));
	QAction::connect(Red, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotRed()));
	QAction::connect(Pink, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotPink()));
	QAction::connect(Blue, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotBlue()));
	QAction::connect(Violet, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotViolet()));
	QAction::connect(Bone, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotBone()));
	QAction::connect(Green, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotGreen()));
	QAction::connect(Darkred, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotDarkred()));
	QAction::connect(Darkblue, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotDarkblue()));
	QAction::connect(Darkgreen, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotDarkgreen()));
	QAction::connect(Orange, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotOrange()));
	QAction::connect(Brown, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotBrown()));
	
	
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
	new mqEditAllFLGColorDialogReaction(menu.addAction("Edit color of all selected flags") << mqSetName("actionEditAllSelectedFlagsColors"));
	new mqEditAllFLGLengthDialogReaction(menu.addAction("Edit length all selected flags") << mqSetName("actionEditAllSelectedFlagsLength"));
	QAction *UpdateAllSelectedFlagsColors = menu.addAction("Update all selected flags colours automatically");
	QAction::connect(UpdateAllSelectedFlagsColors, SIGNAL(triggered()), mqMeshToolsCore::instance(), SLOT(slotUpdateAllSelectedFlagsColors()));
	

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

	QToolBar* ObjectsToolBar = new mqObjectsControlsToolbar(&mainWindow)
		<< mqSetName("ObjectsControlsToolbar");
	ObjectsToolBar->layout()->setSpacing(0);
	mainWindow.addToolBar(Qt::RightToolBarArea, ObjectsToolBar);

	QToolBar* scalarsToolBar = new mqScalarsControlsToolbar(&mainWindow)
		<< mqSetName("ScalarsControlsToolbar");
	scalarsToolBar->layout()->setSpacing(0);
	mainWindow.addToolBar(Qt::BottomToolBarArea, scalarsToolBar);

	QToolBar* cameraToolBar = new mqCameraControlsToolbar(&mainWindow)
		<< mqSetName("CameraControlsToolbar");
	cameraToolBar->layout()->setSpacing(0);
	mainWindow.addToolBar(Qt::LeftToolBarArea, cameraToolBar);

}

