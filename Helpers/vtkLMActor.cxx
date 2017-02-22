/*=========================================================================

Program:   MeshTools
Module:    vtkLMActor.cxx
=========================================================================*/
#include "vtkLMActor.h"
#include  "vtkProbeSource.h"

#include <vtkTextProperty.h>
#include <vtkSphereSource.h>

#include <vtkObjectFactory.h>
#include <vtkMath.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkTexture.h>
#include <vtkTransform.h>
vtkStandardNewMacro(vtkLMActor);



//----------------------------------------------------------------------------
vtkLMActor::vtkLMActor()
{
	this->UndoRedo = new vtkLMActorUndoRedo;
	this->Selected = 0;
	this->Changed = 0;
	this->LMBodyType = 0;

	this->LMBody = vtkSmartPointer<vtkPolyData>::New();
	this->LMLabel = vtkCaptionActor2D::New();
	
	this->LMDrawLabel = 1;//Draws the label
	this->LMType = 0;
	this->SetLMColor(); //Set color according to LMType
	this->LMBodyType = 0; //sphere by default
	this->LMSize = 0.1; // 0.1mm by default 
	this->LMNumber = 1; //1
	this->LMOrigin[0] = 0;
	this->LMOrigin[1] = 0;
	this->LMOrigin[2] = 0;

	this->LMOrientation[0] = 1;
	this->LMOrientation[1] = 0;
	this->LMOrientation[2] = 0;

	this->LMLabelText = NULL;
	
	this->UpdateProps();
	
}

//----------------------------------------------------------------------------
vtkLMActor::~vtkLMActor()
{
	this->UndoRedo->RedoStack.clear();
	this->UndoRedo->UndoStack.clear();
	delete this->UndoRedo;
	

}

void vtkLMActor::SetLMOriginAndOrientation(double origin[3], double orientation[3])
{
	
		

	
	if (this->LMOrigin[0] != origin[0]
		|| this->LMOrigin[1] != origin[1]
		|| this->LMOrigin[2] != origin[2]
		|| this->LMOrientation[0] != orientation[0]
		|| this->LMOrientation[1] != orientation[1]
		|| this->LMOrientation[2] != orientation[2]

		)
	{
		this->LMOrigin[0] = origin[0];
		this->LMOrigin[1] = origin[1];
		this->LMOrigin[2] = origin[2];
		this->LMOrientation[0] = orientation[0];
		this->LMOrientation[1] = orientation[1];
		this->LMOrientation[2] = orientation[2];


		this->Modified();
		cout << "Update proprs" << endl;
		this->UpdateProps();
	}
}


void vtkLMActor::SetLMOrigin(double x, double y, double z)
{
	double origin[3];
	origin[0] = x;
	origin[1] = y;
	origin[2] = z;
	this->SetLMOrigin(origin);
}
void vtkLMActor::SetLMOrigin(double origin[3])
{

	if (this->LMOrigin[0] != origin[0]
		|| this->LMOrigin[1] != origin[1]
		|| this->LMOrigin[2] != origin[2]

		)
	{
		this->LMOrigin[0] = origin[0];
		this->LMOrigin[1] = origin[1];
		this->LMOrigin[2] = origin[2];


		this->Modified();
		//cout << "Update proprs" << endl;
		this->UpdateProps();
	}
}
void vtkLMActor::GetLMOrigin(double origin[3])
{
	double *org = this->GetLMOrigin();

	origin[0] = org[0];
	origin[1] = org[1];
	origin[2] = org[2];

}



double *vtkLMActor::GetLMOrigin()
{
	return this->LMOrigin;
}


void vtkLMActor::SetLMOrientation(double x, double y, double z)
{
	double orientation[3] = { x,y,z };

	this->SetLMOrientation(orientation);
}
void vtkLMActor::SetLMOrientation(double orientation[3])
{

	if (this->LMOrientation[0] != orientation[0]
		|| this->LMOrientation[1] != orientation[1]
		|| this->LMOrientation[2] != orientation[2]

		)
	{
		this->LMOrientation[0] = orientation[0];
		this->LMOrientation[1] = orientation[1];
		this->LMOrientation[2] = orientation[2];


		this->Modified();
		this->UpdateProps();
	}
}
void vtkLMActor::SetLMSize(double size)
{
	if (this->LMType == 1 || this->LMType == 3)
	{
	}
	this->LMSize = size;
	/*if (this->LMType == 1 || this->LMType == 3)
	{
		this->LMSize = 1.2*size;
	}*/
	this->Modified();
	this->UpdateProps();
}
void vtkLMActor::GetLMOrientation(double orientation[3])
{
	double *ori = this->GetLMOrientation();

	orientation[0] = ori[0];
	orientation[1] = ori[1];
	orientation[2] = ori[2];

}



