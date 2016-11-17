/*=========================================================================

Program:  ISE-MeshTools
Module:    vtkOrientationHelperActor.cxx


=========================================================================*/
#include "vtkOrientationHelperActor.h"

#include <vtkActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkConeSource.h>
#include <vtkCylinderSource.h>
#include <vtkLineSource.h>
#include <vtkObject.h>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropCollection.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>

vtkStandardNewMacro(vtkOrientationHelperActor);

vtkCxxSetObjectMacro(vtkOrientationHelperActor, UserDefinedTip, vtkPolyData);
vtkCxxSetObjectMacro(vtkOrientationHelperActor, UserDefinedShaft, vtkPolyData);

//----------------------------------------------------------------------------
vtkOrientationHelperActor::vtkOrientationHelperActor()
{
	this->AxisLabels = 1;

	this->XAxisLabelText = NULL;
	this->YAxisLabelText = NULL;
	this->ZAxisLabelText = NULL;

	this->X2AxisLabelText = NULL;
	this->Y2AxisLabelText = NULL;
	this->Z2AxisLabelText = NULL;

	this->SetXAxisLabelText("X");
	this->SetYAxisLabelText("Y");
	this->SetZAxisLabelText("Z");

	this->SetX2AxisLabelText("-X");
	this->SetY2AxisLabelText("-Y");
	this->SetZ2AxisLabelText("-Z");

	this->XAxisShaft = vtkActor::New();
	this->XAxisShaft->GetProperty()->SetColor(1, 0, 0);

	this->YAxisShaft = vtkActor::New();
	this->YAxisShaft->GetProperty()->SetColor(0, 1, 0);
	
	this->ZAxisShaft = vtkActor::New();
	this->ZAxisShaft->GetProperty()->SetColor(0, 0, 1);


	this->X2AxisShaft = vtkActor::New();
	this->X2AxisShaft->GetProperty()->SetColor(1, 1, 0);

	this->Y2AxisShaft = vtkActor::New();
	this->Y2AxisShaft->GetProperty()->SetColor(0, 1 ,1);

	this->Z2AxisShaft = vtkActor::New();
	this->Z2AxisShaft->GetProperty()->SetColor(1, 0, 1);
	
	this->XAxisTip = vtkActor::New();
	this->XAxisTip->GetProperty()->SetColor(1, 0, 0);
	this->YAxisTip = vtkActor::New();
	this->YAxisTip->GetProperty()->SetColor(0, 1, 0);
	this->ZAxisTip = vtkActor::New();
	this->ZAxisTip->GetProperty()->SetColor(0, 0, 1);


	this->X2AxisTip = vtkActor::New();
	this->X2AxisTip->GetProperty()->SetColor(1, 1, 0);
	this->Y2AxisTip = vtkActor::New();
	this->Y2AxisTip->GetProperty()->SetColor(0, 1, 1);
	this->Z2AxisTip = vtkActor::New();
	this->Z2AxisTip->GetProperty()->SetColor(1, 0, 1);

	this->CylinderSource = vtkCylinderSource::New();
	this->CylinderSource->SetHeight(1.0);

	this->LineSource = vtkLineSource::New();
	this->LineSource->SetPoint1(0.0, 0.0, 0.0);
	this->LineSource->SetPoint2(0.0, 1.0, 0.0);

	this->ConeSource = vtkConeSource::New();
	this->ConeSource->SetDirection(0, 1, 0);
	this->ConeSource->SetHeight(1.0);

	this->SphereSource = vtkSphereSource::New();

	vtkPolyDataMapper *shaftMapper = vtkPolyDataMapper::New();

	this->XAxisShaft->SetMapper(shaftMapper);
	this->YAxisShaft->SetMapper(shaftMapper);
	this->ZAxisShaft->SetMapper(shaftMapper);
	this->X2AxisShaft->SetMapper(shaftMapper);
	this->Y2AxisShaft->SetMapper(shaftMapper);
	this->Z2AxisShaft->SetMapper(shaftMapper);
	shaftMapper->Delete();

	vtkPolyDataMapper *tipMapper = vtkPolyDataMapper::New();

	this->XAxisTip->SetMapper(tipMapper);
	this->YAxisTip->SetMapper(tipMapper);
	this->ZAxisTip->SetMapper(tipMapper);
	this->X2AxisTip->SetMapper(tipMapper);
	this->Y2AxisTip->SetMapper(tipMapper);
	this->Z2AxisTip->SetMapper(tipMapper);

	tipMapper->Delete();

	this->TotalLength[0] = 1.0;
	this->TotalLength[1] = 1.0;
	this->TotalLength[2] = 1.0;
	
	

	this->NormalizedShaftLength[0] = 0.8;
	this->NormalizedShaftLength[1] = 0.8;
	this->NormalizedShaftLength[2] = 0.8;

	
	this->NormalizedTipLength[0] = 0.2;
	this->NormalizedTipLength[1] = 0.2;
	this->NormalizedTipLength[2] = 0.2;
	
	

	this->NormalizedLabelPosition[0] = 1.0;
	this->NormalizedLabelPosition[1] = 1.0;
	this->NormalizedLabelPosition[2] = 1.0;

	this->ConeResolution = 16;
	this->SphereResolution = 16;
	this->CylinderResolution = 16;

	this->ConeRadius = 0.4;
	this->SphereRadius = 0.5;
	this->CylinderRadius = 0.05;

	this->ShaftType = vtkOrientationHelperActor::LINE_SHAFT;
	this->TipType = vtkOrientationHelperActor::CONE_TIP;

	this->UserDefinedTip = NULL;
	this->UserDefinedShaft = NULL;

	this->XAxisLabel = vtkCaptionActor2D::New();
	this->YAxisLabel = vtkCaptionActor2D::New();
	this->ZAxisLabel = vtkCaptionActor2D::New();

	this->X2AxisLabel = vtkCaptionActor2D::New();
	this->Y2AxisLabel = vtkCaptionActor2D::New();
	this->Z2AxisLabel = vtkCaptionActor2D::New();


	this->XAxisLabel->ThreeDimensionalLeaderOff();
	this->XAxisLabel->LeaderOff();
	this->XAxisLabel->BorderOff();
	this->XAxisLabel->SetPosition(0, 0);

	this->X2AxisLabel->ThreeDimensionalLeaderOff();
	this->X2AxisLabel->LeaderOff();
	this->X2AxisLabel->BorderOff();
	this->X2AxisLabel->SetPosition(0, 0);

	this->YAxisLabel->ThreeDimensionalLeaderOff();
	this->YAxisLabel->LeaderOff();
	this->YAxisLabel->BorderOff();
	this->YAxisLabel->SetPosition(0, 0);

	this->Y2AxisLabel->ThreeDimensionalLeaderOff();
	this->Y2AxisLabel->LeaderOff();
	this->Y2AxisLabel->BorderOff();
	this->Y2AxisLabel->SetPosition(0, 0);

	this->ZAxisLabel->ThreeDimensionalLeaderOff();
	this->ZAxisLabel->LeaderOff();
	this->ZAxisLabel->BorderOff();
	this->ZAxisLabel->SetPosition(0, 0);

	this->Z2AxisLabel->ThreeDimensionalLeaderOff();
	this->Z2AxisLabel->LeaderOff();
	this->Z2AxisLabel->BorderOff();
	this->Z2AxisLabel->SetPosition(0, 0);

	this->UpdateProps();
}

