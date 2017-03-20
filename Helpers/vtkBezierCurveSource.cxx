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
	this->SetNumberOfOutputPorts(3);
	
	/*
	A second output could be the selected curve segments.
	this->SetNumberOfOutputPorts(2);
	vtkPolyData *output2 = vtkPolyData::New();
	this->GetExecutive()->SetOutputData(1, output2);
	output2->Delete();*/
	
	vtkPolyData *output2 = vtkPolyData::New();
	this->GetExecutive()->SetOutputData(1, output2);
	output2->Delete();
	vtkPolyData *output3 = vtkPolyData::New();
	this->GetExecutive()->SetOutputData(2, output3);
	output3->Delete();

}




vtkBezierCurveSource::~vtkBezierCurveSource()
{
 
}

void vtkBezierCurveSource::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkPolyDataAlgorithm::PrintSelf(os, indent);

 
  int index = 0;
 
}


vtkPolyData* vtkBezierCurveSource::GetNodeHandleConnexionOutput()
{
  if (this->GetNumberOfOutputPorts() < 3)
    {
    return NULL;
    }
  return vtkPolyData::SafeDownCast( this->GetExecutive()->GetOutputData(2) );
}


vtkPolyData* vtkBezierCurveSource::GetSelectedBezierCurveOutput()
{
	if (this->GetNumberOfOutputPorts() < 2)
	{
		return NULL;
	}
	return vtkPolyData::SafeDownCast(this->GetExecutive()->GetOutputData(1));
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


	//nhc= node handles connexion for selected bezier curve...
	vtkInformation *nhcOutInfo = outputVector->GetInformationObject(2);
	if (nhcOutInfo)
	{
		vtkPolyData *nhcOutput = vtkPolyData::SafeDownCast(nhcOutInfo->Get(vtkDataObject::DATA_OBJECT()));
		this->UpdadeNodeHandleConnexionPolyData(nhcOutput);
	}
	else
	{
		cout << "no information object 2!" << endl;
	}


  //sbc for selected bezier curve...
  vtkInformation *sbcOutInfo = outputVector->GetInformationObject(1);
  if(sbcOutInfo)
    {
    vtkPolyData *sbcOutput = vtkPolyData::SafeDownCast(sbcOutInfo->Get(vtkDataObject::DATA_OBJECT()));
    this->UpdateSelectedBezierCurvePolyData(sbcOutput);
    }
	

  vtkInformation *bcOutInfo = outputVector->GetInformationObject(0);
  if(bcOutInfo)
    {
    vtkPolyData *bcOutput = vtkPolyData::SafeDownCast(bcOutInfo->Get(vtkDataObject::DATA_OBJECT()));
    this->UpdateBezierCurvePolyData(bcOutput);
    }

  return 1;
}


void vtkBezierCurveSource::UpdadeNodeHandleConnexionPolyData(vtkPolyData* pd)
{
	if (!pd)
	{
		return;
	}

	cout << "update Node Handle Connection!" << endl;
	// First a rigid line between node landmarks.
	if (this->Nodes == NULL || this->Handles ==NULL)
	{
		return;
	}
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	
	int nrN = this->Nodes->GetNumberOfItems();
	int nrH = this->Handles->GetNumberOfItems();

	cout << "nr of nodes:" << nrN << endl;
	cout << "nr of Handles:" << nrH << endl;	
	
	
	int nrMin = std::min(nrH, nrN);
	cout << "nr Min:" << nrH << endl;
	points->SetNumberOfPoints(2*nrMin);
	this->Nodes->InitTraversal();
	this->Handles->InitTraversal();
	if (nrMin > 0)
	{
		for (int i = 0; i < nrMin; i++)
		{
			vtkLMActor *MyNode = vtkLMActor::SafeDownCast(this->Nodes->GetNextActor());
			vtkMatrix4x4 *Mat = MyNode->GetMatrix();
			double lmorigin[3] = { Mat->GetElement(0, 3), Mat->GetElement(1, 3), Mat->GetElement(2, 3) };
			double pt[4];
			pt[0] = lmorigin[0];
			pt[1] = lmorigin[1];
			pt[2] = lmorigin[2];
			pt[3] = 1.0;

			points->SetPoint(2 * i, pt[0], pt[1], pt[2]);
			vtkLMActor *MyHandle = vtkLMActor::SafeDownCast(this->Handles->GetNextActor());
			Mat = MyHandle->GetMatrix();
			lmorigin[0] = Mat->GetElement(0, 3);
			lmorigin[1] = Mat->GetElement(1, 3);
			lmorigin[2] = Mat->GetElement(2, 3);
			double pt2[4];
			pt2[0] = lmorigin[0];
			pt2[1] = lmorigin[1];
			pt2[2] = lmorigin[2];
			pt2[3] = 1.0;
			points->SetPoint(2 * i + 1, pt2[0], pt2[1], pt2[2]);

		}


	}
	pd->SetPoints(points);

	vtkSmartPointer<vtkCellArray> cells =
		vtkSmartPointer<vtkCellArray>::New();

	if (nrMin > 0)
	{

		for (int i = 0; i < nrMin; i++)

		{

			vtkIdType ls[2];
			

			
				ls[0] = 2*i; ls[1] = 2*i + 1;
				cells->InsertNextCell(2, ls);					

		}
	}
	pd->SetLines(cells);

	cout << "End update handle node connexion!" << endl;

	
}

