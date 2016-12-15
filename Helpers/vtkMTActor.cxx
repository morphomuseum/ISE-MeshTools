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
//vtkAbstractObjectFactoryNewMacro(vtkMTActor);
vtkStandardNewMacro(vtkMTActor);


//----------------------------------------------------------------------------
vtkMTActor::vtkMTActor()
{
	this->Device = vtkActor::New();
	this->Selected = 1;
	
}

//----------------------------------------------------------------------------
vtkMTActor::~vtkMTActor()
{
	this->Device->Delete();

}

//----------------------------------------------------------------------------
// Shallow copy of an actor.
void vtkMTActor::ShallowCopy(vtkProp *prop)
{
	vtkMTActor *f = vtkMTActor::SafeDownCast(prop);
	

	// Now do superclass
	this->vtkActor::ShallowCopy(prop);
}


//----------------------------------------------------------------------------
void vtkMTActor::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	os << indent << "Selected: "<< this->Selected;
	

}
//----------------------------------------------------------------------
int vtkMTActor::RenderOpaqueGeometry(vtkViewport *vp)
{
	if (!this->Mapper)
	{
		return 0;
	}

	if (!this->Property)
	{
		// force creation of a property
		this->GetProperty();
	}

	if (this->GetIsOpaque())
	{
		vtkRenderer *ren = static_cast<vtkRenderer *>(vp);
		this->Render(ren);
		return 1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
int vtkMTActor::RenderTranslucentPolygonalGeometry(vtkViewport *vp)
{
	if (!this->Mapper)
	{
		return 0;
	}

	if (!this->Property)
	{
		// force creation of a property
		this->GetProperty();
	}

	if (!this->GetIsOpaque())
	{
		vtkRenderer *ren = static_cast<vtkRenderer *>(vp);
		this->Render(ren);
		return 1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
void vtkMTActor::ReleaseGraphicsResources(vtkWindow *w)
{
	this->Device->ReleaseGraphicsResources(w);
	this->Superclass::ReleaseGraphicsResources(w);
}

//-----------------------------------------------------------------------------
// Description:
// Does this prop have some translucent polygonal geometry?
int vtkMTActor::HasTranslucentPolygonalGeometry()
{
	if (!this->Mapper)
	{
		return 0;
	}
	// make sure we have a property
	if (!this->Property)
	{
		// force creation of a property
		this->GetProperty();
	}

	// is this actor opaque ?
	return !this->GetIsOpaque();
}

//-----------------------------------------------------------------------------
// This causes the actor to be rendered. It, in turn, will render the actor's
// property and then mapper.
//-----------------------------------------------------------------------------
// This causes the actor to be rendered. It, in turn, will render the actor's
// property and then mapper.
void vtkMTActor::Render(vtkRenderer *ren)
{
	this->Property->Render(this, ren);

	this->Device->SetProperty(this->Property);
	this->Property->Render(this, ren);
	if (this->BackfaceProperty)
	{
		this->BackfaceProperty->BackfaceRender(this, ren);
		this->Device->SetBackfaceProperty(this->BackfaceProperty);
	}

	/* render the texture */
	if (this->Texture)
	{
		this->Texture->Render(ren);
	}
	this->Device->SetTexture(this->GetTexture());

	// make sure the device has the same matrix
	this->ComputeMatrix();
	this->Device->SetUserMatrix(this->Matrix);

	this->Device->Render(ren, this->Mapper);
}