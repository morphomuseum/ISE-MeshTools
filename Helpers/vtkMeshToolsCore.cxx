/*=========================================================================

  Program:   MeshTools
  Module:    MeshToolsCore.cxx


=========================================================================*/
#include "vtkMeshToolsCore.h"
#include <vtkObjectFactory.h>
#include "vtkMTActor.h"
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
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
	this->mui_Anaglyph = 0;
	this->mui_ShowGrid = 1;
	this->mui_MeshColor[0] = 1;
	this->mui_MeshColor[1] = 0.5;
	this->mui_MeshColor[2] = 0;
	this->mui_MeshColor[3] = 0.75;

	this->mui_BackGroundColor2[0] = 0;
	this->mui_BackGroundColor2[1] = 0;
	this->mui_BackGroundColor2[2] = 0;

	this->mui_BackGroundColor[0] = 0.5;
	this->mui_BackGroundColor[1] = 0.5;
	this->mui_BackGroundColor[2] = 1;

	this->mui_ShowOrientationHelper = 1;
	this->mui_CameraOrtho = 1;
	
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
	//this->RenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	this->RenderWindow = NULL;
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

void vtkMeshToolsCore::Setmui_Anaglyph(int anaglyph)
{
	this->mui_Anaglyph = anaglyph;
	if (this->RenderWindow != NULL)
	{
		if (anaglyph == 1)
		{
			this->RenderWindow->StereoRenderOn();
			this->RenderWindow->Render();
		}
		else
		{
			this->RenderWindow->StereoRenderOff();
			this->RenderWindow->Render();
		}
	}
}

void vtkMeshToolsCore::Setmui_MeshColor(double c1, double c2, double c3, double c4)
{
	double c[4];
	c[0] = c1;
	c[1] = c2;
	c[2] = c3;
	c[3] = c4;
	

	this->Setmui_MeshColor(c);
}
void vtkMeshToolsCore::Setmui_MeshColor(double c[4])
{
	this->mui_MeshColor[0] = c[0];
	this->mui_MeshColor[1] = c[1];
	this->mui_MeshColor[2] = c[3];
	this->mui_MeshColor[3] = c[4];
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected()==1)
		{
			
			//myActor->SetmColor(this->mui_MeshColor);
			//myActor->SetSelected(0);
		}
	}
}

void vtkMeshToolsCore::Setmui_BackGroundColor(double bg1, double bg2, double bg3)
{
	double background[3];
	background[0] = bg1;
	background[1] = bg2;
	background[2] = bg3;

	this->Setmui_BackGroundColor(background);
}
void vtkMeshToolsCore::Setmui_BackGroundColor(double background[3])
{
	this->mui_BackGroundColor[0] = background[0];
	this->mui_BackGroundColor[1] = background[1];
	this->mui_BackGroundColor[2] = background[2];
	this->Renderer->SetBackground(background);
	//this->RenderWindow->Render();
}
void vtkMeshToolsCore::Setmui_BackGroundColor2(double bg1, double bg2, double bg3)
{
	double background[3];
	background[0] = bg1;
	background[1] = bg2;
	background[2] = bg3;

	this->Setmui_BackGroundColor2(background);
}
void vtkMeshToolsCore::Setmui_BackGroundColor2(double background[3])
{
	this->mui_BackGroundColor2[0] = background[0];
	this->mui_BackGroundColor2[1] = background[1];
	this->mui_BackGroundColor2[2] = background[2];
	this->Renderer->SetBackground2(background);
	//this->RenderWindow->Render();
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