//----------------------------------------------------------------------------
vtkOrientationHelperActor::~vtkOrientationHelperActor()
{
	this->CylinderSource->Delete();
	this->LineSource->Delete();
	this->ConeSource->Delete();
	this->SphereSource->Delete();

	this->XAxisShaft->Delete();
	this->YAxisShaft->Delete();
	this->ZAxisShaft->Delete();

	this->X2AxisShaft->Delete();
	this->Y2AxisShaft->Delete();
	this->Z2AxisShaft->Delete();


	this->XAxisTip->Delete();
	this->YAxisTip->Delete();
	this->ZAxisTip->Delete();
	this->X2AxisTip->Delete();
	this->Y2AxisTip->Delete();
	this->Z2AxisTip->Delete();


	this->SetUserDefinedTip(NULL);
	this->SetUserDefinedShaft(NULL);

	this->SetXAxisLabelText(NULL);
	this->SetYAxisLabelText(NULL);
	this->SetZAxisLabelText(NULL);
	this->SetX2AxisLabelText(NULL);
	this->SetY2AxisLabelText(NULL);
	this->SetZ2AxisLabelText(NULL);

	this->XAxisLabel->Delete();
	this->YAxisLabel->Delete();
	this->ZAxisLabel->Delete();

	this->X2AxisLabel->Delete();
	this->Y2AxisLabel->Delete();
	this->Z2AxisLabel->Delete();
}

//----------------------------------------------------------------------------
// Shallow copy of an actor.
void vtkOrientationHelperActor::ShallowCopy(vtkProp *prop)
{
	vtkOrientationHelperActor *a = vtkOrientationHelperActor::SafeDownCast(prop);
	if (a != NULL)
	{
		this->SetAxisLabels(a->GetAxisLabels());
		this->SetXAxisLabelText(a->GetXAxisLabelText());
		this->SetYAxisLabelText(a->GetYAxisLabelText());
		this->SetZAxisLabelText(a->GetZAxisLabelText());
		this->SetX2AxisLabelText(a->GetX2AxisLabelText());
		this->SetY2AxisLabelText(a->GetY2AxisLabelText());
		this->SetZ2AxisLabelText(a->GetZ2AxisLabelText());

		this->SetTotalLength(a->GetTotalLength());
		this->SetNormalizedShaftLength(a->GetNormalizedShaftLength());
		this->SetNormalizedTipLength(a->GetNormalizedTipLength());
		this->SetNormalizedLabelPosition(a->GetNormalizedLabelPosition());
		this->SetConeResolution(a->GetConeResolution());
		this->SetSphereResolution(a->GetSphereResolution());
		this->SetCylinderResolution(a->GetCylinderResolution());
		this->SetConeRadius(a->GetConeRadius());
		this->SetSphereRadius(a->GetSphereRadius());
		this->SetCylinderRadius(a->GetCylinderRadius());
		this->SetTipType(a->GetTipType());
		this->SetShaftType(a->GetShaftType());
		this->SetUserDefinedTip(a->GetUserDefinedTip());
		this->SetUserDefinedShaft(a->GetUserDefinedShaft());
	}

	// Now do superclass
	this->vtkProp3D::ShallowCopy(prop);
}

//----------------------------------------------------------------------------
void vtkOrientationHelperActor::GetActors(vtkPropCollection *ac)
{
	ac->AddItem(this->XAxisShaft);
	ac->AddItem(this->YAxisShaft);
	ac->AddItem(this->ZAxisShaft);
	ac->AddItem(this->X2AxisShaft);
	ac->AddItem(this->Y2AxisShaft);
	ac->AddItem(this->Z2AxisShaft);

	ac->AddItem(this->XAxisTip);
	ac->AddItem(this->YAxisTip);
	ac->AddItem(this->ZAxisTip);
	ac->AddItem(this->X2AxisTip);
	ac->AddItem(this->Y2AxisTip);
	ac->AddItem(this->Z2AxisTip);
}

