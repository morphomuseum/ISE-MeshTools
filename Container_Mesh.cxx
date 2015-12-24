#include "Container_Mesh.h"
//#include <gl/gl.h>
//#include <gl/glu.h>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Double_Window.H>
#include <stdio.h>
#include <stdlib.h>

//#include "matrix.h"
#include <list>

#include <vtkThinPlateSplineTransform.h>


//#include <vtkProcrustesAlignmentFilter.h>
#include <vtkExtractEdges.h>
#include <vtkIdTypeArray.h>
#include <vtkIdList.h>
#include <vtkPolyData.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkFillHolesFilter.h>
#include <vtkDecimatePro.h>
//#include <quadricdecimation
#include <vtkDensifyPolyData.h>
#include <vtkQuadricDecimation.h>
#include <vtkThreshold.h>
#include <vtkCurvatures.h>
#include <vtkSTLWriter.h>
#include <vtkPolyDataWriter.h>
#include "vtkOBJWriter.h"
#include <vtkPLYWriter.h>
#include <vtkMaskFields.h>
 #include <vtkAppendPolyData.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkReflectionFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkGeometryFilter.h>

#include <vtkSmartPointer.h>
#include <vtkCellData.h>
#include <vtkDataSet.h>
#include <vtkPolyData.h>
#include <vtkFloatArray.h>
#include <vtkMath.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkIdList.h>
#include <vtkOctreePointLocator.h>
#include <math.h>
#ifndef _NO_NAMESPACE
//#define	PATH_MAX 200

static float at,bt,ct;
#define PYTHAG(a,b) ((at=fabs(a)) > (bt=fabs(b)) ? \
(ct=bt/at,at*sqrt(1.0+ct*ct)) : (bt ? (ct=at/bt,bt*sqrt(1.0+ct*ct)): 0.0))

//static float maxarg1,maxarg2;
#define MAX(a,b) (maxarg1=(a),maxarg2=(b),(maxarg1) > (maxarg2) ?\
	(maxarg1) : (maxarg2))
#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))

using namespace std;
//using namespace math;
#define STD std
#else
#define STD
#endif




void CONTAINER_MESH::Object_Move_Down()
{	
	if (this->OBJECTS_ROOT !=NULL)
	{
	 this->OBJECTS_ROOT->Object_Move_Down();
	}
}
void CONTAINER_MESH::Object_Move_Up()
{	
	if (this->OBJECTS_ROOT !=NULL)
	{ 
	this->OBJECTS_ROOT->Object_Move_Up();
	}
}
void CONTAINER_MESH::Landmark_Move_Down()
{
	
	if (this->OBJECTS_ROOT !=NULL)
	{
		this->OBJECTS_ROOT->Landmark_Move_Down(0,0);
		this->OBJECTS_ROOT->Landmark_Move_Down(0,1);
	}
}
void CONTAINER_MESH::Landmark_Move_Up()
{
	if (this->OBJECTS_ROOT !=NULL)
	{
		this->OBJECTS_ROOT->Landmark_Move_Up(0,0);
		this->OBJECTS_ROOT->Landmark_Move_Up(0,1);
	}
}





void CONTAINER_MESH::Mesh_container_setlandmarksize(float landmark_size)
{		
			this->OBJECTS_ROOT->SetLandmarkSize(landmark_size);		
}

int CONTAINER_MESH::Get_Landmark_Number(int mymode)
{
	
	int cpt;
	cpt = 0;
	cpt = this->OBJECTS_ROOT->Get_Landmark_Number(mymode);
	
	return cpt;

}

int CONTAINER_MESH::Get_Landmark_Selected(int landmark_mode)
{
	int cpt = 0;
	
	cpt = this->OBJECTS_ROOT->Get_Landmark_Selected(landmark_mode);
	
	return cpt;
	
	return cpt;
}
OBJECT_LANDMARK* CONTAINER_MESH::Get_Selected_Landmark(int landmark_mode)
{
	OBJECT_LANDMARK *p;
	p = NULL;
	int cpt;
	cpt = 0;
	
	cpt = this->OBJECTS_ROOT->Get_Landmark_Selected(landmark_mode);
	if (cpt !=1)
	{return p;}
	else 
	{return this->OBJECTS_ROOT->Get_Selected_Landmark(landmark_mode);}
	
	
}
void CONTAINER_MESH::Hierarchy()
{
	std::cout<<std::endl;
	
	if (this->OBJECTS_ROOT !=NULL)
	{	this->OBJECTS_ROOT->Hierarchy(0);}

}

void CONTAINER_MESH::get_object_names_and_view_values(std::vector<std::string>& object_names, std::vector<int>& object_view_values)
{
	if (this->OBJECTS_ROOT !=NULL)
	{	this->OBJECTS_ROOT->get_object_names_and_view_values(object_names, object_view_values);}
	
}

void CONTAINER_MESH::set_object_view_values( std::vector<int>&object_new_view_values)
{
	if (this->OBJECTS_ROOT !=NULL)
	{	
		std::vector<std::string> object_names;
		std::vector<int> object_view_values;
		this->get_object_names_and_view_values(object_names, object_view_values);
		if (object_names.size() == object_new_view_values.size())
		{			
			OBJECT_MESH *My_Obj;
			My_Obj = this->OBJECTS_ROOT->OBJECTS;			
			int cpt=0;
			while(My_Obj!=NULL)
			{
				if (cpt <object_new_view_values.size())
				{
					//std::cout<<"cpt="<<cpt<<", name="<<object_names[cpt]<<",view="<<object_new_view_values[cpt]<<std::endl;
					My_Obj->view = object_new_view_values[cpt];
					if (object_new_view_values[cpt]==0)
					{My_Obj->selected=0;}
				}				
				My_Obj = My_Obj->nextobj;
				cpt++;
			}	
		}
	}
					
	
	
}

OBJECT_LANDMARK* CONTAINER_MESH::landmarkafter(int afterind, int landmark_mode)
{
	// Returns the landmark object  coming after landmark index "afterind"
	// Returns -1 when "afterind" was the last landmark to save
	// Used while saving landmarks
	int next_index = -1;
	int j=0,i=0;
	OBJECT_LANDMARK * my_Landmarks;
	my_Landmarks = this->OBJECTS_ROOT->LandmarkAfter(afterind, landmark_mode);
	return my_Landmarks;



	
	
}

OBJECT_LANDMARK* CONTAINER_MESH::landmarkbefore(int beforeind, int landmark_mode)
{
	// Returns the landmark object  coming before landmark index "beforeind"
	// Returns -1 when "beofreind" was the last landmark to save
	// Used while saving landmarks
	int next_index = -1;
	int j=0,i=0;
	OBJECT_LANDMARK * my_Landmark;
	my_Landmark = this->OBJECTS_ROOT->LandmarkBefore(beforeind, landmark_mode);
	return my_Landmark;
		
}

	int CONTAINER_MESH::Mesh_container_newlandmarkindex(int landmark_mode)
{

	// Gives the index of the newly created landmark
	// For instance, if 1 2 3 4 7 8 exist, returns 5
	// For instance, if 1 3 4 7 8 exist, returns 2
	// For instance, if 1 2 3 4 5 6 7 8 exist, returns 9
	//int  vertex_qsort_compare (const void* v1, const void* v2);
	
	
	OBJECT_LANDMARK * OneLandmark;
	OBJECT_LANDMARK * NextLandmark;
	OneLandmark = NULL;
	NextLandmark = NULL;
	int next_index = 0;
	int Ok = 0;
	NextLandmark = this->OBJECTS_ROOT->LandmarkAfter(next_index, landmark_mode);
	if (NextLandmark !=NULL)
	{
		Ok = 1	;	
	}

	else 
	{return 1;}
	
	while (Ok)
	{
		if ((NextLandmark->landmark_index - next_index)  !=1)
		{Ok = 0;next_index ++;}
		else
		{
			next_index ++;
			NextLandmark = this->OBJECTS_ROOT->LandmarkAfter(next_index, landmark_mode);
			if (NextLandmark ==NULL)			
			{Ok =0; next_index++;}
			
		}
		

	}

	return next_index;

}

void CONTAINER_MESH::Add_Landmark( OBJECT_LANDMARK * New_Landmark, int landmark_mode)
{
	OBJECT_LANDMARK *Landmark_List;
	
	int nbverts=0;
	float mean[3];mean[0] =0;mean[1] =0;mean[2] =0;
	int nbObj=0;
	int pindex=0;
	
	pindex = Mesh_container_newlandmarkindex(landmark_mode);
	New_Landmark->landmark_index = pindex;
	New_Landmark->flag_label= "Flag ";
	New_Landmark->flag_label.append(itoa( pindex,  10 ));
	if (landmark_mode==0)
	{
		if (this->OBJECTS_ROOT->OBJECTS_LANDMARK ==NULL)
		{	this->OBJECTS_ROOT->OBJECTS_LANDMARK = New_Landmark;
			New_Landmark->nextobj = NULL;
			New_Landmark->prevobj = NULL;
		}
		else
		{
			Landmark_List = this->OBJECTS_ROOT->OBJECTS_LANDMARK;
			int Ok = 1;
			while (Ok)
			{
				if (Landmark_List->nextobj ==NULL)
				{
					Ok = 0;
					New_Landmark->prevobj = Landmark_List;
					Landmark_List->nextobj = New_Landmark;
				}
				else
				{
					Landmark_List = Landmark_List->nextobj;
				}
			
			}
		}
	}
	else if (landmark_mode==1)
	{
		if (this->OBJECTS_ROOT->OBJECTS_LANDMARK_TARGET ==NULL)
		{	this->OBJECTS_ROOT->OBJECTS_LANDMARK_TARGET = New_Landmark;
			New_Landmark->nextobj = NULL;
			New_Landmark->prevobj = NULL;
		}
		else
		{
			Landmark_List = this->OBJECTS_ROOT->OBJECTS_LANDMARK_TARGET;
			int Ok = 1;
			while (Ok)
			{
				if (Landmark_List->nextobj ==NULL)
				{
					Ok = 0;
					New_Landmark->prevobj = Landmark_List;
					Landmark_List->nextobj = New_Landmark;
				}
				else
				{
					Landmark_List = Landmark_List->nextobj;
				}
			
			}
		}
	}
	else // Landmark_tag
	{
		if (this->OBJECTS_ROOT->OBJECTS_LANDMARK_LABELS ==NULL)
		{	this->OBJECTS_ROOT->OBJECTS_LANDMARK_LABELS = New_Landmark;
			New_Landmark->nextobj = NULL;
			New_Landmark->prevobj = NULL;
		}
		else
		{
			Landmark_List = this->OBJECTS_ROOT->OBJECTS_LANDMARK_LABELS;
			int Ok = 1;
			while (Ok)
			{
				if (Landmark_List->nextobj ==NULL)
				{
					Ok = 0;
					New_Landmark->prevobj = Landmark_List;
					Landmark_List->nextobj = New_Landmark;
				}
				else
				{
					Landmark_List = Landmark_List->nextobj;
				}
			
			}
		}
	}
	
	if (this->OBJECTS_ROOT !=NULL)
	{
		/*std::cout << "Call reorder landmarks " 
               << std::endl;*/
		this->OBJECTS_ROOT->Reorder_Landmarks(landmark_mode);
	
	}
	
}
void CONTAINER_MESH::Mesh_Mirror_ThroughY()
{
	

  OBJECT_MESH *My_Obj;
  OBJECT_MESH *My_New_Obj;
	if (this->OBJECTS_ROOT->OBJECTS != NULL)
	{
			
		My_Obj = this->OBJECTS_ROOT->OBJECTS;	
	
		while(My_Obj!=NULL)
		{
			
			if (My_Obj->selected ==1)
			{
				 vtkSmartPointer<vtkReflectionFilter> mfilter =vtkSmartPointer<vtkReflectionFilter>::New();
				 mfilter->CopyInputOff();
	
				mfilter->SetInputData((vtkPolyData*)My_Obj);
				mfilter->SetPlaneToY();
				
				mfilter->Update();       
													
				vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();
				vtkSmartPointer<vtkUnstructuredGrid> grid = vtkSmartPointer<vtkUnstructuredGrid>::New();

				grid= (vtkUnstructuredGrid*)mfilter->GetOutput();
				vtkSmartPointer<vtkGeometryFilter> fgeo = vtkSmartPointer<vtkGeometryFilter>::New();
				
				fgeo->SetInputData(grid);
				fgeo->Update();
				MyObj = fgeo->GetOutput();

				vtkSmartPointer<vtkPolyDataNormals> ObjNormals =  vtkSmartPointer<vtkPolyDataNormals>::New();		
				ObjNormals->SetInputData(MyObj);
				ObjNormals->ComputePointNormalsOn();
				ObjNormals->ComputeCellNormalsOn();
				ObjNormals->ConsistencyOff();
				ObjNormals->Update();	

				vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
				cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());
				cleanPolyDataFilter->PieceInvariantOff();
				cleanPolyDataFilter->ConvertLinesToPointsOff();
				cleanPolyDataFilter->ConvertPolysToLinesOff();
				cleanPolyDataFilter->ConvertStripsToPolysOff();
				cleanPolyDataFilter->PointMergingOn();
				cleanPolyDataFilter->Update();

				MyObj= cleanPolyDataFilter->GetOutput();
				std::cout << "\nVtkReflection new Number of points:"<<MyObj->GetNumberOfPoints()<< std::endl;
				std::cout << "VtkReflection new Number of cells:"<<MyObj->GetNumberOfCells()<< std::endl;										
				std::string newname = My_Obj->name.c_str();
				newname.append("_mir");

				My_New_Obj = this->Mesh_PDcontainerload(MyObj, (char*)newname.c_str());
				
				int numpoints = My_New_Obj->GetNumberOfPoints();
				int numtriangles = My_New_Obj->GetNumberOfCells();				
				
				My_New_Obj->selected = 0;								
				My_New_Obj->color[0]=My_Obj->color[0];
				My_New_Obj->color[1]=My_Obj->color[1];
				My_New_Obj->color[2]=My_Obj->color[2];
				My_New_Obj->color[3]= 1;

				glPushMatrix();
				glLoadMatrixf((GLfloat*) My_Obj->Mat2);
				getmatrix( My_New_Obj->Mat2);
				glPopMatrix();
				glPushMatrix();
				glLoadMatrixf((GLfloat*) My_Obj->Mat1);
				getmatrix( My_New_Obj->Mat1);
				glPopMatrix();

				My_New_Obj->Mat2[3][1]*=-1.0; // mirror : invert Y coordinate
				My_New_Obj->Mat2[0][1]*=-1.0;
				My_New_Obj->Mat2[1][0]*=-1.0;
				My_New_Obj->Mat2[1][2]*=-1.0;
				My_New_Obj->Mat2[2][1]*=-1.0;            			
				
				My_New_Obj->mean[1] *=-1; // center of mass mirrored as well!								

			}//if selected

			My_Obj = My_Obj->nextobj;
		}
	}
				

	

	// Then Mirror the landmarks	  		   		
	this->OBJECTS_ROOT->Mesh_Mirror_ThroughY();

		if (g_auto_delete ==1)
	{
		this->Mesh_Delete(1, -1, 0);
	}

		
}


void CONTAINER_MESH::Mesh_invert(int select_mode)
{

		
	this->OBJECTS_ROOT->Mesh_invert(select_mode);
				

}
void CONTAINER_MESH::Mesh_invert_normals(int select_mode)
{

		
	this->OBJECTS_ROOT->Mesh_invert_normals(select_mode);
				

}
void CONTAINER_MESH::Mesh_Tri_Sort(int side)
{
	
	this->OBJECTS_ROOT->Mesh_Tri_Sort(side);

}
void CONTAINER_MESH::Mesh_containerfree(void)									// Frees The Object (Releasing The Memory)
{		
	free(this->OBJECTS_ROOT->OBJECTS);		
}
void CONTAINER_MESH::Mesh_UnselectAll()
{this->OBJECTS_ROOT->Mesh_Unselect();}
void CONTAINER_MESH::Curve_start(int direction)
{
  this->OBJECTS_ROOT->Curve_start(direction);
}

void CONTAINER_MESH::Mesh_SelectAll()
{this->OBJECTS_ROOT->Mesh_Select();}

void CONTAINER_MESH::Mesh_Draw_Obj(int mode, bool bool_change_pos_obj,bool bool_vbo)
{	
	

			
	this->OBJECTS_ROOT->Mesh_DrawObj(mode, bool_change_pos_obj,bool_vbo);
		
	
}
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
void CONTAINER_MESH::Stick_Selected_Landmarks_On_Surfaces()
{
	glMatrix wc_mat;
	float vv[3], vv1[3], x,y,z, nx,ny,nz;
	vtkIdType ve;
	int ind;
	int print = 0;
	OBJECT_LANDMARK * My_Landmark;
	int num_landmark_H = Get_Landmark_Number (1); //target
	int num_landmark_D = Get_Landmark_Number (0);	//def

	//if (num_landmark_T == num_landmark_D && num_landmark_T >0)
	
	My_Landmark = NULL;
	if (num_landmark_D>0)
	{
		My_Landmark = landmarkafter (0, 0);
	}
	//OBJECT_LANDMARK  = Mesh_container_nextlandmarkafter (1, 0);	
	
	while ( My_Landmark != NULL )
	{		
			//Get world coordinates of landmark number ind on the target;
			My_Landmark->get_world_coordinates_matrix2(wc_mat);		
			vv[0] =0;
			vv[1] =0;
			vv[2] =0;			
			ApplyTransformation(vv, vv1, wc_mat);
			if (My_Landmark->selected ==1)
			{
				GetClosestVertex(vv1,&x,&y,&z, &nx, &ny, &nz, &ve,1);
				My_Landmark->Mat2[3][0]= x;
				My_Landmark->Mat2[3][1]=y; 
				My_Landmark->Mat2[3][2]=z;
				//now change the coordinates of the landmark
				
			}
			ind = My_Landmark->landmark_index;
			My_Landmark = landmarkafter (ind, 0);		
	}
	// Now other landmarks
	My_Landmark = NULL;
	if (num_landmark_H>0)
	{
		My_Landmark = landmarkafter (0, 1);
	}	
	
	while ( My_Landmark != NULL )
	{		
			//Get world coordinates of landmark number ind on the target;
			My_Landmark->get_world_coordinates_matrix2(wc_mat);		
			vv[0] =0;
			vv[1] =0;
			vv[2] =0;			
			ApplyTransformation(vv, vv1, wc_mat);
			if (My_Landmark->selected ==1)
			{
				GetClosestVertex(vv1,&x,&y,&z, &nx, &ny, &nz, &ve, 1);
				My_Landmark->Mat2[3][0]= x;
				My_Landmark->Mat2[3][1]=y; 
				My_Landmark->Mat2[3][2]=z;
				//now change the coordinates of the landmark
			}
			ind = My_Landmark->landmark_index;
			My_Landmark = landmarkafter (ind, 1);		
	}

	
}

OBJECT_LANDMARK * CONTAINER_MESH::CreateLandmark(float x, float y, float z, float nx, float ny, float nz, int landmark_mode)
{
	
	//Returns nearest landmark of this
	// But : orienter la pointe du landmark (dessin\E9e de mani\E8re \E0 ce que la pointe soit \E0 la coordonn\E9e 1 0 0)
	// de mani\E8re \E0 ce qu'elle pointe dans la direction nx, ny, nz
	// On va faire : 
	// 1er) 1 rotation le long de l'axe x de mani\E8re \E0 tourner le landmark le long de cet axe.
	//      1 matrice de rotation le long de l'axe z s'\E9crit de cette mani\E8re :
	//        cosz  -sinz  0
	//        sinz  cosz   0
	//        0     0      1		
	// Je l'appelle MAT_Z (z est un angle qui va de -PI \E0 PI)


	// 2eme) 1 rotation le long de l'axe z de mani\E8re \E0 
	//      1 matrice de rotation le long de l'axe z s'\E9crit :
	//        1    0     0
	//        0    cosx  -sinx
	//        0    sinx  cosx
	// Je l'appelle MAT_X (x est un angle qui va de -PI \E0 PI)


	// Pour appliquer les rotations suivant Z selon un angle z, puis autour de X selon un angle x, 
	// il faudra multiplier les coordonn\E9es par MAT_Z * MAT_X => 
	// Soit par la matrice : 
	//
	//       cosz  -       sinz         0
	//       cosx*sinz  cosx*cosz  -sinx
	//       sinx_sinz  sinx*cosz    cosx
	 
	
	 //C'est cette matrice qui fournira la combinaison des deux rotations.
	// Maintenant, calculons MAT_X et MAT_Z
	// Quelles valeurs ont cosx, sinx, sinz, cosz dans le cas qui m'occupe?

	// Mon but : je veux multiplier les coordonn\E9es de la pointe du landmark initial (0 0 1) de mani\E8re \E0 ce qu'elle
	// atteigne la position (nx, ny, nz)
	// J'ai donc les contraintes suivantes (que je trouve en multipliant MAT_Z*MAT_X par (0,0,1)): 
	// nx = cosz (1)
	// ny = -cosz * sinx (2)
	// nz = sinx * sinz (3)

	// Il est possible de choisir des angles z puis x de mani\E8re \E0 satisfaire ces contraintes. 
	// Une possibilit\E9 est de faire: 
	// I)  la premi\E8re rotation autour de l'axe de mani\E8re \E0 avoir :
	//		(1 0 0) qui devienne (nx, racine(1-nx\B2), 0) 
	//		=> Dans cette premi\E8re rotation, on peut d\E9cider arbitrairement 
	///     que la coordonn\E9e y du point transofm\E9 sera positive
	//		quel que soit nx (>0 ou <0).
	//		cette rotation se fait selon un angle "z"  de rotation autour de l'axe z, qui peut \EAtre positif ou n\E9gatif selon
	//		que nz soit n\E9gatif ou positif, respectivement..
	//		Dans ce cas je veux que :
	//				cosz = nx; (c'est aussi ma contrainte 1) 
	//				sinz = racine(1- cosz\B2)  [d\E9cid\E9 arbitrairement]
	// II) Dans un deuxi\E8me temps j'effectue une rotation autour de l'axe x qui va satisfaire mes contraintes 2 et 3
	//		Dans ce cas, je d\E9duis que :   
	//				cosx= ny/sinz	(d\E9duit de contrainte 2, car je connais d\E9j\E0 sinz)		
    //				sinz= nz/sinz;   ( d\E9duit de la contrainte 3, car je connais d\E9j\E0 sinz)

	// Je suis donc parvenu \E0 calculer une matrice qui satisfait mes contraintes.

	OBJECT_LANDMARK * My_Landmark;		
	//get_screen_projection_matrix(screen_mat);			
 	  
	  My_Landmark = new OBJECT_LANDMARK;
	  glMatrix M2;
	  glPushMatrix();
	  glLoadIdentity();
	  //getMatrix((GLfloat*) M1);
	  getmatrix(M2);
	  glPopMatrix();          
        // translation 
          M2[3][0]=x;M2[3][1]=y;M2[3][2]=z;
         
		  // rotation 
		  // Landmark object oriented along x axis : 0 0 1 
          // assume rot.  
			// 1) around z 
			// 2)    around x 
          // -> values sinz,cosz,sinx,cosx need to be computed
		  
		  float cosz=nx;
          float sinz = sqrt(1.0-(cosz*cosz));
          float cosx,sinx;
          if(sinz==0.0) // just in case we are in the speciale case
		  {
            cosx=1.0;sinx=0.0;	
		  }
          else
		  {
            cosx= ny/sinz;
			
            sinx=nz/sinz;
		  }
         
         M2[0][0]=cosz; //=nx
		 M2[0][1]=cosx*sinz; //=ny
		 M2[0][2]=sinx *sinz;    //=nz
        
		 M2[1][0]=-sinz;
		 M2[1][1]=cosx*cosz;
		 M2[1][2]=sinx*cosz;

         M2[2][0]=0; 
		 M2[2][1]= -sinx; 
		 M2[2][2]=cosx; 

		 My_Landmark->Mat2[0][0]=M2[0][0];
		 My_Landmark->Mat2[0][1]=M2[0][1];
		 My_Landmark->Mat2[0][2]=M2[0][2];
		 My_Landmark->Mat2[0][3]=M2[0][3];
		 My_Landmark->Mat2[1][0]=M2[1][0];
		 My_Landmark->Mat2[1][1]=M2[1][1];
		 My_Landmark->Mat2[1][2]=M2[1][2];
		 My_Landmark->Mat2[1][3]=M2[1][3];
		 My_Landmark->Mat2[2][0]=M2[2][0];
		 My_Landmark->Mat2[2][1]=M2[2][1];
		 My_Landmark->Mat2[2][2]=M2[2][2];
		 My_Landmark->Mat2[2][3]=M2[2][3];
		 My_Landmark->Mat2[3][0]=M2[3][0];
		 My_Landmark->Mat2[3][1]=M2[3][1];
		 My_Landmark->Mat2[3][2]=M2[3][2];
		 My_Landmark->Mat2[3][3]=M2[3][3];
		 if (landmark_mode ==0)
		  {
			My_Landmark->color[0]=1;
			My_Landmark->color[1]=0;
			My_Landmark->color[2]=0;
			My_Landmark->color[3]=1;
			My_Landmark->is_flag=0;					  
		  }
		  else if (landmark_mode ==1)
		  {
			My_Landmark->color[0]=1.0;
			My_Landmark->color[1]=0.7;
			My_Landmark->color[2]=0.2;
			My_Landmark->color[3]=0.5;
			My_Landmark->is_flag=0;					  
		  
		  }
		  else 
		  {
			My_Landmark->color[0]=g_flag_color[0];
			My_Landmark->color[1]=g_flag_color[1];
			My_Landmark->color[2]=g_flag_color[2];
			My_Landmark->flag_length = g_flag_length;
			My_Landmark->color[3]=1.0;
			My_Landmark->is_flag=1;					  
		  }
			My_Landmark->Landmark_init_Mat_Landmark(g_landmark_size, landmark_mode);
			return My_Landmark;
}


