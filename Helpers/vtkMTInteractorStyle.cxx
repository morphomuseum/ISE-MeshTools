/*=========================================================================

  Program:   Visualization Toolkit
  Inspired from Module:    vtkInteractorStyleRubberBandPick.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkMTInteractorStyle.h"
#include "vtkMTActor.h"
#include "vtkLMActor.h"
#include "mqUndoStack.h"
#include "mqMeshToolsCore.h"


#include <vtkIndent.h>
#include <vtkProperty.h>
#include <vtkTransform.h>
#include <vtkCamera.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <vtkObjectFactory.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkUnsignedCharArray.h>
#include <vtkAbstractPropPicker.h>
#include <vtkAssemblyPath.h>
#include <vtkAreaPicker.h>
#include <vtkCallbackCommand.h>
#include <vtkSmartPointer.h>
#include <vtkPropPicker.h>
#include <vtkCellPicker.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
vtkStandardNewMacro(vtkMTInteractorStyle);

#define VTKISMT_ORIENT 0
#define VTKISMT_SELECT 1
#define CTRL_RELEASED 0
#define CTRL_PRESSED 1
#define L_PRESSED 2
#define L_RELEASED 3
#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

//--------------------------------------------------------------------------

vtkMTInteractorStyle::vtkMTInteractorStyle()
{
	this->CurrentMode = VTKISMT_ORIENT;
	this->L = L_RELEASED;
	this->Ctrl = CTRL_RELEASED;
	this->StartPosition[0] = this->StartPosition[1] = 0;
	this->EndPosition[0] = this->EndPosition[1] = 0;
	this->Moving = 0;
	this->PixelArray = vtkUnsignedCharArray::New();
	this->ActorCollection = vtkSmartPointer<vtkMTActorCollection>::New();
	this->LandmarkCollection = vtkSmartPointer<vtkLMActorCollection>::New();
	this->ActorsPositionsSaved = 0;
	this->NumberOfSelectedActors = 0;
}

void vtkMTInteractorStyle::SetActorCollection(vtkSmartPointer<vtkMTActorCollection> ActColl)
{
	this->ActorCollection = ActColl;
}
void vtkMTInteractorStyle::SetLandmarkCollection(vtkSmartPointer<vtkLMActorCollection> LmkColl)
{
	this->LandmarkCollection = LmkColl;
}

//--------------------------------------------------------------------------
vtkMTInteractorStyle::~vtkMTInteractorStyle()
{
  this->PixelArray->Delete();
}

//--------------------------------------------------------------------------
void vtkMTInteractorStyle::StartSelect()
{
	this->CurrentMode = VTKISMT_SELECT;

}
//--------------------------------------------------------------------------
  void vtkMTInteractorStyle::OnKeyPress()
  {
	// Get the keypress
	vtkRenderWindowInteractor *rwi = this->Interactor;
	std::string key = rwi->GetKeySym();
	
	// Output the key that was pressed
	if (key.compare("l") == 0)
	{
		//cout << "l pressed" << endl;
		this->L = L_PRESSED;
	}
	if (key.compare("Control_L") == 0)
	{
		this->Ctrl = CTRL_PRESSED;
		//std::cout << key<< "Pressed" << '\n';
	}
	//cout << this->Interactor->GetKeyCode() << endl;
	//cout << rwi->GetKeySym() << endl;
	if (key.compare("Delete") == 0)
	{
		std::cout << "Pressed " << key << std::endl;
		this->DeleteSelectedActors();
		
	}
	if (key.compare("a") == 0)
	{		
			//cout << "a pressed!" << endl;

			if (this->Ctrl == CTRL_PRESSED)
			{
				std::string action = "Select all actors";
				int Count = BEGIN_UNDO_SET(action);
				this->ActorCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
				{
					vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
					if (myActor->GetSelected() == 0)
					{
						myActor->SaveState(Count);
					}
				}
				END_UNDO_SET();
				//cout << "a and CTRL pressed!" << endl;
				this->ActorCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
				{
					vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
					if (myActor->GetSelected() == 0)
					{
						myActor->SetSelected(1);
						myActor->SetChanged(1);
						
					}


				}
				vtkRenderWindowInteractor *rwi = this->Interactor;
				rwi->Render();
			}
			

	}
	if (key.compare("d") == 0)
		{
			if (this->Ctrl == CTRL_PRESSED)
			{
				std::string action = "Unselect all actors";
				int Count = BEGIN_UNDO_SET(action);
				this->ActorCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
				{
					vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
					if (myActor->GetSelected() == 1)
					{
						myActor->SaveState(Count);
					}
				}
				END_UNDO_SET();
				vtkRenderWindowInteractor *rwi = this->Interactor;
				this->ActorCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
				{
					vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
					if (myActor->GetSelected() == 1)
					{
						myActor->SetSelected(0);
						myActor->SetChanged(1);
						
					}


				}
				rwi->Render();
			}
			
	}
	if (key.compare("z") == 0)
	{
		if (this->Ctrl == CTRL_PRESSED)
		{
			cout << "CTRL-Z detected!" << endl;
			mqMeshToolsCore::instance()->Undo();
			rwi->Render();
		}

	}
	if (key.compare("y") == 0)
	{

		if (this->Ctrl == CTRL_PRESSED)
		{
			cout << "CTRL-Y detected!" << endl;
			mqMeshToolsCore::instance()->Redo();
			rwi->Render();
		}

	}

	// Forward events
	vtkInteractorStyleTrackballCamera::OnKeyPress();
}
  void vtkMTInteractorStyle::OnKeyRelease()
  {
	  // Get the keypress
	  vtkRenderWindowInteractor *rwi = this->Interactor;
	  std::string key = rwi->GetKeySym();

	  // Output the key that was pressed
	  //std::cout << "Pressed " << key << std::endl;
	  if (key.compare("Control_L") == 0)
	  {
		  this->Ctrl = CTRL_RELEASED;
		 // std::cout << key << "Released" << '\n';
	  }
	  if (key.compare("l") == 0)
	  {
		  this->L = L_RELEASED;
		  // std::cout << key << "Released" << '\n';
	  }

	  // Forward events
	  vtkInteractorStyleTrackballCamera::OnKeyRelease();
  }
//--------------------------------------------------------------------------
void vtkMTInteractorStyle::OnChar()
{
  switch (this->Interactor->GetKeyCode())
  {
	
    case 'r':
    case 'R':
	{
		//r toggles the rubber band selection mode for mouse button 1
		if (this->CurrentMode == VTKISMT_ORIENT)
		{
			this->CurrentMode = VTKISMT_SELECT;
		}
		else
		{
			this->CurrentMode = VTKISMT_ORIENT;
		}
		break;
	}
    case 'p' :
    case 'P' :
    {
      vtkRenderWindowInteractor *rwi = this->Interactor;
      int *eventPos = rwi->GetEventPosition();
      this->FindPokedRenderer(eventPos[0], eventPos[1]);
      this->StartPosition[0] = eventPos[0];
      this->StartPosition[1] = eventPos[1];
      this->EndPosition[0] = eventPos[0];
      this->EndPosition[1] = eventPos[1];
      this->Pick();
      break;
    }
    default:
      this->Superclass::OnChar();
  }
}

void vtkMTInteractorStyle::Dolly(double factor)
{
	if (this->CurrentRenderer == NULL)
	{
		return;
	}

	vtkCamera *camera = this->CurrentRenderer->GetActiveCamera();
	if (camera->GetParallelProjection())
	{
		camera->SetParallelScale(camera->GetParallelScale() / factor);
		//cout << "Dolly camera parallell" << endl;
		camera->Dolly(factor);
	}
	else
	{
		camera->Dolly(factor);
		
	}
	if (this->AutoAdjustCameraClippingRange)
	{
		this->CurrentRenderer->ResetCameraClippingRange();
	}

	if (this->Interactor->GetLightFollowCamera())
	{
		this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
	}

	this->Interactor->Render();
}
void vtkMTInteractorStyle::RubberStart()
{
	if (!this->Interactor)
	{
		return;
	}

	//otherwise record the rubber band starting coordinate

	this->Moving = 1;

	vtkRenderWindow *renWin = this->Interactor->GetRenderWindow();

	this->StartPosition[0] = this->Interactor->GetEventPosition()[0];
	this->StartPosition[1] = this->Interactor->GetEventPosition()[1];
	this->EndPosition[0] = this->StartPosition[0];
	this->EndPosition[1] = this->StartPosition[1];

	this->PixelArray->Initialize();
	this->PixelArray->SetNumberOfComponents(4);
	int *size = renWin->GetSize();
	this->PixelArray->SetNumberOfTuples(size[0] * size[1]);

	renWin->GetRGBACharPixelData(0, 0, size[0] - 1, size[1] - 1, 1, this->PixelArray);

	this->FindPokedRenderer(this->StartPosition[0], this->StartPosition[1]);
}
//--------------------------------------------------------------------------
void vtkMTInteractorStyle::OnRightButtonDown()
{
	
	if (this->Ctrl != CTRL_PRESSED)
	{
		this->CurrentMode = VTKISMT_SELECT;
		this->RubberStart();
	}
	else
	{
		this->ActorsPositionsSaved = 0;//allow to save position
		int x = this->Interactor->GetEventPosition()[0];
		int y = this->Interactor->GetEventPosition()[1];

		this->FindPokedRenderer(x, y);
		if (this->CurrentRenderer == NULL)
		{
			return;
		}

		this->GrabFocus(this->EventCallbackCommand);
		this->NumberOfSelectedActors = this->getNumberOfSelectedActors();
		this->StartSpin();
	}

	
}
void vtkMTInteractorStyle::OnLeftButtonDown()
{
  if (this->CurrentMode != VTKISMT_SELECT)
  {
    //if not in rubber band mode, let the parent class handle it
	  //this->Interactor->GetControlKey()
	  if (this->Ctrl != CTRL_PRESSED)
	  {
		  if (this->L==L_PRESSED)
		  { 
		  
			  //int* clickPos = this->GetInteractor()->GetEventPosition();
			  int x = this->Interactor->GetEventPosition()[0];
			  int y = this->Interactor->GetEventPosition()[1];
			  std::cout << "Clicked at "
				  << x << " " << y   << std::endl;
			  if (this->CurrentRenderer != NULL)
			  {
				  std::cout << "Current renderer:" << this->CurrentRenderer << endl;
				  // Pick from this location.
				 /* vtkSmartPointer<vtkPropPicker>  picker =
					  vtkSmartPointer<vtkPropPicker>::New();*/
				
				  vtkSmartPointer<vtkCellPicker> picker =
					  vtkSmartPointer<vtkCellPicker>::New();

				  picker->Pick(x, y, 0, this->CurrentRenderer);


				 
				  double* pos = picker->GetPickPosition();
				  std::cout << "Pick position (world coordinates) is: "
					  << pos[0] << " " << pos[1]
					  << " " << pos[2] << std::endl;
				  double* norm = picker->GetPickNormal();
				  std::cout << "Pick normal : "
					  << norm[0] << " " << norm[1]
					  << " " << norm[2] << std::endl;

				  std::cout << "Picked actor: " << picker->GetActor() << std::endl;
				  if (picker->GetActor() == NULL) { cout << "Picked Null actor" << endl; }
				  else
				  {
					  //Create a LMActor
					  
					  VTK_CREATE(vtkLMActor, myLM);
					  int num = this->LandmarkCollection->GetNextLandmarkNumber();
					  myLM->SetLMOrigin(pos[0], pos[1], pos[2]);
					  myLM->SetLMOrientation(norm[0], norm[1], norm[2]);
					  myLM->SetLMSize(0.1);
					  myLM->SetLMType(1);
					  myLM->SetLMNumber(num);
					  myLM->SetLMBodyType(0);

					 
					  //Create a sphere
					  vtkSmartPointer<vtkSphereSource> sphereSource =
						  vtkSmartPointer<vtkSphereSource>::New();
					  sphereSource->SetCenter(pos[0], pos[1], pos[2]);
					  sphereSource->SetRadius(0.1);
					  sphereSource->Update();
					  //Create a mapper and actor
					  vtkSmartPointer<vtkPolyDataMapper> mapper =
						  vtkSmartPointer<vtkPolyDataMapper>::New();
					 // mapper->SetInputData(sphereSource->GetOutput());
					  mapper->SetInputData(myLM->getLMBody());
					  mapper->Update();					
					  myLM->SetMapper(mapper);
			
					  
					  //myLM->PrintSelf(cout, vtkIndent(1));
					  this->LandmarkCollection->AddItem(myLM);
					  //this->CurrentRenderer->AddActor(myLM);
					  this->Interactor->Render();
				  }

			  }
			  //this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetDefaultRenderer()->AddActor(actor);
			
		  
		  }
		  else
		  {
			  this->Superclass::OnLeftButtonDown();
		  }
	  }
	  else
	  {
		  this->ActorsPositionsSaved = 0;//allow to save position
		  int x = this->Interactor->GetEventPosition()[0];
		  int y = this->Interactor->GetEventPosition()[1];

		  this->FindPokedRenderer(x, y);
		  //this->FindPickedActor(x, y);
		  if (this->CurrentRenderer == NULL )
		  {
			  return;
		  }

		  this->GrabFocus(this->EventCallbackCommand);
		 
		  //cout << "Start Rotate CTRL" << endl;
		  this->NumberOfSelectedActors = this->getNumberOfSelectedActors();
		  this->StartRotate();
		 

	  }
    return;
  }
  this->RubberStart();

  
}

