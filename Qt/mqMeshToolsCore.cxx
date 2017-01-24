/*=========================================================================

  Program:   MeshTools
  Module:    MeshToolsCore.cxx


=========================================================================*/
#include "mqMeshToolsCore.h"
#include "vtkMTActor.h"
#include "vtkLMActor.h"
#include <vtkSphereSource.h>
#include <vtkCubeAxesActor.h>

#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>


#include "mqUndoStack.h"


//-----------------------------------------------------------------------------
mqMeshToolsCore* mqMeshToolsCore::Instance = 0;

//-----------------------------------------------------------------------------
mqMeshToolsCore* mqMeshToolsCore::instance()
{
	return mqMeshToolsCore::Instance;
}



mqMeshToolsCore::mqMeshToolsCore()
{

	mqMeshToolsCore::Instance = this;
	this->mui_Anaglyph = this->mui_DefaultAnaglyph = 0;
	this->mui_ShowGrid = this->mui_DefaultShowGrid = 1;
	
	this->mui_MeshColor[0] = this->mui_DefaultMeshColor[0] = 0.631373;
	this->mui_MeshColor[1] = this->mui_DefaultMeshColor[1] = 0.572549;
	this->mui_MeshColor[2] = this->mui_DefaultMeshColor[2] = 0.372549;
	this->mui_MeshColor[3] = this->mui_DefaultMeshColor[3] = 1;
	cout << "mui_DefaultMeshColor[3] (alpha)=" << mui_DefaultMeshColor[3] << endl;
	this->mui_BackGroundColor2[0] = this->mui_DefaultBackGroundColor2[0] = 0;
	this->mui_BackGroundColor2[1] = this->mui_DefaultBackGroundColor2[1] = 0;
	this->mui_BackGroundColor2[2] = this->mui_DefaultBackGroundColor2[2] = 0;

	this->mui_BackGroundColor[0] = this->mui_DefaultBackGroundColor[0] = 0.5;
	this->mui_BackGroundColor[1] = this->mui_DefaultBackGroundColor[1] = 0.5;
	this->mui_BackGroundColor[2] = this->mui_DefaultBackGroundColor[2] = 1;

	this->mui_ShowOrientationHelper = this->mui_DefaultShowOrientationHelper = 1;
	this->mui_CameraOrtho = this->mui_DefaultCameraOrtho = 1;
	this->mui_CameraCentreOfMassAtOrigin = this->mui_DefaultCameraCentreOfMassAtOrigin = 0;
	//this->UndoStack = vtkSmartPointer<vtkUndoStack>::New();
	mqUndoStack* undoStack = new mqUndoStack();
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
	this->Renderer = vtkSmartPointer<vtkRenderer>::New();
	this->ActorCollection = vtkSmartPointer<vtkMTActorCollection>::New();
	this->ActorCollection->SetRenderer(this->Renderer);

	this->LandmarkCollection = vtkSmartPointer<vtkLMActorCollection>::New();
	this->LandmarkCollection->SetRenderer(this->Renderer);
	
	

	this->Renderer->SetUseDepthPeeling(1);
	this->Renderer->SetMaximumNumberOfPeels(100);
	this->Renderer->SetOcclusionRatio(0.1);
	this->Camera = this->Renderer->GetActiveCamera();
	this->GridActor = vtkSmartPointer<vtkGridActor>::New();
	this->GridActor->SetGridType(2);	

	this->LMActor = vtkSmartPointer<vtkLMActor>::New();
	this->LMActor->SetLMType(0);
	this->LMActor->SetLMBodyType(0);
	this->LMActor->SetLMSize(3);
	this->LMActor->SetLMOrigin(10,10,10);
	//this->Renderer->AddActor(this->LMActor);
	
	this->Renderer->AddActor(this->GridActor);
	/*vtkSmartPointer<vtkSphereSource> sphereSource =
		vtkSmartPointer<vtkSphereSource>::New();
	
	
	sphereSource->Update();

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(sphereSource->GetOutputPort());

	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);*/
	
	/*vtkSmartPointer<vtkCubeAxesActor> cubeaxesactor =
		vtkSmartPointer<vtkCubeAxesActor>::New();
	cubeaxesactor->SetCamera(this->Camera);
	this->Renderer->AddActor(cubeaxesactor);*/

	
}



void mqMeshToolsCore::Render()
{
	this->RenderWindow->Render();
}

