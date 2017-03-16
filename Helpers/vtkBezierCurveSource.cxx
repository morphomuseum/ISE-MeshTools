/****************************************************************************
 
 **
 ****************************************************************************/

/**
  
*/

#include "vtkBezierCurveSource.h"

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
	this->SetNumberOfOutputPorts(2);



	vtkPolyData *output2 = vtkPolyData::New();
	this->GetExecutive()->SetOutputData(1, output2);
	output2->Delete();
}

void vtkBezierCurveSource::CreateLandmark(double coord[3], int lmk_type, int node_type)
{
	cout << "vtkBezierCurveSource CreateLandmark:" << lmk_type << ", " << node_type << endl;
	// lmk_type : 2 for curve nodes
	// lmk_type : 3 for curve handles
	
	//node_type: only used if mode ==2, curve node
	//node_type: 1 curve starting point 
	//node_type: 0 normal node
	//node_type: 2 curve milestone
	//node_type: 3 connect to preceding starting point

	VTK_CREATE(vtkLMActor, myLM);
	int num = 0;
	myLM->SetLMType(lmk_type);

	if (lmk_type == NODE_LMK)
	{
		num = this->Nodes->GetNextLandmarkNumber();
	}
	else if (lmk_type == HANDLE_LMK)
	{
		num = this->Handles->GetNextLandmarkNumber();
	}
	double ori[3] = { coord[0], coord[1], coord[2] + 1 };

	myLM->SetLMOriginAndOrientation(coord, ori);
	myLM->SetLMSize(0.01);		
	
	if (lmk_type == NODE_LMK)
	{
		// to do : 
		myLM->SetLMType(NODE_LMK);
		if (node_type>-1)
		{
			//lmtype: 1 curve starting point
			//lmtype: 0 normal node
			//lmtype: 2 curve milestone
			//lmtype: 3 connect to preceding starting point
			if (node_type == NORMAL_NODE) { myLM->SetLMType(NODE_LMK);  myLM->SetLMNodeType(NORMAL_NODE); }
			if (node_type == STARTING_NODE) { myLM->SetLMType(NODE_LMK);  myLM->SetLMNodeType(STARTING_NODE); }
			if (node_type == MILESTONE_NODE) { myLM->SetLMType(NODE_LMK);  myLM->SetLMNodeType(MILESTONE_NODE); }
			if (node_type == CONNECT_NODE) { myLM->SetLMType(NODE_LMK);  myLM->SetLMNodeType(CONNECT_NODE); }
		}
		else
		{
			if (num > 1)
			{
				vtkLMActor *myPrecedingLM = NULL;
				//@implement GETLMBefore(num)
				//vtkLMActor *myPrecedingLM = this->NodeLandmarkCollection->GetLMBefore(num);
				if (myPrecedingLM != NULL)
				{
					if (myPrecedingLM->GetLMNodeType() == CONNECT_NODE)// if curve ending point
					{
						myLM->SetLMNodeType(STARTING_NODE); // curve starting point
					}
					else
					{
						myLM->SetLMNodeType(NORMAL_NODE); // curve conventional node
					}
				}
				else
				{
					myLM->SetLMNodeType(NORMAL_NODE); // curve conventional node
				}
			}
			else // num ==1
			{
				myLM->SetLMNodeType(STARTING_NODE); //curve starting point
			}
		}
	}
	else if (lmk_type == HANDLE_LMK)
	{
		myLM->SetLMType(HANDLE_LMK); //curve handle
	}
	

	myLM->SetLMNumber(num);

	myLM->SetLMBodyType(0);

	myLM->SetSelected(0);

	/*vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(myLM->getLMBody());
	mapper->Update();
	myLM->SetMapper(mapper);*/


	//myLM->PrintSelf(cout, vtkIndent(1));


	if (lmk_type == NODE_LMK)
	{
		this->Nodes->AddItem(myLM);
		
		cout << "Node added" << endl;
	}
	else if (lmk_type == HANDLE_LMK)
	{
		this->Handles->AddItem(myLM);
		
		cout << "Handle added" << endl;
		
		
	}
	


}

vtkBezierCurveSource::vtkBezierCurveSource(vtkSmartPointer<vtkLMActorCollection> Nodes, vtkSmartPointer<vtkLMActorCollection> Handles)
{
  this->Nodes = Nodes;
  this->Handles = Handles;
  
  this->SetNumberOfInputPorts(0);
  this->SetNumberOfOutputPorts(2);



  vtkPolyData *output2 = vtkPolyData::New();
  this->GetExecutive()->SetOutputData(1, output2);
  output2->Delete();
}

vtkBezierCurveSource::~vtkBezierCurveSource()
{
 
}

void vtkBezierCurveSource::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkPolyDataAlgorithm::PrintSelf(os, indent);

 
  int index = 0;
 
}

vtkPolyData* vtkBezierCurveSource::GetControlPointsOutput()
{
  if (this->GetNumberOfOutputPorts() < 2)
    {
    return NULL;
    }
  return vtkPolyData::SafeDownCast( this->GetExecutive()->GetOutputData(1) );
}