void vtkMTInteractorStyle::DeleteSelectedActors()
{
	this->ActorCollection->DeleteSelectedActors();
	this->Interactor->Render();
}
int vtkMTInteractorStyle::getNumberOfSelectedActors()
{
	this->ActorCollection->InitTraversal();
	int cpt = 0;
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());

		if (myActor->GetSelected() == 1)
		{
			cpt++;
		}
		
	}
	return cpt;
}

void vtkMTInteractorStyle::SaveSelectedActorsPositions()
{
	if (this->ActorsPositionsSaved == 0)
	{
		std::string action = "Action!";
		//cout << "State = " << this->State << endl;
		switch (this->State)
		{
		case VTKIS_ROTATE:
			action = "Rotate selected actors";
			//cout << "VTK_IS_ROTATE: action=" << action.c_str() << endl;
			break;
		case VTKIS_PAN:
			action = "Translate selected actors";
			//cout << "VTK_IS_PAN: action=" << action.c_str() << endl;
			break;
		case VTKIS_SPIN:
			action = "Spin selected actors";
			//cout << "VTK_IS_SPIN: action=" << action.c_str() << endl;
			break;
		}

		int Count = BEGIN_UNDO_SET(action);

		//cout << action.c_str() << endl;
		this->ActorCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				cout << "Call myActor Save Position with count"<<Count << endl;
				myActor->SaveState(Count);
			}
		}
		//vtkMeshToolsCore::instance()->getUndoStack()->
		
		END_UNDO_SET();
		this->ActorsPositionsSaved = 1;
	}

}
//--------------------------------------------------------------------------
void vtkMTInteractorStyle::OnMouseMove()
{
  if (this->CurrentMode != VTKISMT_SELECT )
  {
	  if (this->Ctrl != CTRL_PRESSED)
	  {
		  //let the parent class handle it
		  this->Superclass::OnMouseMove();
	  }
	  else
	  {
		 
		  //copied from Trackball Actor
		  int x = this->Interactor->GetEventPosition()[0];
		  int y = this->Interactor->GetEventPosition()[1];
		  //should be only once!
		  if (this->NumberOfSelectedActors>0 )
		  {
			  
			  this->SaveSelectedActorsPositions();
			  // cout << "We should increment the GLOBAL undo stack number now!";
		  }
		  switch (this->State)
		  {
		  case VTKIS_ROTATE:
			  this->FindPokedRenderer(x, y);
			  //cout << "ROTATE" << endl;
			  this->RotateActors();
			  this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
			  break;

		  case VTKIS_PAN:
			  this->FindPokedRenderer(x, y);
			  //cout << "PAN" << endl;
			  this->PanActors();
			  this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
			  break;		  

		  case VTKIS_SPIN:
			  this->FindPokedRenderer(x, y);
			  //cout << "SPIN" << endl;
			  this->SpinActors();
			  this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
			  break;

		
		  }
	  }
    return;
  }

  if (!this->Interactor || !this->Moving)
  {
    return;
  }

  this->EndPosition[0] = this->Interactor->GetEventPosition()[0];
  this->EndPosition[1] = this->Interactor->GetEventPosition()[1];
  int *size = this->Interactor->GetRenderWindow()->GetSize();
  if (this->EndPosition[0] > (size[0]-1))
  {
    this->EndPosition[0] = size[0]-1;
  }
  if (this->EndPosition[0] < 0)
  {
    this->EndPosition[0] = 0;
  }
  if (this->EndPosition[1] > (size[1]-1))
  {
    this->EndPosition[1] = size[1]-1;
  }
  if (this->EndPosition[1] < 0)
  {
    this->EndPosition[1] = 0;
  }
  this->RedrawRubberBand();
}