double *vtkLMActor::GetLMOrientation()
{
	return this->LMOrientation;
}

void vtkLMActor::TransformPoint(vtkMatrix4x4* matrix, double pointin[3], double pointout[3]) {
	double pointPred[4]; double pointNew[4] = { 0, 0, 0, 0 };
	pointPred[0] = pointin[0];
	pointPred[1] = pointin[1];
	pointPred[2] = pointin[2];
	pointPred[3] = 1;

	matrix->MultiplyPoint(pointPred, pointNew);
	pointout[0] = pointNew[0];
	pointout[1] = pointNew[1];
	pointout[2] = pointNew[2];
}

void vtkLMActor::CreateLMLabelText()
{
	//instantiates the label

	// LMType : 
	// 0 : -x x -y y
	// 1 :  -x x -z z
	// 2 : -y y -z z
	// Position of the label can be computed using this matrix!

	double init_pos[3] = { 0,0,0 };
	double mult = 1;
	if (this->LMType == 1 || this->LMType == 3)
	{
		mult = 1.2;
	}
	if (this->LMBodyType == 0)
	{
		init_pos[0] += 0.5*1.1*mult*this->LMSize;
	}
	else
	{
		init_pos[0] += 3*1.1*mult*this->LMSize;
	}

	
	double final_pos[3];

	

	this->TransformPoint(this->GetMatrix(), init_pos, final_pos);


	vtkSmartPointer<vtkTextProperty> mproperty = vtkSmartPointer<vtkTextProperty>::New();

	double color[3] = { this->mColor[0], this->mColor[1], this->mColor[2] };


	std::string myStrLabel;
	myStrLabel = std::to_string(this->LMNumber);
	mproperty->SetColor(color);



	mproperty->SetFontFamilyToArial();

	this->LMLabel->SetCaptionTextProperty(mproperty);
	this->LMLabel->SetPosition(0, 0);
	this->LMLabel->SetAttachmentPoint(final_pos);
	this->LMLabel->SetHeight(0.03);
	this->LMLabel->BorderOff();
	this->LMLabel->LeaderOff();
	this->SetLMLabelText(myStrLabel.c_str());
	this->LMLabel->SetCaption(this->LMLabelText);

}

void vtkLMActor::CreateLMBody()
{

	
	vtkMatrix4x4 *Mat = this->GetMatrix();
	
	/*cout << "Initial matrix:" << endl;
	Mat->PrintSelf(cout, vtkIndent(1));*/
	Mat->SetElement(0, 3, this->LMOrigin[0]);
	Mat->SetElement(1, 3, this->LMOrigin[1]);
	Mat->SetElement(2, 3, this->LMOrigin[2]);

	vtkMath::Normalize(this->LMOrientation);
			
	// rotation 
	// Landmark object oriented along x axis : 1 0 0 
	// assume rot.  
	// 1) around z 
	// 2)    around x 
	// -> values sinz,cosz,sinx,cosx need to be computed

	float cosz = this->LMOrientation[0]; // nx
	float sinz = sqrt(1.0 - (cosz*cosz));
	float cosx, sinx;
	if (sinz == 0.0) // just in case we are in the speciale case
	{
		cosx = 1.0; sinx = 0.0;
	}
	else
	{
		cosx = this->LMOrientation[1] / sinz;

		sinx = this->LMOrientation[2] / sinz;
	}
	
	Mat->SetElement(0, 0, cosz); //=nx
	Mat->SetElement(1, 0, cosx*sinz); //=ny
	Mat->SetElement(2, 0, sinx *sinz);    //=nz

	Mat->SetElement(0, 1, -sinz);
	Mat->SetElement(1, 1, cosx*cosz);
	Mat->SetElement(2, 1, sinx*cosz);

	Mat->SetElement(0, 2, 0); 
	Mat->SetElement(1, 2, -sinx);
	Mat->SetElement(2, 2, cosx); 


	/*cout << "Modified matrix:" << endl;
	Mat->PrintSelf(cout, vtkIndent(1));*/

	
	this->SetUserMatrix(Mat);

	double pos[3];
	this->GetPosition(pos);
	
	
	if (this->LMBodyType == 0)
	{
		vtkSmartPointer<vtkSphereSource> sphereSource =
			vtkSmartPointer<vtkSphereSource>::New();

		sphereSource->SetCenter(0, 0, 0);
		sphereSource->SetRadius(0.5*this->LMSize);
		if (this->LMType == 1 || this->LMType == 3)
		{
			sphereSource->SetRadius(0.5*1.2*this->LMSize);
		}
		sphereSource->Update();
		this->LMBody = sphereSource->GetOutput();
	}
	else
	{
		vtkSmartPointer<vtkProbeSource> probeSource =
			vtkSmartPointer<vtkProbeSource>::New();

		probeSource->SetInvert(true);		
		probeSource->SetArrowLength(3*this->LMSize);
		if (this->LMType == 1 || this->LMType == 3)
		{
			probeSource->SetArrowLength(3*1.2*this->LMSize);
		}
		probeSource->Update();
		this->LMBody = probeSource->GetOutput();
	}




}