void vtkBezierCurveSource::UpdateSelectedBezierCurvePolyData(vtkPolyData* pd)
{
	if (!pd)
	{
		return;
	}

	cout << "update bezier curve poly data!" << endl;
	// First a rigid line between node landmarks.
	if (this->Nodes == NULL)
	{
		return;
	}
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();

	int nrSelectedPoints = this->Nodes->GetNumberOfSelectedActors();
	cout << "nr of selected nodes:" << nrSelectedPoints << endl;
	points->SetNumberOfPoints(nrSelectedPoints);
	this->Nodes->InitTraversal();
	for (int i = 0; i<nrSelectedPoints; i++)
	{
		vtkLMActor *MyActor = this->Nodes->GetNextSelectedActor();

		// do something
		vtkMatrix4x4 *Mat = MyActor->GetMatrix();

		/*cout << "Initial matrix:" << endl;
		Mat->PrintSelf(cout, vtkIndent(1));*/

		// we get LM coordinates through the Matrix instead of "LMOrigin" because when displacing landmarks, 
		// LMOrigin is only updated when the landmark movement ends.
		double lmorigin[3] = { Mat->GetElement(0, 3), Mat->GetElement(1, 3), Mat->GetElement(2, 3) };
		//double lmorientation[3] = { 0,0,1 };



		double coord[3];
		MyActor->GetLMOrigin(coord);
		double pt[4];

		/*pt[0] = coord[0];
		pt[1] = coord[1];
		pt[2] = coord[2];*/
		pt[0] = lmorigin[0];
		pt[1] = lmorigin[1];
		pt[2] = lmorigin[2];
		pt[3] = 1.0;
		//cout << "Point " << i << ": " << pt[0] << ", "<<pt[1] << ", " << pt[2] << endl;
		points->SetPoint(i, pt[0], pt[1], pt[2]);
	}

	pd->SetPoints(points);

	vtkSmartPointer<vtkCellArray> cells =
		vtkSmartPointer<vtkCellArray>::New();

	this->Nodes->InitTraversal();
	vtkLMActor *MyPrecedingActor = vtkLMActor::SafeDownCast(this->Nodes->GetNextActor());
	vtkLMActor *MyNextActor = NULL;
	if (nrSelectedPoints > 1)
	{

		for (int i = 0; i < nrSelectedPoints - 1; i++)

		{

			vtkIdType ls[2];
			if (MyNextActor == NULL)
			{
				MyNextActor = this->Nodes->GetNextSelectedActor();
			}

			if (MyNextActor->GetLMNodeType() != STARTING_NODE)
			{
				ls[0] = i; ls[1] = i + 1;
				cells->InsertNextCell(2, ls);
			}

			MyPrecedingActor = MyNextActor;
			MyNextActor = this->Nodes->GetNextSelectedActor();

		}
	}
	pd->SetLines(cells);

	cout << "End update bezier curve poly data!" << endl;

	
}

