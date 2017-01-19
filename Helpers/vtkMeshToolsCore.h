/*=========================================================================

  Program:   MeshTools
  Module:    MeshToolsCore.h


=========================================================================*/
/**
 * @class   MeshToolsCore
 * @brief   Contains the most important objects such as the undo stack and the 
 * 
*/

#ifndef vtkMeshToolsCore_h
#define vtkMeshToolsCore_h

#include "vtkOrientationHelperWidget.h"
#include "vtkMTActorCollection.h"
#include "vtkGridActor.h"
//#include "vtkUndoStack.h" => for some reason the ompilation fails if this header is included
//#include "vtkUndoStackInternal.h"

#include <vtkObject.h>
#include <vtkSmartPointer.h>    
#include <vtkCamera.h>
#include <vtkRenderWindow.h>
// 
class mqUndoStack;

class  vtkMeshToolsCore : public vtkObject
{
public:
	vtkSetMacro(mui_ShowGrid, int);
	vtkGetMacro(mui_ShowGrid, int);
	vtkGetMacro(mui_DefaultShowGrid, int);
	//vtkSetMacro(mui_Anaglyph, int);
	void Setmui_Anaglyph(int anaglyph);
	vtkGetMacro(mui_Anaglyph, int);
	vtkGetMacro(mui_DefaultAnaglyph, int);


	vtkSetMacro(mui_ShowOrientationHelper, int);
	vtkGetMacro(mui_ShowOrientationHelper, int);
	vtkGetMacro(mui_DefaultShowOrientationHelper, int);

	vtkSetMacro(mui_CameraCentreOfMassAtOrigin, int);
	vtkGetMacro(mui_CameraCentreOfMassAtOrigin, int);
	vtkGetMacro(mui_DefaultCameraCentreOfMassAtOrigin, int);

	vtkSetMacro(mui_CameraOrtho, int);
	vtkGetMacro(mui_CameraOrtho, int);
	vtkGetMacro(mui_DefaultCameraOrtho, int);


	//vtkSetVector4Macro(mui_MeshColor, double);
	vtkGetVector4Macro(mui_MeshColor, double);
	vtkGetVector4Macro(mui_DefaultMeshColor, double);

	//vtkSetVector3Macro(mui_BackGroundColor, double);
	vtkGetVector3Macro(mui_BackGroundColor, double);
	vtkGetVector3Macro(mui_DefaultBackGroundColor, double);
	//vtkSetVector3Macro(mui_BackGroundColor2, double);
	vtkGetVector3Macro(mui_BackGroundColor2, double);
	vtkGetVector3Macro(mui_DefaultBackGroundColor2, double);

	void Setmui_MeshColor(double c1, double c2, double c3, double c4);
	void Setmui_MeshColor(double c[3]);
	

	void Setmui_BackGroundColor(double bg1, double bg2, double bg3);
	void Setmui_BackGroundColor(double background[3]);
	void Setmui_BackGroundColor2(double bg1, double bg2, double bg3);
	void Setmui_BackGroundColor2(double background[3]);
  static vtkMeshToolsCore* New();
  static vtkMeshToolsCore* instance();
  vtkTypeMacro(vtkMeshToolsCore, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  //void SetRenderWindow(vtkSmartPointer<vtkRenderWindow> renwin)
  void SetRenderWindow(vtkRenderWindow *renwin)
  {
	  this->RenderWindow = renwin;
  }

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
  
protected:
	vtkMeshToolsCore();
	~vtkMeshToolsCore();
	//vtkUndoStack* mUndoStack;
	vtkSmartPointer<vtkMTActorCollection> ActorCollection;
	//vtkMTActorCollection *ActorCollection;
	vtkSmartPointer<vtkRenderer> Renderer;
	//vtkSmartPointer<vtkRenderWindow> RenderWindow;
	vtkRenderWindow* RenderWindow;
	vtkSmartPointer<vtkCamera> Camera;
	vtkSmartPointer<vtkGridActor> GridActor;
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
	static vtkMeshToolsCore* Instance;

};

#endif
