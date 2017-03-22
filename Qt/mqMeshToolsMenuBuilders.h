/*=========================================================================

   Program: ISE-MeshTools
   Module:    pqMeshToolsMenuBuilders.h

   

========================================================================*/
#ifndef mqMeshToolsMenuBuilders_h
#define mqMeshToolsMenuBuilders_h



class QMenu;
class QWidget;
class QMainWindow;


class mqMeshToolsMenuBuilders
{
public:


	
  /**
  * Builds standard File menu.
  */
  //static void buildFileMenu(QMenu& menu, QMainWindow* mainWindow = 0);
  static void buildFileMenu(QMenu& menu);

  static void buildLandmarksMenu(QMenu& menu);

  /**
  * Builds the standard Edit menu.
  */
  //static void buildEditMenu(QMenu& menu, QMainWindow* mainWindow = 0);
  static void buildEditMenu(QMenu& menu);

  /**
  * Builds the help menu.
  */
  //static void buildHelpMenu(QMenu& menu, QMainWindow* mainWindow = 0);
  static void buildHelpMenu(QMenu& menu);

  /**
  * Builds and adds all standard MeshTools toolbars.
  */
  static void buildToolbars(QMainWindow& mainWindow);
  
};

#endif
