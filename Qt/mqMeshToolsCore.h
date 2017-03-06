/*=========================================================================

  Program:   MeshTools
  Module:    MeshToolsCore.h


=========================================================================*/
/**
 * @class   MeshToolsCore
 * @brief   Contains the most important objects such as the undo stack and the 
 * 
*/

#ifndef mqMeshToolsCore_h
#define mqMeshToolsCore_h
#include <QObject>
#include <QPointer>
#include "vtkOrientationHelperWidget.h"
#include "vtkMTActorCollection.h"
#include "vtkLMActorCollection.h"
#include "vtkGridActor.h"
#include "vtkLMActor.h"
//#include "vtkUndoStack.h" => for some reason the ompilation fails if this header is included
//#include "vtkUndoStackInternal.h"


#include <vtkSmartPointer.h>    
#include <vtkCamera.h>
#include <vtkRenderWindow.h>
#include <QMainWindow>
// 
class mqUndoStack;

class  mqMeshToolsCore : public QObject
{
	Q_OBJECT
		typedef QObject Superclass;
public:
	void UnselectAll(int Count);
	void SelectAll(int Count);
	void Setmui_Anaglyph(int anaglyph);
	int Getmui_DefaultAnaglyph();
	int Getmui_Anaglyph();

	void Setmui_ShowGrid(int showgrid);
	int Getmui_ShowGrid();
	int Getmui_DefaultShowGrid();

	void Setmui_ShowOrientationHelper(int orientationHelper);
	int Getmui_DefaultShowOrientationHelper();
	int Getmui_ShowOrientationHelper();

	void Setmui_CameraCentreOfMassAtOrigin(int comao);
	int Getmui_DefaultCameraCentreOfMassAtOrigin();
	int Getmui_CameraCentreOfMassAtOrigin();


	void Setmui_CameraOrtho(int ortho);
	int Getmui_DefaultCameraOrtho();
	int Getmui_CameraOrtho();

	void Setmui_X1Label(QString label);
	QString Getmui_DefaultX1Label();
	QString Getmui_X1Label();


	void Setmui_X2Label(QString label);
	QString Getmui_DefaultX2Label();
	QString Getmui_X2Label();

	void Setmui_Y1Label(QString label);
	QString Getmui_DefaultY1Label();
	QString Getmui_Y1Label();


	void Setmui_Y2Label(QString label);
	QString Getmui_DefaultY2Label();
	QString Getmui_Y2Label();

	void Setmui_Z1Label(QString label);
	QString Getmui_DefaultZ1Label();
	QString Getmui_Z1Label();


	void Setmui_Z2Label(QString label);
	QString Getmui_DefaultZ2Label();
	QString Getmui_Z2Label();


	double* Getmui_MeshColor();
	void Getmui_MeshColor(double c[4]);
	double* Getmui_DefaultMeshColor();
	void Getmui_DefaultMeshColor(double c[4]);
	void Setmui_MeshColor(double c1, double c2, double c3, double c4);
	void Setmui_MeshColor(double c[4]);

	
	double* Getmui_FlagColor();
	void Getmui_FlagColor(double c[3]);
	double* Getmui_DefaultFlagColor();
	void Getmui_DefaultFlagColor(double c[3]);
	void Setmui_FlagColor(double c1, double c2, double c3);
	void Setmui_FlagColor(double c[3]);

	void Setmui_LandmarkBodyType(int type);
	int Getmui_DefaultLandmarkBodyType();
	int Getmui_LandmarkBodyType();

	void Setmui_LandmarkMode(int mode);
	int Getmui_DefaultLandmarkMode();
	int Getmui_LandmarkMode();

	void Setmui_LandmarkRenderingSize(double size);
	double Getmui_DefaultLandmarkRenderingSize();
	double Getmui_LandmarkRenderingSize();


	void Setmui_AdjustLandmarkRenderingSize(int adjust);
	int Getmui_DefaultAdjustLandmarkRenderingSize();
	int Getmui_AdjustLandmarkRenderingSize();

	void Setmui_AdjustScaleFactor(double factor);
	double Getmui_DefaultAdjustScaleFactor();
	double Getmui_AdjustScaleFactor();

	void Setmui_FlagRenderingSize(double size);
	double Getmui_DefaultFlagRenderingSize();
	double Getmui_FlagRenderingSize();
	

	

	double* Getmui_BackGroundColor();
	void Getmui_BackGroundColor(double bg[3]);
	double* Getmui_DefaultBackGroundColor();
	void Getmui_DefaultBackGroundColor(double bg[3]);
	void Setmui_BackGroundColor(double bg1, double bg2, double bg3);
	void Setmui_BackGroundColor(double background[3]);

	
	
	

	double* Getmui_BackGroundColor2();
	void Getmui_BackGroundColor2(double bg[3]);
	double* Getmui_DefaultBackGroundColor2();
	void Getmui_DefaultBackGroundColor2(double bg[3]);

	void Setmui_BackGroundColor2(double bg1, double bg2, double bg3);
	void Setmui_BackGroundColor2(double background[3]);
  //static mqMeshToolsCore* New();
  static mqMeshToolsCore* instance();
  void UpdateLandmarkSettings();
  void UpdateLandmarkSettings(vtkLMActor *myActor);
  void SetMainWindow(QMainWindow *_mainWindow);
  QMainWindow* GetMainWindow();
  //void SetRenderWindow(vtkSmartPointer<vtkRenderWindow> renwin)
  void SetRenderWindow(vtkRenderWindow *renwin)
  {
	  this->RenderWindow = renwin;
  }
  vtkSmartPointer<vtkLMActorCollection> getNormalLandmarkCollection();
  vtkSmartPointer<vtkLMActorCollection> getTargetLandmarkCollection();
  vtkSmartPointer<vtkLMActorCollection> getNodeLandmarkCollection();
  vtkSmartPointer<vtkLMActorCollection> getHandleLandmarkCollection();
  vtkSmartPointer<vtkLMActorCollection> getFlagLandmarkCollection();