//----------------------------------------------------------------------------
int vtkOrientationHelperActor::RenderOpaqueGeometry(vtkViewport *vp)
{
	int renderedSomething = 0;

	this->UpdateProps();

	renderedSomething += this->XAxisShaft->RenderOpaqueGeometry(vp);
	renderedSomething += this->YAxisShaft->RenderOpaqueGeometry(vp);
	renderedSomething += this->ZAxisShaft->RenderOpaqueGeometry(vp);
	renderedSomething += this->X2AxisShaft->RenderOpaqueGeometry(vp);
	renderedSomething += this->Y2AxisShaft->RenderOpaqueGeometry(vp);
	renderedSomething += this->Z2AxisShaft->RenderOpaqueGeometry(vp);

	renderedSomething += this->XAxisTip->RenderOpaqueGeometry(vp);
	renderedSomething += this->YAxisTip->RenderOpaqueGeometry(vp);
	renderedSomething += this->ZAxisTip->RenderOpaqueGeometry(vp);
	renderedSomething += this->X2AxisTip->RenderOpaqueGeometry(vp);
	renderedSomething += this->Y2AxisTip->RenderOpaqueGeometry(vp);
	renderedSomething += this->Z2AxisTip->RenderOpaqueGeometry(vp);

	if (this->AxisLabels)
	{
		renderedSomething += this->XAxisLabel->RenderOpaqueGeometry(vp);
		renderedSomething += this->YAxisLabel->RenderOpaqueGeometry(vp);
		renderedSomething += this->ZAxisLabel->RenderOpaqueGeometry(vp);
		renderedSomething += this->X2AxisLabel->RenderOpaqueGeometry(vp);
		renderedSomething += this->Y2AxisLabel->RenderOpaqueGeometry(vp);
		renderedSomething += this->Z2AxisLabel->RenderOpaqueGeometry(vp);
	}

	renderedSomething = (renderedSomething > 0) ? (1) : (0);
	return renderedSomething;
}

//-----------------------------------------------------------------------------
int vtkOrientationHelperActor::RenderTranslucentPolygonalGeometry(vtkViewport *vp)
{
	int renderedSomething = 0;

	this->UpdateProps();

	renderedSomething += this->XAxisShaft->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->YAxisShaft->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->ZAxisShaft->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->X2AxisShaft->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->Y2AxisShaft->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->Z2AxisShaft->RenderTranslucentPolygonalGeometry(vp);

	renderedSomething += this->XAxisTip->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->YAxisTip->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->ZAxisTip->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->X2AxisTip->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->Y2AxisTip->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->Z2AxisTip->RenderTranslucentPolygonalGeometry(vp);

	if (this->AxisLabels)
	{
		renderedSomething += this->XAxisLabel->RenderTranslucentPolygonalGeometry(vp);
		renderedSomething += this->YAxisLabel->RenderTranslucentPolygonalGeometry(vp);
		renderedSomething += this->ZAxisLabel->RenderTranslucentPolygonalGeometry(vp);
		renderedSomething += this->X2AxisLabel->RenderTranslucentPolygonalGeometry(vp);
		renderedSomething += this->Y2AxisLabel->RenderTranslucentPolygonalGeometry(vp);
		renderedSomething += this->Z2AxisLabel->RenderTranslucentPolygonalGeometry(vp);
	}

	renderedSomething = (renderedSomething > 0) ? (1) : (0);
	return renderedSomething;
}

//-----------------------------------------------------------------------------
// Description:
// Does this prop have some translucent polygonal geometry?
int vtkOrientationHelperActor::HasTranslucentPolygonalGeometry()
{
	int result = 0;

	this->UpdateProps();

	result |= this->XAxisShaft->HasTranslucentPolygonalGeometry();
	result |= this->YAxisShaft->HasTranslucentPolygonalGeometry();
	result |= this->ZAxisShaft->HasTranslucentPolygonalGeometry();

	result |= this->X2AxisShaft->HasTranslucentPolygonalGeometry();
	result |= this->Y2AxisShaft->HasTranslucentPolygonalGeometry();
	result |= this->Z2AxisShaft->HasTranslucentPolygonalGeometry();


	result |= this->XAxisTip->HasTranslucentPolygonalGeometry();
	result |= this->YAxisTip->HasTranslucentPolygonalGeometry();
	result |= this->ZAxisTip->HasTranslucentPolygonalGeometry();
	result |= this->X2AxisTip->HasTranslucentPolygonalGeometry();
	result |= this->Y2AxisTip->HasTranslucentPolygonalGeometry();
	result |= this->Z2AxisTip->HasTranslucentPolygonalGeometry();

	if (this->AxisLabels)
	{
		result |= this->XAxisLabel->HasTranslucentPolygonalGeometry();
		result |= this->YAxisLabel->HasTranslucentPolygonalGeometry();
		result |= this->ZAxisLabel->HasTranslucentPolygonalGeometry();
		result |= this->X2AxisLabel->HasTranslucentPolygonalGeometry();
		result |= this->Y2AxisLabel->HasTranslucentPolygonalGeometry();
		result |= this->Z2AxisLabel->HasTranslucentPolygonalGeometry();
	}
	return result;
}

//-----------------------------------------------------------------------------
int vtkOrientationHelperActor::RenderOverlay(vtkViewport *vp)
{
	int renderedSomething = 0;

	if (!this->AxisLabels)
	{
		return renderedSomething;
	}

	this->UpdateProps();

	renderedSomething += this->XAxisLabel->RenderOverlay(vp);
	renderedSomething += this->YAxisLabel->RenderOverlay(vp);
	renderedSomething += this->ZAxisLabel->RenderOverlay(vp);
	renderedSomething += this->X2AxisLabel->RenderOverlay(vp);
	renderedSomething += this->Y2AxisLabel->RenderOverlay(vp);
	renderedSomething += this->Z2AxisLabel->RenderOverlay(vp);

	renderedSomething = (renderedSomething > 0) ? (1) : (0);
	return renderedSomething;
}