void CONTAINER_MESH::Mesh_CreateLandmarkAtMouse(float x, float y, int landmark_mode)
{

	OBJECT_LANDMARK * My_Landmark;
	
	float xx, yy, zz, nxx, nyy, nzz;
	vtkIdType ve;
	float mouse_vertex[3];
	
  // contains the coordinates of the mouse pointer (=mouse vertex)
  // assume that the mouse vertex is at the front plane of the viewing cube, i.e. z-coord= -1.0
    mouse_vertex[0]=x;
	mouse_vertex[1]=y;
	mouse_vertex[2]= -1;
	GetClosestVertex(mouse_vertex,&xx,&yy,&zz, &nxx, &nyy, &nzz, &ve, 2);
	//std::cout<<"Tag: mouse vertex :"<<x<<","<<y<<""<<std::endl;
	//std::cout<<"Closest vertex :"<<xx<<","<<yy<<","<<zz<<std::endl;
	//std::cout <<"Closest vertex ID : "<<ve<<std::endl;
	My_Landmark = NULL;	
	My_Landmark = CreateLandmark(xx,yy, zz, nxx, nyy, nzz, landmark_mode); 	
	if (My_Landmark != NULL)
	{
			this->Add_Landmark (My_Landmark, landmark_mode);

	}

 


}



OBJECT_MESH* CONTAINER_MESH::GetClosestVertex(float input [3], float *x,float *y,float *z, float *nx,float *ny,float *nz, vtkIdType* ve, int mode)
									 
{	

float closest[3];
float nclosest[3];
 closest[0]=0;
 closest[1]=0;
 closest[2]=0;

 float vvs[3], vv1[3];



 float xx, yy,zz;
 vtkIdType veve;
 vtkIdType vemin;
  float nxx, nyy, nzz;
 OBJECT_MESH *My_Obj;
 OBJECT_MESH *Return_Obj=NULL;
 
 float min_dist;
 float dist;
 glMatrix screen_mat;
 glMatrix cam_mat;
 glMatrix screenproj;
		
  get_projection_matrix(screenproj);
		get_camera_transform_matrix(cam_mat);
		glPushMatrix();
		glLoadIdentity();	
		glMultMatrixf((GLfloat*) screenproj);
		glMultMatrixf((GLfloat*) cam_mat);
		getmatrix(screen_mat);
		glPopMatrix ();

	min_dist = 1000000000;
 if (this->OBJECTS_ROOT->OBJECTS != NULL)
 {
			
	My_Obj = this->OBJECTS_ROOT->OBJECTS;	
	Return_Obj=My_Obj;
	while(My_Obj!=NULL)
	{

		
		// We only do this on viewed objects
		if (My_Obj->view ==1)
		{
		
			
			My_Obj->Mesh_Find_Closest_Vertex(input, &xx, &yy, &zz,&nxx, &nyy, &nzz, &veve, mode);
			if (mode ==1)
			{
				dist = (xx - input[0])*(xx - input[0]) + (yy - input[1])*(yy - input[1]) + (zz - input[2])*(zz - input[2]);
			}
			else
			{
				vv1[0]=xx;
				vv1[1]=yy;
				vv1[2]=zz;
				ApplyTransformation(vv1,vvs,screen_mat);	// projection of vv1 on the screen
				dist = (vvs[0] - input[0])*(vvs[0] - input[0]) + (vvs[1] - input[1])*(vvs[1] - input[1]) + 0.001*(vvs[2] - input[2])*(vvs[2] - input[2]);
			}
			if (dist<min_dist)
			{
				Return_Obj=My_Obj;
				closest[0]=xx;
				closest[1]=yy;
				closest[2]=zz;
				nclosest[0]=nxx;
				nclosest[1]=nyy;
				nclosest[2]=nzz;
				min_dist = dist;
				vemin =veve;
			}
		}
		My_Obj = My_Obj->nextobj;
	}				
 }
 	*x=closest[0];
	*y=closest[1];
	*z=closest[2];
	*nx=nclosest[0];
	*ny=nclosest[1];
	*nz=nclosest[2];
	*ve = vemin;
	return Return_Obj;
}

void CONTAINER_MESH::Selected_Landmarks_Change_Orientation(int landmark_mode)
{

	OBJECT_LANDMARK *p;
	float xx, yy, zz, nx, ny, nz, input[3];
	vtkIdType ve;
	
	 int ind =0;
	
	 p = this->landmarkafter(ind, landmark_mode);

	 while (p !=NULL)
	 {
  
		if (p->selected==1)
		{
			input[0]= p->Mat2[3][0];
			input[1] = p->Mat2[3][1];
			input[2]= p->Mat2[3][2];

			GetClosestVertex(input,&xx,&yy,&zz, &nx, &ny, &nz, &ve, 1);
			
			

			//Returns nearest landmark of this
			// But : orienter la pointe du landmark (dessin\E9e de mani\E8re \E0 ce que la pointe soit \E0 la coordonn\E9e 1 0 0)
			// de mani\E8re \E0 ce qu'elle pointe dans la direction nx, ny, nz
			// On va faire : 
			// 1er) 1 rotation le long de l'axe x de mani\E8re \E0 tourner le landmark le long de cet axe.
			//      1 matrice de rotation le long de l'axe z s'\E9crit de cette mani\E8re :
			//        cosz  -sinz  0
			//        sinz  cosz   0
			//        0     0      1		
			// Je l'appelle MAT_Z (z est un angle qui va de -PI \E0 PI)


			// 2eme) 1 rotation le long de l'axe z de mani\E8re \E0 
			//      1 matrice de rotation le long de l'axe z s'\E9crit :
			//        1    0     0
			//        0    cosx  -sinx
			//        0    sinx  cosx
			// Je l'appelle MAT_X (x est un angle qui va de -PI \E0 PI)


			// Pour appliquer les rotations suivant Z selon un angle z, puis autour de X selon un angle x, 
			// il faudra multiplier les coordonn\E9es par MAT_Z * MAT_X => 
			// Soit par la matrice : 
			//
			//       cosz  -       sinz         0
			//       cosx*sinz  cosx*cosz  -sinx
			//       sinx_sinz  sinx*cosz    cosx
			 
			
			 //C'est cette matrice qui fournira la combinaison des deux rotations.
			// Maintenant, calculons MAT_X et MAT_Z
			// Quelles valeurs ont cosx, sinx, sinz, cosz dans le cas qui m'occupe?

			// Mon but : je veux multiplier les coordonn\E9es de la pointe du landmark initial (0 0 1) de mani\E8re \E0 ce qu'elle
			// atteigne la position (nx, ny, nz)
			// J'ai donc les contraintes suivantes (que je trouve en multipliant MAT_Z*MAT_X par (0,0,1)): 
			// nx = cosz (1)
			// ny = -cosz * sinx (2)
			// nz = sinx * sinz (3)

			// Il est possible de choisir des angles z puis x de mani\E8re \E0 satisfaire ces contraintes. 
			// Une possibilit\E9 est de faire: 
			// I)  la premi\E8re rotation autour de l'axe de mani\E8re \E0 avoir :
			//		(1 0 0) qui devienne (nx, racine(1-nx\B2), 0) 
			//		=> Dans cette premi\E8re rotation, on peut d\E9cider arbitrairement 
			///     que la coordonn\E9e y du point transofm\E9 sera positive
			//		quel que soit nx (>0 ou <0).
			//		cette rotation se fait selon un angle "z"  de rotation autour de l'axe z, qui peut \EAtre positif ou n\E9gatif selon
			//		que nz soit n\E9gatif ou positif, respectivement..
			//		Dans ce cas je veux que :
			//				cosz = nx; (c'est aussi ma contrainte 1) 
			//				sinz = racine(1- cosz\B2)  [d\E9cid\E9 arbitrairement]
			// II) Dans un deuxi\E8me temps j'effectue une rotation autour de l'axe x qui va satisfaire mes contraintes 2 et 3
			//		Dans ce cas, je d\E9duis que :   
			//				cosx= ny/sinz	(d\E9duit de contrainte 2, car je connais d\E9j\E0 sinz)		
			//				sinz= nz/sinz;   ( d\E9duit de la contrainte 3, car je connais d\E9j\E0 sinz)

			// Je suis donc parvenu \E0 calculer une matrice qui satisfait mes contraintes.         
			 // rotation 
		  // Landmark object oriented along x axis : 0 0 1 
			  // assume rot.  
				// 1) around z 
				// 2)    around x 
			  // -> values sinz,cosz,sinx,cosx need to be computed
			  
			
			  float cosz=nx;
			  float sinz = sqrt(1.0-(cosz*cosz));
			  float cosx,sinx;
			  if(sinz==0.0) // just in case we are in the speciale case
			  {
				cosx=1.0;sinx=0.0;	
			  }
			  else
			  {
				cosx= ny/sinz;
				
				sinx=nz/sinz;
			  }
		     
			 p->Mat2[0][0]=cosz; //=nx
			 p->Mat2[0][1]=cosx*sinz; //=ny
			 p->Mat2[0][2]=sinx *sinz;    //=nz
		    
			 p->Mat2[1][0]=-sinz;
			 p->Mat2[1][1]=cosx*cosz;
			 p->Mat2[1][2]=sinx*cosz;

			 p->Mat2[2][0]=0; 
			 p->Mat2[2][1]= -sinx; 
			 p->Mat2[2][2]=cosx; 
			 p->selected=0;
		}
		 ind = p->landmark_index;
		 p = this->landmarkafter(ind, landmark_mode);
	 }

}

void CONTAINER_MESH::Mesh_MoveLandmarkAtMouse(OBJECT_LANDMARK *p, float x, float y, int landmark_mode)
{


	if (p!=NULL)
	{
		float xx, yy, zz, nx, ny, nz;
		vtkIdType ve;
		float mouse_vertex[3];
		

	  // contains the coordinates of the mouse pointer (=mouse vertex)
	  // assume that the mouse vertex is at the front plane of the viewing cube, i.e. z-coord= -1.0
		mouse_vertex[0]=x;mouse_vertex[1]=y;mouse_vertex[2]= -1;
		std::cout<<"x="<<x<<"y="<<y<<std::endl;
		GetClosestVertex(mouse_vertex,&xx,&yy,&zz, &nx, &ny, &nz,&ve, 2);
			
		

		//Returns nearest landmark of this
		// But : orienter la pointe du landmark (dessin\E9e de mani\E8re \E0 ce que la pointe soit \E0 la coordonn\E9e 1 0 0)
		// de mani\E8re \E0 ce qu'elle pointe dans la direction nx, ny, nz
		// On va faire : 
		// 1er) 1 rotation le long de l'axe x de mani\E8re \E0 tourner le landmark le long de cet axe.
		//      1 matrice de rotation le long de l'axe z s'\E9crit de cette mani\E8re :
		//        cosz  -sinz  0
		//        sinz  cosz   0
		//        0     0      1		
		// Je l'appelle MAT_Z (z est un angle qui va de -PI \E0 PI)


		// 2eme) 1 rotation le long de l'axe z de mani\E8re \E0 
		//      1 matrice de rotation le long de l'axe z s'\E9crit :
		//        1    0     0
		//        0    cosx  -sinx
		//        0    sinx  cosx
		// Je l'appelle MAT_X (x est un angle qui va de -PI \E0 PI)


		// Pour appliquer les rotations suivant Z selon un angle z, puis autour de X selon un angle x, 
		// il faudra multiplier les coordonn\E9es par MAT_Z * MAT_X => 
		// Soit par la matrice : 
		//
		//       cosz  -       sinz         0
		//       cosx*sinz  cosx*cosz  -sinx
		//       sinx_sinz  sinx*cosz    cosx
		 
		
		 //C'est cette matrice qui fournira la combinaison des deux rotations.
		// Maintenant, calculons MAT_X et MAT_Z
		// Quelles valeurs ont cosx, sinx, sinz, cosz dans le cas qui m'occupe?

		// Mon but : je veux multiplier les coordonn\E9es de la pointe du landmark initial (0 0 1) de mani\E8re \E0 ce qu'elle
		// atteigne la position (nx, ny, nz)
		// J'ai donc les contraintes suivantes (que je trouve en multipliant MAT_Z*MAT_X par (0,0,1)): 
		// nx = cosz (1)
		// ny = -cosz * sinx (2)
		// nz = sinx * sinz (3)

		// Il est possible de choisir des angles z puis x de mani\E8re \E0 satisfaire ces contraintes. 
		// Une possibilit\E9 est de faire: 
		// I)  la premi\E8re rotation autour de l'axe de mani\E8re \E0 avoir :
		//		(1 0 0) qui devienne (nx, racine(1-nx\B2), 0) 
		//		=> Dans cette premi\E8re rotation, on peut d\E9cider arbitrairement 
		///     que la coordonn\E9e y du point transofm\E9 sera positive
		//		quel que soit nx (>0 ou <0).
		//		cette rotation se fait selon un angle "z"  de rotation autour de l'axe z, qui peut \EAtre positif ou n\E9gatif selon
		//		que nz soit n\E9gatif ou positif, respectivement..
		//		Dans ce cas je veux que :
		//				cosz = nx; (c'est aussi ma contrainte 1) 
		//				sinz = racine(1- cosz\B2)  [d\E9cid\E9 arbitrairement]
		// II) Dans un deuxi\E8me temps j'effectue une rotation autour de l'axe x qui va satisfaire mes contraintes 2 et 3
		//		Dans ce cas, je d\E9duis que :   
		//				cosx= ny/sinz	(d\E9duit de contrainte 2, car je connais d\E9j\E0 sinz)		
		//				sinz= nz/sinz;   ( d\E9duit de la contrainte 3, car je connais d\E9j\E0 sinz)

		// Je suis donc parvenu \E0 calculer une matrice qui satisfait mes contraintes.         
		 // rotation 
	  // Landmark object oriented along x axis : 0 0 1 
		  // assume rot.  
			// 1) around z 
			// 2)    around x 
		  // -> values sinz,cosz,sinx,cosx need to be computed
		  
		  p->Mat2[3][0] =xx;p->Mat2[3][1]= yy;p->Mat2[3][2]= zz;
		  float cosz=nx;
		  float sinz = sqrt(1.0-(cosz*cosz));
		  float cosx,sinx;
		  if(sinz==0.0) // just in case we are in the speciale case
		  {
			cosx=1.0;sinx=0.0;	
		  }
		  else
		  {
			cosx= ny/sinz;
			
			sinx=nz/sinz;
		  }
	     
		 p->Mat2[0][0]=cosz; //=nx
		 p->Mat2[0][1]=cosx*sinz; //=ny
		 p->Mat2[0][2]=sinx *sinz;    //=nz
	    
		 p->Mat2[1][0]=-sinz;
		 p->Mat2[1][1]=cosx*cosz;
		 p->Mat2[1][2]=sinx*cosz;

		 p->Mat2[2][0]=0; 
		 p->Mat2[2][1]= -sinx; 
		 p->Mat2[2][2]=cosx; 	
	 }
}

void CONTAINER_MESH::Mesh_Select(float x, float y)
{
		OBJECT_MESH* My_Obj=NULL;
		float xx, yy, zz, nx, ny, nz;
		vtkIdType ve;
		float mouse_vertex[3];
		

	  // contains the coordinates of the mouse pointer (=mouse vertex)
	  // assume that the mouse vertex is at the front plane of the viewing cube, i.e. z-coord= -1.0
		mouse_vertex[0]=x;mouse_vertex[1]=y;mouse_vertex[2]= -1;
		My_Obj = GetClosestVertex(mouse_vertex,&xx,&yy,&zz, &nx, &ny, &nz,&ve, 2);
		if (My_Obj !=NULL)
		{
			if (My_Obj->selected ==1)
			{My_Obj->selected =0;}
			else
			{My_Obj->selected =1;
			My_Obj->view =1;
			}
		}
	 
}




