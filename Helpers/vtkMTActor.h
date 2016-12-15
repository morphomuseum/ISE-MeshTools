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

	
	// Description:
	void ShallowCopy(vtkProp *prop);

	vtkSetMacro(Selected, int);
	vtkGetMacro(Selected, int);
	vtkBooleanMacro(Selected, int);



	

protected:
	vtkMTActor();
	~vtkMTActor();

	// Description:
	// Shallow copy of an axes actor. Overloads the virtual vtkProp method.
	int Selected;

	


private:
	vtkMTActor(const vtkMTActor&);  // Not implemented.
	void operator=(const vtkMTActor&);  // Not implemented.
	

};

#endif

