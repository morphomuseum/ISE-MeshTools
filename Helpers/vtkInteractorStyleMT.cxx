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
#include "vtkInteractorStyleMT.h"

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

vtkStandardNewMacro(vtkInteractorStyleMT);

#define VTKISMT_ORIENT 0
#define VTKISMT_SELECT 1
#define CTRL_RELEASED 0
#define CTRL_PRESSED 1
//--------------------------------------------------------------------------

vtkInteractorStyleMT::vtkInteractorStyleMT()
{
	this->CurrentMode = VTKISMT_ORIENT;
	this->Ctrl = CTRL_RELEASED;
	this->StartPosition[0] = this->StartPosition[1] = 0;
	this->EndPosition[0] = this->EndPosition[1] = 0;
	this->Moving = 0;
	this->PixelArray = vtkUnsignedCharArray::New();
	this->ActorCollection = vtkSmartPointer<vtkActorCollection>::New();
}

void vtkInteractorStyleMT::SetActorCollection(vtkSmartPointer<vtkActorCollection> ActColl)
{
	this->ActorCollection = ActColl;
}

//--------------------------------------------------------------------------
vtkInteractorStyleMT::~vtkInteractorStyleMT()
{
  this->PixelArray->Delete();
}

//--------------------------------------------------------------------------
void vtkInteractorStyleMT::StartSelect()
{
	this->CurrentMode = VTKISMT_SELECT;

}
//--------------------------------------------------------------------------
  void vtkInteractorStyleMT::OnKeyPress()
  {
	// Get the keypress
	vtkRenderWindowInteractor *rwi = this->Interactor;
	std::string key = rwi->GetKeySym();
	
	// Output the key that was pressed
	//std::cout << "Pressed " << key << std::endl;
	if (key.compare("Control_L") == 0)
	{
		this->Ctrl = CTRL_PRESSED;
		std::cout << key<< "Pressed" << '\n';
	}


	// Forward events
	vtkInteractorStyleTrackballCamera::OnKeyPress();
}
  void vtkInteractorStyleMT::OnKeyRelease()
  {
	  // Get the keypress
	  vtkRenderWindowInteractor *rwi = this->Interactor;
	  std::string key = rwi->GetKeySym();

	  // Output the key that was pressed
	  //std::cout << "Pressed " << key << std::endl;
	  if (key.compare("Control_L") == 0)
	  {
		  this->Ctrl = CTRL_RELEASED;
		  std::cout << key << "Released" << '\n';
	  }


	  // Forward events
	  vtkInteractorStyleTrackballCamera::OnKeyRelease();
  }
//--------------------------------------------------------------------------
void vtkInteractorStyleMT::OnChar()
{
  switch (this->Interactor->GetKeyCode())
  {
    case 'r':
    case 'R':
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

void vtkInteractorStyleMT::RubberStart()
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
void vtkInteractorStyleMT::OnRightButtonDown()
{
	this->CurrentMode = VTKISMT_SELECT;
	this->RubberStart();
}
void vtkInteractorStyleMT::OnLeftButtonDown()
{
  if (this->CurrentMode != VTKISMT_SELECT)
  {
    //if not in rubber band mode, let the parent class handle it
	  //this->Interactor->GetControlKey()
	  if (this->Ctrl != CTRL_PRESSED)
	  {
		  this->Superclass::OnLeftButtonDown();
	  }
	  else
	  {

		  int x = this->Interactor->GetEventPosition()[0];
		  int y = this->Interactor->GetEventPosition()[1];

		  this->FindPokedRenderer(x, y);
		  //this->FindPickedActor(x, y);
		  if (this->CurrentRenderer == NULL )
		  {
			  return;
		  }

		  this->GrabFocus(this->EventCallbackCommand);
		 
		  cout << "StartSpin CTRL" << endl;
		  this->StartSpin();
		 

	  }
    return;
  }
  this->RubberStart();

  
}


//--------------------------------------------------------------------------
void vtkInteractorStyleMT::OnMouseMove()
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

		  switch (this->State)
		  {
		  case VTKIS_ROTATE:
			  this->FindPokedRenderer(x, y);
			  this->RotateActors();
			  this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
			  break;

		  case VTKIS_PAN:
			  this->FindPokedRenderer(x, y);
			  this->PanActors();
			  this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
			  break;

		  case VTKIS_DOLLY:
			  this->FindPokedRenderer(x, y);
			  this->DollyActors();
			  this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
			  break;

		  case VTKIS_SPIN:
			  this->FindPokedRenderer(x, y);
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

void vtkInteractorStyleMT::RubberStop()
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

void vtkInteractorStyleMT::OnRightButtonUp()
{
	

	if (this->CurrentMode != VTKISMT_SELECT)
	{
		//if not in rubber band mode,  let the parent class handle it
		this->Superclass::OnRightButtonUp();
		return;
	}
	this->RubberStop();
	
	this->CurrentMode = VTKISMT_ORIENT;
}
//--------------------------------------------------------------------------
void vtkInteractorStyleMT::OnLeftButtonUp()
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

void vtkInteractorStyleMT::OnMiddleButtonDown()
{
	if (this->Ctrl != CTRL_PRESSED)
	{
		//if not in rubber band mode,  let the parent class handle it
		this->Superclass::OnMiddleButtonDown();
	}
	else
	{
		int x = this->Interactor->GetEventPosition()[0];
		int y = this->Interactor->GetEventPosition()[1];

		this->FindPokedRenderer(x, y);
		//this->FindPickedActor(x, y);
		if (this->CurrentRenderer == NULL 
			//|| this->InteractionProp == NULL
			)
		{
			return;
		}

		this->GrabFocus(this->EventCallbackCommand);
		if (this->Interactor->GetControlKey())
		{
			this->StartDolly();
		}
		else
		{
			this->StartPan();
		}
	}
}

//----------------------------------------------------------------------------
void vtkInteractorStyleMT::OnMiddleButtonUp()
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
		case VTKIS_DOLLY:
			this->EndDolly();
			break;

		case VTKIS_PAN:
			this->EndPan();
			break;
		}

		if (this->Interactor)
		{
			this->ReleaseFocus();
		}
	}
}