void vtkMTInteractorStyle::RubberStop()
{
	if (!this->Interactor || !this->Moving)
	{
		return;
	}

//otherwise record the rubber band end coordinate and then fire off a pick
if ((this->StartPosition[0] != this->EndPosition[0])
	|| (this->StartPosition[1] != this->EndPosition[1]))
{
	this->Pick();
}
this->Moving = 0;
}

void vtkMTInteractorStyle::OnRightButtonUp()
{	
	if (this->CurrentMode != VTKISMT_SELECT)
	{
		
		if (this->Ctrl != CTRL_PRESSED)
		{
			//if not in rubber band mode,  let the parent class handle it
			this->Superclass::OnRightButtonUp();
		}
		else
		{
			 // we allow to resave the positions later!
			switch (this->State)
			{
			case VTKIS_PAN:
				this->EndPan();
				break;

			case VTKIS_SPIN:
				this->EndSpin();
				break;

			case VTKIS_ROTATE:
				//cout << "End rotate" << endl;
				this->EndRotate();
				break;
			}

			if (this->Interactor)
			{
				this->ReleaseFocus();
			}
		}
		//if not in rubber band mode,  let the parent class handle it
		
		return;
	}
	this->RubberStop();	
	this->CurrentMode = VTKISMT_ORIENT;
}
//--------------------------------------------------------------------------
void vtkMTInteractorStyle::OnLeftButtonUp()
{
  if (this->CurrentMode != VTKISMT_SELECT)
  {
	  if (this->Ctrl != CTRL_PRESSED)
	  {
		  //if not in rubber band mode,  let the parent class handle it
		  this->Superclass::OnLeftButtonUp();
	  }
	else
	{
	  switch (this->State)
	  {
	  case VTKIS_PAN:
		  this->EndPan();
		  break;

	  case VTKIS_SPIN:
		  this->EndSpin();
		  break;

	  case VTKIS_ROTATE:
		  //cout << "End rotate" << endl;
		  this->EndRotate();
		  break;
	  }

	  if (this->Interactor)
	  {
		  this->ReleaseFocus();
	  }
  }
    return;
  }
  this->RubberStop();

}