void CONTAINER_MESH::Mesh_TPS(int basis, float percentage)
{
	double v_init[3];
	double v_final[3];
	float v_trans[3];
	

	float factor = 1.0;

	float input = (percentage) / 100.0;
	if (input >0)
	{
		factor = input;
	}
	std::cout << "Factor=" << factor << std::endl;

	OBJECT_MESH *My_Input;
	OBJECT_MESH *My_New_Obj;

	int ok = 0;
	if ((this->Get_Landmark_Number(0)) > 0 && (this->Get_Landmark_Number(0) == this->Get_Landmark_Number(1)))
	{
		ok = 1;
	}
	if (this->OBJECTS_ROOT->OBJECTS != NULL && ok == 1)
	{

		My_Input = this->OBJECTS_ROOT->OBJECTS;

		while (My_Input != NULL)
		{

			if (My_Input->selected == 1)
			{
				vtkSmartPointer<vtkThinPlateSplineTransform> tps =
					vtkSmartPointer<vtkThinPlateSplineTransform>::New();

				if (basis == 0)
				{
					tps->SetBasisToR();
				}
				else
				{
					tps->SetBasisToR2LogR();
				}
				vtkSmartPointer< vtkPoints > p1 = vtkSmartPointer< vtkPoints >::New();
				vtkSmartPointer< vtkPoints > p2 = vtkSmartPointer< vtkPoints >::New();
				
				// recupère les landmarks source et target
				RecoverLandmarks(p1, p2);

				tps->SetSourceLandmarks(p1);
				tps->SetTargetLandmarks(p2);
				tps->Update();

				vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
				vtkSmartPointer<vtkPolyData> MyTPSInput = vtkSmartPointer<vtkPolyData>::New();
				MyTPSInput->DeepCopy((vtkPolyData*)My_Input);
				
				// recupère la position de l'object s'il a bougé
				Recover_PositionObject_in_PolyData(My_Input, MyTPSInput);

				transformFilter->SetInputData((vtkPolyData*)MyTPSInput);

				/// applique le calcul du tps à l'objet
				transformFilter->SetTransform(tps);  
				transformFilter->Update();

				// mise à jour du maillage sortant pour le tps
				vtkSmartPointer<vtkPolyData> My_Output = vtkSmartPointer<vtkPolyData>::New();
				My_Output = transformFilter->GetOutput();

				// mise a jours des normales
				Update_Normal(My_Output);
				//My_New_Obj

				// application du pourcentage 
				for (int i = 0; i<My_Output->GetNumberOfPoints(); i++) {
					// for every triangle 
					MyTPSInput->GetPoint((vtkIdType)i, v_init);
					My_Output->GetPoint((vtkIdType)i, v_final);
					v_trans[0] = factor*(float)v_final[0] + (1 - factor)*v_init[0];
					v_trans[1] = factor*(float)v_final[1] + (1 - factor)*v_init[1];
					v_trans[2] = factor*(float)v_final[2] + (1 - factor)*v_init[2];

					My_Output->GetPoints()->SetPoint((vtkIdType)i, v_trans);
				}

				std::string newname = My_Input->name.c_str();
				newname.append("_tps");

				My_New_Obj = this->Mesh_PDcontainerload(My_Output, (char*)newname.c_str());


				std::cout << "My_New_Obj->name:" << My_New_Obj->name << std::endl;

				int numpoints = My_New_Obj->GetNumberOfPoints();
				int numtriangles = My_New_Obj->GetNumberOfCells();

				My_New_Obj->selected = 0;
				My_New_Obj->color[0] = My_Input->color[0];
				My_New_Obj->color[1] = My_Input->color[1];
				My_New_Obj->color[2] = My_Input->color[2];
				My_New_Obj->color[3] = 1;


				/*glPushMatrix();
				glLoadMatrixf((GLfloat*) My_Input->Mat2);
				getmatrix( My_New_Obj->Mat2);
				glPopMatrix();
				glPushMatrix();
				glLoadMatrixf((GLfloat*) My_Input->Mat1);
				getmatrix( My_New_Obj->Mat1);
				glPopMatrix();7
				*/

			}//if selected

			My_Input = My_Input->nextobj;
		}//while

		if (g_auto_delete == 1)
		{
			this->Mesh_Delete(1, -1, 0);
		}
	}

}
void CONTAINER_MESH::Mesh_Decimate(int percentage, int method)
{
	
	//0, vtk Decimate Pro
	//1, vtk Quadric decimation
     float decimation_factor = 0.1;

	 float input = ((float)percentage)/100.0;
	 if (input >0 && input<1) 
	 {
		 decimation_factor = input;
	 }
	 int num_edge_to_collapse = (int) (this->numvert*input);


	OBJECT_MESH *My_Obj;
	OBJECT_MESH *My_New_Obj;
	
	if (this->OBJECTS_ROOT->OBJECTS != NULL)
 {
			
	My_Obj = this->OBJECTS_ROOT->OBJECTS;	
	
	while(My_Obj!=NULL)
	{
			
			if (My_Obj->selected ==1)
			{
				
				 vtkSmartPointer<vtkDecimatePro> decimate =
				 vtkSmartPointer<vtkDecimatePro>::New();
				decimate->SetInputData((vtkPolyData*)My_Obj);
				decimate->SetTargetReduction(decimation_factor);

				 vtkSmartPointer<vtkQuadricDecimation> decimate2 =
				 vtkSmartPointer<vtkQuadricDecimation>::New();
				 decimate2->SetInputData((vtkPolyData*)My_Obj);
				 decimate2->SetTargetReduction((double)(decimation_factor));

				if (method ==0)
				{
					decimate->Update();
				}
				else
				{decimate2->Update();}

				vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();
				if (method ==0)
				{
					MyObj=decimate->GetOutput();
				}
				else
				{
				 	MyObj=decimate2->GetOutput();
				}

				
				vtkSmartPointer<vtkPolyDataNormals> ObjNormals =  vtkSmartPointer<vtkPolyDataNormals>::New();		
				ObjNormals->SetInputData(MyObj);
				ObjNormals->ComputePointNormalsOn();
				ObjNormals->ComputeCellNormalsOn();
				//ObjNormals->AutoOrientNormalsOff();
				ObjNormals->ConsistencyOff();

				ObjNormals->Update();
				
				 vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
				 cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());
				 cleanPolyDataFilter->PieceInvariantOff();
				cleanPolyDataFilter->ConvertLinesToPointsOff();
				cleanPolyDataFilter->ConvertPolysToLinesOff();
				cleanPolyDataFilter->ConvertStripsToPolysOff();
				cleanPolyDataFilter->PointMergingOn();
			   cleanPolyDataFilter->Update();

				MyObj= cleanPolyDataFilter->GetOutput();
				std::cout << "\nVtkConnectivity new Number of points:"<<MyObj->GetNumberOfPoints()<< std::endl;
				std::cout << "VtkConnectivity new Number of cells:"<<MyObj->GetNumberOfCells()<< std::endl;	

				
				
				std::string newname = My_Obj->name.c_str();
				newname.append("_dcm");
				My_New_Obj = this->Mesh_PDcontainerload(MyObj, (char*)newname.c_str());
				std::cout << "My_New_Obj->name:"<<My_New_Obj->name<< std::endl;	
				int numpoints = My_New_Obj->GetNumberOfPoints();
				int numtriangles = My_New_Obj->GetNumberOfCells();
				My_New_Obj->selected = 0;								
				My_New_Obj->color[0]=My_Obj->color[0];
				My_New_Obj->color[1]=My_Obj->color[1];
				My_New_Obj->color[2]=My_Obj->color[2];
				My_New_Obj->color[3]= 1;
				

				
				vtkSmartPointer<vtkUnsignedCharArray> colors =
					vtkSmartPointer<vtkUnsignedCharArray>::New();
				colors->SetNumberOfComponents(4);

				colors->SetNumberOfTuples(numpoints);
				colors = (vtkUnsignedCharArray*)My_New_Obj->GetPointData()->GetScalars("RGB");
				if (colors == NULL){
					cout << ((vtkUnsignedCharArray*)My_Obj->GetPointData()->GetScalars("RGB") == NULL);
					colors = (vtkUnsignedCharArray*)My_Obj->GetPointData()->GetScalars("RGB");
					colors->SetName("RGB");
					My_New_Obj->GetPointData()->AddArray(colors);
				}




				glPushMatrix();
				glLoadMatrixf((GLfloat*) My_Obj->Mat2);
				getmatrix( My_New_Obj->Mat2);
				glPopMatrix();
				glPushMatrix();
				glLoadMatrixf((GLfloat*) My_Obj->Mat1);
				getmatrix( My_New_Obj->Mat1);
				glPopMatrix();


			}//if selected

			My_Obj = My_Obj->nextobj;
	}

		if (g_auto_delete ==1)
	{
		this->Mesh_Delete(1, -1, 0);
	}

	}

}
void CONTAINER_MESH::Mesh_Densify(int subdivision_nr)
{
	
	//0, vtk Decimate Pro
	//1, vtk Quadric decimation
     float sub_nr = 1;

	 
	 if (subdivision_nr >0 && subdivision_nr<10) 
	 {
		 sub_nr = subdivision_nr;
	 }
	

	OBJECT_MESH *My_Obj;
	OBJECT_MESH *My_New_Obj;
	
	if (this->OBJECTS_ROOT->OBJECTS != NULL)
 {
			
	My_Obj = this->OBJECTS_ROOT->OBJECTS;	
	
	while(My_Obj!=NULL)
	{
			
			if (My_Obj->selected ==1)
			{
				
				 vtkSmartPointer<vtkDensifyPolyData> densify =
				 vtkSmartPointer<vtkDensifyPolyData>::New();
				densify->SetInputData((vtkPolyData*)My_Obj);
				
				densify->SetNumberOfSubdivisions(sub_nr); 
				densify->Update();
				
				vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();
			
				MyObj=densify->GetOutput();
				

				
				vtkSmartPointer<vtkPolyDataNormals> ObjNormals =  vtkSmartPointer<vtkPolyDataNormals>::New();		
				ObjNormals->SetInputData(MyObj);
				ObjNormals->ComputePointNormalsOn();
				ObjNormals->ComputeCellNormalsOn();
				//ObjNormals->AutoOrientNormalsOff();
				ObjNormals->ConsistencyOff();

				ObjNormals->Update();
				
				 vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
				 cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());
				 cleanPolyDataFilter->PieceInvariantOff();
				cleanPolyDataFilter->ConvertLinesToPointsOff();
				cleanPolyDataFilter->ConvertPolysToLinesOff();
				cleanPolyDataFilter->ConvertStripsToPolysOff();
				cleanPolyDataFilter->PointMergingOn();
			   cleanPolyDataFilter->Update();

				MyObj= cleanPolyDataFilter->GetOutput();
				std::cout << "\nVtkConnectivity new Number of points:"<<MyObj->GetNumberOfPoints()<< std::endl;
				std::cout << "VtkConnectivity new Number of cells:"<<MyObj->GetNumberOfCells()<< std::endl;	

				
				
				std::string newname = My_Obj->name.c_str();
				newname.append("_dens");

				My_New_Obj = this->Mesh_PDcontainerload(MyObj, (char*)newname.c_str());
				
				std::cout << "My_New_Obj->name:"<<My_New_Obj->name<< std::endl;	

				int numpoints = My_New_Obj->GetNumberOfPoints();
				int numtriangles = My_New_Obj->GetNumberOfCells();
			
				My_New_Obj->selected = 0;								
				My_New_Obj->color[0]=My_Obj->color[0];
				My_New_Obj->color[1]=My_Obj->color[1];
				My_New_Obj->color[2]=My_Obj->color[2];
				My_New_Obj->color[3]= 1;


				glPushMatrix();
				glLoadMatrixf((GLfloat*) My_Obj->Mat2);
				getmatrix( My_New_Obj->Mat2);
				glPopMatrix();
				glPushMatrix();
				glLoadMatrixf((GLfloat*) My_Obj->Mat1);
				getmatrix( My_New_Obj->Mat1);
				glPopMatrix();

				
				



			}//if selected

			My_Obj = My_Obj->nextobj;
	}

		if (g_auto_delete ==1)
	{
		this->Mesh_Delete(1, -1, 0);
	}

	}
	
}
void CONTAINER_MESH::Mesh_Fill_Holes(int holes_size)
{
	
     float size = 1000.0;

	 float input = ((float)holes_size);
	 if (input >0 ) 
	 {
		 size = input;
	 }

	OBJECT_MESH *My_Obj;
	OBJECT_MESH *My_New_Obj;
	
	if (this->OBJECTS_ROOT->OBJECTS != NULL)
 {
			
	My_Obj = this->OBJECTS_ROOT->OBJECTS;	
	
	while(My_Obj!=NULL)
	{
			
			if (My_Obj->selected ==1)
			{
				 vtkSmartPointer<vtkFillHolesFilter> fillholes =
				 vtkSmartPointer<vtkFillHolesFilter>::New();
				fillholes->SetInputData((vtkPolyData*)My_Obj);
				
				fillholes->SetHoleSize(size);
				
				fillholes->Update();

				vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();
				MyObj=fillholes->GetOutput();

				vtkSmartPointer<vtkPolyDataNormals> ObjNormals =  vtkSmartPointer<vtkPolyDataNormals>::New();		
				ObjNormals->SetInputData(MyObj);
				ObjNormals->ComputePointNormalsOn();
				ObjNormals->ComputeCellNormalsOn();
				ObjNormals->ConsistencyOn();
				//ObjNormals->AutoOrientNormalsOff();
				//ObjNormals->ConsistencyOff();

				ObjNormals->Update();
				
				 vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
				 cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());
				 cleanPolyDataFilter->PieceInvariantOff();
				cleanPolyDataFilter->ConvertLinesToPointsOff();
				cleanPolyDataFilter->ConvertPolysToLinesOff();
				cleanPolyDataFilter->ConvertStripsToPolysOff();
				cleanPolyDataFilter->PointMergingOn();
			   cleanPolyDataFilter->Update();

				MyObj= cleanPolyDataFilter->GetOutput();
				std::cout << "\nVtkConnectivity new Number of points:"<<MyObj->GetNumberOfPoints()<< std::endl;
				std::cout << "VtkConnectivity new Number of cells:"<<MyObj->GetNumberOfCells()<< std::endl;	

				
				
				std::string newname = My_Obj->name.c_str();
				newname.append("_dcm");

				My_New_Obj = this->Mesh_PDcontainerload(MyObj, (char*)newname.c_str());
				
				std::cout << "My_New_Obj->name:"<<My_New_Obj->name<< std::endl;	

				int numpoints = My_New_Obj->GetNumberOfPoints();
				int numtriangles = My_New_Obj->GetNumberOfCells();
			
				My_New_Obj->selected = 0;								
				My_New_Obj->color[0]=My_Obj->color[0];
				My_New_Obj->color[1]=My_Obj->color[1];
				My_New_Obj->color[2]=My_Obj->color[2];
				My_New_Obj->color[3]= 1;


				glPushMatrix();
				glLoadMatrixf((GLfloat*) My_Obj->Mat2);
				getmatrix( My_New_Obj->Mat2);
				glPopMatrix();
				glPushMatrix();
				glLoadMatrixf((GLfloat*) My_Obj->Mat1);
				getmatrix( My_New_Obj->Mat1);
				glPopMatrix();

				
				



			}//if selected

			My_Obj = My_Obj->nextobj;
	}

		if (g_auto_delete ==1)
	{
		this->Mesh_Delete(1, -1, 0);
	}

	}
	
}
void CONTAINER_MESH::smooth(int iterationnumber, double relaxation_factor)
{
     

	OBJECT_MESH *My_Obj;
	OBJECT_MESH *My_New_Obj;

	if (this->OBJECTS_ROOT->OBJECTS != NULL)
 {
			
	My_Obj = this->OBJECTS_ROOT->OBJECTS;	
	
	while(My_Obj!=NULL)
	{
			
			if (My_Obj->selected ==1)
			{
				vtkSmartPointer<vtkSmoothPolyDataFilter> Sfilter = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
				Sfilter->SetInputData((vtkPolyData*)My_Obj);
				Sfilter->SetNumberOfIterations(iterationnumber);
				Sfilter->SetRelaxationFactor(relaxation_factor);
				Sfilter->Update();

				vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();
				MyObj=Sfilter->GetOutput();

				vtkSmartPointer<vtkPolyDataNormals> ObjNormals =  vtkSmartPointer<vtkPolyDataNormals>::New();		
				ObjNormals->SetInputData(MyObj);
				ObjNormals->ComputePointNormalsOn();
				ObjNormals->ComputeCellNormalsOn();
				//ObjNormals->AutoOrientNormalsOff();
				ObjNormals->ConsistencyOff();

				ObjNormals->Update();
				
				 vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
				 cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());
				 cleanPolyDataFilter->PieceInvariantOff();
				cleanPolyDataFilter->ConvertLinesToPointsOff();
				cleanPolyDataFilter->ConvertPolysToLinesOff();
				cleanPolyDataFilter->ConvertStripsToPolysOff();
				cleanPolyDataFilter->PointMergingOn();
			   cleanPolyDataFilter->Update();

				MyObj= cleanPolyDataFilter->GetOutput();
				std::cout << "\nVtkConnectivity new Number of points:"<<MyObj->GetNumberOfPoints()<< std::endl;
				std::cout << "VtkConnectivity new Number of cells:"<<MyObj->GetNumberOfCells()<< std::endl;	

				
				
				std::string newname = My_Obj->name.c_str();
				newname.append("_smth");

				My_New_Obj = this->Mesh_PDcontainerload(MyObj, (char*)newname.c_str());
				
				std::cout << "My_New_Obj->name:"<<My_New_Obj->name<< std::endl;	

				int numpoints = My_New_Obj->GetNumberOfPoints();
				int numtriangles = My_New_Obj->GetNumberOfCells();
			
				My_New_Obj->selected = 0;								
				My_New_Obj->color[0]=My_Obj->color[0];
				My_New_Obj->color[1]=My_Obj->color[1];
				My_New_Obj->color[2]=My_Obj->color[2];
				My_New_Obj->color[3]= 1;


				glPushMatrix();
				glLoadMatrixf((GLfloat*) My_Obj->Mat2);
				getmatrix( My_New_Obj->Mat2);
				glPopMatrix();
				glPushMatrix();
				glLoadMatrixf((GLfloat*) My_Obj->Mat1);
				getmatrix( My_New_Obj->Mat1);
				glPopMatrix();

				
				



			}//if selected

			My_Obj = My_Obj->nextobj;
	}

		if (g_auto_delete ==1)
	{
		this->Mesh_Delete(1, -1, 0);
	}

	}
		
}




void CONTAINER_MESH::Mesh_Ungroup()
{
	std::cout<<"Before:"<<std::endl;
	this->Hierarchy();
	this->OBJECTS_ROOT->Mesh_Ungroup();
	std::cout<<"After:"<<std::endl;
	this->Hierarchy();
}
void CONTAINER_MESH::SC_calc_depth()
{this->OBJECTS_ROOT->SC_calc_depth();}

void CONTAINER_MESH::SC_calc_thickness_between_objects(float thickness_max_distance, 
									   OBJECT_MESH *source, 
									   OBJECT_MESH *target,
									   Fl_Double_Window* thicknessWindow2)
{
	
	if (source->selected ==1)
	{												
		vtkSmartPointer<vtkFloatArray> newScalars = 
		vtkSmartPointer<vtkFloatArray>::New();
		newScalars->SetNumberOfComponents(1); 
		newScalars->SetNumberOfTuples(source->numvert);			
		vtkIdType ve;			
		double vvd[3];
		double *vvnd;
		float vvn1[3];
		float vvn11[3];
		float vv[3];

		float vv1[3];
		float vv11[3];

		float vvn2[3];
		float vvn22[3];
		
		float vv2[3];
		float vv22[3];

		float AB[3]; // vector between points 1 and 2
		float ABnorm[3];// normalized vector between points 1 and 2.

		float projected_vv2[3];  // projected point along -normal of point 1 towards point 2.
		float AC[3]; // vector between points 1 and projection of point 2
		 
			std::cout<<"Maximal  thickness limit: "<< thickness_max_distance<<std::endl;

		float curr_dist; // absolute distance
						
		vtkSmartPointer<vtkFloatArray> norms_source = 
		vtkSmartPointer<vtkFloatArray>::New();
		norms_source = vtkFloatArray::SafeDownCast(source->GetPointData()->GetNormals());																					

		vtkSmartPointer<vtkFloatArray> norms_target = 
		vtkSmartPointer<vtkFloatArray>::New();
		norms_target = vtkFloatArray::SafeDownCast(target->GetPointData()->GetNormals());																					

		// First, we need to partion vertices data : the algorithm would takes ages
		// to compute if we have to compare each vertex to all vertices! ( n\B2 is too long)

		// Strategy : we divide the vertices into a number of cubes with a side size of thickness_max_distance mm
		// Then we will only compute the distance between a vertex belonging to 1 cube to all the vertices of the given cube
		// and also to the vertices of all adjacent cubes (9*3 - 1 = 26 other cubes). This way, we will manage to compare this
		// vertex to all possible vertices <= thickness limit in distance (+ some other a little bit further). 

		float min[3];
		float max[3];
		glMatrix  wc_mat_source;
		glMatrix  wc_mat_target;
		
		source->get_world_coordinates_matrix(wc_mat_source);
		target->get_world_coordinates_matrix(wc_mat_target);

		min[0]=10000000000;
		min[1]=10000000000;
		min[2]=10000000000;
		
		max[0]=-10000000000;
		max[1]=-10000000000;
		max[2]=-10000000000;

		for ( ve=0;ve<source->numvert;ve++)
		{
			source->GetPoint(ve, vvd);		
			vv1[0] =  (float) vvd[0];
			vv1[1] =  (float) vvd[1];
			vv1[2] =  (float) vvd[2];
			ApplyTransformation(vv1,vv,wc_mat_source);		
			if (vv[0]<min[0]){min[0]=vv[0];}
			if (vv[1]<min[1]){min[1]=vv[1];}
			if (vv[2]<min[2]){min[2]=vv[2];}
			if (vv[0]>max[0]){max[0]=vv[0];}
			if (vv[1]>max[1]){max[1]=vv[1];}
			if (vv[2]>max[2]){max[2]=vv[2];}
		}
		for ( ve=0;ve<target->numvert;ve++)
		{
			target->GetPoint(ve, vvd);		
			vv1[0] =  (float) vvd[0];
			vv1[1] =  (float) vvd[1];
			vv1[2] =  (float) vvd[2];
			ApplyTransformation(vv1,vv,wc_mat_target);		
			if (vv[0]<min[0]){min[0]=vv[0];}
			if (vv[1]<min[1]){min[1]=vv[1];}
			if (vv[2]<min[2]){min[2]=vv[2];}
			if (vv[0]>max[0]){max[0]=vv[0];}
			if (vv[1]>max[1]){max[1]=vv[1];}
			if (vv[2]>max[2]){max[2]=vv[2];}
		}
		std::cout<<"source="<<source->name.c_str()<<std::endl;
		std::cout<<"target="<<target->name.c_str()<<std::endl;
		std::cout<<"min[0]="<<min[0]<<"min[1]="<<min[1]<<"min[2]="<<min[2]<<std::endl;
		std::cout<<"max[0]="<<max[0]<<"max[1]="<<max[1]<<"max[2]="<<max[2]<<std::endl;
		
		std::cout<<"Try to partition Data of source object"<<std::endl;
		source->Partition_Data (min[0],min[1],min[2],max[0],max[1],max[2],thickness_max_distance);
		std::cout<<"Source data partition done"<<std::endl;
		std::cout<<"Try to partition Data of target object"<<std::endl;
		target->Partition_Data (min[0],min[1],min[2],max[0],max[1],max[2],thickness_max_distance);
		std::cout<<"Target data partition done"<<std::endl;
		std::cout<<"total cube nr="<<source->pt_total_cube_nr<<std::endl;
		

		// now we will loop into the vertices of each cube...

		char cpercent[10];
		Fl::check();                               // give fltk some cpu to gray out button			
		Fl_Double_Window *w = thicknessWindow2;           // access parent window				
		w->begin();    
		Fl_Progress *progress = new Fl_Progress(20,100,200,30);
		progress->minimum(0);                      // set progress range to be 0.0 ~ 1.0
		progress->maximum(1);
		progress->color(0x88888800);               // background color
		progress->selection_color(0x4444ff00);     // progress bar color
		progress->labelcolor(FL_WHITE);            // percent text color				
		w->end();				
		progress->show();
		w->redraw();
		Fl::check();  

		vtkIdType ci;
		// check if source and target have same total amount of cubes!
		if (source->pt_total_cube_nr == target->pt_total_cube_nr)
		{
			for (ci=0;ci<source->pt_total_cube_nr; ci++)
			{
				
				int percent = (int)((100*ci)/source->pt_total_cube_nr);
				
				itoa(percent,cpercent,10);
				 strcat (cpercent,"%");
				 progress->value((float)percent/100.0); 					  
				 progress->label(cpercent);  
				 Fl::check();   
				//std::cout<<"progression:"<<percent<<"%\r";
				 
				std::cout<<"cube index:"<<ci<<"\r";
				//std::cout<<"source->pt_vertices_per_cube[ci]:"<<source->pt_vertices_per_cube[ci]<<"\n";
				//std::cout<<"target->pt_vertices_per_cube_compare[ci]:"<<target->pt_vertices_per_cube_compare[ci]<<"\n";

				//std::cout<<"cube"<<actual_cube_raw_index<<" out of "<<total_cube_nr<<"\r";

				
		
				// end of preparation.
				// we are still in cxi, cyi, czi
				// and only here do we start to compare all the vertices of this cube!
				for (vtkIdType i=0;i<source->pt_vertices_per_cube[ci];i++)
				{
					//if (ci==2)
					//{
						//std::cout<<"source i:"<<i<<"\r";
				
					//}

					ve = source->pt_cubes[ci][i];
					//if (ci==2)
					//{
					//	std::cout<<"i="<<i<<"and ve="<<ve<<"\n";
				
					//}
					source->GetPoint(ve, vvd);			
				
					vv1[0] =  (float) vvd[0];
					vv1[1] =  (float) vvd[1];
					vv1[2] =  (float) vvd[2];
					ApplyTransformation(vv1,vv11,wc_mat_source);		
				
					vvnd= norms_source->GetTuple(ve);

					vvn1[0] =  (float) -vvnd[0];
					vvn1[1] =  (float) -vvnd[1];
					vvn1[2] =  (float) -vvnd[2];	
					ApplyRotation(vvn1, vvn11,wc_mat_source);	//rotation of the normals..
							
					
					float newscalar = 0;
					float min_dist = thickness_max_distance;
					float tmp_dist =0;
					
					float min_cos=0.342; // 70 degrees => Don't want to compute thickness using badly oriented vertices.
					float cur_cos = 1.0; // compare ve1's normal and ve2's normal
					float cur_cos2 = 1.0; //compare  ve1's normal and vector between ve1 and ve2
					
					
					for (vtkIdType j=0;j<target->pt_vertices_per_cube_compare[ci];j++)
					{	
						//if (ci==2)
						//{
						//	std::cout<<"j="<<j<<"\r";	
						//}
						vtkIdType ve2 = target->pt_cubes_compare[ci][j];
						
						//if (ci==2)
						//{
							//std::cout<<"j="<<j<<",ve2="<<ve2<<"\r";
				
						//}
						target->GetPoint(ve2, vvd);						
						//if (ci==2)
						//{
						//	std::cout<<"ok get point j="<<j<<"\r";
						//
						//}
						
						vv2[0] =  (float) vvd[0];
						vv2[1] =  (float) vvd[1];
						vv2[2] =  (float) vvd[2];
						ApplyTransformation(vv2,vv22,wc_mat_target);		
				
						vvnd= norms_target->GetTuple(ve2);
						
						vvn2[0] =  (float) vvnd[0];
						vvn2[1] =  (float) vvnd[1];
						vvn2[2] =  (float) vvnd[2];	
						ApplyRotation(vvn2, vvn22,wc_mat_target);	//rotation of the normals..
					

						AB[0]=vv22[0]-vv11[0];
						AB[1]=vv22[1]-vv11[1];
						AB[2]=vv22[2]-vv11[2];
								
						// distance between points 1 and 2
						curr_dist=(float)sqrt(AB[0]*AB[0]+AB[1]*AB[1]+AB[2]*AB[2]);
						ABnorm[0]=0;ABnorm[1]=0; ABnorm[2]=0;
						if (curr_dist>0)
						{
							ABnorm[0]= AB[0]/curr_dist;
							ABnorm[1]= AB[1]/curr_dist;
							ABnorm[2]= AB[2]/curr_dist;
						}


						if (curr_dist <thickness_max_distance)
						{
							// calculate projected point along the normal of point 1 
							// towards point 2
							// seach if vv1n et vv2n sont suffisamment dans la bonne direction.
							cur_cos = vvn11[0]*vvn22[0]+vvn11[1]*vvn22[1]+vvn11[2]*vvn22[2];
							cur_cos2 = ABnorm[0]*vvn11[0]+ABnorm[1]*vvn11[1]+ABnorm[2]*vvn11[2];
							if (cur_cos> min_cos && cur_cos2>min_cos)
							{
								// we have a candidate!
								// compute projected point 2 along vvn1 !
								projected_vv2[0]  =vv11[0] + vvn11[0]*AB[0];
								projected_vv2[1]  =vv11[1] + vvn11[1]*AB[1];
								projected_vv2[2]  =vv11[2] + vvn11[2]*AB[2];
								AC[0] = projected_vv2[0]  -vv11[0];
								AC[1] = projected_vv2[1]  -vv11[1];
								AC[2] = projected_vv2[2]  -vv11[2];
								//tmp_dist = (float)sqrt(AC[0]*AC[0]+AC[1]*AC[1]+AC[2]*AC[2]);
								// well I am not sure I want this distance!!!
								tmp_dist = curr_dist;

							}
							else
							{
								tmp_dist = thickness_max_distance;
							}
						}
						else
						{
							tmp_dist = thickness_max_distance;
						}
						if (tmp_dist< min_dist){min_dist = tmp_dist;}												
						
					}// for all vertices to compare																			  					
					newScalars->InsertTuple1(ve, min_dist);
				}// for all the vertices in actual cube										
			}// for all cubes
					// ve_cubecoords[0] + cube_nr[0]*(ve_cubecoords[1]+ cube_nr[1]*ve_cubecoords[2]) 			
			//std::cout<<"New Scalar computation done "<<std::endl;
			newScalars->SetName("Thickness");
			source->GetPointData()->RemoveArray("Thickness");			
			//std::cout<<"Add array "<<std::endl;
			source->GetPointData()->AddArray(newScalars);
			//std::cout<<"Set Active scalar"<<std::endl;
			source->GetPointData()->SetActiveScalars("Thickness");	
		}
		else
		{
			fl_alert("Partition failed : not the same number of cubes in source and target objects.");
		}
		w->remove(progress);     
		delete(progress); 
						 				   
		source->selected = 0;
		target->selected = 0;

		source->Partition_Data_Delete ();
		target->Partition_Data_Delete ();
	}//selected

}
void CONTAINER_MESH::SC_calc_thickness(float thickness_max_distance, Fl_Double_Window* thicknessWindow)
{
	this->OBJECTS_ROOT->SC_calc_thickness(thickness_max_distance, thicknessWindow);
}