  vtkSmartPointer<vtkMTActorCollection> getActorCollection();
  //vtkMTActorCollection* getActorCollection();
  vtkSmartPointer<vtkRenderer> getRenderer();
  vtkSmartPointer<vtkCamera> getCamera();
  vtkSmartPointer<vtkGridActor> getGridActor();
  void Redo(); // calls the undoStack Redo function
  void Undo(); // callse the undoStack Undo function
  void Render(); // called when something has changed and the app needs to redraw
  void Redo(int Count); // send redo message to the concerned elements (actors mostly!)
  void Erase(int Count); // erase action number Count (in actors!)
  void Undo(int Count); // send undo message to the concerned elements (actors!)
  double AdjustedLandmarkSize();
  //vtkSmartPointer<vtkUndoStack> getUndoStack();
  mqUndoStack* getUndoStack();
  void setUndoStack(mqUndoStack* stack);
  mqUndoStack* getUndoStack() const { return this->UndoStack; }
  mqMeshToolsCore();
  void AdjustCameraAndGrid();
  void ReplaceCameraAndGrid();
  void DollyCameraForPerspectiveMode();
  void DollyCameraForParallelScale();
  void ResetCameraOrthoPerspective();
  void SetGridVisibility();
  void SetOrientationHelperVisibility();
  std::string CheckingName(std::string name_obj, int cpt_name);
  vtkMTActor* GetLastActor();
  vtkLMActor* GetLastLandmark(int mode);
  void ApplyMatrix(vtkSmartPointer<vtkMatrix4x4> Mat, int mode);
  void InitializeOrientationHelper();
  void SetOrientationHelperLabels(std::string X1, std::string X2, std::string Y1, std::string Y2, std::string Z1, std::string Z2);
  void CreateLandmark(double coord[3], double ori[3], int lmk_type, int node_type = -1);
protected:
	
	~mqMeshToolsCore();
	//vtkUndoStack* mUndoStack;
	vtkSmartPointer<vtkMTActorCollection> ActorCollection;
	vtkSmartPointer<vtkLMActorCollection> NormalLandmarkCollection;
	vtkSmartPointer<vtkLMActorCollection> TargetLandmarkCollection;
	vtkSmartPointer<vtkLMActorCollection> NodeLandmarkCollection;
	vtkSmartPointer<vtkLMActorCollection> HandleLandmarkCollection;
	vtkSmartPointer<vtkLMActorCollection> FlagLandmarkCollection;
	//vtkMTActorCollection *ActorCollection;
	vtkSmartPointer<vtkRenderer> Renderer;
	//vtkSmartPointer<vtkRenderWindow> RenderWindow;
	vtkRenderWindow* RenderWindow;
	vtkSmartPointer<vtkCamera> Camera;
	vtkSmartPointer<vtkGridActor> GridActor;
	vtkSmartPointer<vtkLMActor> LMActor;
	vtkSmartPointer<vtkLMActor> LMActor2;
	QMainWindow* MainWindow;
	//vtkUndoStack* UndoStack;
	mqUndoStack* UndoStack;
	//vtkSmartPointer<vtkUndoStack> UndoStack;
	int mui_ShowGrid;
	int mui_Anaglyph;
	int mui_ShowOrientationHelper;
	int mui_CameraCentreOfMassAtOrigin;
	int mui_CameraOrtho;

	QString mui_X1Label;
	QString mui_X2Label;
	QString mui_Y1Label;
	QString mui_Y2Label;
	QString mui_Z1Label;
	QString mui_Z2Label;

	QString mui_DefaultX1Label;
	QString mui_DefaultX2Label;
	QString mui_DefaultY1Label;
	QString mui_DefaultY2Label;
	QString mui_DefaultZ1Label;
	QString mui_DefaultZ2Label;

	int mui_DefaultShowGrid;
	int mui_DefaultAnaglyph;
	int mui_DefaultShowOrientationHelper;
	int mui_DefaultCameraCentreOfMassAtOrigin;
	int mui_DefaultCameraOrtho;

	double mui_MeshColor[4];
	double mui_FlagColor[3];
	double mui_BackGroundColor[3];
	double mui_BackGroundColor2[3];

	double mui_DefaultMeshColor[4];
	double mui_DefaultFlagColor[3];
	double mui_DefaultBackGroundColor[3];
	double mui_DefaultBackGroundColor2[3];
	
	int mui_DefaultLandmarkBodyType;
	int mui_LandmarkBodyType;

	double mui_DefaultLandmarkRenderingSize;
	double mui_LandmarkRenderingSize;

	double mui_DefaultAdjustLandmarkRenderingSize;
	double mui_AdjustLandmarkRenderingSize;

	double mui_DefaultAdjustScaleFactor;
	double mui_AdjustScaleFactor;

	int mui_DefaultFlagRenderingSize;
	int mui_FlagRenderingSize;

	int mui_DefaultLandmarkMode; // 0 normal 1target 2node 3handle 4flag
	int mui_LandmarkMode;

	vtkOrientationHelperWidget* OrientationHelperWidget;

private:
	static mqMeshToolsCore* Instance;

};

#endif
