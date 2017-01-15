/*=========================================================================

  Program:   MeshTools
  Module:    MeshToolsCore.cxx


=========================================================================*/
#include "vtkMeshToolsCore.h"
#include <vtkObjectFactory.h>
#include "vtkMTActor.h"
#include <vtkRenderer.h>
#include "vtkUndoStack.h"
#include "vtkUndoSet.h"
#include "vtkUndoElement.h"

vtkStandardNewMacro(vtkMeshToolsCore);
//-----------------------------------------------------------------------------
vtkMeshToolsCore* vtkMeshToolsCore::Instance = 0;

//-----------------------------------------------------------------------------
vtkMeshToolsCore* vtkMeshToolsCore::instance()
{
	return vtkMeshToolsCore::Instance;
}

vtkMeshToolsCore::vtkMeshToolsCore()
{

	vtkMeshToolsCore::Instance = this;

	
	//this->UndoStack = vtkSmartPointer<vtkUndoStack>::New();
	vtkUndoStack* undoStack = vtkUndoStack::New();
	this->setUndoStack(undoStack);
	//this->mUndoStack = undoStack;
	//MeshTools::testint = 10;
	//MeshTools::Instance = this;
	//this->SetUndoCount(0);
	//vtkUndoStack* undoStack = vtkUndoStack::New();
	//vtkUndoSet* undoSet = vtkUndoSet::New();
	//vtkUndoElement* undoElement = vtkUndoElement::New();
	//undoStack->Push("Test", undoSet);

	/*
	vtkSMSession* session = vtkSMSession::New();
	vtkSMSessionProxyManager* pxm = session->GetSessionProxyManager();

	vtkSMProxy* sphere = pxm->NewProxy("sources", "SphereSource");
	sphere->UpdateVTKObjects();
	QVERIFY(sphere != NULL);
	QCOMPARE(vtkSMPropertyHelper(sphere, "Radius").GetAsDouble(), 0.5);

	vtkSMUndoStack* undoStack = vtkSMUndoStack::New();
	vtkUndoSet* undoSet = vtkUndoSet::New();
	vtkSMRemoteObjectUpdateUndoElement* undoElement = vtkSMRemoteObjectUpdateUndoElement::New();
	undoElement->SetSession(session);

	vtkSMMessage before;
	before.CopyFrom(*sphere->GetFullState());
	vtkSMPropertyHelper(sphere, "Radius").Set(1.2);
	sphere->UpdateVTKObjects();
	vtkSMMessage after;
	after.CopyFrom(*sphere->GetFullState());
	undoElement->SetUndoRedoState(&before, &after);

	undoSet->AddElement(undoElement);
	undoElement->Delete();
	undoStack->Push("ChangeRadius", undoSet);
	undoSet->Delete();

	QVERIFY(static_cast<bool>(undoStack->CanUndo()) == true);
	undoStack->Undo();
	QVERIFY(static_cast<bool>(undoStack->CanUndo()) == false);
	sphere->UpdateVTKObjects();
	QCOMPARE(vtkSMPropertyHelper(sphere, "Radius").GetAsDouble(), 0.5);

	QVERIFY(static_cast<bool>(undoStack->CanRedo()) == true);
	undoStack->Redo();
	sphere->UpdateVTKObjects();
	QCOMPARE(vtkSMPropertyHelper(sphere, "Radius").GetAsDouble(), 1.2);
	QVERIFY(static_cast<bool>(undoStack->CanRedo()) == false);

	undoStack->Delete();

	sphere->Delete();
	session->Delete();
	*/
	this->ActorCollection = vtkSmartPointer<vtkMTActorCollection>::New();
	//this->ActorCollection = vtkMTActorCollection::New();
	this->Renderer = vtkSmartPointer<vtkRenderer>::New();

	this->Renderer->SetUseDepthPeeling(1);
	this->Renderer->SetMaximumNumberOfPeels(100);
	this->Renderer->SetOcclusionRatio(0.1);
	this->Camera = this->Renderer->GetActiveCamera();
	this->GridActor = vtkSmartPointer<vtkGridActor>::New();
	this->GridActor->SetGridType(2);	
	this->Renderer->AddActor(this->GridActor);
}
void vtkMeshToolsCore::Undo()
{
	// a Set is only a label (action) and an id
	//vtkUndoSet *MyUndoSet = this->UndoStack->GetNextUndoSet();
	//this->ActorCollection->Undo(MySet);
	cout << "Root Undo!" << endl;
	this->UndoStack->Undo(); // removes the next undo set.. 

}
void vtkMeshToolsCore::Undo(int Count)
{
	cout << "Undo(" <<Count<<")"<< endl;
	//Calls for the Undo method in vtkActorCollection for this particular Count etc.. 
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
		cout << "MyActor undo!" << endl;
		myActor->Undo(Count);		
	}
}
void vtkMeshToolsCore::Redo()
{
	cout << "Root Redo!" << endl;
	this->UndoStack->Redo(); // removes the next undo set.. 
}

void vtkMeshToolsCore::Redo(int Count)
{
	cout << "Redo(" << Count << ")" << endl;
	//Calls for the Undo method in vtkActorCollection for this particular Count etc.. 
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
		cout << "MyActor Redo!" << endl;
		myActor->Redo(Count);
	}
}
void vtkMeshToolsCore::Erase(int Count)
{
	//cout << "Erase(" << Count << ")" << endl;
	//Calls for the Undo method in vtkActorCollection for this particular Count etc.. 
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
		//cout << "MyActor Erase!" << endl;
		myActor->Erase(Count);
	}
}
void vtkMeshToolsCore::setUndoStack(vtkUndoStack* stack)
{
	if (stack != this->UndoStack)
	{
		this->UndoStack = stack;
		/*if (stack)
		{
			stack->setParent(this);
		}*/
		//emit this->undoStackChanged(stack);
	}
}

//-----------------------------------------------------------------------------
vtkMeshToolsCore::~vtkMeshToolsCore()
{
	//this->ActorCollection->Delete();
	if (vtkMeshToolsCore::Instance == this)
	{
		vtkMeshToolsCore::Instance = 0;
	}
}
/*vtkSmartPointer<vtkUndoStack> vtkMeshToolsCore::getUndoStack()
{
	return this->UndoStack;
}*/

vtkUndoStack* vtkMeshToolsCore::getUndoStack()
{
return this->UndoStack;
}


vtkSmartPointer<vtkMTActorCollection> vtkMeshToolsCore::getActorCollection()
{
	return this->ActorCollection;
}
/*
vtkMTActorCollection* vtkMeshToolsCore::getActorCollection()
{
	return this->ActorCollection;
}*/

vtkSmartPointer<vtkRenderer> vtkMeshToolsCore::getRenderer()
{
	return this->Renderer;
}
vtkSmartPointer<vtkCamera> vtkMeshToolsCore::getCamera()
{
	return this->Camera;
}
vtkSmartPointer<vtkGridActor> vtkMeshToolsCore::getGridActor()
{
	return this->GridActor;
}

//-----------------------------------------------------------------------------
void vtkMeshToolsCore::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}