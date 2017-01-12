/*=========================================================================

Program:   MeshTools
Module:    vtkMTActor.cxx
=========================================================================*/
#include "vtkMTActor.h"

#include <vtkObjectFactory.h>
#include <vtkMath.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkTexture.h>
#include <vtkTransform.h>
vtkStandardNewMacro(vtkMTActor);



//----------------------------------------------------------------------------
vtkMTActor::vtkMTActor()
{
	this->UndoRedo = new vtkMTActorUndoRedo;
	this->Selected = 1;
	this->Changed = 0;
	
}

//----------------------------------------------------------------------------
vtkMTActor::~vtkMTActor()
{
	this->UndoRedo->RedoStack.clear();
	this->UndoRedo->UndoStack.clear();
	delete this->UndoRedo;
	

}

void vtkMTActor::Undo(int mCount)
{

	cout << "Inside actor Undo" << endl;
	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}
	if (mCount == this->UndoRedo->UndoStack.back().UndoCount)
	{
		cout << "Undo actor event " << this->UndoRedo->UndoStack.back().UndoCount << endl;
		// ici : faire l'appel global à undo de ce count là!!  
		this->PopUndoStack();
	}
	
	
	

}
void vtkMTActor::Redo(int mCount)
{
	cout << "Inside actor Undo" << endl;
	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}
	if (mCount == this->UndoRedo->RedoStack.back().UndoCount)
	{
		cout << "Redo actor event " << this->UndoRedo->RedoStack.back().UndoCount << endl;
		// ici : faire l'appel global à undo de ce count là!!  
		this->PopRedoStack();
	}
}
void vtkMTActor::Erase(int mCount)
{
	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}
	int oldestCount = this->UndoRedo->UndoStack.front().UndoCount;
	if (oldestCount <= mCount)
	{
		//cout << "ERASE actor event " << oldestCount << endl;
		this->UndoRedo->UndoStack.erase(this->UndoRedo->UndoStack.begin());
	}
}

void vtkMTActor::PopUndoStack()
{
	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}
		
	vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
	this->GetMatrix(Mat);	
	vtkSmartPointer<vtkMatrix4x4> SavedMat = vtkSmartPointer<vtkMatrix4x4>::New();
	SavedMat->DeepCopy(Mat);
	// Now put undo Matrix inside object : 
	Mat->DeepCopy(this->UndoRedo->UndoStack.back().Matrix);
	
	std::cout << "Old Matrix: " << endl << *SavedMat << std::endl;
	std::cout << "New Matrix: " << endl << *Mat << std::endl;

	vtkProp3D *prop3D = vtkProp3D::SafeDownCast(this);
	vtkTransform *newTransform = vtkTransform::New();
	newTransform->PostMultiply();
	newTransform->SetMatrix(Mat);
	prop3D->SetPosition(newTransform->GetPosition());
	prop3D->SetScale(newTransform->GetScale());
	prop3D->SetOrientation(newTransform->GetOrientation());
	newTransform->Delete();


	this->GetMatrix(Mat);
	std::cout << "Real Matrix: " << endl << *Mat << std::endl;
	this->UndoRedo->RedoStack.push_back(vtkMTActorUndoRedo::Element(SavedMat, this->UndoRedo->UndoStack.back().UndoCount));
	this->UndoRedo->UndoStack.pop_back();
	this->Modified();
}
void vtkMTActor::PopRedoStack()
{
	if (this->UndoRedo->RedoStack.empty())
	{
		return;
	}
	vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
	this->GetMatrix(Mat);
	vtkSmartPointer<vtkMatrix4x4> SavedMat = vtkSmartPointer<vtkMatrix4x4>::New();
	SavedMat->DeepCopy(Mat);
	// Now put redp Matrix inside object : 
	Mat->DeepCopy(this->UndoRedo->RedoStack.back().Matrix);
	vtkProp3D *prop3D = vtkProp3D::SafeDownCast(this);
	vtkTransform *newTransform = vtkTransform::New();
	newTransform->PostMultiply();
	newTransform->SetMatrix(Mat);
	prop3D->SetPosition(newTransform->GetPosition());
	prop3D->SetScale(newTransform->GetScale());
	prop3D->SetOrientation(newTransform->GetOrientation());
	newTransform->Delete();

	this->UndoRedo->UndoStack.push_back(vtkMTActorUndoRedo::Element(SavedMat, this->UndoRedo->RedoStack.back().UndoCount));
	this->UndoRedo->RedoStack.pop_back();
	this->Modified();
}

void vtkMTActor::SavePosition(int mCount)
{
	//cout << "myActor Save Position: redostack clear." << endl;
	this->UndoRedo->RedoStack.clear();

	//int Count = 2;//MeshTools::instance()->GetUndoCount()+1;
	int Count = mCount;

	
	// détruit ce qui est trop vieux dans le vector!
	//to do!
	vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
	this->GetMatrix(Mat);
	//std::cout << "Saved Matrix: " << endl << *Mat << std::endl;

	vtkSmartPointer<vtkMatrix4x4> SavedMat = vtkSmartPointer<vtkMatrix4x4>::New();
	SavedMat->DeepCopy(Mat);
	//std::cout << "Saved Matrix Copy: " << endl << *SavedMat << std::endl;
	this->UndoRedo->UndoStack.push_back(vtkMTActorUndoRedo::Element(SavedMat, mCount));

}
//----------------------------------------------------------------------------
// Shallow copy of an actor.
void vtkMTActor::ShallowCopy(vtkProp *prop)
{
	vtkMTActor *f = vtkMTActor::SafeDownCast(prop);
	

	// Now do superclass
	this->vtkOpenGLActor::ShallowCopy(prop);
}


//----------------------------------------------------------------------------
void vtkMTActor::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	os << indent << "Selected: "<< this->Selected;
	

}
// Actual actor render method.
void vtkMTActor::Render(vtkRenderer *ren, vtkMapper *mapper)
{
	this->Superclass::Render(ren, mapper);
}