//----------------------------------------------------------------------------
void vtkOrientationHelperActor::ReleaseGraphicsResources(vtkWindow *win)
{
	this->XAxisShaft->ReleaseGraphicsResources(win);
	this->YAxisShaft->ReleaseGraphicsResources(win);
	this->ZAxisShaft->ReleaseGraphicsResources(win);

	this->X2AxisShaft->ReleaseGraphicsResources(win);
	this->Y2AxisShaft->ReleaseGraphicsResources(win);
	this->Z2AxisShaft->ReleaseGraphicsResources(win);

	this->XAxisTip->ReleaseGraphicsResources(win);
	this->YAxisTip->ReleaseGraphicsResources(win);
	this->ZAxisTip->ReleaseGraphicsResources(win);

	this->X2AxisTip->ReleaseGraphicsResources(win);
	this->Y2AxisTip->ReleaseGraphicsResources(win);
	this->Z2AxisTip->ReleaseGraphicsResources(win);

	this->XAxisLabel->ReleaseGraphicsResources(win);
	this->YAxisLabel->ReleaseGraphicsResources(win);
	this->ZAxisLabel->ReleaseGraphicsResources(win);

	this->X2AxisLabel->ReleaseGraphicsResources(win);
	this->Y2AxisLabel->ReleaseGraphicsResources(win);
	this->Z2AxisLabel->ReleaseGraphicsResources(win);
}

//----------------------------------------------------------------------------
void vtkOrientationHelperActor::GetBounds(double bounds[6])
{
	double *bds = this->GetBounds();
	bounds[0] = bds[0];
	bounds[1] = bds[1];
	bounds[2] = bds[2];
	bounds[3] = bds[3];
	bounds[4] = bds[4];
	bounds[5] = bds[5];
}

//----------------------------------------------------------------------------
// Get the bounds for this Actor as (Xmin,Xmax,Ymin,Ymax,Zmin,Zmax).
double *vtkOrientationHelperActor::GetBounds()
{
	// Ugly

	double bounds[6];
	int i;

	this->XAxisShaft->GetBounds(this->Bounds);

	this->YAxisShaft->GetBounds(bounds);
	for (i = 0; i < 3; ++i)
	{
		this->Bounds[2 * i + 1] =
			(bounds[2 * i + 1]>this->Bounds[2 * i + 1]) ? (bounds[2 * i + 1]) : (this->Bounds[2 * i + 1]);
	}

	this->ZAxisShaft->GetBounds(bounds);
	for (i = 0; i < 3; ++i)
	{
		this->Bounds[2 * i + 1] =
			(bounds[2 * i + 1]>this->Bounds[2 * i + 1]) ? (bounds[2 * i + 1]) : (this->Bounds[2 * i + 1]);
	}

	this->XAxisTip->GetBounds(bounds);
	for (i = 0; i < 3; ++i)
	{
		this->Bounds[2 * i + 1] =
			(bounds[2 * i + 1]>this->Bounds[2 * i + 1]) ? (bounds[2 * i + 1]) : (this->Bounds[2 * i + 1]);
	}

	this->YAxisTip->GetBounds(bounds);
	for (i = 0; i < 3; ++i)
	{
		this->Bounds[2 * i + 1] =
			(bounds[2 * i + 1]>this->Bounds[2 * i + 1]) ? (bounds[2 * i + 1]) : (this->Bounds[2 * i + 1]);
	}

	this->ZAxisTip->GetBounds(bounds);
	for (i = 0; i < 3; ++i)
	{
		this->Bounds[2 * i + 1] =
			(bounds[2 * i + 1]>this->Bounds[2 * i + 1]) ? (bounds[2 * i + 1]) : (this->Bounds[2 * i + 1]);
	}


	

	double dbounds[6];
	(vtkPolyDataMapper::SafeDownCast(this->YAxisShaft->GetMapper()))->
		GetInput()->GetBounds(dbounds);

	for (i = 0; i < 3; ++i)
	{
		this->Bounds[2 * i + 1] =
			(dbounds[2 * i + 1]>this->Bounds[2 * i + 1]) ? (dbounds[2 * i + 1]) : (this->Bounds[2 * i + 1]);
	}

	// We want this actor to rotate / re-center about the origin, so give it
	// the bounds it would have if the axes were symmetric.
	// @@ so ugly... 
	for (i = 0; i < 3; ++i)
	{
		this->Bounds[2 * i] = -this->Bounds[2 * i + 1];
	}

	return this->Bounds;
}

//----------------------------------------------------------------------------
unsigned long int vtkOrientationHelperActor::GetMTime()
{
	unsigned long mTime = this->Superclass::GetMTime();
	return mTime;
}

//----------------------------------------------------------------------------
unsigned long int vtkOrientationHelperActor::GetRedrawMTime()
{
	unsigned long mTime = this->GetMTime();
	return mTime;
}

//----------------------------------------------------------------------------
void vtkOrientationHelperActor::SetTotalLength(double x, double y, double z)
{
	if (this->TotalLength[0] != x ||
		this->TotalLength[1] != y ||
		this->TotalLength[2] != z)
	{
		this->TotalLength[0] = x;
		this->TotalLength[1] = y;
		this->TotalLength[2] = z;

		if (x < 0.0 || y < 0.0 || z < 0.0)
		{
			vtkGenericWarningMacro("One or more axes lengths are < 0 \
                        and may produce unexpected results.");
		}

		this->Modified();

		this->UpdateProps();
	}
}



