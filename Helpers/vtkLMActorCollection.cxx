/*=========================================================================

Program:   MeshTools
Module:    vtkMTActorCollection.cxx
=========================================================================*/
#include "vtkLMActorCollection.h"
#include "vtkLMActor.h"
#include "vtkMTActor.h"
#include <vtkObjectFactory.h>
#include <vtkMath.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkCenterOfMass.h>
#include <vtkSmartPointer.h>


#include <vtkDataSet.h>
#include <vtkMapper.h>

#include "mqUndoStack.h"
#include "mqMeshToolsCore.h"

vtkStandardNewMacro(vtkLMActorCollection);


//----------------------------------------------------------------------------
vtkLMActorCollection::vtkLMActorCollection()
{
	
}

//----------------------------------------------------------------------------
vtkLMActorCollection::~vtkLMActorCollection()
{
	

}
void vtkLMActorCollection::AddItem(vtkActor *a)
{
	//We only want vtkLMActors in this collection!
	std::string str1("vtkLMActor");
	if (str1.compare(a->GetClassName()) == 0)
	{
		this->Superclass::AddItem(a);
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(a);
		//this->Renderer->AddActor(a);
		//this->CurrentRenderer->AddActor(myLM);
		this->Renderer->AddActor(myActor->GetLMLabelActor2D());

	}
}

int vtkLMActorCollection::GetNextLandmarkNumber()
{
	//we assume here that landmarks are well ordered in the collection... 
	//if we have 1 2 3 4 6 7 8, we should return 5
	// if we have nothing, we should return 1
	// if we have 1 2 3 4, we should return 5
	int next = 1;
	int previous = 1;
	this->InitTraversal();
	if (this->GetNumberOfItems() == 0) { return 1; }
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->GetNextActor());
		next = myActor->GetLMNumber();
		if (next - previous > 1) { next = previous + 1; return next; }
		else
		{
			previous = next;			
		}
		
	}
	next++;
	return next;
}

void vtkLMActorCollection::DeleteSelectedActors()
{
	int anychange = 0;
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->GetNextActor());
		if (myActor->GetSelected() == 1) { anychange = 1; }
	}
	if (anychange == 1)
	{
		vtkSmartPointer<vtkActorCollection> undocoll = vtkSmartPointer<vtkActorCollection>::New();
		std::string action = "Delete selected landmarks";
		int mCount = BEGIN_UNDO_SET(action);

		int done = 0;
		while (!done)
		{

			if (this->GetNumberOfItems() == 0) { done = 1; }
			else
			{
				this->InitTraversal();
				int found = 0;
				for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
				{

					vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->GetNextActor());
					if (!found && myActor->GetSelected() == 1) {
						undocoll->AddItem(myActor);
						this->RemoveItem(myActor);
						this->Renderer->RemoveActor(myActor);
						found = 1;
					}

				}
				if (found == 0) { done = 1; }
			}
		}
		this->UndoRedo->UndoStack.push_back(vtkMTCollectionUndoRedo::Element(undocoll, mCount));
		END_UNDO_SET();
		this->Changed = 1;
	}



} //delete all selected actors

int vtkLMActorCollection::ActorChanged()
{
	int anychange = 0;
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->GetNextActor());
		if (myActor->GetChanged() == 1) { anychange = 1; }
	}
	return anychange;
}

