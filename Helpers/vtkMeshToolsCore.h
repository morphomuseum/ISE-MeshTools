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
// 
class vtkUndoStack;

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
  void Redo(); // calls the undoStack Redo function
  void Undo(); // callse the undoStack Undo function
  
  void Redo(int Count); // send redo message to the concerned elements (actors mostly!)
  void Undo(int Count); // send undo message to the concerned elements (actors!)

  //vtkSmartPointer<vtkUndoStack> getUndoStack();
  vtkUndoStack* getUndoStack();
  void setUndoStack(vtkUndoStack* stack);
  vtkUndoStack* getUndoStack() const { return this->UndoStack; }
  
protected:
	vtkMeshToolsCore();
	~vtkMeshToolsCore();
	//vtkUndoStack* mUndoStack;
	vtkSmartPointer<vtkMTActorCollection> ActorCollection;
	//vtkMTActorCollection *ActorCollection;
	vtkSmartPointer<vtkRenderer> Renderer;
	vtkSmartPointer<vtkCamera> Camera;
	vtkSmartPointer<vtkGridActor> GridActor;
	vtkUndoStack* UndoStack;
	//vtkSmartPointer<vtkUndoStack> UndoStack;
	
	

private:
	static vtkMeshToolsCore* Instance;

};

#endif