//----------------------------------------------------------------------------
void vtkOrientationHelperActor::SetNormalizedShaftLength(double x, double y, double z)
{
	if (this->NormalizedShaftLength[0] != x ||
		this->NormalizedShaftLength[1] != y ||
		this->NormalizedShaftLength[2] != z)
	{
		this->NormalizedShaftLength[0] = x;
		this->NormalizedShaftLength[1] = y;
		this->NormalizedShaftLength[2] = z;

		if (x < 0.0 || x > 1.0 || y < 0.0 || y > 1.0 || z < 0.0 || z > 1.0)
		{
			vtkGenericWarningMacro("One or more normalized shaft lengths \
      are < 0 or > 1 and may produce unexpected results.");
		}

		this->Modified();

		this->UpdateProps();
	}
}



//----------------------------------------------------------------------------
void vtkOrientationHelperActor::SetNormalizedTipLength(double x, double y, double z)
{
	if (this->NormalizedTipLength[0] != x ||
		this->NormalizedTipLength[1] != y ||
		this->NormalizedTipLength[2] != z)
	{
		this->NormalizedTipLength[0] = x;
		this->NormalizedTipLength[1] = y;
		this->NormalizedTipLength[2] = z;

		if (x < 0.0 || x > 1.0 || y < 0.0 || y > 1.0 || z < 0.0 || z > 1.0)
		{
			vtkGenericWarningMacro("One or more normalized tip lengths \
      are < 0 or > 1 and may produce unexpected results.");
		}

		this->Modified();

		this->UpdateProps();
	}
}




//----------------------------------------------------------------------------
void vtkOrientationHelperActor::SetNormalizedLabelPosition(double x, double y, double z)
{
	if (this->NormalizedLabelPosition[0] != x ||
		this->NormalizedLabelPosition[1] != y ||
		this->NormalizedLabelPosition[2] != z)
	{
		this->NormalizedLabelPosition[0] = x;
		this->NormalizedLabelPosition[1] = y;
		this->NormalizedLabelPosition[2] = z;

		if (x < 0.0 || y < 0.0 || z < 0.0)
		{
			vtkGenericWarningMacro("One or more label positions are < 0 \
                        and may produce unexpected results.");
		}

		this->Modified();

		this->UpdateProps();
	}
}


//----------------------------------------------------------------------------
void vtkOrientationHelperActor::SetShaftType(int type)
{
	if (this->ShaftType != type)
	{
		if (type < vtkOrientationHelperActor::CYLINDER_SHAFT || \
			type > vtkOrientationHelperActor::USER_DEFINED_SHAFT)
		{
			vtkErrorMacro("Undefined axes shaft type.");
			return;
		}

		if (type == vtkOrientationHelperActor::USER_DEFINED_SHAFT && \
			this->UserDefinedShaft == NULL)
		{
			vtkErrorMacro("Set the user defined shaft before changing the type.");
			return;
		}

		this->ShaftType = type;

		this->Modified();

		this->UpdateProps();
	}
}

//----------------------------------------------------------------------------
void vtkOrientationHelperActor::SetTipType(int type)
{
	if (this->TipType != type)
	{
		if (type < vtkOrientationHelperActor::CONE_TIP || \
			type > vtkOrientationHelperActor::USER_DEFINED_TIP)
		{
			vtkErrorMacro("Undefined axes tip type.");
			return;
		}

		if (type == vtkOrientationHelperActor::USER_DEFINED_TIP && \
			this->UserDefinedTip == NULL)
		{
			vtkErrorMacro("Set the user defined tip before changing the type.");
			return;
		}

		this->TipType = type;

		this->Modified();

		this->UpdateProps();
	}
}

