/*=========================================================================

Program:   MeshTools
Module:    vtkMTActorCollection.cxx
=========================================================================*/
#include "vtkMTActorCollection.h"
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

vtkStandardNewMacro(vtkMTActorCollection);


//----------------------------------------------------------------------------
vtkMTActorCollection::vtkMTActorCollection()
{
	//this->Selected = 1;
	this->centerOfMass[0] = 0;
	this->centerOfMass[1] = 0;
	this->centerOfMass[2] = 0;
	this->centerOfMassOfSelectedActors[0] = 0;
	this->centerOfMassOfSelectedActors[1] = 0;
	this->centerOfMassOfSelectedActors[2] = 0;
	this->BoundingBoxLength = 0;
	this->BoundingBoxLengthOfSelectedActors = 0;

	this->Changed = 0;
}

//----------------------------------------------------------------------------
vtkMTActorCollection::~vtkMTActorCollection()
{
	

}

void vtkMTActorCollection::ApplyProperties(vtkProperty *p)
{
	this->Superclass::ApplyProperties(p);
	
}

int vtkMTActorCollection::ActorChanged()
{
	int anychange = 0;
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{		
		vtkMTActor *myActor= vtkMTActor::SafeDownCast(this->GetNextActor());
		if (myActor->GetChanged() == 1) { anychange = 1; }
	}
	return anychange;
}
void vtkMTActorCollection::ComputeCenterOfMass()
{
	this->centerOfMass[0] = 0;
	this->centerOfMass[1] = 0;
	this->centerOfMass[2] = 0;
	this->centerOfMassOfSelectedActors[0] = 0;
	this->centerOfMassOfSelectedActors[1] = 0;
	this->centerOfMassOfSelectedActors[2] = 0;


	vtkIdType globalvn = 0;
	vtkIdType globalSelectedvn = 0;
	vtkSmartPointer<vtkCenterOfMass> centerOfMassFilter =
		vtkSmartPointer<vtkCenterOfMass>::New();
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->GetNextActor());


		centerOfMassFilter->SetInputData(myActor->GetMapper()->GetInput());
		centerOfMassFilter->SetUseScalarsAsWeights(false);
		double acenter[3];
		vtkTransform *Transform = vtkTransform::New();
		centerOfMassFilter->Update();
		centerOfMassFilter->GetCenter(acenter);
		if (myActor->GetMatrix() != NULL)
		{
			Transform->SetMatrix(myActor->GetMatrix());
			
	
			vtkSmartPointer<vtkPoints>  data = vtkSmartPointer<vtkPoints> ::New();
		   data->SetDataTypeToDouble();
		   data->SetNumberOfPoints(1);
			data->SetPoint(0, acenter);			
			vtkSmartPointer<vtkPolyData> pdata = vtkSmartPointer<vtkPolyData>::New();
			pdata->SetPoints(data);		

			vtkSmartPointer<vtkTransformFilter> transformFilter =
				vtkSmartPointer<vtkTransformFilter>::New();
			transformFilter->SetInputData(pdata);
			transformFilter->SetTransform(Transform);
			transformFilter->Update();
			vtkSmartPointer<vtkPoints> Tdata = vtkSmartPointer<vtkPoints>::New();
			Tdata = transformFilter->GetOutput()->GetPoints();
	
			//get transformed center!
			double *tcenter = Tdata->GetPoint(0);
			//cout << "acenter:" << acenter[0] << "," << acenter[1] << "," << acenter[2] << endl;
			//cout << "tcenter:" << tcenter[0] << "," << tcenter[1] << "," << tcenter[2] << endl;
			acenter[0] = tcenter[0];
			acenter[1] = tcenter[1];
			acenter[2] = tcenter[2];

		}
		else
		{
			cout << "My Actor Matrix is null!" << endl;
		}
		vtkIdType vertnum = myActor->GetMapper()->GetInput()->GetNumberOfPoints();
		centerOfMass[0] += acenter[0] * vertnum;
		centerOfMass[1] += acenter[1] * vertnum;
		centerOfMass[2] += acenter[2] * vertnum;
		if (myActor->GetSelected() == 1)
		{
		
			centerOfMassOfSelectedActors[0] += acenter[0] * vertnum;
			centerOfMassOfSelectedActors[1] += acenter[1] * vertnum;
			centerOfMassOfSelectedActors[2] += acenter[2] * vertnum;
			globalSelectedvn += vertnum;
		}

		globalvn += vertnum;
		//cout << "MTACOLL: Total VN = " << globalvn << endl;
		//cout << "MTACOLL: acenter[] = " << acenter[0] <<"," << acenter[1] << "," << acenter[2] << endl;
	}
	//cout << "MTACOLL: Result: Total VN = " << globalvn << endl;
	if (globalvn > 0)
	{
		centerOfMass[0] /= globalvn;
		centerOfMass[1] /= globalvn;
		centerOfMass[2] /= globalvn;

	}
	if (globalSelectedvn > 0)
	{
		centerOfMassOfSelectedActors[0] /= globalSelectedvn;
		centerOfMassOfSelectedActors[1] /= globalSelectedvn;
		centerOfMassOfSelectedActors[2] /= globalSelectedvn;

	}

}
void vtkMTActorCollection::ComputeBoundingBoxLength()
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
		vtkMTActor* actor = vtkMTActor::SafeDownCast(this->GetNextActor());
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
	//cout << "A:" << A[0] << "," << A[1] << "," << A[2] << endl;
	//cout << "B:" << B[0] << "," << B[1] << "," << B[2] << endl;	
	diag[0] = B[0] - A[0];
	diag[1] = B[1] - A[1];
	diag[2] = B[2] - A[2];
	lengthxyz = sqrt((diag[0])*(diag[0]) + (diag[1])*(diag[1]) + (diag[2])*(diag[2]));
	//cout << "Bounding box lenght selected:" << lengthxyz << endl;
	this->SetBoundingBoxLengthOfSelectedActors(lengthxyz);
}

