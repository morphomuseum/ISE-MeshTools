/*=========================================================================

Program:   MeshTools
Module:    vtkMTActorCollection.h



=========================================================================*/


#ifndef vtkLMActorCollection_h
#define vtkLMActorCollection_h


#include "vtkMTActorCollection.h"
#include "vtkLMActor.h"
#include <vtkSmartPointer.h>
#include <vector>



class  vtkLMActorCollection : public vtkMTActorCollection
{
public:
	static vtkLMActorCollection *New();
	vtkTypeMacro(vtkLMActorCollection, vtkMTActorCollection);
	
	void AddItem(vtkActor *a);	

	void PrintSelf(ostream& os, vtkIndent indent);
	
	
	int GetNextLandmarkNumber();

	

protected:
	
	vtkLMActorCollection();
	~vtkLMActorCollection();
	virtual void ComputeCenterOfMass();
	void ReorderLandmarks();
	

private:
	vtkLMActorCollection(const vtkLMActorCollection&);  // Not implemented.
	void operator=(const vtkMTActorCollection&);  // Not implemented.
	
};

#endif