void CONTAINER_MESH::Set_Active_Scalar()
{
	 OBJECT_MESH *My_Obj;  
   if (this->OBJECTS_ROOT->OBJECTS != NULL)
	{
			
		My_Obj = this->OBJECTS_ROOT->OBJECTS;	
		My_Obj->Set_Active_Scalar();
		
   }
}

void CONTAINER_MESH::Initialize_Scalar(int scalar)
{
	OBJECT_MESH *My_Obj;  
   if (this->OBJECTS_ROOT->OBJECTS != NULL)
	{
			
		My_Obj = this->OBJECTS_ROOT->OBJECTS;	
		My_Obj->Initialize_Scalar(scalar);
		
   }
	
}

void CONTAINER_MESH::Remove_Scalar(int scalar)
{
	OBJECT_MESH *My_Obj;  
   if (this->OBJECTS_ROOT->OBJECTS != NULL)
	{			
		My_Obj = this->OBJECTS_ROOT->OBJECTS;	
		My_Obj->Remove_Scalar(scalar);		
   }
	
}
void CONTAINER_MESH::SC_calc_curv(int mode)
{
  OBJECT_MESH *My_Obj;
  
   if (this->OBJECTS_ROOT->OBJECTS != NULL)
	{
			
		My_Obj = this->OBJECTS_ROOT->OBJECTS;	
	
		while (My_Obj != NULL)
		{

			if (My_Obj->selected == 1)
			{
				//cout << "selected curv" << endl; 

				vtkSmartPointer<vtkCurvatures> curvaturesFilter =
					vtkSmartPointer<vtkCurvatures>::New();
				g_active_scalar = mode;
				std::string sc_name = this->Get_Active_Scalar_Name();

				//cout << "curv   a  " << sc_name.c_str() << endl;
				My_Obj->GetPointData()->RemoveArray(sc_name.c_str());
				//cout << "curv   a bis" << endl;
				if (mode == 1)
				{
					curvaturesFilter->SetCurvatureTypeToMaximum();
				}
				else if (mode == 2)
				{
					curvaturesFilter->SetCurvatureTypeToMinimum();
				}
				else if (mode == 3)
				{
					curvaturesFilter->SetCurvatureTypeToGaussian();
				}
				else if (mode == 4)
				{
					curvaturesFilter->SetCurvatureTypeToMean();
				}
				//cout << "curv   b" << endl;
				curvaturesFilter->SetInputData((vtkPolyData*)My_Obj);
				curvaturesFilter->Update();
				vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();
				MyObj = curvaturesFilter->GetOutput();
				//cout << "curv   c" << endl;
				// Get active scalars of output
				vtkFloatArray *freshScalars;
				freshScalars = (vtkFloatArray*)MyObj->GetPointData()->GetScalars();

				My_Obj->selected = 0;
				//cout << "curv   d" << endl;
				vtkSmartPointer<vtkFloatArray> newScalars =
					vtkSmartPointer<vtkFloatArray>::New();
				newScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				newScalars->SetNumberOfTuples(My_Obj->numvert);
				float sca;
				//cout << "curv   e" << endl;
				for (int i = 0; i < My_Obj->numvert; i++)	// for each vertex of this
				{

					sca = freshScalars->GetTuple((vtkIdType)i)[0];
					newScalars->InsertTuple1(i, sca);
				}
				//std::cout<<"Scalar insertion done!"<<sca<<std::endl;

				newScalars->SetName(sc_name.c_str());
				My_Obj->GetPointData()->AddArray(newScalars);
				My_Obj->GetPointData()->SetActiveScalars(sc_name.c_str());

				//cout << "finish" << endl;
			}//if selected

			My_Obj = My_Obj->nextobj;
		}
	}
			

	
	// A changer : ajouter les calculs de courbure \E0 l'objet initial!
	//if (g_auto_delete ==1)
	//{
	//this->Mesh_Delete(1, -1, 0);
	//}

}

void CONTAINER_MESH::SC_calc_mean()
{
	this->OBJECTS_ROOT->SC_calc_mean();
}

void CONTAINER_MESH::Get_Landmark_Coordinates (int landmark_number,float *x,float *y,float *z, int landmark_mode)
{
	OBJECT_LANDMARK *MyLandmark;
	MyLandmark = NULL;	
	//this->OBJECTS_ROOT->Get_Landmark_Coordinates (landmark_number, &wcx, &wcy, &wcz);
	MyLandmark = this->OBJECTS_ROOT->LandmarkAfter(landmark_number-1, landmark_mode);
	if (MyLandmark !=NULL && MyLandmark->landmark_index == landmark_number)
	{		
		*x = MyLandmark->Mat2[3][0];
		*y= MyLandmark->Mat2[3][1];
		*z=MyLandmark->Mat2[3][2];
	}
	else
	{
		*x = 0;
		*y= 0;
		*z=0;
	}
}
void CONTAINER_MESH::Mesh_Group()
{
	
		
	this->OBJECTS_ROOT->Mesh_Group();


	//OBJECT_MESH *logical_obj = new OBJECT_MESH;
	//logical_obj->is_logical = 1;

	
}




void CONTAINER_MESH::Mesh_Decompose_Tag (int min_region_size, Fl_Double_Window* myWindow)
{

	std::cout<< "min_region_size="<<min_region_size<<std::endl;
	
	OBJECT_MESH *My_Obj;
	OBJECT_MESH *My_New_Obj;  
	

	if (this->OBJECTS_ROOT->OBJECTS != NULL)
	{			
		My_Obj = this->OBJECTS_ROOT->OBJECTS;		
		while(My_Obj!=NULL)
		{
			
			if (My_Obj->selected ==1)
			{
				//1) Copy Tags into new TMP scalar				
				vtkSmartPointer<vtkFloatArray> newScalars = 
				vtkSmartPointer<vtkFloatArray>::New();

				newScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				newScalars->SetNumberOfTuples(My_Obj->numvert);				
				
				vtkFloatArray *currentTags;		
				currentTags = (vtkFloatArray*)My_Obj->GetPointData()->GetScalars("Tags");

				for (vtkIdType i=0;i<My_Obj->numvert;i++)	// for each vertex 
				{									
					//std::cout<<"vertex"<<i<<", current region:"<<currentRegions->GetTuple(i)[0]<<std::endl;
						newScalars->InsertTuple1(i, currentTags->GetTuple(i)[0]);																				
				}
				newScalars->SetName("TMP");

				My_Obj->GetPointData()->RemoveArray("TMP");				
				My_Obj->GetPointData()->AddArray(newScalars);
				My_Obj->GetPointData()->SetActiveScalars("TMP");

				//2 Decomposition
				vtkSmartPointer<vtkIdTypeArray> region_sizes = vtkSmartPointer<vtkIdTypeArray>::New();				
				region_sizes = My_Obj->Mesh_Get_Tag_Region_Sizes();				
				int percent;
				char cpercent[10];				
				Fl::check();                               // give fltk some cpu to gray out button			
				Fl_Double_Window *w = myWindow;           // access parent window				
				w->begin();    
				Fl_Progress *progress = new Fl_Progress(20,50,200,30);
				progress->minimum(0);                      // set progress range to be 0.0 ~ 1.0
				progress->maximum(1);
				progress->color(0x88888800);               // background color
				progress->selection_color(0x4444ff00);     // progress bar color
				progress->labelcolor(FL_WHITE);            // percent text color				
				w->end();				
				progress->show();
				w->redraw();
				Fl::check();  
				int region_nr = region_sizes->GetNumberOfTuples();
				int cpt=1;
				for (vtkIdType i =0; i<region_sizes->GetNumberOfTuples(); i++)
				{
					 percent = (int)((100*i)/region_nr);
					 itoa( percent, cpercent, 10);
					 strcat (cpercent,"%");
					 progress->value((float)percent/100.0); 					  
					 progress->label(cpercent);  
					 Fl::check();    
					 std::cout<<"progression:"<<percent<<"%\r";										
					if (region_sizes->GetTuple((vtkIdType)i)[0] >= (vtkIdType)min_region_size)
					{
							vtkSmartPointer<vtkThreshold> selector =
						vtkSmartPointer<vtkThreshold>::New();
						vtkSmartPointer<vtkMaskFields> scalarsOff =
						vtkSmartPointer<vtkMaskFields>::New();
						vtkSmartPointer<vtkGeometryFilter> geometry =
						vtkSmartPointer<vtkGeometryFilter>::New();
						selector->SetInputData((vtkPolyData*)My_Obj);
						selector->SetInputArrayToProcess(0, 0, 0,
							vtkDataObject::FIELD_ASSOCIATION_CELLS,
							vtkDataSetAttributes::SCALARS);
						selector->SetAllScalars(g_tag_extraction_criterion_all);
						selector->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "TMP");
						selector->ThresholdBetween((double)(i), (double)(i));
						selector->Update();
							
						scalarsOff->SetInputData(selector->GetOutput());
						scalarsOff->CopyAttributeOff(vtkMaskFields::POINT_DATA,vtkDataSetAttributes::SCALARS);
						scalarsOff->CopyAttributeOff(vtkMaskFields::CELL_DATA, vtkDataSetAttributes::SCALARS);
						scalarsOff->Update();
						geometry->SetInputData(scalarsOff->GetOutput());
						geometry->Update();
																
						vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();						
						MyObj= geometry->GetOutput();
						//Rajouter un filtre...
						std::string newname = My_Obj->name.c_str();
						newname.append("_");
						newname.append(itoa( cpt,  10 ));
						cpt++;

						My_New_Obj = this->Mesh_PDcontainerload(MyObj, (char*)newname.c_str());
						
						My_New_Obj->GetPointData()->SetActiveScalars("Tags");
						int numpoints = My_New_Obj->GetNumberOfPoints();
						int numtriangles = My_New_Obj->GetNumberOfCells();
					
						My_New_Obj->selected = 0;								
						My_New_Obj->color[0]=My_Obj->color[0];
						My_New_Obj->color[1]=My_Obj->color[1];
						My_New_Obj->color[2]=My_Obj->color[2];
						My_New_Obj->color[3]= 1;


						glPushMatrix();
						glLoadMatrixf((GLfloat*) My_Obj->Mat2);
						getmatrix( My_New_Obj->Mat2);
						glPopMatrix();
						glPushMatrix();
						glLoadMatrixf((GLfloat*) My_Obj->Mat1);
						getmatrix( My_New_Obj->Mat1);
						glPopMatrix();
						My_New_Obj->selected =0;
						
					} // if size big enough
				
				}// for all regions!
				w->remove(progress);     
				delete(progress); 				


			}//selected
			My_Obj = My_Obj->nextobj;
		}//while

		
	}//root not null

	if (g_auto_delete ==1)
		{
			this->Mesh_Delete(1, -1, 0);
		}
}



void CONTAINER_MESH::Mesh_Extract_Scalar_Range (double scalar_min, double scalar_max)
{

	std::cout<< "scalar_min="<<scalar_min<<", scalar_max="<<scalar_max<<std::endl;
	OBJECT_MESH *My_Obj;
	OBJECT_MESH *My_New_Obj;  
	std::string active_scalar = this->Get_Active_Scalar_Name();
	std::cout<< "active_scalar="<<active_scalar.c_str()<<std::endl;
   if (this->OBJECTS_ROOT->OBJECTS != NULL)
	{			
		My_Obj = this->OBJECTS_ROOT->OBJECTS;		
		while(My_Obj!=NULL)
		{		
			if (My_Obj->selected ==1)
			{			
				//1) Copy Active Scalar into new TMP scalar				
				vtkSmartPointer<vtkFloatArray> newScalars = 
				vtkSmartPointer<vtkFloatArray>::New();

				newScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				newScalars->SetNumberOfTuples(My_Obj->numvert);				
				
				vtkFloatArray *currentScalar;		
				currentScalar = (vtkFloatArray*)My_Obj->GetPointData()->GetScalars();

				if (currentScalar !=NULL)
				{
					for (vtkIdType i=0;i<My_Obj->numvert;i++)	// for each vertex 
					{									
						//std::cout<<"vertex"<<i<<", current region:"<<currentRegions->GetTuple(i)[0]<<std::endl;
							newScalars->InsertTuple1(i, currentScalar->GetTuple(i)[0]);																				
					}
					newScalars->SetName("TMP");

					My_Obj->GetPointData()->RemoveArray("TMP");				
					My_Obj->GetPointData()->AddArray(newScalars);
					My_Obj->GetPointData()->SetActiveScalars("TMP");
																								
							
					vtkSmartPointer<vtkThreshold> selector =
					vtkSmartPointer<vtkThreshold>::New();
					vtkSmartPointer<vtkMaskFields> scalarsOff =
					vtkSmartPointer<vtkMaskFields>::New();
					vtkSmartPointer<vtkGeometryFilter> geometry =
					vtkSmartPointer<vtkGeometryFilter>::New();
					selector->SetInputData((vtkPolyData*)My_Obj);
					selector->SetInputArrayToProcess(0, 0, 0,
										vtkDataObject::FIELD_ASSOCIATION_CELLS,
										vtkDataSetAttributes::SCALARS);
					selector->SetAllScalars(g_tag_extraction_criterion_all);
					selector->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "TMP");
					selector->ThresholdBetween(scalar_min, scalar_max);
					selector->Update();
					std::cout << "\n Extract_Scalar_Range new Number of points:"<<selector->GetOutput()->GetNumberOfPoints()<< std::endl;
					std::cout << "\nExtract_Scalar_Range new Number of cells:"<<selector->GetOutput()->GetNumberOfCells()<< std::endl;	
					
					scalarsOff->SetInputData(selector->GetOutput());
					scalarsOff->CopyAttributeOff(vtkMaskFields::POINT_DATA,vtkDataSetAttributes::SCALARS);
					scalarsOff->CopyAttributeOff(vtkMaskFields::CELL_DATA, vtkDataSetAttributes::SCALARS);
					scalarsOff->Update();
					geometry->SetInputData(scalarsOff->GetOutput());
					geometry->Update();
					
									
					vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();
					

					MyObj= geometry->GetOutput();
					std::cout << "\nExtract_Scalar_Range new Number of points:"<<MyObj->GetNumberOfPoints()<< std::endl;
					std::cout << "\nExtract_Scalar_Range  new Number of cells:"<<MyObj->GetNumberOfCells()<< std::endl;	
								
					if (MyObj->GetNumberOfPoints()>10)
					{
						std::string newname = My_Obj->name.c_str();
						newname.append("_scr");
						

						My_New_Obj = this->Mesh_PDcontainerload(MyObj, (char*)newname.c_str());

						//My_New_Obj = this->Mesh_PDcontainerload(MyObj, My_Obj->name);
						//std::cout << "My_New_Obj->name:"<<My_New_Obj->name<< std::endl;	
						My_New_Obj->GetPointData()->SetActiveScalars(active_scalar.c_str());
						int numpoints = My_New_Obj->GetNumberOfPoints();
						int numtriangles = My_New_Obj->GetNumberOfCells();
					
						My_New_Obj->selected = 0;								
						My_New_Obj->color[0]=My_Obj->color[0];
						My_New_Obj->color[1]=My_Obj->color[1];
						My_New_Obj->color[2]=My_Obj->color[2];
						My_New_Obj->color[3]= 1;


						glPushMatrix();
						glLoadMatrixf((GLfloat*) My_Obj->Mat2);
						getmatrix( My_New_Obj->Mat2);
						glPopMatrix();
						glPushMatrix();
						glLoadMatrixf((GLfloat*) My_Obj->Mat1);
						getmatrix( My_New_Obj->Mat1);
						glPopMatrix();
					}
				}// if current scalar is not null;
				else
				{
					// if there was nothing into current active scalar... do not delete object!
					My_Obj->selected =0;
				
				}
						
		}//if this is selected
	
			My_Obj = My_Obj->nextobj;
	}// while

		if (g_auto_delete ==1)
		{
			this->Mesh_Delete(1, -1, 0);
		}
   }// object root not null

}

void CONTAINER_MESH::Mesh_Delete_Scalar (double scalar_to_remove)
{

	OBJECT_MESH *My_Obj;
	OBJECT_MESH *My_New_Obj;  
	
   if (this->OBJECTS_ROOT->OBJECTS != NULL)
	{			
		My_Obj = this->OBJECTS_ROOT->OBJECTS;		
		while(My_Obj!=NULL)
		{		
			if (My_Obj->selected ==1)
			{			
				//1) Copy Active Scalar into new TMP scalar				
				vtkSmartPointer<vtkFloatArray> newScalars = 
				vtkSmartPointer<vtkFloatArray>::New();

				newScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				newScalars->SetNumberOfTuples(My_Obj->numvert);				
				
				vtkFloatArray *currentTags;		
				currentTags = (vtkFloatArray*)My_Obj->GetPointData()->GetScalars("Tags");

				if (currentTags !=NULL)
				{
					for (vtkIdType i=0;i<My_Obj->numvert;i++)	// for each vertex 
					{									
						//std::cout<<"vertex"<<i<<", current region:"<<currentRegions->GetTuple(i)[0]<<std::endl;
						if (scalar_to_remove == currentTags->GetTuple(i)[0])
						{
							newScalars->InsertTuple1(i, -1);
						}
						else
						{
							newScalars->InsertTuple1(i, currentTags->GetTuple(i)[0]);
						}
					}
					newScalars->SetName("TMP");

					My_Obj->GetPointData()->RemoveArray("TMP");				
					My_Obj->GetPointData()->AddArray(newScalars);
					My_Obj->GetPointData()->SetActiveScalars("TMP");
																								
							
					vtkSmartPointer<vtkThreshold> selector =
					vtkSmartPointer<vtkThreshold>::New();
					vtkSmartPointer<vtkMaskFields> scalarsOff =
					vtkSmartPointer<vtkMaskFields>::New();
					vtkSmartPointer<vtkGeometryFilter> geometry =
					vtkSmartPointer<vtkGeometryFilter>::New();
					selector->SetInputData((vtkPolyData*)My_Obj);
					selector->SetInputArrayToProcess(0, 0, 0,
										vtkDataObject::FIELD_ASSOCIATION_CELLS,
										vtkDataSetAttributes::SCALARS);
					selector->SetAllScalars(g_tag_extraction_criterion_all);
					selector->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "TMP");
					selector->ThresholdByUpper(0.0);
					selector->Update();
					std::cout << "\nDelete_Scalar_Range new Number of points:"<<selector->GetOutput()->GetNumberOfPoints()<< std::endl;
					std::cout << "Delete_Scalar_Range new Number of cells:"<<selector->GetOutput()->GetNumberOfCells()<< std::endl;	
					
					scalarsOff->SetInputData(selector->GetOutput());
					scalarsOff->CopyAttributeOff(vtkMaskFields::POINT_DATA,vtkDataSetAttributes::SCALARS);
					scalarsOff->CopyAttributeOff(vtkMaskFields::CELL_DATA, vtkDataSetAttributes::SCALARS);
					scalarsOff->Update();
					geometry->SetInputData(scalarsOff->GetOutput());
					geometry->Update();
					
									
					vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();
					

					MyObj= geometry->GetOutput();
					std::cout << "Extract_Scalar_Range new Number of points:"<<MyObj->GetNumberOfPoints()<< std::endl;
					std::cout << "Extract_Scalar_Range  new Number of cells:"<<MyObj->GetNumberOfCells()<< std::endl;	
								
					if (MyObj->GetNumberOfPoints()>10)
					{
						std::string newname = My_Obj->name.c_str();
						newname.append("_delsc");
						

						My_New_Obj = this->Mesh_PDcontainerload(MyObj, (char*)newname.c_str());
						
						//std::cout << "My_New_Obj->name:"<<My_New_Obj->name<< std::endl;	
						My_New_Obj->GetPointData()->SetActiveScalars("Tags");
						int numpoints = My_New_Obj->GetNumberOfPoints();
						int numtriangles = My_New_Obj->GetNumberOfCells();
					
						My_New_Obj->selected = 0;								
						My_New_Obj->color[0]=My_Obj->color[0];
						My_New_Obj->color[1]=My_Obj->color[1];
						My_New_Obj->color[2]=My_Obj->color[2];
						My_New_Obj->color[3]= 1;


						glPushMatrix();
						glLoadMatrixf((GLfloat*) My_Obj->Mat2);
						getmatrix( My_New_Obj->Mat2);
						glPopMatrix();
						glPushMatrix();
						glLoadMatrixf((GLfloat*) My_Obj->Mat1);
						getmatrix( My_New_Obj->Mat1);
						glPopMatrix();
					}
				}// if current tags is not null;
				else
				{
					// if there was nothing into Tags..... do not delete object!
					My_Obj->selected =0;
				
				}
						
		}//if this is selected
	
			My_Obj = My_Obj->nextobj;
	}// while

		if (g_auto_delete ==1)
		{
			this->Mesh_Delete(1, -1, 0);
		}
   }// object root not null

}

void CONTAINER_MESH::Mesh_Tag_Connected_Regions()
{

		OBJECT_MESH *My_Obj;
		OBJECT_MESH *My_New_Obj;
		
	if (this->OBJECTS_ROOT->OBJECTS != NULL)
 {
			
	My_Obj = this->OBJECTS_ROOT->OBJECTS;	
	
	while(My_Obj!=NULL)
	{
			
			if (My_Obj->selected ==1)
			{
				vtkSmartPointer<vtkPolyDataConnectivityFilter> cfilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
				cfilter->SetInputData((vtkPolyData*)My_Obj);

				cfilter->SetExtractionModeToAllRegions(); 
				cfilter->ColorRegionsOn();
				cfilter->Update();       
				int regions = cfilter->GetNumberOfExtractedRegions(); 
				std::cout << "\nVtkConnectivity number of regions:"<<regions<< std::endl;
				
				
				vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
				cleanPolyDataFilter->SetInputData(cfilter->GetOutput());
				cleanPolyDataFilter->PieceInvariantOff();
				cleanPolyDataFilter->ConvertLinesToPointsOff();
				cleanPolyDataFilter->ConvertPolysToLinesOff();
				cleanPolyDataFilter->ConvertStripsToPolysOff();
				cleanPolyDataFilter->PointMergingOn();
				cleanPolyDataFilter->Update();


				vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();			
				MyObj=cleanPolyDataFilter->GetOutput();		

				std::cout << "\nVtkConnected regions new Number of points:"<<MyObj->GetNumberOfPoints()<< std::endl;
				std::cout << "VtkConnected regions new Number of cells:"<<MyObj->GetNumberOfCells()<< std::endl;					
				std::string newname = My_Obj->name.c_str();
				
				My_New_Obj = this->Mesh_PDcontainerload(MyObj, (char*)newname.c_str());
				
				std::cout << "My_New_Obj->name:"<<My_New_Obj->name<< std::endl;	

				int numpoints = My_New_Obj->GetNumberOfPoints();
				int numtriangles = My_New_Obj->GetNumberOfCells();
				My_New_Obj->selected = 0;								
				My_New_Obj->color[0]=My_Obj->color[0];
				My_New_Obj->color[1]=My_Obj->color[1];
				My_New_Obj->color[2]=My_Obj->color[2];
				My_New_Obj->color[3]= 1;
				glPushMatrix();
				glLoadMatrixf((GLfloat*) My_Obj->Mat2);
				getmatrix( My_New_Obj->Mat2);
				glPopMatrix();
				glPushMatrix();
				glLoadMatrixf((GLfloat*) My_Obj->Mat1);
				getmatrix( My_New_Obj->Mat1);
				glPopMatrix();

				vtkSmartPointer<vtkIdTypeArray> region_sizes = vtkSmartPointer<vtkIdTypeArray>::New();
				region_sizes=cfilter->GetRegionSizes();

				vtkSmartPointer<vtkIntArray> newTags = 
				vtkSmartPointer<vtkIntArray>::New();

				newTags->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				newTags->SetNumberOfTuples(My_Obj->numvert);				
				vtkSmartPointer<vtkIdTypeArray> currentRegions = vtkSmartPointer<vtkIdTypeArray>::New(); 																		
				currentRegions = vtkIdTypeArray::SafeDownCast(My_New_Obj->GetPointData()->GetArray("RegionId"));						
				for (int i=0;i<My_New_Obj->numvert;i++)	// for each vertex 
				{									
					//std::cout<<"vertex"<<i<<", current region:"<<currentRegions->GetTuple(i)[0]<<std::endl;
						newTags->InsertTuple1(i, (int)currentRegions->GetTuple(i)[0]);																				
				}		
				newTags->SetName("Tags");
			    My_New_Obj->GetPointData()->RemoveArray("RegionId");
				My_New_Obj->GetPointData()->RemoveArray("Tags");
				My_New_Obj->GetPointData()->AddArray(newTags);
				My_New_Obj->GetPointData()->SetActiveScalars("Tags");								

			}//if selected

			My_Obj = My_Obj->nextobj;
	}//while				
	this->Mesh_Delete(1, -1, 0);

 }					

}

