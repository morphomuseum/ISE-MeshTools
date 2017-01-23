/*=========================================================================

Program:   MeshTools
Module:    vtkLMActor.cxx


=========================================================================*/
#include "vtkLMActor.h"

#include <vtkActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkMath.h>
#include <vtkObject.h>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropCollection.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>
#include <vtkLine.h>
#include <vtkCellData.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>


vtkStandardNewMacro(vtkLMActor);


//----------------------------------------------------------------------------
vtkLMActor::vtkLMActor()
{
	this->LMDrawLabel = 1;
	this->LMType = 0;
	this->LMBodyType = 0; //sphere by default
	this->LMSize = 1; // 1mm by default 
	this->LMNumber = 1;
	this->LMOrigin[0] = 1;
	this->LMOrigin[1] = 1;
	this->LMOrigin[2] = 1;

	this->LMOrientation[0] = 1;
	this->LMOrientation[1] = 0;
	this->LMOrientation[2] = 0;

	this->LMLabelText = NULL;


	this->LMLabel = vtkCaptionActor2D::New();
	this->LMBody = vtkActor::New();
	LMBody->GetProperty()->SetOpacity(1);
	LMBody->GetProperty()->SetLineWidth(3);




	this->UpdateProps();
}

//----------------------------------------------------------------------------
vtkLMActor::~vtkLMActor()
{

	this->LMBody->Delete();
	this->SetLMLabelText(NULL);
	this->LMLabel->Delete();
}

//----------------------------------------------------------------------------
// Shallow copy of an actor.
void vtkLMActor::ShallowCopy(vtkProp *prop)
{
	vtkLMActor *a = vtkLMActor::SafeDownCast(prop);
	if (a != NULL)
	{
		this->SetLMDrawLabel(a->GetLMDrawLabel());
		this->SetLMLabelText(a->GetLMLabelText());
		this->SetLMOrigin(a->GetLMOrigin());
		this->SetLMOrientation(a->GetLMOrientation());
		this->SetLMSize(a->GetLMSize());
		this->SetLMType(a->GetLMType());
		this->SetLMNumber(a->GetLMNumber());
		this->SetLMBodyType(a->GetLMBodyType());

	}

	// Now do superclass
	this->vtkProp3D::ShallowCopy(prop);
}

//----------------------------------------------------------------------------
void vtkLMActor::GetActors(vtkPropCollection *ac)
{
	ac->AddItem(this->LMBody);


}

//----------------------------------------------------------------------------
int vtkLMActor::RenderOpaqueGeometry(vtkViewport *vp)
{
	int renderedSomething = 0;

	this->UpdateProps();

	renderedSomething += this->LMBody->RenderOpaqueGeometry(vp);



	if (this->LMDrawLabel)
	{
		renderedSomething += this->LMLabel->RenderOpaqueGeometry(vp);

	}

	renderedSomething = (renderedSomething > 0) ? (1) : (0);
	return renderedSomething;
}

//-----------------------------------------------------------------------------
int vtkLMActor::RenderTranslucentPolygonalGeometry(vtkViewport *vp)
{
	int renderedSomething = 0;

	this->UpdateProps();

	renderedSomething += this->LMBody->RenderTranslucentPolygonalGeometry(vp);


	if (this->LMDrawLabel)
	{
		renderedSomething += this->LMLabel->RenderTranslucentPolygonalGeometry(vp);

	}

	renderedSomething = (renderedSomething > 0) ? (1) : (0);
	return renderedSomething;
}

//-----------------------------------------------------------------------------
// Description:
// Does this prop have some translucent polygonal geometry?
int vtkLMActor::HasTranslucentPolygonalGeometry()
{
	int result = 0;

	this->UpdateProps();

	result |= this->LMBody->HasTranslucentPolygonalGeometry();



	if (this->LMDrawLabel)
	{
		result |= this->LMLabel->HasTranslucentPolygonalGeometry();

	}
	return result;
}