void vtkMTActorCollection::ApplyChanges()
{
	this->ComputeCenterOfMass();
	this->ComputeBoundingBoxLength();
	

	// Reset state "changed" to 0 for this and all actors.
	this->Changed = 0;
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->GetNextActor());
		myActor->SetChanged(0);
	}
}


void vtkMTActorCollection::GetCenterOfMass(double center[3])
{
	if (this->Changed == 1 || this->ActorChanged() == 1) { ApplyChanges(); }
	double *com = this->GetCenterOfMass();

	center[0] = com[0];
	center[1] = com[1];
	center[2] = com[2];
	

}
double vtkMTActorCollection::GetBoundingBoxLength()
{
	if (this->Changed == 1 || this->ActorChanged() == 1) { ApplyChanges(); }
	return this->BoundingBoxLength;


}

double vtkMTActorCollection::GetBoundingBoxLengthOfSelectedActors()
{
	if (this->Changed == 1 || this->ActorChanged() == 1) { ApplyChanges(); }
	return this->BoundingBoxLengthOfSelectedActors;


}

double *vtkMTActorCollection::GetCenterOfMass()
{
	if (this->Changed == 1 || this->ActorChanged() == 1) { ApplyChanges(); }
	return this->centerOfMass;
}

void vtkMTActorCollection::GetCenterOfMassOfSelectedActors(double center[3])
{
	if (this->Changed == 1 || this->ActorChanged() == 1) { ApplyChanges(); }
	double *com = this->GetCenterOfMassOfSelectedActors();

	center[0] = com[0];
	center[1] = com[1];
	center[2] = com[2];


}



double *vtkMTActorCollection::GetCenterOfMassOfSelectedActors()
{
	if (this->Changed == 1 || this->ActorChanged()==1) { ApplyChanges(); }
	return this->centerOfMassOfSelectedActors;
}

//----------------------------------------------------------------------------
void vtkMTActorCollection::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	//os << indent << "Selected: "<< this->Selected;
	

}