void vtkMTInteractorStyle::OnMiddleButtonDown()
{
	if (this->Ctrl != CTRL_PRESSED)
	{
		//if not in rubber band mode,  let the parent class handle it
		this->Superclass::OnMiddleButtonDown();
	}
	else
	{
		
		this->ActorsPositionsSaved = 0;//allow to save position
		int x = this->Interactor->GetEventPosition()[0];
		int y = this->Interactor->GetEventPosition()[1];

		this->FindPokedRenderer(x, y);
		//this->FindPickedActor(x, y);
		if (this->CurrentRenderer == NULL 
			|| this->ActorCollection == NULL
			)
		{
			return;
		}

		this->GrabFocus(this->EventCallbackCommand);
		this->NumberOfSelectedActors = this->getNumberOfSelectedActors();
		this->StartPan();
		
	}
}

//----------------------------------------------------------------------------
void vtkMTInteractorStyle::OnMiddleButtonUp()
{
	if (this->CurrentMode != VTKISMT_SELECT)
	{

		if (this->Ctrl != CTRL_PRESSED)
		{
			//if not in rubber band mode,  let the parent class handle it
			this->Superclass::OnMiddleButtonUp();
		}
		else
		{
			switch (this->State)
			{
			case VTKIS_PAN:
				this->EndPan();
				break;

			case VTKIS_SPIN:
				this->EndSpin();
				break;

			case VTKIS_ROTATE:
				//cout << "End rotate" << endl;
				this->EndRotate();
				break;
			}

			if (this->Interactor)
			{
				this->ReleaseFocus();
			}
		}
		//if not in rubber band mode,  let the parent class handle it

		return;
	}

		
}

