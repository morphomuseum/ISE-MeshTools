/*=========================================================================

Program:   MeshTools
Module:    vtkMTActorCollection.h



=========================================================================*/


#ifndef vtkMTActorCollection_h
#define vtkMTActorCollection_h


#include <vtkActorCollection.h>
#include <vtkSmartPointer.h>
#include <vector>

class vtkMTCollectionUndoRedo
{
public:
	struct Element
	{
		vtkSmartPointer<vtkActorCollection> Collection;
		int UndoCount;
		Element(vtkSmartPointer<vtkActorCollection> col,  int Count)
		{
			this->Collection = col;
			this->UndoCount = Count;
			
		}
	};
	typedef std::vector<Element> VectorOfElements;
	VectorOfElements UndoStack;
	VectorOfElements RedoStack;
};

class  vtkMTActorCollection : public vtkActorCollection
{
public:
	static vtkMTActorCollection *New();
	vtkTypeMacro(vtkMTActorCollection, vtkActorCollection);
	
	void AddItem(vtkActor *a);
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
	void DeleteSelectedActors(); //delete all selected actors
	void Redo(int mCount); // Try to redo (if exists) "mCount" event
	void Erase(int mCount); // Try to erase (if exists) "mCount" event
	void Undo(int mCount); // Try to undo (if exists) "mCount" event
	void PopUndoStack();
	void PopRedoStack();
	vtkSmartPointer<vtkRenderer> getRenderer() { return this->Renderer; }
	void SetRenderer(vtkSmartPointer<vtkRenderer> rend)
	{
		this->Renderer = rend;
	}

	

protected:
	
	vtkMTActorCollection();
	~vtkMTActorCollection();
	void ApplyChanges(); // when actors have been modified (actor themselves selection/unselection or list add, remove etc...)
	void ComputeBoundingBoxLength();
	virtual void ComputeCenterOfMass(); // will be overridden in derived vtkLMActorCollection Class
	int ActorChanged(); // tells whether an actor was changed.
	double centerOfMass[3];
	double centerOfMassOfSelectedActors[3];
	int Changed;
	double BoundingBoxLength;
	double BoundingBoxLengthOfSelectedActors;
	vtkSmartPointer<vtkRenderer> Renderer;
	vtkMTCollectionUndoRedo* UndoRedo;


private:
	vtkMTActorCollection(const vtkMTActorCollection&);  // Not implemented.
	void operator=(const vtkMTActorCollection&);  // Not implemented.
	
};

#endif

