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
#include "vtkUndoStack.h"
//#include "vtkUndoStackInternal.h"

#include <vtkObject.h>
#include <vtkSmartPointer.h>    
#include <vtkCamera.h>

class  vtkMeshToolsCore : public vtkObject
{
public:
  static vtkMeshToolsCore* New();
  static vtkMeshToolsCore* instance();
  vtkTypeMacro(vtkMeshToolsCore, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  vtkSmartPointer<vtkMTActorCollection> getActorCollection();
  //vtkMTActorCollection* getActorCollection();
  vtkSmartPointer<vtkRenderer> getRenderer();
  vtkSmartPointer<vtkCamera> getCamera();
  vtkSmartPointer<vtkGridActor> getGridActor();
  
  int getUndoCount();
  //vtkSmartPointer<vtkUndoStack> getUndoStack();
  //vtkUndoStack* getUndoStack();

  void setUndoCount(int Count);
protected:
	vtkMeshToolsCore();
	~vtkMeshToolsCore();
	//vtkUndoStack* mUndoStack;
	vtkSmartPointer<vtkMTActorCollection> ActorCollection;
	//vtkMTActorCollection *ActorCollection;
	vtkSmartPointer<vtkRenderer> Renderer;
	vtkSmartPointer<vtkCamera> Camera;
	vtkSmartPointer<vtkGridActor> GridActor;
	
	//vtkSmartPointer<vtkUndoStack> UndoStack;
	int UndoCount;
	

private:
	static vtkMeshToolsCore* Instance;

};

#endif