void vtkLMActorCollection::ComputeBoundingBoxLength()
{
	double largestbounds[6];
	largestbounds[0] = DBL_MAX;
	largestbounds[1] = -DBL_MAX;
	largestbounds[2] = DBL_MAX;
	largestbounds[3] = -DBL_MAX;
	largestbounds[4] = DBL_MAX;
	largestbounds[5] = -DBL_MAX;
	double largestboundsselected[6];
	largestboundsselected[0] = DBL_MAX;
	largestboundsselected[1] = -DBL_MAX;
	largestboundsselected[2] = DBL_MAX;
	largestboundsselected[3] = -DBL_MAX;
	largestboundsselected[4] = DBL_MAX;
	largestboundsselected[5] = -DBL_MAX;

	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkLMActor* actor = vtkLMActor::SafeDownCast(this->GetNextActor());
		if (i == 0) {
			actor->GetBounds(largestbounds);
			if (actor->GetSelected() == 1)
			{
				actor->GetBounds(largestboundsselected);
			}
		}
		else
		{
			double bounds[6];

			actor->GetBounds(bounds);
			if (bounds[0] < largestbounds[0]) { largestbounds[0] = bounds[0]; }
			if (bounds[1] > largestbounds[1]) { largestbounds[1] = bounds[1]; }
			if (bounds[2] < largestbounds[2]) { largestbounds[2] = bounds[2]; }
			if (bounds[3] > largestbounds[3]) { largestbounds[3] = bounds[3]; }
			if (bounds[4] < largestbounds[4]) { largestbounds[4] = bounds[4]; }
			if (bounds[5] > largestbounds[5]) { largestbounds[5] = bounds[5]; }
			if (actor->GetSelected() == 1)
			{

				if (bounds[0] < largestboundsselected[0]) { largestboundsselected[0] = bounds[0]; }
				if (bounds[1] > largestboundsselected[1]) { largestboundsselected[1] = bounds[1]; }
				if (bounds[2] < largestboundsselected[2]) { largestboundsselected[2] = bounds[2]; }
				if (bounds[3] > largestboundsselected[3]) { largestboundsselected[3] = bounds[3]; }
				if (bounds[4] < largestboundsselected[4]) { largestboundsselected[4] = bounds[4]; }
				if (bounds[5] > largestboundsselected[5]) { largestboundsselected[5] = bounds[5]; }
			}

		}

	}
	double A[3];//min
	double B[3];//max

	A[0] = largestbounds[0];
	A[1] = largestbounds[2];
	A[2] = largestbounds[4];
	B[0] = largestbounds[1];
	B[1] = largestbounds[3];
	B[2] = largestbounds[5];
	this->SetBoundingBox(largestbounds);
	//cout << "A:" << A[0] << "," << A[1] << "," << A[2] << endl;
	//cout << "B:" << B[0] << "," << B[1] << "," << B[2] << endl;
	double diag[3];
	diag[0] = B[0] - A[0];
	diag[1] = B[1] - A[1];
	diag[2] = B[2] - A[2];
	double lengthxyz = sqrt((diag[0])*(diag[0]) + (diag[1])*(diag[1]) + (diag[2])*(diag[2]));

	this->SetBoundingBoxLength(lengthxyz);
	//cout << "Bounding box lenght:" << lengthxyz << endl;
	A[0] = largestboundsselected[0];
	A[1] = largestboundsselected[2];
	A[2] = largestboundsselected[4];
	B[0] = largestboundsselected[1];
	B[1] = largestboundsselected[3];
	B[2] = largestboundsselected[5];
	this->SetBoundingBoxSelected(largestboundsselected);
	//cout << "A:" << A[0] << "," << A[1] << "," << A[2] << endl;
	//cout << "B:" << B[0] << "," << B[1] << "," << B[2] << endl;	
	diag[0] = B[0] - A[0];
	diag[1] = B[1] - A[1];
	diag[2] = B[2] - A[2];
	lengthxyz = sqrt((diag[0])*(diag[0]) + (diag[1])*(diag[1]) + (diag[2])*(diag[2]));
	//cout << "Bounding box lenght selected:" << lengthxyz << endl;
	this->SetBoundingBoxLengthOfSelectedActors(lengthxyz);
}
void vtkLMActorCollection::ComputeCenterOfMass()
{
	this->centerOfMass[0] = 0;
	this->centerOfMass[1] = 0;
	this->centerOfMass[2] = 0;
	this->centerOfMassOfSelectedActors[0] = 0;
	this->centerOfMassOfSelectedActors[1] = 0;
	this->centerOfMassOfSelectedActors[2] = 0;


	vtkIdType nLM = this->GetNumberOfItems();
	vtkIdType nSelectedLM = 0;
	vtkSmartPointer<vtkCenterOfMass> centerOfMassFilter =
		vtkSmartPointer<vtkCenterOfMass>::New();
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->GetNextActor());


		double acenter[3];
		myActor->GetLMOrigin(acenter);
		
		
		centerOfMass[0] += acenter[0] ;
		centerOfMass[1] += acenter[1] ;
		centerOfMass[2] += acenter[2] ;
		if (myActor->GetSelected() == 1)
		{
		
			centerOfMassOfSelectedActors[0] += acenter[0] ;
			centerOfMassOfSelectedActors[1] += acenter[1] ;
			centerOfMassOfSelectedActors[2] += acenter[2] ;
			nSelectedLM++;
		}			
	}	
	if (nLM > 0)
	{
		centerOfMass[0] /= nLM;
		centerOfMass[1] /= nLM;
		centerOfMass[2] /= nLM;

	}
	if (nSelectedLM > 0)
	{
		centerOfMassOfSelectedActors[0] /= nSelectedLM;
		centerOfMassOfSelectedActors[1] /= nSelectedLM;
		centerOfMassOfSelectedActors[2] /= nSelectedLM;
	}
	this->SetGlobalSelectedVN(nSelectedLM);
	this->SetGlobalVN(nLM);
}


//----------------------------------------------------------------------------
void vtkLMActorCollection::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	//os << indent << "Selected: "<< this->Selected;
	

}
void vtkLMActorCollection::ApplyChanges()
{
	cout << "LMthis?" << endl;
	this->ComputeCenterOfMass();
	cout << "LMthat?" << endl;
	this->ComputeBoundingBoxLength();
	cout << "LMor?" << endl;

	// Reset state "changed" to 0 for this and all actors.
	this->Changed = 0;
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		cout << "LMthut?" << endl;
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->GetNextActor());
		myActor->SetChanged(0);
	}
}