//--------------------------------------------------------------------------
void vtkMTInteractorStyle::RedrawRubberBand()
{
  //update the rubber band on the screen
  int *size = this->Interactor->GetRenderWindow()->GetSize();

  vtkUnsignedCharArray *tmpPixelArray = vtkUnsignedCharArray::New();
  tmpPixelArray->DeepCopy(this->PixelArray);
  unsigned char *pixels = tmpPixelArray->GetPointer(0);

  int min[2], max[2];

  min[0] = this->StartPosition[0] <= this->EndPosition[0] ?
    this->StartPosition[0] : this->EndPosition[0];
  if (min[0] < 0) { min[0] = 0; }
  if (min[0] >= size[0]) { min[0] = size[0] - 1; }

  min[1] = this->StartPosition[1] <= this->EndPosition[1] ?
    this->StartPosition[1] : this->EndPosition[1];
  if (min[1] < 0) { min[1] = 0; }
  if (min[1] >= size[1]) { min[1] = size[1] - 1; }

  max[0] = this->EndPosition[0] > this->StartPosition[0] ?
    this->EndPosition[0] : this->StartPosition[0];
  if (max[0] < 0) { max[0] = 0; }
  if (max[0] >= size[0]) { max[0] = size[0] - 1; }

  max[1] = this->EndPosition[1] > this->StartPosition[1] ?
    this->EndPosition[1] : this->StartPosition[1];
  if (max[1] < 0) { max[1] = 0; }
  if (max[1] >= size[1]) { max[1] = size[1] - 1; }

  int i;
  for (i = min[0]; i <= max[0]; i++)
  {
    pixels[4*(min[1]*size[0]+i)] = 255 ^ pixels[4*(min[1]*size[0]+i)];
    pixels[4*(min[1]*size[0]+i)+1] = 255 ^ pixels[4*(min[1]*size[0]+i)+1];
    pixels[4*(min[1]*size[0]+i)+2] = 0 ^ pixels[4*(min[1]*size[0]+i)+2];
    pixels[4*(max[1]*size[0]+i)] = 255 ^ pixels[4*(max[1]*size[0]+i)];
    pixels[4*(max[1]*size[0]+i)+1] = 255 ^ pixels[4*(max[1]*size[0]+i)+1];
    pixels[4*(max[1]*size[0]+i)+2] = 0^ pixels[4*(max[1]*size[0]+i)+2];
  }
  for (i = min[1]+1; i < max[1]; i++)
  {
    pixels[4*(i*size[0]+min[0])] = 255 ^ pixels[4*(i*size[0]+min[0])];
    pixels[4*(i*size[0]+min[0])+1] = 255 ^ pixels[4*(i*size[0]+min[0])+1];
    pixels[4*(i*size[0]+min[0])+2] = 0 ^ pixels[4*(i*size[0]+min[0])+2];
    pixels[4*(i*size[0]+max[0])] = 255 ^ pixels[4*(i*size[0]+max[0])];
    pixels[4*(i*size[0]+max[0])+1] = 255 ^ pixels[4*(i*size[0]+max[0])+1];
    pixels[4*(i*size[0]+max[0])+2] =0 ^ pixels[4*(i*size[0]+max[0])+2];
  }

  this->Interactor->GetRenderWindow()->SetRGBACharPixelData(0, 0, size[0]-1, size[1]-1, pixels, 0);
  this->Interactor->GetRenderWindow()->Frame();

  tmpPixelArray->Delete();
}

