/*=========================================================================

Program:   MeshTools
Module:    vtkLMActor.h



=========================================================================*/


#ifndef vtkLMActor_h
#define vtkLMActor_h


#include "vtkMTActor.h"
#include <vtkMatrix4x4.h>
#include <vtkPolyData.h>
#include <vtkCaptionActor2D.h>
#include <vtkSmartPointer.h>
#include <vector>

class vtkLMActorUndoRedo
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

class  vtkLMActor : public vtkMTActor
{
public:
	static vtkLMActor *New();
	
	vtkTypeMacro(vtkLMActor, vtkOpenGLActor);
	void PrintSelf(ostream& os, vtkIndent indent);

	
	// Description:
	void ShallowCopy(vtkProp *prop);


	
	
	

	// Actual actor render method.
	void Render(vtkRenderer *ren, vtkMapper *mapper);
	virtual void SaveState(int mCount);
	virtual void Redo(int mCount); // Try to redo (if exists) "mCount" event
	virtual void Erase(int mCount); // Try to erase (if exists) "mCount" event
	virtual void Undo(int mCount); // Try to undo (if exists) "mCount" event
	virtual void PopUndoStack();
	virtual void PopRedoStack();

	// Landmark 100% specific methods
	vtkSmartPointer<vtkCaptionActor2D> GetLMLabelActor2D(){	return this->LMLabel;}
	vtkSetStringMacro(LMLabelText);
	vtkGetStringMacro(LMLabelText);
	vtkSmartPointer<vtkPolyData> getLMBody() { return this->LMBody; }
	// Enable/disable drawing the axis labels.
	vtkSetMacro(LMDrawLabel, int);
	vtkGetMacro(LMDrawLabel, int);
	vtkGetMacro(LMType, int);
	void SetLMType(int type);
	vtkGetMacro(LMBodyType, int);
	void SetLMBodyType(int type);
	vtkGetMacro(LMSize, double);
	void SetLMSize(double size);
	vtkGetMacro(LMNumber, int);
	void SetLMNumber(int num);
	void SetLMOrigin(double x, double y, double z);
	void SetLMOrigin(double origin[3]);
	void GetLMOrigin(double origin[3]);
	double * GetLMOrigin();
	void SetLMColor();
	void SetLMOrientation(double x, double y, double z);
	void SetLMOrientation(double orientation[3]);
	void GetLMOrientation(double orientation[3]);
	double * GetLMOrientation();
	

protected:
	vtkLMActor();
	~vtkLMActor();
	void               UpdateProps();

	vtkSmartPointer<vtkPolyData> LMBody;
	vtkLMActorUndoRedo* UndoRedo;
	char              *LMLabelText;
	vtkSmartPointer<vtkCaptionActor2D> LMLabel;
	int LMNumber; // landmark number in sequence
	int LMBodyType; //0; sphere //1 needle needle 
	int LMType; // 0; normal landmark (red) // 1 target landmark (yellow)
				// 2; curve node (dark red) // 3; curve handle (orange) // 4 curve starting point (green)
				// 5 curve milestone (blue) // 6 curve ending point (cyann)
	int                LMDrawLabel;
	double		     LMSize;
	double			LMOrigin[3];
	double			LMOrientation[3];

private:
	vtkLMActor(const vtkLMActor&);  // Not implemented.
	void operator=(const vtkLMActor&);  // Not implemented.
	void CreateLMBody(); //instantiates LM Body (sphere or needle)
	void CreateLMLabelText();//instantiates the  label actor
	
};

#endif

