/*=========================================================================

Program:   MeshTools
Module:    vtkMTActor.h



=========================================================================*/


#ifndef vtkMTActor_h
#define vtkMTActor_h


#include <vtkOpenGLActor.h>
#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>
#include <vtkSmartPointer.h>
#include <vector>

class vtkMTActorUndoRedo
{
public:
	struct Element
	{
		vtkSmartPointer<vtkMatrix4x4> Matrix;
		int UndoCount;
		Element(vtkSmartPointer<vtkMatrix4x4> m, int Count)
		{
			this->Matrix =m;
			this->UndoCount = Count;

		}
	};
	typedef std::vector<Element> VectorOfElements;
	VectorOfElements UndoStack;
	VectorOfElements RedoStack;
};

class  vtkMTActor : public vtkOpenGLActor
{
public:
	static vtkMTActor *New();
	
	vtkTypeMacro(vtkMTActor, vtkOpenGLActor);
	void PrintSelf(ostream& os, vtkIndent indent);

	
	// Description:
	void ShallowCopy(vtkProp *prop);

	//vtkSetMacro(Selected, int);
	vtkGetMacro(Selected, int);
	vtkBooleanMacro(Selected, int);

	vtkSetMacro(Changed, int);
	vtkGetMacro(Changed, int);
	vtkBooleanMacro(Changed, int);

	vtkSetVector4Macro(mColor, double);
	vtkGetVector4Macro(mColor, double);

	// Actual actor render method.
	void Render(vtkRenderer *ren, vtkMapper *mapper);
	void SetSelected(int selected);
	void SavePosition(int mCount);
	void Redo(int mCount); // Try to redo (if exists) "mCount" event
	void Erase(int mCount); // Try to erase (if exists) "mCount" event
	void Undo(int mCount); // Try to undo (if exists) "mCount" event
	void PopUndoStack();
	void PopRedoStack();


protected:
	vtkMTActor();
	~vtkMTActor();

	int Selected;
	int Changed; // used by MTActorCollection class to recompute global center of mass and center of mass
	//of selected objects etc... 
	double mColor[4]; // mesh "uniform" color (no vertex coloring) and transparency.
	vtkMTActorUndoRedo* UndoRedo;


private:
	vtkMTActor(const vtkMTActor&);  // Not implemented.
	void operator=(const vtkMTActor&);  // Not implemented.
	
};

#endif

