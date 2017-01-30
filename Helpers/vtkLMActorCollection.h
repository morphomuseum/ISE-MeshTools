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

	void DeleteSelectedActors();

protected:
	
	vtkLMActorCollection();
	~vtkLMActorCollection();
	virtual int ActorChanged(); // tells whether an actor was changed.
	virtual void ComputeBoundingBoxLength();
	virtual void ComputeCenterOfMass();
	void ReorderLandmarks();
	virtual void ApplyChanges(); // when actors have been modified (actor themselves selection/unselection or list add, remove etc...)

private:
	vtkLMActorCollection(const vtkLMActorCollection&);  // Not implemented.
	void operator=(const vtkMTActorCollection&);  // Not implemented.
	
};

#endif