void CONTAINER_MESH::Mesh_Decompose(int min_region_size, Fl_Double_Window *mywindow)
{		
	OBJECT_MESH *My_Obj;
	OBJECT_MESH *My_New_Obj;
		
	if (this->OBJECTS_ROOT->OBJECTS != NULL)
 {
			
	My_Obj = this->OBJECTS_ROOT->OBJECTS;	
	
	while(My_Obj!=NULL)
	{
			
			if (My_Obj->selected ==1)
			{


				// First : set TMP scalar 
				vtkSmartPointer<vtkPolyDataConnectivityFilter> cfilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
				cfilter->SetInputData((vtkPolyData*)My_Obj);

				cfilter->SetExtractionModeToAllRegions(); 
				cfilter->ColorRegionsOn();
				cfilter->Update();       
				int regions = cfilter->GetNumberOfExtractedRegions(); 
				std::cout << "\nVtkConnectivity number of regions:"<<regions<< std::endl;
				vtkSmartPointer<vtkIdTypeArray> region_sizes = vtkSmartPointer<vtkIdTypeArray>::New();
				region_sizes=cfilter->GetRegionSizes();

				vtkSmartPointer<vtkFloatArray> newScalars = 
				vtkSmartPointer<vtkFloatArray>::New();

				newScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				newScalars->SetNumberOfTuples(My_Obj->numvert);
				
				vtkSmartPointer<vtkIdTypeArray> currentRegions = vtkSmartPointer<vtkIdTypeArray>::New(); 												
	
				//my_data->GetNu
				currentRegions = vtkIdTypeArray::SafeDownCast(cfilter->GetOutput()->GetPointData()->GetArray("RegionId"));
			
				//std::cout<<"vertex"<<i<<", current region:"<<currentRegions->GetTuple(i)<<std::endl;

				for (int i=0;i<My_Obj->numvert;i++)	// for each vertex 
				{			
						
					//std::cout<<"vertex"<<i<<", current region:"<<currentRegions->GetTuple(i)[0]<<std::endl;
						newScalars->InsertTuple1(i, (double)currentRegions->GetTuple(i)[0]);
							
													
				}		
				newScalars->SetName("TMP");
				vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();			
				MyObj=cfilter->GetOutput();
				
								

			    MyObj->GetPointData()->RemoveArray("RegionId");
				MyObj->GetPointData()->RemoveArray("TMP");
				MyObj->GetPointData()->AddArray(newScalars);
				MyObj->GetPointData()->SetActiveScalars("TMP");


				// Now decompose!								
				int percent;
				char cpercent[10];
				int region_nr = region_sizes->GetNumberOfTuples();							
				Fl::check();                               // give fltk some cpu to gray out button			
				Fl_Double_Window *w = mywindow;           // access parent window				
				w->begin();    
				Fl_Progress *progress = new Fl_Progress(20,50,200,30);
				progress->minimum(0);                      // set progress range to be 0.0 ~ 1.0
				progress->maximum(1);
				progress->color(0x88888800);               // background color
				progress->selection_color(0x4444ff00);     // progress bar color
				progress->labelcolor(FL_WHITE);            // percent text color				
				w->end();				
				progress->show();
				w->redraw();
				Fl::check();        

				int cpt=0;
				for (vtkIdType i =0; i<region_sizes->GetNumberOfTuples(); i++)
				{
					 percent = (int)((100*i)/region_nr);
					 //cpercent = sprintf(cpercent, "%d%%", int(percent));
					 
					 itoa( percent, cpercent, 10);
					 strcat (cpercent,"%");
					 progress->value((float)percent/100.0); 					  
					 progress->label(cpercent);  
					 Fl::check();    

					std::cout<<"progression:"<<percent<<"%\r";										
					if (region_sizes->GetTuple((vtkIdType)i)[0] >= (vtkIdType)min_region_size)
					{
						vtkSmartPointer<vtkThreshold> selector =
						vtkSmartPointer<vtkThreshold>::New();
						vtkSmartPointer<vtkMaskFields> scalarsOff =
						vtkSmartPointer<vtkMaskFields>::New();
						vtkSmartPointer<vtkGeometryFilter> geometry =
						vtkSmartPointer<vtkGeometryFilter>::New();

						selector->SetInputData((vtkPolyData*)MyObj);
						
						selector->SetInputArrayToProcess(0, 0, 0,
							vtkDataObject::FIELD_ASSOCIATION_CELLS,
							vtkDataSetAttributes::SCALARS);
						selector->SetAllScalars(g_tag_extraction_criterion_all);
						selector->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "TMP");
						selector->ThresholdBetween((double)(i), (double)(i));
						selector->Update();
							
						scalarsOff->SetInputData(selector->GetOutput());
						scalarsOff->CopyAttributeOff(vtkMaskFields::POINT_DATA,vtkDataSetAttributes::SCALARS);
						scalarsOff->CopyAttributeOff(vtkMaskFields::CELL_DATA, vtkDataSetAttributes::SCALARS);
						scalarsOff->Update();
						geometry->SetInputData(scalarsOff->GetOutput());
						geometry->Update();
								
						vtkSmartPointer<vtkPolyData> MyObj2 = vtkSmartPointer<vtkPolyData>::New();										
						MyObj2= geometry->GetOutput();	

						std::string newname = My_Obj->name.c_str();
						newname.append("_");
						newname.append(itoa( cpt,  10 ));
						cpt++;

						My_New_Obj = this->Mesh_PDcontainerload(MyObj2, (char*)newname.c_str());
						
						int numpoints = My_New_Obj->GetNumberOfPoints();
						int numtriangles = My_New_Obj->GetNumberOfCells();
					
						My_New_Obj->selected = 0;								
						My_New_Obj->color[0]=My_Obj->color[0];
						My_New_Obj->color[1]=My_Obj->color[1];
						My_New_Obj->color[2]=My_Obj->color[2];
						My_New_Obj->color[3]= 1;


						glPushMatrix();
						glLoadMatrixf((GLfloat*) My_Obj->Mat2);
						getmatrix( My_New_Obj->Mat2);
						glPopMatrix();
						glPushMatrix();
						glLoadMatrixf((GLfloat*) My_Obj->Mat1);
						getmatrix( My_New_Obj->Mat1);
						glPopMatrix();
						My_New_Obj->selected =0;
						
					} // if size big enough
				
				}// for all regions!
				w->remove(progress);     
				delete(progress); 

			}//if selected

			My_Obj = My_Obj->nextobj;
	}
				

		if (g_auto_delete ==1)
	{
		this->Mesh_Delete(1, -1, 0);
	}


 }
			

		
		
	
	
}


void CONTAINER_MESH::Mesh_Largest_Region()
{		
	OBJECT_MESH *My_Obj;
	OBJECT_MESH *My_New_Obj;
		
	if (this->OBJECTS_ROOT->OBJECTS != NULL)
 {
			
	My_Obj = this->OBJECTS_ROOT->OBJECTS;	
	
	while(My_Obj!=NULL)
	{
			
			if (My_Obj->selected ==1)
			{
				vtkSmartPointer<vtkPolyDataConnectivityFilter> cfilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
				cfilter->SetInputData((vtkPolyData*)My_Obj);
				cfilter->SetExtractionModeToLargestRegion(); 
				cfilter->Update();

				vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();
				MyObj=cfilter->GetOutput();

				vtkSmartPointer<vtkPolyDataNormals> ObjNormals =  vtkSmartPointer<vtkPolyDataNormals>::New();		
				ObjNormals->SetInputData(MyObj);
				ObjNormals->ComputePointNormalsOn();
				ObjNormals->ComputeCellNormalsOn();
				//ObjNormals->AutoOrientNormalsOff();
				ObjNormals->ConsistencyOff();

				ObjNormals->Update();
				
				 vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
				 cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());
				 cleanPolyDataFilter->PieceInvariantOff();
				cleanPolyDataFilter->ConvertLinesToPointsOff();
				cleanPolyDataFilter->ConvertPolysToLinesOff();
				cleanPolyDataFilter->ConvertStripsToPolysOff();
				cleanPolyDataFilter->PointMergingOn();
			   cleanPolyDataFilter->Update();

				MyObj= cleanPolyDataFilter->GetOutput();
				std::cout << "\nVtkConnectivity new Number of points:"<<MyObj->GetNumberOfPoints()<< std::endl;
				std::cout << "VtkConnectivity new Number of cells:"<<MyObj->GetNumberOfCells()<< std::endl;	

				
				std::string newname = My_Obj->name.c_str();
				newname.append("_largest");				
				My_New_Obj = this->Mesh_PDcontainerload(MyObj, (char*)newname.c_str());
				

				std::cout << "My_New_Obj->name:"<<My_New_Obj->name<< std::endl;	

				int numpoints = My_New_Obj->GetNumberOfPoints();
				int numtriangles = My_New_Obj->GetNumberOfCells();
			My_New_Obj->selected = 0;								
				My_New_Obj->color[0]=My_Obj->color[0];
				My_New_Obj->color[1]=My_Obj->color[1];
				My_New_Obj->color[2]=My_Obj->color[2];
				My_New_Obj->color[3]= 1;


				glPushMatrix();
				glLoadMatrixf((GLfloat*) My_Obj->Mat2);
				getmatrix( My_New_Obj->Mat2);
				glPopMatrix();
				glPushMatrix();
				glLoadMatrixf((GLfloat*) My_Obj->Mat1);
				getmatrix( My_New_Obj->Mat1);
				glPopMatrix();

				
				



			}//if selected

			My_Obj = My_Obj->nextobj;
	}

		if (g_auto_delete ==1)
	{
		this->Mesh_Delete(1, -1, 0);
	}

				


 }
			

		
		
	
	
}

float CONTAINER_MESH::scalars_get_min()
{

	float my_min;
	float my_minroot;

	my_min = NULL;
	my_minroot = NULL;
	
	
	if (this->OBJECTS_ROOT != NULL)
	{
		my_minroot = this->OBJECTS_ROOT->scalars_get_min();
	}
	if (my_minroot == NULL)
	{my_min =-1;}
	else{my_min = my_minroot;}

	return my_min;
}

float CONTAINER_MESH::scalars_get_max()
{

	float my_max;
	float my_maxroot;

	my_max = NULL;
	my_maxroot = NULL;
	
	
	if (this->OBJECTS_ROOT != NULL)
	{
		my_maxroot = this->OBJECTS_ROOT->scalars_get_max();
	}
	if (my_maxroot == NULL)
	{my_max =1;}
	else{my_max = my_maxroot;}

	return my_max;
}

void CONTAINER_MESH::Fill_Holes(int tag_id, int max_size, Fl_Double_Window* fillholesWindow2)
{

	int magic_wand_over = g_magic_wand_over;
	int tag_tool = g_tag_tool;
	g_tag_tool=2;
	g_magic_wand_over =0;
	// we only fill holes on the firstly selected mesh...

	OBJECT_MESH *My_Obj = this->Get_Selected_Mesh(0);
	
	  if (My_Obj!=NULL && My_Obj->selected ==1)
	  {

		  char cpercent[10];
		Fl::check();                               // give fltk some cpu to gray out button			
		Fl_Double_Window *w = fillholesWindow2;           // access parent window				
		w->begin();    
		Fl_Progress *progress = new Fl_Progress(20,100,200,30);
		progress->minimum(0);                      // set progress range to be 0.0 ~ 1.0
		progress->maximum(1);
		progress->color(0x88888800);               // background color
		progress->selection_color(0x4444ff00);     // progress bar color
		progress->labelcolor(FL_WHITE);            // percent text color				
		w->end();				
		progress->show();
		w->redraw();
		Fl::check();  
		int percent = 0;
		int prev_percent=0;
				
		itoa(percent,cpercent,10);
		//strcat (cpercent,"%");
		progress->value((float)percent/100.0); 					  
		 progress->label(cpercent);  
		 Fl::check();  
				int tag_value;
				vtkSmartPointer<vtkIntArray> newTags = 
				vtkSmartPointer<vtkIntArray>::New();

				newTags->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				newTags->SetNumberOfTuples(My_Obj->numvert);
				vtkIntArray *currentTags;
		
				currentTags = (vtkIntArray*)My_Obj->GetPointData()->GetScalars("Tags");
				
				tag_value = tag_id;

				// first : copy all current tags into newTags
				for (int i=0;i<My_Obj->numvert;i++)	// for each vertex 
				{			
						
					if (currentTags!=NULL)
					{
						newTags->InsertTuple1(i, currentTags->GetTuple(i)[0]);
					
					}
					else
					{
						
							newTags->InsertTuple1(i, 0);
							
					}
													
				}
				
			vtkSmartPointer<vtkExtractEdges> extractEdges =
			vtkSmartPointer<vtkExtractEdges>::New();
			extractEdges->SetInputData((vtkPolyData*)My_Obj);
			extractEdges->Update();
			vtkSmartPointer<vtkPolyData> mesh = extractEdges->GetOutput();

						
			vtkSmartPointer<vtkIdList> boundary_ids =
			vtkSmartPointer<vtkIdList>::New();
			
			vtkSmartPointer<vtkIdList> connected_ids =
			vtkSmartPointer<vtkIdList>::New();

			vtkSmartPointer<vtkIntArray> boundary_nrs =
			vtkSmartPointer<vtkIntArray>::New();

			double vn[3];
			vn[0]=0;
			vn[1]=0;
			vn[2]=1;
			boundary_nrs->SetNumberOfComponents(1);
			boundary_nrs->SetNumberOfTuples(1);


			double picked_value =0;

			// Je prends comme limite les ids qui n'ont pas la couleur mais qui jouxtent des choses de cette 
			// couleur
			
			for (vtkIdType ve = 0; ve<My_Obj->numvert; ve++)
			{
				 percent = (int)(100*ve/(3*My_Obj->numvert));
				if (prev_percent!=percent)
				{
					itoa( percent, cpercent, 10);
					 strcat (cpercent,"%");
					 progress->value((float)percent/100.0); 					  
					progress->label(cpercent);  
					Fl::check();  
				}
				prev_percent=percent;
				boundary_nrs->InsertNextTuple1(-1);
				//int already =0;
				//already = My_Obj->Already_Listed(connected_ids->GetId(ve2), boundary_ids);
				if (newTags->GetTuple1(ve)!= (double)tag_value)
				{

					connected_ids= My_Obj->GetConnectedVertices(mesh, vn, picked_value, ve,-1);
				
					int is_boundary =0;
					for (vtkIdType ve2 =0; ve2 < connected_ids->GetNumberOfIds(); ve2++)
					{
						
						if (newTags->GetTuple1(connected_ids->GetId(ve2))== (double)tag_value )
						{is_boundary=1;}						
					}
					if (is_boundary ==1)
					{
						boundary_ids->InsertNextId(ve);
					}
					
				}
					
			}
		percent = 33;		
		itoa( percent, cpercent, 10);
		 strcat (cpercent,"%");
		progress->value((float)percent/100.0); 					  
		 progress->label(cpercent);  
		 Fl::check();  
			
			/*std::cout<<"We have "<<boundary_ids->GetNumberOfIds()<<" boundary ids"<<std::endl;
			for (vtkIdType ve=0;ve< boundary_ids->GetNumberOfIds(); ve++)
			{
				std::cout<<"ve:"<<ve<<", ind="<<boundary_ids->GetId(ve)<<std::endl;
			}*/


			
			 vtkIdType boundary_nr=0;

			 // Maintenant je regarde tout ces ids de bordure
			 for (vtkIdType j = 0; j<boundary_ids->GetNumberOfIds(); j++)
			 {
				 percent = 33 + (int)(100*j/(3*boundary_ids->GetNumberOfIds()));
				if (prev_percent!=percent)
				{
					itoa( percent, cpercent, 10);
					 strcat (cpercent,"%");
					 progress->value((float)percent/100.0); 					  
					progress->label(cpercent);  
					Fl::check();  
				}
				prev_percent=percent;

				 vtkIdType ve = boundary_ids->GetId(j);
				 // si ce vertex n'est pas d\E9j\E0 marqu\E9
				 //std::cout<<"Treat ve:"<<ve<<std::endl;
				 if (boundary_nrs->GetTuple1(ve) == (double)-1)
				 {
					  std::list<vtkIdType> ve_belonging_to_bd;
					  ve_belonging_to_bd.push_back(ve);
					  boundary_nrs->SetTuple1(ve, boundary_nr);
					  while(!ve_belonging_to_bd.empty()) {
							vtkIdType ve2 = ve_belonging_to_bd.front();    
							
							// std::cout<<"ve2:"<<ve2<<", j="<<j<<", boundary = "<<boundary_nr<<std::endl;										 
							// std::cout<<"ve2: boundary nr ="<<boundary_nrs->GetTuple1(ve2)<<std::endl;
							
							 ve_belonging_to_bd.pop_front();
							connected_ids= My_Obj->GetConnectedVertices(mesh, vn, picked_value, ve2,-1);
							for (vtkIdType i=0; i<connected_ids->GetNumberOfIds(); i++)
							{
								// si on est bien dans une couleur diff\E9rente de celle du tag s\E9lectionn\E9
								if(newTags->GetTuple1(connected_ids->GetId(i))!= (double)tag_value)
								{
									//on regarde si c'est un vertex qui appartient \E0 la liste des boundaries.
									int belongs_to=0;
									for (vtkIdType k=0;k<boundary_ids->GetNumberOfIds(); k++)
									{
										if (boundary_ids->GetId(k) == connected_ids->GetId(i))
										{belongs_to=1;}
									}

									//on regarde s'il a \E9t\E9 d\E9j\E0 marqu\E9 :
									if (belongs_to ==1 && boundary_nrs->GetTuple1(connected_ids->GetId(i)) == -1)
									{
										ve_belonging_to_bd.push_back(connected_ids->GetId(i));
										boundary_nrs->SetTuple1(connected_ids->GetId(i), boundary_nr);
							
										// std::cout<<"add "<<connected_ids->GetId(i)<<" to list"<<std::endl;
			
									}
								}
							}							
							
					  }

				// on avait trouv\E9 une nouvelle ligne...
				boundary_nr++;
				}

			 }


			/*std::cout<<"We have "<<boundary_nr<<" boundaries"<<std::endl;
			for (vtkIdType ve=0;ve< boundary_ids->GetNumberOfIds(); ve++)
			{
				std::cout<<"ve:"<<ve<<", ind="<<boundary_ids->GetId(ve)<<" boundary id="<<boundary_nrs->GetTuple1(boundary_ids->GetId(ve))<<std::endl;
			}*/

			/*int cpt =0;
			for (vtkIdType ve = 0; ve<My_Obj->numvert; ve++)
			{
				if (boundary_nrs->GetTuple1(ve)!= -1){cpt++;}
			}*/

		 std::cout<<"Found "<<boundary_nr<<" boundaries"<<std::endl;
		

		// check if source and target have same total amount of cubes!
		
				
				 
				//std::cout<<"progression:"<<percent<<"%\r";
				 

			 for (int i = 0; i<boundary_nr; i++)
			 {


				 percent = 66+(int)((100*i)/(3*boundary_nr));
				
				if (prev_percent!=percent)
				{
					itoa( percent, cpercent, 10);
					 strcat (cpercent,"%");
					 progress->value((float)percent/100.0); 					  
					progress->label(cpercent);  
					Fl::check();  
				}
				prev_percent=percent;
				 // we search for one single vertex belonging to this boundary
				 vtkIdType ve;
				 for (vtkIdType j = 0; j<boundary_ids->GetNumberOfIds(); j++)
				{
					 vtkIdType my_ve = boundary_ids->GetId(j);
						// si ce vertex n'est pas d\E9j\E0 marqu\E9
					 //std::cout<<"Treat ve:"<<ve<<std::endl;
					 if (boundary_nrs->GetTuple1(my_ve) == i)
					 {ve=my_ve; break;}
				 }

				// std::cout<<"ve:"<<ve<<", boundary "<<i<<std::endl;
				 // ve should be one of the vertices at the boundary.

				 vtkSmartPointer<vtkIdList> ids =
				vtkSmartPointer<vtkIdList>::New();
				vtkSmartPointer<vtkIdList> newids =
				vtkSmartPointer<vtkIdList>::New();
				vtkSmartPointer<vtkIdList> exnids =
				vtkSmartPointer<vtkIdList>::New();
						
				vtkSmartPointer<vtkIdList> oldids =
				vtkSmartPointer<vtkIdList>::New();
				vtkSmartPointer<vtkIdList> veryoldids =
				vtkSmartPointer<vtkIdList>::New();

				ids->SetNumberOfIds(0);
				ids->InsertNextId(ve);
				newids->InsertNextId(ve);
				oldids->InsertNextId(ve);
				picked_value = newTags->GetTuple1(ve);
				
				veryoldids->InsertNextId(ve);
				int cpt =0;	
				int list_changed =1;
				double vn[3];
				vn[0]=(double)0;
				vn[1]=(double)0;
				vn[2]=(double)1;

				while (list_changed == 1)						
				{							
					My_Obj->GetConnectedVertices(mesh,vn, picked_value,ids, newids, exnids,oldids,veryoldids, &list_changed);
					if (ids->GetNumberOfIds()>max_size)
					{break;}
					//std::cout<<"Tag magic wand level "<<cpt<<": list_changed="<<list_changed<<std::endl;
					cpt++;
				}
			
				//std::cout<<"Number of ids to tag:"<<ids->GetNumberOfIds()<<std::endl;
				if (ids->GetNumberOfIds()<max_size)
				{
					for (vtkIdType j=0;j<ids->GetNumberOfIds();j++)
					{
						newTags->SetTuple1(ids->GetId(j),tag_value);
					}

				}
			 }
			 w->remove(progress);     
			delete(progress); 

			newTags->SetName("Tags");		
			
			My_Obj->GetPointData()->RemoveArray("Tags");
			My_Obj->GetPointData()->AddArray(newTags);
			My_Obj->GetPointData()->SetActiveScalars("Tags");
			My_Obj->Update_RGB();
		g_magic_wand_over = magic_wand_over;
		g_tag_tool=tag_tool;
	  }

}