//-----------------------------------------------------------------------------
int vtkLMActor::RenderOverlay(vtkViewport *vp)
{
	int renderedSomething = 0;

	if (!this->LMDrawLabel)
	{
		return renderedSomething;
	}

	this->UpdateProps();

	renderedSomething += this->LMLabel->RenderOverlay(vp);



	renderedSomething = (renderedSomething > 0) ? (1) : (0);
	return renderedSomething;
}

//----------------------------------------------------------------------------
void vtkLMActor::ReleaseGraphicsResources(vtkWindow *win)
{
	this->LMBody->ReleaseGraphicsResources(win);



	this->LMLabel->ReleaseGraphicsResources(win);


}

//----------------------------------------------------------------------------



void vtkLMActor::SetLMOrigin(double x, double y, double z)
{
	double origin[3];
	origin[0] = x;
	origin[1] = y;
	origin[2] = z;
	this->SetLMOrigin(origin);
}
void vtkLMActor::SetLMOrigin(double origin[3])
{

	if (this->LMOrigin[0] != origin[0]
		|| this->LMOrigin[1] != origin[1]
		|| this->LMOrigin[2] != origin[2]

		)
	{
		this->LMOrigin[0] = origin[0];
		this->LMOrigin[1] = origin[1];
		this->LMOrigin[2] = origin[2];


		this->Modified();
		this->UpdateProps();
	}
}
void vtkLMActor::GetLMOrigin(double origin[3])
{
	double *org = this->GetLMOrigin();

	origin[0] = org[0];
	origin[1] = org[1];
	origin[2] = org[2];

}



double *vtkLMActor::GetLMOrigin()
{
	return this->LMOrigin;
}


void vtkLMActor::SetLMOrientation(double orientation[3])
{

	if (this->LMOrientation[0] != orientation[0]
		|| this->LMOrientation[1] != orientation[1]
		|| this->LMOrientation[2] != orientation[2]

		)
	{
		this->LMOrientation[0] = orientation[0];
		this->LMOrientation[1] = orientation[1];
		this->LMOrientation[2] = orientation[2];


		this->Modified();
		this->UpdateProps();
	}
}
void vtkLMActor::GetLMOrientation(double orientation[3])
{
	double *ori = this->GetLMOrientation();

	orientation[0] = ori[0];
	orientation[1] = ori[1];
	orientation[2] = ori[2];

}



double *vtkLMActor::GetLMOrientation()
{
	return this->LMOrientation;
}


void vtkLMActor::CreateLMLabelText()
{
	//instantiates the label

	// LMType : 
	// 0 : -x x -y y
	// 1 :  -x x -z z
	// 2 : -y y -z z

	double pos[3] = { this->LMOrigin[0] + 1.1*this->LMSize , this->LMOrigin[1], this->LMOrigin[2] };

	// Create six colors - one for each line
	double red[3] = { 1, 0, 0 }; // LMType=0
	double yellow[3] = { 1, 1, 0 }; // LMType = 1 (target LM)
	double darkred[3] = { 0.5, 0, 0 }; // LMType = 2 (curve node: dark red)
	double orange[3] = { 1, 0.5, 0 }; // LMType = 3 (curve handle : orange)
	double green[3] = { 0, 1, 0 }; // LMType=4 (curve starting point)
	double blue[3] = { 0, 0, 1 }; // LMType = 5 (curve milestone)	
	double cyan[3] = { 0, 1, 1 }; // LMType = 6 (curve ending point)



	vtkSmartPointer<vtkTextProperty> mproperty = vtkSmartPointer<vtkTextProperty>::New();



	std::string myStrLabel;
	myStrLabel = std::to_string(this->LMNumber);

	if (this->LMType == 0)
	{
		mproperty->SetColor(red);
	}
	else if (this->LMType == 1)
	{
		mproperty->SetColor(yellow);
	}
	else if (this->LMType == 2)
	{
		mproperty->SetColor(darkred);
	}
	else if (this->LMType == 3)
	{
		mproperty->SetColor(orange);
	}
	else if (this->LMType == 4)
	{
		mproperty->SetColor(green);
	}
	else if (this->LMType == 5)
	{
		mproperty->SetColor(blue);
	}
	else if (this->LMType == 6)
	{
		mproperty->SetColor(cyan);
	}



	mproperty->SetFontFamilyToArial();

	this->LMLabel->SetCaptionTextProperty(mproperty);
	this->LMLabel->SetPosition(0, 0);
	this->LMLabel->SetAttachmentPoint(pos);
	this->LMLabel->SetHeight(0.03);
	this->LMLabel->BorderOff();
	this->LMLabel->LeaderOff();
	this->SetLMLabelText(myStrLabel.c_str());
	this->LMLabel->SetCaption(this->LMLabelText);


}

