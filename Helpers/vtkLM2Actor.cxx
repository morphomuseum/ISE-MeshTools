/*=========================================================================

Program:   MeshTools
Module:    vtkLM2Actor.cxx
=========================================================================*/
#include "vtkLM2Actor.h"

#include <vtkObjectFactory.h>
#include <vtkMath.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkTexture.h>
#include <vtkTransform.h>
vtkStandardNewMacro(vtkLM2Actor);



//----------------------------------------------------------------------------
vtkLM2Actor::vtkLM2Actor()
{
	this->UndoRedo = new vtkLM2ActorUndoRedo;
	this->Selected = 1;
	this->Changed = 0;
	
}

//----------------------------------------------------------------------------
vtkLM2Actor::~vtkLM2Actor()
{
	this->UndoRedo->RedoStack.clear();
	this->UndoRedo->UndoStack.clear();
	delete this->UndoRedo;
	

}
void vtkLM2Actor::Undo(int mCount)
{

	//cout << "Inside actor Undo" << endl;
	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}
	if (mCount == this->UndoRedo->UndoStack.back().UndoCount)
	{
		//cout << "Undo actor event " << this->UndoRedo->UndoStack.back().UndoCount << endl;
		// ici : faire l'appel global à undo de ce count là!!  
		this->PopUndoStack();
	}
	
	
	

}
void vtkLM2Actor::Redo(int mCount)
{
	//cout << "Inside actor Undo, try to undo " <<mCount<< endl;
	if (this->UndoRedo->RedoStack.empty())
	{
		cout << "Redo Stack empty!" << endl;
		return;
	}
	//cout << "Youngest redo count= " << this->UndoRedo->RedoStack.back().UndoCount<< endl;

	if (mCount == this->UndoRedo->RedoStack.back().UndoCount)
	{
		//cout << "Redo actor event " << this->UndoRedo->RedoStack.back().UndoCount << endl;
		// ici : faire l'appel global à undo de ce count là!!  
		this->PopRedoStack();
	}
}
void vtkLM2Actor::Erase(int mCount)
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

void vtkLM2Actor::PopUndoStack()
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

	
	
	double myCurrentColor[4];
	int mCurrentSelected = this->Selected;
	myCurrentColor[0] = this->mColor[0];
	myCurrentColor[1] = this->mColor[1];
	myCurrentColor[2] = this->mColor[2];
	myCurrentColor[3] = this->mColor[3];
	int myCurrentType = this->Type;
	this->Type = this->UndoRedo->UndoStack.back().Type;
	this->mColor[0] = this->UndoRedo->UndoStack.back().Color[0];
	this->mColor[1] = this->UndoRedo->UndoStack.back().Color[1];
	this->mColor[2] = this->UndoRedo->UndoStack.back().Color[2];
	this->mColor[3] = this->UndoRedo->UndoStack.back().Color[3];
	this->SetSelected(this->UndoRedo->UndoStack.back().Selected);
	cout << "PopUndoStack Set Selected: " << mCurrentSelected << endl;
	this->UndoRedo->RedoStack.push_back(vtkLM2ActorUndoRedo::Element(SavedMat, myCurrentColor, mCurrentSelected,myCurrentType, this->UndoRedo->UndoStack.back().UndoCount));
	this->UndoRedo->UndoStack.pop_back();
	this->Modified();
}
void vtkLM2Actor::PopRedoStack()
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

	double myCurrentColor[4];
	int mCurrentSelected = this->Selected;
	myCurrentColor[0] = this->mColor[0];
	myCurrentColor[1] = this->mColor[1];
	myCurrentColor[2] = this->mColor[2];
	myCurrentColor[3] = this->mColor[3];
	int myCurrentType = this->Type;
	this->Type = this->UndoRedo->RedoStack.back().Type;
	this->mColor[0] = this->UndoRedo->RedoStack.back().Color[0];
	this->mColor[1] = this->UndoRedo->RedoStack.back().Color[1];
	this->mColor[2] = this->UndoRedo->RedoStack.back().Color[2];
	this->mColor[3] = this->UndoRedo->RedoStack.back().Color[3];
	this->SetSelected(this->UndoRedo->RedoStack.back().Selected);
	cout << "PopRedoStack Set Selected: " << mCurrentSelected << endl;
	this->UndoRedo->UndoStack.push_back(vtkLM2ActorUndoRedo::Element(SavedMat, myCurrentColor, mCurrentSelected, myCurrentType, this->UndoRedo->RedoStack.back().UndoCount));
	this->UndoRedo->RedoStack.pop_back();
	this->Modified();
}

void vtkLM2Actor::SaveState(int mCount)
{
	//cout << "myActor Save Position: redostack clear." << endl;
	this->UndoRedo->RedoStack.clear();

	//int Count = 2;//MeshTools::instance()->GetUndoCount()+1;
	int Count = mCount;

	
	// détruit ce qui est trop vieux dans le vector!
	//to do!
	vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
	this->GetMatrix(Mat);
	double myColor[4];
	int mSelected = this->Selected;
	myColor[0] = this->mColor[0];
	myColor[1] = this->mColor[1];
	myColor[2] = this->mColor[2];
	myColor[3] = this->mColor[3];
	//std::cout << "Saved Matrix: " << endl << *Mat << std::endl;
	int mType = this->Type;
	vtkSmartPointer<vtkMatrix4x4> SavedMat = vtkSmartPointer<vtkMatrix4x4>::New();
	SavedMat->DeepCopy(Mat);
	//std::cout << "Saved Matrix Copy: " << endl << *SavedMat << std::endl;
	this->UndoRedo->UndoStack.push_back(vtkLM2ActorUndoRedo::Element(SavedMat, myColor, mSelected, mType, mCount));

}
//----------------------------------------------------------------------------
// Shallow copy of an actor.
void vtkLM2Actor::ShallowCopy(vtkProp *prop)
{
	vtkLM2Actor *f = vtkLM2Actor::SafeDownCast(prop);
	

	// Now do superclass
	this->vtkMTActor::ShallowCopy(prop);
}


//----------------------------------------------------------------------------
void vtkLM2Actor::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	os << indent << "Selected: "<< this->Selected;
	

}
// Actual actor render method.
void vtkLM2Actor::Render(vtkRenderer *ren, vtkMapper *mapper)
{
	this->Superclass::Render(ren, mapper);
}