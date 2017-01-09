/*=========================================================================

Program:   MeshTools
Module:    vtkMTActor.cxx
=========================================================================*/
#include "vtkMTActor.h"

#include <vtkObjectFactory.h>
#include <vtkMath.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkTexture.h>
vtkStandardNewMacro(vtkMTActor);



int vtkMTActor::getTestInt()
{
	return vtkMTActor::testint;
}
//----------------------------------------------------------------------------
vtkMTActor::vtkMTActor()
{
	vtkMTActor::testint = 0;
	this->Selected = 1;
	this->Changed = 0;
	
}

//----------------------------------------------------------------------------
vtkMTActor::~vtkMTActor()
{
	

}

//----------------------------------------------------------------------------
// Shallow copy of an actor.
void vtkMTActor::ShallowCopy(vtkProp *prop)
{
	vtkMTActor *f = vtkMTActor::SafeDownCast(prop);
	

	// Now do superclass
	this->vtkOpenGLActor::ShallowCopy(prop);
}


//----------------------------------------------------------------------------
void vtkMTActor::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	os << indent << "Selected: "<< this->Selected;
	

}
// Actual actor render method.
void vtkMTActor::Render(vtkRenderer *ren, vtkMapper *mapper)
{
	this->Superclass::Render(ren, mapper);
}