//--------------------------------------------------------------------------
void vtkMTInteractorStyle::Pick()
{
  //find rubber band lower left, upper right and center
  double rbcenter[3];
  int *size = this->Interactor->GetRenderWindow()->GetSize();
  int min[2], max[2];
  min[0] = this->StartPosition[0] <= this->EndPosition[0] ?
    this->StartPosition[0] : this->EndPosition[0];
  if (min[0] < 0) { min[0] = 0; }
  if (min[0] >= size[0]) { min[0] = size[0] - 2; }

  min[1] = this->StartPosition[1] <= this->EndPosition[1] ?
    this->StartPosition[1] : this->EndPosition[1];
  if (min[1] < 0) { min[1] = 0; }
  if (min[1] >= size[1]) { min[1] = size[1] - 2; }

  max[0] = this->EndPosition[0] > this->StartPosition[0] ?
    this->EndPosition[0] : this->StartPosition[0];
  if (max[0] < 0) { max[0] = 0; }
  if (max[0] >= size[0]) { max[0] = size[0] - 2; }

  max[1] = this->EndPosition[1] > this->StartPosition[1] ?
    this->EndPosition[1] : this->StartPosition[1];
  if (max[1] < 0) { max[1] = 0; }
  if (max[1] >= size[1]) { max[1] = size[1] - 2; }

  rbcenter[0] = (min[0] + max[0])/2.0;
  rbcenter[1] = (min[1] + max[1])/2.0;
  rbcenter[2] = 0;

  if (this->State == VTKIS_NONE)
  {
    //tell the RenderWindowInteractor's picker to make it happen
    vtkRenderWindowInteractor *rwi = this->Interactor;

    vtkAssemblyPath *path = NULL;
    rwi->StartPickCallback();
    vtkAbstractPropPicker *picker =
      vtkAbstractPropPicker::SafeDownCast(rwi->GetPicker());
    if ( picker != NULL )
    {
      vtkAreaPicker *areaPicker = vtkAreaPicker::SafeDownCast(picker);
      if (areaPicker != NULL)
      {
        areaPicker->AreaPick(min[0], min[1], max[0], max[1],
                             this->CurrentRenderer);
      }
      else
      {
        picker->Pick(rbcenter[0], rbcenter[1],
                     0.0, this->CurrentRenderer);
      }
      path = picker->GetPath();
    }
    if ( path == NULL )
    {
      this->HighlightProp(NULL);
      this->PropPicked = 0;
    }
    else
    {
      //highlight the one prop that the picker saved in the path
      //this->HighlightProp(path->GetFirstNode()->GetViewProp());
      this->PropPicked = 1;
    }
    rwi->EndPickCallback();
  }

  this->Interactor->Render();
}


