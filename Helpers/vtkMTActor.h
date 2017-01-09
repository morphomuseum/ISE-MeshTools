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
	static int getTestInt();
	vtkTypeMacro(vtkMTActor, vtkOpenGLActor);
	void PrintSelf(ostream& os, vtkIndent indent);

	
	// Description:
	void ShallowCopy(vtkProp *prop);

	vtkSetMacro(Selected, int);
	vtkGetMacro(Selected, int);
	vtkBooleanMacro(Selected, int);

	vtkSetMacro(Changed, int);
	vtkGetMacro(Changed, int);
	vtkBooleanMacro(Changed, int);

	// Actual actor render method.
	void Render(vtkRenderer *ren, vtkMapper *mapper);
	

	

protected:
	vtkMTActor();
	~vtkMTActor();

	int Selected;
	int Changed; // used by MTActorCollection class to recompute global center of mass and center of mass
	//of selected objects etc... 

	


private:
	vtkMTActor(const vtkMTActor&);  // Not implemented.
	void operator=(const vtkMTActor&);  // Not implemented.
	static int testint;
};

#endif