void mqMeshToolsCore::Setmui_ShowOrientationHelper(int orientationHelper) { this->mui_ShowOrientationHelper = orientationHelper; }
int mqMeshToolsCore::Getmui_DefaultShowOrientationHelper() { return this->mui_DefaultShowOrientationHelper; }
int mqMeshToolsCore::Getmui_ShowOrientationHelper() { return this->mui_ShowOrientationHelper; }

void mqMeshToolsCore::Setmui_CameraCentreOfMassAtOrigin(int comao) { this->mui_CameraCentreOfMassAtOrigin = comao; }
int mqMeshToolsCore::Getmui_DefaultCameraCentreOfMassAtOrigin() { return this->mui_DefaultCameraCentreOfMassAtOrigin; }
int mqMeshToolsCore::Getmui_CameraCentreOfMassAtOrigin() { return this->mui_CameraCentreOfMassAtOrigin; }


void mqMeshToolsCore::Setmui_CameraOrtho(int ortho) { this->mui_CameraOrtho = ortho; }
int mqMeshToolsCore::Getmui_DefaultCameraOrtho() { return this->mui_DefaultCameraOrtho; }
int mqMeshToolsCore::Getmui_CameraOrtho() { return this->mui_CameraOrtho; }

void mqMeshToolsCore::Setmui_ShowGrid(int showgrid) { this->mui_ShowGrid = showgrid; }
int mqMeshToolsCore::Getmui_ShowGrid() { return this->mui_ShowGrid; }
int mqMeshToolsCore::Getmui_DefaultShowGrid() { return this->mui_DefaultShowGrid; };

int mqMeshToolsCore::Getmui_DefaultAnaglyph() { return this->mui_DefaultAnaglyph; }
int mqMeshToolsCore::Getmui_Anaglyph() { return this->mui_Anaglyph; }
void mqMeshToolsCore::Setmui_Anaglyph(int anaglyph)
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




double* mqMeshToolsCore::Getmui_MeshColor() { return this->mui_MeshColor; }
void mqMeshToolsCore::Getmui_MeshColor(double c[4])
{
	double *co = this->Getmui_MeshColor();

	c[0] = co[0];
	c[1] = co[1];
	c[2] = co[2];
	c[3] = co[3];
}
double* mqMeshToolsCore::Getmui_DefaultMeshColor() { return this->mui_DefaultMeshColor; }
void mqMeshToolsCore::Getmui_DefaultMeshColor(double c[4])
{
	double *co = this->Getmui_DefaultMeshColor();

	c[0] = co[0];
	c[1] = co[1];
	c[2] = co[2];
	c[3] = co[3];
}
void mqMeshToolsCore::Setmui_MeshColor(double c1, double c2, double c3, double c4)
{
	double c[4];
	c[0] = c1;
	c[1] = c2;
	c[2] = c3;
	c[3] = c4;
	

	this->Setmui_MeshColor(c);
}
void mqMeshToolsCore::Setmui_MeshColor(double c[4])
{
	this->mui_MeshColor[0] = c[0];
	this->mui_MeshColor[1] = c[1];
	this->mui_MeshColor[2] = c[2];
	this->mui_MeshColor[3] = c[3];
	//cout << "Core: this->mui_MeshColor[3]="<<this->mui_MeshColor[3] << endl;
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


double* mqMeshToolsCore::Getmui_BackGroundColor() { return this->mui_BackGroundColor; }
void mqMeshToolsCore::Getmui_BackGroundColor(double bg[3])
{
	double *bgr = this->Getmui_BackGroundColor();

	bg[0] = bgr[0];
	bg[1] = bgr[1];
	bg[2] = bgr[2];
	
}


double* mqMeshToolsCore::Getmui_DefaultBackGroundColor() { return this->mui_DefaultBackGroundColor; }
void mqMeshToolsCore::Getmui_DefaultBackGroundColor(double bg[3])
{
	double *bgr = this->Getmui_DefaultBackGroundColor();

	bg[0] = bgr[0];
	bg[1] = bgr[1];
	bg[2] = bgr[2];

}


double* Getmui_DefaultBackGroundColor();
void Getmui_DefaultBackGroundColor(double bg[3]);
void mqMeshToolsCore::Setmui_BackGroundColor(double bg1, double bg2, double bg3)
{
	double background[3];
	background[0] = bg1;
	background[1] = bg2;
	background[2] = bg3;

	this->Setmui_BackGroundColor(background);
}
void mqMeshToolsCore::Setmui_BackGroundColor(double background[3])
{
	this->mui_BackGroundColor[0] = background[0];
	this->mui_BackGroundColor[1] = background[1];
	this->mui_BackGroundColor[2] = background[2];
	this->Renderer->SetBackground(background);
	//this->RenderWindow->Render();
}

double* mqMeshToolsCore::Getmui_BackGroundColor2() { return this->mui_BackGroundColor2; }
void mqMeshToolsCore::Getmui_BackGroundColor2(double bg[3])
{
	double *bgr = this->Getmui_BackGroundColor2();

	bg[0] = bgr[0];
	bg[1] = bgr[1];
	bg[2] = bgr[2];

}


double* mqMeshToolsCore::Getmui_DefaultBackGroundColor2() { return this->mui_DefaultBackGroundColor2; }
void mqMeshToolsCore::Getmui_DefaultBackGroundColor2(double bg[3])
{
	double *bgr = this->Getmui_DefaultBackGroundColor2();

	bg[0] = bgr[0];
	bg[1] = bgr[1];
	bg[2] = bgr[2];

}

void mqMeshToolsCore::Setmui_BackGroundColor2(double bg1, double bg2, double bg3)
{
	double background[3];
	background[0] = bg1;
	background[1] = bg2;
	background[2] = bg3;

	this->Setmui_BackGroundColor2(background);
}
void mqMeshToolsCore::Setmui_BackGroundColor2(double background[3])
{
	this->mui_BackGroundColor2[0] = background[0];
	this->mui_BackGroundColor2[1] = background[1];
	this->mui_BackGroundColor2[2] = background[2];
	this->Renderer->SetBackground2(background);
	//this->RenderWindow->Render();
}

void mqMeshToolsCore::Undo()
{
	// a Set is only a label (action) and an id
	//vtkUndoSet *MyUndoSet = this->UndoStack->GetNextUndoSet();
	//this->ActorCollection->Undo(MySet);
	cout << "Root Undo!" << endl;
	this->UndoStack->undo(); // removes the next undo set.. 

}
void mqMeshToolsCore::Undo(int Count)
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
	this->ActorCollection->Undo(Count);

	//@@TODO! 
	this->LandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->LandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->ActorCollection->GetNextActor());
		cout << "MyLMActor undo!" << endl;
		//myActor->Undo(Count);
	}
	// To update to take into account reorder!
	this->LandmarkCollection->Undo(Count);

}
void mqMeshToolsCore::Redo()
{
	cout << "Root Redo!" << endl;
	this->UndoStack->redo(); // removes the next undo set.. 
}