//----------------------------------------------------------------------------
void vtkOrientationHelperActor::UpdateProps()
{
	this->CylinderSource->SetRadius(this->CylinderRadius);
	this->CylinderSource->SetResolution(this->CylinderResolution);

	this->ConeSource->SetResolution(this->ConeResolution);
	this->ConeSource->SetRadius(this->ConeRadius);

	this->SphereSource->SetThetaResolution(this->SphereResolution);
	this->SphereSource->SetPhiResolution(this->SphereResolution);
	this->SphereSource->SetRadius(this->SphereRadius);

	switch (this->ShaftType)
	{
	case vtkOrientationHelperActor::CYLINDER_SHAFT:
		(vtkPolyDataMapper::SafeDownCast(this->XAxisShaft->GetMapper()))->
			SetInputConnection(this->CylinderSource->GetOutputPort());
		break;
	case vtkOrientationHelperActor::LINE_SHAFT:
		(vtkPolyDataMapper::SafeDownCast(this->XAxisShaft->GetMapper()))->
			SetInputConnection(this->LineSource->GetOutputPort());
		break;
	case vtkOrientationHelperActor::USER_DEFINED_SHAFT:
		(vtkPolyDataMapper::SafeDownCast(this->XAxisShaft->GetMapper()))->
			SetInputData(this->UserDefinedShaft);
	}

	switch (this->TipType)
	{
	case vtkOrientationHelperActor::CONE_TIP:
		(vtkPolyDataMapper::SafeDownCast(this->XAxisTip->GetMapper()))->
			SetInputConnection(this->ConeSource->GetOutputPort());
		break;
	case vtkOrientationHelperActor::SPHERE_TIP:
		(vtkPolyDataMapper::SafeDownCast(this->XAxisTip->GetMapper()))->
			SetInputConnection(this->SphereSource->GetOutputPort());
		break;
	case vtkOrientationHelperActor::USER_DEFINED_TIP:
		(vtkPolyDataMapper::SafeDownCast(this->XAxisTip->GetMapper()))->
			SetInputData(this->UserDefinedTip);
	}

	vtkPolyDataMapper::SafeDownCast(this->XAxisTip->GetMapper())->
		GetInputAlgorithm()->Update();
	vtkPolyDataMapper::SafeDownCast(this->XAxisShaft->GetMapper())->
		GetInputAlgorithm()->Update();

	if (this->GetUserTransform())
	{
		this->XAxisShaft->SetUserTransform(NULL);
		this->YAxisShaft->SetUserTransform(NULL);
		this->ZAxisShaft->SetUserTransform(NULL);
		this->XAxisTip->SetUserTransform(NULL);
		this->YAxisTip->SetUserTransform(NULL);
		this->ZAxisTip->SetUserTransform(NULL);

		this->X2AxisShaft->SetUserTransform(NULL);
		this->Y2AxisShaft->SetUserTransform(NULL);
		this->Z2AxisShaft->SetUserTransform(NULL);
		this->X2AxisTip->SetUserTransform(NULL);
		this->Y2AxisTip->SetUserTransform(NULL);
		this->Z2AxisTip->SetUserTransform(NULL);

	}

	double scale[3];
	double bounds[6];

	(vtkPolyDataMapper::SafeDownCast(this->XAxisShaft->GetMapper()))->
		GetInput()->GetBounds(bounds);

	// The shaft and tip geometry are both initially along direction 0 1 0
	// in the case of cylinder, line, and cone.  Build up the axis from
	// constituent elements defined in their default positions.

	int i;
	for (i = 0; i < 3; ++i)
	{
		scale[i] =
			this->NormalizedShaftLength[i] * this->TotalLength[i] /
			(bounds[3] - bounds[2]);
	}

	vtkTransform *xTransform = vtkTransform::New();
	vtkTransform *yTransform = vtkTransform::New();
	vtkTransform *zTransform = vtkTransform::New();

	vtkTransform *x2Transform = vtkTransform::New();
	vtkTransform *y2Transform = vtkTransform::New();
	vtkTransform *z2Transform = vtkTransform::New();


	xTransform->RotateZ(-90);

	zTransform->RotateX(90);

	xTransform->Scale(scale[0], scale[0], scale[0]);
	yTransform->Scale(scale[1], scale[1], scale[1]);
	zTransform->Scale(scale[2], scale[2], scale[2]);

	xTransform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);
	yTransform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);
	zTransform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);


	x2Transform->RotateZ(90);
	y2Transform->RotateZ(180);
	z2Transform->RotateX(-90);

	x2Transform->Scale(scale[0], scale[0], scale[0]);
	y2Transform->Scale(scale[1], scale[1], scale[1]);
	z2Transform->Scale(scale[2], scale[2], scale[2]);

	x2Transform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);
	y2Transform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);
	z2Transform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);

	this->XAxisShaft->SetScale(xTransform->GetScale());
	this->XAxisShaft->SetPosition(xTransform->GetPosition());
	this->XAxisShaft->SetOrientation(xTransform->GetOrientation());

	this->X2AxisShaft->SetScale(x2Transform->GetScale());
	this->X2AxisShaft->SetPosition(x2Transform->GetPosition());
	this->X2AxisShaft->SetOrientation(x2Transform->GetOrientation());

	this->YAxisShaft->SetScale(yTransform->GetScale());
	this->YAxisShaft->SetPosition(yTransform->GetPosition());
	this->YAxisShaft->SetOrientation(yTransform->GetOrientation());

	this->Y2AxisShaft->SetScale(y2Transform->GetScale());
	this->Y2AxisShaft->SetPosition(y2Transform->GetPosition());
	this->Y2AxisShaft->SetOrientation(y2Transform->GetOrientation());

	this->ZAxisShaft->SetScale(zTransform->GetScale());
	this->ZAxisShaft->SetPosition(zTransform->GetPosition());
	this->ZAxisShaft->SetOrientation(zTransform->GetOrientation());

	this->Z2AxisShaft->SetScale(z2Transform->GetScale());
	this->Z2AxisShaft->SetPosition(z2Transform->GetPosition());
	this->Z2AxisShaft->SetOrientation(z2Transform->GetOrientation());

	(vtkPolyDataMapper::SafeDownCast(this->XAxisTip->GetMapper()))->
		GetInput()->GetBounds(bounds);

	xTransform->Identity();
	yTransform->Identity();
	zTransform->Identity();

	x2Transform->Identity();
	y2Transform->Identity();
	z2Transform->Identity();

	xTransform->RotateZ(-90);
	zTransform->RotateX(90);

	xTransform->Scale(this->TotalLength[0], this->TotalLength[0], this->TotalLength[0]);
	yTransform->Scale(this->TotalLength[1], this->TotalLength[1], this->TotalLength[1]);
	zTransform->Scale(this->TotalLength[2], this->TotalLength[2], this->TotalLength[2]);



	xTransform->Translate(0, (1.0 - this->NormalizedTipLength[0]), 0);
	yTransform->Translate(0, (1.0 - this->NormalizedTipLength[1]), 0);
	zTransform->Translate(0, (1.0 - this->NormalizedTipLength[2]), 0);

	xTransform->Scale(this->NormalizedTipLength[0],
		this->NormalizedTipLength[0],
		this->NormalizedTipLength[0]);

	yTransform->Scale(this->NormalizedTipLength[1],
		this->NormalizedTipLength[1],
		this->NormalizedTipLength[1]);

	zTransform->Scale(this->NormalizedTipLength[2],
		this->NormalizedTipLength[2],
		this->NormalizedTipLength[2]);

	xTransform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);
	yTransform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);
	zTransform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);


	x2Transform->RotateZ(90);
	y2Transform->RotateZ(180);
	z2Transform->RotateX(-90);

	x2Transform->Scale(this->TotalLength[0], this->TotalLength[0], this->TotalLength[0]);
	y2Transform->Scale(this->TotalLength[1], this->TotalLength[1], this->TotalLength[1]);
	z2Transform->Scale(this->TotalLength[2], this->TotalLength[2], this->TotalLength[2]);



	x2Transform->Translate(0, (1.0 - this->NormalizedTipLength[0]), 0);
	y2Transform->Translate(0, (1.0 - this->NormalizedTipLength[1]), 0);
	z2Transform->Translate(0, (1.0 - this->NormalizedTipLength[2]), 0);

	x2Transform->Scale(this->NormalizedTipLength[0],
		this->NormalizedTipLength[0],
		this->NormalizedTipLength[0]);

	y2Transform->Scale(this->NormalizedTipLength[1],
		this->NormalizedTipLength[1],
		this->NormalizedTipLength[1]);

	z2Transform->Scale(this->NormalizedTipLength[2],
		this->NormalizedTipLength[2],
		this->NormalizedTipLength[2]);

	x2Transform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);
	y2Transform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);
	z2Transform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);

	this->XAxisTip->SetScale(xTransform->GetScale());
	this->XAxisTip->SetPosition(xTransform->GetPosition());
	this->XAxisTip->SetOrientation(xTransform->GetOrientation());

	this->YAxisTip->SetScale(yTransform->GetScale());
	this->YAxisTip->SetPosition(yTransform->GetPosition());
	this->YAxisTip->SetOrientation(yTransform->GetOrientation());

	this->ZAxisTip->SetScale(zTransform->GetScale());
	this->ZAxisTip->SetPosition(zTransform->GetPosition());
	this->ZAxisTip->SetOrientation(zTransform->GetOrientation());

	this->X2AxisTip->SetScale(x2Transform->GetScale());
	this->X2AxisTip->SetPosition(x2Transform->GetPosition());
	this->X2AxisTip->SetOrientation(x2Transform->GetOrientation());

	this->Y2AxisTip->SetScale(y2Transform->GetScale());
	this->Y2AxisTip->SetPosition(y2Transform->GetPosition());
	this->Y2AxisTip->SetOrientation(y2Transform->GetOrientation());

	this->Z2AxisTip->SetScale(z2Transform->GetScale());
	this->Z2AxisTip->SetPosition(z2Transform->GetPosition());
	this->Z2AxisTip->SetOrientation(z2Transform->GetOrientation());

	xTransform->Delete();
	yTransform->Delete();
	zTransform->Delete();

	x2Transform->Delete();
	y2Transform->Delete();
	z2Transform->Delete();

	this->XAxisLabel->SetCaption(this->XAxisLabelText);
	this->YAxisLabel->SetCaption(this->YAxisLabelText);
	this->ZAxisLabel->SetCaption(this->ZAxisLabelText);

	this->X2AxisLabel->SetCaption(this->X2AxisLabelText);
	this->Y2AxisLabel->SetCaption(this->Y2AxisLabelText);
	this->Z2AxisLabel->SetCaption(this->Z2AxisLabelText);

	this->XAxisShaft->GetBounds(bounds);
	double offset = this->NormalizedLabelPosition[0] * (bounds[1] - bounds[0]);
	this->XAxisLabel->SetAttachmentPoint(bounds[0] + offset,
		bounds[2] - (bounds[3] - bounds[2])*2.0,
		bounds[5] + (bounds[5] - bounds[4]) / 2.0);

	this->YAxisShaft->GetBounds(bounds);
	offset = this->NormalizedLabelPosition[1] * (bounds[3] - bounds[2]);
	this->YAxisLabel->SetAttachmentPoint((bounds[0] + bounds[1]) / 2,
		bounds[2] + offset,
		bounds[5] + (bounds[5] - bounds[4]) / 2.0);

	this->ZAxisShaft->GetBounds(bounds);
	offset = this->NormalizedLabelPosition[2] * (bounds[5] - bounds[4]);
	this->ZAxisLabel->SetAttachmentPoint(bounds[0],
		bounds[2] - (bounds[3] - bounds[2])*2.0,
		bounds[4] + offset);

	this->X2AxisShaft->GetBounds(bounds);
	offset = -1* this->NormalizedLabelPosition[0] * (bounds[1] - bounds[0]);
	
	this->X2AxisLabel->SetAttachmentPoint(bounds[0] + offset,
		bounds[2] - (bounds[3] - bounds[2])*2.0,
		bounds[5] + (bounds[5] - bounds[4]) / 2.0);

	this->Y2AxisShaft->GetBounds(bounds);
	offset = -1 * this->NormalizedLabelPosition[1] * (bounds[3] - bounds[2]);
	this->Y2AxisLabel->SetAttachmentPoint((bounds[0] + bounds[1]) / 2,
		bounds[2] + offset,
		bounds[5] + (bounds[5] - bounds[4]) / 2.0);

	this->Z2AxisShaft->GetBounds(bounds);
	offset = -1 * this->NormalizedLabelPosition[2] * (bounds[5] - bounds[4]);
	this->Z2AxisLabel->SetAttachmentPoint(bounds[0],
		bounds[2] - (bounds[3] - bounds[2])*2.0,
		bounds[4] + offset);

	vtkLinearTransform* transform = this->GetUserTransform();
	if (transform)
	{
		this->XAxisShaft->SetUserTransform(transform);
		this->YAxisShaft->SetUserTransform(transform);
		this->ZAxisShaft->SetUserTransform(transform);

		this->XAxisTip->SetUserTransform(transform);
		this->YAxisTip->SetUserTransform(transform);
		this->ZAxisTip->SetUserTransform(transform);
		
		this->X2AxisShaft->SetUserTransform(transform);
		this->Y2AxisShaft->SetUserTransform(transform);
		this->Z2AxisShaft->SetUserTransform(transform);

		this->X2AxisTip->SetUserTransform(transform);
		this->Y2AxisTip->SetUserTransform(transform);
		this->Z2AxisTip->SetUserTransform(transform);

		double newpos[3];
		double* pos = this->XAxisLabel->GetAttachmentPoint();
		transform->TransformPoint(pos, newpos);
		this->XAxisLabel->SetAttachmentPoint(newpos);

		pos = this->YAxisLabel->GetAttachmentPoint();
		transform->TransformPoint(pos, newpos);
		this->YAxisLabel->SetAttachmentPoint(newpos);

		pos = this->ZAxisLabel->GetAttachmentPoint();
		transform->TransformPoint(pos, newpos);
		this->ZAxisLabel->SetAttachmentPoint(newpos);

		pos = this->X2AxisLabel->GetAttachmentPoint();
		transform->TransformPoint(pos, newpos);
		this->X2AxisLabel->SetAttachmentPoint(newpos);

		pos = this->Y2AxisLabel->GetAttachmentPoint();
		transform->TransformPoint(pos, newpos);
		this->Y2AxisLabel->SetAttachmentPoint(newpos);

		pos = this->Z2AxisLabel->GetAttachmentPoint();
		transform->TransformPoint(pos, newpos);
		this->Z2AxisLabel->SetAttachmentPoint(newpos);
	}
}

