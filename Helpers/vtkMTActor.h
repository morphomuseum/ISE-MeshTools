/*=========================================================================

Program:   MeshTools
Module:    vtkMTActor.h



=========================================================================*/


#ifndef vtkMTActor_h
#define vtkMTActor_h


#include <vtkOpenGLActor.h>


class  vtkMTActor : public vtkOpenGLActor
{
public:
	static vtkMTActor *New();
	vtkTypeMacro(vtkMTActor, vtkOpenGLActor);
	void PrintSelf(ostream& os, vtkIndent indent);

	
	// Description:
	void ShallowCopy(vtkProp *prop);

	vtkSetMacro(Selected, int);
	vtkGetMacro(Selected, int);
	vtkBooleanMacro(Selected, int);


	// Actual actor render method.
	void Render(vtkRenderer *ren, vtkMapper *mapper);
	

	

protected:
	vtkMTActor();
	~vtkMTActor();

	int Selected;

	


private:
	vtkMTActor(const vtkMTActor&);  // Not implemented.
	void operator=(const vtkMTActor&);  // Not implemented.
	
};

#endif

