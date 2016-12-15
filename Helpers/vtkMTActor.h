/*=========================================================================

Program:   MeshTools
Module:    vtkMTActor.h



=========================================================================*/


#ifndef vtkMTActor_h
#define vtkMTActor_h


#include "vtkActor.h"


class  vtkMTActor : public vtkActor
{
public:
	static vtkMTActor *New();
	vtkTypeMacro(vtkMTActor, vtkActor);
	void PrintSelf(ostream& os, vtkIndent indent);

	//@{
	/**
	* This causes the actor to be rendered. It in turn will render the actor's
	* property, texture map and then mapper. If a property hasn't been
	* assigned, then the actor will create one automatically.
	*/
	virtual int RenderOpaqueGeometry(vtkViewport *viewport);
	virtual int RenderTranslucentPolygonalGeometry(vtkViewport *viewport);
	virtual void Render(vtkRenderer *ren);
	//@}

	/**
	* Does this prop have some translucent polygonal geometry?
	*/
	virtual int HasTranslucentPolygonalGeometry();

	/**
	* Release any graphics resources associated with this vtkProp3DFollower.
	*/
	virtual void ReleaseGraphicsResources(vtkWindow*);
	// Description:
	void ShallowCopy(vtkProp *prop);

	vtkSetMacro(Selected, int);
	vtkGetMacro(Selected, int);
	vtkBooleanMacro(Selected, int);



	

protected:
	vtkMTActor();
	~vtkMTActor();
	vtkActor  *Device;

	// Description:
	// Shallow copy of an axes actor. Overloads the virtual vtkProp method.
	int Selected;

	


private:
	vtkMTActor(const vtkMTActor&);  // Not implemented.
	void operator=(const vtkMTActor&);  // Not implemented.
	// hide the two parameter Render() method from the user and the compiler.
	virtual void Render(vtkRenderer *, vtkMapper *) {}

};

#endif

