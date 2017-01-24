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
// 
class mqUndoStack;

class  mqMeshToolsCore : public QObject
{
	Q_OBJECT
		typedef QObject Superclass;
public:
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

	double* Getmui_MeshColor();
	void Getmui_MeshColor(double c[4]);
	double* Getmui_DefaultMeshColor();
	void Getmui_DefaultMeshColor(double c[4]);
	void Setmui_MeshColor(double c1, double c2, double c3, double c4);
	void Setmui_MeshColor(double c[4]);

	



	

	//vtkSetVector3Macro(mui_BackGroundColor, double);
	//vtkGetVector3Macro(mui_BackGroundColor, double);
	//vtkGetVector3Macro(mui_DefaultBackGroundColor, double);

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
  
  //void SetRenderWindow(vtkSmartPointer<vtkRenderWindow> renwin)
  void SetRenderWindow(vtkRenderWindow *renwin)
  {
	  this->RenderWindow = renwin;
  }
  vtkSmartPointer<vtkLMActorCollection> getLandmarkCollection();
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

  //vtkSmartPointer<vtkUndoStack> getUndoStack();
  mqUndoStack* getUndoStack();
  void setUndoStack(mqUndoStack* stack);
  mqUndoStack* getUndoStack() const { return this->UndoStack; }
  mqMeshToolsCore();
protected:
	
	~mqMeshToolsCore();
	//vtkUndoStack* mUndoStack;
	vtkSmartPointer<vtkMTActorCollection> ActorCollection;
	vtkSmartPointer<vtkLMActorCollection> LandmarkCollection;
	//vtkMTActorCollection *ActorCollection;
	vtkSmartPointer<vtkRenderer> Renderer;
	//vtkSmartPointer<vtkRenderWindow> RenderWindow;
	vtkRenderWindow* RenderWindow;
	vtkSmartPointer<vtkCamera> Camera;
	vtkSmartPointer<vtkGridActor> GridActor;
	vtkSmartPointer<vtkLMActor> LMActor;
	//vtkUndoStack* UndoStack;
	mqUndoStack* UndoStack;
	//vtkSmartPointer<vtkUndoStack> UndoStack;
	int mui_ShowGrid;
	int mui_Anaglyph;
	int mui_ShowOrientationHelper;
	int mui_CameraCentreOfMassAtOrigin;
	int mui_CameraOrtho;

	int mui_DefaultShowGrid;
	int mui_DefaultAnaglyph;
	int mui_DefaultShowOrientationHelper;
	int mui_DefaultCameraCentreOfMassAtOrigin;
	int mui_DefaultCameraOrtho;

	double mui_MeshColor[4];
	double mui_BackGroundColor[3];
	double mui_BackGroundColor2[3];

	double mui_DefaultMeshColor[4];
	double mui_DefaultBackGroundColor[3];
	double mui_DefaultBackGroundColor2[3];
	

private:
	static mqMeshToolsCore* Instance;

};

#endif