void vtkLMActor::SetLMColor()
{
	// Create six colors - one for each line
	double red[4] = { 1, 0.4, 0.4, 1 };// LMType=0 VERT

	double yellow[4] = { 1, 1, 0,0.5 }; 
	double darkred[4] = { 0.5, 0, 0, 1 }; 
	double orange[4] = { 1, 0.5, 0, 0.5 }; 
	double green[4] = { 0.5, 1, 0, 1 };  
	double blue[4] = { 0, 0.5, 1, 1 }; 
	
	double cyan[4] = { 0, 1, 1, 1 }; 
	double violet[4] = { 0.7, 0, 1, 0.5 }; 
	if (this->LMType == 0) { this->SetmColor(green); }// LMType = 0 (normal LM) vert
	if (this->LMType == 1) { this->SetmColor(orange); }// LMType = 1 (target LM) jaune
	if (this->LMType == 2) { this->SetmColor(red); }// LMType = 2( curve node LM) rosé
	if (this->LMType == 3) { this->SetmColor(violet); }// LMType = 3( curve handle) violet
	if (this->LMType == 4) { this->SetmColor(darkred); }// LMType = 4( curve starting point) rouge sombre
	if (this->LMType == 5) { this->SetmColor(blue); } // LMType=5 (curve milestone) bleu
	if (this->LMType == 6) { this->SetmColor(cyan); }// LMType=6 (curve end point start to 0) cyan
}

//----------------------------------------------------------------------------
void vtkLMActor::SetLMType(int type)
{
	this->LMType = type;
	this->SetLMColor();
	if (this->LMType != type && type >= 0 && type <= 6)
	{


		
		this->LMType = type;
		this->Modified();
		this->UpdateProps();
	}


}

void vtkLMActor::SetLMBodyType(int type)
{
	if (this->LMBodyType != type && type >= 0 && type <= 1)
	{
		this->LMBodyType = type;
		this->Modified();
		this->UpdateProps();
	}


}
/*
void vtkLMActor::SetLMSize(double size)
{
	if (this->LMSize != size && size>0)
	{
		this->LMSize = size;
		this->Modified();
		this->UpdateProps();
	}

}*/


void vtkLMActor::SetLMNumber(int num)
{
	if (this->LMNumber != num && num>0)
	{
		this->LMNumber = num;
		this->Modified();
		this->UpdateProps();
	}

}

void vtkLMActor::Undo(int mCount)
{

	cout << "Inside LM actor Undo" << endl;
	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}
	if (mCount == this->UndoRedo->UndoStack.back().UndoCount)
	{
		//cout << "Undo actor event " << this->UndoRedo->UndoStack.back().UndoCount << endl;
		// ici : faire l'appel global à undo de ce count là!!  
		cout << "Call Pop Undo Stack from LMActor" << endl;
		this->PopUndoStack();
	}
	
	
	

}
void vtkLMActor::Redo(int mCount)
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
void vtkLMActor::Erase(int mCount)
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

