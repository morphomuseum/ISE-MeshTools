/*=========================================================================

Program:   MeshTools
Module:    vtkLMActor.h



=========================================================================*/


#ifndef vtkLMActor_h
#define vtkLMActor_h


#include <vtkOpenGLActor.h>
#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>
#include <vector>

class vtkActor;
class vtkCaptionActor2D;
class vtkConeSource;
class vtkCylinderSource;
class vtkLineSource;
class vtkPolyData;
class vtkPropCollection;
class vtkProperty;
class vtkRenderer;
class vtkSphereSource;

class vtkLMActorUndoRedo
{
public:
	struct Element
	{
		vtkSmartPointer<vtkMatrix4x4> Matrix;
		//double Color[4];
		int Selected;
		int Number;
		int Type;
		int UndoCount;
		std::string Label;
		Element(vtkSmartPointer<vtkMatrix4x4> m, 
//			double c[4], 
			int selected, int number, int type, std::string label, int Count)
		{
			this->Matrix = m;
			this->Number = number;
			this->Type = type;
			this->Label = label;
			this->UndoCount = Count;
			/*this->Color[0] = c[0];
			this->Color[1] = c[1];
			this->Color[2] = c[2];
			this->Color[3] = c[3];*/
			this->Selected = selected;
		}
	};
	typedef std::vector<Element> VectorOfElements;
	VectorOfElements UndoStack;
	VectorOfElements RedoStack;
};

class  vtkLMActor : public  vtkOpenGLActor
{
public:
	static vtkLMActor *New();
	vtkTypeMacro(vtkLMActor, vtkProp3D);
	void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// For some exporters and other other operations we must be
	// able to collect all the actors or volumes. These methods
	// are used in that process.
	virtual void GetActors(vtkPropCollection *);

	// Description:
	// Support the standard render methods.
	virtual int RenderOpaqueGeometry(vtkViewport *viewport);
	virtual int RenderTranslucentPolygonalGeometry(vtkViewport *viewport);
	virtual int RenderOverlay(vtkViewport *viewport);

	// Description:
	// Does this prop have some translucent polygonal geometry?
	virtual int HasTranslucentPolygonalGeometry();

	// Description:
	// Shallow copy of an axes actor. Overloads the virtual vtkProp method.
	void ShallowCopy(vtkProp *prop);

	// Description:
	// Release any graphics resources that are being consumed by this actor.
	// The parameter window could be used to determine which graphic
	// resources to release.
	void ReleaseGraphicsResources(vtkWindow *);

	// Description:
	// Get the bounds for this Actor as (Xmin,Xmax,Ymin,Ymax,Zmin,Zmax). (The
	// method GetBounds(double bounds[6]) is available from the superclass.)
	
	double *GetBounds();
	vtkPolyData *GetSphere() { return this->LMSphere; }
	void SetLMOrigin(double x, double y, double z);
	void SetLMOrigin(double origin[3]);
	void GetLMOrigin(double origin[3]);
	double * GetLMOrigin();

	void SetLMOrientation(double x, double y, double z);
	void SetLMOrientation(double orientation[3]);
	void GetLMOrientation(double orientation[3]);
	double * GetLMOrientation();

	void SaveState(int mCount);
	void Redo(int mCount); // Try to redo (if exists) "mCount" event
	void Erase(int mCount); // Try to erase (if exists) "mCount" event
	void Undo(int mCount); // Try to undo (if exists) "mCount" event
	void PopUndoStack();
	void PopRedoStack();
	// Description:
	// Get the actors mtime plus consider its properties and texture if set.

#if VTK_MINOR_VERSION >= 1

	/**
	* Get the actors mtime plus consider its properties and texture if set.
	*/
	vtkMTimeType GetMTime();

	/**
	* Return the mtime of anything that would cause the rendered image to
	* appear differently. Usually this involves checking the mtime of the
	* prop plus anything else it depends on such as properties, textures
	* etc.
	*/
	virtual vtkMTimeType GetRedrawMTime();

#else
	unsigned long int GetMTime();

	// Description:
	// Return the mtime of anything that would cause the rendered image to
	// appear differently. Usually this involves checking the mtime of the
	// prop plus anything else it depends on such as properties, textures
	// etc.

	virtual unsigned long GetRedrawMTime();
#endif







	// Description:
	// Retrieve handles to the X, Y and Z axis (so that you can set their text
	// properties for example)
	vtkCaptionActor2D *GetLMLabelActor2D()
	{
		return this->LMLabel;
	}


	// Description:
	// Set/get the label text.
	vtkSetStringMacro(LMLabelText);
	vtkGetStringMacro(LMLabelText);



	// Description:


	// Enable/disable drawing the axis labels.
	vtkSetMacro(LMDrawLabel, int);
	vtkGetMacro(LMDrawLabel, int);

	void SetSelected(int selected);
	vtkGetMacro(Selected, int);

	vtkBooleanMacro(LMDrawLabel, int);
	vtkGetMacro(LMType, int);
	void SetLMType(int type);
	vtkGetMacro(LMBodyType, int);
	void SetLMBodyType(int type);
	vtkGetMacro(LMSize, double);
	void SetLMSize(double size);

	vtkGetMacro(LMNumber, int);
	void SetLMNumber(int num);

	vtkSetVector4Macro(LMColor, double);
	vtkGetVector4Macro(LMColor, double);
	void SetLMColor();
protected:
	vtkLMActor();
	~vtkLMActor();

	vtkPolyData *LMSphere;
	vtkOpenGLActor          *LMBody;
	void               UpdateProps();
	char              *LMLabelText;
	vtkCaptionActor2D *LMLabel;
	int LMNumber; // landmark number in sequence
	double LMColor[4]; // mesh "uniform" color (no vertex coloring) and transparency.
	int LMBodyType; //0; sphere //1 needle needle 
	int LMType; // 0; normal landmark (red) // 1 target landmark (yellow)
				// 2; curve node (dark red) // 3; curve handle (orange) // 4 curve starting point (green)
				// 5 curve milestone (blue) // 6 curve ending point (cyann)
	int                LMDrawLabel;
	double		     LMSize;
	double			LMOrigin[3];
	double			LMOrientation[3];
	int Selected;
	vtkLMActorUndoRedo* UndoRedo;
private:
	vtkLMActor(const vtkLMActor&);  // Not implemented.
	void operator=(const vtkLMActor&);  // Not implemented.
	void CreateLMBody(); //instantiates LM Body (sphere or needle)
	void CreateLMLabelText();//instantiates the  label actor



};

#endif

