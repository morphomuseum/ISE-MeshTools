/*=========================================================================

Program:   MeshTools
Module:    vtkMTActorCollection.h



=========================================================================*/


#ifndef vtkMTActorCollection_h
#define vtkMTActorCollection_h


#include <vtkActorCollection.h>


class  vtkMTActorCollection : public vtkActorCollection
{
public:
	static vtkMTActorCollection *New();
	vtkTypeMacro(vtkMTActorCollection, vtkActorCollection);
	
	
	vtkSetMacro(BoundingBoxLength, double);
	vtkSetMacro(BoundingBoxLengthOfSelectedActors, double);
	double GetBoundingBoxLengthOfSelectedActors();
	double GetBoundingBoxLength();

	vtkSetMacro(Changed, int);
	vtkGetMacro(Changed, int);
	vtkBooleanMacro(Changed, int);
	void PrintSelf(ostream& os, vtkIndent indent);
	
	void ApplyProperties(vtkProperty *p);
	
	void GetCenterOfMass(double center[3]);
	void GetCenterOfMassOfSelectedActors(double center[3]);
	double * GetCenterOfMass();
	double * GetCenterOfMassOfSelectedActors();

	

	

protected:
	vtkMTActorCollection();
	~vtkMTActorCollection();
	void ApplyChanges(); // when actors have been modified (actor themselves selection/unselection or list add, remove etc...)
	void ComputeBoundingBoxLength();
	void ComputeCenterOfMass();
	int ActorChanged(); // tells whether an actor was changed.
	double centerOfMass[3];
	double centerOfMassOfSelectedActors[3];
	int Changed;
	double BoundingBoxLength;
	double BoundingBoxLengthOfSelectedActors;
	
	


private:
	vtkMTActorCollection(const vtkMTActorCollection&);  // Not implemented.
	void operator=(const vtkMTActorCollection&);  // Not implemented.
	
};

#endif