void vtkMTInteractorStyle::RotateActors()
{
	if (this->CurrentRenderer == NULL 
		|| this->ActorCollection == NULL
		)
	{
		return;
	}

	vtkRenderWindowInteractor *rwi = this->Interactor;
	vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();

	// First get the origin of the assembly
	/*double *obj_center = this->InteractionProp->GetCenter();*/
	double *rot_center = this->ActorCollection->GetCenterOfMassOfSelectedActors();
	//cout << "Rotation center: " << rot_center[0] << "," << rot_center[1] << "," << rot_center[2] << endl;
	double boundRadius = this->ActorCollection->GetBoundingBoxLengthOfSelectedActors();
	//cout << "Bound Radius: " << boundRadius << endl;
	if (boundRadius == std::numeric_limits<double>::infinity())
	{
		boundRadius = 60;
	}
	else
	{
		boundRadius *= 0.5;
	}
	// GetLength gets the length of the diagonal of the bounding box
	/*double boundRadius = this->InteractionProp->GetLength() * 0.5;*/


	// Get the view up and view right vectors
	double view_up[3], view_look[3], view_right[3];

	cam->OrthogonalizeViewUp();
	cam->ComputeViewPlaneNormal();
	cam->GetViewUp(view_up);
	vtkMath::Normalize(view_up);
	cam->GetViewPlaneNormal(view_look);
	vtkMath::Cross(view_up, view_look, view_right);
	vtkMath::Normalize(view_right);

	// Get the furtherest point from object position+origin
	double outsidept[3];

	outsidept[0] = rot_center[0] + view_right[0] * boundRadius;
	outsidept[1] = rot_center[1] + view_right[1] * boundRadius;
	outsidept[2] = rot_center[2] + view_right[2] * boundRadius;

	// Convert them to display coord
	double disp_obj_center[3];

	this->ComputeWorldToDisplay(rot_center[0], rot_center[1], rot_center[2],
		disp_obj_center);

	this->ComputeWorldToDisplay(outsidept[0], outsidept[1], outsidept[2],
		outsidept);

	double radius = sqrt(vtkMath::Distance2BetweenPoints(disp_obj_center,
		outsidept));
	double nxf = (rwi->GetEventPosition()[0] - disp_obj_center[0]) / radius;

	double nyf = (rwi->GetEventPosition()[1] - disp_obj_center[1]) / radius;

	double oxf = (rwi->GetLastEventPosition()[0] - disp_obj_center[0]) / radius;

	double oyf = (rwi->GetLastEventPosition()[1] - disp_obj_center[1]) / radius;

	if (((nxf * nxf + nyf * nyf) <= 1.0) &&
		((oxf * oxf + oyf * oyf) <= 1.0))
	{
		double newXAngle = vtkMath::DegreesFromRadians(asin(nxf));
		double newYAngle = vtkMath::DegreesFromRadians(asin(nyf));
		double oldXAngle = vtkMath::DegreesFromRadians(asin(oxf));
		double oldYAngle = vtkMath::DegreesFromRadians(asin(oyf));

		double scale[3];
		scale[0] = scale[1] = scale[2] = 1.0;

		double **rotate = new double*[2];

		rotate[0] = new double[4];
		rotate[1] = new double[4];

		rotate[0][0] = newXAngle - oldXAngle;
		rotate[0][1] = view_up[0];
		rotate[0][2] = view_up[1];
		rotate[0][3] = view_up[2];

		rotate[1][0] = oldYAngle - newYAngle;
		rotate[1][1] = view_right[0];
		rotate[1][2] = view_right[1];
		rotate[1][3] = view_right[2];

		this->ActorCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				//cout << "Apply prop3Dtransform" << endl;
				for (vtkIdType j = 0; j < 2; j++)
				{
					for (vtkIdType k = 0; k < 4; k++)
					{
						//cout << "rotate["<<j<<"]"<<"["<<k<<"]="<< rotate[j][k] << endl;

					}
				}
				
				//cout << "scale:" << scale[0] << ","<< scale[1] << ","<< scale[2] << endl;
				
				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				myActor->SetChanged(1);
			}
		}
		

		delete[] rotate[0];
		delete[] rotate[1];
		delete[] rotate;

		if (this->AutoAdjustCameraClippingRange)
		{
			this->CurrentRenderer->ResetCameraClippingRange();
		}

		rwi->Render();
	}
}

//----------------------------------------------------------------------------
void vtkMTInteractorStyle::SpinActors()
{
	if (this->CurrentRenderer == NULL 
		|| this->ActorCollection == NULL
		)
	{
		return;
	}

	vtkRenderWindowInteractor *rwi = this->Interactor;
	vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();

	// Get the axis to rotate around = vector from eye to origin

	double *spin_center = this->ActorCollection->GetCenterOfMassOfSelectedActors();
	//double obj_center[3] = { 0,0, 0 };
	
	double motion_vector[3];
	double view_point[3];

	if (cam->GetParallelProjection())
	{
		// If parallel projection, want to get the view plane normal...
		cam->ComputeViewPlaneNormal();
		cam->GetViewPlaneNormal(motion_vector);
	}
	else
	{
		// Perspective projection, get vector from eye to center of actor
		cam->GetPosition(view_point);
		motion_vector[0] = view_point[0] - spin_center[0];
		motion_vector[1] = view_point[1] - spin_center[1];
		motion_vector[2] = view_point[2] - spin_center[2];
		vtkMath::Normalize(motion_vector);
	}

	double disp_obj_center[3];

	this->ComputeWorldToDisplay(spin_center[0], spin_center[1], spin_center[2],
		disp_obj_center);

	double newAngle =
		vtkMath::DegreesFromRadians(atan2(rwi->GetEventPosition()[1] - disp_obj_center[1],
			rwi->GetEventPosition()[0] - disp_obj_center[0]));

	double oldAngle =
		vtkMath::DegreesFromRadians(atan2(rwi->GetLastEventPosition()[1] - disp_obj_center[1],
			rwi->GetLastEventPosition()[0] - disp_obj_center[0]));

	double scale[3];
	scale[0] = scale[1] = scale[2] = 1.0;

	double **rotate = new double*[1];
	rotate[0] = new double[4];

	rotate[0][0] = newAngle - oldAngle;
	rotate[0][1] = motion_vector[0];
	rotate[0][2] = motion_vector[1];
	rotate[0][3] = motion_vector[2];

	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMTActor *myActor= vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
		vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
		if (myActor->GetSelected() == 1)
		{
			this->Prop3DTransform(myPropr,
				spin_center,
				1,
				rotate,
				scale);
			myActor->SetChanged(1);
		}
		
	}
	

	delete[] rotate[0];
	delete[] rotate;

	if (this->AutoAdjustCameraClippingRange)
	{
		this->CurrentRenderer->ResetCameraClippingRange();
	}

	rwi->Render();
}

