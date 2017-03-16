/****************************************************************************
 
 **
 ****************************************************************************/

/**
  
*/

#include "vtkBezierCurveSource.h"
#include "vtkLMActor.h"

#include <vtkObjectFactory.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkImageData.h>
#include <vtkExecutive.h>
#include <vtkPolyDataMapper.h>
#include <vtkOutputWindow.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

#include <cmath>

#define NORMAL_LMK 0
#define TARGET_LMK 1
#define NODE_LMK 2
#define HANDLE_LMK 3
#define FLAG_LMK 4

#define NORMAL_NODE 0
#define STARTING_NODE 1
#define MILESTONE_NODE 2
#define CONNECT_NODE 3


#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

vtkStandardNewMacro(vtkBezierCurveSource);

void vtkBezierCurveSource::SetNodes(vtkSmartPointer<vtkLMActorCollection> Nodes)
{
	this->Nodes = Nodes;


}
void vtkBezierCurveSource::SetHandles(vtkSmartPointer<vtkLMActorCollection> Handles)
{
	this->Handles = Handles;
}

vtkBezierCurveSource::vtkBezierCurveSource()
{
	this->Nodes = NULL;
	this->Handles = NULL;




	this->SetNumberOfInputPorts(0);
	this->SetNumberOfOutputPorts(1);
	
	/*
	A second output could be the selected curve segments.
	this->SetNumberOfOutputPorts(2);
	vtkPolyData *output2 = vtkPolyData::New();
	this->GetExecutive()->SetOutputData(1, output2);
	output2->Delete();*/
}




vtkBezierCurveSource::~vtkBezierCurveSource()
{
 
}

void vtkBezierCurveSource::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkPolyDataAlgorithm::PrintSelf(os, indent);

 
  int index = 0;
 
}
/*
Could be transformed into "GetBezierSelectedCurveOutput".
vtkPolyData* vtkBezierCurveSource::GetControlPointsOutput()
{
  if (this->GetNumberOfOutputPorts() < 2)
    {
    return NULL;
    }
  return vtkPolyData::SafeDownCast( this->GetExecutive()->GetOutputData(1) );
}
*/
vtkPolyData* vtkBezierCurveSource::GetBezierCurveOutput()
{
  return vtkPolyData::SafeDownCast( this->GetExecutive()->GetOutputData(0) );
}




int vtkBezierCurveSource::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector),
  vtkInformationVector *outputVector)
{

  /*
  
  Could be used to compute the selected segments...
  vtkInformation *cpOutInfo = outputVector->GetInformationObject(1);
  if(cpOutInfo)
    {
    vtkPolyData *cpOutput = vtkPolyData::SafeDownCast(cpOutInfo->Get(vtkDataObject::DATA_OBJECT()));
    this->UpdateControlPointsPolyData(cpOutput);
    }
	*/

  vtkInformation *bsOutInfo = outputVector->GetInformationObject(0);
  if(bsOutInfo)
    {
    vtkPolyData *bsOutput = vtkPolyData::SafeDownCast(bsOutInfo->Get(vtkDataObject::DATA_OBJECT()));
    this->UpdateBezierCurvePolyData(bsOutput);
    }

  return 1;
}

/*void vtkBezierCurveSource::UpdateControlPointsPolyData(vtkPolyData* pd)
{
  if(!pd)
    {
    return;
    }

  // Create points array (geometry)
  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();

  int nrPoints = this->NumberOfControlPoints[0]*this->NumberOfControlPoints[1];
  points->SetNumberOfPoints(nrPoints);

  for(int i=0; i<nrPoints; i++)
    {
    double pt[4];
    pt[0] = this->ControlPoints[i*3];
    pt[1] = this->ControlPoints[i*3+1];
    pt[2] = this->ControlPoints[i*3+2];
    pt[3] = 1.0;

    this->Transform->MultiplyPoint(pt,pt);
    points->SetPoint(i, pt[0], pt[1], pt[2]);
    }
	
  pd->SetPoints(points);

  // Create quads-segment array (topology)
  /*int grid_x = this->NumberOfControlPoints[0];
  int grid_y = this->NumberOfControlPoints[1];

  vtkSmartPointer<vtkCellArray> cells =
    vtkSmartPointer<vtkCellArray>::New();

  for(int i=0; i<grid_x-1; i++)
    {
    for(int j=0; j<grid_y-1; j++)
      {
      int base = j*grid_x + i;
      int a = base;
      int b = base+1;
      int c = base+grid_x+1;
      int d = base+grid_x;

      vtkIdType ls[2];

      ls[0] = a; ls[1] = b;
      cells->InsertNextCell(2, ls);

      ls[0] = b; ls[1] = c;
      cells->InsertNextCell(2, ls);

      ls[0] = c; ls[1] = d;
      cells->InsertNextCell(2, ls);

      ls[0] = d; ls[1] = a;
      cells->InsertNextCell(2, ls);
      }
    }
  pd->SetLines(cells);

}
*/



void vtkBezierCurveSource::UpdateBezierCurvePolyData(vtkPolyData* pd)
{
  if(!pd)
    {
    return;
    }

  // First a rigid line between node landmarks.
  if (this->Nodes == NULL)
  {
	  return;
  }
  vtkSmartPointer<vtkPoints> points =
	  vtkSmartPointer<vtkPoints>::New();
  
  int nrPoints = this->Nodes->GetNumberOfItems();
 
  points->SetNumberOfPoints(nrPoints);
  this->Nodes->InitTraversal();
  for (int i = 0; i<nrPoints; i++)
  {
	  vtkLMActor *MyActor = vtkLMActor::SafeDownCast(this->Nodes->GetNextActor());
	  
	  double coord[3];
	  MyActor->GetLMOrigin(coord);
	  double pt[4];

	  pt[0] = coord[0];
	  pt[1] = coord[1];
	  pt[2] = coord[2];
	  pt[3] = 1.0;
	  cout << "Point " << i << ": " << pt[0] << ", "<<pt[1] << ", " << pt[2] << endl;
	  points->SetPoint(i, pt[0], pt[1], pt[2]);
  }

  pd->SetPoints(points);

  vtkSmartPointer<vtkCellArray> cells =
	  vtkSmartPointer<vtkCellArray>::New();

  for (int i = 0; i < nrPoints - 1; i++)

  {
	  vtkIdType ls[2];

	  ls[0] = i; ls[1] = i+1;
	  cells->InsertNextCell(2, ls);

	 
  }

pd->SetLines(cells);
  
  // Create quads-segment array (topology)
  /*int grid_x = this->NumberOfControlPoints[0];
  int grid_y = this->NumberOfControlPoints[1];

  

  for(int i=0; i<grid_x-1; i++)
  {
  for(int j=0; j<grid_y-1; j++)
  {
  int base = j*grid_x + i;
  int a = base;
  int b = base+1;
  int c = base+grid_x+1;
  int d = base+grid_x;

  vtkIdType ls[2];

  ls[0] = a; ls[1] = b;
  cells->InsertNextCell(2, ls);

  ls[0] = b; ls[1] = c;
  cells->InsertNextCell(2, ls);

  ls[0] = c; ls[1] = d;
  cells->InsertNextCell(2, ls);

  ls[0] = d; ls[1] = a;
  cells->InsertNextCell(2, ls);
  }
  }
  pd->SetLines(cells);

  */
}