void vtkBezierCurveSource::UpdateBezierCurvePolyData(vtkPolyData* pd)
{
  if(!pd)
    {
    return;
    }

  cout << "update bezier curve poly data!" << endl;
  // First a rigid line between node landmarks.
  if (this->Nodes == NULL)
  {
	  return;
  }
 


  vtkLMActor  * ob_H1, *ob_H2, *ob_N1, *ob_N2, *ob_Nstart, *ob_Hstart, *ob_HA, *ob_HB, *ob_NA, *ob_NB;
  ob_H1 = NULL;
  ob_N1 = NULL;
  ob_H2 = NULL;
  ob_N2 = NULL;
  ob_Hstart = NULL;
  ob_Nstart = NULL;
  ob_NA = NULL;
  ob_NB = NULL;
  ob_HA = NULL;
  ob_HB = NULL;


  int k, m, num_seg, ind, ind2, indh, indh2;
  int nint = 98; // will iterate nint , to draw nint+1 lines, that is nint+2 points
  double t;

  int num_landmark_H = this->Handles->GetNumberOfItems(); 
  int num_landmark_N = this->Nodes->GetNumberOfItems();
  int nbp = num_landmark_H;
  double vv[3], dd1[3], dd2[3], hh1[3], hh2[3], intvv[3], intvv2[3];

  int print = 0;
  num_seg = 0;
  //if (num_landmark_T == num_landmark_D && num_landmark_T >0)
  if (num_landmark_N >1 && num_landmark_N != num_landmark_H)
  {	// in this case, just draw segments
	  num_seg++;
	  ob_N1 = this->Nodes->GetLandmarkAfter(0);
	  ob_N2 = this->Nodes->GetLandmarkAfter(1);
	  k = 0;
	  vtkSmartPointer<vtkPoints> points =
		  vtkSmartPointer<vtkPoints>::New();

	  int nrPoints = nrPoints = num_landmark_N;
	  cout << "nr of nodes:" << nrPoints << endl;
	  points->SetNumberOfPoints(nrPoints);
	  this->Nodes->InitTraversal();
	  
	  for (int i = 0; i<nrPoints; i++)
	  {
		  vtkLMActor *MyActor = vtkLMActor::SafeDownCast(this->Nodes->GetNextActor());
		  vtkMatrix4x4 *Mat = MyActor->GetMatrix();
		  double lmorigin[3] = { Mat->GetElement(0, 3), Mat->GetElement(1, 3), Mat->GetElement(2, 3) };
		  double coord[3];
		  double pt[4];
		  pt[0] = lmorigin[0];
		  pt[1] = lmorigin[1];
		  pt[2] = lmorigin[2];
		  pt[3] = 1.0;
		  points->SetPoint(i, pt[0], pt[1], pt[2]);
	  }
	  pd->SetPoints(points);
	  vtkSmartPointer<vtkCellArray> cells =
		  vtkSmartPointer<vtkCellArray>::New();
	  int i= 0;
	  while (ob_N1 != NULL && ob_N2 != NULL)
	  {

		  if (ob_N1->GetLMNodeType() == STARTING_NODE) { num_seg++; }
		  //int selected = this->Is_Segment_Selected(num_seg);
		  //std::cout<<"Num seg ="<<num_seg<<", selected="<<selected<<std::endl;
		  if (ob_N2->GetLMNodeType()==NORMAL_NODE || ob_N2->GetLMNodeType() == MILESTONE_NODE)
		  {
			  vtkIdType ls[2];
			  
				  ls[0] = i; ls[1] = i + 1;
				  cells->InsertNextCell(2, ls);
				  
			  
			
		  }
		  /*if (ob_N2 != NULL && ob_N2->GetLMNodeType() == STARTING_NODE == 1)
		  {
			  num_seg++;
		  }*/

		  ind = ob_N1->GetLMNumber();
		  ob_N1 = this->Nodes->GetLandmarkAfter(ind);
		  if (ob_N1 != NULL)
		  {
			  ind2 = ob_N1->GetLMNumber();
			  ob_N2 = this->Nodes->GetLandmarkAfter(ind2);
		  }
		  
	  }

	  pd->SetLines(cells);
  }
  

  /*else if (num_landmark_D >1 && num_landmark_D == num_landmark_H)
  {
	  num_seg++;
	  ob_H1 = landmarkafter(0, 1);
	  ob_Hstart = landmarkafter(0, 1);
	  ob_H2 = landmarkafter(1, 1);
	  ob_D1 = landmarkafter(0, 0);
	  ob_Dstart = landmarkafter(0, 0);
	  ob_D2 = landmarkafter(1, 0);
	  k = 0;
	  while (ob_D1 != NULL)
	  {

		  //stop drawing if the second point is a "start" of a new curve

		  //Get world coordinates of landmark number ind on the target;
		  if (ob_D1->Is_Curve_Start() == 2) { num_seg++; }
		  int selected = this->Is_Segment_Selected(num_seg);

		  //Get world coordinates of landmark number ind on the target;
		  if (ob_D1->Is_Curve_Start() == 3)
		  {
			  ob_DA = ob_D1;
			  ob_HA = ob_H1;
			  ob_DB = ob_Dstart;
			  ob_HB = ob_Hstart;

		  }
		  else
		  {
			  ob_DA = ob_D1;
			  ob_HA = ob_H1;
			  if (ob_D2 != NULL)
			  {
				  ob_DB = ob_D2;
				  ob_HB = ob_H2;
			  }
			  else
			  {
				  ob_DB = ob_D1;
				  ob_HB = ob_H1;
			  }
		  }

		  ob_DA->get_world_coordinates_matrix2(wc_mat);
		  vv[0] = 0;
		  vv[1] = 0;
		  vv[2] = 0;
		  ApplyTransformation(vv, dd1, wc_mat);


		  ob_DB->get_world_coordinates_matrix2(wc_mat);
		  vv[0] = 0;
		  vv[1] = 0;
		  vv[2] = 0;
		  ApplyTransformation(vv, dd2, wc_mat);

		  ob_HA->get_world_coordinates_matrix2(wc_mat);
		  vv[0] = 0;
		  vv[1] = 0;
		  vv[2] = 0;
		  ApplyTransformation(vv, hh1, wc_mat);

		  ob_HB->get_world_coordinates_matrix2(wc_mat);
		  vv[0] = 0;
		  vv[1] = 0;
		  vv[2] = 0;
		  ApplyTransformation(vv, hh2, wc_mat);

		  glLineWidth(1);
		  glColor3f(blue[0], blue[1], blue[2]);
		  glBegin(GL_LINES);
		  glVertex3d(dd1[0], dd1[1], dd1[2]);
		  glVertex3d(hh1[0], hh1[1], hh1[2]);
		  glEnd();
		  //trick : second handle is mirrored relative to the second point!
		  hh2[0] = dd2[0] - (hh2[0] - dd2[0]);
		  hh2[1] = dd2[1] - (hh2[1] - dd2[1]);
		  hh2[2] = dd2[2] - (hh2[2] - dd2[2]);

		  // At this stage : we have all the input we need!
		  // Just draw the Bezier curve between dd1 and dd2

		  int compute = 1;
		  if (ob_D2 == NULL && ob_D1->Is_Curve_Start() == 0) { compute = 0; }
		  if (ob_D2 == NULL && ob_D1->Is_Curve_Start() != 3) { compute = 0; }
		  if (ob_D2 != NULL && ob_D2->Is_Curve_Start() == 1 && ob_D1->Is_Curve_Start() != 3) { compute = 0; }


		  if (compute == 1)
		  {
			  if (selected == 0)
			  {
				  glLineWidth(2);
				  glColor3f(green5[0], green5[1], green5[2]);
			  }
			  else
			  {
				  glLineWidth(4);
				  glColor3f(red[0], red[1], red[2]);
			  }
			  for (m = 0; m <= nint; m++)
			  {
				  // t is [0.. 1]
				  t = (((float)m)) / (((float)nint + 1));
				  glBegin(GL_LINES);
				  intvv[0] = (1 - t)*(1 - t)*(1 - t)*dd1[0] + 3 * (1 - t)*(1 - t)*t*hh1[0] + 3 * (1 - t)*t*t*hh2[0] + t*t*t*dd2[0];
				  intvv[1] = (1 - t)*(1 - t)*(1 - t)*dd1[1] + 3 * (1 - t)*(1 - t)*t*hh1[1] + 3 * (1 - t)*t*t*hh2[1] + t*t*t*dd2[1];
				  intvv[2] = (1 - t)*(1 - t)*(1 - t)*dd1[2] + 3 * (1 - t)*(1 - t)*t*hh1[2] + 3 * (1 - t)*t*t*hh2[2] + t*t*t*dd2[2];
				  glVertex3d(intvv[0], intvv[1], intvv[2]);


				  t = (((float)m + 1)) / (((float)nint + 1));
				  intvv2[0] = (1 - t)*(1 - t)*(1 - t)*dd1[0] + 3 * (1 - t)*(1 - t)*t*hh1[0] + 3 * (1 - t)*t*t*hh2[0] + t*t*t*dd2[0];
				  intvv2[1] = (1 - t)*(1 - t)*(1 - t)*dd1[1] + 3 * (1 - t)*(1 - t)*t*hh1[1] + 3 * (1 - t)*t*t*hh2[1] + t*t*t*dd2[1];
				  intvv2[2] = (1 - t)*(1 - t)*(1 - t)*dd1[2] + 3 * (1 - t)*(1 - t)*t*hh1[2] + 3 * (1 - t)*t*t*hh2[2] + t*t*t*dd2[2];

				  glVertex3d(intvv2[0], intvv2[1], intvv2[2]);
				  glEnd();
			  }

		  }


		  if (ob_D2 != NULL && ob_D2->Is_Curve_Start() == 1)
		  {
			  num_seg++;
		  }

		  ind = ob_D1->landmark_index;
		  indh = ob_H1->landmark_index;

		  ob_H1 = landmarkafter(indh, 1);
		  ob_D1 = landmarkafter(ind, 0);
		  if (ob_D1 != NULL)
		  {
			  ind2 = ob_D1->landmark_index;
			  ob_D2 = landmarkafter(ind2, 0);
		  }
		  if (ob_H1 != NULL)
		  {
			  indh2 = ob_H1->landmark_index;
			  ob_H2 = landmarkafter(indh2, 1);
		  }
		  if (ob_D1 != NULL&& ob_D1->Is_Curve_Start() == 1) {
			  ob_Hstart = ob_H1;
			  ob_Dstart = ob_D1;

		  }
		  k++;
	  }
  }
  */




  /*for (int i = 0; i<nrPoints; i++)
  {
	  vtkLMActor *MyActor = vtkLMActor::SafeDownCast(this->Nodes->GetNextActor());
	  
	  // do something
	  vtkMatrix4x4 *Mat = MyActor->GetMatrix();

	  // we get LM coordinates through the Matrix instead of "LMOrigin" because when displacing landmarks, 
	  // LMOrigin is only updated when the landmark movement ends.
	  double lmorigin[3] = { Mat->GetElement(0, 3), Mat->GetElement(1, 3), Mat->GetElement(2, 3) };
	  //double lmorientation[3] = { 0,0,1 };
	 


	  double coord[3];
	  MyActor->GetLMOrigin(coord);
	  double pt[4];

	 
	  pt[0] = lmorigin[0];
	  pt[1] = lmorigin[1];
	  pt[2] = lmorigin[2];
	  pt[3] = 1.0;
	  //cout << "Point " << i << ": " << pt[0] << ", "<<pt[1] << ", " << pt[2] << endl;
	  points->SetPoint(i, pt[0], pt[1], pt[2]);
  }

  pd->SetPoints(points);

  vtkSmartPointer<vtkCellArray> cells =
	  vtkSmartPointer<vtkCellArray>::New();

 this->Nodes->InitTraversal();
  vtkLMActor *MyPrecedingActor = vtkLMActor::SafeDownCast(this->Nodes->GetNextActor());
  vtkLMActor *MyNextActor = NULL;
  if (nrPoints > 1)
  {
	  for (int i = 0; i < nrPoints - 1; i++)
	  {
		  vtkIdType ls[2];
		  if (MyNextActor == NULL)
		  {
			  MyNextActor = vtkLMActor::SafeDownCast(this->Nodes->GetNextActor());
		  }
		  if (MyNextActor->GetLMNodeType() != STARTING_NODE)
		  {
			  ls[0] = i; ls[1] = i + 1;
			  cells->InsertNextCell(2, ls);
		  }
		  MyPrecedingActor = MyNextActor;
		  MyNextActor = vtkLMActor::SafeDownCast(this->Nodes->GetNextActor());
	  }
  }

cout << "End update bezier curve poly data!" << endl; 
*/



}
/*
int CONTAINER_MESH::Get_Curve_Segment_Number()
{

int num_seg, ind;
OBJECT_LANDMARK *ob_D1;
ob_D1=NULL;

int num_landmark_D = Get_Landmark_Number(0);	//number of anchor landmarks

int print = 0;
if (print == 1)
{
std::cout<<std::endl<<"Start count segments curve";
}
num_seg=0;
//if (num_landmark_T == num_landmark_D && num_landmark_T >0)
if (num_landmark_D >1)
{

ob_D1 = landmarkafter (0, 0);


while ( ob_D1 != NULL)
{
if (ob_D1->Is_Curve_Start() ==1 || ob_D1->Is_Curve_Start() ==2 )
{
num_seg++;
}
ind = ob_D1->landmark_index;
ob_D1 = landmarkafter (ind, 0);

}
}
return num_seg;
}
float CONTAINER_MESH::Get_Curve_Segment_Length(int seg)
{

float length=0;
OBJECT_LANDMARK *ob_H1, *ob_H2, *ob_D1,*ob_D2, *ob_Dstart, *ob_Hstart, *ob_HA,*ob_HB, *ob_DA, *ob_DB;
ob_H1=NULL;
ob_D1=NULL;
ob_H2=NULL;
ob_D2=NULL;
ob_Hstart=NULL;
ob_Dstart=NULL;
ob_DA=NULL;
ob_DB=NULL;
ob_HA=NULL;
ob_HB=NULL;

int k,m,num_seg, ind, ind2, indh, indh2;
int nint=1002; // will iterate nint , to draw nint+1 lines, that is nint+2 points
float t;

int num_landmark_H = Get_Landmark_Number (1); //target
int num_landmark_D = Get_Landmark_Number (0);	//def
int nbp = num_landmark_H;
glMatrix wc_mat;
float vv[3], dd1[3],dd2[3], hh1[3], hh2[3], intvv[3],intvv2[3];

int print = 0;

num_seg=0;
//if (num_landmark_T == num_landmark_D && num_landmark_T >0)
if (num_landmark_D >1 && num_landmark_D == num_landmark_H)
{
num_seg++;


ob_H1 = landmarkafter (0, 1);
ob_Hstart = landmarkafter (0, 1);
ob_H2 = landmarkafter (1, 1);
ob_D1 = landmarkafter (0, 0);
ob_Dstart = landmarkafter (0, 0);
ob_D2 = landmarkafter (1, 0);
k= 0;
while ( ob_D1 != NULL )
{
//stop drawing if the second point is a "start" of a new curve
if (ob_D1->Is_Curve_Start() ==2){num_seg++;}
//Get world coordinates of landmark number ind on the target;
if (ob_D1->Is_Curve_Start() ==3)
{
ob_DA=ob_D1;
ob_HA=ob_H1;
ob_DB=ob_Dstart;
ob_HB=ob_Hstart;

}
else
{
ob_DA=ob_D1;
ob_HA=ob_H1;
if (ob_D2 !=NULL)
{
ob_DB=ob_D2;
ob_HB=ob_H2;
}
else
{
ob_DB=ob_D1;
ob_HB=ob_H1;
}

}

ob_DA->get_world_coordinates_matrix2(wc_mat);
vv[0] =0;
vv[1] =0;
vv[2] =0;
ApplyTransformation(vv, dd1, wc_mat);


ob_DB->get_world_coordinates_matrix2(wc_mat);
vv[0] =0;
vv[1] =0;
vv[2] =0;
ApplyTransformation(vv, dd2, wc_mat);

ob_HA->get_world_coordinates_matrix2(wc_mat);
vv[0] =0;
vv[1] =0;
vv[2] =0;
ApplyTransformation(vv, hh1, wc_mat);

ob_HB->get_world_coordinates_matrix2(wc_mat);
vv[0] =0;
vv[1] =0;
vv[2] =0;
ApplyTransformation(vv, hh2, wc_mat);

//trick : second handle is mirrored relative to the second point!
hh2[0]= dd2[0]-(hh2[0]-dd2[0]);
hh2[1]= dd2[1]-(hh2[1]-dd2[1]);
hh2[2]= dd2[2]-(hh2[2]-dd2[2]);


// At this stage : we have all the input we need!
// Just draw the Bezier curve between dd1 and dd2

//we draw a negment in two cases
// 1st point is "3" then we have to and next point is "
//compute something in all cases except when
//- D2 is 1 (clean start) and D1 is not a 3 (close curve)
// D2 is null and D1 is not a 3.
int compute = 1;
if (ob_D2 ==NULL && ob_D1->Is_Curve_Start()!=3){compute =0;}
if (ob_D2 !=NULL && ob_D2->Is_Curve_Start()==1&& ob_D1->Is_Curve_Start()!=3){compute =0;}


if (compute==1)
{


for (m=0; m<=nint; m++)
{

// t is [0.. 1]
t = (((float)m))/(((float)nint+1));
intvv[0]=  (1-t)*(1-t)*(1-t)*dd1[0] +3*(1-t)*(1-t)*t*hh1[0]+3*(1-t)*t*t*hh2[0]+t*t*t*dd2[0];
intvv[1]=  (1-t)*(1-t)*(1-t)*dd1[1] +3*(1-t)*(1-t)*t*hh1[1]+3*(1-t)*t*t*hh2[1]+t*t*t*dd2[1];
intvv[2]=  (1-t)*(1-t)*(1-t)*dd1[2] +3*(1-t)*(1-t)*t*hh1[2]+3*(1-t)*t*t*hh2[2]+t*t*t*dd2[2];


t = (((float)m+1))/(((float)nint+1));
intvv2[0]=  (1-t)*(1-t)*(1-t)*dd1[0] +3*(1-t)*(1-t)*t*hh1[0]+3*(1-t)*t*t*hh2[0]+t*t*t*dd2[0];
intvv2[1]=  (1-t)*(1-t)*(1-t)*dd1[1] +3*(1-t)*(1-t)*t*hh1[1]+3*(1-t)*t*t*hh2[1]+t*t*t*dd2[1];
intvv2[2]=  (1-t)*(1-t)*(1-t)*dd1[2] +3*(1-t)*(1-t)*t*hh1[2]+3*(1-t)*t*t*hh2[2]+t*t*t*dd2[2];
if (seg == num_seg)
{
length += sqrt ((intvv[0]-intvv2[0])*(intvv[0]-intvv2[0])+
(intvv[1]-intvv2[1])*(intvv[1]-intvv2[1])+
(intvv[2]-intvv2[2])*(intvv[2]-intvv2[2])
);
}


}//end for


}// end if start ==0


if (ob_D2!=NULL && ob_D2->Is_Curve_Start() ==1 )
{
num_seg++;
}


ind = ob_D1->landmark_index;
indh = ob_H1->landmark_index;

ob_H1 = landmarkafter (indh, 1);
ob_D1 = landmarkafter (ind, 0);
if (ob_D1 !=NULL)
{
ind2 = ob_D1->landmark_index;
ob_D2 = landmarkafter (ind2, 0);
}
if (ob_H1 !=NULL)
{
indh2 = ob_H1->landmark_index;
ob_H2 = landmarkafter (indh2, 1);
}
if (ob_D1!=NULL&& ob_D1->Is_Curve_Start() ==1 ){
ob_Hstart = ob_H1;
ob_Dstart = ob_D1;

}

k++;
}
}
return length;
}
int CONTAINER_MESH::Is_Segment_Selected(int seg)
{
// Ici, on cherche à savoir si il y a au moins un landmark/poignée
// sélectionnée pour le segment donné.

OBJECT_LANDMARK *ob_H1, *ob_H2, *ob_D1,*ob_D2;
ob_H1=NULL;
ob_D1=NULL;
ob_H2=NULL;
ob_D2=NULL;


int selected =0;
int k,m,num_seg, ind, ind2, indh, indh2;


int num_landmark_H = Get_Landmark_Number (1); //target
int num_landmark_D = Get_Landmark_Number (0);	//def
int nbp = num_landmark_H;
glMatrix wc_mat;
float vv[3], dd1[3],dd2[3], hh1[3], hh2[3], intvv[3],intvv2[3];

int print = 0;
num_seg=0;
//if (num_landmark_T == num_landmark_D && num_landmark_T >0)
if (num_landmark_D >1 && num_landmark_D != num_landmark_H)
{	// in this case, just draw segments
num_seg++;
ob_D1 = landmarkafter (0, 0);
ob_D2 = landmarkafter (1, 0);

while ( ob_D1 != NULL )
{

//stop drawing if the second point is a "start" of a new curve
if (ob_D1 != NULL && ob_D1->selected ==1){selected =1;}

if (ob_D1->Is_Curve_Start() ==2){
if (seg == num_seg){return selected;}
num_seg++;
selected=0;

}

if (ob_D2!=NULL && ob_D2->Is_Curve_Start() ==1 )
{
if (seg == num_seg){return selected;}
num_seg++;
selected=0;
}
ind = ob_D1->landmark_index;

ob_D1 = landmarkafter (ind, 0);
if (ob_D1 !=NULL)
{
ind2 = ob_D1->landmark_index;
ob_D2 = landmarkafter (ind2, 0);
}

}
if (num_seg == seg)
{
return selected;
}
else
{return 0;}


}
else if (num_landmark_D >1 && num_landmark_D == num_landmark_H)
{
num_seg++;
ob_H1 = landmarkafter (0, 1);
ob_H2 = landmarkafter (1, 1);
ob_D1 = landmarkafter (0, 0);
ob_D2 = landmarkafter (1, 0);
k= 0;

while ( ob_D1 != NULL )
{

//stop drawing if the second point is a "start" of a new curve
if (
ob_D1 != NULL&& ob_H1 != NULL &&(
ob_H1->selected ==1 ||ob_D1->selected ==1)
){selected =1;}

if (ob_D1->Is_Curve_Start() ==2){
if (seg == num_seg){return selected;}
num_seg++;
selected=0;

}

if (ob_D2!=NULL && ob_D2->Is_Curve_Start() ==1 )
{
if (seg == num_seg){return selected;}
num_seg++;
selected=0;
}
ind = ob_D1->landmark_index;
indh = ob_H1->landmark_index;

ob_H1 = landmarkafter (indh, 1);
ob_D1 = landmarkafter (ind, 0);
if (ob_D1 !=NULL)
{
ind2 = ob_D1->landmark_index;
ob_D2 = landmarkafter (ind2, 0);
}
if (ob_H1 !=NULL)
{
indh2 = ob_H1->landmark_index;
ob_H2 = landmarkafter (indh2, 1);
}

}
}

if (num_seg == seg)
{
return selected;
}
else
{return 0;}
}
void CONTAINER_MESH::Mesh_Draw_Curve()
{

glLineWidth(2);
glDisable(GL_LIGHTING);
GLfloat green5[] = {0., .5, 0., 1.};
GLfloat blue[] = {0.2, .2, .8, 1.};
GLfloat red[] = {1.0, .2, .2, 1.};

glColor3f(green5[0], green5[1], green5[2]);
OBJECT_LANDMARK *ob_H1, *ob_H2, *ob_D1,*ob_D2, *ob_Dstart, *ob_Hstart, *ob_HA,*ob_HB, *ob_DA, *ob_DB;
ob_H1=NULL;
ob_D1=NULL;
ob_H2=NULL;
ob_D2=NULL;
ob_Hstart=NULL;
ob_Dstart=NULL;
ob_DA=NULL;
ob_DB=NULL;
ob_HA=NULL;
ob_HB=NULL;


int k,m,num_seg, ind, ind2, indh, indh2;
int nint=98; // will iterate nint , to draw nint+1 lines, that is nint+2 points
float t;

int num_landmark_H = Get_Landmark_Number (1); //target
int num_landmark_D = Get_Landmark_Number (0);	//def
int nbp = num_landmark_H;
glMatrix wc_mat;
float vv[3], dd1[3],dd2[3], hh1[3], hh2[3], intvv[3],intvv2[3];

int print = 0;
num_seg=0;
//if (num_landmark_T == num_landmark_D && num_landmark_T >0)
if (num_landmark_D >1 && num_landmark_D != num_landmark_H)
{	// in this case, just draw segments
num_seg++;
ob_D1 = landmarkafter (0, 0);
ob_D2 = landmarkafter (1, 0);
k= 0;



while ( ob_D1 != NULL && ob_D2 != NULL)
{
if (ob_D1->Is_Curve_Start() ==2){num_seg++;}
int selected = this->Is_Segment_Selected(num_seg);
//std::cout<<"Num seg ="<<num_seg<<", selected="<<selected<<std::endl;
if (ob_D2->Is_Curve_Start() ==0 || ob_D2->Is_Curve_Start() ==2)
{
//Get world coordinates of landmark number ind on the target;
ob_D1->get_world_coordinates_matrix2(wc_mat);
vv[0] =0;
vv[1] =0;
vv[2] =0;
ApplyTransformation(vv, dd1, wc_mat);

ob_D2->get_world_coordinates_matrix2(wc_mat);
vv[0] =0;
vv[1] =0;
vv[2] =0;
ApplyTransformation(vv, dd2, wc_mat);
if (selected ==0)
{
glLineWidth(2);
glColor3f(green5[0], green5[1], green5[2]);
}
else
{
glLineWidth(4);
glColor3f(red[0], red[1], red[2]);
}
glBegin (GL_LINES);
glVertex3d(dd1[0],dd1[1],dd1[2]);
glVertex3d(dd2[0],dd2[1],dd2[2]);
glEnd();
}
if (ob_D2!=NULL && ob_D2->Is_Curve_Start() ==1 )
{
num_seg++;
}
ind = ob_D1->landmark_index;
ob_D1 = landmarkafter (ind, 0);
if (ob_D1 !=NULL)
{
ind2 = ob_D1->landmark_index;
ob_D2 = landmarkafter (ind2, 0);
}
k++;
}


}
else if (num_landmark_D >1 && num_landmark_D == num_landmark_H)
{
num_seg++;
ob_H1 = landmarkafter (0, 1);
ob_Hstart = landmarkafter (0, 1);
ob_H2 = landmarkafter (1, 1);
ob_D1 = landmarkafter (0, 0);
ob_Dstart = landmarkafter (0, 0);
ob_D2 = landmarkafter (1, 0);
k= 0;
while ( ob_D1 != NULL )
{

//stop drawing if the second point is a "start" of a new curve

//Get world coordinates of landmark number ind on the target;
if (ob_D1->Is_Curve_Start() ==2){num_seg++;}
int selected = this->Is_Segment_Selected(num_seg);

//Get world coordinates of landmark number ind on the target;
if (ob_D1->Is_Curve_Start() ==3)
{
ob_DA=ob_D1;
ob_HA=ob_H1;
ob_DB=ob_Dstart;
ob_HB=ob_Hstart;

}
else
{
ob_DA=ob_D1;
ob_HA=ob_H1;
if (ob_D2 !=NULL)
{
ob_DB=ob_D2;
ob_HB=ob_H2;
}
else
{
ob_DB=ob_D1;
ob_HB=ob_H1;
}
}

ob_DA->get_world_coordinates_matrix2(wc_mat);
vv[0] =0;
vv[1] =0;
vv[2] =0;
ApplyTransformation(vv, dd1, wc_mat);


ob_DB->get_world_coordinates_matrix2(wc_mat);
vv[0] =0;
vv[1] =0;
vv[2] =0;
ApplyTransformation(vv, dd2, wc_mat);

ob_HA->get_world_coordinates_matrix2(wc_mat);
vv[0] =0;
vv[1] =0;
vv[2] =0;
ApplyTransformation(vv, hh1, wc_mat);

ob_HB->get_world_coordinates_matrix2(wc_mat);
vv[0] =0;
vv[1] =0;
vv[2] =0;
ApplyTransformation(vv, hh2, wc_mat);

glLineWidth(1);
glColor3f(blue[0], blue[1], blue[2]);
glBegin (GL_LINES);
glVertex3d(dd1[0],dd1[1],dd1[2]);
glVertex3d(hh1[0],hh1[1],hh1[2]);
glEnd();
//trick : second handle is mirrored relative to the second point!
hh2[0]= dd2[0]-(hh2[0]-dd2[0]);
hh2[1]= dd2[1]-(hh2[1]-dd2[1]);
hh2[2]= dd2[2]-(hh2[2]-dd2[2]);

// At this stage : we have all the input we need!
// Just draw the Bezier curve between dd1 and dd2

int compute = 1;
if (ob_D2 ==NULL && ob_D1->Is_Curve_Start()==0){compute =0;}
if (ob_D2 ==NULL && ob_D1->Is_Curve_Start()!=3){compute =0;}
if (ob_D2 !=NULL && ob_D2->Is_Curve_Start()==1&& ob_D1->Is_Curve_Start()!=3){compute =0;}


if (compute==1)
{
if (selected ==0)
{
glLineWidth(2);
glColor3f(green5[0], green5[1], green5[2]);
}
else
{
glLineWidth(4);
glColor3f(red[0], red[1], red[2]);
}
for (m=0; m<=nint; m++)
{
// t is [0.. 1]
t = (((float)m))/(((float)nint+1));
glBegin (GL_LINES);
intvv[0]=  (1-t)*(1-t)*(1-t)*dd1[0] +3*(1-t)*(1-t)*t*hh1[0]+3*(1-t)*t*t*hh2[0]+t*t*t*dd2[0];
intvv[1]=  (1-t)*(1-t)*(1-t)*dd1[1] +3*(1-t)*(1-t)*t*hh1[1]+3*(1-t)*t*t*hh2[1]+t*t*t*dd2[1];
intvv[2]=  (1-t)*(1-t)*(1-t)*dd1[2] +3*(1-t)*(1-t)*t*hh1[2]+3*(1-t)*t*t*hh2[2]+t*t*t*dd2[2];
glVertex3d(intvv[0],intvv[1],intvv[2]);


t = (((float)m+1))/(((float)nint+1));
intvv2[0]=  (1-t)*(1-t)*(1-t)*dd1[0] +3*(1-t)*(1-t)*t*hh1[0]+3*(1-t)*t*t*hh2[0]+t*t*t*dd2[0];
intvv2[1]=  (1-t)*(1-t)*(1-t)*dd1[1] +3*(1-t)*(1-t)*t*hh1[1]+3*(1-t)*t*t*hh2[1]+t*t*t*dd2[1];
intvv2[2]=  (1-t)*(1-t)*(1-t)*dd1[2] +3*(1-t)*(1-t)*t*hh1[2]+3*(1-t)*t*t*hh2[2]+t*t*t*dd2[2];

glVertex3d(intvv2[0],intvv2[1],intvv2[2]);
glEnd();
}

}


if (ob_D2!=NULL && ob_D2->Is_Curve_Start() ==1 )
{
num_seg++;
}

ind = ob_D1->landmark_index;
indh = ob_H1->landmark_index;

ob_H1 = landmarkafter (indh, 1);
ob_D1 = landmarkafter (ind, 0);
if (ob_D1 !=NULL)
{
ind2 = ob_D1->landmark_index;
ob_D2 = landmarkafter (ind2, 0);
}
if (ob_H1 !=NULL)
{
indh2 = ob_H1->landmark_index;
ob_H2 = landmarkafter (indh2, 1);
}
if (ob_D1!=NULL&& ob_D1->Is_Curve_Start() ==1 ){
ob_Hstart = ob_H1;
ob_Dstart = ob_D1;

}
k++;
}
}


glEnable(GL_LIGHTING);
}
*/