int CONTAINER_MESH::mround(float mnum)
{
	return (int)(mnum + 0.5);
}
void CONTAINER_MESH::Convert_RGB_To_Tags(int exact)
{
	
		OBJECT_MESH *My_Obj;
		
	if (this->OBJECTS_ROOT->OBJECTS != NULL)
 {
			
	My_Obj = this->OBJECTS_ROOT->OBJECTS;	
	
	while(My_Obj!=NULL)
	{
			
			if (My_Obj->selected ==1)
			{
				//std::cout<<"Found one object!"<<std::endl;
				 vtkUnsignedCharArray* test = (vtkUnsignedCharArray*)My_Obj->GetPointData()->GetScalars("RGB");
				 if (test !=NULL)
				{
					//std::cout<<"Test not null!"<<std::endl;
				   vtkSmartPointer<vtkUnsignedCharArray> colors =
				   vtkSmartPointer<vtkUnsignedCharArray>::New();
				   colors->SetNumberOfComponents(3);				
				   colors=(vtkUnsignedCharArray*)My_Obj->GetPointData()->GetScalars("RGB");      
				  
				vtkSmartPointer<vtkIntArray> newTags = 
				vtkSmartPointer<vtkIntArray>::New();

				newTags->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				newTags->SetNumberOfTuples(My_Obj->numvert);	


				if (exact ==0)
				{
					//std::cout<<"Non exact!"<<std::endl;

					// first : build a table of 25 first distinct colours found in RGB scalars.
				   GLfloat rgb_distinct_colors[25][4];
				   for (int i =0;i<25;i++)
				   {
						rgb_distinct_colors[i][0]=0;
						rgb_distinct_colors[i][1]=0;
						rgb_distinct_colors[i][2]=0;
						rgb_distinct_colors[i][3]=1;
				   }
				   //std::cout<<"RGB list initiated!"<<std::endl;

				   int cdistinct=0; //current number of distinct colours found
					
					for (int i=0;i<My_Obj->numvert;i++)	// for each vertex 
					{
						if (cdistinct<25)
						{
							//std::cout<<"Try to get tuple"<< i<<"!"<<std::endl;
							int cur_r =colors->GetTuple(i)[0];
							int cur_g =colors->GetTuple(i)[1];
							int cur_b =colors->GetTuple(i)[2];
							//std::cout<<"RGB "<<i<<"="<<cur_r<<","<<cur_g<<","<<cur_b<<std::endl;
							int already=0;
							for (int j=0; j<cdistinct;j++)
							{
								if ( (cur_r == mround(255*rgb_distinct_colors[j][0]))
									  &&(cur_g == mround(255*rgb_distinct_colors[j][1]))
									  &&(cur_b == mround(255*rgb_distinct_colors[j][2]))
									  )
								{
									already=1;
								}

							}
							if (already==0)
							{
								//std::cout<<"Found "<<i<<"="<<cur_r<<","<<cur_g<<","<<cur_b<<std::endl;
							
								rgb_distinct_colors[cdistinct][0]= (float)cur_r/255;
								rgb_distinct_colors[cdistinct][1]= (float)cur_g/255;
								rgb_distinct_colors[cdistinct][2]= (float)cur_b/255;
								cdistinct++;
							}
							
						
						}
						else
						{break;}


					}
					// then edit g_tag_colors[25][4] accordingly
					for (int i=0;i<cdistinct;i++)
					{
						g_tag_colors[i][0]=rgb_distinct_colors[i][0];
						g_tag_colors[i][1]=rgb_distinct_colors[i][1];
						g_tag_colors[i][2]=rgb_distinct_colors[i][2];
					}											
							
				}
				
				// now we match RGB and Tags
				for (int i=0;i<My_Obj->numvert;i++)	// for each vertex 
				{				
							int cur_r =colors->GetTuple(i)[0];
							int cur_g =colors->GetTuple(i)[1];
							int cur_b =colors->GetTuple(i)[2];
							int tag_id=0;
							//std::cout<<"Now match! RGB "<<i<<"="<<cur_r<<","<<cur_g<<","<<cur_b<<std::endl;
							
							for (int j=0; j<25;j++)
							{
								if ( (cur_r == mround(255*g_tag_colors[j][0]))
									  &&(cur_g == mround(255*g_tag_colors[j][1]))
									  &&(cur_b == mround(255*g_tag_colors[j][2])))
								{
									tag_id=j;break;
								}

							}
						//std::cout<<"vertex"<<i<<", current region:"<<currentRegions->GetTuple(i)[0]<<std::endl;					
						// get Tag id corresponding to actual RGB colour...
						//newTags->InsertTuple1(i, (int)currentRegions->GetTuple(i)[0]);																				
						newTags->InsertTuple1(i, tag_id);																				
				}	
				newTags->SetName("Tags");
				My_Obj->GetPointData()->RemoveArray("Tags");		
			   	My_Obj->GetPointData()->AddArray(newTags);
				My_Obj->GetPointData()->SetActiveScalars("Tags");								
				My_Obj->selected=0;
				My_Obj->view=1;
				}//if RGB colour scalar exists
			}//if selected

			My_Obj = My_Obj->nextobj;
	}//while				
	

 }		
}

void CONTAINER_MESH::Mesh_Tag(float x, float y, int mode)
{
	float xx, yy, zz, nx, ny, nz;
	vtkIdType ve;
	OBJECT_MESH *My_Obj;
	float mouse_vertex[3];
		
	  // contains the coordinates of the mouse pointer (=mouse vertex)
	  // assume that the mouse vertex is at the front plane of the viewing cube, i.e. z-coord= -1.0
	  mouse_vertex[0]=x;mouse_vertex[1]=y;mouse_vertex[2]= -1;
	  My_Obj = GetClosestVertex(mouse_vertex,&xx,&yy,&zz, &nx, &ny, &nz, &ve, 2);
	  //std::cout<<"Tag: mouse vertex :"<<x<<","<<y<<""<<std::endl;
	  //std::cout<<"Closest vertex :"<<xx<<","<<yy<<","<<zz<<std::endl;
	  //std::cout <<"Closest vertex ID : "<<ve<<std::endl;

	  double picked_value=0.0;
	  if (My_Obj!=NULL && My_Obj->selected ==1)
	  {

				

				int tag_value;
				vtkSmartPointer<vtkIntArray> newTags = 
				vtkSmartPointer<vtkIntArray>::New();

				newTags->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				newTags->SetNumberOfTuples(My_Obj->numvert);
				vtkIntArray *currentTags;
		
				currentTags = (vtkIntArray*)My_Obj->GetPointData()->GetScalars("Tags");
				if (mode==0){tag_value = 0;}
				else{tag_value = g_active_tag;}

				// first : copy all current tags into newTags
				for (int i=0;i<My_Obj->numvert;i++)	// for each vertex 
				{			
						if (i !=(int)ve)
						{
							if (currentTags!=NULL)
							{
								newTags->InsertTuple1(i, currentTags->GetTuple(i)[0]);
							
							}
							else
							{
								
									newTags->InsertTuple1(i, 0);
									
							}
						}
						else
						{
							newTags->InsertTuple1(i, g_active_tag);
							if (currentTags!=NULL)
							{
								picked_value=currentTags->GetTuple(i)[0];
							}
						}							
				}
				
				// Tag at some depth level.
				// if pencil is active!
				int list_changed =1;
				double vn[3];
				vn[0]=(double)nx;
				vn[1]=(double)ny;
				vn[2]=(double)nz;
				

				if (g_tag_tool ==0)
				{
					if (g_pencil_extension>0)
					{
						vtkSmartPointer<vtkExtractEdges> extractEdges =
						vtkSmartPointer<vtkExtractEdges>::New();
						extractEdges->SetInputData((vtkPolyData*)My_Obj);
						extractEdges->Update();
						vtkSmartPointer<vtkPolyData> mesh = extractEdges->GetOutput();

						
						vtkSmartPointer<vtkIdList> ids =
						vtkSmartPointer<vtkIdList>::New();
						vtkSmartPointer<vtkIdList> newids =
							vtkSmartPointer<vtkIdList>::New();
						vtkSmartPointer<vtkIdList> exnids =
						vtkSmartPointer<vtkIdList>::New();
						vtkSmartPointer<vtkIdList> oldids =
						vtkSmartPointer<vtkIdList>::New();
						vtkSmartPointer<vtkIdList> veryoldids =
						vtkSmartPointer<vtkIdList>::New();
				
						ids->InsertNextId(ve);
						newids->InsertNextId(ve);
						//oldids->InsertNextId(ve);
						//veryoldids->InsertNextId(ve);

						for (int i = 0; i<g_pencil_extension; i++)
						{
							
							My_Obj->GetConnectedVertices(mesh,vn, picked_value, ids, newids,exnids , oldids, veryoldids, &list_changed);
							if (list_changed==0){break;}
							//std::cout<<"Tag pencil level 0: list_changed="<<list_changed<<std::endl;
						}
						for (vtkIdType j=0;j<ids->GetNumberOfIds();j++)
						{
							newTags->SetTuple1(ids->GetId(j),tag_value);
						}
						
					}
				}//pencil
				else if (g_tag_tool ==1 || g_tag_tool ==2) // magic wand or bucket!
				{
					
						vtkSmartPointer<vtkExtractEdges> extractEdges =
						vtkSmartPointer<vtkExtractEdges>::New();
						extractEdges->SetInputData((vtkPolyData*)My_Obj);
						extractEdges->Update();
						vtkSmartPointer<vtkPolyData> mesh = extractEdges->GetOutput();

						
						vtkSmartPointer<vtkIdList> ids =
						vtkSmartPointer<vtkIdList>::New();
						vtkSmartPointer<vtkIdList> newids =
						vtkSmartPointer<vtkIdList>::New();
						vtkSmartPointer<vtkIdList> exnids =
						vtkSmartPointer<vtkIdList>::New();
						
						vtkSmartPointer<vtkIdList> oldids =
						vtkSmartPointer<vtkIdList>::New();
						vtkSmartPointer<vtkIdList> veryoldids =
						vtkSmartPointer<vtkIdList>::New();
						ids->InsertNextId(ve);
						newids->InsertNextId(ve);
						oldids->InsertNextId(ve);
						veryoldids->InsertNextId(ve);
						int cpt =0;
						
						while (list_changed == 1)						
						{
							
							My_Obj->GetConnectedVertices(mesh,vn, picked_value,ids, newids, exnids,oldids,veryoldids, &list_changed);
							//std::cout<<"Tag magic wand level "<<cpt<<": list_changed="<<list_changed<<std::endl;
							cpt++;
						}
						if (g_tag_tool==1)
						{
							//std::cout<<"Tag magic wand level "<<cpt<<": vertices tagged:"<<ids->GetNumberOfIds()<<std::endl;
						}
						if (g_tag_tool==2)
						{
							//std::cout<<"Flood bucket level "<<cpt<<": vertices tagged:"<<ids->GetNumberOfIds()<<std::endl;
						}
						for (vtkIdType j=0;j<ids->GetNumberOfIds();j++)
						{
							newTags->SetTuple1(ids->GetId(j),tag_value);
						}
						
					
					
				}
			
			newTags->SetName("Tags");		
			
			My_Obj->GetPointData()->RemoveArray("Tags");
			My_Obj->GetPointData()->AddArray(newTags);
			My_Obj->GetPointData()->SetActiveScalars("Tags");
			My_Obj->Update_RGB();
			
			//My_Obj->bool_changed_tags_color = 1;
	  }
		
}


void CONTAINER_MESH::Mesh_Tag_Within_Lasso(float x, float y, int lasso_x, int lasso_y, int h_screen, int w_screen, int mode)
{
	float xx, yy, zz, nx, ny, nz;
	vtkIdType ve;
	OBJECT_MESH *My_Obj;
	float mouse_vertex[3];	
	// contains the coordinates of the mouse pointer (=mouse vertex)
	// assume that the mouse vertex is at the front plane of the viewing cube, i.e. z-coord= -1.0
	mouse_vertex[0]=x;mouse_vertex[1]=y;mouse_vertex[2]= -1;
	My_Obj = GetClosestVertex(mouse_vertex,&xx,&yy,&zz, &nx, &ny, &nz, &ve, 2);
	  //std::cout<<"Tag: mouse vertex :"<<x<<","<<y<<""<<std::endl;
	  //std::cout<<"Closest vertex :"<<xx<<","<<yy<<","<<zz<<std::endl;
	  //std::cout <<"Closest vertex ID : "<<ve<<std::endl;

	  double picked_value=0.0;
	  if (My_Obj!=NULL && My_Obj->selected ==1)
	  {
				int tag_value;
				vtkSmartPointer<vtkIntArray> newTags = 
				vtkSmartPointer<vtkIntArray>::New();

				newTags->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				newTags->SetNumberOfTuples(My_Obj->numvert);
				vtkIntArray *currentTags;
		
				currentTags = (vtkIntArray*)My_Obj->GetPointData()->GetScalars("Tags");
				if (mode==0){tag_value = 0.0;}
				else{tag_value = (double)g_active_tag;}

				// first : copy all current tags into newTags
				for (int i=0;i<My_Obj->numvert;i++)	// for each vertex 
				{			
						if (i !=(int)ve)
						{
							if (currentTags!=NULL)
							{
								newTags->InsertTuple1(i, currentTags->GetTuple(i)[0]);
							
							}
							else
							{
								
									newTags->InsertTuple1(i, 0);
									
							}
						}
						else
						{
							newTags->InsertTuple1(i, g_active_tag);
							if (currentTags!=NULL)
							{
								picked_value=currentTags->GetTuple(i)[0];
							}
						}							
				}
				
				// Tag at some depth level.
				// if pencil is active!
				int list_changed =1;
				double vn[3];
				vn[0]=(double)nx;
				vn[1]=(double)ny;
				vn[2]=(double)nz;
				

				if (g_tag_tool ==0)
				{
					if (g_pencil_extension>0)
					{
						vtkSmartPointer<vtkExtractEdges> extractEdges =
						vtkSmartPointer<vtkExtractEdges>::New();
						extractEdges->SetInputData((vtkPolyData*)My_Obj);
						extractEdges->Update();
						vtkSmartPointer<vtkPolyData> mesh = extractEdges->GetOutput();

						
						vtkSmartPointer<vtkIdList> ids =
						vtkSmartPointer<vtkIdList>::New();
						vtkSmartPointer<vtkIdList> newids =
							vtkSmartPointer<vtkIdList>::New();
						vtkSmartPointer<vtkIdList> exnids =
						vtkSmartPointer<vtkIdList>::New();
						vtkSmartPointer<vtkIdList> oldids =
						vtkSmartPointer<vtkIdList>::New();
						vtkSmartPointer<vtkIdList> veryoldids =
						vtkSmartPointer<vtkIdList>::New();
				
						ids->InsertNextId(ve);
						newids->InsertNextId(ve);
						//oldids->InsertNextId(ve);
						//veryoldids->InsertNextId(ve);

						for (int i = 0; i<g_pencil_extension; i++)
						{
							
							My_Obj->GetConnectedVerticesLasso(mesh,vn, picked_value, ids, newids,exnids , oldids, veryoldids, &list_changed, lasso_x, lasso_y, h_screen, w_screen);
							if (list_changed==0){break;}
							//std::cout<<"Tag pencil level 0: list_changed="<<list_changed<<std::endl;
						}
						for (vtkIdType j=0;j<ids->GetNumberOfIds();j++)
						{
							newTags->SetTuple1(ids->GetId(j),tag_value);
						}
						
					}
				}//pencil
				else if (g_tag_tool ==1 || g_tag_tool ==2) // magic wand or bucket!
				{
					
						vtkSmartPointer<vtkExtractEdges> extractEdges =
						vtkSmartPointer<vtkExtractEdges>::New();
						extractEdges->SetInputData((vtkPolyData*)My_Obj);
						extractEdges->Update();
						vtkSmartPointer<vtkPolyData> mesh = extractEdges->GetOutput();

						
						vtkSmartPointer<vtkIdList> ids =
						vtkSmartPointer<vtkIdList>::New();
						vtkSmartPointer<vtkIdList> newids =
						vtkSmartPointer<vtkIdList>::New();
						vtkSmartPointer<vtkIdList> exnids =
						vtkSmartPointer<vtkIdList>::New();
						
						vtkSmartPointer<vtkIdList> oldids =
						vtkSmartPointer<vtkIdList>::New();
						vtkSmartPointer<vtkIdList> veryoldids =
						vtkSmartPointer<vtkIdList>::New();
						ids->InsertNextId(ve);
						newids->InsertNextId(ve);
						oldids->InsertNextId(ve);
						veryoldids->InsertNextId(ve);
						int cpt =0;
						
						while (list_changed == 1)						
						{
							
							My_Obj->GetConnectedVerticesLasso(mesh,vn, picked_value,ids, newids, exnids,oldids,veryoldids, &list_changed, lasso_x, lasso_y, h_screen, w_screen);
							//std::cout<<"Tag magic wand level "<<cpt<<": list_changed="<<list_changed<<std::endl;
							cpt++;
						}
						if (g_tag_tool==1)
						{
							//std::cout<<"Tag magic wand level "<<cpt<<": vertices tagged:"<<ids->GetNumberOfIds()<<std::endl;
						}
						if (g_tag_tool==2)
						{
							//std::cout<<"Flood bucket level "<<cpt<<": vertices tagged:"<<ids->GetNumberOfIds()<<std::endl;
						}
						for (vtkIdType j=0;j<ids->GetNumberOfIds();j++)
						{
							newTags->SetTuple1(ids->GetId(j),tag_value);
						}
						
					
					
				}
			
			newTags->SetName("Tags");		
			
			My_Obj->GetPointData()->RemoveArray("Tags");
			My_Obj->GetPointData()->AddArray(newTags);
			My_Obj->GetPointData()->SetActiveScalars("Tags");
			My_Obj->Update_RGB();
		
	  }
		
}

void CONTAINER_MESH::Merge_Tags(int source, int target)
{
	OBJECT_MESH *My_Obj;
	if (this->OBJECTS_ROOT->OBJECTS != NULL)
	{			
		My_Obj = this->OBJECTS_ROOT->OBJECTS;		
		while(My_Obj!=NULL)
		{			
			if (My_Obj->selected ==1)
			{						
				vtkSmartPointer<vtkIntArray> newTags = 
				vtkSmartPointer<vtkIntArray>::New();
				newTags->SetNumberOfComponents(1); 
				newTags->SetNumberOfTuples(My_Obj->numvert);
				vtkFloatArray *currentTags;		
				currentTags = (vtkFloatArray*)My_Obj->GetPointData()->GetScalars("Tags");
				int source_tag_value =  source;
				int target_tag_value =  target;
								
				for (int i=0;i<My_Obj->numvert;i++)	// for each vertex 
				{			
					// If scalars do not exist... then value is 0.0 (initialize	tags)
					if (currentTags==NULL)
					{
						if (source_tag_value ==0)
						{
							newTags->InsertTuple1(i, target_tag_value);
						}
						else
						{
							newTags->InsertTuple1(i, 0);					
						}
					}
					else
					{
						if (currentTags->GetTuple(i)[0] == source_tag_value)
						{					
							newTags->InsertTuple1(i, target_tag_value);
						}
						else
						{
							newTags->InsertTuple1(i, currentTags->GetTuple(i)[0]);
						}
					}																										
				}// for all vertices												
				newTags->SetName("Tags");						
				My_Obj->GetPointData()->RemoveArray("Tags");
				My_Obj->GetPointData()->AddArray(newTags);
				My_Obj->GetPointData()->SetActiveScalars("Tags");					
			}// selected
			My_Obj = My_Obj->nextobj;
		}// while		
	}// object root not null		
}// end function


void CONTAINER_MESH::Mesh_Lasso_Vertices(int h_screen, int w_screen, float mousex, float mousey, int mode)
{		
	// mode = 0 : delete mark vertices
	// mode = 1 : lasso + 
	// mode = 2 : lasso - ... which is now removed...
	glMatrix screen_mat, wc_mat;	
	
    POLYGON_VERTEX proj_screen;
	int proj_is_inside;
	int direction; //if 1, delete vertices inside polygon, else delete vertices outside...
    float vv[3]; 
	float vv1[3];
    double vvd[3];
	OBJECT_MESH *My_Obj;
	OBJECT_MESH *My_New_Obj;

  
   if (this->OBJECTS_ROOT->OBJECTS != NULL)
	{
			
		My_Obj = this->OBJECTS_ROOT->OBJECTS;	
	
		while(My_Obj!=NULL)
		{
			
			if (My_Obj->selected ==1)
			{
	

				vtkSmartPointer<vtkIntArray> newTags = 
				vtkSmartPointer<vtkIntArray>::New();

				newTags->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				newTags->SetNumberOfTuples(My_Obj->numvert);
				newTags->SetNumberOfTuples(My_Obj->numvert);
				vtkIntArray *currentTags;
		
				currentTags = (vtkIntArray*)My_Obj->GetPointData()->GetScalars("Tags");												
					
				glPushMatrix();
				glLoadIdentity();
				glMultMatrixf((GLfloat*) My_Obj->Mat2);
					glTranslated(
							My_Obj->mean[0],
							My_Obj->mean[1],
							My_Obj->mean[2]
							);	
				glMultMatrixf((GLfloat*) My_Obj->Mat1);
					glTranslated(
							-My_Obj->mean[0],
							-My_Obj->mean[1],
							-My_Obj->mean[2]
							);	
				My_Obj->get_screen_projection_matrix(screen_mat);
				My_Obj->get_world_coordinates_matrix(wc_mat);
				glPopMatrix();
		
				int i;

		
				proj_screen.x = mousex;
				proj_screen.y = mousey;
				direction = poly.POLYGON_POINT_INSIDE(proj_screen);			


			
			for (i=0;i<My_Obj->numvert;i++)	// for each vertex 
			{			
				My_Obj->GetPoint(i, vvd);
			
				vv1[0] =  (float) vvd[0];
				vv1[1] =  (float) vvd[1];
				vv1[2] =  (float) vvd[2];

				ApplyTransformation(vv1,vv,screen_mat);						
				
				proj_screen.x = (int) w_screen*(vv[0] +1 )/2;
				proj_screen.y = (int) h_screen*(vv[1] +1 )/2;

				proj_is_inside = poly.POLYGON_POINT_INSIDE(proj_screen);			
				if (direction ==0)
				{
					if (proj_is_inside ==0){proj_is_inside =1;}
					else
					{proj_is_inside =0;}
				}
				if ((vv[2]>-1.0) && vv[2]<1.0 && (proj_is_inside ==1))
				{
					//
					if (mode ==0)
					{
						newTags->InsertTuple1(i, 1);	
					}
					else if (mode ==1)
					{
						newTags->InsertTuple1(i, g_active_tag);
					}
					else 
					{
						newTags->InsertTuple1(i, 0);
					}

				}
				else
				{
					if (mode ==0 || currentTags ==NULL)
					{
						newTags->InsertTuple1(i, 0);	
					}
					else
					{
						newTags->InsertTuple1(i, currentTags->GetTuple(i)[0]);
					}
				}
								
			}
		
		
			newTags->SetName("Tags");		
			
			My_Obj->GetPointData()->RemoveArray("Tags");
			My_Obj->GetPointData()->AddArray(newTags);
			My_Obj->GetPointData()->SetActiveScalars("Tags");
			My_Obj->Update_RGB();
			
			if (mode ==0)
			{
				vtkSmartPointer<vtkThreshold> selector =
				vtkSmartPointer<vtkThreshold>::New();
				vtkSmartPointer<vtkMaskFields> scalarsOff =
				vtkSmartPointer<vtkMaskFields>::New();
				vtkSmartPointer<vtkGeometryFilter> geometry =
			    vtkSmartPointer<vtkGeometryFilter>::New();
				selector->SetInputData((vtkPolyData*)My_Obj);
				selector->SetInputArrayToProcess(0, 0, 0,
                                    vtkDataObject::FIELD_ASSOCIATION_CELLS,
                                    vtkDataSetAttributes::SCALARS);
				selector->SetAllScalars(g_tag_extraction_criterion_all);				
				selector->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "Tags");
				selector->ThresholdBetween(0.9, 1.1);
				selector->Update();
				std::cout << "\nSelector new Number of points:"<<selector->GetOutput()->GetNumberOfPoints()<< std::endl;
				std::cout << "\nSelector new Number of cells:"<<selector->GetOutput()->GetNumberOfCells()<< std::endl;	
				
				scalarsOff->SetInputData(selector->GetOutput());
				scalarsOff->CopyAttributeOff(vtkMaskFields::POINT_DATA,vtkDataSetAttributes::SCALARS);
				scalarsOff->CopyAttributeOff(vtkMaskFields::CELL_DATA, vtkDataSetAttributes::SCALARS);
				scalarsOff->Update();
				geometry->SetInputData(scalarsOff->GetOutput());
				geometry->Update();				


				


				vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();
				

				MyObj= geometry->GetOutput();
				std::cout << "\nLasso new Number of points:"<<MyObj->GetNumberOfPoints()<< std::endl;
				std::cout << "\nLasso  new Number of cells:"<<MyObj->GetNumberOfCells()<< std::endl;	
							
				if (MyObj->GetNumberOfPoints()>10)
				{
					std::string newname = My_Obj->name.c_str();
					newname.append("_lc");
					
						My_New_Obj = this->Mesh_PDcontainerload(MyObj, (char*)newname.c_str());
					
					std::cout << "My_New_Obj->name:"<<My_New_Obj->name<< std::endl;	

					int numpoints = My_New_Obj->GetNumberOfPoints();
					int numtriangles = My_New_Obj->GetNumberOfCells();
				
					My_New_Obj->selected = 0;								
					My_New_Obj->color[0]=My_Obj->color[0];
					My_New_Obj->color[1]=My_Obj->color[1];
					My_New_Obj->color[2]=My_Obj->color[2];
					My_New_Obj->color[3]= 1;


					glPushMatrix();
					glLoadMatrixf((GLfloat*) My_Obj->Mat2);
					getmatrix( My_New_Obj->Mat2);
					glPopMatrix();
					glPushMatrix();
					glLoadMatrixf((GLfloat*) My_Obj->Mat1);
					getmatrix( My_New_Obj->Mat1);
					glPopMatrix();
				}

			}
			else
			{
				// do nothing and unselect to avoid object deletion!
				if (g_tag_mode==0)
				{
					My_Obj->selected =0;
				}
			}


	}//if this is selected
	
			My_Obj = My_Obj->nextobj;
	}// while

		if (g_auto_delete ==1 && mode==0)
		{
			this->Mesh_Delete(1, -1, 0);
		}
   }// object root not null

		

}