//----------------------------------------------------------------------------
void vtkMTInteractorStyle::PanActors()
{
	if (this->CurrentRenderer == NULL 
		|| this->ActorCollection == NULL)
	{
		return;
	}

	vtkRenderWindowInteractor *rwi = this->Interactor;

	// Use initial center as the origin from which to pan

	double *pan_center = this->ActorCollection->GetCenterOfMassOfSelectedActors();

	double disp_obj_center[3], new_pick_point[4];
	double old_pick_point[4], motion_vector[3];

	this->ComputeWorldToDisplay(pan_center[0], pan_center[1], pan_center[2],
		disp_obj_center);

	this->ComputeDisplayToWorld(rwi->GetEventPosition()[0],
		rwi->GetEventPosition()[1],
		disp_obj_center[2],
		new_pick_point);

	this->ComputeDisplayToWorld(rwi->GetLastEventPosition()[0],
		rwi->GetLastEventPosition()[1],
		disp_obj_center[2],
		old_pick_point);

	motion_vector[0] = new_pick_point[0] - old_pick_point[0];
	motion_vector[1] = new_pick_point[1] - old_pick_point[1];
	motion_vector[2] = new_pick_point[2] - old_pick_point[2];

	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
		vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
		if (myActor->GetSelected() == 1)
		{
			if (myPropr->GetUserMatrix() != NULL)
			{
				vtkTransform *t = vtkTransform::New();
				t->PostMultiply();
				t->SetMatrix(myPropr->GetUserMatrix());
				t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
				myPropr->GetUserMatrix()->DeepCopy(t->GetMatrix());
				t->Delete();
			}
			else
			{
				myPropr->AddPosition(motion_vector[0],
					motion_vector[1],
					motion_vector[2]);
			}
			myActor->SetChanged(1);
		}
	}

	

	if (this->AutoAdjustCameraClippingRange)
	{
		this->CurrentRenderer->ResetCameraClippingRange();
	}

	rwi->Render();
}



//--------------------------------------------------------------------------
void vtkMTInteractorStyle::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
void vtkMTInteractorStyle::Prop3DTransform(vtkProp3D *prop3D,
	double *boxCenter,
	int numRotation,
	double **rotate,
	double *scale)
{
	vtkMatrix4x4 *oldMatrix = vtkMatrix4x4::New();
	prop3D->GetMatrix(oldMatrix);

	double orig[3];
	prop3D->GetOrigin(orig);

	vtkTransform *newTransform = vtkTransform::New();
	newTransform->PostMultiply();
	if (prop3D->GetUserMatrix() != NULL)
	{
		newTransform->SetMatrix(prop3D->GetUserMatrix());
	}
	else
	{
		newTransform->SetMatrix(oldMatrix);
	}

	newTransform->Translate(-(boxCenter[0]), -(boxCenter[1]), -(boxCenter[2]));

	for (int i = 0; i < numRotation; i++)
	{
		newTransform->RotateWXYZ(rotate[i][0], rotate[i][1],
			rotate[i][2], rotate[i][3]);
	}

	if ((scale[0] * scale[1] * scale[2]) != 0.0)
	{
		newTransform->Scale(scale[0], scale[1], scale[2]);
	}

	newTransform->Translate(boxCenter[0], boxCenter[1], boxCenter[2]);

	// now try to get the composit of translate, rotate, and scale
	newTransform->Translate(-(orig[0]), -(orig[1]), -(orig[2]));
	newTransform->PreMultiply();
	newTransform->Translate(orig[0], orig[1], orig[2]);

	if (prop3D->GetUserMatrix() != NULL)
	{
		// apply new transform to prop3D
		newTransform->GetMatrix(prop3D->GetUserMatrix());
	}
	else
	{
		prop3D->SetPosition(newTransform->GetPosition());
		prop3D->SetScale(newTransform->GetScale());
		prop3D->SetOrientation(newTransform->GetOrientation());
	}
	oldMatrix->Delete();
	newTransform->Delete();
}