void vtkLMActor::PopUndoStack()
{
	cout << "Current LM PopUndoStack: " << endl;
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
	
	std::cout << "Matrix about to be saved: " << endl << *SavedMat << std::endl;
	std::cout << "Matrix retrieved in stack, about to be set: " << endl << *Mat << std::endl;
	/*
	vtkProp3D *prop3D = vtkProp3D::SafeDownCast(this);
	vtkTransform *newTransform = vtkTransform::New();
	newTransform->PostMultiply();
	newTransform->SetMatrix(Mat);
	prop3D->SetPosition(newTransform->GetPosition());
	prop3D->SetScale(newTransform->GetScale());
	prop3D->SetOrientation(newTransform->GetOrientation());
	newTransform->Delete();
	*/
	this->SetUserMatrix(Mat);
	this->GetMatrix(Mat);
	
	std::cout << "Matrix after setting : " << endl << *Mat << std::endl;

	
	
	double myCurrentColor[4];
	int mCurrentSelected = this->Selected;
	myCurrentColor[0] = this->mColor[0];
	myCurrentColor[1] = this->mColor[1];
	myCurrentColor[2] = this->mColor[2];
	myCurrentColor[3] = this->mColor[3];
	int myCurrentType = this->LMType;
	this->LMType = this->UndoRedo->UndoStack.back().Type;
	this->mColor[0] = this->UndoRedo->UndoStack.back().Color[0];
	this->mColor[1] = this->UndoRedo->UndoStack.back().Color[1];
	this->mColor[2] = this->UndoRedo->UndoStack.back().Color[2];
	this->mColor[3] = this->UndoRedo->UndoStack.back().Color[3];
	this->SetSelected(this->UndoRedo->UndoStack.back().Selected);
	cout << "PopUndoStack Set Selected: " << mCurrentSelected << endl;
	this->UndoRedo->RedoStack.push_back(vtkLMActorUndoRedo::Element(SavedMat, myCurrentColor, mCurrentSelected,myCurrentType, this->UndoRedo->UndoStack.back().UndoCount));
	this->UndoRedo->UndoStack.pop_back();
	this->CreateLMLabelText();
	this->Modified();
	//this->UpdateProps();
}
void vtkLMActor::PopRedoStack()
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
	/*vtkProp3D *prop3D = vtkProp3D::SafeDownCast(this);
	vtkTransform *newTransform = vtkTransform::New();
	newTransform->PostMultiply();
	newTransform->SetMatrix(Mat);
	prop3D->SetPosition(newTransform->GetPosition());
	prop3D->SetScale(newTransform->GetScale());
	prop3D->SetOrientation(newTransform->GetOrientation());
	newTransform->Delete();*/
	this->SetUserMatrix(Mat);
	this->GetMatrix(Mat);

	double myCurrentColor[4];
	int mCurrentSelected = this->Selected;
	myCurrentColor[0] = this->mColor[0];
	myCurrentColor[1] = this->mColor[1];
	myCurrentColor[2] = this->mColor[2];
	myCurrentColor[3] = this->mColor[3];
	int myCurrentType = this->LMType;
	this->LMType = this->UndoRedo->RedoStack.back().Type;
	this->mColor[0] = this->UndoRedo->RedoStack.back().Color[0];
	this->mColor[1] = this->UndoRedo->RedoStack.back().Color[1];
	this->mColor[2] = this->UndoRedo->RedoStack.back().Color[2];
	this->mColor[3] = this->UndoRedo->RedoStack.back().Color[3];
	this->SetSelected(this->UndoRedo->RedoStack.back().Selected);
	cout << "PopRedoStack Set Selected: " << mCurrentSelected << endl;
	this->UndoRedo->UndoStack.push_back(vtkLMActorUndoRedo::Element(SavedMat, myCurrentColor, mCurrentSelected, myCurrentType, this->UndoRedo->RedoStack.back().UndoCount));
	this->UndoRedo->RedoStack.pop_back();
	this->CreateLMLabelText();
	this->Modified();
}

void vtkLMActor::SaveState(int mCount)
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
	int mType = this->LMType;
	vtkSmartPointer<vtkMatrix4x4> SavedMat = vtkSmartPointer<vtkMatrix4x4>::New();
	SavedMat->DeepCopy(Mat);
	cout << "Saved current LM state: " << endl;
	this->UndoRedo->UndoStack.push_back(vtkLMActorUndoRedo::Element(SavedMat, myColor, mSelected, mType, mCount));

}
//----------------------------------------------------------------------------
// Shallow copy of an actor.
void vtkLMActor::ShallowCopy(vtkProp *prop)
{
	vtkLMActor *f = vtkLMActor::SafeDownCast(prop);
	

	// Now do superclass
	this->vtkMTActor::ShallowCopy(prop);
}

void vtkLMActor::UpdateProps()
{
	//Recreates the text actor and the LMBody

	this->CreateLMBody();
	this->CreateLMLabelText();

}

//----------------------------------------------------------------------------
void vtkLMActor::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	os << indent << "Selected: "<< this->Selected;
	

}
// Actual actor render method.
void vtkLMActor::Render(vtkRenderer *ren, vtkMapper *mapper)
{
	this->Superclass::Render(ren, mapper);
}