//----------------------------------------------------------------------------
vtkProperty *vtkOrientationHelperActor::GetXAxisTipProperty()
{
	return this->XAxisTip->GetProperty();
}
vtkProperty *vtkOrientationHelperActor::GetX2AxisTipProperty()
{
	return this->X2AxisTip->GetProperty();
}
//----------------------------------------------------------------------------
vtkProperty *vtkOrientationHelperActor::GetYAxisTipProperty()
{
	return this->YAxisTip->GetProperty();
}
vtkProperty *vtkOrientationHelperActor::GetY2AxisTipProperty()
{
	return this->Y2AxisTip->GetProperty();
}
//----------------------------------------------------------------------------
vtkProperty *vtkOrientationHelperActor::GetZAxisTipProperty()
{
	return this->ZAxisTip->GetProperty();
}
vtkProperty *vtkOrientationHelperActor::GetZ2AxisTipProperty()
{
	return this->Z2AxisTip->GetProperty();
}

//----------------------------------------------------------------------------
vtkProperty *vtkOrientationHelperActor::GetXAxisShaftProperty()
{
	return this->XAxisShaft->GetProperty();
}
vtkProperty *vtkOrientationHelperActor::GetX2AxisShaftProperty()
{
	return this->X2AxisShaft->GetProperty();
}