void mqMeshToolsCore::Redo(int Count)
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
	this->ActorCollection->Redo(Count);

	//@@TODO! 
	this->LandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->LandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->ActorCollection->GetNextActor());
		cout << "MyLMActor redo!" << endl;
		//myActor->Redo(Count);
	}
	// To update to take into account reorder!
	this->LandmarkCollection->Redo(Count);

}
void mqMeshToolsCore::Erase(int Count)
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
	this->ActorCollection->Erase(Count);

	this->LandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->LandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->ActorCollection->GetNextActor());
		//cout << "MyActor Erase!" << endl;
		//@@ TO IMPLEMENT
		//myActor->Erase(Count);
	}
	this->LandmarkCollection->Erase(Count);

}
void mqMeshToolsCore::setUndoStack(mqUndoStack* stack)
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
mqMeshToolsCore::~mqMeshToolsCore()
{
	//this->ActorCollection->Delete();
	if (mqMeshToolsCore::Instance == this)
	{
		mqMeshToolsCore::Instance = 0;
	}
}
/*vtkSmartPointer<vtkUndoStack> mqMeshToolsCore::getUndoStack()
{
	return this->UndoStack;
}*/

mqUndoStack* mqMeshToolsCore::getUndoStack()
{
return this->UndoStack;
}


vtkSmartPointer<vtkMTActorCollection> mqMeshToolsCore::getActorCollection()
{
	return this->ActorCollection;
}
vtkSmartPointer<vtkLMActorCollection> mqMeshToolsCore::getLandmarkCollection()
{
	return this->LandmarkCollection;
}
/*
vtkMTActorCollection* mqMeshToolsCore::getActorCollection()
{
	return this->ActorCollection;
}*/

vtkSmartPointer<vtkRenderer> mqMeshToolsCore::getRenderer()
{
	return this->Renderer;
}
vtkSmartPointer<vtkCamera> mqMeshToolsCore::getCamera()
{
	return this->Camera;
}
vtkSmartPointer<vtkGridActor> mqMeshToolsCore::getGridActor()
{
	return this->GridActor;
}

