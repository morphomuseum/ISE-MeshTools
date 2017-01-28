/*=========================================================================

Program:   MeshTools
Module:    vtkLM2Actor.h



=========================================================================*/


#ifndef vtkLM2Actor_h
#define vtkLM2Actor_h


#include "vtkMTActor.h"
#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>
#include <vector>

class vtkLM2ActorUndoRedo
{
public:
	struct Element
	{
		vtkSmartPointer<vtkMatrix4x4> Matrix;
		double Color[4];
		int Selected;
		int Type;
		int UndoCount;
		Element(vtkSmartPointer<vtkMatrix4x4> m, double c[4], int selected, int type, int Count)
		{
			this->Matrix =m;
			this->UndoCount = Count;
			this->Color[0] = c[0];
			this->Color[1] = c[1];
			this->Color[2] = c[2];
			this->Color[3] = c[3];
			this->Selected = selected;
			this->Type = type;
		}
	};
	typedef std::vector<Element> VectorOfElements;
	VectorOfElements UndoStack;
	VectorOfElements RedoStack;
};

class  vtkLM2Actor : public vtkMTActor
{
public:
	static vtkLM2Actor *New();
	
	vtkTypeMacro(vtkLM2Actor, vtkOpenGLActor);
	void PrintSelf(ostream& os, vtkIndent indent);

	
	// Description:
	void ShallowCopy(vtkProp *prop);


	vtkSetMacro(Type, int);
	vtkGetMacro(Type, int);
	
	

	// Actual actor render method.
	void Render(vtkRenderer *ren, vtkMapper *mapper);
	virtual void SaveState(int mCount);
	virtual void Redo(int mCount); // Try to redo (if exists) "mCount" event
	virtual void Erase(int mCount); // Try to erase (if exists) "mCount" event
	virtual void Undo(int mCount); // Try to undo (if exists) "mCount" event
	virtual void PopUndoStack();
	virtual void PopRedoStack();


protected:
	vtkLM2Actor();
	~vtkLM2Actor();
	int Type;
	vtkLM2ActorUndoRedo* UndoRedo;


private:
	vtkLM2Actor(const vtkLM2Actor&);  // Not implemented.
	void operator=(const vtkLM2Actor&);  // Not implemented.
	
};

#endif