void vtkLMActor::CreateLMBody()
{


	vtkSmartPointer<vtkSphereSource> sphereSource =
	vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetCenter(this->LMOrigin[0], this->LMOrigin[1], this->LMOrigin[2]);
	sphereSource->SetRadius(this->LMSize);
	if (this->LMType == 1 || this->LMType == 3)
	{
		sphereSource->SetRadius(1.1*this->LMSize);
	}
	sphereSource->Update();

	// Setup the visualization pipeline
	vtkSmartPointer<vtkPolyDataMapper> mapper =
	vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(sphereSource->GetOutputPort());
	mapper->Update();

	this->LMBody->SetMapper(mapper);

	// Create six colors - one for each line
	double red[3] = { 1, 0, 0 }; // LMType=0
	double yellow[3] = { 1, 1, 0 }; // LMType = 1 (target LM)
	double darkred[3] = { 0.5, 0, 0 }; // LMType = 2 (curve node: dark red)
	double orange[3] = { 1, 0.5, 0 }; // LMType = 3 (curve handle : orange)
	double green[3] = { 0, 1, 0 }; // LMType=4 (curve starting point)
	double blue[3] = { 0, 0, 1 }; // LMType = 5 (curve milestone)
	double cyan[3] = { 0, 1, 1 }; // LMType = 6 (curve ending point)
	

	if (this->LMType == 0)
	{
		this->LMBody->GetProperty()->SetColor(red);
	}
	else if (this->LMType == 1)
	{
		this->LMBody->GetProperty()->SetColor(yellow);
		this->LMBody->GetProperty()->SetOpacity(0.5);
	}
	else if (this->LMType == 2)
	{
		this->LMBody->GetProperty()->SetColor(darkred);
		
	}
	else if (this->LMType == 3)
	{	
		this->LMBody->GetProperty()->SetColor(orange);
		this->LMBody->GetProperty()->SetOpacity(0.5);
	}
	else if (this->LMType == 4)
	{
		this->LMBody->GetProperty()->SetColor(green); 
	}
	else if (this->LMType == 5)
	{
		
		this->LMBody->GetProperty()->SetColor(blue);
	}
	else if (this->LMType == 6)
	{
		this->LMBody->GetProperty()->SetColor(cyan);
	}
		

	sphereSource->Update();
	mapper->Update();

	
}