//----------------------------------------------------------------------------
vtkProperty *vtkOrientationHelperActor::GetYAxisShaftProperty()
{
	return this->YAxisShaft->GetProperty();
}

//----------------------------------------------------------------------------
vtkProperty *vtkOrientationHelperActor::GetY2AxisShaftProperty()
{
	return this->Y2AxisShaft->GetProperty();
}
//----------------------------------------------------------------------------
vtkProperty *vtkOrientationHelperActor::GetZAxisShaftProperty()
{
	return this->ZAxisShaft->GetProperty();
}
//----------------------------------------------------------------------------
vtkProperty *vtkOrientationHelperActor::GetZ2AxisShaftProperty()
{
	return this->Z2AxisShaft->GetProperty();
}
//----------------------------------------------------------------------------
void vtkOrientationHelperActor::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	os << indent << "UserDefinedShaft: ";
	if (this->UserDefinedShaft)
	{
		os << this->UserDefinedShaft << endl;
	}
	else
	{
		os << "(none)" << endl;
	}

	os << indent << "UserDefinedTip: ";
	if (this->UserDefinedTip)
	{
		os << this->UserDefinedTip << endl;
	}
	else
	{
		os << "(none)" << endl;
	}

	os << indent << "XAxisLabelText: " << (this->XAxisLabelText ?
		this->XAxisLabelText : "(none)")
		<< endl;
	os << indent << "YAxisLabelText: " << (this->YAxisLabelText ?
		this->YAxisLabelText : "(none)")
		<< endl;
	os << indent << "ZAxisLabelText: " << (this->ZAxisLabelText ?
		this->ZAxisLabelText : "(none)")
		<< endl;

	os << indent << "AxisLabels: " << (this->AxisLabels ? "On\n" : "Off\n");

	os << indent << "ShaftType: " << this->ShaftType << endl;
	os << indent << "TipType: " << this->TipType << endl;
	os << indent << "SphereRadius: " << this->SphereRadius << endl;
	os << indent << "SphereResolution: " << this->SphereResolution << endl;
	os << indent << "CylinderRadius: " << this->CylinderRadius << endl;
	os << indent << "CylinderResolution: " << this->CylinderResolution << endl;
	os << indent << "ConeRadius: " << this->ConeRadius << endl;
	os << indent << "ConeResolution: " << this->ConeResolution << endl;

	os << indent << "NormalizedShaftLength: "
		<< this->NormalizedShaftLength[0] << ","
		<< this->NormalizedShaftLength[1] << ","
		<< this->NormalizedShaftLength[2] << endl;

	os << indent << "NormalizedTipLength: "
		<< this->NormalizedTipLength[0] << ","
		<< this->NormalizedTipLength[1] << ","
		<< this->NormalizedTipLength[2] << endl;

	os << indent << "TotalLength: "
		<< this->TotalLength[0] << ","
		<< this->TotalLength[1] << ","
		<< this->TotalLength[2] << endl;

	os << indent << "NormalizedLabelPosition: "
		<< this->NormalizedLabelPosition[0] << ","
		<< this->NormalizedLabelPosition[1] << ","
		<< this->NormalizedLabelPosition[2] << endl;
}