vtkPolyData* vtkBezierCurveSource::GetBezierCurveOutput()
{
  return vtkPolyData::SafeDownCast( this->GetExecutive()->GetOutputData(0) );
}






int vtkBezierCurveSource::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector),
  vtkInformationVector *outputVector)
{
  vtkInformation *cpOutInfo = outputVector->GetInformationObject(1);
  if(cpOutInfo)
    {
    vtkPolyData *cpOutput = vtkPolyData::SafeDownCast(cpOutInfo->Get(vtkDataObject::DATA_OBJECT()));
    this->UpdateControlPointsPolyData(cpOutput);
    }

  vtkInformation *bsOutInfo = outputVector->GetInformationObject(0);
  if(bsOutInfo)
    {
    vtkPolyData *bsOutput = vtkPolyData::SafeDownCast(bsOutInfo->Get(vtkDataObject::DATA_OBJECT()));
    this->UpdateBezierCurvePolyData(bsOutput);
    }

  return 1;
}

void vtkBezierCurveSource::UpdateControlPointsPolyData(vtkPolyData* pd)
{
  if(!pd)
    {
    return;
    }

  // Create points array (geometry)
  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();

  /*int nrPoints = this->NumberOfControlPoints[0]*this->NumberOfControlPoints[1];
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
	*/
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
  pd->SetLines(cells);*/

}


double BezierBlend(int k, double mu, int n);
void EvalBezierCurve(const double* controlPoints, int m, int n, int dimx, int dimy, vtkPoints* CurvePoints);

void vtkBezierCurveSource::UpdateBezierCurvePolyData(vtkPolyData* pd)
{
  if(!pd)
    {
    return;
    }

  // First construct a grid.
  // Construct the basic grid
  

  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkDoubleArray> tcoords =
    vtkSmartPointer<vtkDoubleArray>::New();

  //points->SetNumberOfPoints(grid_x*grid_y);
 // tcoords->SetNumberOfComponents(2);
  //tcoords->SetNumberOfTuples(grid_x*grid_y);

  /*for(int i=0; i<grid_x; i++)
    {
    for(int j=0; j<grid_y; j++)
      {
      // Points need not be computed, because the EvalBezierCurve()
      // method does it for us.

      double s = double(i)/double(grid_x);
      double t = double(j)/double(grid_y);
      tcoords->SetTuple2(j*grid_x+i, s, t);
      }
    }
	*/
  // Now evaluate the bezier curve on the grid.
  /*int m = this->NumberOfControlPoints[0];
  int n = this->NumberOfControlPoints[1];
  int dimx = this->Dimensions[0];
  int dimy = this->Dimensions[1];
  EvalBezierCurve(this->ControlPoints, m, n, dimx, dimy, points);

  for(int i=0; i<points->GetNumberOfPoints(); i++)
    {
    double *point = points->GetPoint(i);
    double pt[4];
    pt[0] = point[0];
    pt[1] = point[1];
    pt[2] = point[2];
    pt[3] = 1.0;

    this->Transform->MultiplyPoint(pt,pt);
    points->SetPoint(i, pt[0],pt[1],pt[2]);
    }
	*/


  // Set the points into the output polydata.
  /*
  pd->SetPoints(points);

  pd->GetPointData()->SetTCoords(tcoords);

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

      vtkIdType tgl[3];
      tgl[2] = a; tgl[1] = b; tgl[0] = c;
      cells->InsertNextCell(3, tgl);
      tgl[2] = a; tgl[1] = c; tgl[0] = d;
      cells->InsertNextCell(3, tgl);
      }
    }
  pd->SetStrips(cells);
  */
}


// Evaluates a bezier Curve.
void EvalBezierCurve(const double* controlPoints, int m, int n, int dimx, int dimy, vtkPoints* CurvePoints)
{
  int i, j, ki, kj;
  double mui, muj, bi, bj;

  int ptIndex = 0;
  int ctrlPtIndex = 0;
  double pt[3];

  for(i=0; i<dimx; i++)
    {
    mui = i / (double)(dimx-1);
    for(j=0; j<dimy; j++)
      {
      muj = j / (double)(dimy-1);

      // Get the Curve point and initialize it.
      CurvePoints->GetPoint(ptIndex, pt);
      pt[0] = pt[1] = pt[2] = 0;

      // Loop through all control points and compute its effect on the
      // Curve point
      ctrlPtIndex = 0;
      for (ki=0; ki<m; ki++)
        {
        bi = BezierBlend(ki,mui,m-1);

        for (kj=0; kj<n; kj++)
          {
          bj = BezierBlend(kj,muj,n-1);

          const double* ctrlPt = controlPoints + (ctrlPtIndex*3);
          ++ctrlPtIndex;

          pt[0] += (ctrlPt[0] * bi * bj);
          pt[1] += (ctrlPt[1] * bi * bj);
          pt[2] += (ctrlPt[2] * bi * bj);
          }
        }

      CurvePoints->SetPoint(ptIndex, pt);
      ++ptIndex;
      }
    }
}