//----------------------------------------------------------------------------
// Get the bounds for this Actor as (Xmin,Xmax,Ymin,Ymax,Zmin,Zmax).
double *vtkLMActor::GetBounds()
{


	if (this->LMBodyType == 0) // sphere
	{
		this->Bounds[0] = this->LMOrigin[0]-this->LMSize / 2;
		this->Bounds[1] = this->LMOrigin[0]+this->LMSize / 2;
		this->Bounds[2] = this->LMOrigin[1] -this->LMSize / 2;
		this->Bounds[3] = this->LMOrigin[1]+this->LMSize / 2;
		this->Bounds[4] = this->LMOrigin[2] -this->LMSize / 2;
		this->Bounds[5] = this->LMOrigin[2]+this->LMSize / 2;
	}
	else
	{
		//needle
		this->Bounds[0] = this->LMOrigin[0];
		this->Bounds[1] = this->LMOrigin[0]+this->LMSize;
		this->Bounds[2] = this->LMOrigin[1] -this->LMSize / 4;
		this->Bounds[3] = this->LMOrigin[1]+this->LMSize / 4;
		this->Bounds[4] = this->LMOrigin[2] -this->LMSize / 4;
		this->Bounds[5] = this->LMOrigin[2]+this->LMSize / 4;
	}



	return this->Bounds;
}

//----------------------------------------------------------------------------
#if VTK_MINOR_VERSION >= 1

vtkMTimeType vtkLMActor::GetMTime()
{
	vtkMTimeType mTime = this->Superclass::GetMTime();
	return mTime;
}

vtkMTimeType vtkLMActor::GetRedrawMTime()
{
	vtkMTimeType mTime = this->GetMTime();
	return mTime;
}
#else
unsigned long int vtkLMActor::GetMTime()
{
	unsigned long mTime = this->Superclass::GetMTime();
	return mTime;
}



//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
unsigned long int vtkLMActor::GetRedrawMTime()
{
	unsigned long mTime = this->GetMTime();
	return mTime;
}

#endif


//----------------------------------------------------------------------------
void vtkLMActor::SetLMType(int type)
{
	if (this->LMType != type && type >= 0 && type <= 6)
	{
		this->LMType = type;
		this->Modified();
		this->UpdateProps();
	}


}

void vtkLMActor::SetLMBodyType(int type)
{
	if (this->LMBodyType != type && type >= 0 && type <= 1)
	{
		this->LMBodyType = type;
		this->Modified();
		this->UpdateProps();
	}


}

void vtkLMActor::SetLMSize(double size)
{
	if (this->LMSize != size && size>0)
	{
		this->LMSize = size;
		this->Modified();
		this->UpdateProps();
	}

}


void vtkLMActor::SetLMNumber(int num)
{
	if (this->LMNumber != num && num>0)
	{
		this->LMNumber = num;
		this->Modified();
		this->UpdateProps();
	}

}



//----------------------------------------------------------------------------
void vtkLMActor::UpdateProps()
{
	//Recreates the text actor and the LMBody

	this->CreateLMBody();
	this->CreateLMLabelText();

}


//----------------------------------------------------------------------------
void vtkLMActor::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	os << indent << "LMLabelText: ";
	os << this->LMLabelText << endl;
	//this->LMLabel->PrintSelf(os, indent);
	this->LMBody->PrintSelf(os, vtkIndent(5));



	os << indent << "LMType: ";
	if (this->LMType >= 0)
	{
		os << this->LMType << endl;
	}
	else
	{
		os << "(no LM Type)" << endl;
	}

	os << indent << "LM origin: ";
	if (this->LMOrigin[0])
	{
		os << this->LMOrigin[0] << "," << this->LMOrigin[1] << "," << this->LMOrigin[2] << endl;
	}
	else
	{
		os << "(no LM Origin)" << endl;
	}

	os << indent << "LM orientation: ";
	if (this->LMOrientation[0])
	{
		os << this->LMOrientation[0] << "," << this->LMOrientation[1] << "," << this->LMOrientation[2] << endl;
	}
	else
	{
		os << "(no LM Origin)" << endl;
	}

	os << indent << "LMSize: ";
	if (this->LMSize)
	{
		os << this->LMSize << endl;
	}
	else
	{
		os << "(no LM Size)" << endl;
	}



	os << indent << "LMLabelText: " << (this->LMLabelText ?
		this->LMLabelText : "(none)")
		<< endl;


	os << indent << "LMDrawLabel: " << (this->LMDrawLabel ? "On\n" : "Off\n");


}