void CONTAINER_MESH::Mesh_area_volume()
{
	

				
	// Delete everything selected 						
				this->OBJECTS_ROOT->Mesh_area_volume();							
				

}

void CONTAINER_MESH::Mesh_Delete(int mode, int numtri, float volume)	// Frees The Object (Releasing The Memory)
	{									// Frees Points
										// Delete everything selected 
// mode = 0 : delete all!
//mode = 1 num tri
// mode = 2 volume

// numtri: if = -1 : delete anyway.
// otherwise: only delete whe object=>numtri < numtri
	OBJECT_MESH * Parent;
	OBJECT_MESH * Child;
	OBJECT_MESH * SubChild;
		
	    int Ok = 1;
		int Root_found = 0;
		Parent = Child = SubChild = NULL;
		
		if (this->OBJECTS_ROOT->OBJECTS == NULL)
		{Ok = 0;}
		else
		{
			Parent = this->OBJECTS_ROOT->OBJECTS;
			
		}				
		//First : find new root object;
		while (Ok!=0)
		{	if (
					(Parent->selected==1 && Parent->view ==1)
					&& (
							((Parent->numtri<numtri || numtri==-1)&& mode==1)
							||(Parent->get_volume()<volume && mode==2)
						)
				)
			{
				if (Parent->nextobj !=NULL)
				{
					Child = Parent->nextobj;
					//@@@
					//std::cout << "\nMyObj Parent 1 Reference Count:"<<Parent->GetReferenceCount();
					Parent->Delete();
					//delete Parent;
					Parent = NULL;
					Parent = Child;	
					Parent->prevobj = NULL;
				}
				else 
				{				
					//@@@
					//std::cout << "\nParent 2 Reference Count:"<<Parent->GetReferenceCount();
					//Parent->FreeDelete();					
					Parent->Delete();// For VTK Only
					//std::cout << "\nParent 2 Reference Count After Delete:"<<Parent->GetReferenceCount();
					//Parent->FastDelete();
					//delete Parent;
					Parent = NULL;
					this->OBJECTS_ROOT->OBJECTS = NULL;
					Ok =0;
				}			 		
			}
			else //Root exists!
			{Ok = 0;
			this->OBJECTS_ROOT->OBJECTS = Parent;
			Root_found = 1;
			}
		}
		// Now delete every selected child;
		if (this->OBJECTS_ROOT->OBJECTS == NULL)
		{Ok = 0;}
		else
		{
			Ok = 1;
			Parent = this->OBJECTS_ROOT->OBJECTS;
			Child = Parent->nextobj;
			if (Child !=NULL)
			{SubChild = Child->nextobj;}
			
		}	
		//Parent can never be selected
		while (Ok !=0)
		{			
			Ok = Ok;						
			if (Child != NULL)
			{
				if (Child->selected == 1 && Child->view ==1 &&
					(
					
						//Child->numtri<numtri || numtri==-1)
						((Child->numtri<numtri || numtri==-1)&& mode==1)
						||(Child->get_volume()<volume && mode==2)
						)
					)
				{				
					//@@@
					//std::cout << "\nChild 1 Reference Count:"<<Child->GetReferenceCount();
					Child->Delete();
					//delete Child;
					Child = NULL;
					Parent->nextobj = NULL;
					if (SubChild == NULL)
					{
						Ok = 0; //fini
					}
					else
					{						

						Parent->nextobj = SubChild;
						SubChild->prevobj = Parent;
						Child = SubChild;
						if (SubChild !=NULL)
						{SubChild = SubChild->nextobj;}
					}
				}
				else
				{
					Parent = Child;
					Child = SubChild;
					if (SubChild !=NULL)
					{SubChild = SubChild->nextobj;}
				}
			}
			else
			{Ok = 0;}																							
		}//while
		
		//Now delete Log object
		OBJECT_LOG * ParentLog;
		OBJECT_LOG * ChildLog;
		OBJECT_LOG * SubChildLog;
		Ok = 1;
		Root_found = 0;
		ParentLog = ChildLog = SubChildLog = NULL;
		
		if (this->OBJECTS_ROOT->OBJECTS_LOG == NULL)
		{Ok = 0;}
		else
		{
			ParentLog = this->OBJECTS_ROOT->OBJECTS_LOG;
			
		}				
		//First : find new root object;
		while (Ok!=0)
		{	if (ParentLog->selected==1 && Parent->view ==1 && (numtri==-1))
			{
				if (ParentLog->nextobj !=NULL)
				{
					ChildLog = ParentLog->nextobj;
				
					//@@@
					//std::cout << "\nParent Log Reference Count:"<<ParentLog->GetReferenceCount();
					ParentLog->Delete();
					//delete ParentLog;
					ParentLog = NULL;
					ParentLog = ChildLog;	
					ParentLog->prevobj = NULL;
				}
				else 
				{				
					//@@@
					//std::cout << "\nParent Log 2 Reference Count:"<<ParentLog->GetReferenceCount();
					ParentLog->Delete();					
					//delete ParentLog;
					ParentLog = NULL;
					this->OBJECTS_ROOT->OBJECTS_LOG = NULL;
					Ok =0;
				}			 		
			}
			else //Root exists!
			{Ok = 0;
			this->OBJECTS_ROOT->OBJECTS_LOG = ParentLog;
			Root_found = 1;
			}
		}
		
		// Now delete every selected childLog;
		if (this->OBJECTS_ROOT->OBJECTS_LOG == NULL)
		{Ok = 0;}
		else
		{
			Ok = 1;
			ParentLog = this->OBJECTS_ROOT->OBJECTS_LOG;
			ChildLog = ParentLog->nextobj;
			if (ChildLog !=NULL)
			{SubChildLog = ChildLog->nextobj;}
			
		}	
		//Parent can never be selected
		while (Ok !=0)
		{			
			Ok = Ok;						
			if (ChildLog != NULL)
			{
				if (ChildLog->selected == 1 && (numtri==-1))
				{	//@@@
					//std::cout << "\nChild Log Reference Count:"<<ChildLog->GetReferenceCount();
					ChildLog->Delete();
					
					//delete ChildLog;
					ChildLog = NULL;
					ParentLog->nextobj = NULL;
					if (SubChildLog == NULL)
					{
						Ok = 0; //fini
					}
					else
					{						

						ParentLog->nextobj = SubChildLog;
						SubChildLog->prevobj = ParentLog;
						ChildLog = SubChildLog;
						if (SubChildLog !=NULL)
						{SubChildLog = SubChildLog->nextobj;}
					}
				}
				else
				{
					ParentLog = ChildLog;
					ChildLog = SubChildLog;
					if (SubChildLog !=NULL)
					{SubChildLog = SubChildLog->nextobj;}
				}
			}
			else
			{Ok = 0;}																							
		}//whileLog

	OBJECT_LANDMARK * ParentP;
	OBJECT_LANDMARK * ChildP;
	OBJECT_LANDMARK * SubChildP;
		
	    Ok = 1;
		Root_found = 0;
		ParentP = ChildP = SubChildP = NULL;
		
		if (this->OBJECTS_ROOT->OBJECTS_LANDMARK == NULL)
		{Ok = 0;}
		else
		{
			ParentP = this->OBJECTS_ROOT->OBJECTS_LANDMARK;
			
		}				
		//First : find new root object;
		while (Ok!=0)
		{	if (ParentP->selected==1 && (ParentP->numtri<numtri || numtri==-1))
			{
				if (ParentP->nextobj !=NULL)
				{
					ChildP = ParentP->nextobj;
					ParentP->Delete();
					//delete ParentP;
					ParentP = NULL;
					ParentP = ChildP;	
					ParentP->prevobj = NULL;
				}
				else 
				{				
					ParentP->Delete();
					//delete ParentP;
					ParentP = NULL;
					this->OBJECTS_ROOT->OBJECTS_LANDMARK = NULL;
					Ok =0;
				}			 		
			}
			else //Root exists!
			{Ok = 0;
			this->OBJECTS_ROOT->OBJECTS_LANDMARK = ParentP;
			Root_found = 1;
			}
		}
		// Now delete every selected child;
		if (this->OBJECTS_ROOT->OBJECTS_LANDMARK == NULL)
		{Ok = 0;}
		else
		{
			Ok = 1;
			ParentP = this->OBJECTS_ROOT->OBJECTS_LANDMARK;
			ChildP = ParentP->nextobj;
			if (ChildP !=NULL)
			{SubChildP = ChildP->nextobj;}
			
		}	
		//Parent can never be selected
		while (Ok !=0)
		{			
			Ok = Ok;						
			if (ChildP != NULL)
			{
				if (ChildP->selected == 1 && (ChildP->numtri<numtri || numtri==-1))
				{	
					ChildP->Delete();
					//delete ChildP;
					ChildP = NULL;
					ParentP->nextobj = NULL;
					if (SubChildP == NULL)
					{
						Ok = 0; //fini
					}
					else
					{						

						ParentP->nextobj = SubChildP;
						SubChildP->prevobj = ParentP;
						ChildP = SubChildP;
						if (SubChildP !=NULL)
						{SubChildP = SubChildP->nextobj;}
					}
				}
				else
				{
					ParentP = ChildP;
					ChildP = SubChildP;
					if (SubChildP !=NULL)
					{SubChildP = SubChildP->nextobj;}
				}
			}
			else
			{Ok = 0;}																							
		}//while Landmarks
		
		


		//Now the target landmarks
		
	    Ok = 1;
		Root_found = 0;
		ParentP = ChildP = SubChildP = NULL;
		
		if (this->OBJECTS_ROOT->OBJECTS_LANDMARK_TARGET == NULL)
		{Ok = 0;}
		else
		{
			ParentP = this->OBJECTS_ROOT->OBJECTS_LANDMARK_TARGET;
			
		}				
		//First : find new root object;
		while (Ok!=0)
		{	if (ParentP->selected==1 && (ParentP->numtri<numtri || numtri==-1))
			{
				if (ParentP->nextobj !=NULL)
				{
					ChildP = ParentP->nextobj;
					ParentP->Delete();
					//delete ParentP;
					ParentP = NULL;
					ParentP = ChildP;	
					ParentP->prevobj = NULL;
				}
				else 
				{				
					ParentP->Delete();
					//delete ParentP;
					ParentP = NULL;
					this->OBJECTS_ROOT->OBJECTS_LANDMARK_TARGET = NULL;
					Ok =0;
				}			 		
			}
			else //Root exists!
			{Ok = 0;
			this->OBJECTS_ROOT->OBJECTS_LANDMARK_TARGET = ParentP;
			Root_found = 1;
			}
		}
		// Now delete every selected child;
		if (this->OBJECTS_ROOT->OBJECTS_LANDMARK_TARGET == NULL)
		{Ok = 0;}
		else
		{
			Ok = 1;
			ParentP = this->OBJECTS_ROOT->OBJECTS_LANDMARK_TARGET;
			ChildP = ParentP->nextobj;
			if (ChildP !=NULL)
			{SubChildP = ChildP->nextobj;}
			
		}	
		//Parent can never be selected
		while (Ok !=0)
		{			
			Ok = Ok;						
			if (ChildP != NULL)
			{
				if (ChildP->selected == 1 && (ChildP->numtri<numtri || numtri==-1))
				{	ChildP->Delete();
					//delete ChildP;
					ChildP = NULL;
					ParentP->nextobj = NULL;
					if (SubChildP == NULL)
					{
						Ok = 0; //fini
					}
					else
					{						

						ParentP->nextobj = SubChildP;
						SubChildP->prevobj = ParentP;
						ChildP = SubChildP;
						if (SubChildP !=NULL)
						{SubChildP = SubChildP->nextobj;}
					}
				}
				else
				{
					ParentP = ChildP;
					ChildP = SubChildP;
					if (SubChildP !=NULL)
					{SubChildP = SubChildP->nextobj;}
				}
			}
			else
			{Ok = 0;}																							
		}//while Landmarks


		//Now the Flag landmarks
		
	    Ok = 1;
		Root_found = 0;
		ParentP = ChildP = SubChildP = NULL;
		
		if (this->OBJECTS_ROOT->OBJECTS_LANDMARK_LABELS == NULL)
		{Ok = 0;}
		else
		{
			ParentP = this->OBJECTS_ROOT->OBJECTS_LANDMARK_LABELS;
			
		}				
		//First : find new root object;
		while (Ok!=0)
		{	if (ParentP->selected==1 && (ParentP->numtri<numtri || numtri==-1))
			{
				if (ParentP->nextobj !=NULL)
				{
					ChildP = ParentP->nextobj;
					ParentP->Delete();
					//delete ParentP;
					ParentP = NULL;
					ParentP = ChildP;	
					ParentP->prevobj = NULL;
				}
				else 
				{				
					ParentP->Delete();
					//delete ParentP;
					ParentP = NULL;
					this->OBJECTS_ROOT->OBJECTS_LANDMARK_LABELS = NULL;
					Ok =0;
				}			 		
			}
			else //Root exists!
			{Ok = 0;
			this->OBJECTS_ROOT->OBJECTS_LANDMARK_LABELS = ParentP;
			Root_found = 1;
			}
		}
		// Now delete every selected child;
		if (this->OBJECTS_ROOT->OBJECTS_LANDMARK_LABELS == NULL)
		{Ok = 0;}
		else
		{
			Ok = 1;
			ParentP = this->OBJECTS_ROOT->OBJECTS_LANDMARK_LABELS;
			ChildP = ParentP->nextobj;
			if (ChildP !=NULL)
			{SubChildP = ChildP->nextobj;}
			
		}	
		//Parent can never be selected
		while (Ok !=0)
		{			
			Ok = Ok;						
			if (ChildP != NULL)
			{
				if (ChildP->selected == 1 && (ChildP->numtri<numtri || numtri==-1))
				{	ChildP->Delete();
					//delete ChildP;
					ChildP = NULL;
					ParentP->nextobj = NULL;
					if (SubChildP == NULL)
					{
						Ok = 0; //fini
					}
					else
					{						

						ParentP->nextobj = SubChildP;
						SubChildP->prevobj = ParentP;
						ChildP = SubChildP;
						if (SubChildP !=NULL)
						{SubChildP = SubChildP->nextobj;}
					}
				}
				else
				{
					ParentP = ChildP;
					ChildP = SubChildP;
					if (SubChildP !=NULL)
					{SubChildP = SubChildP->nextobj;}
				}
			}
			else
			{Ok = 0;}																							
		}//while Flag Landmarks

		
	}



void CONTAINER_MESH::rollinit_objects()
{

	this->OBJECTS_ROOT->rollinit_objects();

	
	

}


int CONTAINER_MESH::Mesh_write(OBJECT_MESH *My_Obj, std::string filename, int type, int file_type,int save_norms)
{
	// Here objects are always saved into their original position.
	//(this function is always called with the "savec network" thing


	  // File_type 0 : stl
	  // File_type 1 : vtk
      // File_type 3 : obj
	  // File_type 4 : ply

	std::string STLext (".stl");
	std::string STLext2 (".STL");
	std::string VTKext (".vtk");
	std::string VTKext2 (".VTK");
	std::string OBJext (".obj");
	std::string OBJext2 (".OBJ");
	std::string PLYext (".ply");
	std::string PLYext2 (".PLY");
	
	 
	  std::cout << "\n Function Mesh_write : "<<filename.c_str()<<"\n type = "<<type;
if (file_type==0)
{
		vtkSmartPointer<vtkSTLWriter> Writer =
		vtkSmartPointer<vtkSTLWriter>::New();
		  if (type==0)
		  {
			Writer->SetFileTypeToBinary();
		  }
		  else
		  {
			  Writer->SetFileTypeToASCII();
		  }
		  std::size_t found = filename.find(STLext);
			std::size_t found2 = filename.find(STLext2);
			if (found == std::string::npos && found2 == std::string::npos)
			{
				filename.append(".stl");

			}
	  Writer->SetFileName(filename.c_str());
	  Writer->SetInputData((vtkPolyData*)My_Obj);	
	  Writer->Write();
}
else if (file_type==1)
{
		vtkSmartPointer<vtkPolyDataWriter> Writer =
		vtkSmartPointer<vtkPolyDataWriter>::New();
		  if (type==0)
		  {
			Writer->SetFileTypeToBinary();
		  }
		  else
		  {
			  Writer->SetFileTypeToASCII();
		  }
		  std::size_t found = filename.find(VTKext);
		  std::size_t found2 = filename.find(VTKext2);
		  if (found == std::string::npos && found2 == std::string::npos)
		  {
			filename.append(".vtk");
		  }
	  Writer->SetFileName(filename.c_str());
	  	vtkSmartPointer<vtkFloatArray> norms1 = 
				vtkSmartPointer<vtkFloatArray>::New();
 	vtkSmartPointer<vtkFloatArray> norms2 = 
				vtkSmartPointer<vtkFloatArray>::New();

		norms1 = vtkFloatArray::SafeDownCast
					(My_Obj->GetPointData()->GetNormals());
		norms2 = vtkFloatArray::SafeDownCast
					(My_Obj->GetCellData()->GetNormals());

	  My_Obj->GetPointData()->SetNormals(NULL);
	  My_Obj->GetCellData()->SetNormals(NULL);

	  Writer->SetInputData((vtkPolyData*)My_Obj);	


	 Writer->Write();
	 
	  My_Obj->GetPointData()->SetNormals(norms1);
	  My_Obj->GetCellData()->SetNormals(norms2);
}	
else if (file_type==2)
{
	std::cout << "\nStart Write Obj";
		vtkSmartPointer<vtkOBJWriter> Writer =
		vtkSmartPointer<vtkOBJWriter>::New();
	 std::size_t found = filename.find(OBJext);
	  std::size_t found2 = filename.find(OBJext2);
	  if (found == std::string::npos && found2 == std::string::npos)
	  {
		filename.append(".obj");
	  }	 
	  Writer->SetFileName(filename.c_str());
	  Writer->SetInputData((vtkPolyData*)My_Obj);	
	 Writer->Write();
	 std::cout << "\nEnd Write Obj";
}	
else 
{
		vtkSmartPointer<vtkPLYWriter> Writer =
		vtkSmartPointer<vtkPLYWriter>::New();
		  if (type==0)
		  {
			Writer->SetFileTypeToBinary();
			Writer->SetDataByteOrderToLittleEndian();
			std::cout << "\nBinary Little endian";
		  }
		  else if (type==1)
		  {
			  Writer->SetFileTypeToBinary();
			  Writer->SetDataByteOrderToBigEndian();
			  std::cout << "\nBinary Big endian";

		  }
		  else 
		  {
			  Writer->SetFileTypeToASCII();
			  std::cout << "\nASCII";
		  }

 // Test if RGB scalar exists.
		  vtkUnsignedCharArray* test = (vtkUnsignedCharArray*)My_Obj->GetPointData()->GetScalars("RGB");
		  if (test !=NULL)
		  {
		   vtkSmartPointer<vtkUnsignedCharArray> colors =
			vtkSmartPointer<vtkUnsignedCharArray>::New();
			colors->SetNumberOfComponents(3);				
			colors=(vtkUnsignedCharArray*)My_Obj->GetPointData()->GetScalars("RGB");      
			colors->SetName("Colors");	  
	   		Writer->SetArrayName("Colors");
		  }
	  std::size_t found = filename.find(PLYext);
	  std::size_t found2 = filename.find(PLYext2);
	  if (found == std::string::npos && found2 == std::string::npos)
	  {
		filename.append(".ply");
	  }
	  Writer->SetFileName(filename.c_str());
	  Writer->SetInputData((vtkPolyData*)My_Obj);	
	  Writer->Write();
	  std::cout << "\nWriter should have written : "<<filename.c_str();
}	


	 return 1;
}

std::string CONTAINER_MESH::Get_Scalar_Name(int scalar)
{
	std::string Depth ("Depth");	
	std::string Max ("Maximum_Curvature");	
	std::string Min ("Minimum_Curvature");	
	std::string Gauss ("Gauss_Curvature");	
	std::string MeanC ("Mean_Curvature");	
	std::string Tags ("Tags");
	std::string Thickness ("Thickness");
	std::string NoneN("None");

	if (scalar ==0)
	{return Depth;}
	else if (scalar ==1)
	{
		return Max;
	}
	else if (scalar ==2)
	{
		return Min;
	}
	else if (scalar ==3)
	{
		return Gauss;
	}
	else if (scalar ==4)
	{
		return MeanC;
	}
	else if (scalar ==-1)
	{
		return Tags;
	}
	else if (scalar ==5)
	{
		return Thickness;
	}
	else
	{
		return NoneN;
	}
}

std::string CONTAINER_MESH::Get_Active_Scalar_Name()
{

	return this->Get_Scalar_Name(g_active_scalar);
}

