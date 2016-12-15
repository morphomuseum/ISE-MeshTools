/*=========================================================================

Program:   MeshTools
Module:    vtkMTActor.cxx
=========================================================================*/
#include "vtkMTActor.h"

#include <vtkObjectFactory.h>
#include <vtkMath.h>

//vtkAbstractObjectFactoryNewMacro(vtkMTActor);
vtkStandardNewMacro(vtkMTActor);


//----------------------------------------------------------------------------
vtkMTActor::vtkMTActor()
{
	this->Mapper = NULL;
	this->Property = NULL;
	this->BackfaceProperty = NULL;
	this->Texture = NULL;

	this->ForceOpaque = false;
	this->ForceTranslucent = false;

	// The mapper bounds are cache to know when the bounds must be recomputed
	// from the mapper bounds.
	
	vtkMath::UninitializeBounds(this->MapperBounds);

	this->Selected = 1;
	
}

//----------------------------------------------------------------------------
vtkMTActor::~vtkMTActor()
{


}

//----------------------------------------------------------------------------
// Shallow copy of an actor.
void vtkMTActor::ShallowCopy(vtkProp *prop)
{
	
	// Now do superclass
	this->vtkActor::ShallowCopy(prop);
}


//----------------------------------------------------------------------------
void vtkMTActor::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	os << indent << "Selected: "<< this->Selected;
	

}