//--------------------------------------------------------------------------
void vtkInteractorStyleMT::RedrawRubberBand()
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
void vtkInteractorStyleMT::Pick()
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


void vtkInteractorStyleMT::RotateActors()
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
	double obj_center[3] = { 0,0, 0 };
	double boundRadius = 10;
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

	outsidept[0] = obj_center[0] + view_right[0] * boundRadius;
	outsidept[1] = obj_center[1] + view_right[1] * boundRadius;
	outsidept[2] = obj_center[2] + view_right[2] * boundRadius;

	// Convert them to display coord
	double disp_obj_center[3];

	this->ComputeWorldToDisplay(obj_center[0], obj_center[1], obj_center[2],
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


		/*this->Prop3DTransform(this->InteractionProp,
			obj_center,
			2,
			rotate,
			scale);*/

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
void vtkInteractorStyleMT::SpinActors()
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

	/*double *obj_center = this->InteractionProp->GetCenter();*/
	double obj_center[3] = { 0,0, 0 };
	
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
		motion_vector[0] = view_point[0] - obj_center[0];
		motion_vector[1] = view_point[1] - obj_center[1];
		motion_vector[2] = view_point[2] - obj_center[2];
		vtkMath::Normalize(motion_vector);
	}

	double disp_obj_center[3];

	this->ComputeWorldToDisplay(obj_center[0], obj_center[1], obj_center[2],
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

	/*this->Prop3DTransform(this->InteractionProp,
		obj_center,
		1,
		rotate,
		scale);*/

	delete[] rotate[0];
	delete[] rotate;

	if (this->AutoAdjustCameraClippingRange)
	{
		this->CurrentRenderer->ResetCameraClippingRange();
	}

	rwi->Render();
}

//----------------------------------------------------------------------------
void vtkInteractorStyleMT::PanActors()
{
	if (this->CurrentRenderer == NULL 
		|| this->ActorCollection == NULL)
	{
		return;
	}

	vtkRenderWindowInteractor *rwi = this->Interactor;

	// Use initial center as the origin from which to pan

	/*double *obj_center = this->InteractionProp->GetCenter();*/

	double disp_obj_center[3], new_pick_point[4];
	double old_pick_point[4], motion_vector[3];

/*	this->ComputeWorldToDisplay(obj_center[0], obj_center[1], obj_center[2],
		disp_obj_center);*/

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

	/*if (this->InteractionProp->GetUserMatrix() != NULL)
	{
		vtkTransform *t = vtkTransform::New();
		t->PostMultiply();
		t->SetMatrix(this->InteractionProp->GetUserMatrix());
		t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
		this->InteractionProp->GetUserMatrix()->DeepCopy(t->GetMatrix());
		t->Delete();
	}
	else
	{
		this->InteractionProp->AddPosition(motion_vector[0],
			motion_vector[1],
			motion_vector[2]);
	}*/

	if (this->AutoAdjustCameraClippingRange)
	{
		this->CurrentRenderer->ResetCameraClippingRange();
	}

	rwi->Render();
}

//----------------------------------------------------------------------------
void vtkInteractorStyleMT::DollyActors()
{
	if (this->CurrentRenderer == NULL 
		|| this->ActorCollection== NULL)
	{
		return;
	}

	vtkRenderWindowInteractor *rwi = this->Interactor;
	vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();

	double view_point[3], view_focus[3];
	double motion_vector[3];

	cam->GetPosition(view_point);
	cam->GetFocalPoint(view_focus);

	double *center = this->CurrentRenderer->GetCenter();

	int dy = rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1];
	double yf = dy / center[1] * this->MotionFactor;
	double dollyFactor = pow(1.1, yf);

	dollyFactor -= 1.0;
	motion_vector[0] = (view_point[0] - view_focus[0]) * dollyFactor;
	motion_vector[1] = (view_point[1] - view_focus[1]) * dollyFactor;
	motion_vector[2] = (view_point[2] - view_focus[2]) * dollyFactor;

	/*if (this->InteractionProp->GetUserMatrix() != NULL)
	{
		vtkTransform *t = vtkTransform::New();
		t->PostMultiply();
		t->SetMatrix(this->InteractionProp->GetUserMatrix());
		t->Translate(motion_vector[0], motion_vector[1],
			motion_vector[2]);
		this->InteractionProp->GetUserMatrix()->DeepCopy(t->GetMatrix());
		t->Delete();
	}
	else
	{
		this->InteractionProp->AddPosition(motion_vector);
	}*/

	if (this->AutoAdjustCameraClippingRange)
	{
		this->CurrentRenderer->ResetCameraClippingRange();
	}

	rwi->Render();
}

//--------------------------------------------------------------------------
void vtkInteractorStyleMT::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