int CONTAINER_MESH::Mesh_write(std::string filename, int type, int mode, int file_type,int save_norms)
{		
	  // Type 0 : Binary
	  // Mode 1 : ASCII

	  // Mode 0 : orignal position
	  // Mode 1 : moved position
	
	  // File_type 0 : stl
	  // File_type 1 : vtk
      // File_type 3 : obj
	  // File_type 4 : ply

	  
	
	std::string STLext (".stl");
	std::string STLext2 (".STL");
	std::string VTKext (".vtk");
	std::string VTKext2 (".VTK");
	std::string OBJext (".obj");
	std::string OBJext2 (".OBJ");
	std::string PLYext (".ply");
	std::string PLYext2 (".PLY");
				
	
	  vtkSmartPointer<vtkAppendPolyData> mergedObjects =  vtkSmartPointer<vtkAppendPolyData>::New();	  
	  OBJECT_MESH * My_Obj;
	  int Ok =1;

	  double v[3];
      float ve[3];
	  float vepos[3];
           
      glMatrix themat;
	  
	  if (this->OBJECTS_ROOT->OBJECTS !=NULL)
	  {My_Obj = this->OBJECTS_ROOT->OBJECTS;}
	  else {Ok = 0;}
	  
	  while (Ok!=0)
	  {
	
		if (My_Obj->selected ==1)
		{
			if (mode ==0)
			{
				mergedObjects->AddInputData ((vtkPolyData*)My_Obj);
			}
			else
			{
				glPushMatrix();	
				glLoadIdentity();
				glMultMatrixf((GLfloat*) My_Obj->Mat2);
				glMultMatrixf((GLfloat*) My_Obj->Mat1);				
				getmatrix(themat);
				vtkSmartPointer<vtkPolyData> toSave = vtkSmartPointer<vtkPolyData>::New();
				toSave->DeepCopy((vtkPolyData*)My_Obj); 
				for (int i=0;i<toSave->GetNumberOfPoints();i++) {
					// for every triangle 
					toSave->GetPoint((vtkIdType)i, v);
					ve[0]=(float)v[0];
					ve[1]=(float)v[1];
					ve[2]=(float)v[2];
					
					ApplyTransformation(ve, vepos, themat);	
					toSave->GetPoints()->SetPoint((vtkIdType) i, vepos);
				}
				mergedObjects->AddInputData (toSave);
				glPopMatrix();
			}
		}
		if (My_Obj->nextobj != NULL)
		{My_Obj = My_Obj->nextobj;}
		else
		{Ok=0;}
	  }
	  // To Do : append also logical objects
	  if ( this->OBJECTS_ROOT->OBJECTS_LOG !=NULL)
	  {
		  glPushMatrix();	
		  glLoadIdentity();
		  this->OBJECTS_ROOT->OBJECTS_LOG->MergeObjects(mergedObjects, mode);
		  glPopMatrix();
	  
	  }


	  Ok =1;
	  mergedObjects->Update();
	  if (save_norms ==0)
	  {
		mergedObjects->GetOutput()->GetPointData()->SetNormals(NULL);
		mergedObjects->GetOutput()->GetCellData()->SetNormals(NULL);
	  }
	  if (file_type ==0)
	  {
			  vtkSmartPointer<vtkSTLWriter> Writer =
			vtkSmartPointer<vtkSTLWriter>::New();
			  if (type==0)
			  {
				Writer->SetFileTypeToBinary();
			  }
			  else
			  {
				  Writer->SetFileTypeToASCII();
			  }
			// test if "extension exists!"
			//
		    std::size_t found = filename.find(STLext);
			std::size_t found2 = filename.find(STLext2);
			if (found == std::string::npos && found2 == std::string::npos)
			{
				filename.append(".stl");

			}
			ifstream file(filename.c_str());
			int overwrite=1;
			if (file)
			{
				overwrite = fl_ask("Overwrite?");
				file.close();

			}
			if (overwrite==1)
			{	
				Writer->SetFileName(filename.c_str());
				Writer->SetInputData(mergedObjects->GetOutput());
				//  stlWrite->Update();
				Writer->Write();
			}
	  }
	  
      if (file_type ==1)
	  {
			  vtkSmartPointer<vtkPolyDataWriter> Writer =
			vtkSmartPointer<vtkPolyDataWriter>::New();
			  if (type==0)
			  {
				Writer->SetFileTypeToBinary();
			  }
			  else
			  {
				  Writer->SetFileTypeToASCII();
			  }
			  std::size_t found = filename.find(VTKext);
			  std::size_t found2 = filename.find(VTKext2);
			  if (found == std::string::npos && found2 == std::string::npos)
			  {
				filename.append(".vtk");
			  }
			  			ifstream file(filename.c_str());
				int overwrite=1;
				if (file)
				{
					overwrite = fl_ask("Overwrite?");
					file.close();

				}
				if (overwrite==1)
				{	
					Writer->SetFileName(filename.c_str());
					Writer->SetInputData(mergedObjects->GetOutput());
					//  stlWrite->Update();
					Writer->Write();
				}
	  }
	
	   if (file_type==2)
		{
		vtkSmartPointer<vtkOBJWriter> Writer =
		vtkSmartPointer<vtkOBJWriter>::New();
	  std::size_t found = filename.find(OBJext);
	  std::size_t found2 = filename.find(OBJext2);
	  if (found == std::string::npos && found2 == std::string::npos)
	  {
		filename.append(".obj");
	  }
	  ifstream file(filename.c_str());
	  int overwrite=1;
	  if (file)
	  {
	 	overwrite = fl_ask("Overwrite?");
		file.close();
	  }
	  if (overwrite==1)
	  {	
		Writer->SetFileName(filename.c_str());
		Writer->SetInputData((vtkPolyData*)My_Obj);	
		Writer->Write();
	  }
}	
	if (file_type==3)
	{
		vtkSmartPointer<vtkPLYWriter> Writer =
		vtkSmartPointer<vtkPLYWriter>::New();
		  if (type==0)
		  {
			Writer->SetFileTypeToBinary();
			Writer->SetDataByteOrderToLittleEndian();
			//std::cout << "\nBinary Little endian";
		  }
		  else if (type==1)
		  {
			  Writer->SetFileTypeToBinary();
			  Writer->SetDataByteOrderToBigEndian();
			 // std::cout << "\nBinary Big endian";

		  }
		  else 
		  {
			  Writer->SetFileTypeToASCII();
			  //std::cout << "\nASCII";
		  }

		 /* // test if TAG scalar exists
			vtkFloatArray *currentTags;		
		    currentTags = (vtkFloatArray*)MyMergedObject->GetPointData()->GetScalars("Tags");
		    if (currentTags != NULL)
			{
				//std::cout<<"Current tags are not null!"<<std::endl;
				int scale_id =g_color_scale_id;
				g_color_scale_id=-1;
				GLfloat cv[4];
						
				vtkSmartPointer<vtkUnsignedCharArray> newcolors =
				vtkSmartPointer<vtkUnsignedCharArray>::New();
				newcolors->SetNumberOfComponents(3);				
				newcolors->SetNumberOfTuples(MyMergedObject->GetNumberOfPoints());
		  
				// now we creat and populate RGB and Tags
				for (int i=0;i<MyMergedObject->GetNumberOfPoints();i++)	// for each vertex 
				{			//@@@@@
					double cur_t = currentTags->GetTuple(i)[0];
					int nr,ng,nb;
					ConvertScalarToColor((float)cur_t, cv, 1);
					nr= (unsigned char)(mround(255*cv[0]));
					ng= (unsigned char)(mround(255*cv[1]));
					nb= (unsigned char)(mround(255*cv[2]));
					
					if (i<500)
					{
						//std::cout<<"nr="<<nr<<", ng="<<ng<<", nb="<<nb<<std::endl;
					}
					////newcolors->InsertTuple3(i, (unsigned char)mround(255*cv[0]),(unsigned char)mround(255*cv[1]),(unsigned char)mround(255*cv[2]));																				
					//newcolors->InsertTuple3(i, 0,0,255);																				
					newcolors->InsertTuple3(i, nr,ng,nb);																				
				}
				newcolors->SetName("RGB");
				//std::cout<<"New colors added to My Obj! file!"<<std::endl;
				
				MyMergedObject->GetPointData()->RemoveArray("RGB");		
			   	MyMergedObject->GetPointData()->AddArray(newcolors);								
				g_color_scale_id=scale_id;

		}
		*/
		  vtkPolyData *MyMergedObject = mergedObjects->GetOutput();
			
		  // Test if RGB scalar exists.
		  vtkUnsignedCharArray* test = (vtkUnsignedCharArray*)MyMergedObject->GetPointData()->GetScalars("RGB");
		  if (test !=NULL)
		  {
			 // std::cout<<"Colors found!"<<std::endl;
				
		   vtkSmartPointer<vtkUnsignedCharArray> colors =
			vtkSmartPointer<vtkUnsignedCharArray>::New();
			colors->SetNumberOfComponents(4);				
			colors=(vtkUnsignedCharArray*)MyMergedObject->GetPointData()->GetScalars("RGB");      
			
			  vtkSmartPointer<vtkUnsignedCharArray> colorsRGB =
			vtkSmartPointer<vtkUnsignedCharArray>::New();
			colorsRGB->SetNumberOfComponents(3);				
			colorsRGB->SetNumberOfTuples(MyMergedObject->GetNumberOfPoints());      
				for (int i=0;i<MyMergedObject->GetNumberOfPoints();i++)	// for each vertex 
				{			//@@@@@
					
					int nr,ng,nb;
					
					nr= colors->GetTuple(i)[0];
					ng= colors->GetTuple(i)[1];
					nb= colors->GetTuple(i)[2];
					
																						
					colorsRGB->InsertTuple3(i, nr,ng,nb);																				
				}
				colorsRGB->SetName("Colors");
				MyMergedObject->GetPointData()->AddArray(colorsRGB);								
			//colors->SetName("Colors");	  
			std::cout<<"Colors num of tuples :"<<colors->GetNumberOfTuples()<<std::endl;
			for (int i=0;i<10; i++)
			{
				//std::cout<<"RGB stuff i:"<<colors->GetTuple(i)[0]<<","<<colors->GetTuple(i)[1]<<","<<colors->GetTuple(i)[2]<<std::endl;
							//std::cout<<"RGB "<<i<<"="<<cur_r<<","<<cur_g<<","<<cur_b<<std::endl;
			}
	   		Writer->SetArrayName("Colors");
		  }

	  
	  std::size_t found = filename.find(PLYext);

	  std::size_t found2 = filename.find(PLYext2);
	  if (found == std::string::npos && found2 == std::string::npos)
	  {
		filename.append(".ply");
	  }
	  ifstream file(filename.c_str());
	  int overwrite=1;
	  if (file)
	  {
	 	overwrite = fl_ask("Overwrite?");
		file.close();
	  }
	  if (overwrite==1)
	  {	
		  Writer->SetFileName(filename.c_str());
		  //
		  Writer->SetInputData(MyMergedObject);
		  //Writer->SetInputData((vtkPolyData*)My_Obj);	
		  Writer->Write();
	  }
	  //std::cout << "\nWriter should have written : "<<filename.c_str();
	}	
	return 1;	 
}

void CONTAINER_MESH::Update_RGB()
{

	if (this->OBJECTS_ROOT != NULL)
	{
		this->OBJECTS_ROOT->Update_RGB();
	}
}

OBJECT_MESH * CONTAINER_MESH::Mesh_PDcontainerload(vtkSmartPointer<vtkPolyData> MyObj, char *MESHname)						// Loads Object From File (name)
//OBJECT_MESH * CONTAINER_MESH::Mesh_PDcontainerload(OBJECT_MESH *MyObj, char *MESHname)						// Loads Object From File (name)
{
	OBJECT_MESH		*My_Obj, *Parent;	
	float mean[3];		
	double ve[3];

	numtri = MyObj->GetNumberOfCells();
	numvert= MyObj->GetNumberOfPoints(); 
	My_Obj = new OBJECT_MESH;     
	My_Obj->DeepCopy(MyObj);  
	My_Obj->name = MESHname;

  if (this->OBJECTS_ROOT->OBJECTS != NULL)
	{							
	Parent = this->OBJECTS_ROOT->OBJECTS;
	//Search for last object in the pile of the root.
	int Ok =1;
	while (Ok != 0)
	{
		 if (Parent->nextobj !=NULL)
		{
			Parent = Parent->nextobj;		
		}
		else
		{
			Ok = 0;
		}
	}
	Parent->nextobj = My_Obj; //My_Obj = new one created
		My_Obj->prevobj = Parent;//Link parent to child
		My_Obj->nextobj = NULL;
	}
	else
	{
		this->OBJECTS_ROOT->OBJECTS = My_Obj;
		My_Obj->prevobj = NULL;
		My_Obj->nextobj = NULL;
	}
	
	My_Obj->numvert = numvert; 
	My_Obj->numtri = numtri;
	My_Obj->Mesh_init_Mat();		
		  
  	
	vtkSmartPointer<vtkIdList> points =vtkSmartPointer<vtkIdList>::New();
	//std::cout << "\nPD CONT LOAD Name : "<<My_Obj->name<< " number of tri : "<<numtri;	  
  
  mean[0]=0;
  mean[1]=0;
  mean[2]=0;
  int i;
  for ( i=0;i<numvert;i++)
    {
	  My_Obj->GetPoint(i, ve);
      mean[0]+=(float)ve[0];
      mean[1]+=(float)ve[1];
      mean[2]+=(float)ve[2];
    }

  mean[0]/=numvert;
  mean[1]/=numvert;
  mean[2]/=numvert;
  
  My_Obj->mean[0] = mean[0];
  My_Obj->mean[1] = mean[1];
  My_Obj->mean[2] = mean[2];
 
  if (g_auto_zoom ==1)
  {
	  dmean = 0;
	  for (i=0;i<numvert;i++)
		{
		 My_Obj->GetPoint(i, ve);
	    
		  dmean += sqrt 
			  ((mean[0]-(float)ve[0])*(mean[0]-(float)ve[0]) 
			  +(mean[1]-(float)ve[1])*(mean[1]-(float)ve[1])
			  +(mean[2]-(float)ve[2])*(mean[2]-(float)ve[2]));
	      
		}

	  dmean/=numvert;
	  dmean_ok = 0;
  }
 
/*
  std::cout << "\nMyObj Reference Count:"<<MyObj->GetReferenceCount();
  std::cout << "\nMy_Obj Reference Count:"<<My_Obj->GetReferenceCount();*/


  My_Obj->bool_init_buf = 0;


  My_Obj->modifTab = vtkSmartPointer<vtkFloatArray>::New();   
  My_Obj->modifTab->SetNumberOfComponents(2);
  My_Obj->modifTab->SetNumberOfTuples(4);
  //la premier colonne du tableau indique le mode actuel de l'objet
  // la deuxieme colonne du tableau indique si un changement a eu lieu
  My_Obj->modifTab->SetTuple2(0, My_Obj->selected, 0);
  My_Obj->modifTab->SetTuple2(1, g_tag_mode, 0);
  My_Obj->modifTab->SetTuple2(2, dispmode, 0);
  My_Obj->modifTab->SetTuple2(3, My_Obj->bool_changed_matrix, 0);

return My_Obj;

}

void CONTAINER_MESH::Set_Blend(int blend_value)
{
	this->OBJECTS_ROOT->Set_Blend (blend_value);

}

void CONTAINER_MESH::Compute_Global_Mean(int only_selected)
{ 
	
	// Compute center of mass of all selected objects
	this->OBJECTS_ROOT->Compute_Global_Mean(only_selected);
}

void CONTAINER_MESH::Compute_Global_MinMax()
{

	// Compute center of mass of all selected objects
	this->OBJECTS_ROOT->Compute_Global_MinMax();
}
void CONTAINER_MESH::Compute_Global_Scalar_List()
{ 
	
	this->OBJECTS_ROOT->Compute_Global_Scalar_List();
}

OBJECT_MESH* CONTAINER_MESH::Get_Selected_Mesh(int selected_nr)
{
	//return this->OBJECTS_ROOT->Get_Selected_Mesh(selected_nr);

	 OBJECT_MESH *My_Obj;
	 OBJECT_MESH *My_Return_Obj=NULL;
     int cpt=0;

	if (this->OBJECTS_ROOT->OBJECTS != NULL)
	{
			
		My_Obj = this->OBJECTS_ROOT->OBJECTS;	
	
		while(My_Obj!=NULL)
		{
			
			if (My_Obj->selected ==1)
			{
				if (cpt==selected_nr) {My_Return_Obj= My_Obj; return My_Return_Obj;}
				cpt++;										
			}//if selected

			My_Obj = My_Obj->nextobj;
		}

	}
	return NULL;
}


void CONTAINER_MESH::Compute_Name_Lists()
{ 
	
	this->OBJECTS_ROOT->Compute_Name_Lists();
}

void CONTAINER_MESH::Mesh_Select(float x1, float x2, float y1, float y2, int select_mode)
{

	
// Debut
	
	
	float xmin,xmax,ymin,ymax;
	float x1b,y1b;
	int bchange;

	xmin = x1;
	xmax = x2;
	ymin = y1;
	ymax = y2;

	if (xmin >xmax)
		{
			x1b = xmin;
			xmin = xmax;
			xmax = x1b;
		}
	if (ymin >ymax)
		{
			y1b = ymin;
			ymin = ymax;
			ymax = y1b;
		}
	 
  
		// contains the coordinates of the mouse pointer (=mouse vertex)
		// assume that the mouse vertex is at the front plane of the viewing cube, i.e. z-coord= -1.0
		
		glPushMatrix();
	    glLoadIdentity();
		this->OBJECTS_ROOT->Mesh_Select(xmin, xmax, ymin, ymax, select_mode);
		glPopMatrix();								
		bchange = 0;
		

//	}//If STL
// Fin






}



//on verifie que les objects et les landmarks sont toujours présents pour l'ICP
//bool_onlyMatching sert à le deuxieme fonctionnalité de l'icp
bool CONTAINER_MESH::ExistAlwaysMeshLandmarkICP(bool bool_onlyMatching){
	bool checkUp = false;
	int cpt = 0;
	OBJECT_MESH *object;
	OBJECT_MESH *S, *T;
	// CHECK UP OBJECT_MESH
	if (this->OBJECTS_ROOT->OBJECTS != NULL){
		object = this->OBJECTS_ROOT->OBJECTS;
		while (object != NULL)
		{
			if (object->bool_Source_used_ICP == true){
				S = object;
				cpt += 2;
			}
			if (object->bool_Target_used_ICP == true){
				T = object;
				cpt += 3;
			}
			object = object->nextobj;
		}
	}
	if (!bool_onlyMatching){

		// CHECH UP the number of Landmarks
		if (Get_Landmark_Number(0) == Get_Landmark_Number(1) && Get_Landmark_Number(1) != 0){
			cpt += 4;
		}

		if (cpt == 9){//if 1S et 1T et landmarks-->true
			checkUp = true;
		}
		else
		{
			if (S != NULL){
				S->color[0] = color_obj[0];
				S->color[1] = color_obj[1];
				S->color[2] = color_obj[2];
				S->bool_Source_used_ICP = false;
			}
			if (T != NULL){
				T->bool_Target_used_ICP = false;
			}
		}
	}
	else{
		if (cpt >= 5 && cpt != 6 && cpt != 7)///   à voir
			checkUp = true;
		else{
			if (S != NULL){
				S->color[0] = color_obj[0];
				S->color[1] = color_obj[1];
				S->color[2] = color_obj[2];
				S->bool_Source_used_ICP = false;
			}
			if (T != NULL){
				T->bool_Target_used_ICP = false;
			}
		}
	}
	return checkUp;
}


//on cherche la source dans le container utilisée dans ICP et la met à jour
void CONTAINER_MESH::Set_List_of_ObjectMesh_Coordinates(vtkPoints* new_list_Coord){

	OBJECT_MESH *object;
	if (this->OBJECTS_ROOT->OBJECTS != NULL){
		object = this->OBJECTS_ROOT->OBJECTS;
		while (object != NULL)
		{
			if (object->bool_Source_used_ICP == true){
				object->Update(new_list_Coord);
			}
			if (object->bool_Target_used_ICP == true){
				object->Update(object->GetPoints());
			}
			object = object->nextobj;
		}
	}

}

// On recherche les landmarks source pour les mettre à jour
void CONTAINER_MESH::Set_List_of_SourceLandmarks_Coordinates(vtkPoints* new_landmaks_list){
	int cpt = 0;
	OBJECT_LANDMARK *sourceLandmark = NULL;
	int indSource = 0;
	sourceLandmark = landmarkafter(indSource, 0);// landmark source

	// verifier meme nombre de landmarks
	int numberLandmark = new_landmaks_list->GetNumberOfPoints();

	if (Get_Landmark_Number(0) != numberLandmark){
		fl_alert("ERROR:\n In CONTAINER_MESH::Set_SourceLandmarksList_Coordinates(vtkPoints*) --> not the same number of landmarks.");
	}
	else{
		while (sourceLandmark != NULL && cpt < numberLandmark)  // recover the coordinates fo landmarks
		{
			double* pointLandmark = new_landmaks_list->GetPoint(cpt);
			sourceLandmark->Mat2[3][0] = pointLandmark[0];
			sourceLandmark->Mat2[3][1] = pointLandmark[1];
			sourceLandmark->Mat2[3][2] = pointLandmark[2];
			cpt++;
			/// prends les landmarks suivants
			indSource = sourceLandmark->landmark_index;
			sourceLandmark = landmarkafter(indSource, 0);
		}
	}


}


// recover the source and target landmarks in the container
bool CONTAINER_MESH::RecoverLandmarks(vtkSmartPointer< vtkPoints > landmarks_list_source, vtkSmartPointer< vtkPoints > landmarks_list_target){

	if ((Get_Landmark_Number(0) != Get_Landmark_Number(1)))
	{
		fl_alert("ERROR:: RecoverLandmarks : not the same number of landmarks.");
		return false;
	}
	else{

		landmarks_list_source->SetNumberOfPoints(this->Get_Landmark_Number(0));
		landmarks_list_target->SetNumberOfPoints(this->Get_Landmark_Number(1));
		glMatrix wc_mat;

		float vv[3], vv1[3];
		int cpt = 0;
		OBJECT_LANDMARK *sourceLandmark = NULL;
		OBJECT_LANDMARK *targetLandmark = NULL;
		int indSource = 0;
		int indTarget = 0;
		sourceLandmark = landmarkafter(indSource, 0);// landmark source
		targetLandmark = landmarkafter(indTarget, 1);// landmark target
		while (sourceLandmark != NULL)  // recover the coordinates fo landmarks
		{
			sourceLandmark->get_world_coordinates_matrix(wc_mat);
			vv[0] = 0;
			vv[1] = 0;
			vv[2] = 0;
			ApplyTransformation(vv, vv1, wc_mat);
			landmarks_list_source->SetPoint((vtkIdType)cpt, vv1);// id , coord
			/// prends les landmarks suivants source
			indSource = sourceLandmark->landmark_index;
			sourceLandmark = landmarkafter(indSource, 0);


			if (targetLandmark != NULL){
				targetLandmark->get_world_coordinates_matrix(wc_mat);
				vv[0] = 0;
				vv[1] = 0;
				vv[2] = 0;
				ApplyTransformation(vv, vv1, wc_mat);
				landmarks_list_target->SetPoint((vtkIdType)cpt, vv1);
				/// prends les landmarks suivants target
				indTarget = targetLandmark->landmark_index;
				targetLandmark = landmarkafter(indTarget, 1);
			}
			cpt++;


		}
		return true;
	}
}

//Mise à jour de la normale d'une objet mesh.
void CONTAINER_MESH::Update_Normal(vtkPolyData* My_Output){
	// Update Normal of object mesh points
	vtkSmartPointer<vtkPolyDataNormals> ObjNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
	ObjNormals->SetInputData(My_Output);
	ObjNormals->ComputePointNormalsOn();
	ObjNormals->ComputeCellNormalsOn();
	ObjNormals->ConsistencyOff();

	ObjNormals->Update();

	vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
	cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());
	cleanPolyDataFilter->PieceInvariantOff();
	cleanPolyDataFilter->ConvertLinesToPointsOff();
	cleanPolyDataFilter->ConvertPolysToLinesOff();
	cleanPolyDataFilter->ConvertStripsToPolysOff();
	cleanPolyDataFilter->PointMergingOn();
	cleanPolyDataFilter->Update();

	My_Output = cleanPolyDataFilter->GetOutput();
	std::cout << "\nVtkConnectivity new Number of points:" << My_Output->GetNumberOfPoints() << std::endl;
	std::cout << "VtkConnectivity new Number of cells:" << My_Output->GetNumberOfCells() << std::endl;
}

//On recupère les coordonnées des points de l'objet déplacé dans l'affichage
void CONTAINER_MESH::Recover_PositionObject_in_PolyData(const OBJECT_MESH* My_Input, vtkSmartPointer<vtkPolyData> My_Out){
	glMatrix themat;
	double *v;
	float ve[3];
	float vepos[3];
	// recupère les deux matrices de l'objet
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixf((GLfloat*)My_Input->Mat2);
	glMultMatrixf((GLfloat*)My_Input->Mat1);
	getmatrix(themat);

	// avec la mise a jour du deplacement.
	for (int i = 0; i<My_Out->GetNumberOfPoints(); i++) {
		// for every triangle 
		v = My_Out->GetPoint((vtkIdType)i);
		ve[0] = (float)v[0];
		ve[1] = (float)v[1];
		ve[2] = (float)v[2];

		ApplyTransformation(ve, vepos, themat); // applique la matrice de déplacement aux points
		My_Out->GetPoints()->SetPoint((vtkIdType)i, vepos);
	}
	glPopMatrix();
}


//On active la couleur initiale de l'objet sélectionné
void CONTAINER_MESH::Active_InitRGB(){
	OBJECT_MESH *object;
	if (this->OBJECTS_ROOT->OBJECTS != NULL){
		object = this->OBJECTS_ROOT->OBJECTS;
		while (object != NULL)
		{
			if (object->selected == 1)
				if (object->GetPointData()->GetScalars("Init_RGB") != NULL){
					cout << "int RGB : ";
					vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
					colors->SetNumberOfComponents(4);
					colors->SetNumberOfTuples(numvert);
					colors ->DeepCopy((vtkUnsignedCharArray*)object->GetPointData()->GetScalars("Init_RGB"));
					
					colors->SetName("RGB");
					object->GetPointData()->RemoveArray("RGB");
					object->GetPointData()->AddArray(colors);
					cout << "fait" << endl;				
					object->selected = 0;
					object->bool_changed_init_color = 1;
				}
			object = object->nextobj;
		}
	}

}


