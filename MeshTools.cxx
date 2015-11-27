#include <FL/Fl_Progress.H>

//
//
// MeshTools class implementation 
//

#include <GL/glew.h>
#include <FL/gl.h>
#include "MeshTools.h"
const long lNAME_SIZE = 1024; 
//#include <gl/gl.h>
#include <math.h>
//#include <time.h>
#include "CDataFile.h"
#include <FL/filename.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/fl_ask.H>
#include <set>
#include <iostream>

using namespace std;
#define PACKET_SIZE  1024
#include <stdio.h>

#ifdef WIN32
#include "stdafx.h"
#endif

#include <vtkPolyData.h>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkSTLReader.h>
#include <vtkPLYReader.h>
#include <vtkOBJReader.h>
#include <vtkSmartPointer.h>
#include <vtkCellData.h>
#include <vtkDataSet.h>
#include <vtkPolyData.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkMath.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkIdList.h>














char* MeshTools::GetVersion()
	{
		return Version;
	}
	

void MeshTools::Hierachy() //Print object hierachy
{Cont_Mesh.Hierarchy();}


void MeshTools::get_object_names_and_view_values(std::vector<std::string>& object_names, std::vector<int>& object_view_values)
{
	Cont_Mesh.get_object_names_and_view_values(object_names, object_view_values);
}
void MeshTools::set_object_view_values( std::vector<int>&object_new_view_values)
{
	Cont_Mesh.set_object_view_values(object_new_view_values);
}

float MeshTools::Get_Optimal_FOV_Depth()
{
	float tf = getTranslationFactor();
	//Returns the number of pixels 1 mm represents on the screen;
	float px=1;
	if (tf>0)
	{
		px = 1/tf;
	}
	struct {GLint x, y, width, height;} viewport;	 
	glGetIntegerv(GL_VIEWPORT, &viewport.x);
    float opt_depth= 2*px*(float)(viewport.width+viewport.height); 
	// we see approx 4 times as deep as the width or height of the field of view
	//std::cout<<"Opt depth = "<<opt_depth<<"mm"<<std::endl;
	return opt_depth;
}
std::string MeshTools::get_matrix(float m1[16], float m2[16])
{
	std::string Name="";
	OBJECT_MESH * My_Obj;
	OBJECT_LOG * My_Log;

	int found;

		int Ok = 1;
		if (Cont_Mesh.OBJECTS_ROOT->OBJECTS != NULL)
		{
			My_Obj = Cont_Mesh.OBJECTS_ROOT->OBJECTS;
		}
		else
		{	Ok = 0;}

		while (Ok !=0)
		{
			if (My_Obj->selected ==1)
			{
				found =1;
				Ok = 0;
			}
			else
			{
				if (My_Obj->nextobj != NULL)
				{My_Obj = My_Obj->nextobj ;}
				else
				{Ok = 0;}				
			}			
		}
		

		if (found ==1)
		{

			m1[0] = My_Obj->Mat1[0][0];
			m1[1] = My_Obj->Mat1[0][1];
			m1[2] = My_Obj->Mat1[0][2];
			m1[3] = My_Obj->Mat1[0][3];
			m1[4] = My_Obj->Mat1[1][0];
			m1[5] = My_Obj->Mat1[1][1];
			m1[6] = My_Obj->Mat1[1][2];
			m1[7] = My_Obj->Mat1[1][3];
			m1[8] = My_Obj->Mat1[2][0];
			m1[9] = My_Obj->Mat1[2][1];
			m1[10] = My_Obj->Mat1[2][2];
			m1[11] = My_Obj->Mat1[2][3];
			m1[12] = My_Obj->Mat1[3][0];
			m1[13] = My_Obj->Mat1[3][1];
			m1[14] = My_Obj->Mat1[3][2];
			m1[15] = My_Obj->Mat1[3][3];
			
			
			m2[0] = My_Obj->Mat2[0][0];
			m2[1] = My_Obj->Mat2[0][1];
			m2[2] = My_Obj->Mat2[0][2];
			m2[3] = My_Obj->Mat2[0][3];
			m2[4] = My_Obj->Mat2[1][0];
			m2[5] = My_Obj->Mat2[1][1];
			m2[6] = My_Obj->Mat2[1][2];
			m2[7] = My_Obj->Mat2[1][3];
			m2[8] = My_Obj->Mat2[2][0];
			m2[9] = My_Obj->Mat2[2][1];
			m2[10] = My_Obj->Mat2[2][2];
			m2[11] = My_Obj->Mat2[2][3];
			m2[12] = My_Obj->Mat2[3][0];
			m2[13] = My_Obj->Mat2[3][1];
			m2[14] = My_Obj->Mat2[3][2];
			m2[15] = My_Obj->Mat2[3][3];

			Name = My_Obj->name.c_str();
									
			return Name;
		
		
		}
		else
		{
			Ok = 1;
			if (Cont_Mesh.OBJECTS_ROOT->OBJECTS_LOG != NULL)
			{
				My_Log = Cont_Mesh.OBJECTS_ROOT->OBJECTS_LOG;
			}	
			else
			{	Ok = 0;}
			while (Ok !=0)
			{
				if (My_Log->selected ==1)
				{
					found =1;
					Ok = 0;
				}
				else
				{
					if (My_Log->nextobj != NULL)
					{My_Log = My_Log->nextobj ;}
					else
					{Ok = 0;}				
				}			
			}
			

			if (found ==1)
			{

				m1[0] = My_Log->Mat1[0][0];
				m1[1] = My_Log->Mat1[0][1];
				m1[2] = My_Log->Mat1[0][2];
				m1[3] = My_Log->Mat1[0][3];
				m1[4] = My_Log->Mat1[1][0];
				m1[5] = My_Log->Mat1[1][1];
				m1[6] = My_Log->Mat1[1][2];
				m1[7] = My_Log->Mat1[1][3];
				m1[8] = My_Log->Mat1[2][0];
				m1[9] = My_Log->Mat1[2][1];
				m1[10] = My_Log->Mat1[2][2];
				m1[11] = My_Log->Mat1[2][3];
				m1[12] = My_Log->Mat1[3][0];
				m1[13] = My_Log->Mat1[3][1];
				m1[14] = My_Log->Mat1[3][2];
				m1[15] = My_Log->Mat1[3][3];
				
				
				m2[0] = My_Log->Mat2[0][0];
				m2[1] = My_Log->Mat2[0][1];
				m2[2] = My_Log->Mat2[0][2];
				m2[3] = My_Log->Mat2[0][3];
				m2[4] = My_Log->Mat2[1][0];
				m2[5] = My_Log->Mat2[1][1];
				m2[6] = My_Log->Mat2[1][2];
				m2[7] = My_Log->Mat2[1][3];
				m2[8] = My_Log->Mat2[2][0];
				m2[9] = My_Log->Mat2[2][1];
				m2[10] = My_Log->Mat2[2][2];
				m2[11] = My_Log->Mat2[2][3];
				m2[12] = My_Log->Mat2[3][0];
				m2[13] = My_Log->Mat2[3][1];
				m2[14] = My_Log->Mat2[3][2];
				m2[15] = My_Log->Mat2[3][3];
				Name = My_Log->name.c_str();									
				return Name;
													
				
			
			
			}//Fin found ==1
		
		}//Fin else
		

	
	
	return Name;

}

std::string MeshTools::get_name_of_first_selected_obj()
{
	std::string Name="";
	OBJECT_MESH * My_Obj;
	OBJECT_LOG * My_Log;

	int found;

		int Ok = 1;
		if (Cont_Mesh.OBJECTS_ROOT->OBJECTS != NULL)
		{
			My_Obj = Cont_Mesh.OBJECTS_ROOT->OBJECTS;
		}
		else
		{	Ok = 0;}

		while (Ok !=0)
		{
			if (My_Obj->selected ==1)
			{
				found =1;
				Ok = 0;
			}
			else
			{
				if (My_Obj->nextobj != NULL)
				{My_Obj = My_Obj->nextobj ;}
				else
				{Ok = 0;}				
			}			
		}
		

		if (found ==1)
		{

		

			Name = My_Obj->name.c_str();
									
			return Name;
		
		
		}
		else
		{
			Ok = 1;
			if (Cont_Mesh.OBJECTS_ROOT->OBJECTS_LOG != NULL)
			{
				My_Log = Cont_Mesh.OBJECTS_ROOT->OBJECTS_LOG;
			}	
			else
			{	Ok = 0;}
			while (Ok !=0)
			{
				if (My_Log->selected ==1)
				{
					found =1;
					Ok = 0;
				}
				else
				{
					if (My_Log->nextobj != NULL)
					{My_Log = My_Log->nextobj ;}
					else
					{Ok = 0;}				
				}			
			}
			

			if (found ==1)
			{

			
				Name = My_Log->name.c_str();									
				return Name;																							
			}//Fin found ==1		
		}//Fin else				
	return Name;

}

int MeshTools::set_matrix(float m1[16], float m2[16],  int mode)
{
	//mode = 0 : only for first selected object
	//mode = 1 : for all selected objects
	OBJECT_MESH * My_Obj;	
	OBJECT_LOG * My_Log;
	int found =0;
	
		int Ok = 1;
		if (Cont_Mesh.OBJECTS_ROOT->OBJECTS != NULL)
		{
			My_Obj = Cont_Mesh.OBJECTS_ROOT->OBJECTS;
		}
		else
		{	Ok = 0;}
		
		while (Ok !=0)
		{
			
			if (My_Obj->selected ==1)
			{
			

				if (mode ==0)
				{

					found =1;
					Ok = 0;
					
				}
				else
				{

					// change matrix 
					My_Obj->bool_changed_matrix = 1;

					My_Obj->Mat1[0][0]= m1[0];
					My_Obj->Mat1[0][1]= m1[1];
					My_Obj->Mat1[0][2]= m1[2];
					My_Obj->Mat1[0][3]= m1[3];
					My_Obj->Mat1[1][0]= m1[4];
					My_Obj->Mat1[1][1]= m1[5];
					My_Obj->Mat1[1][2]= m1[6];
					My_Obj->Mat1[1][3]= m1[7];
					My_Obj->Mat1[2][0]= m1[8];
					My_Obj->Mat1[2][1]= m1[9];
					My_Obj->Mat1[2][2]= m1[10];
					My_Obj->Mat1[2][3]= m1[11];
					My_Obj->Mat1[3][0]= m1[12];
					My_Obj->Mat1[3][1]= m1[13];
					My_Obj->Mat1[3][2]= m1[14];
					My_Obj->Mat1[3][3]= m1[15];
					
					
					My_Obj->Mat2[0][0]= m2[0];
					My_Obj->Mat2[0][1]= m2[1];
					My_Obj->Mat2[0][2]= m2[2];
					My_Obj->Mat2[0][3]= m2[3];
					My_Obj->Mat2[1][0]= m2[4];
					My_Obj->Mat2[1][1]= m2[5];
					My_Obj->Mat2[1][2]= m2[6];
					My_Obj->Mat2[1][3]= m2[7];
					My_Obj->Mat2[2][0]= m2[8];
					My_Obj->Mat2[2][1]= m2[9];
					My_Obj->Mat2[2][2]= m2[10];
					My_Obj->Mat2[2][3]= m2[11];
					My_Obj->Mat2[3][0]= m2[12];
					My_Obj->Mat2[3][1]= m2[13];
					My_Obj->Mat2[3][2]= m2[14];
					My_Obj->Mat2[3][3]= m2[15];
					if (My_Obj->nextobj != NULL)
					{
						My_Obj = My_Obj->nextobj ;
						
					}
						else
						{Ok = 0;}	
					}

				
			}
			else
			{
				if (My_Obj->nextobj != NULL)
				{
					My_Obj = My_Obj->nextobj ;
				
				}
				else
				{Ok = 0;}				
			}			
		}
		
		
		if (found ==1 && mode==0)
		{
			My_Obj->bool_changed_matrix = 1;

			My_Obj->Mat1[0][0]= m1[0];
			My_Obj->Mat1[0][1]= m1[1];
			My_Obj->Mat1[0][2]= m1[2];
			My_Obj->Mat1[0][3]= m1[3];
			My_Obj->Mat1[1][0]= m1[4];
			My_Obj->Mat1[1][1]= m1[5];
			My_Obj->Mat1[1][2]= m1[6];
			My_Obj->Mat1[1][3]= m1[7];
			My_Obj->Mat1[2][0]= m1[8];
			My_Obj->Mat1[2][1]= m1[9];
			My_Obj->Mat1[2][2]= m1[10];
			My_Obj->Mat1[2][3]= m1[11];
			My_Obj->Mat1[3][0]= m1[12];
			My_Obj->Mat1[3][1]= m1[13];
			My_Obj->Mat1[3][2]= m1[14];
			My_Obj->Mat1[3][3]= m1[15];
			
			
			My_Obj->Mat2[0][0]= m2[0];
			My_Obj->Mat2[0][1]= m2[1];
			My_Obj->Mat2[0][2]= m2[2];
			My_Obj->Mat2[0][3]= m2[3];
			My_Obj->Mat2[1][0]= m2[4];
			My_Obj->Mat2[1][1]= m2[5];
			My_Obj->Mat2[1][2]= m2[6];
			My_Obj->Mat2[1][3]= m2[7];
			My_Obj->Mat2[2][0]= m2[8];
			My_Obj->Mat2[2][1]= m2[9];
			My_Obj->Mat2[2][2]= m2[10];
			My_Obj->Mat2[2][3]= m2[11];
			My_Obj->Mat2[3][0]= m2[12];
			My_Obj->Mat2[3][1]= m2[13];
			My_Obj->Mat2[3][2]= m2[14];
			My_Obj->Mat2[3][3]= m2[15];
			return 1;
		
		
		}
		else
		{
			Ok = 1;
			if (Cont_Mesh.OBJECTS_ROOT->OBJECTS_LOG != NULL)
			{
				My_Log = Cont_Mesh.OBJECTS_ROOT->OBJECTS_LOG;
			}
			else
			{	Ok = 0;}

			while (Ok !=0)
			{
				if (My_Log->selected ==1)
				{

					if (mode ==0)
					{
						found =1;
						Ok=0;
					}
					else
					{
						My_Log->Mat1[0][0]= m1[0];
						My_Log->Mat1[0][1]= m1[1];
						My_Log->Mat1[0][2]= m1[2];
						My_Log->Mat1[0][3]= m1[3];
						My_Log->Mat1[1][0]= m1[4];
						My_Log->Mat1[1][1]= m1[5];
						My_Log->Mat1[1][2]= m1[6];
						My_Log->Mat1[1][3]= m1[7];
						My_Log->Mat1[2][0]= m1[8];
						My_Log->Mat1[2][1]= m1[9];
						My_Log->Mat1[2][2]= m1[10];
						My_Log->Mat1[2][3]= m1[11];
						My_Log->Mat1[3][0]= m1[12];
						My_Log->Mat1[3][1]= m1[13];
						My_Log->Mat1[3][2]= m1[14];
						My_Log->Mat1[3][3]= m1[15];
						
						
						My_Log->Mat2[0][0]= m2[0];
						My_Log->Mat2[0][1]= m2[1];
						My_Log->Mat2[0][2]= m2[2];
						My_Log->Mat2[0][3]= m2[3];
						My_Log->Mat2[1][0]= m2[4];
						My_Log->Mat2[1][1]= m2[5];
						My_Log->Mat2[1][2]= m2[6];
						My_Log->Mat2[1][3]= m2[7];
						My_Log->Mat2[2][0]= m2[8];
						My_Log->Mat2[2][1]= m2[9];
						My_Log->Mat2[2][2]= m2[10];
						My_Log->Mat2[2][3]= m2[11];
						My_Log->Mat2[3][0]= m2[12];
						My_Log->Mat2[3][1]= m2[13];
						My_Log->Mat2[3][2]= m2[14];
						My_Log->Mat2[3][3]= m2[15];
						if (My_Log->nextobj != NULL)
						{My_Log = My_Log->nextobj ;
						}	
						else
							{Ok = 0;}
						}
					
				}
				else
				{
					if (My_Log->nextobj != NULL)
					{My_Log = My_Log->nextobj ;
					}
					else
					{Ok = 0;}				
				}			
			}
			

			if (found ==1 && mode==0)
			{


				My_Log->Mat1[0][0]= m1[0];
				My_Log->Mat1[0][1]= m1[1];
				My_Log->Mat1[0][2]= m1[2];
				My_Log->Mat1[0][3]= m1[3];
				My_Log->Mat1[1][0]= m1[4];
				My_Log->Mat1[1][1]= m1[5];
				My_Log->Mat1[1][2]= m1[6];
				My_Log->Mat1[1][3]= m1[7];
				My_Log->Mat1[2][0]= m1[8];
				My_Log->Mat1[2][1]= m1[9];
				My_Log->Mat1[2][2]= m1[10];
				My_Log->Mat1[2][3]= m1[11];
				My_Log->Mat1[3][0]= m1[12];
				My_Log->Mat1[3][1]= m1[13];
				My_Log->Mat1[3][2]= m1[14];
				My_Log->Mat1[3][3]= m1[15];
				
				
				My_Log->Mat2[0][0]= m2[0];
				My_Log->Mat2[0][1]= m2[1];
				My_Log->Mat2[0][2]= m2[2];
				My_Log->Mat2[0][3]= m2[3];
				My_Log->Mat2[1][0]= m2[4];
				My_Log->Mat2[1][1]= m2[5];
				My_Log->Mat2[1][2]= m2[6];
				My_Log->Mat2[1][3]= m2[7];
				My_Log->Mat2[2][0]= m2[8];
				My_Log->Mat2[2][1]= m2[9];
				My_Log->Mat2[2][2]= m2[10];
				My_Log->Mat2[2][3]= m2[11];
				My_Log->Mat2[3][0]= m2[12];
				My_Log->Mat2[3][1]= m2[13];
				My_Log->Mat2[3][2]= m2[14];
				My_Log->Mat2[3][3]= m2[15];

				return 1;
				
			
			
			}
		
		
		}//Fin else
	
	return 0;

}

int MeshTools::set_name(std::string Name, int mode)
{

	//mode = 0 only for first selected object
	//mode = 1 : for all selected objects
		OBJECT_MESH * My_Obj;	
		OBJECT_MESH * My_Obj_Name_To_Change = NULL;	
		int found =0;
	
		int Ok = 1;
		int nfound=0;
		if (Cont_Mesh.OBJECTS_ROOT->OBJECTS != NULL)
		{
			My_Obj = Cont_Mesh.OBJECTS_ROOT->OBJECTS;
		}
		else
		{	Ok = 0;}
		
		if (Name == ""){
			fl_alert("No name?");
		}
		else{

			while (Ok != 0)
			{

				if (My_Obj->selected == 1 && My_Obj->view == 1)
				{


					if (nfound == 0)
					{
						My_Obj_Name_To_Change = My_Obj;
						nfound = 1;
					}
					if (mode == 0)
					{
						Ok = 0;
					}
					found = 1;
					if (mode == 1)
					{
						My_Obj->name = Name.c_str();
						My_Obj->selected = 0;
					}

				}
				else
				{
					if (My_Obj->nextobj != NULL)
					{
						My_Obj = My_Obj->nextobj;
					}
					else
					{
						Ok = 0;
					}
				}
			}


			if (nfound == 1 && mode == 0)
			{
				if (My_Obj_Name_To_Change != NULL)
				{

					string s = Name;
					CheckingName(&Name);

					if (s != My_Obj_Name_To_Change->name)
						if (Name != s)// existe dejà
							fl_alert("the name already exists!");
						else
							My_Obj_Name_To_Change->name = Name.c_str();

					My_Obj_Name_To_Change->selected = 0;

					return 1;
				}
			}

	}
	return 0;

}

float MeshTools::getTranslationFactor()
{
	//Returns the number of pixels 1 mm represents on the screen;
	glMatrix mat;
	float p1[3], p2[3], p1b[3],p2b[3];
	float nbpix, dist;
	p1[0] = p1[1] =p1[2] =0;
	p2[0] = 0; p2[1] =1;p2[2] =0;
	get_projection_matrix(mat);
	ApplyTransformation(p1,p1b,mat);
	ApplyTransformation(p2,p2b,mat);
	dist = sqrt ((p1b[0]-p2b[0])*(p1b[0]-p2b[0]) + (p1b[1]-p2b[1])*(p1b[1]-p2b[1]) + (p1b[2]-p2b[2])*(p1b[2]-p2b[2]));
	nbpix = dist * h()/2;
	return nbpix;
	
	

}

int MeshTools::Get_mode_cam_centre_of_mass()
{
	return g_mode_cam_centre_of_mass;
}

int MeshTools::Get_Display_All()
{
	return g_display_all;
}

void MeshTools::Set_Display_All(int all)
{
	g_display_all = all;
}

int MeshTools::Get_Active_Scalar()
{
	return  g_active_scalar;
}
 
void MeshTools::Remove_Scalar(int scalar)
{
	Cont_Mesh.Remove_Scalar(scalar);
}

void MeshTools::Initialize_Scalar(int scalar)
{
	Cont_Mesh.Initialize_Scalar(scalar);
}
void MeshTools::Set_Active_Scalar(int active_scalar)
{
	if(g_active_scalar != active_scalar)
	{
		g_active_scalar = active_scalar;
		Cont_Mesh.Set_Active_Scalar();
		
	}
	Cont_Mesh.Update_RGB();
	
}
void MeshTools::Set_Active_Tag(int active_tag)
{
	if (g_active_tag>=0 && g_active_tag<25)
	{
		g_active_tag = active_tag;
	}
	else
	{
		g_active_tag=1;
	}
	
}
int MeshTools::Get_Active_Tag()
{
	if (g_active_tag>=0 && g_active_tag<25)
	{
		return g_active_tag;
	}
	else
	{
		return 1;
	}
	
}
void MeshTools::Compute_Name_Lists()
{
	g_selected_names.clear();
	g_distinct_selected_names.clear();
	this->Cont_Mesh.Compute_Name_Lists();
}
void MeshTools::Compute_Global_Scalar_List()
{ 
	// Compute list of existing scalars

	//std::cout<<"Clear list"<<std::endl;
	g_scalar_list.clear();
	g_scalar_list_selected.clear();
	//std::cout<<"List cleared"<<std::endl;
	this->Cont_Mesh.Compute_Global_Scalar_List();
}

std::vector<std::string> MeshTools::Get_Selected_Names_List()
{
	this->Compute_Name_Lists();
	return g_selected_names;
}


std::vector<std::string> MeshTools::Get_Scalar_List()
{
	return g_scalar_list;
}

void MeshTools::Print_Distinct_Selected_Names_List()
{ 
	// Print list of names of selected objects scalars	
	this->Compute_Name_Lists();
	std::cout<<std::endl<<"-------------------"<<std::endl;
	if (g_distinct_selected_names.size()==0)
	{
		std::cout<<"No distinct selected object"<<std::endl;
	}
	else
	{
		if (g_distinct_selected_names.size()==1)
		{
			std::cout<<"Found 1 distinct selected object:"<<std::endl;
		}
		else
		{
			std::cout<<"Found "<<g_distinct_selected_names.size()<<" distinct selected objects:"<<std::endl;
		}

		
	}
	for(int t=0;t<g_distinct_selected_names.size();++t){
		std::cout<<g_distinct_selected_names.at(t)<<std::endl;

         }

	std::cout<<std::endl<<"-------------------"<<std::endl;
		
}
void MeshTools::Print_Selected_Names_List()
{ 
	// Print list of names of selected objects scalars	
	this->Compute_Name_Lists();
	std::cout<<std::endl<<"-------------------"<<std::endl;
	if (g_selected_names.size()==0)
	{
		std::cout<<"No selected object"<<std::endl;
	}
	else
	{
		if (g_selected_names.size()==1)
		{
			std::cout<<"Found 1 selected object:"<<std::endl;
		}
		else
		{
			std::cout<<"Found "<<g_selected_names.size()<<" selected objects:"<<std::endl;
		}

		
	}
	for(int t=0;t<g_selected_names.size();++t){
		std::cout<<g_selected_names.at(t)<<std::endl;

         }

	std::cout<<std::endl<<"-------------------"<<std::endl;
		
}

void MeshTools::Print_Global_Scalar_List()
{ 
	// Print list of existing scalars	
	std::cout<<std::endl<<"-------------------"<<std::endl;
	if (g_scalar_list.size()==0)
	{
		std::cout<<"No scalar in object list"<<std::endl;
	}
	else
	{
		if (g_scalar_list.size()==1)
		{
			std::cout<<"Found 1 scalar:"<<std::endl;
		}
		else
		{
			std::cout<<"Found "<<g_scalar_list.size()<<" scalars:"<<std::endl;
		}

		
	}
	for(int t=0;t<g_scalar_list.size();++t){
		std::cout<<g_scalar_list.at(t)<<std::endl;

           }

	std::cout<<std::endl<<"-------------------"<<std::endl;
	if (g_scalar_list_selected.size()==0)
	{
		std::cout<<"Selected objects No scalar in object list"<<std::endl;
	}
	else
	{
		if (g_scalar_list_selected.size()==1)
		{
			std::cout<<"Selected objects : found 1 scalar:"<<std::endl;
		}
		else
		{
			std::cout<<"Selected objects: found "<<g_scalar_list.size()<<" scalars:"<<std::endl;
		}

		
	}
	for(int t=0;t<g_scalar_list_selected.size();++t){
		std::cout<<g_scalar_list_selected.at(t)<<std::endl;

           }
}

void MeshTools::Compute_Global_Mean()
{ // Compute center of mass of all selected objects
	
	this->Cont_Mesh.Compute_Global_Mean();
	if (g_mode_cam_centre_of_mass == 1)
	{
		this->Cam_Centre_At_Landmark(-2);
	}
	
}

char* MeshTools::ftoa( double f, double sigfigs )
    {
    char a[81];
    int prec, width, front;

    front = (f==0)? 1 : (int)log10(fabs(f))+1;
    if ( sigfigs < 1.0 && sigfigs >= 0.0 )  // fit number to tolerance
        {
        double rem = fabs(f) - int(f);
        prec=0;
        int num = (int)rem;
        while ( rem*pow((float)10,prec) - num > sigfigs )
            num = int(rem*pow((float)10,++prec));
        width = front;
        sprintf(a, "%#*.*f", width, prec, f );
        }
    else
        {
        if ( sigfigs < 2.0 ) sigfigs = 2.0;
            
        if ( front > (int)sigfigs )
            {
            sprintf( a, "%#.*e", (int)sigfigs-1, f );
            }
        else
            {
            prec = (int)sigfigs - front;
            if ( f==0.0 ) width = 2;
            else width = front + prec + 1;
            sprintf( a, "%#*.*f", width, prec, f );
            }
        }
    return strdup(a);
    }


void MeshTools::Set_Pencil_Extension_Level(int extension_level)
{
	if (extension_level>=0)
	{
		g_pencil_extension=extension_level;
	}
}
int MeshTools::Get_Pencil_Extension_Level()
{return g_pencil_extension;}


void MeshTools::Set_Magic_Wand_Extension_Level(int extension_level)
{
	if (extension_level>=0 && extension_level <=180)
	{
		g_magic_wand_extension=extension_level;
		g_magic_wand_extension_min_cos = cos((double)(extension_level)*PI/180);
		//std::cout<<"Magic wand min cos = "<<g_magic_wand_extension_min_cos<<std::endl;
	}
}
void MeshTools::Set_Magic_Wand_Override(int over)
{
	if (over ==1 || over ==0)
	{
		g_magic_wand_over = over;
	}

}

int MeshTools::Get_Magic_Wand_Extension_Level()
{return g_magic_wand_extension;}


void MeshTools::Activate_Pencil()
{
	g_tag_tool = 0;
}
void MeshTools::Activate_Magic_Wand()
{
	g_tag_tool = 1;
}
void MeshTools::Activate_Flood_Fill()
{
	g_tag_tool = 2;
}

void MeshTools::Set_Grid_Plane(int grid_plane)
{
	g_grid_plane = grid_plane;
}

void MeshTools::Reset_Camera()
{

	camera.zoom = 1.0;
  SetPosition(POS_FACE);
  camera.atx = 0;
  camera.aty = 0;
  camera.atz = 0;
   camera.tx = 0;
    camera.ty = 0;
	 camera.tz = -100;
	//camera.near1 = 0.1;
	 camera.near1 = 0.0;
	camera.far1 = 200;
	
}
void MeshTools::Set_far(float farv)
{
 if (farv >0.1)
 {
 	camera.far1 = (double)farv;
 }
}

MeshTools::MeshTools(int x,int y,int w,int h,const char *l)
            : Fl_Gl_Window(x,y,w,h,l)
{
	// MeshTools constructor
	
	

		OBJECT_MESH::init();
		
	modelandmark=1; // by default show landmarks
	this->landmark_mode=0; // by default: red landmarks
	g_grid_plane =0; // 0 show z y // 1: show z x // 2: show xy
	
	 g_sc_show_below_min=1;
	 g_sc_show_above_max=1;
	float ve[3];
	g_mean[0]=0;
	g_mean[1]=0;
	g_mean[2]=0;
	g_mean_all[0] = 0;
	g_mean_all[1] = 0;
	g_mean_all[2] = 0;

	g_nmean=0;
	g_nmean_all = 0;

	grid_size = 10;
	typedef float *floatptr;
	float **points = new floatptr[10];
	for (int i = 0; i < 10; i++) {
    points[i] = (float*)malloc(3*sizeof(float));
	}
	
	
	ve[0] = 0.1; ve[1] = 1.1; ve[2] = 0.1;
	points[0][0]= 0.1; 
	points[0][1]= 1.1; 
	points[0][2]= 0.05; 
	points[1][0]= 0; 
	points[1][1]= 0; 
	points[1][2]= 1; 
	points[2][0]= 0; 
	points[2][1]= 1; 
	points[2][2]= 0; 
	points[3][0]= 1; 
	points[3][1]= 1; 
	points[3][2]= 1; 
	points[4][0]= 2; 
	points[4][1]= 1; 
	points[4][2]= 1; 
	points[5][0]= 2; 
	points[5][1]= 2; 
	points[5][2]= 1; 
	points[6][0]= 2; 
	points[6][1]= 2; 
	points[6][2]= 2; 
	points[7][0]= 3; 
	points[7][1]= 1; 
	points[7][2]= 1; 
	points[8][0]= 3; 
	points[8][1]= 1; 
	points[8][2]= 2; 
	points[9][0]= 0.1; 
	points[9][1]= 1.1; 
	points[9][2]= 0.11; 
	
	Disp_Vertices_Ids = 0;
	Disp_Triangle_Ids = 0;
	Disp_Scalars_Mode = 0;
	Disp_Tags_Mode=0;
	Min_Color = -1;
	Max_Color = 1;
	g_lambda = 100;
	this->blend_value =50;


	CDataFile ExistingDF("MeshTools.ini", 1);
	select_mode = kpressed = 0;
	this->disp_cull_face = 0;
	bool_change_mode = 1;
	this->disp_curve = 0;
	poly.Polygon_init();
	this->Cont_Mesh.sauv_ve = NULL;
	
	this->Cont_Mesh.OBJECTS_ROOT = new OBJECT_LOG;

	this->Cont_Mesh.OBJECTS_ROOT->OBJECTS = NULL;
	this->Cont_Mesh.OBJECTS_ROOT->nextobj = NULL;
	this->Cont_Mesh.OBJECTS_ROOT->prevobj = NULL;
	this->Cont_Mesh.OBJECTS_ROOT->name = "Root";
	
	this->Cont_Mesh.OBJECTS_ROOT->level = 0; //BASAL OBJECT

	g_mode_tool=0;
	g_selected_names.clear();
	g_distinct_selected_names.clear();

	g_scalar_list.clear();
	g_active_scalar =-1; // Tags...
	// 0 => depth
	// 1 =>	"Maximum_Curvature"
	// 2 => "Minimum_Curvature"
	// 3 => "Gauss_Curvature"
	// 4 => "Mean_Curvature"
	// 4 => "Mean_Curvature"

	g_color_scale_id=0;
	
	
	g_scalar_list_selected.clear();
	g_landmark_size = 10; // 10 mm landmarks
	g_landmark_size =  ExistingDF.GetFloat("size", "landmarks");
	
	
    g_landmark_type =  ExistingDF.GetInt("type", "landmarks");
	g_display_all=  ExistingDF.GetInt("all", "display");
	g_fov_adapt =  ExistingDF.GetInt("adapt", "fov");
	
	g_move_cm =  ExistingDF.GetInt("move_at_cm", "surfaces");
	g_mode_cam_centre_of_mass = ExistingDF.GetInt("move_at_cm", "camera");
	
	g_auto_zoom =  ExistingDF.GetInt("auto_zoom", "surfaces");
	g_color_scale_id=ExistingDF.GetInt("id","color_scale");
	//std::cout<<"Constructor: g_color_scale_id"<<g_color_scale_id<<std::endl;
	g_auto_delete =  ExistingDF.GetInt("auto_delete", "structure");
	g_enable_2sides =  ExistingDF.GetInt("2_sides", "lightning");
	g_lambda = 1;
	affiche_rect=0;
	Cont_Mesh.vect_list_status = 0;
	affiche_triangles = 1;
	rect_x1 =0;
	rect_x2=0;
	rect_y1 = 0;
	rect_y2 = 0;
	color_arrow_size = 1;
	//fValue  = ExistingDF.GetFloat("main_key_float", "Main");
	color_arrow [0] = ExistingDF.GetFloat("r", "color_arrow");
	//lightpos1=0;
	lightpos1=ExistingDF.GetFloat("lightpos1", "lightposition");
	lightpos2=ExistingDF.GetFloat("lightpos2", "lightposition");
	ambiant=ExistingDF.GetFloat("ambiant", "lightposition");
	diffuse=ExistingDF.GetFloat("diffuse", "lightposition");
	specular=ExistingDF.GetFloat("specular", "lightposition");
	color_arrow [1] = ExistingDF.GetFloat("g", "color_arrow");
	color_arrow [2] = ExistingDF.GetFloat("b", "color_arrow");
	color_grid [0] = ExistingDF.GetFloat("r", "color_grid");
	color_grid [1] = ExistingDF.GetFloat("g", "color_grid");
	color_grid [2] = ExistingDF.GetFloat("b", "color_grid");
	color_back [0] = ExistingDF.GetFloat("r", "color_back");
	color_back [1] = ExistingDF.GetFloat("g", "color_back");
	color_back [2] = ExistingDF.GetFloat("b", "color_back");
	color_obj [0] = ExistingDF.GetFloat("r", "color_obj");
	color_obj [1] = ExistingDF.GetFloat("g", "color_obj");
	color_obj [2] = ExistingDF.GetFloat("b", "color_obj");
	
	g_flag_color [0] = ExistingDF.GetFloat("r", "g_flag_color");
	g_flag_color [1] = ExistingDF.GetFloat("g", "g_flag_color");
	g_flag_color [2] = ExistingDF.GetFloat("b", "g_flag_color");
	g_flag_length =  ExistingDF.GetFloat("length", "g_flag_length");

	color_max [0] = ExistingDF.GetFloat("r", "color_surf_max");
	color_max [1] = ExistingDF.GetFloat("g", "color_surf_max");
	color_max [2] = ExistingDF.GetFloat("b", "color_surf_max");
	color_min [0] = ExistingDF.GetFloat("r", "color_surf_min");
	color_min [1] = ExistingDF.GetFloat("g", "color_surf_min");
	color_min [2] = ExistingDF.GetFloat("b", "color_surf_min");
	color_max2 [0] = ExistingDF.GetFloat("r", "color_distnorm_max");
	color_max2 [1] = ExistingDF.GetFloat("g", "color_distnorm_max");
	color_max2 [2] = ExistingDF.GetFloat("b", "color_distnorm_max");
	color_min2 [0] = ExistingDF.GetFloat("r", "color_distnorm_min");
	color_min2 [1] = ExistingDF.GetFloat("g", "color_distnorm_min");
	color_min2 [2] = ExistingDF.GetFloat("b", "color_distnorm_min");
	g_pencil_extension = 3;
	g_magic_wand_extension=45;
	g_magic_wand_over = 1;

    g_magic_wand_extension_min_cos=0.70710678;

	g_orientation_labels[0] = ExistingDF.GetString("z_plus","orientation");
	g_orientation_labels[1] = ExistingDF.GetString("z_minus","orientation");
	g_orientation_labels[2] = ExistingDF.GetString("y_plus","orientation");
	g_orientation_labels[3] = ExistingDF.GetString("y_minus","orientation");
	g_orientation_labels[4] = ExistingDF.GetString("x_plus","orientation");
	g_orientation_labels[5] = ExistingDF.GetString("x_minus","orientation");


	g_tag_tool = 0; // pencil
	g_tag_extraction_criterion_all = 1; // all cell must satisfy threshold criterion...
	g_active_tag = ExistingDF.GetInt("id","active_tag");
	g_tag_colors[0][0] = ExistingDF.GetFloat("r","tag_00");
	g_tag_colors[0][1] = ExistingDF.GetFloat("g","tag_00");
	g_tag_colors[0][2] = ExistingDF.GetFloat("b","tag_00");
	g_tag_colors[0][3] = ExistingDF.GetFloat("a","tag_00");
	g_tag_labels[0] = ExistingDF.GetString("label","tag_00");
	//g_tag_labels[0]= "Test \E9\E9\E9 \E0\E0 eu i";
	//std::cout<<"g_tag_labels[0]="<<g_tag_labels[0]<<std::endl;

	g_tag_colors[1][0] = ExistingDF.GetFloat("r","tag_01");
	g_tag_colors[1][1] = ExistingDF.GetFloat("g","tag_01");
	g_tag_colors[1][2] = ExistingDF.GetFloat("b","tag_01");
	g_tag_colors[1][3] = ExistingDF.GetFloat("a","tag_01");
	g_tag_labels[1] = ExistingDF.GetString("label","tag_01");
	//std::cout<<"g_tag_labels[1]="<<g_tag_labels[1]<<std::endl;

	g_tag_colors[2][0] = ExistingDF.GetFloat("r","tag_02");
	g_tag_colors[2][1] = ExistingDF.GetFloat("g","tag_02");
	g_tag_colors[2][2] = ExistingDF.GetFloat("b","tag_02");
	g_tag_colors[2][3] = ExistingDF.GetFloat("a","tag_02");
	g_tag_labels[2] = ExistingDF.GetString("label","tag_02");
	//std::cout<<"g_tag_labels[2]="<<g_tag_labels[2]<<std::endl;

	g_tag_colors[3][0] = ExistingDF.GetFloat("r","tag_03");
	g_tag_colors[3][1] = ExistingDF.GetFloat("g","tag_03");
	g_tag_colors[3][2] = ExistingDF.GetFloat("b","tag_03");
	g_tag_colors[3][3] = ExistingDF.GetFloat("a","tag_03");
	g_tag_labels[3] = ExistingDF.GetString("label","tag_03");
	//std::cout<<"g_tag_labels[3]="<<g_tag_labels[3]<<std::endl;

	g_tag_colors[4][0] = ExistingDF.GetFloat("r","tag_04");
	g_tag_colors[4][1] = ExistingDF.GetFloat("g","tag_04");
	g_tag_colors[4][2] = ExistingDF.GetFloat("b","tag_04");
	g_tag_colors[4][3] = ExistingDF.GetFloat("a","tag_04");
	g_tag_labels[4] = ExistingDF.GetString("label","tag_04");
	//std::cout<<"g_tag_labels[4]="<<g_tag_labels[4]<<std::endl;

	g_tag_colors[5][0] = ExistingDF.GetFloat("r","tag_05");
	g_tag_colors[5][1] = ExistingDF.GetFloat("g","tag_05");
	g_tag_colors[5][2] = ExistingDF.GetFloat("b","tag_05");
	g_tag_colors[5][3] = ExistingDF.GetFloat("a","tag_05");
	g_tag_labels[5] = ExistingDF.GetString("label","tag_05");
	//std::cout<<"g_tag_labels[5]="<<g_tag_labels[5]<<std::endl;
	
	g_tag_colors[6][0] = ExistingDF.GetFloat("r","tag_06");
	g_tag_colors[6][1] = ExistingDF.GetFloat("g","tag_06");
	g_tag_colors[6][2] = ExistingDF.GetFloat("b","tag_06");
	g_tag_colors[6][3] = ExistingDF.GetFloat("a","tag_06");
	g_tag_labels[6] = ExistingDF.GetString("label","tag_06");
	//std::cout<<"g_tag_labels[6]="<<g_tag_labels[6]<<std::endl;
	
	g_tag_colors[7][0] = ExistingDF.GetFloat("r","tag_07");
	g_tag_colors[7][1] = ExistingDF.GetFloat("g","tag_07");
	g_tag_colors[7][2] = ExistingDF.GetFloat("b","tag_07");
	g_tag_colors[7][3] = ExistingDF.GetFloat("a","tag_07");
	g_tag_labels[7] = ExistingDF.GetString("label","tag_07");
	//std::cout<<"g_tag_labels[7]="<<g_tag_labels[7]<<std::endl;
	
	g_tag_colors[8][0] = ExistingDF.GetFloat("r","tag_08");
	g_tag_colors[8][1] = ExistingDF.GetFloat("g","tag_08");
	g_tag_colors[8][2] = ExistingDF.GetFloat("b","tag_08");
	g_tag_colors[8][3] = ExistingDF.GetFloat("a","tag_08");
	g_tag_labels[8] = ExistingDF.GetString("label","tag_08");
	//std::cout<<"g_tag_labels[8]="<<g_tag_labels[8]<<std::endl;
	
	g_tag_colors[9][0] = ExistingDF.GetFloat("r","tag_09");
	g_tag_colors[9][1] = ExistingDF.GetFloat("g","tag_09");
	g_tag_colors[9][2] = ExistingDF.GetFloat("b","tag_09");
	g_tag_colors[9][3] = ExistingDF.GetFloat("a","tag_09");
	g_tag_labels[9] = ExistingDF.GetString("label","tag_09");
	//std::cout<<"g_tag_labels[9]="<<g_tag_labels[9]<<std::endl;
	
	g_tag_colors[10][0] = ExistingDF.GetFloat("r","tag_10");
	g_tag_colors[10][1] = ExistingDF.GetFloat("g","tag_10");
	g_tag_colors[10][2] = ExistingDF.GetFloat("b","tag_10");
	g_tag_colors[10][3] = ExistingDF.GetFloat("a","tag_10");
	g_tag_labels[10] = ExistingDF.GetString("label","tag_10");
	//std::cout<<"g_tag_labels[10]="<<g_tag_labels[10]<<std::endl;
	
	g_tag_colors[11][0] = ExistingDF.GetFloat("r","tag_11");
	g_tag_colors[11][1] = ExistingDF.GetFloat("g","tag_11");
	g_tag_colors[11][2] = ExistingDF.GetFloat("b","tag_11");
	g_tag_colors[11][3] = ExistingDF.GetFloat("a","tag_11");
	g_tag_labels[11] = ExistingDF.GetString("label","tag_11");
	//std::cout<<"g_tag_labels[11]="<<g_tag_labels[11]<<std::endl;
	
	g_tag_colors[12][0] = ExistingDF.GetFloat("r","tag_12");
	g_tag_colors[12][1] = ExistingDF.GetFloat("g","tag_12");
	g_tag_colors[12][2] = ExistingDF.GetFloat("b","tag_12");
	g_tag_colors[12][3] = ExistingDF.GetFloat("a","tag_12");
	g_tag_labels[12] = ExistingDF.GetString("label","tag_12");
	//std::cout<<"g_tag_labels[12]="<<g_tag_labels[12]<<std::endl;
	
	g_tag_colors[13][0] = ExistingDF.GetFloat("r","tag_13");
	g_tag_colors[13][1] = ExistingDF.GetFloat("g","tag_13");
	g_tag_colors[13][2] = ExistingDF.GetFloat("b","tag_13");
	g_tag_colors[13][3] = ExistingDF.GetFloat("a","tag_13");
	g_tag_labels[13] = ExistingDF.GetString("label","tag_13");
	//std::cout<<"g_tag_labels[13]="<<g_tag_labels[13]<<std::endl;
	
	g_tag_colors[14][0] = ExistingDF.GetFloat("r","tag_14");
	g_tag_colors[14][1] = ExistingDF.GetFloat("g","tag_14");
	g_tag_colors[14][2] = ExistingDF.GetFloat("b","tag_14");
	g_tag_colors[14][3] = ExistingDF.GetFloat("a","tag_14");
	g_tag_labels[14] = ExistingDF.GetString("label","tag_14");
	//std::cout<<"g_tag_labels[14]="<<g_tag_labels[14]<<std::endl;
	
	g_tag_colors[15][0] = ExistingDF.GetFloat("r","tag_15");
	g_tag_colors[15][1] = ExistingDF.GetFloat("g","tag_15");
	g_tag_colors[15][2] = ExistingDF.GetFloat("b","tag_15");
	g_tag_colors[15][3] = ExistingDF.GetFloat("a","tag_15");
	g_tag_labels[15] = ExistingDF.GetString("label","tag_15");
	//std::cout<<"g_tag_labels[15]="<<g_tag_labels[15]<<std::endl;
	
	g_tag_colors[16][0] = ExistingDF.GetFloat("r","tag_16");
	g_tag_colors[16][1] = ExistingDF.GetFloat("g","tag_16");
	g_tag_colors[16][2] = ExistingDF.GetFloat("b","tag_16");
	g_tag_colors[16][3] = ExistingDF.GetFloat("a","tag_16");
	g_tag_labels[16] = ExistingDF.GetString("label","tag_16");
	//std::cout<<"g_tag_labels[16]="<<g_tag_labels[16]<<std::endl;
	
	g_tag_colors[17][0] = ExistingDF.GetFloat("r","tag_17");
	g_tag_colors[17][1] = ExistingDF.GetFloat("g","tag_17");
	g_tag_colors[17][2] = ExistingDF.GetFloat("b","tag_17");
	g_tag_colors[17][3] = ExistingDF.GetFloat("a","tag_17");
	g_tag_labels[17] = ExistingDF.GetString("label","tag_17");
	//std::cout<<"g_tag_labels[17]="<<g_tag_labels[17]<<std::endl;
	
	g_tag_colors[18][0] = ExistingDF.GetFloat("r","tag_18");
	g_tag_colors[18][1] = ExistingDF.GetFloat("g","tag_18");
	g_tag_colors[18][2] = ExistingDF.GetFloat("b","tag_18");
	g_tag_colors[18][3] = ExistingDF.GetFloat("a","tag_18");
	g_tag_labels[18] = ExistingDF.GetString("label","tag_18");
	//std::cout<<"g_tag_labels[18]="<<g_tag_labels[18]<<std::endl;
	
	g_tag_colors[19][0] = ExistingDF.GetFloat("r","tag_19");
	g_tag_colors[19][1] = ExistingDF.GetFloat("g","tag_19");
	g_tag_colors[19][2] = ExistingDF.GetFloat("b","tag_19");
	g_tag_colors[19][3] = ExistingDF.GetFloat("a","tag_19");
	g_tag_labels[19] = ExistingDF.GetString("label","tag_19");
	//std::cout<<"g_tag_labels[19]="<<g_tag_labels[19]<<std::endl;
	
	g_tag_colors[20][0] = ExistingDF.GetFloat("r","tag_20");
	g_tag_colors[20][1] = ExistingDF.GetFloat("g","tag_20");
	g_tag_colors[20][2] = ExistingDF.GetFloat("b","tag_20");
	g_tag_colors[20][3] = ExistingDF.GetFloat("a","tag_20");
	g_tag_labels[20] = ExistingDF.GetString("label","tag_20");
	//std::cout<<"g_tag_labels[20]="<<g_tag_labels[20]<<std::endl;
	
	g_tag_colors[21][0] = ExistingDF.GetFloat("r","tag_21");
	g_tag_colors[21][1] = ExistingDF.GetFloat("g","tag_21");
	g_tag_colors[21][2] = ExistingDF.GetFloat("b","tag_21");
	g_tag_colors[21][3] = ExistingDF.GetFloat("a","tag_21");
	g_tag_labels[21] = ExistingDF.GetString("label","tag_21");
	//std::cout<<"g_tag_labels[21]="<<g_tag_labels[21]<<std::endl;
	
	g_tag_colors[22][0] = ExistingDF.GetFloat("r","tag_22");
	g_tag_colors[22][1] = ExistingDF.GetFloat("g","tag_22");
	g_tag_colors[22][2] = ExistingDF.GetFloat("b","tag_22");
	g_tag_colors[22][3] = ExistingDF.GetFloat("a","tag_22");
	g_tag_labels[22] = ExistingDF.GetString("label","tag_22");
	//std::cout<<"g_tag_labels[22]="<<g_tag_labels[22]<<std::endl;
	
	g_tag_colors[23][0] = ExistingDF.GetFloat("r","tag_23");
	g_tag_colors[23][1] = ExistingDF.GetFloat("g","tag_23");
	g_tag_colors[23][2] = ExistingDF.GetFloat("b","tag_23");
	g_tag_colors[23][3] = ExistingDF.GetFloat("a","tag_23");
	g_tag_labels[23] = ExistingDF.GetString("label","tag_23");
	//std::cout<<"g_tag_labels[23]="<<g_tag_labels[23]<<std::endl;
	
	g_tag_colors[24][0] = ExistingDF.GetFloat("r","tag_24");
	g_tag_colors[24][1] = ExistingDF.GetFloat("g","tag_24");
	g_tag_colors[24][2] = ExistingDF.GetFloat("b","tag_24");
	g_tag_colors[24][3] = ExistingDF.GetFloat("a","tag_24");
	g_tag_labels[24] = ExistingDF.GetString("label","tag_24");
	//std::cout<<"g_tag_labels[24]="<<g_tag_labels[24]<<std::endl;
	
	color_surf_range_min = ExistingDF.GetFloat("color_surf_range_min", "color_surf_range");
	color_surf_range_max = ExistingDF.GetFloat("color_surf_range_max", "color_surf_range");
	//color_distnorm_range_min = ExistingDF.GetFloat("color_distnorm_range_min", "color_distnorm_range");
	//color_distnorm_range_max = ExistingDF.GetFloat("color_distnorm_range_max", "color_distnorm_range");
    vAng = 0.0;
	landmarkx=landmarky=landmarkz=1;
    hAng=0.0;
	zAng=0.0;
	zoom = 120;
	g_zoom = 0;
	axeangle = 0;
	dispmode = 1;
	cpt_dispmode4and5 = 0;
	rotx =0; roty = 0; rotz = 0; rotx2 =0; roty2 = 0; rotz2 = 0; tx2 = 0; ty2 = 0; tz2 = 0;tx=ty=tz=0;
	axerot[0] = 0;axerot[1] = 0;axerot[2] = 0;
    size=10.0;
	xshift=0.0;
	yshift=0.0;
	position = 1;
	showgrid = 1;
	showorientation = 1;
	SetMode (MODE_NOTHING); //Nothing is loaded;
	light_position[0] = 0;
	light_position[1] = 0;
	light_position[2] = 110;
	light_position[3] = 0;
	zshift=0.0;
    id_affiche = 0;
	bcouleur = 1;
	couleur_mode=0;
	vertrouge = 0;
	affiche_norm_triangles = 0;
	affiche_norm_points = 0;
	affiche_num = 0;
	camera.zoom = 1.0;
  SetPosition(POS_FACE);
  camera.atx = 0;
  camera.aty = 0;
  camera.atz = 0;
   camera.tx = 0;
    camera.ty = 0;
	 camera.tz = -100;
	//camera.near1 = 0.1;
	 camera.near1 = 0.0;
	camera.far1 = 200;
	
	Cont_Mesh.dmean_ok = 1;
		
	Cont_Mesh.containerallocate(1);		
	Cont_Mesh.nbLANDMARKS=0;

    /* The cube definition. These are the vertices of a unit cube
     * centered on the origin.*/
    
    boxv0[0] = -0.5; boxv0[1] = -0.5; boxv0[2] = -0.5;
    boxv1[0] =  0.5; boxv1[1] = -0.5; boxv1[2] = -0.5;
    boxv2[0] =  0.5; boxv2[1] =  0.5; boxv2[2] = -0.5;
    boxv3[0] = -0.5; boxv3[1] =  0.5; boxv3[2] = -0.5;
    boxv4[0] = -0.5; boxv4[1] = -0.5; boxv4[2] =  0.5;
    boxv5[0] =  0.5; boxv5[1] = -0.5; boxv5[2] =  0.5;
    boxv6[0] =  0.5; boxv6[1] =  0.5; boxv6[2] =  0.5;
    boxv7[0] = -0.5; boxv7[1] =  0.5; boxv7[2] =  0.5;
	color_arrow[3]=1.0;
	color_grid[3]=1.0;
	color_back[3]=1.0;
	color_obj[3]=1.0;
	color_min[3]=1.0;
	color_min2[3]=1.0;
	//color_max[0]=0.5;
	//color_max[1]=0.0;
	//color_max[2]=0.23;
	color_max[3]=1.0;
	color_max2[3]=1.0;
	
	
	//std::string version = glGetString(GL_VERSION);
	
}



void MeshTools::Mesh_Tri_Sort( int side)
{
	
	Cont_Mesh.Mesh_Tri_Sort(side);
	
}
void MeshTools::DrawColorScale(int x, int y, int w1, int h1)
{

	double Min_Value = this->Get_sc_min();
	double Max_Value = this->Get_sc_max();
	double Mean_Value = (Min_Value+Max_Value)/2;
	double sigfigs =4;
	int curint = (int)(w1/25);
	int w2 = curint*25;
	//include un mode sp\E9cial pour les scalars.
	
	//std::cout<<"Min="<<Min_Value<<",Max="<<Max_Value<<",Mean="<<Mean_Value<<std::endl;
	if (this->disp_cull_face == 1)
		{
			 glDisable(GL_CULL_FACE);  	
		}
		

	char info1[10], info2[10],info3[10];
	
	//char info1[50], info2[50],info3[50];
	//char info1[50], info2[50],info3[50];
	

	char *Min_Value_a;
	char *Max_Value_a;
	char *Mean_Value_a;
	//std::cout<<"Try to get Min_Value_a"<<std::endl;
	Min_Value_a = ftoa(Min_Value,sigfigs);
	//std::cout<<"Min_Value_a="<<Min_Value_a<<std::endl;
	//std::cout<<"Try to get Max_Value_a"<<std::endl;
	Max_Value_a = ftoa(Max_Value,sigfigs);
	//std::cout<<"Max_Value_a="<<Max_Value_a<<std::endl;
	//std::cout<<"Try to get Mean_Value_a"<<std::endl;
	Mean_Value_a = ftoa(Mean_Value,sigfigs);
	//std::cout<<"Mean_Value_a="<<Mean_Value_a<<std::endl;


	strcpy(info1,"");
	strcpy(info2,"");
	strcpy(info3,"");

	////std::cout<<"Try to cat Min to info"<<std::endl;
	
	strcat(info1,Min_Value_a);	
	//std::cout<<"Try to cat Mean to info"<<std::endl;
	
	strcat(info2,Mean_Value_a);
    //std::cout<<"Try to cat Max to info"<<std::endl;
	
	strcat(info3,Max_Value_a);

	//std::cout<<"info1="<<info1<<std::endl;
	//std::cout<<"info2="<<info2<<std::endl;
	//std::cout<<"info3="<<info3<<std::endl;

	//std::cout<<"Try to affiche info1"<<std::endl;
	if (g_color_scale_id != -1)
	{
		AfficheTexte (x,h() -y, info1, blanc);
		//std::cout<<"Try to affiche info2"<<std::endl;

		AfficheTexte (x +(int)(w2/2),h() -y, info2, blanc);
		//std::cout<<"Try to affiche info3"<<std::endl;

		AfficheTexte (x+w2,h() -y, info3, blanc);
	}
	
	//std::cout<<"Try to affiche rectangle"<<std::endl;

	//AfficheRectangle(x+15, x+w1+15, h()-(y+10),  h()-(y+10+h1),  rouge);
	GLfloat color[4];
	float value = 0.0;
	int curx;
	//std::cout<<"g_color_scale_id"<<g_color_scale_id<<std::endl;

	for (int i = 0; i<25;i++)
	{
			curx = (x+15)+i*curint; 
		if (g_color_scale_id !=-1)
		{
			value = (float) ((Min_Value)*(24-i) + Max_Value*i)/24;
			ConvertScalarToColor( value,  color,  1.0);
			color[3]=1.0; // no culling
			AfficheRectangle(curx, curx+curint, h()-(y+10),  h()-(y+10+h1),  color);
		}
		else
		{
			color[0]=g_tag_colors[i][0];
			color[1]=g_tag_colors[i][1];
			color[2]=g_tag_colors[i][2];
			color[3]=1.0;
			
			itoa( i, info1, 10);
			//std::cout<<"info1:"<<info1<<std::endl;
			AfficheTexte (curx+4,h()-y, info1, blanc);
			AfficheRectangle(curx, curx+curint, h()-(y+7),  h()-(y+10+h1), color);
		}
	}

	if (g_color_scale_id !=-1)
	{
		AfficheRectangle(x+15, x+17, h()-(y+5),  h()-(y+10+h1),  blanc);

		AfficheRectangle(x+int(w1/2)+15, x+int(w2/2)+17, h()-(y+5),  h()-(y+10+h1),  blanc);
		
		AfficheRectangle(x+ w2+13, x+w1+15, h()-(y+5),  h()-(y+10+h1),  blanc);
	}
	if (this->disp_cull_face == 1)
		{
			 glEnable(GL_CULL_FACE);  	
		}
}
	
void MeshTools::AfficheRectangle(int x1, int x2, int y1, int y2,  GLfloat couleur[4])
{
glPushMatrix();
	glDisable(GL_LIGHTING);
	glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		//gluOrtho2D (0, w(), 0, h());
	glOrtho( 0, w(), 0, h(), -1, 1);	
		glMatrixMode (GL_MODELVIEW); 
		glPushMatrix();
		glLoadIdentity(); 
  glColor4f(couleur[0],couleur[1],couleur[2],couleur[3]);
  glMaterialfv(GL_FRONT,GL_AMBIENT,couleur);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,couleur);
  glMaterialfv(GL_FRONT,GL_SPECULAR,couleur);
  glMaterialfv(GL_FRONT,GL_SHININESS,couleur);
  glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);


	glBegin(GL_QUADS); // dessin du carr\E9
	glVertex2d(x1,y1); 
	glVertex2d(x1,y2); 
	glVertex2d(x2,y2); 
	glVertex2d(x2,y1); 
	glEnd();

	glPopMatrix();
	glEnable(GL_LIGHTING);
	glPopMatrix();
}
void MeshTools::Draw_Warning_Invertion(int x, int y)
{


	char info1[10], info2[50], info3[50];

	strcpy (info1,"Tag Mode:");
	strcpy (info2,"Selected meshes are colored.");
	strcpy (info3,"Unselected meshes appear grey.");

	AfficheTexte (x,h() -y, info1, rouge);
	AfficheTexte (x,h() -y-15, info2, blanc);
	AfficheTexte (x,h()-y-30, info3, blanc);
	
	

}
void MeshTools::Mesh_draw_landmark_infos(int x, int y)
{


int landmark_number;
int landmark_number_deformable;
int landmark_number_flags;
int landmark_selected;
char info1[50], info2[40],info3[50],info4[50];
char landmark_nr[10];

//char * landmark_nr;
char landmark_nr_def[10];
char landmark_nr_flag[10];
//char *landmark_nr_def;
char landmark_sel[10];
//char * landmark_sel;
	
	landmark_number = Cont_Mesh.Get_Landmark_Number(0);	
	landmark_number_deformable = Cont_Mesh.Get_Landmark_Number(1);	
	landmark_number_flags = Cont_Mesh.Get_Landmark_Number(2);	
	landmark_selected = Cont_Mesh.Get_Landmark_Selected(0)+Cont_Mesh.Get_Landmark_Selected(1)+Cont_Mesh.Get_Landmark_Selected(2);
    
	itoa( landmark_number, landmark_nr, 10);
	//landmark_nr = itoa( landmark_number, 10);
	
	itoa( landmark_number_deformable, landmark_nr_def, 10);
	itoa( landmark_number_flags, landmark_nr_flag, 10);

	itoa( landmark_selected, landmark_sel, 10);
	//landmark_sel = itoa( landmark_selected, 10);

	strcpy(info1,"Normal landmark nr:");
	strcpy(info2,"Target landmark nr:");
	strcpy(info4,"Flag landmark nr:");
	strcpy(info3,"Total nr selected:");

	strcat(info1,landmark_nr);	
	strcat(info2,landmark_nr_def);
	strcat(info4,landmark_nr_flag);
    strcat(info3,landmark_sel);
	AfficheTexte (x,h() -y, info1, blanc);
	
	AfficheTexte (x,h() -y-15, info2, blanc);
	AfficheTexte (x,h() -y-30, info4, blanc);
	//AfficheTexte (x,h() -y-210, info1, rouge);
	//AfficheTexte (x,h() -y-25, info3, blanc);
	if (landmark_selected>1)
	{
		AfficheTexte (x,h()-y-45, info3, rouge);
	}
	else
	{
	  AfficheTexte (x,h()-y -45, info3, blanc);
	}
	

}





void MeshTools::Mesh_Mirror_ThroughY()
{
		Cont_Mesh.Mesh_Mirror_ThroughY();
}
void MeshTools::Mesh_draw_Obj(int mode, bool bool_change_pos_obj)
{

					 
	Cont_Mesh.Mesh_Draw_Obj(mode, bool_change_pos_obj,bool_vbo);

}
void MeshTools::Mesh_draw_Curve()
{
	
	Cont_Mesh.Mesh_Draw_Curve();
}

void MeshTools::DrawOrientations(int x, int y, float size)
{
	double PI = 3.1415926535897932384626;

float uv[3];

char z_plus_l[50], z_minus_l[50];
char y_plus_l[50], y_minus_l[50];
char x_plus_l[50], x_minus_l[50];
	

	strcpy(z_plus_l,"");
	strcpy(z_minus_l,"");
	strcpy(y_plus_l,"");
	strcpy(y_minus_l,"");
	strcpy(x_plus_l,"");
	strcpy(x_minus_l,"");
	
	strcat(z_plus_l,g_orientation_labels[0].c_str());		
	strcat(z_minus_l,g_orientation_labels[1].c_str());		
	strcat(y_plus_l,g_orientation_labels[2].c_str());		
	strcat(y_minus_l,g_orientation_labels[3].c_str());		
	strcat(x_plus_l,g_orientation_labels[4].c_str());		
	strcat(x_minus_l,g_orientation_labels[5].c_str());		
	
	//std::cout<<"Try to affiche info1"<<std::endl;
	
		//AfficheTexte (x,h() -y, info1, blanc);


//projecting 1 0 0
//uv[0]=cos(PI*camera.az/180.0)*cos(-PI*camera.el/180.0);
//uv[1]=sin(PI*camera.az/180.0)*cos(-PI*camera.el/180.0);
//uv[2]=-sin(-PI*camera.el/180.0);

//projecting 0 1 0 projecting Y
//uv[0]=cos(PI*camera.az/180.0)*sin(-PI*camera.el/180.0)*sin(-PI*camera.tw/180.0) - sin(PI*camera.az/180.0)*cos(-PI*camera.tw/180.0);
//uv[1]=sin(PI*camera.az/180.0)*sin(-PI*camera.el/180.0)*sin(-PI*camera.tw/180.0) + cos(PI*camera.az/180.0)*cos(-PI*camera.tw/180.0);
//uv[2]=cos(-PI*camera.el/180.0)*sin(-PI*camera.tw/180.0);

// projecting 0 0 1 projecting Z
//uv[0]=cos(PI*camera.az/180.0)*sin(-PI*camera.el/180.0)*cos(-PI*camera.tw/180.0) + sin(PI*camera.az/180.0)*sin(-PI*camera.tw/180.0);
//uv[1]=sin(PI*camera.az/180.0)*sin(-PI*camera.el/180.0)*cos(-PI*camera.tw/180.0) - cos(PI*camera.az/180.0)*sin(-PI*camera.tw/180.0);
//uv[2]=cos(-PI*camera.el/180.0)*cos(-PI*camera.tw/180.0);

//Create => This one is projecting X
//uv[0]=cos(3.1415926535897932384626*camera.az/180.0)*cos(3.1415926535897932384626*camera.el/180.0);
//uv[1]=sin(3.1415926535897932384626*camera.az/180.0)*cos(3.1415926535897932384626*camera.el/180.0);
//uv[2]=sin(3.1415926535897932384626*camera.el/180.0);
/*
glMaterialfv(GL_FRONT,GL_AMBIENT,jaune);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,jaune);
  glMaterialfv(GL_FRONT,GL_SPECULAR,jaune);
  glMaterialfv(GL_FRONT,GL_SHININESS,jaune);
  glMaterialfv(GL_FRONT,GL_EMISSION,jaune);
glPushMatrix();
glBegin(GL_LINES);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(uv[0],uv[1],uv[2]);
glEnd();
glRasterPos3f(uv[0],uv[1],uv[2]);
printString("p");


glPopMatrix();*/
glPushMatrix();


glMatrix screen_mat;
 glMatrix cam_mat;
 glMatrix screenproj;
		
  get_projection_matrix(screenproj);
		get_camera_transform_matrix_no_translation(cam_mat);
		glPushMatrix();
		glLoadIdentity();	
		glMultMatrixf((GLfloat*) screenproj);
		glMultMatrixf((GLfloat*) cam_mat);
		getmatrix(screen_mat);
		glPopMatrix ();

float vv1[3];
float z_plus[3];
float z_minus[3];
float y_plus[3];
float y_minus[3];
float x_plus[3];
float x_minus[3];

struct {GLint x, y, width, height;} viewport;	 
glGetIntegerv(GL_VIEWPORT, &viewport.x);
float mzoom =zoom *(1-g_zoom/100);
vv1[0]=0;
vv1[1]=0;
vv1[2]=mzoom ;
ApplyTransformation(vv1,z_plus,screen_mat);


vv1[0]=0;
vv1[1]=0;
vv1[2]=-mzoom;
ApplyTransformation(vv1,z_minus,screen_mat);
vv1[0]=0;
vv1[1]=mzoom;
vv1[2]=0;
ApplyTransformation(vv1,y_plus,screen_mat);
vv1[0]=0;
vv1[1]=-mzoom;
vv1[2]=0;
ApplyTransformation(vv1,y_minus,screen_mat);
vv1[0]=mzoom;
vv1[1]=0;
vv1[2]=0;
ApplyTransformation(vv1,x_plus,screen_mat);
vv1[0]=-mzoom;
vv1[1]=0;
vv1[2]=0;
ApplyTransformation(vv1,x_minus,screen_mat);




glDisable(GL_LIGHTING);
	glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		//gluOrtho2D (0, w(), 0, h());
		glOrtho( 0, w(), 0, h(), -1, 1);
		glMatrixMode (GL_MODELVIEW); 
		glPushMatrix();
		glLoadIdentity(); 

/* std::cout<<"zplus="<<z_plus[0]<<","<<z_plus[1]<<","<<z_plus[2]<<std::endl;
  std::cout<<"zminus="<<z_minus[0]<<","<<z_minus[1]<<","<<z_minus[2]<<std::endl;
  std::cout<<"yplus="<<y_plus[0]<<","<<y_plus[1]<<","<<y_plus[2]<<std::endl;
  std::cout<<"yminus="<<y_minus[0]<<","<<y_minus[1]<<","<<y_minus[2]<<std::endl;
  std::cout<<"xplus="<<x_plus[0]<<","<<x_plus[1]<<","<<x_plus[2]<<std::endl;
  std::cout<<"xminus="<<x_minus[0]<<","<<x_minus[1]<<","<<x_minus[2]<<std::endl;*/
		int z_is_min =0;
		int y_is_min = 0;
		int x_is_min = 0;
		float z_d = z_plus[0]*z_plus[0]+z_plus[1]*z_plus[1];
		float y_d = y_plus[0]*y_plus[0]+y_plus[1]*y_plus[1];
		float x_d = x_plus[0]*x_plus[0]+x_plus[1]*x_plus[1];
	
		if (z_d > y_d)
		{
			if (y_d>x_d)
			{
				x_is_min=1;
			}
			else
			{
				y_is_min=1;
			}
		}
		else
		{
				if (z_d>x_d)
			{
				x_is_min=1;
			}
			else
			{
				z_is_min=1;
			}
		
		}

  glColor4f(rouge[0],rouge[1],rouge[2],rouge[3]);
  glMaterialfv(GL_FRONT,GL_AMBIENT,rouge);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,rouge);
  glMaterialfv(GL_FRONT,GL_SPECULAR,rouge);
  glMaterialfv(GL_FRONT,GL_SHININESS,rouge);
  glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);
 

	glBegin(GL_LINES); // 
	glVertex2d(x,y); 
	glVertex2d(
		x+size*z_plus[0]*viewport.width/1000,
		y+size*z_plus[1]*viewport.height/1000);
	glEnd();

	

	glBegin(GL_LINES); // 
	glVertex2d(x,y); 
	glVertex2d(
		x+size*z_minus[0]*viewport.width/1000,
		y+size*z_minus[1]*viewport.height/1000
		);
	glEnd();
	if( z_is_min==0)
	{
		AfficheTexte (x+1.5*size*z_plus[0]*viewport.width/1000,
		y+1.5*size*z_plus[1]*viewport.height/1000
		, z_plus_l, rouge);

		AfficheTexte (x+1.5*size*z_minus[0]*viewport.width/1000,
		y+1.5*size*z_minus[1]*viewport.height/1000
		, z_minus_l, rouge);
	}
	

	/* 
	*/

glColor4f(orange[0],orange[1],orange[2],orange[3]);
  glMaterialfv(GL_FRONT,GL_AMBIENT,orange);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,orange);
  glMaterialfv(GL_FRONT,GL_SPECULAR,orange);
  glMaterialfv(GL_FRONT,GL_SHININESS,orange);
  glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);
	

	glBegin(GL_LINES); // 
	glVertex2d(x,y); 
	glVertex2d(x+size*y_plus[0]*viewport.width/1000,y+size*y_plus[1]*viewport.height/1000);
	glEnd();

	glBegin(GL_LINES); // 
	glVertex2d(x,y); 
	glVertex2d(x+size*y_minus[0]*viewport.width/1000,y+size*y_minus[1]*viewport.height/1000);
	glEnd();
	if( y_is_min==0)
	{
		AfficheTexte (x+1.5*size*y_plus[0]*viewport.width/1000,
			y+1.5*size*y_plus[1]*viewport.height/1000
			, y_plus_l, orange);
		AfficheTexte (x+1.5*size*y_minus[0]*viewport.width/1000,
			y+1.5*size*y_minus[1]*viewport.height/1000
			, y_minus_l, orange);
	}

glColor4f(jaune[0],jaune[1],jaune[2],jaune[3]);
  glMaterialfv(GL_FRONT,GL_AMBIENT,jaune);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,jaune);
  glMaterialfv(GL_FRONT,GL_SPECULAR,jaune);
  glMaterialfv(GL_FRONT,GL_SHININESS,jaune);
  glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);
	

	glBegin(GL_LINES); // 
	glVertex2d(x,y); 
	glVertex2d(x+size*x_plus[0]*viewport.width/1000,y+size*x_plus[1]*viewport.height/1000);
	glEnd();



	glBegin(GL_LINES); // 
	glVertex2d(x,y); 
	glVertex2d(x+size*x_minus[0]*viewport.width/1000,y+size*x_minus[1]*viewport.height/1000);
	glEnd();
	if( x_is_min==0)
	{
		AfficheTexte (x+1.5*size*x_plus[0]*viewport.width/1000,
		y+1.5*size*x_plus[1]*viewport.height/1000
		, x_plus_l, jaune);
	
		AfficheTexte (x+1.5*size*x_minus[0]*viewport.width/1000,
			y+1.5*size*x_minus[1]*viewport.height/1000
			, x_minus_l, jaune);
	}
	glPopMatrix();
	glEnable(GL_LIGHTING);
// FIN TMP
glPopMatrix ();

}


void MeshTools::drawGrid()
{float i;
  int count = 0;
   float size_factor = grid_size/10;
  glLineWidth(1);
  glDisable(GL_LIGHTING);
  GLfloat white4[] = {.4, .4, .4, 1.};
  GLfloat white1[] = {.1, .1, .1, 1.};
  GLfloat green5[] = {0., .5, 0., 1.};
  GLfloat green2[] = {0., .2, 0., 1.};
  GLfloat black[] = {0., 0., 0., 1.};
  GLfloat mat_shininess[] = {7.};		
  float delt[3];
  if (g_mode_cam_centre_of_mass == 1)
  {
	  delt[0] = g_mean_all[0];
	  delt[1] = g_mean_all[1];
	  delt[2] = g_mean_all[2];
  }
  else
  {
	  delt[0] = 0;
	  delt[1] = 0;
	  delt[2] = 0;
  }
  //std::cout<<"g_grid_plane="<<g_grid_plane<<std::endl;
  if (g_grid_plane==2)
  {
	  for(i=-10.;i<=10.;i+=1) 
	  {
			glColor3f(color_grid[0], color_grid[1], color_grid[2]);
			glBegin(GL_LINES);     
			//glNormal3f(0.,0.,1.);
			glVertex3f(10*i*size_factor+ delt[0],100*size_factor+delt[1],0.+delt[2]);
			glVertex3f(10*i*size_factor + delt[0],-100*size_factor+delt[1],0. + delt[2]);
		
			
			count++;
			glEnd();  
	  }
	  for(i=-10.;i<=10.;i+=1) 
	  {
			glColor3f(color_grid[0], color_grid[1], color_grid[2]);
			 glBegin(GL_LINES);     
			//glNormal3f(0.,0.,1.);
			glVertex3f(-100*size_factor + delt[0],10*i*size_factor + delt[1],0. + delt[2]);
			glVertex3f(100*size_factor + delt[0],10*i*size_factor + delt[1],0. + delt[2]);
			
			
			count++;
			glEnd();  
	  }
  }
   if (g_grid_plane ==1)
  {
	for(i=-10.;i<=10.;i+=1) 
	  {
	    

			glColor3f(color_grid[0], color_grid[1], color_grid[2]);
			glBegin(GL_LINES);     
			//glNormal3f(0.,0.,1.);
			glVertex3f(10*i*size_factor + delt[0],0 + delt[1],100*size_factor + delt[2]);
			glVertex3f(10*i*size_factor + delt[0],0 + delt[1],-100*size_factor + delt[2]);
		
			
			count++;
			glEnd();  
	  }
	  for(i=-10.;i<=10.;i+=1) 
	  {
	    
			glColor3f(color_grid[0], color_grid[1], color_grid[2]);
			 glBegin(GL_LINES);     
			//glNormal3f(0.,0.,1.);
			glVertex3f(-100*size_factor + delt[0],0 + delt[1],10*i*size_factor + delt[2]);
			glVertex3f(100*size_factor + delt[0],0 + delt[1],10*i*size_factor + delt[2]);
			
			
			count++;
			glEnd();  
	  }
  }
   if (g_grid_plane ==0)
  {
	  for(i=-10.;i<=10.;i+=1) 
	  {
	    
	       
			glColor3f(color_grid[0], color_grid[1], color_grid[2]);
			 glBegin(GL_LINES);     
			//glNormal3f(0.,0.,1.);
			glVertex3f(0 + delt[0],-100*size_factor + delt[1],10*i*size_factor + delt[2]);
			glVertex3f(0 + delt[0],100*size_factor + delt[1],10*i*size_factor + delt[2]);
			
			
			count++;
			glEnd();  
	  }
	  
	 for(i=-10.;i<=10.;i+=1) 
	  {
	    
			glColor3f(color_grid[0], color_grid[1], color_grid[2]);
			 glBegin(GL_LINES);     
			//glNormal3f(0.,0.,1.);
			glVertex3f(0 + delt[0],10*i*size_factor + delt[1],-100*size_factor + delt[2]);
			glVertex3f(0 + delt[0],10*i*size_factor + delt[1],100*size_factor + delt[2]);
			
			
			count++;
			glEnd();  
	  }

 }

glLineWidth(2);
if (g_mode_cam_centre_of_mass == 0)
{
	glColor3f(1.0, 0.7, 0.1);
}
else
{
	glColor3f(0.0, 0.7, 1.0);
}



		 glBegin(GL_LINES);  

		 // crosses

 if (g_grid_plane ==0)
  {
		//glNormal3f(0.,0.,1.);
		glVertex3f(0 + delt[0],0 + delt[1],-100*size_factor + delt[2]);
		glVertex3f(0 + delt[0],0 + delt[1],100*size_factor + delt[2]);
		glVertex3f(0 + delt[0],-100*size_factor + delt[1],0 + delt[2]);
		glVertex3f(0 + delt[0],100*size_factor + delt[1],0 + delt[2]);

		
 }
if (g_grid_plane ==1)
  {
	  glVertex3f(0 + delt[0],0 + delt[1],-100*size_factor + delt[2]);
		glVertex3f(0 + delt[0],0 + delt[1],100*size_factor + delt[2]);
		glVertex3f(-100*size_factor + delt[0],0 + delt[1],0 + delt[2]);
		glVertex3f(100*size_factor + delt[0],0 + delt[1],0 + delt[2]);
 }

if (g_grid_plane ==2)
  {
					
		glVertex3f(-100*size_factor + delt[0],0 + delt[1],0 + delt[2]);
		glVertex3f(100*size_factor + delt[0],0 + delt[1],0 + delt[2]);
		glVertex3f(0 + delt[0],-100*size_factor + delt[1],0 + delt[2]);
		glVertex3f(0 + delt[0],100*size_factor + delt[1],0 + delt[2]);
 }

// Frames
 if (g_grid_plane ==0)
  {
		
	   // 0, y , -z
		glVertex3f(0 + delt[0],100*size_factor + delt[1],-100*size_factor + delt[2]);
		// 0, y , z
		glVertex3f(0 + delt[0],100*size_factor + delt[1],100*size_factor + delt[2]);

		// 0, -y , z
		glVertex3f(0 + delt[0],-100*size_factor + delt[1],100*size_factor + delt[2]);
		// 0, y , z
		glVertex3f(0 + delt[0],100*size_factor + delt[1],100*size_factor + delt[2]);

		// 0, -y , -z
		glVertex3f(0 + delt[0],-100*size_factor + delt[1],-100*size_factor + delt[2]);
		// 0, -y , z
		glVertex3f(0 + delt[0],-100*size_factor + delt[1],100*size_factor + delt[2]);

		// 0, -y , -z
		glVertex3f(0 + delt[0],-100*size_factor + delt[1],-100*size_factor + delt[2]);
		// 0, y , -z
		glVertex3f(0 + delt[0],100*size_factor + delt[1],-100*size_factor + delt[2]);

 }
  if (g_grid_plane ==1)
  {
	   // x, 0 , -z
		glVertex3f(100*size_factor + delt[0],0 + delt[1],-100*size_factor + delt[2]);
		// x, 0 , +z
		glVertex3f(100*size_factor + delt[0],0 + delt[1],100*size_factor + delt[2]);

		// -x, 0 , z
		glVertex3f(-100*size_factor + delt[0],0 + delt[1],100*size_factor + delt[2]);
		// x, 0 , z
		glVertex3f(100*size_factor + delt[0],0 + delt[1],100*size_factor + delt[2]);
		
		// -x, 0 , -z
		glVertex3f(-100*size_factor + delt[0],0 + delt[1],-100*size_factor + delt[2]);
		// -x, 0 , z
		glVertex3f(-100*size_factor + delt[0],0 + delt[1],100*size_factor + delt[2]);

		// -x, 0 , -z
		glVertex3f(-100*size_factor + delt[0],0 + delt[1],-100*size_factor + delt[2]);
		// x, 0 , -z
		glVertex3f(100*size_factor + delt[0],0 + delt[1],-100*size_factor + delt[2]);
		
 }
   if (g_grid_plane ==2)
  {

	   // x , -y, 0
		glVertex3f(100*size_factor + delt[0],-100*size_factor + delt[1],0 + delt[2]);
		//x, +y,0 
		glVertex3f(100*size_factor + delt[0],100*size_factor + delt[1],0 + delt[2]);

		//-x, y, 0
		glVertex3f(-100*size_factor + delt[0],100*size_factor + delt[1],0 + delt[2]);
		//x, y, 0
		glVertex3f(100*size_factor + delt[0],100*size_factor + delt[1],0 + delt[2]);

		//-x, -y, 0
		glVertex3f(-100*size_factor + delt[0],-100*size_factor + delt[1],0 + delt[2]);
		//-x, y, 0
		glVertex3f(-100*size_factor + delt[0],100*size_factor + delt[1],0 + delt[2]);
		
		// -x, -y , 0
		glVertex3f(-100*size_factor + delt[0],-100*size_factor + delt[1],0 + delt[2]);
		// x, -y , 0
		glVertex3f(100*size_factor + delt[0],-100*size_factor + delt[1],0 + delt[2]);
		
		
   }
glEnd();  
glLineWidth(1);
glEnable(GL_LIGHTING);

if (g_grid_plane !=0)
  {

glMaterialfv(GL_FRONT,GL_AMBIENT,jaune);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,jaune);
  glMaterialfv(GL_FRONT,GL_SPECULAR,jaune);
  glMaterialfv(GL_FRONT,GL_SHININESS,jaune);
  glMaterialfv(GL_FRONT,GL_EMISSION,jaune);

  glRasterPos3f(101*size_factor + delt[0],0.0f + delt[1],0.0f + delt[2]);

printString("x");
glRasterPos3f(101*size_factor + delt[0],0.0f + delt[1],0.0f + delt[2]);
printString("x");


glRasterPos3f(-101*size_factor + delt[0],0.0f + delt[1],0.0f + delt[2]);
printString("-x");
glRasterPos3f(-101*size_factor + delt[0],0.0f + delt[1],0.0f + delt[2]);
printString("-x");
}

if (g_grid_plane !=1)
  {

glMaterialfv(GL_FRONT,GL_AMBIENT,orange);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,orange);
  glMaterialfv(GL_FRONT,GL_SPECULAR,orange);
  glMaterialfv(GL_FRONT,GL_SHININESS,orange);
  glMaterialfv(GL_FRONT,GL_EMISSION,orange);

glRasterPos3f(0.0f + delt[0],101*size_factor + delt[1],0.0f + delt[2]);
printString("y");
glRasterPos3f(0.0f + delt[0],101*size_factor + delt[1],0.0f + delt[2]);
printString("y");
glRasterPos3f(0.0f + delt[0],-101*size_factor + delt[1],0.0f + delt[2]);
printString("-y");

}
if (g_grid_plane !=2)
  {
glMaterialfv(GL_FRONT,GL_AMBIENT,rouge);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,rouge);
  glMaterialfv(GL_FRONT,GL_SPECULAR,rouge);
  glMaterialfv(GL_FRONT,GL_SHININESS,rouge);
  glMaterialfv(GL_FRONT,GL_EMISSION,rouge);

glRasterPos3f(0.0f + delt[0],0.0f + delt[1],101*size_factor + delt[2]);
printString("z");
glRasterPos3f(0.0f + delt[0],0.0f + delt[1],101*size_factor + delt[2]);
printString("z");
glRasterPos3f(0.0f + delt[0], 0.0f + delt[1], -101 * size_factor + delt[2]);
printString("-z");
glRasterPos3f(0.0f + delt[0], 0.0f + delt[1], -101 * size_factor + delt[2]);
printString("-z");

}
glMaterialfv(GL_FRONT,GL_AMBIENT,blanc);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,blanc);
  glMaterialfv(GL_FRONT,GL_SPECULAR,blanc);
  glMaterialfv(GL_FRONT,GL_SHININESS,blanc);
  glMaterialfv(GL_FRONT,GL_EMISSION,blanc);

}

float MeshTools::Get_100pixels_in_mm()
{
	float numpix_1mm = getTranslationFactor();
	return 100/numpix_1mm;

}
void MeshTools::Set_100pixels_in_mm (float val)
{
	float numpix_1mm = getTranslationFactor();
	float current_100_pixels = 100/numpix_1mm;
	float factor = current_100_pixels/val;
	zoom = zoom/factor;
	Cont_Mesh.dmean = this->zoom/10;
	Cont_Mesh.dmean_ok =0;
	this->redraw();
}







enum RETVALUE
{
	SUCCESS,
	ERROR_API_CALL_FAILED,
	ERROR_FAILURE_WHILE_LOADING_LIBRARY,
	ERROR_OS_VERSION_NOT_SUPPORTED,
	ERROR_SOFTWAREKEY_NOT_FOUND,
	ERROR_CONVERSION_CHAR_2_WCHAR_FAILED
};


void MeshTools::color_getgridcolor (uchar *r, uchar *g, uchar *b)
	{
	int r1, g1, b1;

	
	r1 = floor((double)color_grid[0]*255);
	g1 = floor((double)color_grid[1]*255);
	b1 = floor((double)color_grid[2]*255);
 

	*r = r1;
	*g = g1;
	*b = b1;

	}


void MeshTools::color_getbackcolor (uchar *r, uchar *g, uchar *b)
	{
	int r1, g1, b1;

	
	r1 = floor((double)color_back[0]*255);
	g1 = floor((double)color_back[1]*255);
	b1 = floor((double)color_back[2]*255);
 

	*r = r1;
	*g = g1;
	*b = b1;

	}

float MeshTools::GetSpecular()
{

return specular;
}
float MeshTools::GetAmbiant()
{

return ambiant;
}

float MeshTools::GetDiffuse()
{

return diffuse;
}
void MeshTools::set_tag_color (int tag_id, uchar r, uchar g, uchar b)
{
	if (tag_id>=0 && tag_id<25)
	{
		g_tag_colors[tag_id][0]=r;
		g_tag_colors[tag_id][1]=g;
		g_tag_colors[tag_id][2]=b;
		g_tag_colors[tag_id][0]=g_tag_colors[tag_id][0]/255;
		g_tag_colors[tag_id][1]=g_tag_colors[tag_id][1]/255;
		g_tag_colors[tag_id][2]=g_tag_colors[tag_id][2]/255;
	}
	this->Update_RGB();
}
void MeshTools::set_tag_alpha (int tag_id, int val)
{
	if (tag_id>=0 && tag_id<25)
	{
		g_tag_colors[tag_id][3]=(float)val/100;		
		Cont_Mesh.Update_RGB();
	}
}

int MeshTools::get_tag_alpha (int tag_id)
{
	if (tag_id>=0 && tag_id<25)
	{
		return (int)(g_tag_colors[tag_id][3]*100);		
	}
	else
	{return 100;}
}
std::string MeshTools::get_tag_label (int tag_id)
{
	if (tag_id>=0 && tag_id<25)
	{
		return g_tag_labels[tag_id];
	}
	else
	{
		std::string wrong = "Wrong";
		return wrong;
	}

}
void MeshTools::get_orientation_labels (std::string &z_plus,
									   std::string &z_minus,
									   std::string &y_plus,
									   std::string &y_minus,
									   std::string &x_plus,
									   std::string &x_minus
									   )
{
	z_plus = g_orientation_labels[0].c_str();
	z_minus = g_orientation_labels[1].c_str();
	y_plus = g_orientation_labels[2].c_str();
	y_minus = g_orientation_labels[3].c_str();
	x_plus = g_orientation_labels[4].c_str();
	x_minus = g_orientation_labels[5].c_str();
}

void MeshTools::set_orientation_labels (std::string z_plus,
									   std::string z_minus,
									   std::string y_plus,
									   std::string y_minus,
									   std::string x_plus,
									   std::string x_minus
									   )
{
	g_orientation_labels[0] = z_plus.c_str();
	g_orientation_labels[1] = z_minus.c_str();
	g_orientation_labels[2] = y_plus.c_str();
	g_orientation_labels[3] = y_minus.c_str();
	g_orientation_labels[4] = x_plus.c_str();
	g_orientation_labels[5] = x_minus.c_str();
}

std::string MeshTools::get_flag ()
{

	OBJECT_LANDMARK * My_Flag= NULL;
	My_Flag = Cont_Mesh.Get_Selected_Landmark(2);
	if (My_Flag!=NULL)
	{return My_Flag->flag_label;}

	std::string nothing = "No flag currently selected";
	return nothing;


}

float MeshTools::get_flag_length ()
{

	OBJECT_LANDMARK * My_Flag= NULL;
	My_Flag = Cont_Mesh.Get_Selected_Landmark(2);
	if (My_Flag!=NULL)
	{return My_Flag->flag_length;}

	float length  = 25;
	return 25;


}

float MeshTools::get_g_flag_length ()
{

	
	return g_flag_length;


}
void MeshTools::get_flag_color (uchar *r, uchar *g, uchar *b)
{
	OBJECT_LANDMARK * My_Flag= NULL;
	My_Flag = Cont_Mesh.Get_Selected_Landmark(2);
	int r1, g1, b1;
	if (My_Flag!=NULL)
	{
		

	
		r1 = floor((double)My_Flag->color[0]*255);
		g1 = floor((double)My_Flag->color[1]*255);
		b1 = floor((double)My_Flag->color[2]*255);
	 
		
	}
	else 
	{
		r1=125;
		g1=125;
		b1=125;
		
	}
	*r = r1;
	*g = g1;
	*b = b1;


}
void MeshTools::get_g_flag_color (uchar *r, uchar *g, uchar *b)
{
	
	int r1, g1, b1;
	
		

	
	r1 = floor((double)g_flag_color[0]*255);
	g1 = floor((double)g_flag_color[1]*255);
	b1 = floor((double)g_flag_color[2]*255);
	 
		
	
	*r = r1;
	*g = g1;
	*b = b1;


}	


	void MeshTools::set_flag ( std::string label, float length)
	{
		//
	OBJECT_LANDMARK * My_Flag= NULL;
	My_Flag = Cont_Mesh.Get_Selected_Landmark(2);
	if (My_Flag!=NULL)
	{
		My_Flag->flag_label = label.c_str();
		My_Flag->flag_length = length;
		My_Flag->selected=0;
	}
		
	}

void MeshTools::set_flag_colour (uchar r, uchar g, uchar b)
{
	GLfloat flag_color[4];
	
	flag_color[0] = r;
	flag_color[1] = g;
	flag_color[2] = b;
	flag_color[0] = flag_color[0]/255;
	flag_color[1] = flag_color[1]/255;
	flag_color[2] = flag_color[2]/255;

	OBJECT_LANDMARK * My_Flag= NULL;
	My_Flag = Cont_Mesh.Get_Selected_Landmark(2);
	if (My_Flag!=NULL)
	{
		My_Flag->color[0] = flag_color[0];
		My_Flag->color[1] = flag_color[1];
		My_Flag->color[2] = flag_color[2];
			
	}

}
void MeshTools::set_g_flag_colour (uchar r, uchar g, uchar b)
{
	
	
	g_flag_color[0] = r;
	g_flag_color[1] = g;
	g_flag_color[2] = b;
	g_flag_color[0] = g_flag_color[0]/255;
	g_flag_color[1] = g_flag_color[1]/255;
	g_flag_color[2] = g_flag_color[2]/255;
	

}

void MeshTools::set_all_flag (uchar r, uchar g, uchar b, float length)
{
	
	GLfloat flag_color[4];
	
	flag_color[0] = r;
	flag_color[1] = g;
	flag_color[2] = b;
	flag_color[0] = flag_color[0]/255;
	flag_color[1] = flag_color[1]/255;
	flag_color[2] = flag_color[2]/255;

	OBJECT_LANDMARK * My_Flag= NULL;


		My_Flag =  Cont_Mesh.landmarkafter (0, 2); // get first flag
				
		int Ok = 1;
		if (My_Flag==NULL)
		{Ok = 0;}	

		while (Ok!=0)
		{       						
				if (My_Flag->selected ==1)
				{
					My_Flag->color[0] = flag_color[0];
					My_Flag->color[1] = flag_color[1];
					My_Flag->color[2] = flag_color[2];
					My_Flag->flag_length = length;
				}
				My_Flag->selected=0;

				if (My_Flag->nextobj!=NULL)
				{My_Flag = My_Flag->nextobj;}
				else{Ok = 0;}		
		}//while
					
}

void MeshTools::set_flag_length (float length)
{
	// set length of only one flag (the selected one!)
	OBJECT_LANDMARK * My_Flag= NULL;
		My_Flag = Cont_Mesh.Get_Selected_Landmark(2);
		if (My_Flag!=NULL && length >0)
		{
			My_Flag->flag_length = length;
			My_Flag->selected=0;						
			
		}
}
void MeshTools::set_g_flag_length (float length)
{
	if (length>0){g_flag_length=length;}
}
void MeshTools::set_tag_label (int tag_id, std::string label)
{
	if (tag_id>=0 && tag_id<25)
	{
		g_tag_labels[tag_id]= label.c_str();
	}

}

void MeshTools::get_tag_color (int tag_id, uchar *r, uchar *g, uchar *b)
{
	if (tag_id>=0 && tag_id<25)
	{
			int r1, g1, b1;

	
		r1 = floor((double)g_tag_colors[tag_id][0]*255);
		g1 = floor((double)g_tag_colors[tag_id][1]*255);
		b1 = floor((double)g_tag_colors[tag_id][2]*255);
	 
		*r = r1;
		*g = g1;
		*b = b1;
	}
}
void MeshTools::color_getobjcolor (uchar *r, uchar *g, uchar *b)
	{
	int r1, g1, b1;

	
	r1 = floor((double)color_obj[0]*255);
	g1 = floor((double)color_obj[1]*255);
	b1 = floor((double)color_obj[2]*255);
 

	*r = r1;
	*g = g1;
	*b = b1;

	}






    void MeshTools::color_setgridcolor (uchar r, uchar g, uchar b)
	{
	color_grid[0] = r;
	color_grid[1] = g;
	color_grid[2] = b;
	color_grid[0] = color_grid[0]/255;
	color_grid[1] = color_grid[1]/255;
	color_grid[2] = color_grid[2]/255;

	}
void MeshTools::color_setbackcolor (uchar r, uchar g, uchar b)
	{
	color_back[0] = r;
	color_back[1] = g;
	color_back[2] = b;
	color_back[0] = color_back[0]/255;
	color_back[1] = color_back[1]/255;
	color_back[2] = color_back[2]/255;

	}
float MeshTools::Get_Grid_Size()
{
	return this->grid_size;	

}

void MeshTools::Set_Grid_Size(float size)
{
	this->grid_size = size;	

}

int MeshTools::Get_Blend()
{
	return this->blend_value;	

}

void MeshTools::Set_Blend(int blend_value)
{
	g_lambda = blend_value;
	this->blend_value = blend_value;
	this->Cont_Mesh.Set_Blend(blend_value);
	this->redraw();
	this->valid(0);
}

void MeshTools::color_setobjcolor (uchar r, uchar g, uchar b)
	{
	OBJECT_MESH * My_Obj;
	OBJECT_LOG * My_Log;
	color_obj[0] = r;
	color_obj[1] = g;
	color_obj[2] = b;
	color_obj[0] = color_obj[0];
	color_obj[1] = color_obj[1];
	color_obj[2] = color_obj[2];

	int Ok = 1;
	if (Cont_Mesh.OBJECTS_ROOT->OBJECTS !=NULL)
	{My_Obj = Cont_Mesh.OBJECTS_ROOT->OBJECTS;}
	else {Ok = 0;}
	while (Ok!=0)
	{       
		
		if (My_Obj->selected ==1)
		{
			My_Obj->color[0] = color_obj[0];
			My_Obj->color[1] = color_obj[1];
			My_Obj->color[2] = color_obj[2];
			My_Obj->Update_RGB();
			//My_Obj->color[3] = 1;
			My_Obj->selected =0;
		}
		
		if (My_Obj->nextobj!=NULL)
		{My_Obj = My_Obj->nextobj;}
		else{Ok = 0;}		
	}//while
	Ok=1;
	if (Cont_Mesh.OBJECTS_ROOT->OBJECTS_LOG !=NULL)
	{My_Log = Cont_Mesh.OBJECTS_ROOT->OBJECTS_LOG;}
	else {Ok = 0;}
	while (Ok!=0)
	{       
		
		if (My_Log->selected ==1)
		{
			My_Log->color[0] = color_obj[0];
			My_Log->color[1] = color_obj[1];
			My_Log->color[2] = color_obj[2];
			//My_Log->color[3] = 1;
			My_Log->selected =0;
		}
		
		if (My_Log->nextobj!=NULL)
		{My_Log = My_Log->nextobj;}
		else{Ok = 0;}		
	}//while

	
}
void MeshTools::Cam_Centre_At_Landmark(int landmark_number)
{
	if (landmark_number>=0)
	{
		this->Cont_Mesh.Get_Landmark_Coordinates(landmark_number, &camera.atx, &camera.aty, &camera.atz, this->landmark_mode);
		g_mode_cam_centre_of_mass = 0;
	}
	else if  (landmark_number==-1)
	{
		camera.atx = 0;
		camera.aty = 0;
		camera.atz = 0;
		camera.tx = 0;
		camera.ty = 0;
		camera.tz = -100;
		g_mode_cam_centre_of_mass = 0;
	}
	else
	{
		camera.atx = g_mean_all[0];
		camera.aty = g_mean_all[1];
		camera.atz = g_mean_all[2];
		camera.tx = -g_mean_all[0];
		camera.ty = -g_mean_all[1];
		camera.tz = camera.tz-g_mean_all[2];
		g_mode_cam_centre_of_mass = 1;
	}

}

//static clock_t current_time = 0;/// pour calculer le FPS
//static double last_time = 0;

void MeshTools::draw() {

	//last_time = (double)current_time;
	
	if (!valid()) 
	{

        glLoadIdentity();
		makeRasterFont();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
		GLfloat light_ambient[] = { ambiant*1.0, ambiant*1.0, ambiant*1.0, 1.0 };
		GLfloat light_diffuse[] = { diffuse*1.0, diffuse*1.0, diffuse*1.0, 1.0 };
		GLfloat light_specular[] = { specular*1.0, specular*1.0, specular*1.0, 1.0 };
		GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };

		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	    
	 
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		GLfloat glfLight[] = {-4.0f, 4.0f, 4.0f, 0.0f};
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	 
	    glDepthFunc(GL_LEQUAL);
	    glEnable(GL_DEPTH_TEST);

	    glEnable(GL_NORMALIZE);

	  glShadeModel(GL_SMOOTH);
	
	  glEnable(GL_BLEND);
	  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	  
	  int red_bits, green_bits, blue_bits;
	  struct {GLint x, y, width, height;} viewport;
	  glEnable(GL_DEPTH_TEST);	/* turn on z-buffer */
	  
	  glGetIntegerv(GL_RED_BITS, &red_bits);
	  glGetIntegerv(GL_GREEN_BITS, &green_bits);
	  glGetIntegerv(GL_BLUE_BITS, &blue_bits);
	  glGetIntegerv(GL_VIEWPORT, &viewport.x);
	  std::cout<<"OpenGL version:"<<glGetString(GL_VERSION)<<endl;
	  std::cout<<"OpenGL window has"<< red_bits<<" bits red, "<<green_bits<<" green, "<<blue_bits<<" blue; viewport is "<<viewport.width<<"x"<<viewport.height<<std::endl;
	  
	  GLenum err = glewInit();
	  if (GLEW_OK != err)
	  {
		  ////	/* Problem: glewInit failed, something is seriously wrong. */
		  fprintf(stdout, "GLEW Error: %s\n", glewGetErrorString(err));
	  }
	  fprintf(stdout, "GLEW Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	  /* setup perspective camera with OpenGL */
	  
	  
	  /* from here on we're setting modeling transformations */
	 
	 
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();

	  glOrtho( -(double)(zoom*(1-g_zoom/100))* w()/h(),
		   (double)(zoom*(1-g_zoom/100))* w()/h(),
		   -(double)(zoom*(1-g_zoom/100)), 
		   (double)(zoom*(1-g_zoom/100)), 
		   camera.near1, 
		   camera.far1);

	  /* from here on we're setting modeling transformations */
	  glMatrixMode(GL_MODELVIEW);
	  
	  glLoadIdentity();

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		   glFrontFace(GL_CW);


		   glEnable(GL_POLYGON_OFFSET_LINE);
		   glPolygonOffset(-1, -1);
    }// fin if valid

	if (bool_change_mode==1)
	{
		drawDispMode();
	}



	glClearColor(color_back[0], color_back[1], color_back[2], color_back[3]);
	//cout << color_back[0] << "," << color_back[1] << "," << color_back[2] << "," << color_back[3];
	//Reset interface values when dmean_ok has changed
	if (Cont_Mesh.dmean_ok ==0)
	{

		if (Cont_Mesh.dmean!=0)
		{
		
			if (g_fov_adapt==0)
			{
				camera.tz = -100;
				camera.far1 = 200;
			}
			else
			{
				float opt_fov_depth = this->Get_Optimal_FOV_Depth();
				camera.far1 = opt_fov_depth;
				camera.tz = -opt_fov_depth/2;	

				
			}
		}
		//Output 
		Fl_Group* Pointeur00 = (Fl_Group*)(this->parent());//Bottom group
		Fl_Group* Pointeur01 = (Fl_Group*)(Pointeur00->parent());//Main Window group
		Fl_Group* Pointeur02 =NULL;
		Pointeur00 = (Fl_Group*)(Pointeur01->child(0));//Top group
		Pointeur01 = (Fl_Group*)(Pointeur00->child(0));//Unresizable group
	
		Pointeur00 = (Fl_Group*)(this->parent());//Bottom group
		Pointeur01 = (Fl_Group*)(Pointeur00->child(3));//SS group
		//SS group
		//Xpan slider

		//Xpan1 slider
		Pointeur02 = (Fl_Group*)(Pointeur01->child(0)); // SS_unresizable
		SpecialSlider2* Pointeur3 = (SpecialSlider2*)(Pointeur02->child(0));//Xpan1 widget
		Pointeur3->maximum(Pointeur3->value() + Cont_Mesh.dmean);
		Pointeur3->minimum(Pointeur3->value() - Cont_Mesh.dmean);
		Pointeur3->step(Cont_Mesh.dmean/1000);	

		//EE group
		//Zpan slider
		Pointeur01 = (Fl_Group*)(Pointeur00->child(2));//EE group
		Pointeur02 = (Fl_Group*)(Pointeur01->child(0)); // EE_unresizable
		SpecialSlider2* Pointeur2 = (SpecialSlider2*)(Pointeur02->child(0));//Zpan widget
		Pointeur2->maximum(Pointeur2->value() + 5*Cont_Mesh.dmean);
		Pointeur2->minimum(Pointeur2->value() - 5*Cont_Mesh.dmean);
		Pointeur2->step(Cont_Mesh.dmean/100);
		//Ypan slider
		
		/*SpecialSlider3* Pointeur4 = (SpecialSlider3*)(Pointeur02->child(2));//Zoom widget
		Pointeur4->changevalue(10*Cont_Mesh.dmean);
		Pointeur4->minimum(3*Pointeur4->value()/2);
		Pointeur4->maximum(Pointeur4->value()/2);
		Pointeur4->step(Cont_Mesh.dmean/100);
		*/
		//WW group
		//Zpan1 slider
		Pointeur01 = (Fl_Group*)(Pointeur00->child(1));//WW group
		Pointeur02 = (Fl_Group*)(Pointeur01->child(0)); // WW_unresizable
		Pointeur3 = (SpecialSlider2*)(Pointeur02->child(0));//Zpan1 widget
		Pointeur3->maximum(Pointeur3->value() + Cont_Mesh.dmean);
		Pointeur3->minimum(Pointeur3->value() - Cont_Mesh.dmean);
		Pointeur3->step(Cont_Mesh.dmean/1000);

		//Ypan1 slider
		Pointeur3 = (SpecialSlider2*)(Pointeur02->child(2));//Ypan1 widget
		Pointeur3->maximum(Pointeur3->value() + Cont_Mesh.dmean);
		Pointeur3->minimum(Pointeur3->value() - Cont_Mesh.dmean);
		Pointeur3->step(Cont_Mesh.dmean/1000);
	

		this->zoom = 10*Cont_Mesh.dmean;
		Cont_Mesh.dmean_ok =1;
		this->redraw();
	}

	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
if (this->showorientation ==1){DrawOrientations(70,70,40);}
	if(Disp_Scalars_Mode ==1)
	{
		
		
			this->DrawColorScale(w()-300,h()-40,250,25);
		
		
		
	}

	if(Disp_Tags_Mode ==1)
	{
		
				this->DrawColorScale(w()-700,h()-40,650,25);
		
		
		
	}

	if ((this->affiche_rect ==1))
	{
		glPushMatrix();
		//this->AfficheRectangle2(this->rect_x1, this->rect_x2, this->rect_y1, this->rect_y2, jaune,w(),h());
		this->AfficheRectangle2(this->rect_x1, this->rect_x2, this->rect_y1, this->rect_y2, jaune);
		glPopMatrix();
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	  
	  glOrtho( -(double)(zoom*(1-g_zoom/100))* w()/h(),
		   (double)(zoom*(1-g_zoom/100))* w()/h(),
		   -(double)(zoom*(1-g_zoom/100)), 
		   (double)(zoom*(1-g_zoom/100)), 
		   camera.near1, 
		   camera.far1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();    
	glMatrix cam;
	get_camera_transform_matrix(cam);
	glMultMatrixf((GLfloat *)cam);

	if (this->showgrid ==1){drawGrid();}
	

	glEnable(GL_LIGHTING);

	GLfloat curSize;
	curSize = 5;
	glPointSize(curSize);
  
	if (this->disp_curve == 1)
	{
		 Mesh_draw_Curve();  	
	}

	/// dessiner les correspondances si bool_ICP = true 
	// sinon remet la couleur par défaut à la source.
	if (bool_ICP == true){
		if (!Cont_Mesh.ExistAlwaysMeshLandmarkICP(registration->bool_onlyMatching) ){
			bool_ICP = false;
			free(registration);
		}
		else{
			Mesh_draw_ICPMode_info(300,12);
			registration->Draw_matching();
		}
	}

	
	Mesh_draw_Obj(this->modelandmark, bool_change_pos_obj);/// dessine les objects et landmarks 
	
	
	
	if (Cont_Mesh.Get_Landmark_Number(0)>0||Cont_Mesh.Get_Landmark_Number(1)>0||Cont_Mesh.Get_Landmark_Number(2)>0)
	{
		Mesh_draw_landmark_infos(5,12);

	}
	if (g_tag_mode>0)
	{
		Draw_Warning_Invertion (w()-300, 12);
	}

	if (g_mode_tool == 1 || g_mode_tool==2 || g_mode_tool==3)
	{

		Mesh_Draw_Polygon();
	}

	

	#ifndef MESA
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA
    glPopMatrix();

	//current_time = clock();
	//double fps = 1.0 / ((double)(current_time - last_time) / CLOCKS_PER_SEC);
	//cout << "fps  ::  " << fps << endl;
}


int MeshTools::handle(int event) 
{
  int handled = 1;
  
  
  static int prev_x, prev_y;
  int delta_x=0, delta_y=0; 
  	prev_x=mouse.x;
    prev_y=mouse.y;
if (g_mode_tool == 0)
{
	this->cursor(FL_CURSOR_DEFAULT);
  switch(event) {
	  case FL_MOUSEWHEEL:
		  this->SetZoom(3*Fl::event_dy());
		  rollinit_camera();
	 
	 break;
	 case FL_MOVE:
	 case FL_ENTER:
	 
		Fl::focus(this);
		break;
  case FL_RELEASE:
    mouse.x = (Fl::event_x());
    mouse.y = (Fl::event_y());    
    mouse.button = 0;
	bool_change_pos_obj = 0;
	if ((affiche_rect ==1))
	{
		this->Mesh_Select(this->rect_x1, this->rect_x2, this->rect_y1, this->rect_y2, select_mode);
		
	}
	else{
		if (tx2!=0 || ty2!=0 || tz2!=0 || rotx2!=0 || roty2!=0 || rotz2!=0)
			rollinit_objects();
		else
			rollinit_camera();
	}
	this->affiche_rect = 0;
	kpressed =0;
	kpressed2 =0;

    break;

  case FL_PUSH:

	  Fl::focus(this);
	    mouse.x = (Fl::event_x());
    mouse.y = (Fl::event_y());
	//std::cout<<"Fl_push... kpressed="<<kpressed<<std::endl;
	this->rect_x1 =	this->rect_x2 = mouse.x;
	this->rect_y1 =  this->rect_y2= h()- mouse.y;
    mouse.button = (Fl::event_button());
	if (mouse.button ==3)
	{

		this->affiche_rect =1;
		
		//Mesh_Select(mouse.x,mouse.y);
		//this->Mesh_Select(mouse.x-2, mouse.x+2, (h()- mouse.y)-2, (h()- mouse.y)+2, select_mode);
		this->rect_x1= mouse.x-2;
		this->rect_x2=mouse.x+2;
		this->rect_y1=(h()- mouse.y)-2;
		this->rect_y2=(h()- mouse.y)+2;
	}
	
	if (kpressed ==108) //"L" for "Landmark".
	{
		if (mouse.button ==1)
		{
			Mesh_CreateLandmarkAtMouse(mouse.x,mouse.y);
		}
		else if (mouse.button ==3)
		{Mesh_MoveLandmarkAtMouse(mouse.x,mouse.y);}
	}
    else if (kpressed ==116) //"T" for "Tags!".
	{
		
		if (mouse.button ==3)
		{
			//std::cout<<"Erase Tag"<<std::endl;
			Mesh_Tag(mouse.x,mouse.y, 0);
		}
		else if (mouse.button ==1)
		{
			//std::cout<<"Tag surface"<<std::endl;
			Mesh_Tag(mouse.x,mouse.y, 1);
		}
	}

    break;
  case FL_DRAG:
		
    mouse.x = (Fl::event_x());
    mouse.y = (Fl::event_y());
    delta_x=mouse.x-prev_x;
    delta_y=mouse.y-prev_y; 
  	  
	this->rect_x2= mouse.x;
	this->rect_y2 = h()- mouse.y;
	
	if ((this->select_mode ==0 && kpressed !=FL_Control_L )
		|| (this->select_mode ==3 && kpressed !=FL_Control_L) 
		|| (kpressed ==FL_Control_L && (this->select_mode ==2 || this->select_mode ==1)))
	{
		if(mouse.button == 1)
		{ 
			if (kpressed !=108) //"L"
			{

				if(abs(delta_x) > abs(delta_y))
				{			 
					rotz += (GLdouble) (delta_x);
					//rollinit_camera(); 
				}
				else        
				{		 
					roty  += (GLdouble) (delta_y); 		  
				//rollinit_camera();
				}
				//rollinit_camera();
			}
			else
			{Mesh_MoveLandmarkAtMouse(mouse.x,mouse.y);}
		}
		else if((mouse.button==2))
		{			
			tx += (GLdouble) delta_x / (getTranslationFactor());
			ty -= (GLdouble) delta_y /(getTranslationFactor());
			
		}
	}
	if ((this->select_mode ==2 && kpressed !=FL_Control_L ) ||
		(this->select_mode ==0 && kpressed ==FL_Control_L )||
		(this->select_mode ==3 && kpressed ==FL_Control_L )
		)
	{		
		bool_change_pos_obj = 1;
		if(mouse.button == 1)
		{ 
			if (kpressed !=108) //"L"
			{

				if(abs(delta_x) > abs(delta_y))
				{		
					rotz2 += (GLdouble) (delta_x); 
				}
				else        
				{		
					roty2  += (GLdouble) (delta_y); 		  
				}
			}
			else{Mesh_MoveLandmarkAtMouse(mouse.x,mouse.y);}
		}

		else if(mouse.button==2)
		{
			ty2 += (GLdouble) delta_x/(getTranslationFactor());
			tz2 -= (GLdouble) delta_y/(getTranslationFactor());
		}
	}
  
    break;
  case FL_FOCUS :
  case FL_UNFOCUS :
    return 1;

  
  case FL_KEYDOWN:
	   /// EVENT
	if (Fl::event_key() ==FL_Control_L || Fl::event_key() ==FL_Control_R)
		{
			kpressed = FL_Control_L;
		}
	if (Fl::event_key() ==FL_Delete)
		{
			kpressed = FL_Delete;
		}
	if (Fl::event_key() ==108) //L
		{kpressed = 108;}
	if (Fl::event_key() ==116) //T
		{kpressed = 116;
	}
	
	if (Fl::event_key() ==97)//A
		{kpressed = 97;}
	if (Fl::event_key() ==100)//D
		{kpressed = 100;}
	if (Fl::event_key() == 32)// Space
	{	kpressed = 32;}
	
	/// RESULT ACCORDING TO KEY PRESSED
	if ( kpressed == FL_Delete) // Suppr
	{
		this->Delete();
		this->redraw();
		kpressed = 0;
	}
	if (kpressed ==99) // C = close polygon
	{poly.Polygon_close();}
	if (kpressed2==0){kpressed2=kpressed;}

	if (kpressed == 97 && kpressed2 == FL_Control_L) // CTRL + A
	{
		this->Mesh_SelectAll();
		this->redraw();
		kpressed=0;
		kpressed2=0;
	}
	if (kpressed == 100 && kpressed2 == FL_Control_L) //CTRL +D
	{
		this->Mesh_UnselectAll();
		this->redraw();
		kpressed=0;
		kpressed2=0;
	}
	if (kpressed == 32){ // SPACE
		/// fais changer le mode transformation de ICP
		if (bool_ICP == true){
			if (!Cont_Mesh.ExistAlwaysMeshLandmarkICP(registration->bool_onlyMatching)){
				bool_ICP = false;
				free(registration);
			}
			else{
				registration->SetMode();
				this->redraw();
			}
		}
		this->redraw();
	}
	break;
  case FL_KEYUP:
	kpressed =0;
	kpressed2 =0;
	break;
  default:
    // pass other events to the base class...
	prev_x=mouse.x;
    prev_y=mouse.y;
	 handled= Fl_Gl_Window::handle(event);
  }
  }
  else // MODE LASSO (mode 1 : delete // mode 2 : tag + // mode 3 : tag -
  {
	  this->cursor(FL_CURSOR_CROSS);
	switch(event) {
	 case FL_MOVE:
	 case FL_ENTER:
			mouse.x = (Fl::event_x());
		mouse.y = (Fl::event_y());   
		Fl::focus(this);
		break;
   case FL_RELEASE:
	mouse.button =0;
    mouse.x = (Fl::event_x());
    mouse.y = (Fl::event_y());       
    break;

  case FL_PUSH:
	  mouse.button = (Fl::event_button());
	 Fl::focus(this);	 
    mouse.x = (Fl::event_x());
    mouse.y = (Fl::event_y());	
	//this->rect_y1 =  this->rect_y2= h()- mouse.y;
	if (kpressed !=116) // if kpressed !=T
	{
		std::cout<<"kpressed="<<kpressed;
		if(mouse.button == 1)
		{
			if (poly.statut == 0)
			{
				poly.Polygon_add(mouse.x,h() -mouse.y);
			}
			else
			{
				int rep;
				POLYGON_VERTEX clic;
				clic.x = mouse.x;
				clic.y = h() - mouse.y;
				rep = poly.POLYGON_POINT_INSIDE(clic);
			
			}
		}
		if (mouse.button ==2 || (mouse.button ==3 && kpressed==99)) // Middle click or Right click + C
		{
				if (poly.statut ==1)
				{
					if (g_mode_tool ==1)
					{
						//delete marked vertices
						Cont_Mesh.Mesh_Lasso_Vertices(h(),w(),mouse.x,h() -mouse.y, 0);
					}
					else if (g_mode_tool == 2) // lasso +
					{
						// tag vertices
						Cont_Mesh.Mesh_Lasso_Vertices(h(),w(),mouse.x,h() -mouse.y, 1);
					}
					else // lasso -
					{
						Cont_Mesh.Mesh_Lasso_Vertices(h(),w(),mouse.x,h() -mouse.y, 2);
					}

				}
				poly.Polygon_init();
				g_mode_tool = 0;
		}
		if(mouse.button == 3)
		{
		
			poly.Polygon_close();
		}
	}
	else // l\E0 on est en mode lasso et on a press\E9 TAG!
	{
		if (poly.statut ==1)
		{

			if(mouse.button == 1) // left click
			{
			
					std::cout<<"Tag within lasso"<<std::endl;
					Mesh_Tag_Within_Lasso (mouse.x, mouse.y, mouse.x,h() -mouse.y, 1); // 3 : tag within laso
					//delete marked vertices
					//Cont_Mesh.Mesh_Lasso_Vertices(h(),w(),mouse.x,h() -mouse.y, 0);
						
			}

			else if (mouse.button ==3)
			{
				std::cout<<"Erase within lasso"<<std::endl;
					Mesh_Tag_Within_Lasso (mouse.x, mouse.y, mouse.x,h() -mouse.y, 0);// 2 : clear within lasso
			
			}
				
			poly.Polygon_init();
			g_mode_tool = 0;
		}
	
	
	}
    break;
 
  case FL_FOCUS :
  case FL_UNFOCUS :
    return 1;  
  case FL_KEYDOWN:
	if (Fl::event_key() ==FL_Control_L)
		{
			kpressed = FL_Control_L;
		}
	if (Fl::event_key() ==FL_Delete)
	{
		kpressed = FL_Delete;
	}
	if (Fl::event_key() ==108) //L
	{kpressed = 108;}
	
	if (Fl::event_key() ==97) // A
		{kpressed = 97;}
	if (Fl::event_key() ==99) // C
		{kpressed = 99;}
	if (Fl::event_key() ==116) // T
		{kpressed = 116;}
	if (Fl::event_key() ==100)//D
		{kpressed = 100;}
	if (kpressed2==0){kpressed2=kpressed;}
	if ( kpressed == FL_Delete) // CTRL + A
	{
		this->Delete();
		this->redraw();
		kpressed = 0;
	}
	if (kpressed == 97 && kpressed2 == FL_Control_L) // CTRL + A
	{
		this->Mesh_SelectAll();
		this->redraw();
		kpressed=0;
		kpressed2=0;
	}
	if (kpressed == 100 && kpressed2 == FL_Control_L) //CTRL +D
	{
		this->Mesh_UnselectAll();
		this->redraw();
		kpressed=0;
		kpressed2=0;
	}
	break;
  case FL_KEYUP:
	kpressed =0;	
	kpressed2 =0;	
	break;
  default:
    // pass other events to the base class...
	prev_x=mouse.x;
    prev_y=mouse.y;
   handled= Fl_Gl_Window::handle(event);
  }
  }
  if ((event != FL_MOVE && event != FL_LEAVE && event !=FL_ENTER && event !=FL_PUSH
	  && event !=FL_KEYUP && event != FL_KEYDOWN) || g_mode_tool!=0
	  )
  {
	  //std::cout<<"EVENT"<<event<<std::endl;
	this->redraw();
  }
  
  return (handled);  // Returning one acknowledges that we handled this event
}
void MeshTools::Mesh_area_volume()
{
	Cont_Mesh.Mesh_area_volume();
}
void MeshTools::Change_Disp_Vertices_Ids()
{
	if (Disp_Vertices_Ids ==0)
	{Disp_Vertices_Ids = 1;}
	else
	{Disp_Vertices_Ids =0;}
}

void MeshTools::Update_RGB()
{

	Cont_Mesh.Update_RGB();
}
void MeshTools::Change_Scalars_Display_Mode()
{
	if (Disp_Scalars_Mode ==0)
	{Disp_Scalars_Mode = 1;}
	else
	{Disp_Scalars_Mode = 0;}
	Disp_Tags_Mode = 0;
}
void MeshTools::Change_Tags_Display_Mode()
{
	if (Disp_Tags_Mode ==0)
	{Disp_Tags_Mode = 1;}
	else
	{Disp_Tags_Mode = 0;}

	Disp_Scalars_Mode = 0;

}

void MeshTools::Desactivate_Scalars_Display_Mode()
{
	Disp_Scalars_Mode = 0;
	Disp_Tags_Mode = 0;

}
void MeshTools::Activate_Scalars_Display_Mode()
{
	
	Disp_Scalars_Mode = 1;
	Disp_Tags_Mode = 0;
	
}

void MeshTools::Desactivate_Tags_Display_Mode()
{
	Disp_Tags_Mode = 0;
	Disp_Scalars_Mode = 0;

}
void MeshTools::Activate_Tags_Display_Mode()
{
	Disp_Tags_Mode = 1;
	Disp_Scalars_Mode = 0;

}
void MeshTools::Set_Tag_Mode(int tag_mode)
{
	g_tag_mode = tag_mode;
}


void MeshTools::Change_Disp_Triangle_Ids()
{
	if (Disp_Triangle_Ids ==0)
	{Disp_Triangle_Ids = 1;}
	else
	{Disp_Triangle_Ids =0;}
}

void MeshTools::Ungroup()
{
	Cont_Mesh.Mesh_Ungroup();
	this->Compute_Global_Mean();
}
void MeshTools::Group()
{
	Cont_Mesh.Mesh_Group();
	this->Compute_Global_Mean();
}
void MeshTools::Mesh_Add_landmark()
{

	Cont_Mesh.Mesh_container_newlandmarkindex(this->landmark_mode);
	
}



void MeshTools::Mesh_UnselectAll()
{

	Cont_Mesh.Mesh_UnselectAll();
	this->Compute_Global_Mean();
	this->Compute_Global_Scalar_List();

}

void MeshTools::Mesh_SelectAll()
{

	Cont_Mesh.Mesh_SelectAll();
	this->Compute_Global_Mean();
	this->Compute_Global_Scalar_List();
}

void MeshTools::Object_Move_Down()
{
	 Cont_Mesh.Object_Move_Down();
}
void MeshTools::Object_Move_Up()
{
	 Cont_Mesh.Object_Move_Up();
}


void MeshTools::Landmark_Move_Down()
{
	 Cont_Mesh.Landmark_Move_Down();
}
void MeshTools::Landmark_Move_Up()
{
	 Cont_Mesh.Landmark_Move_Up();
}

float MeshTools::scalars_get_min()
{
	if (g_color_scale_id !=-1)
	{
		float min = Cont_Mesh.scalars_get_min();
		if ( min == 1000000000){return 0;}
		else return min;
		
	}
	else
	{
		return 0;
	}

}
float MeshTools::scalars_get_max()
{
	if (g_color_scale_id !=-1)
	{
		float max = Cont_Mesh.scalars_get_max();
		if ( max == -1000000000){return 1;}
		else return max;
		
	}
	else
	{
	  return 1.0;
	}
}


void MeshTools::AfficheRectangle2(int x1, int x2, int y1, int y2,  GLfloat couleur[4])
{

	glDisable(GL_LIGHTING);
	glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		//gluOrtho2D (0, w(), 0, h());
		glOrtho( 0, w(), 0, h(), -1, 1);
		glMatrixMode (GL_MODELVIEW); 
		glPushMatrix();
		glLoadIdentity(); 
	
  glColor4f(couleur[0],couleur[1],couleur[2],couleur[3]);
  glMaterialfv(GL_FRONT,GL_AMBIENT,couleur);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,couleur);
  glMaterialfv(GL_FRONT,GL_SPECULAR,couleur);
  glMaterialfv(GL_FRONT,GL_SHININESS,couleur);
  glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);
	

	glBegin(GL_LINES); // dessin du r\E9ctangle
	glVertex2d(x1,y1); 
	glVertex2d(x1,y2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2d(x1,y2);
	glVertex2d(x2,y2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2d(x2,y2);
	glVertex2d(x2,y1);
	glEnd();
	glBegin(GL_LINES);
	glVertex2d(x2,y1);
	glVertex2d(x1,y1); 
	glEnd();

	
	glPopMatrix();
	glEnable(GL_LIGHTING);
// FIN TMP
}

void MeshTools::AfficheTexte(int x, int y, char *chaine,  GLfloat couleur[4] )
{
	glDisable(GL_LIGHTING);
	glMaterialfv(GL_FRONT,GL_EMISSION,couleur);
  glMaterialfv(GL_FRONT,GL_AMBIENT,couleur);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,couleur);
  glMaterialfv(GL_FRONT,GL_SPECULAR,couleur);
  glMaterialfv(GL_FRONT,GL_SHININESS,high_shininess);
	//glMaterialfv(GL_FRONT,GL_EMISSION,blanc);
  	glColor4f(couleur[0],couleur[1],couleur[2],couleur[3]);	  
	glPushMatrix();
	
  	glPushAttrib(GL_LIST_BIT);
	glListBase(baseList);
	glCallLists((GLsizei)strlen(texte), GL_UNSIGNED_BYTE, texte);
	glPopAttrib();
	//Debut Move
	glPushAttrib(GL_TRANSFORM_BIT | GL_VIEWPORT_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	
	glViewport(x, y, 0, 0);
	
	glRasterPos4f(0, 0, 0, 1);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();

	//Fin Move
//	std::cout<<"chaine="<<chaine<<std::endl;
    printString(chaine);
	glPushAttrib(GL_LIST_BIT);
	glListBase(baseList);
	glCallLists((GLsizei)strlen(texte), GL_UNSIGNED_BYTE, texte);
	glPopAttrib();
	glPopMatrix();
	glEnable(GL_LIGHTING);
// FIN TMP
}





void MeshTools::Delete_volume_smaller_than(float volume)
{
//	if (this->mode == MODE_STL)
//	{ 
		if (volume >0)
		{		
			this->Mesh_DeleteSmallVolume(volume);
		}
//}
	
}

void MeshTools::Delete_smaller_than(int numtri)
{
//	if (this->mode == MODE_STL)
//	{ 
		if (numtri <1)
		{numtri=-1;}

		 this->Mesh_DeleteSmall(numtri);
//}
	
}



void MeshTools::Delete()
{
//	if (this->mode == MODE_STL)
//	{ 
		this->Mesh_Delete();
//}
this->Compute_Global_Mean();	
this->Compute_Global_Scalar_List();
}

void MeshTools::Mesh_DeleteSmallVolume (float volume)
{
	Cont_Mesh.Mesh_Delete(2, 100, volume);	
	this->Compute_Global_Mean();
	this->Compute_Global_Scalar_List();
}


void MeshTools::Mesh_DeleteSmall (int numtri)
{
	Cont_Mesh.Mesh_Delete(1, numtri, 0);		
	this->Compute_Global_Mean();
	this->Compute_Global_Scalar_List();
}

int MeshTools::Mesh_Delete()
{

		Cont_Mesh.Mesh_Delete(1, -1, 0);
		return 1;	

}


void MeshTools::Landmark_Mode(int mode)
{
	//0 : normal landmarks
	//1 : target landmarks / curve handles
	//2 : flags
	this->landmark_mode = mode;


}
void MeshTools::panx2(float x)
{

	ty2 = zoom*x/100;
	bool_change_pos_obj = 1;
}
void MeshTools::pany2(float y)
{

	tz2 = -zoom*y/100;
	bool_change_pos_obj = 1;
//}
}
void MeshTools::panz2(float z)
{
	
	tx2 = zoom*z/100;
	bool_change_pos_obj = 1;
}
void MeshTools::panz(float z)
{
		tz=zoom*z/100;
	};

void MeshTools::rollx(float x){

	rotz = x;
}
void MeshTools::rollx2(float x){

	rotz2 = x;
	bool_change_pos_obj = 1;
}
void MeshTools::rolly(float y){

	roty =y;

}
void MeshTools::rolly2(float y){

	roty2 =y;
	bool_change_pos_obj = 1;
}

void MeshTools::rollz(float z){

	rotx =-z;
}
void MeshTools::rollz2(float z){

	rotx2 =-z;
	bool_change_pos_obj = 1;


}

void MeshTools::rollinit_objects()
{
	// As far as I remember, it applies the rollz2, tz2 ... to the matrix of selected objects
	//TEST : PUT THIS IN CONTAINER_MESH
	Cont_Mesh.rollinit_objects();
	
	//glMatrix NewMat;
	float uvx[3], uvy[3], uvz[3];
	double PI = 3.1415926535897932384626;
	// "viewing" 1 0 0
	uvx[0]=cos(PI*camera.az/180.0)*cos(-PI*camera.el/180.0);
	uvx[1]=sin(PI*camera.az/180.0)*cos(-PI*camera.el/180.0);
	uvx[2]=-sin(-PI*camera.el/180.0);
	//"viewing" 0 1 0
	uvy[0]=cos(PI*camera.az/180.0)*sin(-PI*camera.el/180.0)*sin(-PI*camera.tw/180.0) - sin(PI*camera.az/180.0)*cos(-PI*camera.tw/180.0);
	uvy[1]=sin(PI*camera.az/180.0)*sin(-PI*camera.el/180.0)*sin(-PI*camera.tw/180.0) + cos(PI*camera.az/180.0)*cos(-PI*camera.tw/180.0);
	uvy[2]=cos(-PI*camera.el/180.0)*sin(-PI*camera.tw/180.0);
	//"viewing" 0 0 1
	uvz[0]=cos(PI*camera.az/180.0)*sin(-PI*camera.el/180.0)*cos(-PI*camera.tw/180.0) + sin(PI*camera.az/180.0)*sin(-PI*camera.tw/180.0);
	uvz[1]=sin(PI*camera.az/180.0)*sin(-PI*camera.el/180.0)*cos(-PI*camera.tw/180.0) - cos(PI*camera.az/180.0)*sin(-PI*camera.tw/180.0);
	uvz[2]=cos(-PI*camera.el/180.0)*cos(-PI*camera.tw/180.0);
	int Ok = 1;

	rotx2 = 0;
	roty2 = 0;
	rotz2 = 0;
	tx2 = 0;
	ty2 = 0;
	tz2 = 0;
	bool_change_pos_obj = 0;
	// tz correction in case we rotate the camera around some place away from 0 0 0
	if (g_mode_cam_centre_of_mass == 1)
	{
		camera.atx = g_mean_all[0];
		camera.aty = g_mean_all[1];
		camera.atz = g_mean_all[2];
		camera.tx = -g_mean_all[0];
		camera.ty = -g_mean_all[1];
		//camera.tz -= g_mean_all[2];
	}

	
}
void MeshTools::rollinit_camera()
{
	double newaz, newel, newtw;

	glPushMatrix();
	glLoadIdentity();
    glRotated(rotx, 1.0, 0.0, 0.0);
	glRotated(roty, 0.0, 1.0, 0.0);	
	glRotated(camera.tw, 1.0, 0.0, 0.0);
	glRotated(camera.el, 0.0, 1.0, 0.0);
	glRotated(-camera.az, 0.0, 0.0, 1.0); 
	glRotated(rotz, 0.0, 0.0, 1.0); 
	float m[16];
	double PI=3.1415926535897932384626;

	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glPopMatrix();
	
	double param,param1;

	//EL
	param = (double)m[8];
	newel = asin (param) * 180 / PI;


	//TW
	param = (double)m[9];
	param1 = (double)m[10];
	newtw = - atan2 (param, param1) * 180 / PI;

	//AZ
	param = (double)m[4];
	param1 = (double)m[0];
	newaz = atan2 (param, param1) * 180 / PI;

	//Reinitialisation des parametres
	camera.tx = camera.tx + tx;
	camera.ty = camera.ty + ty;
	camera.tz = camera.tz + tz;
	camera.az = newaz;
	camera.tw = newtw;
	camera.el = newel;
	rotx = 0;
	roty = 0;
	rotz = 0;
	tx = ty = tz = 0;
	zoom = zoom *(1-g_zoom/100);
	g_zoom=0;
	
}

void MeshTools::Mesh_Select(int x1, int x2, int y1, int y2, int select_mode)
{

	
// Debut
	
	float xmin,xmax,ymin,ymax;
	

	xmin = (float)2*x1/w() -1;
	xmax = (float)2*x2/w() -1;
	ymin = -(-2*(float)y1/h() +1);
	ymax = -(-2*(float)y2/h() +1);
	//std::cout<<"Start Mesh Select"<<std::endl;
	Cont_Mesh.Mesh_Select(xmin, xmax, ymin, ymax, select_mode);
	//std::cout<<"Compute global mean"<<std::endl;
	this->Compute_Global_Mean();
	//std::cout<<"Compute global scalar list"<<std::endl;
	this->Compute_Global_Scalar_List();
	//std::cout<<"Mesh Select done"<<std::endl;
	this->redraw();

}
void MeshTools::Mesh_CreateLandmarkAtMouse(int x, int y)
{
	float mousex, mousey;
	mousex = (float)2*x/w() -1;
	mousey = -2*(float)y/h() +1;
	Cont_Mesh.Mesh_CreateLandmarkAtMouse(mousex,mousey, this->landmark_mode);

}
void MeshTools::Convert_RGB_To_Tags(int exact)
{
	Cont_Mesh.Convert_RGB_To_Tags(exact);
}
void MeshTools:: Mesh_Tag_Connected_Regions()
{
	Cont_Mesh.Mesh_Tag_Connected_Regions();
}
void  MeshTools::Merge_Tags(int source, int target)
{
	this->Set_Active_Scalar(-1);
	this->SetColorScaleId(-1); // calls update RGB
	if ((source>=0)&&(source<25)&&(target>=0)&&(target<25))
	{
		if (source !=target)
		{
			Cont_Mesh.Merge_Tags(source,target);
			Cont_Mesh.Update_RGB();
		}
	}
}

void MeshTools::Mesh_Decompose_Tag(int min_region_size, Fl_Double_Window* myWindow)
{
	
	//this->Set_Active_Scalar(5);
	
	if (min_region_size<0)
	{
		min_region_size=0;
	}
	Cont_Mesh.Mesh_Decompose_Tag(min_region_size, myWindow);

}
void MeshTools::Mesh_Delete_Tag (int tag_value)
{
	if (tag_value<0) 
	{
		tag_value =0;
	}
	double tag = (double) tag_value;
	this->Set_Active_Scalar(-1);
	this->SetColorScaleId(-1);

	Cont_Mesh.Mesh_Delete_Scalar ( tag);
	
}
void MeshTools::Mesh_Delete_Active_Tag ()
{
	
	
	this->Set_Active_Scalar(-1);
	this->SetColorScaleId(-1);

	Cont_Mesh.Mesh_Delete_Scalar((double)g_active_tag);
	
}
void  MeshTools::Set_Tag_Extraction_Criterion(int all)
{
	g_tag_extraction_criterion_all = all;

}

void MeshTools::Mesh_Extract_Active_Tag()
{
	this->Set_Active_Scalar(-1);
	this->SetColorScaleId(-1);
	Cont_Mesh.Mesh_Extract_Scalar_Range((double)g_active_tag, (double)g_active_tag);
}
void MeshTools::Mesh_Extract_Tag (int tag_value)
{
	if (tag_value<0) 
	{
		tag_value =0;
	}
	double tag = (double) tag_value;
	this->Set_Active_Scalar(-1);
	this->SetColorScaleId(-1);
	Cont_Mesh.Mesh_Extract_Scalar_Range(tag_value, tag_value);
}
void MeshTools::Mesh_Delete_All_Non_Zero_Tags()
{	
	this->Set_Active_Scalar(-1);
	this->SetColorScaleId(-1);
	Cont_Mesh.Mesh_Extract_Scalar_Range(0.0, 0.0);
}

void MeshTools::Mesh_Extract_Scalar_Range(float scalar_min, float scalar_max)
{

	if (scalar_max >= scalar_min)
	{
		Cont_Mesh.Mesh_Extract_Scalar_Range((double)scalar_min, (double)scalar_max);
	}

}
	

void MeshTools::Mesh_Tag(int x, int y, int mode)
{
	this->Set_Active_Scalar(-1);
	this->SetColorScaleId(-1);
	float mousex, mousey;
	mousex = (float)2*x/w() -1;
	mousey = -2*(float)y/h() +1;
	Cont_Mesh.Mesh_Tag(mousex,mousey, mode);

}

void MeshTools::Mesh_Tag_Within_Lasso(int x, int y, int lasso_x, int lasso_y, int mode)
{
	this->Set_Active_Scalar(-1);
	this->SetColorScaleId(-1);
	float mousex, mousey;
	mousex = (float)2*x/w() -1;
	mousey = -2*(float)y/h() +1;
	Cont_Mesh.Mesh_Tag_Within_Lasso(mousex,mousey,lasso_x,lasso_y, h(), w(), mode);

}
void MeshTools::Fill_Holes (int tag_id, int max_size,  Fl_Double_Window* fillholesWindow2)
{
	Cont_Mesh.Fill_Holes(tag_id, max_size, fillholesWindow2);
}
void MeshTools::Mesh_Select(int x, int y)
{
	float mousex, mousey;
	mousex = (float)2*x/w() -1;
	mousey = -2*(float)y/h() +1;
	Cont_Mesh.Mesh_Select(mousex,mousey);
	this->redraw();

}

void MeshTools::Mesh_MoveLandmarkAtMouse(int x, int y)
{
	float mousex, mousey;
	mousex = (float)2*x/w() -1;
	mousey = -2*(float)y/h() +1;
	Cont_Mesh.Mesh_MoveLandmarkAtMouse(Cont_Mesh.Get_Selected_Landmark(this->landmark_mode), mousex,mousey, this->landmark_mode);
	this->redraw();

}
void MeshTools::SC_Show_Below_Min(int value)
{
	if (value==0 || value ==1)
	{
		g_sc_show_below_min=value;
	}
}

void MeshTools::SC_Show_Above_Max(int value)
{
	if (value==0 || value ==1)
	{
		g_sc_show_above_max=value;
	}
}
void MeshTools::SC_calc_depth()
{
	Cont_Mesh.SC_calc_depth();
	this->Compute_Global_Scalar_List();
}


void MeshTools::SC_calc_thickness_between_objects(float thickness_max_distance, 
									   int source, 
									   int target,
									   Fl_Double_Window* thicknessWindow2)
{
	//std::cout<<"Source="<<source<<std::endl;
	//std::cout<<"Target="<<target<<std::endl;

	if (source <0 || target <0)
	{
		fl_alert("You must select objects to be able to compute thickness.");
	}
	else
	{
		OBJECT_MESH *My_Source = Cont_Mesh.Get_Selected_Mesh(source);
		OBJECT_MESH *My_Target = Cont_Mesh.Get_Selected_Mesh(target);
		if (My_Source ==NULL || My_Target==NULL)
		{fl_alert("At least one of the selected meshes could not be retrieved!");}
		else
		{
			Cont_Mesh.SC_calc_thickness_between_objects(thickness_max_distance, My_Source, My_Target, thicknessWindow2);
			this->Compute_Global_Scalar_List();
		}
	}
}

void MeshTools::SC_calc_thickness(float thickness_max_distance, Fl_Double_Window* thicknessWindow)
{
	Cont_Mesh.SC_calc_thickness(thickness_max_distance,thicknessWindow);
	this->Compute_Global_Scalar_List();
}

void MeshTools::SC_calc_curvature(int mode)
{
	Cont_Mesh.SC_calc_curv(mode);
	this->Compute_Global_Scalar_List();
}
void MeshTools::Mesh_TPS( int basis, float percentage)
{
	Cont_Mesh.Mesh_TPS(basis,percentage);
}
void MeshTools::Mesh_Decimate(int percentage, int method)
{
	Cont_Mesh.Mesh_Decimate(percentage, method);
}
void MeshTools::Mesh_Densify(int subdivision_nr)
{
Cont_Mesh.Mesh_Densify(subdivision_nr);
}
void MeshTools::Mesh_Fill_Holes(int holes_size)
{
	Cont_Mesh.Mesh_Fill_Holes(holes_size);
}
void MeshTools::SC_calc_mean()
{
	if (g_active_scalar!=-1)// No mean for tags!
	{
		Cont_Mesh.SC_calc_mean();
	}
}


double MeshTools::cam_getnear()
{return camera.near1;}
double MeshTools::cam_getfar()
{return camera.far1;}
double MeshTools::cam_gettx()
{return camera.tx;}
double MeshTools::cam_getty()
{return camera.ty;}
double MeshTools::cam_gettz()
{return camera.tz;}
double MeshTools::cam_getel()
{return camera.el;}
double MeshTools::cam_gettw()
{return camera.tw;}
double MeshTools::cam_getaz()
{return camera.az;}
void MeshTools::Set_sc_min (float value)
{
	if (g_color_scale_id!=-1)
	{Min_Color =value;
	}
	
	else
	{
		Min_Color = 0;
	}
}
void MeshTools::Set_sc_max (float value)
{
	if (g_color_scale_id!=-1)
	{
		Max_Color =value;
	}
	else
	{
		Max_Color = 24;
	}

}
float MeshTools::Get_sc_min ()
{
	if (g_color_scale_id!=-1)
	{
	
		return Min_Color;
	}
	else
	{
	
		return 0;
	}
	

}
float MeshTools::Get_sc_max ()
{
	if (g_color_scale_id!=-1)
	{
	
		return Max_Color;
	}
	else
	{
		return 24;
	}
	
}

void MeshTools::cam_validate(double near1, double far1, double az, double el, double tw, double tx, double ty, double tz)
{
	
	camera.tx =tx;
	camera.ty =ty;
	camera.tz =tz;
	camera.el =el;
	camera.tw =tw;
	camera.az =az;
	camera.near1 =near1;
	camera.far1 =far1;
}




void MeshTools::Mesh_Smooth (int iterationnumber, double relaxation_factor)
{
	Cont_Mesh.smooth (iterationnumber, relaxation_factor);
}



void MeshTools::SetAmbiant (float a)

{
	ambiant = a;
	this->valid(0);
}

void MeshTools::SetDiffuse (float d)

{
	diffuse = d;
	this->valid(0);
}
void MeshTools:: SetSpecular (float s)

{
	specular = s;
	this->valid(0);
}

void MeshTools::lightroll1(float pos)
{

lightpos1=pos;
SetLightPosition2();

}

void MeshTools::lightroll2(float pos)
{

lightpos2=pos;
SetLightPosition2();

}


void MeshTools::SetLightPosition2()
{
	
	//1 0 0
	cout << "lpoos1  " << lightpos1 << " poos2-  " << lightpos2;
	light_position[0]=110*cos(PI*lightpos1/180.0)*cos(PI*lightpos2/180.0);
	light_position[1]=110*sin(PI*lightpos1/180.0);
	light_position[2]=110*sin(PI*lightpos2/180.0);
	this->valid(0);
	

	

//this->valid(0);

}

void MeshTools::SetLightPosition(int pos)
{
	switch ( pos)
{
case POS_FACE : 
	light_position[0] = 0;
	light_position[1] = 0;
	light_position[2] = 110;
	cout << "pos face" << endl;
	break;
case POS_BACK : 
	light_position[0] = 0;
	light_position[1] = 110;
	light_position[2] = -110;
	break;
case POS_BENEATH : 
	light_position[0] = 0;
	light_position[1] = -110;
	light_position[2] = 110;
   break;
case POS_ABOVE : 
	light_position[0] = 0;
	light_position[1] = 110;
	light_position[2] = 110;
	break;
case POS_LEFT : 
   	light_position[0] = 110;
	light_position[1] = 0;
	light_position[2] = 110;
   break;
case POS_RIGHT : 
   light_position[0] = -110;
	light_position[1] = 0;
	light_position[2] = 110;
   break;
default : 
	light_position[0] = 0;
	light_position[1] = 0;
	light_position[2] = 110;
break;
}

	
	this->valid(0);
}
void MeshTools::Change_Cull_Face()
{
	if (this->disp_cull_face ==1)
	{
		this->disp_cull_face = 0;
	}
	else
	{
		this->disp_cull_face = 1;
	}
	//this->valid(0);
	bool_change_mode = 1;
}
void MeshTools::SetModeTool(int mode)
{g_mode_tool = mode;}

void MeshTools::SetMode(int mode)
{this->mode = mode;}

void MeshTools::ChangeLandmarkMode()
{
	if (modelandmark==0){modelandmark=1;}
	else{modelandmark=0;} 
}
void MeshTools::SetDisplayMode(int mode)
{
	dispmode = mode;
	bool_change_mode = 1;
	if (dispmode == 4 || dispmode == 5)
		cpt_dispmode4and5 = 1;
	else
		cpt_dispmode4and5 = 0;
}
void MeshTools::Mesh_Set_Select_Mode(int Mode)
{ this->select_mode = Mode;}

void MeshTools::SetPosition(int pos)
{
	this->position = pos;
	switch ( this->position)
	{
	case POS_FACE : 
		camera.el = 0;camera.az = 0; camera.tw = 0;break;				

	case POS_BACK : 
		camera.el = 0;camera.az = 180; camera.tw = 0;break;		
	case POS_BENEATH : 
		camera.el = -90;camera.az = 0; camera.tw = 0;
		break;
	case POS_ABOVE : 		
		camera.el = 90;camera.az = 0; camera.tw = 0;
		break;
	case POS_LEFT : 
		camera.el = 0;camera.az = 90; camera.tw = 0; break;		
	case POS_RIGHT :  
		camera.el = 0;camera.az = -90; camera.tw = 0;break;
	default : 
			camera.el = 0;camera.az = 90; camera.tw = 0; break;;
}



}
float MeshTools::GetZoom()
{
	return this->zoom;

}
void MeshTools::SetZoom(float zoom)
{
	/*if (zoom<0)
	{zoom = -zoom;}*/
//std::cout<<"zoom="<<zoom<<std::endl;
	g_zoom = zoom;
	//std::cout<<"g_fov_adapt="<<g_fov_adapt<<std::endl;
	if (g_fov_adapt==1)
	{
		float opt_fov_depth = this->Get_Optimal_FOV_Depth();
		camera.far1 = opt_fov_depth;
		camera.tz = -opt_fov_depth/2;
		//std::cout<<"opt_fov_depth="<<opt_fov_depth<<std::endl;
	}

}


void  MeshTools::Mesh_SetOpeningBehaviour (int move_cm)
{
	g_move_cm= move_cm;
}

void MeshTools::Set_fov_adapt(int fov_adapt)
{
	g_fov_adapt = fov_adapt;
}
int MeshTools::Get_fov_adapt()
{
	return g_fov_adapt;
}
void  MeshTools::Mesh_SetZoomBehaviour (int auto_zoom)
{
	g_auto_zoom = auto_zoom;
}
void  MeshTools::SetColorScaleId (int scale_id)
{
	if (scale_id != g_color_scale_id)
	{
		g_color_scale_id = scale_id;
		
		
	}
	Cont_Mesh.Update_RGB();
}
int MeshTools::GetColorScaleId()
{
	return g_color_scale_id;
}
void  MeshTools::Mesh_SetAutoDelete (int auto_delete)
{
	g_auto_delete = auto_delete;
}

int  MeshTools::Mesh_GetAutoDelete ()
{
	return g_auto_delete;
}
int  MeshTools::Mesh_GetOpeningBehaviour ()
{
	return g_move_cm;
}

void  MeshTools::Mesh_Set2SidesLightning (int sides)
{
	g_enable_2sides = sides;
}

int  MeshTools::Mesh_Get2SidesLightning ()
{
	return g_enable_2sides;
}

int  MeshTools::Mesh_GetZoomBehaviour ()
{
	return g_auto_zoom;
}

void MeshTools::Mesh_SetLandmarkType(int landmark_type2)
{
	g_landmark_type = landmark_type2;
	Cont_Mesh.Mesh_container_setlandmarksize(g_landmark_size);
}
int MeshTools::Mesh_GetLandmarkType()
{return g_landmark_type;}

float MeshTools::Mesh_GetLandmarkSize()
{return g_landmark_size;}

void MeshTools::Mesh_SetLandmarkSize(float landmark_size2)
{
	g_landmark_size = landmark_size2;
	Cont_Mesh.Mesh_container_setlandmarksize(g_landmark_size);
}
void MeshTools::ShowGrid()
{
	if (this->showgrid ==0)
	{showgrid =1;}
	else
	{ showgrid =0;}
}
void MeshTools::ShowOrientation()
{
	if (this->showorientation ==0)
	{showorientation =1;}
	else
	{ showorientation =0;}
}
void MeshTools::save_ini_param()
{
	CDataFile ExistingDF("MeshTools.ini", 1);
	
	ExistingDF.SetFloat("lightpos1", lightpos1,"","lightposition");
	ExistingDF.SetFloat("lightpos2", lightpos2,"","lightposition");
	ExistingDF.SetFloat("ambiant", ambiant,"","lightposition");
	ExistingDF.SetFloat("diffuse", diffuse,"","lightposition");
	ExistingDF.SetFloat("specular", specular,"","lightposition");



		ExistingDF.SetFloat("r", color_arrow [0],"","color_arrow");
	ExistingDF.SetFloat("g", color_arrow [1],"","color_arrow");
	ExistingDF.SetFloat("b", color_arrow [2],"","color_arrow");
	
	ExistingDF.SetFloat("r", color_grid [0],"","color_grid");
	ExistingDF.SetFloat("g", color_grid [1],"","color_grid");
	ExistingDF.SetFloat("b", color_grid [2],"","color_grid");

	ExistingDF.SetFloat("r", color_back [0],"","color_back");
	ExistingDF.SetFloat("g", color_back [1],"","color_back");
	ExistingDF.SetFloat("b", color_back [2],"","color_back");
	
	ExistingDF.SetFloat("r", color_obj [0],"","color_obj");
	ExistingDF.SetFloat("g", color_obj [1],"","color_obj");
	ExistingDF.SetFloat("b", color_obj [2],"","color_obj");

	ExistingDF.SetFloat("r", g_flag_color [0],"","g_flag_color");
	ExistingDF.SetFloat("g", g_flag_color [1],"","g_flag_color");
	ExistingDF.SetFloat("b", g_flag_color [2],"","g_flag_color");
	ExistingDF.SetFloat("length", g_flag_length,"","g_flag_length");
	
	ExistingDF.SetFloat("r", color_max [0],"","color_surf_max");
	ExistingDF.SetFloat("g", color_max [1],"","color_surf_max");
	ExistingDF.SetFloat("b", color_max [2],"","color_surf_max");
	
	ExistingDF.SetFloat("r", color_min [0],"","color_surf_min");
	ExistingDF.SetFloat("g", color_min [1],"","color_surf_min");
	ExistingDF.SetFloat("b", color_min [2],"","color_surf_min");

	ExistingDF.SetFloat("r", color_max2 [0],"","color_distnorm_max");
	ExistingDF.SetFloat("g", color_max2 [1],"","color_distnorm_max");
	ExistingDF.SetFloat("b", color_max2 [2],"","color_distnorm_max");

	ExistingDF.SetFloat("r", color_min2 [0],"","color_distnorm_min");
	ExistingDF.SetFloat("g", color_min2 [1],"","color_distnorm_min");
	ExistingDF.SetFloat("b", color_min2 [2],"","color_distnorm_min");

	ExistingDF.SetFloat("color_surf_range_min", color_surf_range_min,"","color_surf_range");
	ExistingDF.SetFloat("color_surf_range_max", color_surf_range_max,"","color_surf_range");
	
	ExistingDF.SetFloat("color_distnorm_range_min", color_distnorm_range_min,"","color_distnorm_range");
	ExistingDF.SetFloat("color_distnorm_range_max", color_distnorm_range_max,"","color_distnorm_range");
	ExistingDF.SetFloat("color_distnorm_range_min", color_distnorm_range_min,"","color_distnorm_range");
	
	ExistingDF.SetFloat("lightpos1", lightpos1,"","lightposition");


	ExistingDF.SetInt("id", g_active_tag,"","active_tag");

	ExistingDF.SetInt("all", g_display_all,"","display");
	
	ExistingDF.SetFloat("size", g_landmark_size,"","landmarks");
	ExistingDF.SetInt("type", g_landmark_type,"","landmarks");
	ExistingDF.SetInt("move_at_cm", g_move_cm,"","surfaces");
	ExistingDF.SetInt("move_at_cm", g_mode_cam_centre_of_mass, "", "camera");
	ExistingDF.SetInt("adapt", g_fov_adapt ,"","fov");
	
	
	ExistingDF.SetInt("auto_zoom", g_auto_zoom,"","surfaces");
	ExistingDF.SetInt("auto_delete", g_auto_delete,"","structure");
	ExistingDF.SetInt("2_sides", g_enable_2sides,"","lightning");

	ExistingDF.SetValue("z_plus", g_orientation_labels[0],"","orientation");
	ExistingDF.SetValue("z_minus", g_orientation_labels[1],"","orientation");

	ExistingDF.SetValue("y_plus", g_orientation_labels[2],"","orientation");
	ExistingDF.SetValue("y_minus", g_orientation_labels[3],"","orientation");

	ExistingDF.SetValue("x_plus", g_orientation_labels[4],"","orientation");
	ExistingDF.SetValue("x_minus", g_orientation_labels[5],"","orientation");


	ExistingDF.SetFloat("r", g_tag_colors[0][0],"","tag_00");
	ExistingDF.SetFloat("g", g_tag_colors[0][1],"","tag_00");
	ExistingDF.SetFloat("b", g_tag_colors[0][2],"","tag_00");
	ExistingDF.SetFloat("a", g_tag_colors[0][3],"","tag_00");
	ExistingDF.SetValue("label", g_tag_labels[0],"","tag_00");

	ExistingDF.SetFloat("r", g_tag_colors[1][0],"","tag_01");
	ExistingDF.SetFloat("g", g_tag_colors[1][1],"","tag_01");
	ExistingDF.SetFloat("b", g_tag_colors[1][2],"","tag_01");
	ExistingDF.SetFloat("a", g_tag_colors[1][3],"","tag_01");
	ExistingDF.SetValue("label", g_tag_labels[1],"","tag_01");

	ExistingDF.SetFloat("r", g_tag_colors[2][0],"","tag_02");
	ExistingDF.SetFloat("g", g_tag_colors[2][1],"","tag_02");
	ExistingDF.SetFloat("b", g_tag_colors[2][2],"","tag_02");
	ExistingDF.SetFloat("a", g_tag_colors[2][3],"","tag_02");
	ExistingDF.SetValue("label", g_tag_labels[2],"","tag_02");

	ExistingDF.SetFloat("r", g_tag_colors[3][0],"","tag_03");
	ExistingDF.SetFloat("g", g_tag_colors[3][1],"","tag_03");
	ExistingDF.SetFloat("b", g_tag_colors[3][2],"","tag_03");
	ExistingDF.SetFloat("a", g_tag_colors[3][3],"","tag_03");
	ExistingDF.SetValue("label", g_tag_labels[3],"","tag_03");

	ExistingDF.SetFloat("r", g_tag_colors[4][0],"","tag_04");
	ExistingDF.SetFloat("g", g_tag_colors[4][1],"","tag_04");
	ExistingDF.SetFloat("b", g_tag_colors[4][2],"","tag_04");
	ExistingDF.SetFloat("a", g_tag_colors[4][3],"","tag_04");
	ExistingDF.SetValue("label", g_tag_labels[4],"","tag_04");

	ExistingDF.SetFloat("r", g_tag_colors[5][0],"","tag_05");
	ExistingDF.SetFloat("g", g_tag_colors[5][1],"","tag_05");
	ExistingDF.SetFloat("b", g_tag_colors[5][2],"","tag_05");
	ExistingDF.SetFloat("a", g_tag_colors[5][3],"","tag_05");
	ExistingDF.SetValue("label", g_tag_labels[5],"","tag_05");

	ExistingDF.SetFloat("r", g_tag_colors[6][0],"","tag_06");
	ExistingDF.SetFloat("g", g_tag_colors[6][1],"","tag_06");
	ExistingDF.SetFloat("b", g_tag_colors[6][2],"","tag_06");
	ExistingDF.SetFloat("a", g_tag_colors[6][3],"","tag_06");
	ExistingDF.SetValue("label", g_tag_labels[6],"","tag_06");

	ExistingDF.SetFloat("r", g_tag_colors[7][0],"","tag_07");
	ExistingDF.SetFloat("g", g_tag_colors[7][1],"","tag_07");
	ExistingDF.SetFloat("b", g_tag_colors[7][2],"","tag_07");
	ExistingDF.SetFloat("a", g_tag_colors[7][3],"","tag_07");
	ExistingDF.SetValue("label", g_tag_labels[7],"","tag_07");

	ExistingDF.SetFloat("r", g_tag_colors[8][0],"","tag_08");
	ExistingDF.SetFloat("g", g_tag_colors[8][1],"","tag_08");
	ExistingDF.SetFloat("b", g_tag_colors[8][2],"","tag_08");
	ExistingDF.SetFloat("a", g_tag_colors[8][3],"","tag_08");
	ExistingDF.SetValue("label", g_tag_labels[8],"","tag_08");

	ExistingDF.SetFloat("r", g_tag_colors[9][0],"","tag_09");
	ExistingDF.SetFloat("g", g_tag_colors[9][1],"","tag_09");
	ExistingDF.SetFloat("b", g_tag_colors[9][2],"","tag_09");
	ExistingDF.SetFloat("a", g_tag_colors[9][3],"","tag_09");
	ExistingDF.SetValue("label", g_tag_labels[9],"","tag_09");

	ExistingDF.SetFloat("r", g_tag_colors[10][0],"","tag_10");
	ExistingDF.SetFloat("g", g_tag_colors[10][1],"","tag_10");
	ExistingDF.SetFloat("b", g_tag_colors[10][2],"","tag_10");
	ExistingDF.SetFloat("a", g_tag_colors[10][3],"","tag_10");
	ExistingDF.SetValue("label", g_tag_labels[10],"","tag_10");

	ExistingDF.SetFloat("r", g_tag_colors[11][0],"","tag_11");
	ExistingDF.SetFloat("g", g_tag_colors[11][1],"","tag_11");
	ExistingDF.SetFloat("b", g_tag_colors[11][2],"","tag_11");
	ExistingDF.SetFloat("a", g_tag_colors[11][3],"","tag_11");
	ExistingDF.SetValue("label", g_tag_labels[11],"","tag_11");

	ExistingDF.SetFloat("r", g_tag_colors[12][0],"","tag_12");
	ExistingDF.SetFloat("g", g_tag_colors[12][1],"","tag_12");
	ExistingDF.SetFloat("b", g_tag_colors[12][2],"","tag_12");
	ExistingDF.SetFloat("a", g_tag_colors[12][2],"","tag_12");
	ExistingDF.SetValue("label", g_tag_labels[12],"","tag_12");

	ExistingDF.SetFloat("r", g_tag_colors[13][0],"","tag_13");
	ExistingDF.SetFloat("g", g_tag_colors[13][1],"","tag_13");
	ExistingDF.SetFloat("b", g_tag_colors[13][2],"","tag_13");
	ExistingDF.SetFloat("a", g_tag_colors[13][3],"","tag_13");
	ExistingDF.SetValue("label", g_tag_labels[13],"","tag_13");

	ExistingDF.SetFloat("r", g_tag_colors[14][0],"","tag_14");
	ExistingDF.SetFloat("g", g_tag_colors[14][1],"","tag_14");
	ExistingDF.SetFloat("b", g_tag_colors[14][2],"","tag_14");
	ExistingDF.SetFloat("a", g_tag_colors[14][3],"","tag_14");
	ExistingDF.SetValue("label", g_tag_labels[14],"","tag_14");

	ExistingDF.SetFloat("r", g_tag_colors[15][0],"","tag_15");
	ExistingDF.SetFloat("g", g_tag_colors[15][1],"","tag_15");
	ExistingDF.SetFloat("b", g_tag_colors[15][2],"","tag_15");
	ExistingDF.SetFloat("a", g_tag_colors[15][3],"","tag_15");
	ExistingDF.SetValue("label", g_tag_labels[15],"","tag_15");

	ExistingDF.SetFloat("r", g_tag_colors[16][0],"","tag_16");
	ExistingDF.SetFloat("g", g_tag_colors[16][1],"","tag_16");
	ExistingDF.SetFloat("b", g_tag_colors[16][2],"","tag_16");
	ExistingDF.SetFloat("a", g_tag_colors[16][3],"","tag_16");
	ExistingDF.SetValue("label", g_tag_labels[16],"","tag_16");

	ExistingDF.SetFloat("r", g_tag_colors[17][0],"","tag_17");
	ExistingDF.SetFloat("g", g_tag_colors[17][1],"","tag_17");
	ExistingDF.SetFloat("b", g_tag_colors[17][2],"","tag_17");
	ExistingDF.SetFloat("a", g_tag_colors[17][3],"","tag_17");
	ExistingDF.SetValue("label", g_tag_labels[17],"","tag_17");

	ExistingDF.SetFloat("r", g_tag_colors[18][0],"","tag_18");
	ExistingDF.SetFloat("g", g_tag_colors[18][1],"","tag_18");
	ExistingDF.SetFloat("b", g_tag_colors[18][2],"","tag_18");
	ExistingDF.SetFloat("a", g_tag_colors[18][3],"","tag_18");
	ExistingDF.SetValue("label", g_tag_labels[18],"","tag_18");

	ExistingDF.SetFloat("r", g_tag_colors[19][0],"","tag_19");
	ExistingDF.SetFloat("g", g_tag_colors[19][1],"","tag_19");
	ExistingDF.SetFloat("b", g_tag_colors[19][2],"","tag_19");
	ExistingDF.SetFloat("a", g_tag_colors[19][3],"","tag_19");
	ExistingDF.SetValue("label", g_tag_labels[19],"","tag_19");

	ExistingDF.SetFloat("r", g_tag_colors[20][0],"","tag_20");
	ExistingDF.SetFloat("g", g_tag_colors[20][1],"","tag_20");
	ExistingDF.SetFloat("b", g_tag_colors[20][2],"","tag_20");
	ExistingDF.SetFloat("a", g_tag_colors[20][3],"","tag_20");
	ExistingDF.SetValue("label", g_tag_labels[20],"","tag_20");

	ExistingDF.SetFloat("r", g_tag_colors[21][0],"","tag_21");
	ExistingDF.SetFloat("g", g_tag_colors[21][1],"","tag_21");
	ExistingDF.SetFloat("b", g_tag_colors[21][2],"","tag_21");
	ExistingDF.SetFloat("a", g_tag_colors[21][3],"","tag_21");
	ExistingDF.SetValue("label", g_tag_labels[21],"","tag_21");

	ExistingDF.SetFloat("r", g_tag_colors[22][0],"","tag_22");
	ExistingDF.SetFloat("g", g_tag_colors[22][1],"","tag_22");
	ExistingDF.SetFloat("b", g_tag_colors[22][2],"","tag_22");
	ExistingDF.SetFloat("a", g_tag_colors[22][3],"","tag_22");
	ExistingDF.SetValue("label", g_tag_labels[22],"","tag_22");

	ExistingDF.SetFloat("r", g_tag_colors[23][0],"","tag_23");
	ExistingDF.SetFloat("g", g_tag_colors[23][1],"","tag_23");
	ExistingDF.SetFloat("b", g_tag_colors[23][2],"","tag_23");
	ExistingDF.SetFloat("a", g_tag_colors[23][3],"","tag_23");
	ExistingDF.SetValue("label", g_tag_labels[23],"","tag_23");

	ExistingDF.SetFloat("r", g_tag_colors[24][0],"","tag_24");
	ExistingDF.SetFloat("g", g_tag_colors[24][1],"","tag_24");
	ExistingDF.SetFloat("b", g_tag_colors[24][2],"","tag_24");
	ExistingDF.SetFloat("a", g_tag_colors[24][3],"","tag_24");
	ExistingDF.SetValue("label", g_tag_labels[24],"","tag_24");

	

	//std::cout<<"ir"<<g_tag_colors[24][0]<<"ig"<<g_tag_colors[24][1]<<"ib"<<g_tag_colors[24][1]<<std::endl;
	
	ExistingDF.Save();
}




void MeshTools::Curve_start (int direction)
{
// selected landmarks are tagged as the starting points of curves (direction ==1)
// selected landmarks are untagged (direction ==0)
//  selected landmarks are tagged so as to  connect to the preceding starting point 1 or 2 (direction ==3)
	// in that case next landmark should be tagged so as to be tagged as 1
// selected landamrks are tagged as the starting points of the curves without gap

Cont_Mesh.Curve_start(direction);
}

void MeshTools::Change_Draw_Curve ()
{
	if (this->disp_curve ==1)
	{
		this->disp_curve = 0;
	}
	else
	{
		this->disp_curve = 1;
	}
	this->valid(0);

}

void MeshTools::batch(int argc, char **argv)
{
		/*	//Meshtools_ui->show(1, argv);
			
			
			this->Landmark_Mode(0);

			printf("\nOpen Target file");
			
			this->Open_STL_File(0,argv[8]);
			printf("\nOpen Target landmarks");			
			this->Open_VER_File(argv[9]);
			
			this->Landmark_Mode(1);
			printf("\nOpen Source file");
			
			this->Open_STL_File(1,argv[6]);
			printf("\nOpen Source landmarks");			
			this->Open_VER_File(argv[7]);
			this->Def_make_landmarks_follow();
			this->redraw();
			printf("\nTps deformation");			
			this->Def_tpsp(atoi(argv[1]), atoi(argv[2]));
			//this->Def_tpsp(1,1);
			printf("\nDiffusion...");
			this->Def_FitDefToTarget(atof(argv[3]), atoi(argv[5]), atoi(argv[4]));
			//			this->Def_FitDefToTarget(atoi(argv[3]), atoi(argv[5]), atoi(argv[4]));
			printf("\nSave stl");
			this->Save_STL_File(argv[10]);
			exit(0);*/

}

void MeshTools::Select_Landmark(int landmark_mode, int lmk_ind)
{
	int cpt;	
	OBJECT_LANDMARK *k = NULL;
	int ind = 0;		
	k = Cont_Mesh.landmarkafter (ind, landmark_mode);
	while ( k != NULL)
	{		
			ind = k->landmark_index;
			if (ind == lmk_ind){k->selected=1;}
			else {k->selected=0;}			
			k = Cont_Mesh.landmarkafter (ind, landmark_mode);
	}
}
void MeshTools::Select_Landmark_Range(int landmark_mode, int lmk_ind_start, int lmk_ind_end)
{

	int cpt;	
	OBJECT_LANDMARK *k = NULL;
	int ind = 0;		
	k = Cont_Mesh.landmarkafter (ind, landmark_mode);
	while ( k != NULL)
	{		
			ind = k->landmark_index;
			if (ind <= lmk_ind_end && ind >= lmk_ind_start){k->selected=1;}
			else {k->selected=0;}			
			k = Cont_Mesh.landmarkafter (ind, landmark_mode);
	}

}




int MeshTools::Save_Landmarks(int file_type,int landmark_mode, int only_selected)
{
	
	int ok=1;
	std::string filename;

	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);				
	fnfc.title("Save Landmarks");		
	
	if (file_type ==1)
	{
		fnfc.filter("VER\t*.{ver,VER}");		

		
	}
	else
	{
		fnfc.filter("LMK\t*.{lmk,LMK}");		
		
	}
	int landmark_number = Cont_Mesh.Get_Landmark_Number(landmark_mode);
	if (landmark_number ==0)
	{
			if (landmark_mode ==0)
			{
				fl_alert("There are currently no normal landmark to be saved");
			}
			else
			{	fl_alert("There are currently no target landmark to be saved");}
			ok =0;
	}

	if (ok==1)
	{
		switch ( fnfc.show() ) 
		{
				case -1: 
					{
						//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
						break;  // ERROR
					}
				case  1: {
					//std::cout<<"CANCEL\n";     
					break;  // CANCEL
						 }
				default: 
					{
							//std::cout<<"PICKED: "<<fnfc.filename()<<"\n";   // FILE CHOSEN										
						filename = this->correct_filename(fnfc.filename());
						if (file_type ==1)
						{
							std::string VERext = ".ver";
							std::string VERext2 = ".VER";
							std::size_t found = filename.find(VERext);
							std::size_t found2 = filename.find(VERext2);
							if (found == std::string::npos && found2 == std::string::npos)
							{
								filename.append(".ver");
							}
						}
						else
						{
							std::string LMKext = ".lmk";
							std::string LMKext2 = ".LMK";
							std::size_t found = filename.find(LMKext);
							std::size_t found2 = filename.find(LMKext2);
							if (found == std::string::npos && found2 == std::string::npos)
							{
								filename.append(".lmk");
							}
						}

						ifstream file(filename.c_str());
						if (file)
						{
							ok = fl_ask("Overwrite?");
							file.close();

						}
						if (ok==1)
						{
							
						    FILE *filein=fopen(filename.c_str(), "w");	
							int cpt;
							glMatrix wc_mat;
							float vv[3], vv1[3], vvn[3], vvn1[3];
							cpt=0;
							OBJECT_LANDMARK *k = NULL;
							int ind = 0;
							
							
							k = Cont_Mesh.landmarkafter (ind, landmark_mode);
							while ( k != NULL)
							{
							

									k->get_world_coordinates_matrix(wc_mat);
									
							
									vv[0] =0;
									vv[1] =0;
									vv[2] =0;
									vvn[0] =1.0;
									vvn[1] =0;
									vvn[2] =0;
									ApplyTransformation(vv,vv1,wc_mat);
									ApplyTransformation(vvn,vvn1,wc_mat);
									cpt++;
									ind = k->landmark_index;
									int print_it=1;
									if (only_selected ==1 && k->selected==0){print_it=0;}
									if (print_it ==1)
									{
										if (file_type ==1)
										{
											fprintf (filein, "%s%d: %f %f %f %f %f %f\n", "landmark", ind, vv1[0], vv1[1], vv1[2], vvn1[0], vvn1[1], vvn1[2]);
										}
										else
										{	fprintf (filein, "%s%d: %f %f %f\n", "landmark", ind, vv1[0], vv1[1], vv1[2]);
										}
									}

									k = Cont_Mesh.landmarkafter (ind, landmark_mode);
							}
							
							fclose(filein);	
						}//overwrite?
						break;

					}
			}
	}
	
return 1;
		
		

}


int MeshTools::Save_flags()
{
	
	int ok=1;
	int flag_number = Cont_Mesh.Get_Landmark_Number(2);
	
	if (flag_number==0)
	{
			
		fl_alert("Can't save flags: no flag currently found to be saved.");
			
			ok =0;
	}
	if (ok==1)
	{
		std::string filename;
		Fl_Native_File_Chooser fnfc;
		fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);				
		fnfc.title("Save flags");				
		fnfc.filter("FLG\t*.{flg,FLG}");		

		switch ( fnfc.show() ) 
		{
				case -1: 
				{
					//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
					break;  // ERROR
				}
				case  1: 
				{
					//std::cout<<"CANCEL\n";     
					break;  // CANCEL
				 }
				default: 
				{
							//std::cout<<"PICKED: "<<fnfc.filename()<<"\n";   // FILE CHOSEN										
						filename = this->correct_filename(fnfc.filename());
						std::string FLGext = ".flg";
						std::string FLGext2 = ".FLG";
						std::size_t found = filename.find(FLGext);
						std::size_t found2 = filename.find(FLGext2);
						if (found == std::string::npos && found2 == std::string::npos)
						{
							filename.append(".flg");
						}
					
						FILE *filein;									
						
						ifstream file(filename.c_str());
						if (file)
						{
							ok = fl_ask("Overwrite?");
							file.close();

						}
						if (ok==1)
						{
														
							filein = fopen(filename.c_str(),"w");					
								
							int cpt;
							glMatrix wc_mat;
							float vv[3], vv1[3], vvn[3], vvn1[3], norm[3];
							cpt=0;
							OBJECT_LANDMARK *k = NULL;
							int ind = 0;
							
							
							k = Cont_Mesh.landmarkafter (ind, 2);
							while ( k != NULL)
							{
							
									k->get_world_coordinates_matrix(wc_mat);								
									vv[0] =0;
									vv[1] =0;
									vv[2] =0;
									vvn[0] =1.0;
									vvn[1] =0;
									vvn[2] =0;
									ApplyTransformation(vv,vv1,wc_mat);
									ApplyTransformation(vvn,vvn1,wc_mat);
									cpt++;
									ind = k->landmark_index;
									//normalize landmark length;
									double dist = sqrt((vv1[0]-vvn1[0])*(vv1[0]-vvn1[0]) +
										(vv1[1]-vvn1[1])*(vv1[1]-vvn1[1]) +
										(vv1[2]-vvn1[2])*(vv1[2]-vvn1[2]) );
									if (dist>0)
									{
										norm[0] = vv1[0]+(-vv1[0] + vvn1[0]) / (float)dist;
										norm[1] = vv1[1]+(-vv1[1] + vvn1[1]) / (float)dist;
										norm[2] = vv1[2]+(-vv1[2] + vvn1[2]) / (float)dist;
									}
									else
									{
										norm[0]=0;
										norm[1]=0;
										norm[2]=1;
									}
									fprintf (filein, "%s\n", k->flag_label.c_str());		
									fprintf (filein, "%f %f %f %f %f %f %f %f %f %f\n", 
										//vv1[0], vv1[1], vv1[2], vvn1[0], vvn1[1], vvn1[2],
										vv1[0], vv1[1], vv1[2], norm[0], norm[1], norm[2],
										k->flag_length,
										k->color[0],
										k->color[1],
										k->color[2]										
										);						

									k = Cont_Mesh.landmarkafter (ind, 2);
							}			
							fclose(filein);	
						}//ok overwrite
						break;
			}//default
		}//switch
	}//ok
	
	return 1;
	
}

int MeshTools::Save_Orientation()
{
	
	std::string filename;
	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);				
	fnfc.title("Save orientation labels");				
	fnfc.filter("ORI\t*.{ori,ORI}");		

	switch ( fnfc.show() ) 
	{
			case -1: 
			{
				//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
				break;  // ERROR
			}
			case  1: 
			{
				//std::cout<<"CANCEL\n";     
				break;  // CANCEL
			 }
			default: 
			{
				
				filename = this->correct_filename(fnfc.filename());
				std::string ORIext = ".ori";
				std::string ORIext2 = ".ORI";
				std::size_t found = filename.find(ORIext);
				std::size_t found2 = filename.find(ORIext2);
				if (found == std::string::npos && found2 == std::string::npos)
				{
					filename.append(".ori");
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
							
					FILE*filein =fopen(filename.c_str(), "w");						
					for (int i =0; i<6; i++)
					{				
						fprintf (filein, "%s\n", g_orientation_labels[i].c_str());				
				
					}			
					fclose(filein);		
				}//overwrite?
				break;
			}
		
	}	
	return 1;			
}

int MeshTools::Save_Tags()
{
	
	std::string filename;
	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);				
	fnfc.title("Save tags");				
	fnfc.filter("TAG\t*.{tag,TAG}");		

	switch ( fnfc.show() ) 
	{
			case -1: 
			{
				//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
				break;  // ERROR
			}
			case  1: 
			{
				//std::cout<<"CANCEL\n";     
				break;  // CANCEL
			 }
			default: 
			{
				filename = this->correct_filename(fnfc.filename());
				std::string TAGext = ".tag";
				std::string TAGext2 = ".TAG";
				std::size_t found = filename.find(TAGext);
				std::size_t found2 = filename.find(TAGext2);
				if (found == std::string::npos && found2 == std::string::npos)
				{
					filename.append(".tag");
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
							
					FILE*filein =fopen(filename.c_str(), "w");						
					for (int i =0; i<25; i++)
					{				
						fprintf (filein, "%s\n", g_tag_labels[i].c_str());				
						fprintf (filein, "%f %f %f  %f\n", g_tag_colors[i][0],
							g_tag_colors[i][1], g_tag_colors[i][2], g_tag_colors[i][3]);
					}			
					fclose(filein);		
				}//overwrite?
				break;
			}
		
	}	
	return 1;			
}

int MeshTools::Move_Selected_Handles (float movementintensity)
{
 	
	int num_landmark_D = Cont_Mesh.Get_Landmark_Number (0); //landmarks
	int num_landmark_H = Cont_Mesh.Get_Landmark_Number (1);	//handles
	int start=0;
	int selected=0;
	if (num_landmark_D == num_landmark_H)
	{
		
			int cpt;
			glMatrix wc_mat;
			float vv[3], vv1[3], vvprec[3], vvnext[3], newcoords[3], vdir[3];
			float dist, length;

			cpt=0;
			OBJECT_LANDMARK *k = NULL;
			OBJECT_LANDMARK *l, *ll = NULL;
			int ind = 0;
			int ind2 = 0;
			vvprec[0]=0;
			vvprec[1]=0;
			vvprec[2]=0;

			// special case : landmark 0


			k = Cont_Mesh.landmarkafter (ind, 0); // k = normal landmark 0
			l = Cont_Mesh.landmarkafter (ind, 1); // l = target landmark 0
			
			while ( k != NULL)
			{
					ll=l; // ll =  target landmark n
					selected = l->selected; // current target landmark is selected?
				    newcoords[0]=l->Mat2[3][0];
					newcoords[1]=l->Mat2[3][1];
					newcoords[2]=l->Mat2[3][2];
					k->get_world_coordinates_matrix(wc_mat);
					vv[0] =0;
					vv[1] =0;
					vv[2] =0;
					ApplyTransformation(vv,vv1,wc_mat); // vv1 = position of normal landmark n
					start = k->Is_Curve_Start();
					if (ind==0){start=1;}
					if (start!=1){start=0;}
					cpt++;
					ind = k->landmark_index;
					ind2 = l->landmark_index;
				
				
					k = Cont_Mesh.landmarkafter (ind, 0); // now is normal landmark n+1
					l = Cont_Mesh.landmarkafter (ind2, 1);// now is target landmark n+1
					vvnext[0]=0;
					vvnext[1]=0;
					vvnext[2]=0;
					if (k!=NULL)
					{
						
						k->get_world_coordinates_matrix(wc_mat);
						vv[0] =0;
						vv[1] =0;
						vv[2] =0;
						ApplyTransformation(vv,vvnext,wc_mat); // vv netx = position of landmark n+1
						
					}
					if (start ==1)
					{
						// only need vv1[] and vvnext[]
						if (k ==NULL || (k!=NULL && k->Is_Curve_Start()))
						{
							//do nothing! means is a start and no next!
						}
						else
						{
							vdir[0]= vvnext[0]-vv1[0];
							vdir[1]= vvnext[1]-vv1[1];
							vdir[2]= vvnext[2]-vv1[2];
							length= sqrt (vdir[0]*vdir[0] + vdir[1]*vdir[1]+vdir[2]*vdir[2]);
							if (length !=0)
							{
								vdir[0]/=length;
								vdir[1]/=length;
								vdir[2]/=length;
							}
							dist = sqrt ((vvnext[0]-vv1[0])*(vvnext[0]-vv1[0]) 
								+ (vvnext[1]-vv1[1])*(vvnext[1]-vv1[1]) +
								(vvnext[2]-vv1[2])*(vvnext[2]-vv1[2]));
							newcoords[0] = vv1[0]+ movementintensity*dist*vdir[0]/100;
							newcoords[1] = vv1[1]+ movementintensity*dist*vdir[1]/100;
							newcoords[2] = vv1[2]+ movementintensity*dist*vdir[2]/100;
							
						}
					}
					else // current handle is not on a curve start
					{
						//if next is null or next is a curve start						
						if (k ==NULL || (k!=NULL && k->Is_Curve_Start()))
						{
							// then need vvprec and vv
							vdir[0]= vv1[0]-vvprec[0];
							vdir[1]= vv1[1]-vvprec[1];
							vdir[2]= vv1[2]-vvprec[2];
							length= sqrt (vdir[0]*vdir[0] + vdir[1]*vdir[1]+vdir[2]*vdir[2]);
							if (length !=0)
							{
								vdir[0]/=length;
								vdir[1]/=length;
								vdir[2]/=length;
							}
							dist = sqrt ((vv1[0]-vvprec[0])*(vv1[0]-vvprec[0]) 
								+ (vv1[1]-vvprec[1])*(vv1[1]-vvprec[1]) +
								(vv1[2]-vvprec[2])*(vv1[2]-vvprec[2]));
							newcoords[0] = vv1[0]+ movementintensity*dist*vdir[0]/100;
							newcoords[1] = vv1[1]+ movementintensity*dist*vdir[1]/100;
							newcoords[2] = vv1[2]+ movementintensity*dist*vdir[2]/100;
						}
						else
						{
							//
							vdir[0]= vvnext[0]-vvprec[0];
							vdir[1]= vvnext[1]-vvprec[1];
							vdir[2]= vvnext[2]-vvprec[2];
							length= sqrt (vdir[0]*vdir[0] + vdir[1]*vdir[1]+vdir[2]*vdir[2]);
							if (length !=0)
							{
								vdir[0]/=length;
								vdir[1]/=length;
								vdir[2]/=length;
							}
							/*dist = (sqrt ((vv1[0]-vvprec[0])*(vv1[0]-vvprec[0]) 
								+ (vv1[1]-vvprec[1])*(vv1[1]-vvprec[1]) +
								(vv1[2]-vvprec[2])*(vv1[2]-vvprec[2]))

								+sqrt ((vvnext[0]-vv1[0])*(vvnext[0]-vv1[0]) 
								+ (vvnext[1]-vv1[1])*(vvnext[1]-vv1[1]) +
								(vvnext[2]-vv1[2])*(vvnext[2]-vv1[2]))
								)
								;*/
							dist = sqrt ((vvnext[0]-vvprec[0])*(vvnext[0]-vvprec[0]) 
								+ (vvnext[1]-vvprec[1])*(vvnext[1]-vvprec[1]) +
								(vvnext[2]-vvprec[2])*(vvnext[2]-vvprec[2]))

								
								;
							newcoords[0] = vv1[0]+ movementintensity*dist*vdir[0]/100;
							newcoords[1] = vv1[1]+ movementintensity*dist*vdir[1]/100;
							newcoords[2] = vv1[2]+ movementintensity*dist*vdir[2]/100;

						}

					}
					vvprec[0] = vv1[0];
					vvprec[1] = vv1[1];
					vvprec[2] = vv1[2];


				if (selected==1)
				{
					// only in this case do we move the landmark
					//M2[3][0]=x;M2[3][1]=y;M2[3][2]=z;
					ll->Mat2[3][0] = newcoords[0];
					ll->Mat2[3][1] = newcoords[1];
					ll->Mat2[3][2] = newcoords[2];
				}
			}		
			return 1;
			
	}
	else {return 0;}
}


int MeshTools::Save_CUR_Infos()
{
	
	int ok=1;
	int landmark_normal_number = Cont_Mesh.Get_Landmark_Number(0);
	int landmark_target_number = Cont_Mesh.Get_Landmark_Number(1);
	
	if (landmark_normal_number <2 || landmark_target_number <2)
	{
			
		fl_alert("Can't save curve infos: there must be at least 2 normal and 2 target landmarks.");
			
			ok =0;
	}
	else if (landmark_normal_number != landmark_target_number)
	{
		fl_alert("Can't save curve infos: normal and target landmark numbers should be identical.");			
			ok =0;
	}
	if (ok==1)
	{
		std::string filename;
		Fl_Native_File_Chooser fnfc;
		fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);				
		fnfc.title("Save curve infos");				
		fnfc.filter("TXT\t*.{txt,TXT}");		

		switch ( fnfc.show() ) 
		{
				case -1: 
				{
					//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
					break;  // ERROR
				}
				case  1: 
				{
					//std::cout<<"CANCEL\n";     
					break;  // CANCEL
				 }
				default: 
				{
					filename = this->correct_filename(fnfc.filename());
					std::string TXText = ".txt";
					std::string TXText2 = ".TXT";
					std::size_t found = filename.find(TXText);
					std::size_t found2 = filename.find(TXText2);
					if (found == std::string::npos && found2 == std::string::npos)
					{
						filename.append(".txt");
					}
					ifstream file(filename.c_str());
					if (file)
					{
						ok = fl_ask("Overwrite?");
						file.close();

					}
					if (ok==1)
					{
						FILE	*filein;							
						filein =fopen(filename.c_str(), "w");	
						//fopen(&filein, filename.c_str(), "w");	
						int nseg = Cont_Mesh.Get_Curve_Segment_Number();
						for (int i=0; i<nseg;i++)
						{
							int j = i+1;
							float length = Cont_Mesh.Get_Curve_Segment_Length(j);
							fprintf (filein, "Curve %d : length= %f mm\n", j, length);	
						}																						
						fclose(filein);		
					}//overwrite?
					break;
				}//default			
		}//switch
	}//ok
	
	return 1;
}


int MeshTools::Save_CUR_LMK_File(int file_type, int decimation)
{	
	int ok=1;
	int landmark_normal_number = Cont_Mesh.Get_Landmark_Number(0);
	int landmark_target_number = Cont_Mesh.Get_Landmark_Number(1);	
	if (landmark_normal_number <2 || landmark_target_number <2)
	{
			
		fl_alert("Can't export curves: there must be at least 2 normal and 2 target landmarks.");
			
			ok =0;
	}
	else if (landmark_normal_number != landmark_target_number)
	{
		fl_alert("Can't export curves: normal and target landmark numbers should be identical.");			
			ok =0;
	}
	if (ok==1)
		{
		std::string filename;
		Fl_Native_File_Chooser fnfc;
		fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);				
		fnfc.title("Export curves");		
		
		if (file_type ==1)
		{
			fnfc.filter("VER\t*.{ver,VER}");		

			
		}
		else
		{
			fnfc.filter("LMK\t*.{lmk,LMK}");		
			
		}

		switch ( fnfc.show() ) 
		{
			case -1: 
			{
				//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
				break;  // ERROR
			}
			case  1: 
			{
				//std::cout<<"CANCEL\n";     
				break;  // CANCEL
			 }
			default: 
			{
				//std::cout<<"PICKED: "<<fnfc.filename()<<"\n";   // FILE CHOSEN					
				//Cont_Mesh.Mesh_STLwrite(szFile, Save_Obj);
				filename = this->correct_filename(fnfc.filename());

				//filename = fnfc.filename();		
				if (file_type ==1)
				{
					std::string VERext = ".ver";
					std::string VERext2 = ".VER";
					std::size_t found = filename.find(VERext);
					std::size_t found2 = filename.find(VERext2);
					if (found == std::string::npos && found2 == std::string::npos)
					{
						filename.append(".ver");
					}
				}
				else
				{
					std::string LMKext = ".lmk";
					std::string LMKext2 = ".LMK";
					std::size_t found = filename.find(LMKext);
					std::size_t found2 = filename.find(LMKext2);
					if (found == std::string::npos && found2 == std::string::npos)
					{
						filename.append(".lmk");
					}
				}
				ifstream file(filename.c_str());
				if (file)
				{
					ok = fl_ask("Overwrite?");
					file.close();

				}
				if (ok==1)
				{


					int num_landmark_D = Cont_Mesh.Get_Landmark_Number (0); //landmarks
					int num_landmark_H = Cont_Mesh.Get_Landmark_Number (1);	//handles
					int num_curves = Cont_Mesh.Get_Curve_Segment_Number();
					int start=0;
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
					float t, t2;


					glMatrix wc_mat;
					float vv[3], dd1[3],dd2[3], hh1[3], hh2[3], intvv[3],intvv2[3], slm[3];
					
					
					int new_segment =0;
					t2=0;
					float preceding_length=0;
					float current_length=0;
					int decimation_index=0;
					float length_to_reach=0;
					float total_length = Cont_Mesh.Get_Curve_Segment_Length(1);
					//float total_length2= Cont_Mesh.Get_Curve_Segment_Length(2);
					

					num_seg=0;
					if (num_landmark_D == num_landmark_H && decimation >1 && decimation <nint)
					{
					
						FILE	*filein;		
					
						//filein = fopen(this->VER_Sp_File, "w");	
						//fopen(&filein, filename.c_str(), "w");	
						//ifstream file(filename.c_str());
						filein=fopen(filename.c_str(), "w");	
					

						num_seg++;
					
						ob_H1 = Cont_Mesh.landmarkafter (0, 1);
						ob_Hstart = Cont_Mesh.landmarkafter (0, 1);
						ob_H2 = Cont_Mesh.landmarkafter (1, 1);
						ob_D1 = Cont_Mesh.landmarkafter (0, 0);
						ob_Dstart = Cont_Mesh.landmarkafter (0, 0);
						ob_D2 = Cont_Mesh.landmarkafter (1, 0);

						k= 0;	
						while ( ob_D1 != NULL )
						{
						
							if (ob_D1->Is_Curve_Start() ==2)
							{
								num_seg++;
								t2=0;
								length_to_reach=0;
								current_length=0;
								preceding_length=0;
								decimation_index=0;
								total_length = Cont_Mesh.Get_Curve_Segment_Length(num_seg);
								
							
							}
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

							// here, we have our 4 coordinates.
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
								 current_length += sqrt ((intvv[0]-intvv2[0])*(intvv[0]-intvv2[0])+
												 (intvv[1]-intvv2[1])*(intvv[1]-intvv2[1])+
												 (intvv[2]-intvv2[2])*(intvv[2]-intvv2[2])
												 );							 						 												

								if ((current_length >= length_to_reach)&&(length_to_reach >=preceding_length)&&(decimation_index<decimation))
								{
									
									slm[0]=0;slm[1]=0;slm[2]=0;
									decimation_index++;

									t=0.5;
									if ((current_length - preceding_length)>0)
									{
										t = (length_to_reach - preceding_length)/(current_length - preceding_length);
									}
									slm[0]= (1-t)*intvv[0] + t*intvv2[0];
									slm[1]= (1-t)*intvv[1] + t*intvv2[1];
									slm[2]= (1-t)*intvv[2] + t*intvv2[2];
									if (file_type==1)
									{
										fprintf (filein, "%s%d%s%d: %f %f %f 0.0 0.0 1.0\n", "curve", num_seg, "-",decimation_index, slm[0], slm[1], slm[2]);
									}
									else
									{
										fprintf (filein, "%s%d%s%d: %f %f %f\n", "curve", num_seg, "-",decimation_index, slm[0], slm[1], slm[2]);
									}
									t2= (float)(((float)decimation_index)/(((float)decimation-1.0)));
									length_to_reach= total_length*t2;						
									
								}

						
								preceding_length = current_length;
								}//for				
							
							}// end if


							if (ob_D2!=NULL && ob_D2->Is_Curve_Start() ==1 )
							{
							 

								num_seg++;
								t2=0;
								length_to_reach=0;
								current_length=0;
								preceding_length=0;
								decimation_index=0;
								total_length = Cont_Mesh.Get_Curve_Segment_Length(num_seg);
						
						
							}
							ind = ob_D1->landmark_index;
							indh = ob_H1->landmark_index;	
									
							ob_H1 = Cont_Mesh.landmarkafter (ind, 1);
							ob_D1 = Cont_Mesh.landmarkafter (ind, 0);
							if (ob_D1 !=NULL)
							{
								ind2 = ob_D1->landmark_index;
								ob_D2 = Cont_Mesh.landmarkafter (ind2, 0);
							}
							if (ob_H1 !=NULL)
							{
								indh2 = ob_H1->landmark_index;
								ob_H2 = Cont_Mesh.landmarkafter (indh2, 1);
							}
							if (ob_D1!=NULL&& ob_D1->Is_Curve_Start() ==1 ){
								ob_Hstart = ob_H1;
								ob_Dstart = ob_D1;

							}											
						}//while						
						fclose(filein);	
					}//if
				}//overwrite?
				break;			
			}//default
			
		}//switch
	}//ok
	
	return 1;

}

int MeshTools::Save_CUR_File()
{
	
	int ok=1;
	int landmark_normal_number = Cont_Mesh.Get_Landmark_Number(0);
	int landmark_target_number = Cont_Mesh.Get_Landmark_Number(1);		
	if (landmark_normal_number <2 || landmark_target_number <2)
	{
			
		fl_alert("Can't save curves: there must be at least 2 normal and 2 target landmarks.");
			
			ok =0;
	}
	else if (landmark_normal_number != landmark_target_number)
	{
		fl_alert("Can't save curves: normal and target landmark numbers should be identical.");			
			ok =0;
	}
	if (ok==1)
	{
		std::string filename;
		Fl_Native_File_Chooser fnfc;
		fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);				
		fnfc.title("Save curves");				
		fnfc.filter("CUR\t*.{cur,CUR}");			
		switch ( fnfc.show() ) 
		{
			case -1: 
			{
				//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
				break;  // ERROR
			}
			case  1: 
			{
				//std::cout<<"CANCEL\n";     
				break;  // CANCEL
			 }
			default: 
			{
				//std::cout<<"PICKED: "<<fnfc.filename()<<"\n";   // FILE CHOSEN					
				//Cont_Mesh.Mesh_STLwrite(szFile, Save_Obj);
				//filename = fnfc.filename();		
				filename = this->correct_filename(fnfc.filename());
				std::string CURext = ".cur";
				std::string CURext2 = ".CUR";
				std::size_t found = filename.find(CURext);
				std::size_t found2 = filename.find(CURext2);
				if (found == std::string::npos && found2 == std::string::npos)
				{
					filename.append(".cur");
				}
				ifstream file(filename.c_str());
				if (file)
				{
					ok = fl_ask("Overwrite?");
					file.close();

				}
				if (ok==1)
				{
					int num_landmark_D = Cont_Mesh.Get_Landmark_Number (0); //landmarks
					int num_landmark_H = Cont_Mesh.Get_Landmark_Number (1);	//handles
					int start=0;
					if (num_landmark_D == num_landmark_H)
					{						
						FILE	*filein;							
						//filein = fopen(this->VER_Sp_File, "w");	
						//fopen(&filein, filename.c_str(), "w");	
						//ifstream file(filename.c_str());
						filein=fopen(filename.c_str(), "w");	
					
						int cpt;
						glMatrix wc_mat;
						glMatrix wc_mat2;
						float vv[3], vv1[3], vv2[3];
						cpt=0;
						OBJECT_LANDMARK *k = NULL;
						OBJECT_LANDMARK *l = NULL;
						int ind = 0;
						int ind2 = 0;
						
						k = Cont_Mesh.landmarkafter (ind, 0);
						l = Cont_Mesh.landmarkafter (ind2, 1);
						while ( k != NULL)
						{
						
								k->get_world_coordinates_matrix(wc_mat);
								vv[0] =0;
								vv[1] =0;
								vv[2] =0;
								ApplyTransformation(vv,vv1,wc_mat);
								start = k->curve_start;
								if (ind==0){start=1;}
								//if (start!=1){start=0;}
								l->get_world_coordinates_matrix(wc_mat2);
								vv[0] =0;
								vv[1] =0;
								vv[2] =0;
								ApplyTransformation(vv,vv2,wc_mat2);
								
								cpt++;
								ind = k->landmark_index;
								ind2 = l->landmark_index;
								fprintf (filein, "%s%d: %f %f %f %f %f %f %d\n", "landmark", ind, vv1[0], vv1[1], vv1[2], vv2[0], vv2[1], vv2[2], start);
								k = Cont_Mesh.landmarkafter (ind, 0);
								l = Cont_Mesh.landmarkafter (ind2, 1);
						}				
						fclose(filein);								
					}//if
				}//overwrite?
				break;
			}//default	
		}//switch
	}//ok
	
	return 1;
}




int MeshTools::Save_POS_File(OBJECT_MESH *My_Obj, std::string filename)
{
	
	FILE	*filein;
	int length=0;
	
	int cpt;
	int ind_obj;
	ind_obj = 0;
	cpt = 0;
	int done = 0;
							
	
	length= filename.length();
	// determine the type of input file
	
	if (length> 0)
	{		
		
		std::string POSext = ".pos";
		std::string POSext2 = ".POS";
		std::size_t found = filename.find(POSext);
		std::size_t found2 = filename.find(POSext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			filename.append(".pos");
		}

																					
		filein = fopen(filename.c_str(), "w");					
				int i;
				for(i=0; i<4; i++) 
				{
					fprintf (filein, "%f %f %f %f\n", 
						My_Obj->Mat1[i][0], 
						My_Obj->Mat1[i][1],
						My_Obj->Mat1[i][2],
						My_Obj->Mat1[i][3]);
				}
				for(i=0; i<4; i++) 
				{
					fprintf (filein, "%f %f %f %f\n", 
						My_Obj->Mat2[i][0], 
						My_Obj->Mat2[i][1],
						My_Obj->Mat2[i][2],
						My_Obj->Mat2[i][3]);
				}																	
			fclose(filein);							
			return 1;		
	}
		else
		{return 0;}
//	}

}
int MeshTools::Save_POS_File()
{
	
	//Only save the position of a basal object...

	OBJECT_MESH * My_Obj;	
	OBJECT_LOG * My_Log;	
	FILE	*filein;
	
	int cpt;
	int ind_obj;
	ind_obj = 0;
	cpt = 0;
	int done = 0;
	//if(this->mode == MODE_STL)
	//{
	std::string filename;
	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);				
	fnfc.title("Save position");		
	fnfc.filter("POS\t*.{pos,POS}");		
	std::string Name = get_name_of_first_selected_obj();	
	fnfc.preset_file(Name.c_str());
	switch ( fnfc.show() ) 
	{
		case -1: 
		{
			//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
			break;  // ERROR
		}
		case  1: 
		{
			//std::cout<<"CANCEL\n";     
			break;  // CANCEL
		 }
		default: 
		{
			//std::cout<<"PICKED: "<<fnfc.filename()<<"\n";   // FILE CHOSEN					
			//Cont_Mesh.Mesh_STLwrite(szFile, Save_Obj);
			filename = this->correct_filename(fnfc.filename());
			std::string POSext = ".pos";
			std::string POSext2 = ".POS";
			std::size_t found = filename.find(POSext);
			std::size_t found2 = filename.find(POSext2);
			if (found == std::string::npos && found2 == std::string::npos)
			{
				filename.append(".pos");
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
				int Ok = 1;
				if (Cont_Mesh.OBJECTS_ROOT->OBJECTS !=NULL)
				{My_Obj =Cont_Mesh.OBJECTS_ROOT->OBJECTS ;}
				else
				{Ok = 0;}
				while (Ok)
				{	
					if ( My_Obj->selected == 1 && done ==0)
					{
						Ok = 0;
						done = 1;
					}
					else
					{	if (My_Obj->nextobj !=NULL)
						{
							My_Obj = My_Obj->nextobj;
						}
						else 
						{
								Ok = 0;
						}
					}
				}
				if (done)
				{					
														
					filein=fopen(filename.c_str(), "w");	
					int i;
					for(i=0; i<4; i++) 
					{
						fprintf (filein, "%f %f %f %f\n", 
							My_Obj->Mat1[i][0], 
							My_Obj->Mat1[i][1],
							My_Obj->Mat1[i][2],
							My_Obj->Mat1[i][3]);
					}
					for(i=0; i<4; i++) 
					{
						fprintf (filein, "%f %f %f %f\n", 
							My_Obj->Mat2[i][0], 
							My_Obj->Mat2[i][1],
							My_Obj->Mat2[i][2],
							My_Obj->Mat2[i][3]);
					}
												
					fclose(filein);		
				}
				else
				{
					//BEGIN
					Ok = 1;
					if (Cont_Mesh.OBJECTS_ROOT->OBJECTS_LOG !=NULL)
					{My_Log =Cont_Mesh.OBJECTS_ROOT->OBJECTS_LOG ;}
					else
					{Ok = 0;}
					while (Ok)
					{	
						if ( My_Log->selected == 1 && done ==0)
						{
							Ok = 0;
							done = 1;
						}
						else
						{	if (My_Log->nextobj !=NULL)
							{
								My_Log = My_Log->nextobj;
							}
							else 
							{
								Ok = 0;
							}
						}
					}
					if (done)
					{
						//filein = fopen(this->POS_Sp_File, "w");	
						//fopen(&filein, filename.c_str(), "w");	
						filein=fopen(filename.c_str(), "w");	
					
						int i;
						
						for(i=0; i<4; i++) 
						{
							fprintf (filein, "%f %f %f %f\n", 
								My_Log->Mat1[i][0], 
								My_Log->Mat1[i][1],
								My_Log->Mat1[i][2],
								My_Log->Mat1[i][3]);
						}
						for(i=0; i<4; i++) 
						{
							fprintf (filein, "%f %f %f %f\n", 
								My_Log->Mat2[i][0], 
								My_Log->Mat2[i][1],
								My_Log->Mat2[i][2],
								My_Log->Mat2[i][3]);
						}
									
						fclose(filein);		
					}
				}//else
			}//overwrite?
			break;
		}//default

	}//switch	
	
	return 1;

	
}




void MeshTools::Open_Pos_File( std::string filename, OBJECT_MESH * My_Obj)
{



	//Open a position file!
	
	int i,j,l;
	int  length;
	FILE	*filein;									// Filename To Open
	
	
	
	length=  filename.length();
	
	 union { float f;
     char c[4];
	 } u; // holds one float or 4 characters (bytes)

	 char	oneline[255];
	 float  param1, param2, param3, param4; // ASCII File 
	
  
	int done = 0;
	if (length>0)
	{	
		int file_exists=1;
		ifstream file(filename.c_str());		
		if (file)
		{
			//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
			file.close();
		}
		else
		{
			
			std::cout<<"file:"<<filename.c_str()<<" does not exists."<<std::endl;			
			file_exists =0;
		}
		
		if (file_exists==1)
		{

			std::string MAText (".mat");
			std::string MAText2 (".MAT");
			std::string POSext (".pos");
			std::string POSext2 (".POS");				
			
			int type =0; // 0 = .POS Ascii File //1 = .MAT binary File

			std::size_t found = filename.find(MAText);
			std::size_t found2 = filename.find(MAText2);
			if (found != std::string::npos || found2 != std::string::npos)
			{type=1; 
				//MAT
			}
			
			found = filename.find(POSext);
			found2 = filename.find(POSext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{type=0; //POS
			}

			//filein = fopen(szFile, "rb");
			if (type ==1)
			{
				filein =fopen(filename.c_str(), "rb");	
			}
			else
			{
				filein =fopen(filename.c_str(), "rt");	
			}

			int Ok = 1;
			
			My_Obj->bool_changed_matrix = 1;
			if (type ==1)
			{
						for( i=0; i<4; i++) 
							for( j=0; j<4; j++)
							{									
								
								for( l=3;l>=0;l--)
								{u.c[l]=fgetc(filein);}
								My_Obj->Mat1[i][j] = u.f;
							}


						for( i=0; i<4; i++) 
							for(  j=0; j<4; j++)
							{									
								for( l=3;l>=0;l--)
								{u.c[l]=fgetc(filein);}
								My_Obj->Mat2[i][j] = u.f;								
							}
						
			}
			else
			{
					for( i=0; i<4; i++) 
					{
							readstr(filein,oneline);
							
							sscanf(oneline, "%f %f %f %f\n", &param1, &param2, &param3, &param4);
							My_Obj->Mat1[i][0]= param1;
							My_Obj->Mat1[i][1]= param2;
							My_Obj->Mat1[i][2]= param3;
							My_Obj->Mat1[i][3]= param4;
						}
						for( i=0; i<4; i++) 
						{
							readstr(filein,oneline);
							sscanf(oneline, "%f %f %f %f\n", &param1, &param2, &param3, &param4);
							My_Obj->Mat2[i][0]= param1;
							My_Obj->Mat2[i][1]= param2;
							My_Obj->Mat2[i][2]= param3;
							My_Obj->Mat2[i][3]= param4;
						}	
						
															  
			}//fin if																		
			fclose (filein);
		}//file exists...
	}	//length

}
void MeshTools::Open_POS_File()
{
	
	//Open a position file!
	OBJECT_MESH * My_Obj;
	OBJECT * First_Obj;
	OBJECT_LOG * My_Log;
	int i,j,l;
	FILE	*filein;									// Filename To Open
	
	std::string filename;
	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);				
	fnfc.title("Load position");		
	fnfc.filter("POS\t*.{pos,POS}");		

	switch ( fnfc.show() ) 
	{
		case -1: 
		{
			//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
			break;  // ERROR
		}
		case  1: 
		{
			//std::cout<<"CANCEL\n";     
			break;  // CANCEL
		 }
		default: 
		{
			//std::cout<<"PICKED: "<<fnfc.filename()<<"\n";   // FILE CHOSEN					
			filename = this->correct_filename(fnfc.filename());
		
			int file_exists=1;
			ifstream file(filename.c_str());		
			if (file)
			{
				//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
				file.close();
			}
			else
			{
				
				std::cout<<"file:"<<filename.c_str()<<" does not exists."<<std::endl;			
				file_exists =0;
			}
			
			if (file_exists==1)
			{
				union { float f;
					char c[4];
				} u; // holds one float or 4 characters (bytes)
			  
				char	oneline[255];
				float  param1, param2, param3, param4; // ASCII File 			
				int done = 0;
				int done2 = 0;
				if (filename.length()> 0)
				{	//filein = fopen(szFile, "rb");
					//fopen_s(&filein, szFile, "rb");	
					std::string MAText (".mat");
					std::string MAText2 (".MAT");
					std::string POSext (".pos");
					std::string POSext2 (".POS");				
					
					int type =0; // 0 = .POS Ascii File //1 = .MAT binary File

					std::size_t found = filename.find(MAText);
					std::size_t found2 = filename.find(MAText2);
					if (found != std::string::npos || found2 != std::string::npos)
					{type=1; 
						//MAT
					}
					
					found = filename.find(POSext);
					found2 = filename.find(POSext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{type=0; //POS
					}

					if (type ==1)
					{
						filein =fopen(filename.c_str(), "rb");	
					}
					else
					{
						filein =fopen(filename.c_str(), "rt");	
					}
					
					
					int Ok = 1;
					if (Cont_Mesh.OBJECTS_ROOT->OBJECTS !=NULL)
					{
						My_Obj = Cont_Mesh.OBJECTS_ROOT->OBJECTS;
					}
					else
					{

						Ok = 0;
					}
					while (Ok !=0)
					{
						if ((My_Obj->selected ==1)&&(done == 0))
						{
							My_Obj->bool_changed_matrix = 1;
							if (type ==1)
							{
								for( i=0; i<4; i++) 
									for( j=0; j<4; j++)
									{									
									
										for( l=3;l>=0;l--)
										{u.c[l]=fgetc(filein);}
										My_Obj->Mat1[i][j] = u.f;
									}


								for( i=0; i<4; i++) 
									for(  j=0; j<4; j++)
									{
										//for( l=0;l<4;l++)
										for( l=3;l>=0;l--)
										{u.c[l]=fgetc(filein);}
										My_Obj->Mat2[i][j] = u.f;								
									}
							}
							else
							{
								for( i=0; i<4; i++) 
								{
									readstr(filein,oneline);
									sscanf(oneline, "%f %f %f %f\n", &param1, &param2, &param3, &param4);
									My_Obj->Mat1[i][0]= param1;
									My_Obj->Mat1[i][1]= param2;
									My_Obj->Mat1[i][2]= param3;
									My_Obj->Mat1[i][3]= param4;
								}
								for( i=0; i<4; i++) 
								{
									readstr(filein,oneline);
									sscanf(oneline, "%f %f %f %f\n", &param1, &param2, &param3, &param4);
									My_Obj->Mat2[i][0]= param1;
									My_Obj->Mat2[i][1]= param2;
									My_Obj->Mat2[i][2]= param3;
									My_Obj->Mat2[i][3]= param4;
								}
							
							}
							done = 1;
							First_Obj = My_Obj;
							

						}//fin if
						if (My_Obj->nextobj !=NULL)
						{My_Obj = My_Obj->nextobj;}
						else{Ok = 0;}
					}//while
				

									
					
					Ok = 1;
					
					if (Cont_Mesh.OBJECTS_ROOT->OBJECTS_LOG !=NULL)
					{
						My_Log = Cont_Mesh.OBJECTS_ROOT->OBJECTS_LOG;
					}
					else
					{

						Ok = 0;
					}
					while (Ok !=0)
					{
						if ((My_Log->selected ==1)&&(done == 0))
						{
							if (type==1)
							{
								for( i=0; i<4; i++) 
									for( j=0; j<4; j++)
									{									
										
										for( l=3;l>=0;l--)
										{u.c[l]=fgetc(filein);}
										My_Log->Mat1[i][j] = u.f;
									}


								for( i=0; i<4; i++) 
									for(  j=0; j<4; j++)
									{
										//for( l=0;l<4;l++)
										for( l=3;l>=0;l--)
										{u.c[l]=fgetc(filein);}
										My_Log->Mat2[i][j] = u.f;								
									}
							}
							
							else
							{
								for( i=0; i<4; i++) 
								{
									readstr(filein,oneline);
									sscanf(oneline, "%f %f %f %f\n", &param1, &param2, &param3, &param4);
									My_Log->Mat1[i][0]= param1;
									My_Log->Mat1[i][1]= param2;
									My_Log->Mat1[i][2]= param3;
									My_Log->Mat1[i][3]= param4;
								}
								for( i=0; i<4; i++) 
								{
									readstr(filein,oneline);
									sscanf(oneline, "%f %f %f %f\n", &param1, &param2, &param3, &param4);
									My_Log->Mat2[i][0]= param1;
									My_Log->Mat2[i][1]= param2;
									My_Log->Mat2[i][2]= param3;
									My_Log->Mat2[i][3]= param4;
								}
							
							
							}	
							done = 1;										
							
							First_Obj=My_Log;
						}//fin if
						if (My_Log->nextobj !=NULL)
						{My_Log = My_Log->nextobj;}
						else{Ok = 0;}
					}//while

					Ok=1;
					if (done==1)
					{					
						
						done =0;
						
						if (Cont_Mesh.OBJECTS_ROOT->OBJECTS_LOG !=NULL)
						{
							My_Log = Cont_Mesh.OBJECTS_ROOT->OBJECTS_LOG;
						}
						else
						{

							Ok = 0;
						}

						while (Ok !=0)
						{
							if ((My_Log->selected ==1))
							{
									for( i=0; i<4; i++) 
										for( j=0; j<4; j++)
										{																	
											My_Log->Mat1[i][j] = First_Obj->Mat1[i][j];
											My_Log->Mat2[i][j] = First_Obj->Mat2[i][j];
										}
							}
							if (My_Log->nextobj !=NULL)
							{My_Log = My_Log->nextobj;}
							else{Ok = 0;}
						}					
						Ok=1;					
						if (Cont_Mesh.OBJECTS_ROOT->OBJECTS !=NULL)
						{
							My_Obj = Cont_Mesh.OBJECTS_ROOT->OBJECTS;
						}
						else
						{

							Ok = 0;
						}
						while (Ok !=0)
						{
							if ((My_Obj->selected ==1))
							{
								My_Obj->bool_changed_matrix = 1;
									for( i=0; i<4; i++) 
										for( j=0; j<4; j++)
										{																	
											My_Obj->Mat1[i][j] = First_Obj->Mat1[i][j];
											My_Obj->Mat2[i][j] = First_Obj->Mat2[i][j];
										}
							}
							if (My_Obj->nextobj !=NULL)
							{My_Obj = My_Obj->nextobj;}
							else{Ok = 0;}
						}
						done =1;
					}
					fclose (filein);
				}//if filename.length...
				break;
			}// if file exists
		}//default
	}//switch
					
	this->Compute_Global_Mean();
	
}
void MeshTools::Open_POS_File_Inv()
{
	
	//Open a position file!
	OBJECT_MESH * My_Obj;
	OBJECT_LOG * My_Log;
	int i,j,l;
	FILE	*filein;									// Filename To Open
	std::string filename;
	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);				
	fnfc.title("Load position");		
	fnfc.filter("POS\t*.{pos,POS}");		

	switch ( fnfc.show() ) 
	{
		case -1: 
		{
			//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
			break;  // ERROR
		}
		case  1: 
		{
			//std::cout<<"CANCEL\n";     
			break;  // CANCEL
		 }
		default: 
		{
			//std::cout<<"PICKED: "<<fnfc.filename()<<"\n";   // FILE CHOSEN					
			filename = this->correct_filename(fnfc.filename());
			int file_exists=1;
			ifstream file(filename.c_str());		
			if (file)
			{
				//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
				file.close();
			}
			else
			{
				
				std::cout<<"file:"<<filename.c_str()<<" does not exists."<<std::endl;			
				file_exists =0;
			}
			
			if (file_exists==1)
			{
				 union { float f;
				 char c[4];
				 } u; // holds one float or 4 characters (bytes)
			  
				 char	oneline[255];
				float  param1, param2, param3, param4; // ASCII File 
				

				int done = 0;
				if (filename.length()> 0)
				{	//filein = fopen(szFile, "rb");
					//fopen_s(&filein, szFile, "rb");	
				
					std::string MAText (".mat");
					std::string MAText2 (".MAT");
					std::string POSext (".pos");
					std::string POSext2 (".POS");				
					
					int type =0; // 0 = .POS Ascii File //1 = .MAT binary File

					std::size_t found = filename.find(MAText);
					std::size_t found2 = filename.find(MAText2);
					if (found != std::string::npos || found2 != std::string::npos)
					{type=1; 
						//MAT
					}
					
					found = filename.find(POSext);
					found2 = filename.find(POSext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{type=0; //POS
					}

					//filein = fopen(szFile, "rb");
					if (type ==1)
					{
						filein =fopen(filename.c_str(), "rb");	
					}
					else
					{
						filein =fopen( filename.c_str(), "rt");	
					}

					
					int Ok = 1;
					if (Cont_Mesh.OBJECTS_ROOT->OBJECTS !=NULL)
					{
						My_Obj = Cont_Mesh.OBJECTS_ROOT->OBJECTS;
					}
					else
					{

						Ok = 0;
					}
					while (Ok !=0)
					{
						if ((My_Obj->selected ==1)&&(done == 0))
						{
							My_Obj->bool_changed_matrix = 1;
							if (type ==1)
							{
								
								for( i=0; i<4; i++) 
									for( j=0; j<4; j++)
							    
									{									
									for( l=3;l>=0;l--)
										{u.c[l]=fgetc(filein);}
										My_Obj->Mat1[j][i] = u.f;
									}
									

								for( i=0; i<4; i++) 
									for(  j=0; j<4; j++)
									{
										//for( l=0;l<4;l++)
										for( l=3;l>=0;l--)
										{u.c[l]=fgetc(filein);}
										My_Obj->Mat2[j][i] = u.f;								
									}
							}
							else
							{
								for( i=0; i<4; i++) 
								{
									readstr(filein,oneline);
									sscanf(oneline, "%f %f %f %f\n", &param1, &param2, &param3, &param4);
									My_Obj->Mat1[0][i]= param1;
									My_Obj->Mat1[1][i]= param2;
									My_Obj->Mat1[2][i]= param3;
									My_Obj->Mat1[3][i]= param4;
								}
								for( i=0; i<4; i++) 
								{
									readstr(filein,oneline);
									sscanf(oneline, "%f %f %f %f\n", &param1, &param2, &param3, &param4);
									My_Obj->Mat2[0][i]= param1;
									My_Obj->Mat2[1][i]= param2;
									My_Obj->Mat2[2][i]= param3;
									My_Obj->Mat2[3][i]= param4;
								}
							
							
							}
							// line colonne
							My_Obj->Mat1[3][0] = -(My_Obj->Mat1[0][3]*My_Obj->Mat1[0][0]+ 
								My_Obj->Mat1[1][3]*My_Obj->Mat1[1][0]
								+My_Obj->Mat1[2][3]*My_Obj->Mat1[2][0]);

							My_Obj->Mat1[3][1] = -(My_Obj->Mat1[0][3]*My_Obj->Mat1[0][1]+ 
								My_Obj->Mat1[1][3]*My_Obj->Mat1[1][1]
								+My_Obj->Mat1[2][3]*My_Obj->Mat1[2][1]);

							My_Obj->Mat1[3][2] = -(My_Obj->Mat1[0][3]*My_Obj->Mat1[0][2]+ 
								My_Obj->Mat1[1][3]*My_Obj->Mat1[1][2]
								+My_Obj->Mat1[2][3]*My_Obj->Mat1[2][2]);
							My_Obj->Mat1[0][3]=0;
							My_Obj->Mat1[1][3]=0;
							My_Obj->Mat1[2][3]=0;
											
							My_Obj->Mat2[3][0] = -(My_Obj->Mat2[0][3]*My_Obj->Mat2[0][0]+ 
								My_Obj->Mat2[1][3]*My_Obj->Mat2[1][0]
								+My_Obj->Mat2[2][3]*My_Obj->Mat2[2][0]);

							My_Obj->Mat2[3][1] = -(My_Obj->Mat2[0][3]*My_Obj->Mat2[0][1]+ 
								My_Obj->Mat2[1][3]*My_Obj->Mat2[1][1]
								+My_Obj->Mat2[2][3]*My_Obj->Mat2[2][1]);

							My_Obj->Mat2[3][2] = -(My_Obj->Mat2[0][3]*My_Obj->Mat2[0][2]+ 
								My_Obj->Mat2[1][3]*My_Obj->Mat2[1][2]
								+My_Obj->Mat2[2][3]*My_Obj->Mat2[2][2]);
							My_Obj->Mat2[0][3]=0;
							My_Obj->Mat2[1][3]=0;
							My_Obj->Mat2[2][3]=0;

									
							done = 1;
						

						}//fin if
						if (My_Obj->nextobj !=NULL)
						{My_Obj = My_Obj->nextobj;}
						else{Ok = 0;}
					}//while


					

					Ok = 1;
					
					if (Cont_Mesh.OBJECTS_ROOT->OBJECTS_LOG !=NULL)
					{
						My_Log = Cont_Mesh.OBJECTS_ROOT->OBJECTS_LOG;
					}
					else
					{

						Ok = 0;
					}
					while (Ok !=0)
					{
						if ((My_Log->selected ==1)&&(done == 0))
						{
							if (type ==1)
							{
								for( i=0; i<4; i++) 
									for( j=0; j<4; j++)
									{									
										for( l=3;l>=0;l--)
										{u.c[l]=fgetc(filein);}
										My_Log->Mat1[j][i] = u.f;
									}


								for( i=0; i<4; i++) 
									for(  j=0; j<4; j++)
									{
										//for( l=0;l<4;l++)
										for( l=3;l>=0;l--)
										{u.c[l]=fgetc(filein);}
										My_Log->Mat2[j][i] = u.f;								
									}
								
							}
							else
							{
								for( i=0; i<4; i++) 
								{
									readstr(filein,oneline);
									sscanf(oneline, "%f %f %f %f\n", &param1, &param2, &param3, &param4);
									My_Log->Mat1[0][i]= param1;
									My_Log->Mat1[1][i]= param2;
									My_Log->Mat1[2][i]= param3;
									My_Log->Mat1[3][i]= param4;
								}
								for( i=0; i<4; i++) 
								{
									readstr(filein,oneline);
									sscanf(oneline, "%f %f %f %f\n", &param1, &param2, &param3, &param4);
									My_Log->Mat2[0][i]= param1;
									My_Log->Mat2[1][i]= param2;
									My_Log->Mat2[2][i]= param3;
									My_Log->Mat2[3][i]= param4;
								}										
							}
							My_Log->Mat1[3][0] = -(My_Log->Mat1[0][3]*My_Log->Mat1[0][0]+ 
								My_Log->Mat1[1][3]*My_Log->Mat1[1][0]
								+My_Log->Mat1[2][3]*My_Log->Mat1[2][0]);

							My_Log->Mat1[3][1] = -(My_Log->Mat1[0][3]*My_Log->Mat1[0][1]+ 
								My_Log->Mat1[1][3]*My_Log->Mat1[1][1]
								+My_Log->Mat1[2][3]*My_Log->Mat1[2][1]);

							My_Log->Mat1[3][2] = -(My_Log->Mat1[0][3]*My_Log->Mat1[0][2]+ 
								My_Log->Mat1[1][3]*My_Log->Mat1[1][2]
								+My_Log->Mat1[2][3]*My_Log->Mat1[2][2]);
							My_Log->Mat1[0][3]=0;
							My_Log->Mat1[1][3]=0;
							My_Log->Mat1[2][3]=0;

							My_Log->Mat2[3][0] = -(My_Log->Mat2[0][3]*My_Log->Mat2[0][0]+ 
								My_Log->Mat2[1][3]*My_Log->Mat2[1][0]
								+My_Log->Mat2[2][3]*My_Log->Mat2[2][0]);

							My_Log->Mat2[3][1] = -(My_Log->Mat2[0][3]*My_Log->Mat2[0][1]+ 
								My_Log->Mat2[1][3]*My_Log->Mat2[1][1]
								+My_Log->Mat2[2][3]*My_Log->Mat2[2][1]);

							My_Log->Mat2[3][2] = -(My_Log->Mat2[0][3]*My_Log->Mat2[0][2]+ 
								My_Log->Mat2[1][3]*My_Log->Mat2[1][2]
								+My_Log->Mat2[2][3]*My_Log->Mat2[2][2]);
							My_Log->Mat2[0][3]=0;
							My_Log->Mat2[1][3]=0;
							My_Log->Mat2[2][3]=0;


							done = 1;										

						}//fin if
						if (My_Log->nextobj !=NULL)
						{My_Log = My_Log->nextobj;}
						else{Ok = 0;}
					}//while								
				
				
					fclose (filein);
				}//if filename.length...
				break;
			}//if file exists
		}//default
	}//switch
	this->Compute_Global_Mean();
	
}

void MeshTools::Save_Infos()
{
	//to do
	
	FILE	*filein;
	float volume, area;
	OBJECT_MESH *My_Obj;
	// strcpy(name, fl_filename_name(s2));
	OBJECT_LOG *My_Log;

	std::string filename;
	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);				
	fnfc.title("Save infos");		
	fnfc.filter("TXT\t*.{txt,TXT}");		

	switch ( fnfc.show() ) 
	{
		case -1: 
		{
			//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
			break;  // ERROR
		}
		case  1: 
		{
			//std::cout<<"CANCEL\n";     
			break;  // CANCEL
		 }
		default: 
		{
			//std::cout<<"PICKED: "<<fnfc.filename()<<"\n";   // FILE CHOSEN					
			//Cont_Mesh.Mesh_STLwrite(szFile, Save_Obj);
			filename = this->correct_filename(fnfc.filename());
			std::string TXText = ".txt";
			std::string TXText2 = ".TXT";
			std::size_t found = filename.find(TXText);
			std::size_t found2 = filename.find(TXText2);
			if (found == std::string::npos && found2 == std::string::npos)
			{
				filename.append(".txt");
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
		
				My_Obj = NULL;
				//filein = fopen(szFile, "w");	
				filein =fopen(filename.c_str(), "w");	
				My_Log = Cont_Mesh.OBJECTS_ROOT;
				if (My_Log !=NULL)
				{My_Obj = My_Log->OBJECTS;}
				int i = 0;
				fprintf (filein, "Name Volume(mm3) Area(mm2) Triangle_number Vert_number\n");
				while (My_Obj !=NULL) 
				{	
					if (My_Obj->selected ==1)
					{
						volume = My_Obj->get_volume();
						area = My_Obj->get_area();
					i++;
					//itoa( camera.tw, tw, 10);																												
					fprintf (filein, "%s %f %f %d %d\n", My_Obj->name.c_str(), volume, area, My_Obj->numtri,My_Obj->numvert);
					}
					My_Obj = My_Obj->nextobj;						
				}				
				fclose (filein);					
			}//overwrite?
			break;
		}//default
	}//switch	
	this->redraw();


}

void MeshTools::Stick_Selected_Landmarks_On_Surfaces()
{

	Cont_Mesh.Stick_Selected_Landmarks_On_Surfaces();
}

int MeshTools::selected_file_exists(std::string path, std::string ext, std::string postfix)
{
	std::string filename;
	int exists =0;
	for (int i=0; i<g_distinct_selected_names.size();i++)
	{
		filename = path.c_str();
		filename.append(g_distinct_selected_names.at(i).c_str());
		if (postfix.length()>0)
		{
			filename.append(postfix.c_str());
		}
		filename.append(ext.c_str());
		ifstream file(filename.c_str());		
		if (file)
		{					
			file.close();
			exists=1;
			break;
		}
	}

	return exists;

}
void MeshTools::Save_NTW_File()
{
	
	// Save network : we keep current object names	
	//New : all files are saved as binary poly data (*.VTK)
	//(the possibility remains to read .ntw files with .STL files)
	//New : now all positions are saved as ASCII .POS files 
	//(the possibility remains to read .ntw files with .MAT positions files)

	FILE	*filein;
	OBJECT_MESH *My_Obj;	
	OBJECT_LOG *My_Log;
	std::string filename;
	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);				
	fnfc.title("Save network");		
	fnfc.filter("NTW\t*.{ntw,NTW}");	
	int ok=1;
	this->Compute_Name_Lists();
	if (g_selected_names.size()==0)
	{
		fl_alert("Can't save network: at least one mesh must be selected");
				ok =0;
	}
	else if (g_selected_names.size()!= g_distinct_selected_names.size())
	{
		fl_alert("Can't save network: at least two selected objects share the same name");
				ok =0;
	}
	if(ok==1)
		{

		switch ( fnfc.show() ) 
		{
			case -1: 
			{
				//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
				break;  // ERROR
			}
			case  1: 
			{
				//std::cout<<"CANCEL\n";     
				break;  // CANCEL
			 }
			default: 
			{
				//std::cout<<"PICKED: "<<fnfc.filename()<<"\n";   // FILE CHOSEN	
				
				//Cont_Mesh.Mesh_STLwrite(szFile, Save_Obj);
				filename = this->correct_filename(fnfc.filename());
				
				std::string NTWext = ".ntw";
				std::string NTWext2 = ".NTW";
				std::size_t found = filename.find(NTWext);
				std::size_t found2 = filename.find(NTWext2);
				if (found == std::string::npos && found2 == std::string::npos)
				{
					filename.append(".ntw");
				}
				ifstream file(filename.c_str());
				int overwrite=1;
				if (file)
				{
					overwrite = fl_ask("Overwrite project file?");
					file.close();

				}
				if (overwrite==1)
				{	

					std::string only_filename = fl_filename_name(filename.c_str());
					std::string project_name = only_filename.c_str();
					int nPos = project_name.find_first_of(".");			
					if ( nPos > -1 )
					{
						project_name = project_name.substr(0, nPos);
					}	
					std::string path = filename.substr(0, (filename.length()-only_filename.length()));
					std::string posExt=".pos";
					std::string vtkExt=".vtk";
					int overwrite_pos=1;
					int overwrite_vtk=1;

					this->Compute_Name_Lists();
					std::string postfix="_";
					postfix.append(project_name.c_str());
					std::string no_postfix="";
					int pos_exists= this->selected_file_exists(path, posExt, postfix);

					int vtk_exists= this->selected_file_exists(path, vtkExt,no_postfix);
					if (vtk_exists==1)
					{
						overwrite_vtk = fl_ask("At least one vtk mesh file already exists: update existing mesh files?");
					}
					if (pos_exists==1)
					{
						overwrite_pos = fl_ask("At least one position file already exists: update existing position files?");
					}
														
										
					
					std::string _vtk_fullpath;
					std::string _pos_fullpath;
					std::string _vtk_file;
					std::string _pos_file;			
				
					My_Obj = NULL;					
					filein =fopen(filename.c_str(), "w");	
					My_Log = Cont_Mesh.OBJECTS_ROOT;
					if (My_Log !=NULL)
					{My_Obj = My_Log->OBJECTS;}
					int i = 0;
					while (My_Obj !=NULL) 
					{	
						if (My_Obj->selected ==1)
						{
							i++;
							//itoa( camera.tw, tw, 10);													
							_vtk_file = My_Obj->name.c_str();
							_pos_file = My_Obj->name.c_str();
							_vtk_file.append(".vtk");
							_pos_file.append(postfix.c_str());
							_pos_file.append(".pos");					
																				
							_vtk_fullpath= path.c_str();
							_vtk_fullpath.append(_vtk_file.c_str());

							_pos_fullpath=path.c_str();
							_pos_fullpath.append(_pos_file.c_str());

							int write =1;
							if (overwrite_vtk==0)
							{
								// in that case, check if file exists...
								ifstream file2(_vtk_fullpath.c_str());
								int overwrite=1;
								if (file2)
								{
									write=0;
									file2.close();
								}							
							}
							if (write==1)
							{Save_Mesh_File (My_Obj, _vtk_fullpath, 1, 0);}

							write =1;
							if (overwrite_pos==0)
							{
								// in that case, check if file exists...								
								ifstream file2(_pos_fullpath.c_str());
								int overwrite=1;
								if (file2)
								{
									write=0;
									file.close();
								}		
							
							}
							if (write ==1)
							{Save_POS_File(My_Obj, _pos_fullpath);}
							fprintf (filein, "%s\n", _vtk_file.c_str());			
							fprintf (filein, "%s\n", _pos_file.c_str());	
							
							fprintf (filein, "%f %f %f %f\n", My_Obj->color[0], My_Obj->color[1],My_Obj->color[2], My_Obj->color[3]);						
						}
						My_Obj = My_Obj->nextobj;				
					}//while
					fclose (filein);
				}//overwrite?
				break;
			}//default
		}//switch	
	}//ok==1
	
	this->redraw();
}
void MeshTools::Open_NTW_File()
{
	
	FILE	*filein;
	
	float color1, color2, color3, color4;
	char param1[1000];
	OBJECT_MESH *My_Obj;
	char	oneline[1000];
	//char *szFile = fl_file_chooser("Load NTW", "*.ntw*", "");
	// Filename To Open
	
	
	int i=0;
	int length;
	std::string filename;
	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);				
	fnfc.title("Open network");		
	fnfc.filter("NTW\t*.{ntw,NTW}");			
	switch ( fnfc.show() ) 
	{
		case -1: 
		{
			//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
			break;  // ERROR
		}
		case  1: 
		{
			//std::cout<<"CANCEL\n";     
			break;  // CANCEL
		 }
		default: 
		{
			filename = this->correct_filename(fnfc.filename());
			int file_exists=1;
			ifstream file(filename.c_str());		
			if (file)
			{
				//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
				file.close();
			}
			else
			{
				
				std::cout<<"file:"<<filename.c_str()<<" does not exists."<<std::endl;			
				file_exists =0;
			}
			
			if (file_exists==1)
			{
				std::string only_filename = fl_filename_name(filename.c_str());
				std::string path = filename.substr(0, (filename.length()-only_filename.length()));
					
				Mesh_UnselectAll();
				//filein = fopen(szFile, "rt");
				//fopen_s(&filein, filename.c_str(), "rt");	
				filein =fopen(filename.c_str(), "r");	
				readstr(filein,oneline);
				int ok=0;
				while (!feof(filein)) 
				{		
					sscanf(oneline, "%s\n", param1);
					
					if (i==0)
					{
						
					  //length=(int)strlen(oneline);						
					  //strncpy(param1, oneline, length-1);
						
						std::string meshname = param1;
						std::string STLext (".stl");
						std::string STLext2 (".STL");
						std::string VTKext (".vtk");
						std::string VTKext2 (".VTK");
						std::string OBJext (".obj");
						std::string OBJext2 (".OBJ");
						std::string PLYext (".ply");
						std::string PLYext2 (".PLY");
						
						int type =0; //0 = stl, 1 = vtk, 2 = obj, 3 = ply
						std::size_t found = meshname.find(STLext);
						std::size_t found2 = meshname.find(STLext2);
						if (found != std::string::npos || found2 != std::string::npos)
						{type=0; 
							//STL
						}

						//std::cout << "0Type= " <<type<< std::endl;
						found = meshname.find(VTKext);
						found2 = meshname.find(VTKext2);
						if (found != std::string::npos || found2 != std::string::npos)
						{type=1; //VTK
						}
						//std::cout << "1Type= " <<type<< std::endl;
						found = meshname.find(OBJext);
						found2 = meshname.find(OBJext2);
						if (found != std::string::npos || found2 != std::string::npos)
						{type=2; //OBJ
						}
						//std::cout << "2Type= " <<type<< std::endl;
						found = meshname.find(PLYext);
						found2 = meshname.find(PLYext2);
						if (found != std::string::npos || found2 != std::string::npos)
						{type=3; //PLY
						}
						//std::cout << "3Type= " <<type<< std::endl;

						std::string meshfilename = fl_filename_name(meshname.c_str());
						OBJECT_MESH *MyObj = NULL;
						vtkSmartPointer<vtkPolyDataNormals> ObjNormals =  vtkSmartPointer<vtkPolyDataNormals>::New();		

						if(meshname.length()==meshfilename.length())
						{
							meshname= path.c_str();
							meshname.append(meshfilename.c_str());
						}

						if (type ==0)
						{

							vtkSmartPointer<vtkSTLReader> reader =
							vtkSmartPointer<vtkSTLReader>::New();

							reader->SetFileName(meshname.c_str());
							reader->Update();
							ObjNormals->SetInputData(reader->GetOutput());
							//MyObj= (OBJECT_MESH*)reader->GetOutput();
						}
						else if (type ==1)
						{
							vtkSmartPointer<vtkPolyDataReader> reader =
							vtkSmartPointer<vtkPolyDataReader>::New();

							reader->SetFileName(meshname.c_str());
							reader->Update();
							ObjNormals->SetInputData(reader->GetOutput());
							//MyObj= (OBJECT_MESH*)reader->GetOutput();
						
						}
						else if (type ==2)
						{
							vtkSmartPointer<vtkOBJReader> reader =
							vtkSmartPointer<vtkOBJReader>::New();

							reader->SetFileName(meshname.c_str());
							reader->Update();
							ObjNormals->SetInputData(reader->GetOutput());
							//MyObj= (OBJECT_MESH*)reader->GetOutput();
						
						}
						else
						{
							vtkSmartPointer<vtkPLYReader> reader =
							vtkSmartPointer<vtkPLYReader>::New();

							reader->SetFileName(meshname.c_str());
							reader->Update();
							ObjNormals->SetInputData(reader->GetOutput());
							//MyObj= (OBJECT_MESH*)reader->GetOutput();
						
						}
						

						
						
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

						MyObj= (OBJECT_MESH*)cleanPolyDataFilter->GetOutput();
						
						std::cout << "\nNumber of points 1:"<<MyObj->GetNumberOfPoints()<< std::endl;
						std::cout << "\nNumber of cells 1:"<<MyObj->GetNumberOfCells()<< std::endl;		

				
						vtkFloatArray* norms = vtkFloatArray::SafeDownCast
							(MyObj->GetCellData()->GetNormals());
								std::cout << "Safe cell downcast done ! " << std::endl;
						if(norms)
						{

								std::cout << "There are here " << norms->GetNumberOfTuples()
							  << " Float Cell normals in norms" << std::endl;
						}
						else
						{
								std::cout << "FloatNorms CELL is null " << std::endl;
						}

						norms = vtkFloatArray::SafeDownCast
							(MyObj->GetPointData()->GetNormals());
								std::cout << "Safe point downcast done ! " << std::endl;
						if(norms)
						{

								std::cout << "There are  " << norms->GetNumberOfTuples()
							  << " Float POINT normals in norms" << std::endl;
						}
						else
						{
								std::cout << "FloatNorms POINTS is null " << std::endl;
						}

				
						if (MyObj->GetNumberOfPoints()>10)
						{

							ok=1;
							
							std::string newname=fl_filename_name(param1);

							int nPos = newname.find_first_of(".");			
							if ( nPos > -1 )
							{
								newname = newname.substr(0, nPos);
							}			

							CheckingName(&newname);
							My_Obj = Cont_Mesh.Mesh_PDcontainerload(MyObj, (char*)newname.c_str());	
							My_Obj->Set_Active_Scalar();
							
							My_Obj->selected=1;
							std::cout <<"Object <<"<<My_Obj->name.c_str()<<">> loaded"<< std::endl;
						}
						else
						{
							ok=0;
						}
													
						
					}
					if (i==1)
					{	
						if (ok)
						{
							
						  //length= (int)strlen(oneline);						
							//strncpy(param1, oneline, length-1);
							std::string posfile=param1;
							//posfile = posfile.substr(0, (posfile.length()-1));// for some reason
												
							std::string posfilename = fl_filename_name(posfile.c_str());										
							if(posfile.length()==posfilename.length())
							{
								posfile= path.c_str();
								posfile.append(posfilename.c_str());
							}
							std::cout <<"Try to load position :<<"<<posfile.c_str()<< std::endl;
							this->Open_Pos_File(posfile, My_Obj);
							//std::cout <<"Object <<"<<My_Obj->name.c_str()<<">> position loaded"<< std::endl;
							My_Obj->selected =0;
						}
					}
					if (i==2)
					{
						if (ok)
						{
							sscanf(oneline, "%f %f %f %f\n", &color1, &color2,&color3, &color4);
							//std::cout <<"color 1"<<color1<<",color 2"<<color3<<",color 3"<<color3<<",color 4"<<color4<< std::endl;
							My_Obj->color[0] = color1; My_Obj->color[1] = color2;My_Obj->color[2] = color3; My_Obj->color[3] = color4;
							My_Obj->blend =color4;
							My_Obj->Update_RGB();
						}
					}
					i ++;
					if (i>2)
					{i = 0;}
					
					readstr(filein,oneline); //read next line
						
				}//While scanff...						
				
				fclose (filein);
				break;
			}//if file exists
		}//default
	}//switch
	this->Compute_Global_Mean();
	this->redraw();
	
}
void MeshTools::Selected_Landmarks_Change_Orientation()
{
	Cont_Mesh.Selected_Landmarks_Change_Orientation(0);
	Cont_Mesh.Selected_Landmarks_Change_Orientation(1);
}

void MeshTools::Open_Landmarks(int landmark_mode)
{

	float  param2, param3, param4, param5, param6, param7;
	float m_ve[3], m_ven[3], leng;
	char param1[50];	
	FILE	*filein;									// Filename To Open
//	OBJECT_LANDMARK * My_Landmark;
	char	oneline[255];
	

	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);				
	fnfc.title("Load landmarks");				
	fnfc.filter("Landmarks\t*.{ver,VER,lmk,LMK}");		
	switch ( fnfc.show() ) 
	{
		case -1: 
		{
			//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
			break;  // ERROR
		}
		case  1: 
		{
			//std::cout<<"CANCEL\n";     
			break;  // CANCEL
		 }
		default: 
		{						
			std::string filename = this->correct_filename(fnfc.filename());
			//char *szFile = fl_file_chooser("Load landmarks", "*.{ver,VER,lmk,LMK}", "");	
			int file_exists=1;
			ifstream file(filename.c_str());		
			if (file)
			{
				//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
				file.close();
			}
			else
			{
				
				std::cout<<"file:"<<filename.c_str()<<" does not exists."<<std::endl;			
				file_exists =0;
			}
			
			if (file_exists==1)
			{
				std::string VERext (".ver");
				std::string VERext2 (".VER");
				std::string LMKext (".lmk");
				std::string LMKext2 (".LMK");
				
				int type =0; //0 = lmk, 1 = ver
				std::size_t found = filename.find(VERext);
				std::size_t found2 = filename.find(VERext2);
				if (found != std::string::npos || found2 != std::string::npos)
				{
					type=1; //VER
					
				}
				else
				{
					type=0;
				}
			
				//filein = fopen(szFile, "rt");
				filein =fopen(filename.c_str(), "r");	
				readstr(filein,oneline);
				feof(filein);
				std::cout << "Try open landmark file "  << std::endl;
				std::cout << "feof(filein)"  <<feof(filein)<< std::endl;
				
				vtkSmartPointer<vtkFloatArray> param_list = vtkSmartPointer<vtkFloatArray>::New();
				param_list->SetNumberOfComponents(1);

				while (!feof(filein)) 
				{		
					//std::cout << "Try sscanf_s "  <<oneline<< std::endl;
					//sscanf(oneline, "%s%f %f %f %f %f %f\n", param1, &param2, &param3, &param4, &param5, &param6, &param7, _countof(param1));
					if (type ==1)
					{
						sscanf(oneline, "%s %f %f %f %f %f %f\n", param1, &param2, &param3, &param4, &param5, &param6, &param7);
						param_list->InsertNextTuple1(param2);
						param_list->InsertNextTuple1(param3);
						param_list->InsertNextTuple1(param4);
						param_list->InsertNextTuple1(param5);
						param_list->InsertNextTuple1(param6);
						param_list->InsertNextTuple1(param7);
					}
					else
					{
						sscanf(oneline, "%s %f %f %f\n", param1, &param2, &param3, &param4);
						param_list->InsertNextTuple1(param2);
						param_list->InsertNextTuple1(param3);
						param_list->InsertNextTuple1(param4);
					}
					create_landmarks(landmark_mode,param_list,type);

					param_list = vtkSmartPointer<vtkFloatArray>::New();
					param_list->SetNumberOfComponents(1);

					
					readstr(filein,oneline); //read next line
						
				}//While scanff...
				fclose (filein);
				break;
			}//if file exists
		}//default	
	}//switch		

	this->redraw();	
	
}

void MeshTools::create_landmarks(int landmark_mode, vtkFloatArray* param_list, int type){

	OBJECT_LANDMARK * My_Landmark;
	float m_ve[3], m_ven[3], leng;
	//char param1[50];

	m_ve[0] = param_list->GetTuple1(0); m_ve[1] = param_list->GetTuple1(1); m_ve[2] = param_list->GetTuple1(2);
	if (type == 1)
	{
		m_ven[0] = param_list->GetTuple1(3) - param_list->GetTuple1(0); 
		m_ven[1] = param_list->GetTuple1(4) - param_list->GetTuple1(1);
		m_ven[2] = param_list->GetTuple1(5) - param_list->GetTuple1(2);
	}
	else
	{
		m_ven[0] = 1.0; m_ven[1] = 0.0; m_ven[2] = 0.0;
	}
	//std::cout << "Read line"  << param1 <<"  "<< param_list->GetTuple1(0)<<"  " <<param_list->GetTuple1(1)<<"  "
	//	<<param_list->GetTuple1(2) <<"  "<<param_list->GetTuple1(3)<<"  " <<param_list->GetTuple1(4)<<"  " <<param_list->GetTuple1(5) <<std::endl;

	leng = sqrt(m_ven[0] * m_ven[0] + m_ven[1] * m_ven[1] + m_ven[2] * m_ven[2]);
	if (leng != 0)
	{
		m_ven[0] /= leng;
		m_ven[1] /= leng;
		m_ven[2] /= leng;
	}
	//std::cout << "leng: "  << leng << std::endl;
	//Cont_Mesh.Mesh_container_newlandmark(landmark_size);	

	My_Landmark = new OBJECT_LANDMARK;
	//std::cout << "New object created: "  << leng << std::endl;

	glMatrix M1, M2;
	glPushMatrix();
	glLoadIdentity();
	//getMatrix((GLfloat*) M1);
	getmatrix(M2);
	getmatrix(M1);
	glPopMatrix();
	if (landmark_mode == 0)
	{
		M1[0][0] = M1[1][1] = M1[2][2] = g_landmark_size;
	}
	else
	{
		M1[0][0] = M1[1][1] = M1[2][2] = 1.5*g_landmark_size;
	}

	// translation 
	M2[3][0] = m_ve[0]; M2[3][1] = m_ve[1]; M2[3][2] = m_ve[2];

	// rotation 
	// Landmark object oriented along x axis : 0 0 1 
	// assume rot.  
	// 1) around z 
	// 2)    around x 
	// -> values sinz,cosz,sinx,cosx need to be computed

	float cosz = m_ven[0]; // nx
	float sinz = sqrt(1.0 - (cosz*cosz));
	float cosx, sinx;
	if (sinz == 0.0) // just in case we are in the speciale case
	{
		cosx = 1.0; sinx = 0.0;
	}
	else
	{
		cosx = m_ven[1] / sinz;

		sinx = m_ven[2] / sinz;
	}

	M2[0][0] = cosz; //=nx
	M2[0][1] = cosx*sinz; //=ny
	M2[0][2] = sinx *sinz;    //=nz

	M2[1][0] = -sinz;
	M2[1][1] = cosx*cosz;
	M2[1][2] = sinx*cosz;

	M2[2][0] = 0;
	M2[2][1] = -sinx;
	M2[2][2] = cosx;


	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++){
			My_Landmark->Mat2[i][j] = M2[i][j];

			if (i == j){
				My_Landmark->Mat1[i][i] = M1[i][i];
			}
		}

	if (landmark_mode == 0){
		My_Landmark->color[0] = 1;
		My_Landmark->color[1] = 0;
		My_Landmark->color[2] = 0;
		My_Landmark->color[3] = 1;
	}
	else
	{
		My_Landmark->color[0] = 1.0;
		My_Landmark->color[1] = 0.7;
		My_Landmark->color[2] = 0.2;
		My_Landmark->color[3] = 0.5;
	}
	//std::cout << "In TestPointNormals: "  << std::endl;
	Cont_Mesh.Add_Landmark(My_Landmark, landmark_mode);
	//std::cout << "Landmark well added: " << std::endl;
}


void MeshTools::Open_flags()
{
	
	float m_ve[3], m_ven[3], leng;
	
	float  param1, param2, param3, param4,param5, param6, param7, param8, param9, param10;
	FILE	*filein;									// Filename To Open
	OBJECT_LANDMARK * My_Landmark;
	char	oneline[1000];
	
	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);				
	fnfc.title("Load flags");				
	fnfc.filter("FLG\t*.{flg,FLG}");		
	switch ( fnfc.show() ) 
	{
		case -1: 
		{
			//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
			break;  // ERROR
		}
		case  1: 
		{
			//std::cout<<"CANCEL\n";     
			break;  // CANCEL
		 }
		default: 
		{						
			std::string filename = this->correct_filename(fnfc.filename());	
			int file_exists=1;
			ifstream file(filename.c_str());		
			if (file)
			{
				//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
				file.close();
			}
			else
			{
				
				std::cout<<"file:"<<filename.c_str()<<" does not exists."<<std::endl;			
				file_exists =0;
			}
			
			if (file_exists==1)
			{
				filein =fopen(filename.c_str(), "r");	
				readstr(filein,oneline);
								
				feof(filein);
				std::cout << "Try open flag file "  << std::endl;
				int cpt=1;
				std::string slabel;

				while (!feof(filein)) 
				{		
					//std::cout << "Try sscanf_s "  <<oneline<< std::endl;
					//sscanf(oneline, "%s%f %f %f %f %f %f\n", param1, &param2, &param3, &param4, &param5, &param6, &param7, _countof(param1));			
					if (cpt%2 ==0)
					{
						sscanf(oneline, "%f %f %f %f %f %f %f %f %f %f\n", &param1, &param2, &param3, &param4, &param5, 
							&param6, &param7, &param8, &param9, &param10
							);			
						float length = 25;
						float r,g,b;
						r=0.5;
						g=0.5;
						b=0.5;
						if (param7> 0 && param7<500000){length = param7;}
						if (param8>= 0 && param8<=1.0){r = param8;}
						if (param9>= 0 && param9<=1.0){g = param9;}
						if (param10>= 0 && param10<=1.0){b = param10;}
						//std::cout<<"Param8="<<param8<<std::endl;
						//std::cout<<"Param9="<<param9<<std::endl;
						//std::cout<<"Param10="<<param10<<std::endl;
						m_ve[0] = param1;m_ve[1] = param2;m_ve[2] = param3;				
						m_ven[0] = param4-param1;m_ven[1] = param5-param2;m_ven[2] = param6-param3;
						//std::cout << "Read line"  << param1 <<"  "<< param2<<"  " <<param3<<"  "
						//	<<param4 <<"  "<<param5<<"  " <<param6<<"  " <<param7 <<std::endl;
					
						leng = sqrt(m_ven[0]*m_ven[0] + m_ven[1]*m_ven[1] + m_ven[2]*m_ven[2]);
						if (leng != 0)
							{
							m_ven[0]/=leng;
							m_ven[1]/=leng;
							m_ven[2]/=leng;
							}
						//std::cout << "leng: "  << leng << std::endl;
						//Cont_Mesh.Mesh_container_newlandmark(landmark_size);	

						My_Landmark = new OBJECT_LANDMARK;
						//std::cout << "New object created: "  << leng << std::endl;
						My_Landmark->is_flag=1;
						My_Landmark->flag_length=length;
						My_Landmark->color[0]=r;
						My_Landmark->color[1]=g;
						My_Landmark->color[2]=b;
						My_Landmark->color[3]=1;

						glMatrix M1,M2;
						glPushMatrix();
						glLoadIdentity();
						//getMatrix((GLfloat*) M1);
						getmatrix(M2);
						getmatrix(M1);
						glPopMatrix();
						
						M1[0][0]=M1[1][1]=M1[2][2]=g_landmark_size;			
						
						// translation 
 						M2[3][0]=m_ve[0];M2[3][1]=m_ve[1];M2[3][2]=m_ve[2];
			         
					  // rotation 
					  // Landmark object oriented along x axis : 0 0 1 
					  // assume rot.  
						// 1) around z 
						// 2)    around x 
					  // -> values sinz,cosz,sinx,cosx need to be computed
					  
					  float cosz=m_ven[0]; // nx
					  float sinz = sqrt(1.0-(cosz*cosz));
					  float cosx,sinx;
					  if(sinz==0.0) // just in case we are in the speciale case
					  {
						cosx=1.0;sinx=0.0;	
					  }
					  else
					  {
						cosx= m_ven[1]/sinz;
						
						sinx=m_ven[2]/sinz;
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
						
						My_Landmark->Mat1[0][0]=M1[0][0];
						My_Landmark->Mat1[1][1]=M1[1][1];
						My_Landmark->Mat1[2][2]=M1[2][2];

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
						//My_Landmark->color[0] = 1;
						//My_Landmark->color[1] = 1;
						//My_Landmark->color[2] = 1;
						//My_Landmark->color[3] = 1;
						
						
						Cont_Mesh.Add_Landmark(My_Landmark, 2);
						//std::string slabel = label;
						My_Landmark->flag_label = slabel.c_str();

					}
					else
					{
						slabel =oneline;
						slabel = slabel.substr(0, (slabel.length()-1));
						//sscanf(oneline, "%s\n", label);
						//std::cout<<"oneline:"<<oneline<<std::endl;
						//std::cout<<"label:"<<label<<std::endl;
					}
					cpt++;
					//std::cout << "Landmark well added: " << std::endl;
					readstr(filein,oneline); //read next line
						
				}//While scanff...
				fclose (filein);
			}//if file exists
			break;
		}//default
	
	}//switch
this->redraw();
	
	
}

void MeshTools::Open_Tags()
{
	
	float  param1, param2, param3, param4;
	FILE	*filein;									// Filename To Open
	char	oneline[1000];	
	//char *sFile = fl_file_chooser("Load tags", "*.{tag,TAG}", "");
	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);				
	fnfc.title("Load tags");				
	fnfc.filter("Tags\t*.{tag,TAG}");		
	switch ( fnfc.show() ) 
	{
		case -1: 
		{
			//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
			break;  // ERROR
		}
		case  1: 
		{
			//std::cout<<"CANCEL\n";     
			break;  // CANCEL
		 }
		default: 
		{						
			std::string filename = this->correct_filename(fnfc.filename());
			int file_exists=1;
			ifstream file(filename.c_str());		
			if (file)
			{
				//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
				file.close();
			}
			else
			{
				
				std::cout<<"file:"<<filename.c_str()<<" does not exists."<<std::endl;			
				file_exists =0;
			}
			
			if (file_exists==1)
			{
				filein =fopen(filename.c_str(), "r");	
				readstr(filein,oneline);
				//std::cout << "Try open Tag file "  << std::endl;
				//std::cout << "feof(filein)"  <<feof(filein)<< std::endl;
				int cpt=1;
				int ic=0;
				int il=0;
				while (!feof(filein) && ic<25 && il<25) 
				{		
					if (cpt%2 ==0)
					{
					
						sscanf(oneline, "%f %f %f %f\n", &param1, &param2, &param3, &param4);
						g_tag_colors[ic][0]= param1;
						g_tag_colors[ic][1]= param2;
						g_tag_colors[ic][2]= param3;
						g_tag_colors[ic][3]= param4;
						ic++;
					}
					else
					{
						//sscanf(oneline, "%s\n", label);
						std::string slabel = oneline;

						slabel = slabel.substr(0, (slabel.length()-1));

						g_tag_labels[il]= slabel.c_str();
						il++;
					}	
					cpt++;
					readstr(filein,oneline); //read next line					
				}//While scanff...
				fclose (filein);	
			}//if file exists
			break;
		}//default	
	}//switch	
	this->redraw();
	
}

void MeshTools::Open_Orientation()
{
	
	float  param1, param2, param3, param4;
	FILE	*filein;									// Filename To Open
	char	oneline[1000];	
	//char *sFile = fl_file_chooser("Load tags", "*.{tag,TAG}", "");
	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);				
	fnfc.title("Load orientation labels");				
	fnfc.filter("Orientation\t*.{ori,ORI}");		
	switch ( fnfc.show() ) 
	{
		case -1: 
		{
			//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
			break;  // ERROR
		}
		case  1: 
		{
			//std::cout<<"CANCEL\n";     
			break;  // CANCEL
		 }
		default: 
		{						
			std::string filename = this->correct_filename(fnfc.filename());
			int file_exists=1;
			ifstream file(filename.c_str());		
			if (file)
			{
				//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
				file.close();
			}
			else
			{
				
				std::cout<<"file:"<<filename.c_str()<<" does not exists."<<std::endl;			
				file_exists =0;
			}
			
			if (file_exists==1)
			{
				filein =fopen(filename.c_str(), "r");	
				readstr(filein,oneline);
				//std::cout << "Try open Tag file "  << std::endl;
				//std::cout << "feof(filein)"  <<feof(filein)<< std::endl;
				int cpt=1;
				int ic=0;
			
				while (!feof(filein) && ic<6) 
				{		
					
						//sscanf(oneline, "%s\n", label);
						std::string slabel = oneline;

						slabel = slabel.substr(0, (slabel.length()-1));

						g_orientation_labels[ic]= slabel.c_str();
						ic++;
						
					cpt++;
					readstr(filein,oneline); //read next line					
				}//While scanff...
				fclose (filein);	
			}//if file exists
			break;
		}//default	
	}//switch	
	this->redraw();	
	
}
void MeshTools::Open_CUR_File()
{
	
	float  param2, param3, param4, param5, param6, param7;
	int param8;
	float m_ved[3], m_veh[3],m_ven[3], leng; //ved is first landmarks // veh is handles
	char param1[10];
	FILE	*filein;									// Filename To Open
	OBJECT_LANDMARK * My_LandmarkD;
	OBJECT_LANDMARK * My_LandmarkH;
	char	oneline[255];
	int start;

	//char *szFile = fl_file_chooser("Load CUR file", "*.CUR*", "");
	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);				
	fnfc.title("Load Curves");				
	fnfc.filter("Curves\t*.{cur,CUR}");		
	switch ( fnfc.show() ) 
	{
		case -1: 
		{
			//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
			break;  // ERROR
		}
		case  1: 
		{
			//std::cout<<"CANCEL\n";     
			break;  // CANCEL
		 }
		default: 
		{						
			std::string filename = this->correct_filename(fnfc.filename());
		
			int file_exists=1;
			ifstream file(filename.c_str());		
			if (file)
			{
				//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
				file.close();
			}
			else
			{
				
				std::cout<<"file:"<<filename.c_str()<<" does not exists."<<std::endl;			
				file_exists =0;
			}
			
			if (file_exists==1)
			{
		
				//filein = fopen(szFile, "rt");
				filein =fopen(filename.c_str(), "rt");	
				readstr(filein,oneline);
				while (!feof(filein)) 
				{		
					//sscanf_s(oneline, "%s %f %f %f %f %f %f %d\n", &param1, &param2, &param3, &param4, &param5, &param6, &param7, &param8);
					sscanf(oneline, "%s %f %f %f %f %f %f %d\n", &param1, &param2, &param3, &param4, &param5, &param6, &param7, &param8);
					m_ved[0] = param2;m_ved[1] = param3;m_ved[2] = param4;
					m_veh[0] = param5;m_veh[1] = param6;m_veh[2] = param7;
					start=param8;
					//if (start !=1){start=0;}
					m_ven[0]=0;m_ven[1]=0;m_ven[2]=1;
					leng = sqrt(m_ven[0]*m_ven[0] + m_ven[1]*m_ven[1] + m_ven[2]*m_ven[2]);
					if (leng != 0)
						{
						m_ven[0]/=leng;
						m_ven[1]/=leng;
						m_ven[2]/=leng;
						}
					//m_ven[0] = param5;m_ven[1] = param6;m_ven[2] = param7;
					My_LandmarkD = new OBJECT_LANDMARK;
					My_LandmarkH = new OBJECT_LANDMARK;
					glMatrix M1,M2;
					glPushMatrix();
					glLoadIdentity();
					//getMatrix((GLfloat*) M1);
					getmatrix(M2);
					getmatrix(M1);
					glPopMatrix();
					M1[0][0]=M1[1][1]=M1[2][2]=g_landmark_size;
					// transl. components:
					M2[3][0]=m_ved[0];M2[3][1]=m_ved[1];M2[3][2]=m_ved[2];
					// rot. components:
					// assume rot. about x and z axes:
					// -> values sinx,cosx,sinz,cosz
					float cosx=m_ven[2];
					float sinx = sqrt(1.0-(cosx*cosx));
					float cosz,sinz;
					if(sinx==0.0)
					{
						cosz=1.0;sinz=0.0;	// no rotation around z
					}
					else
					{
						cosz=-m_ven[1]/sinx;
						sinz=m_ven[0]/sinx;
					}
					M2[0][0]=cosz;M2[0][1]=sinz;M2[0][2]=0;
					M2[1][0]=-cosx*sinz;M2[1][1]=cosx*cosz;M2[1][2]=sinx;
					M2[2][0]=m_ven[0];M2[2][1]=m_ven[1];M2[2][2]=m_ven[2];
					My_LandmarkD->curve_start=start; 
					My_LandmarkD->Mat1[0][0]=M1[0][0];
					My_LandmarkD->Mat1[1][1]=M1[1][1];
					My_LandmarkD->Mat1[2][2]=M1[2][2];

					My_LandmarkD->Mat2[0][0]=M2[0][0];
					My_LandmarkD->Mat2[0][1]=M2[0][1];
					My_LandmarkD->Mat2[0][2]=M2[0][2];
					My_LandmarkD->Mat2[0][3]=M2[0][3];
					My_LandmarkD->Mat2[1][0]=M2[1][0];
					My_LandmarkD->Mat2[1][1]=M2[1][1];
					My_LandmarkD->Mat2[1][2]=M2[1][2];
					My_LandmarkD->Mat2[1][3]=M2[1][3];
					My_LandmarkD->Mat2[2][0]=M2[2][0];
					My_LandmarkD->Mat2[2][1]=M2[2][1];
					My_LandmarkD->Mat2[2][2]=M2[2][2];
					My_LandmarkD->Mat2[2][3]=M2[2][3];
					My_LandmarkD->Mat2[3][0]=M2[3][0];
					My_LandmarkD->Mat2[3][1]=M2[3][1];
					My_LandmarkD->Mat2[3][2]=M2[3][2];
					My_LandmarkD->Mat2[3][3]=M2[3][3];
					
					
					if (start ==0)
					{
						My_LandmarkD->color[0] = 1;
						My_LandmarkD->color[1] = 0;
						My_LandmarkD->color[2] = 0;
						My_LandmarkD->color[3] = 1;
					}
					else if (start==1)
					{
						My_LandmarkD->color[0] = 0;
						My_LandmarkD->color[1] = 1;
						My_LandmarkD->color[2] = 0;
						My_LandmarkD->color[3] = 1;
					}
					else if (start ==2)
					{
						My_LandmarkD->color[0] = 0;
						My_LandmarkD->color[1] = 0;
						My_LandmarkD->color[2] = 1;
						My_LandmarkD->color[3] = 1;
					}
					else if (start ==3)
					{
						My_LandmarkD->color[0] = 0.6;
						My_LandmarkD->color[1] = 0.2;
						My_LandmarkD->color[2] = 0.5;
						My_LandmarkD->color[3] = 1;
					}

					M2[3][0]=m_veh[0];M2[3][1]=m_veh[1];M2[3][2]=m_veh[2];
					My_LandmarkH->Mat1[0][0]=1.5*M1[0][0];
					My_LandmarkH->Mat1[1][1]=1.5*M1[1][1];
					My_LandmarkH->Mat1[2][2]=1.5*M1[2][2];

					My_LandmarkH->Mat2[0][0]=M2[0][0];
					My_LandmarkH->Mat2[0][1]=M2[0][1];
					My_LandmarkH->Mat2[0][2]=M2[0][2];
					My_LandmarkH->Mat2[0][3]=M2[0][3];
					My_LandmarkH->Mat2[1][0]=M2[1][0];
					My_LandmarkH->Mat2[1][1]=M2[1][1];
					My_LandmarkH->Mat2[1][2]=M2[1][2];
					My_LandmarkH->Mat2[1][3]=M2[1][3];
					My_LandmarkH->Mat2[2][0]=M2[2][0];
					My_LandmarkH->Mat2[2][1]=M2[2][1];
					My_LandmarkH->Mat2[2][2]=M2[2][2];
					My_LandmarkH->Mat2[2][3]=M2[2][3];
					My_LandmarkH->Mat2[3][0]=M2[3][0];
					My_LandmarkH->Mat2[3][1]=M2[3][1];
					My_LandmarkH->Mat2[3][2]=M2[3][2];
					My_LandmarkH->Mat2[3][3]=M2[3][3];
					

					My_LandmarkH->color[0] = 1.0;
					My_LandmarkH->color[1] = 0.7;
					My_LandmarkH->color[2] = 0.2;
					My_LandmarkH->color[3] = 0.5;
					
					
					Cont_Mesh.Add_Landmark(My_LandmarkD, 0);
					Cont_Mesh.Add_Landmark(My_LandmarkH, 1);
					readstr(filein,oneline); //read next line
						
				}//While scanff...		
				fclose(filein);		
			}//if file exists
			break;
		}//default	
	}//switch	
	this->redraw();	
	
}

void MeshTools::TestPointNormals(vtkPolyData* polydata)
{
  std::cout << "In TestPointNormals: " << polydata->GetNumberOfPoints() << std::endl;
  // Try to read normals directly
  bool hasPointNormals = GetPointNormals(polydata);
 
  if(!hasPointNormals)
    {
    std::cout << "No point normals were found. Computing normals..." << std::endl;
 
    // Generate normals
    vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
#if VTK_MAJOR_VERSION <= 5
    normalGenerator->SetInput(polydata);
#else
    normalGenerator->SetInputData(polydata);
#endif
    normalGenerator->ComputePointNormalsOn();
    normalGenerator->ComputeCellNormalsOff();
    normalGenerator->Update();
    /*
    // Optional settings
    normalGenerator->SetFeatureAngle(0.1);
    normalGenerator->SetSplitting(1);
    normalGenerator->SetConsistency(0);
    normalGenerator->SetAutoOrientNormals(0);
    normalGenerator->SetComputePointNormals(1);
    normalGenerator->SetComputeCellNormals(0);
    normalGenerator->SetFlipNormals(0);
    normalGenerator->SetNonManifoldTraversal(1);
    */
 
    polydata = normalGenerator->GetOutput();
 
    // Try to read normals again
    hasPointNormals = GetPointNormals(polydata);
 
    std::cout << "On the second try, has point normals? " << hasPointNormals << std::endl;
 
    }
  else
    {
    std::cout << "Point normals were found!" << std::endl;
    }
}
 
void MeshTools::TestCellNormals(vtkPolyData* polydata)
{
  // Try to read normals directly
  bool hasCellNormals = GetCellNormals(polydata);
 
  if(!hasCellNormals)
    {
    std::cout << "No cell normals were found. Computing normals..." << std::endl;
 
    // Generate normals
    vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
#if VTK_MAJOR_VERSION <= 5
    normalGenerator->SetInput(polydata);
#else
    normalGenerator->SetInputData(polydata);
#endif
    normalGenerator->ComputePointNormalsOff();
    normalGenerator->ComputeCellNormalsOn();
    normalGenerator->Update();
    /*
    // Optional settings
    normalGenerator->SetFeatureAngle(0.1);
    normalGenerator->SetSplitting(1);
    normalGenerator->SetConsistency(0);
    normalGenerator->SetAutoOrientNormals(0);
    normalGenerator->SetComputePointNormals(1);
    normalGenerator->SetComputeCellNormals(0);
    normalGenerator->SetFlipNormals(0);
    normalGenerator->SetNonManifoldTraversal(1);
    */
 
    polydata = normalGenerator->GetOutput();
 
    // Try to read normals again
    hasCellNormals = GetCellNormals(polydata);
 
    std::cout << "On the second try, has cell normals? " << hasCellNormals << std::endl;
 
    }
  else
    {
    std::cout << "Cell normals were found!" << std::endl;
    }
}


bool MeshTools::GetPointNormals(vtkPolyData* polydata)
{
  std::cout << "In GetPointNormals: " << polydata->GetNumberOfPoints() << std::endl;
  std::cout << "Looking for point normals..." << std::endl;
 
  // Count points
  vtkIdType numPoints = polydata->GetNumberOfPoints();
  std::cout << "There are " << numPoints << " points." << std::endl;
 
  // Count triangles
  vtkIdType numPolys = polydata->GetNumberOfPolys();
  std::cout << "There are " << numPolys << " polys." << std::endl;
 
  ////////////////////////////////////////////////////////////////
  // Double normals in an array
  vtkDoubleArray* normalDataDouble =
    vtkDoubleArray::SafeDownCast(polydata->GetPointData()->GetArray("Normals"));
 
  if(normalDataDouble)
    {
    int nc = normalDataDouble->GetNumberOfTuples();
    std::cout << "There are " << nc
            << " components in normalDataDouble" << std::endl;
    return true;
    }
 
  ////////////////////////////////////////////////////////////////
  // Float normals in an array
  vtkFloatArray* normalDataFloat =
    vtkFloatArray::SafeDownCast(polydata->GetPointData()->GetArray("Normals"));
 
  if(normalDataFloat)
    {
    int nc = normalDataFloat->GetNumberOfTuples();
    std::cout << "There are " << nc
            << " components in normalDataFloat" << std::endl;
    return true;
    }
 
  ////////////////////////////////////////////////////////////////
  // Point normals
  vtkDoubleArray* normalsDouble =
    vtkDoubleArray::SafeDownCast(polydata->GetPointData()->GetNormals());
 
  if(normalsDouble)
    {
    std::cout << "There are " << normalsDouble->GetNumberOfComponents()
              << " components in normalsDouble" << std::endl;
    return true;
    }
 
  ////////////////////////////////////////////////////////////////
  // Point normals
  vtkFloatArray* normalsFloat =
    vtkFloatArray::SafeDownCast(polydata->GetPointData()->GetNormals());
 
  if(normalsFloat)
    {
    std::cout << "There are " << normalsFloat->GetNumberOfComponents()
              << " components in normalsFloat" << std::endl;
    return true;
    }
 
  /////////////////////////////////////////////////////////////////////
  // Generic type point normals
  vtkDataArray* normalsGeneric = polydata->GetPointData()->GetNormals(); //works
  if(normalsGeneric)
    {
    std::cout << "There are " << normalsGeneric->GetNumberOfTuples()
              << " normals in normalsGeneric" << std::endl;
 
    double testDouble[3];
    normalsGeneric->GetTuple(0, testDouble);
 
    std::cout << "Double: " << testDouble[0] << " "
              << testDouble[1] << " " << testDouble[2] << std::endl;
 
    // Can't do this:
    /*
    float testFloat[3];
    normalsGeneric->GetTuple(0, testFloat);
 
    std::cout << "Float: " << testFloat[0] << " "
              << testFloat[1] << " " << testFloat[2] << std::endl;
    */
    return true;
    }
 
 
  // If the function has not yet quit, there were none of these types of normals
  std::cout << "Normals not found!" << std::endl;
  return false;
 
}

bool MeshTools::GetCellNormals(vtkPolyData* polydata)
{
  std::cout << "Looking for cell normals..." << std::endl;
 
  // Count points
  vtkIdType numCells = polydata->GetNumberOfCells();
  std::cout << "There are " << numCells << " cells." << std::endl;
 
  // Count triangles
  vtkIdType numPolys = polydata->GetNumberOfPolys();
  std::cout << "There are " << numPolys << " polys." << std::endl;
 
  ////////////////////////////////////////////////////////////////
  // Double normals in an array
  vtkDoubleArray* normalDataDouble =
    vtkDoubleArray::SafeDownCast(polydata->GetCellData()->GetArray("Normals"));
 
  if(normalDataDouble)
    {
    int nc = normalDataDouble->GetNumberOfTuples();
    std::cout << "There are " << nc
            << " components in normalDataDouble" << std::endl;
    return true;
    }
 
  ////////////////////////////////////////////////////////////////
  // Double normals in an array
  vtkFloatArray* normalDataFloat =
    vtkFloatArray::SafeDownCast(polydata->GetCellData()->GetArray("Normals"));
 
  if(normalDataFloat)
    {
    int nc = normalDataFloat->GetNumberOfTuples();
    std::cout << "There are " << nc
            << " components in normalDataFloat" << std::endl;
    return true;
    }
 
  ////////////////////////////////////////////////////////////////
  // Point normals
  vtkDoubleArray* normalsDouble =
    vtkDoubleArray::SafeDownCast(polydata->GetCellData()->GetNormals());
 
  if(normalsDouble)
    {
    std::cout << "There are " << normalsDouble->GetNumberOfComponents()
              << " components in normalsDouble" << std::endl;
    return true;
    }
 
  ////////////////////////////////////////////////////////////////
  // Point normals
  vtkFloatArray* normalsFloat =
    vtkFloatArray::SafeDownCast(polydata->GetCellData()->GetNormals());
 
  if(normalsFloat)
    {
    std::cout << "There are " << normalsFloat->GetNumberOfComponents()
              << " components in normalsFloat" << std::endl;
    return true;
    }
 
  /////////////////////////////////////////////////////////////////////
  // Generic type point normals
  vtkDataArray* normalsGeneric = polydata->GetCellData()->GetNormals(); //works
  if(normalsGeneric)
    {
    std::cout << "There are " << normalsGeneric->GetNumberOfTuples()
              << " normals in normalsGeneric" << std::endl;
 
    double testDouble[3];
    normalsGeneric->GetTuple(0, testDouble);
 
    std::cout << "Double: " << testDouble[0] << " "
              << testDouble[1] << " " << testDouble[2] << std::endl;
 
    // Can't do this:
    /*
    float testFloat[3];
    normalsGeneric->GetTuple(0, testFloat);
 
    std::cout << "Float: " << testFloat[0] << " "
              << testFloat[1] << " " << testFloat[2] << std::endl;
    */
    return true;
    }
 
 
  // If the function has not yet quit, there were none of these types of normals
  std::cout << "Normals not found!" << std::endl;
  return false;
 
}
std::string MeshTools::correct_filename(std::string init_filename)
{
// issue introduced with FLTK 1.3.2 : file path are encoded in UTF8
// => This is a problem with paths containing accents
std::string final_filename;

#if defined (WIN32) && !defined(__CYGWIN__)
	   char *correct_filename = NULL;
	   unsigned l = (unsigned) strlen(init_filename.c_str());
	   unsigned wn = fl_utf8toa(init_filename.c_str(), l, NULL, 0) + 1; // Query length
	   correct_filename= (char*)realloc(correct_filename, sizeof(char)*wn);
	   wn = fl_utf8toa(init_filename.c_str(), l, correct_filename, wn);
	   correct_filename[wn] = 0;	  	  
	   final_filename = correct_filename;		
	   return final_filename;

#else
	final_filename=init_filename.c_str();
	return init_filename;
#endif
}


//Original function saved
void MeshTools::Open_Mesh_File() 
{
	
	
	
	OBJECT_MESH *My_Obj;
	std::string filename;
	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);				
	fnfc.title("Open Mesh");		
	fnfc.filter("Mesh\t*.{stl,STL,vtk,VTK,obj,OBJ,ply,PLY}");		
		

	switch ( fnfc.show() ) 
	{
			case -1: 
				{
					//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
					break;  // ERROR
				}
			case  1: {
				//std::cout<<"CANCEL\n";     
				break;  // CANCEL
					 }
			default: 
				{
					//std::cout<<"PICKED: "<<fnfc.filename()<<"\n";   // FILE CHOSEN					
					filename = this->correct_filename(fnfc.filename());
					int file_exists=1;
					ifstream file(filename.c_str());		
					if (file)
					{
						//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
						file.close();
					}
					else
					{
						
						std::cout<<"file:"<<filename.c_str()<<" does not exists."<<std::endl;			
						file_exists =0;
					}
					//file_exists=1;
					if (file_exists==1)
					{
						std::string STLext (".stl");
						std::string STLext2 (".STL");
						std::string VTKext (".vtk");
						std::string VTKext2 (".VTK");
						std::string OBJext (".obj");
						std::string OBJext2 (".OBJ");
						std::string PLYext (".ply");
						std::string PLYext2 (".PLY");
						
						int type =0; //0 = stl, 1 = vtk, 2 = obj, 3 = ply
						std::size_t found = filename.find(STLext);
						std::size_t found2 = filename.find(STLext2);
						if (found != std::string::npos || found2 != std::string::npos)
						{type=0; 
							//STL
						}

						//std::cout << "0Type= " <<type<< std::endl;
						found = filename.find(VTKext);
						found2 = filename.find(VTKext2);
						if (found != std::string::npos || found2 != std::string::npos)
						{type=1; //VTK
						}
						//std::cout << "1Type= " <<type<< std::endl;
						found = filename.find(OBJext);
						found2 = filename.find(OBJext2);
						if (found != std::string::npos || found2 != std::string::npos)
						{type=2; //OBJ
						}
						//std::cout << "2Type= " <<type<< std::endl;
						found = filename.find(PLYext);
						found2 = filename.find(PLYext2);
						if (found != std::string::npos || found2 != std::string::npos)
						{type=3; //PLY
						}
				
						 // Read and display for verification
						
						vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();
						
						if (type ==0)
						{

							vtkSmartPointer<vtkSTLReader> reader =
							vtkSmartPointer<vtkSTLReader>::New();
							
							reader->SetFileName(filename.c_str());
							reader->Update();			
							MyObj= reader->GetOutput();
						}

						else if (type ==1)
						{

							vtkSmartPointer<vtkPolyDataReader> reader =
							vtkSmartPointer<vtkPolyDataReader>::New();						
							reader->SetFileName(filename.c_str());
							reader->Update();			
							MyObj= reader->GetOutput();
						}
						else if (type ==2)
						{

							vtkSmartPointer<vtkOBJReader> reader =
							vtkSmartPointer<vtkOBJReader>::New();
							reader->SetFileName(filename.c_str());
							reader->Update();			
							MyObj= reader->GetOutput();
						}
						else
						{

							vtkSmartPointer<vtkPLYReader> reader =
							vtkSmartPointer<vtkPLYReader>::New();
							reader->SetFileName(filename.c_str());
							reader->Update();			
							MyObj= reader->GetOutput();
						}
						std::cout << "\nNumber of points 1:"<<MyObj->GetNumberOfPoints()<< std::endl;
						std::cout << "\nNumber of cells 1:"<<MyObj->GetNumberOfCells()<< std::endl;		


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

						MyObj= (OBJECT_MESH*)cleanPolyDataFilter->GetOutput();
						
						std::cout << "\nNumber of points:"<<MyObj->GetNumberOfPoints()<< std::endl;
						std::cout << "\nNumber of cells:"<<MyObj->GetNumberOfCells()<< std::endl;		
						
						
						
						vtkFloatArray* norms = vtkFloatArray::SafeDownCast
							(MyObj->GetCellData()->GetNormals());
								std::cout << "Safe cell downcast done ! " << std::endl;
						if(norms)
						{

								std::cout << "There are here " << norms->GetNumberOfTuples()
							  << " Float Cell normals in norms" << std::endl;
						}
						else
						{
								std::cout << "FloatNorms CELL is null " << std::endl;
						}

						norms = vtkFloatArray::SafeDownCast
							(MyObj->GetPointData()->GetNormals());
								std::cout << "Safe point downcast done ! " << std::endl;
						if(norms)
						{

								std::cout << "There are  " << norms->GetNumberOfTuples()
							  << " Float POINT normals in norms" << std::endl;
						}
						else
						{
								std::cout << "FloatNorms POINTS is null " << std::endl;
						}

						if (MyObj->GetNumberOfPoints()>10)
						{


							std::string newname=fl_filename_name(filename.c_str());

							int nPos = newname.find_first_of(".");			
							if ( nPos > -1 )
							{
								newname = newname.substr(0, nPos);
							}
							CheckingName(&newname);
							My_Obj = Cont_Mesh.Mesh_PDcontainerload(MyObj, (char*)newname.c_str());
							My_Obj->Set_Active_Scalar();
							int numpoints = My_Obj->GetNumberOfPoints();
							int numtriangles = My_Obj->GetNumberOfCells();
							std::cout << "Number of points:"<<numpoints<< std::endl;
							std::cout << "Number of cells:"<<numtriangles<< std::endl;

							//std::cout << "2 Mean x:"<<My_Obj->mean[0]<< "Mean y:"<<My_Obj->mean[1]<< "Mean z:"<<My_Obj->mean[2]<< std::endl;
							
							//std::cout << "3 Mean x:"<<My_Obj->mean[0]<< "Mean y:"<<My_Obj->mean[1]<< "Mean z:"<<My_Obj->mean[2]<< std::endl;
						
							My_Obj->selected = 1;		
							
							
							cout << "color init: ";
							vtkSmartPointer<vtkUnsignedCharArray> newcolors =
								vtkSmartPointer<vtkUnsignedCharArray>::New();
							newcolors->SetNumberOfComponents(4);
							newcolors->SetNumberOfTuples(numpoints);
							//ici init_RGB ou RGB_i
							if ((vtkUnsignedCharArray*)MyObj->GetPointData()->GetScalars("RGB") != NULL){
								newcolors->DeepCopy((vtkUnsignedCharArray*)MyObj->GetPointData()->GetScalars("RGB"));
								
								for (int i = 0; i < numpoints; i++)
									newcolors->SetComponent(i, 3, 255.);
								
								newcolors->SetName("Init_RGB");
								My_Obj->GetPointData()->AddArray(newcolors);
							}
							cout << "ok."<<endl;

							My_Obj->color[0] = color_obj[0];
							My_Obj->color[1] = color_obj[1];
							My_Obj->color[2] = color_obj[2];
							My_Obj->color[3] = 1;

							My_Obj->bool_init_buf = 0;
							// Only update RGB if not exists!
							
							 vtkUnsignedCharArray* test = (vtkUnsignedCharArray*)My_Obj->GetPointData()->GetScalars("RGB");
							if (test ==NULL)
							{
								My_Obj->Update_RGB();
							}
							

							//std::cout << "4 Mean x:"<<My_Obj->mean[0]<< "Mean y:"<<My_Obj->mean[1]<< "Mean z:"<<My_Obj->mean[2]<< std::endl;
							
							
							//Move object at center of mass only in some cases
							if(g_move_cm==1)
							{
								My_Obj->Mat2[3][0]= -My_Obj->mean[0];
								My_Obj->Mat2[3][1]=-My_Obj->mean[1]; 
								My_Obj->Mat2[3][2]=-My_Obj->mean[2];
							}
							
							this->Compute_Global_Mean();
							this->Compute_Global_Scalar_List();
							
						}
						this->redraw();
					}//if file exists
					
					break;
				}
	}
		
	
		
}
/*
Hacked function to correct filenames!
void MeshTools::Open_Mesh_File() 
{
	
	
	
	OBJECT_MESH *My_Obj;
	std::string filename;
	char s2[200];
	char s[200];
	size_t length;
	int test;
	//Fl_Native_File_Chooser fnfc;
	//fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);				
	//fnfc.title("Open Mesh");		
	//fnfc.filter("Mesh\t*.{stl,STL,vtk,VTK,obj,OBJ,ply,PLY}");		
	char *szFile = fl_file_chooser("Load SURFACE", "*.{stl,STL,vtk,VTK,obj,OBJ,ply,PLY}", "");
char *szFile2=NULL;
filename = szFile;
#if defined (WIN32) && !defined(__CYGWIN__)
	   unsigned l = (unsigned) strlen(szFile);
	   unsigned wn = fl_utf8toa(szFile, l, NULL, 0) + 1; // Query length
	   szFile2 = (char*)realloc(szFile2, sizeof(char)*wn);
	   wn = fl_utf8toa(szFile, l, szFile2, wn);
	   szFile2[wn] = 0;
	   printf("szFile file %s\n", szFile);
	   std::cout<<"converted szFile file: "<< szFile2<<std::endl;
	   filename = szFile2;		
#endif

	//std::cout<<"fl_utf8locale(): "<<fl_utf8locale()<<"\n";   // 
	sprintf(s2, "%s", szFile);
	length=strlen(s2);
	test = strcmp(s2,"(null)");
	strncpy(s, s2, length-4);
	s[length-4]='\0';
	if (test != 0)
	{	
		
					//std::cout<<"PICKED: "<<szFile<<"\n";   // FILE CHOSEN					
					//filename = szFile;
					int file_exists=1;
					ifstream file(filename.c_str());		
					if (file)
					{
						std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
						file.close();
					}
					else
					{
						
						std::cout<<"file:"<<filename.c_str()<<" does not exists."<<std::endl;			
						file_exists =0;
					}
					//file_exists=1;
					if (file_exists==1)
					{
						std::string STLext (".stl");
						std::string STLext2 (".STL");
						std::string VTKext (".vtk");
						std::string VTKext2 (".VTK");
						std::string OBJext (".obj");
						std::string OBJext2 (".OBJ");
						std::string PLYext (".ply");
						std::string PLYext2 (".PLY");
						
						int type =0; //0 = stl, 1 = vtk, 2 = obj, 3 = ply
						std::size_t found = filename.find(STLext);
						std::size_t found2 = filename.find(STLext2);
						if (found != std::string::npos || found2 != std::string::npos)
						{type=0; 
							//STL
						}

						//std::cout << "0Type= " <<type<< std::endl;
						found = filename.find(VTKext);
						found2 = filename.find(VTKext2);
						if (found != std::string::npos || found2 != std::string::npos)
						{type=1; //VTK
						}
						//std::cout << "1Type= " <<type<< std::endl;
						found = filename.find(OBJext);
						found2 = filename.find(OBJext2);
						if (found != std::string::npos || found2 != std::string::npos)
						{type=2; //OBJ
						}
						//std::cout << "2Type= " <<type<< std::endl;
						found = filename.find(PLYext);
						found2 = filename.find(PLYext2);
						if (found != std::string::npos || found2 != std::string::npos)
						{type=3; //PLY
						}
				
						 // Read and display for verification
						
						vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();
						
						if (type ==0)
						{

							vtkSmartPointer<vtkSTLReader> reader =
							vtkSmartPointer<vtkSTLReader>::New();
							
							reader->SetFileName(filename.c_str());
							reader->Update();			
							MyObj= reader->GetOutput();
						}

						else if (type ==1)
						{

							vtkSmartPointer<vtkPolyDataReader> reader =
							vtkSmartPointer<vtkPolyDataReader>::New();						
							reader->SetFileName(filename.c_str());
							reader->Update();			
							MyObj= reader->GetOutput();
						}
						else if (type ==2)
						{

							vtkSmartPointer<vtkOBJReader> reader =
							vtkSmartPointer<vtkOBJReader>::New();
							reader->SetFileName(filename.c_str());
							reader->Update();			
							MyObj= reader->GetOutput();
						}
						else
						{

							vtkSmartPointer<vtkPLYReader> reader =
							vtkSmartPointer<vtkPLYReader>::New();
							reader->SetFileName(filename.c_str());
							reader->Update();			
							MyObj= reader->GetOutput();
						}
						std::cout << "\nNumber of points 1:"<<MyObj->GetNumberOfPoints()<< std::endl;
						std::cout << "\nNumber of cells 1:"<<MyObj->GetNumberOfCells()<< std::endl;		


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

						MyObj= (OBJECT_MESH*)cleanPolyDataFilter->GetOutput();
						
						std::cout << "\nNumber of points:"<<MyObj->GetNumberOfPoints()<< std::endl;
						std::cout << "\nNumber of cells:"<<MyObj->GetNumberOfCells()<< std::endl;		
						
						
						
						vtkFloatArray* norms = vtkFloatArray::SafeDownCast
							(MyObj->GetCellData()->GetNormals());
								std::cout << "Safe cell downcast done ! " << std::endl;
						if(norms)
						{

								std::cout << "There are here " << norms->GetNumberOfTuples()
							  << " Float Cell normals in norms" << std::endl;
						}
						else
						{
								std::cout << "FloatNorms CELL is null " << std::endl;
						}

						norms = vtkFloatArray::SafeDownCast
							(MyObj->GetPointData()->GetNormals());
								std::cout << "Safe point downcast done ! " << std::endl;
						if(norms)
						{

								std::cout << "There are  " << norms->GetNumberOfTuples()
							  << " Float POINT normals in norms" << std::endl;
						}
						else
						{
								std::cout << "FloatNorms POINTS is null " << std::endl;
						}

						if (MyObj->GetNumberOfPoints()>10)
						{


							std::string newname=fl_filename_name(filename.c_str());

							int nPos = newname.find_first_of(".");			
							if ( nPos > -1 )
							{
								newname = newname.substr(0, nPos);
							}
						
							My_Obj = Cont_Mesh.Mesh_PDcontainerload(MyObj, (char*)newname.c_str());
							My_Obj->Set_Active_Scalar();
							int numpoints = My_Obj->GetNumberOfPoints();
							int numtriangles = My_Obj->GetNumberOfCells();
							std::cout << "Number of points:"<<numpoints<< std::endl;
							std::cout << "Number of cells:"<<numtriangles<< std::endl;

							//std::cout << "2 Mean x:"<<My_Obj->mean[0]<< "Mean y:"<<My_Obj->mean[1]<< "Mean z:"<<My_Obj->mean[2]<< std::endl;
							
							//std::cout << "3 Mean x:"<<My_Obj->mean[0]<< "Mean y:"<<My_Obj->mean[1]<< "Mean z:"<<My_Obj->mean[2]<< std::endl;
						
							My_Obj->selected = 1;		
							
							
							

							My_Obj->color[0] = color_obj[0];
							My_Obj->color[1] = color_obj[1];
							My_Obj->color[2] = color_obj[2];
							My_Obj->color[3] = 1;
							// Only update RGB if not exists!
							
							 vtkUnsignedCharArray* test = (vtkUnsignedCharArray*)My_Obj->GetPointData()->GetScalars("RGB");
							if (test ==NULL)
							{
								My_Obj->Update_RGB();
							}
							

							//std::cout << "4 Mean x:"<<My_Obj->mean[0]<< "Mean y:"<<My_Obj->mean[1]<< "Mean z:"<<My_Obj->mean[2]<< std::endl;
							
							
							//Move object at center of mass only in some cases
							if(g_move_cm==1)
							{
								My_Obj->Mat2[3][0]= -My_Obj->mean[0];
								My_Obj->Mat2[3][1]=-My_Obj->mean[1]; 
								My_Obj->Mat2[3][2]=-My_Obj->mean[2];
							}
							
							this->Compute_Global_Mean();
							this->Compute_Global_Scalar_List();
							//this->SetMode(MODE_STL);
						}
						this->redraw();
					}//if file exists
					
					
				}
	
		
	//szFile = fl_file_chooser("Load SURFACE", "*.{stl,STL,vtk,VTK,obj,OBJ,ply,PLY}", "");	
		
}
*/

// Called from Save_NTW only!
void MeshTools::Save_Mesh_File(OBJECT_MESH *My_Obj, std::string filename, int file_type, int save_norms)
{
	
	long cpt;
	size_t length;

	cpt =0;						
						
	length=filename.length();

	//printf ("\nLength:%d",length);
	if (length > 0)
	{	
		// NTW : always write to binary...
		Cont_Mesh.Mesh_write(My_Obj, filename, 0, file_type, save_norms);

						
	}			
}

// Direct Save Mesh
void MeshTools::Save_Mesh_File(int type, int mode, int file_type, int save_norms) 
{
	
	// type : 0 binary, 1 ASCII
	// mode : 0 keep original position, 1 modified
	// file_type : 0 STL 1 VTK 3 PLY 2 OBJ
	long cpt;
	
	cpt =0;
	std::string Name = get_name_of_first_selected_obj();
	std::string szFile=""; 
		
	//char *szFile;
	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
	fnfc.preset_file(Name.c_str());
	if (file_type==1)
	{		
		fnfc.title("Save VTK");		
		fnfc.filter("VTK\t*.{vtk,VTK}");								
	}
	else if (file_type==0)
	{
		fnfc.title("Save STL");		
		fnfc.filter("STL\t*.{stl,STL}");		
	}
	else if (file_type==3)
	{
		fnfc.title("Save PLY");		
		fnfc.filter("PLY\t*.{ply,PLY}");		
	}
	else if (file_type==2)
	{
		fnfc.title("Save OBJ");		
		fnfc.filter("STL\t*.{obj,OBJ}");		
		
	}

	switch ( fnfc.show() ) 
	{
			case -1: 
				{
					//std::cout<<"ERROR: "<<fnfc.errmsg()<<"\n";   
					break;  // ERROR
				}
			case  1: {
				//std::cout<<"CANCEL\n";     
				break;  // CANCEL
					 }
			default: 
				{
					//std::cout<<"PICKED: "<<fnfc.filename()<<"\n";   // FILE CHOSEN					
					//Cont_Mesh.Mesh_STLwrite(szFile, Save_Obj);
					szFile = this->correct_filename(fnfc.filename());					
					if (Cont_Mesh.Mesh_write(szFile, type, mode, file_type, save_norms))
					{
		
					}
				break;
				}
	}
	
	
}


void MeshTools::resize(int x, int y, int w, int h)
{
    Fl_Gl_Window::resize(x, y, w, h);

    if( shown() )
    {
        make_current();
        glViewport(0, 0, w, h);
        invalidate();
    }
}
void MeshTools::Mesh_Decompose(int min_region_size, Fl_Double_Window *decomposeWindow)

{
	
	Cont_Mesh.Mesh_Decompose(min_region_size, decomposeWindow);
	this->redraw();


}
void MeshTools::Mesh_Largest_Region()

{
	Cont_Mesh.Mesh_Largest_Region();
	this->redraw();


}

void MeshTools::Mesh_Draw_Polygon()
{
  glDisable(GL_LIGHTING);
 
  
  glPushMatrix();
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  //gluOrtho2D (0, w(), 0, h());
	glOrtho( 0, w(), 0, h(), -1, 1);		
  glMatrixMode (GL_MODELVIEW); 
  glPushMatrix();
  glLoadIdentity(); 
  if (poly.statut ==0)
	{
	  glColor3f(jaune[0], jaune[1], jaune[2]);
	  for (int i =0;i<poly.point_number-1;i++)
	  {
		  glBegin (GL_LINES);
		  //glVertex2d(poly.point_list[i].x,poly.point_list[i].y); 
		  //glVertex2d(poly.point_list[i+1].x,poly.point_list[i+1].y);
		  glVertex3d(poly.point_list[i].x,poly.point_list[i].y, 1); 
		  glVertex3d(poly.point_list[i+1].x,poly.point_list[i+1].y, 1);
		  glEnd();
	  }
	  if (poly.point_number>0)
	  {
		glBegin (GL_LINES);
		glVertex3d(poly.point_list[poly.point_number-1].x,poly.point_list[poly.point_number-1].y, 1); 
		glVertex3d(mouse.x,h() -mouse.y, 1);
		glEnd();
	  }
	}
  else
  {
	  glLineWidth(2);
	  glColor3f(rouge[0], rouge[1], rouge[2]);
	  //glBegin(GL_POLYGON);
	 /* glBegin (GL_LINES);
	  for (int i =0;i<poly.point_number;i++)
	  {
			glVertex3d(poly.point_list[i].x,poly.point_list[i].y, 1);                
	  }
	  glEnd();*/
	  for (int i =0;i<poly.point_number-1;i++)
	  {
		  glBegin (GL_LINES);
		  //glVertex2d(poly.point_list[i].x,poly.point_list[i].y); 
		  //glVertex2d(poly.point_list[i+1].x,poly.point_list[i+1].y);
		  glVertex3d(poly.point_list[i].x,poly.point_list[i].y, 1); 
		  glVertex3d(poly.point_list[i+1].x,poly.point_list[i+1].y, 1);
		  glEnd();
	  }
	  if (poly.point_number>0)
	  {
		glBegin (GL_LINES);
		glVertex3d(poly.point_list[poly.point_number-1].x,poly.point_list[poly.point_number-1].y, 1); 
		glVertex3d(poly.point_list[0].x,poly.point_list[0].y, 1);
		glEnd();
	  }
  }
  glPopMatrix();
  glMatrixMode (GL_MODELVIEW); 
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  
  glLoadIdentity();
  //gluPerspective(/*vertical field of view*/ 45.,
  //               /*aspect ratio*/ (double) w()/h(),
  //               /*znear*/ camera.near1, /*zfar*/ camera.far1);
   glOrtho( -(double)zoom* w()/h(),
	   (double)zoom* w()/h(),
	   -(double)zoom, 
	   (double)zoom, 
	   camera.near1, 
	   camera.far1);


glMatrixMode(GL_MODELVIEW);

  glEnable(GL_LIGHTING);
  //glLoadIdentity(); 
}
void MeshTools::Mesh_invert()
{
	Cont_Mesh.Mesh_invert(this->select_mode);
}

void MeshTools::Mesh_invert_normals()
{
	Cont_Mesh.Mesh_invert_normals(this->select_mode);
}


// on fais le ICP à partir d'une source et d'une cible
void MeshTools::Registration(int source, int target, vtkFloatArray *tabOfParameters, vtkFloatArray* tab_loadsave_parameter, vtkFloatArray *tabDisplay, int modeTransformation, bool bool_onlyMatching){
	if (source < 0 || target < 0)
	{
		fl_alert("You must select objects to be able to compute the registration.");
	}
	else
	{
		OBJECT_MESH *My_Source = Cont_Mesh.Get_Selected_Mesh(source);
		OBJECT_MESH *My_Target = Cont_Mesh.Get_Selected_Mesh(target);
		if (My_Source == NULL || My_Target == NULL)
		{
			fl_alert("At least one of the selected meshes could not be retrieved!");
		}
		else
		{
			cout << "Test color (en cours) ::  ";
			//verification de la présence de couleur ou de la même couleur entre les objets
			bool existColor = true, sameColor = true;
			My_Source->CheckSameColor(My_Target, &existColor, &sameColor);
			bool W_color = tabOfParameters->GetTuple(2)[0] != -1;

			bool colorIsOk = (existColor && sameColor) || (!existColor && !sameColor);

			if (colorIsOk){
				cout << "ok." << endl;
				bool_ICP = true;
				My_Source->bool_Source_used_ICP = true;
				My_Target->bool_Target_used_ICP = true;


				//verification de la présence des landmarks
				bool existland = Cont_Mesh.ExistAlwaysMeshLandmarkICP(bool_onlyMatching);
				if (existland){

					// donne une couleur à la source 
					My_Source->color[0] = 0.f;
					My_Source->color[1] = 0.5f;
					My_Source->color[2] = 0.5f;

					//on calcule les courbures
					cout << "Compute curv : ";
					Compute_curv_ICP(My_Source, My_Target);
					cout << "ok." << endl;

					// ICP 
					registration = new ICP(My_Source, My_Target, Cont_Mesh, loaded_parameter_list, tabOfParameters, tab_loadsave_parameter, tabDisplay, modeTransformation, existColor, bool_onlyMatching);

					// deselectionne les objets
					My_Source->selected = 0;
					My_Target->selected = 0;



				}
				else{/// si pas le bonne nombre de landmarks (et source et target existe mais on l'a deja verifier)
					bool_ICP = false;
					My_Source->bool_Source_used_ICP = false;
					My_Target->bool_Target_used_ICP = false;
					fl_alert("ERROR:: Registration: not the same number of landmarks");

					// deselectionne les objets
					My_Source->selected = 1;
					My_Target->selected = 1;

					free(registration);
				}
			}
			else{
				bool_ICP = false;
				My_Source->bool_Source_used_ICP = false;
				My_Target->bool_Target_used_ICP = false;
				fl_alert("ERROR: not the same number of color \n\tOR not the same color between the meshes \n\tOR not color for one of the two meshes.");

				// deselectionne les objets
				My_Source->selected = 1;
				My_Target->selected = 1;

				free(registration);
			}

		}
	}
}




//on calcule les courbures pour les deux objets si ça n'a pas été calculé
void MeshTools::Compute_curv_ICP(OBJECT_MESH* My_Source, OBJECT_MESH*My_Target){
	
	const char * curv[2];
	curv[0] = "Maximum_Curvature";
	curv[1] = "Minimum_Curvature";

	int i = 0;
	int cpt_curv = 0;

	while (i < 2){

		if (My_Source->GetPointData()->GetScalars(curv[i]) == NULL)
			cpt_curv += 2;
		if (My_Target->GetPointData()->GetScalars(curv[i]) == NULL)
			cpt_curv += 3;

		if (cpt_curv != 0){
			Cont_Mesh.SC_calc_curv(i+1);
		}
		if (cpt_curv == 2 || cpt_curv == 5){
			My_Source->selected = 1;
			My_Source->SC_calc_mean();// on fait 2 lissages
			My_Source->SC_calc_mean();
			My_Source->selected = 1;
		}
		if (cpt_curv == 3 || cpt_curv == 5){
			My_Target->selected = 1;
			My_Target->SC_calc_mean();// on fait 2 lissages
			My_Target->SC_calc_mean();
			My_Target->selected = 1;
		}
		cpt_curv = 0;
		i++;
	}
}

//On ajoute un indice au nom si le nom existe déjà.
//fonction recurente pour savoir quel indice lui donner.
void MeshTools::CheckingName(string *name_obj, int cpt_name){
	string s_cpt_name = std::to_string(cpt_name);
	string name = *name_obj + "(" + s_cpt_name + ")";
	OBJECT_MESH *object;

	if (cpt_name == 0){
		name = *name_obj;
	}

	if (Cont_Mesh.OBJECTS_ROOT->OBJECTS != NULL){
		int cpt = 0;
		object = Cont_Mesh.OBJECTS_ROOT->OBJECTS;
		while (object != NULL)// on parcours tous les objets
		{
			if (object->name == name){// si il existe déjà, on augmente l'indice
				cpt_name++;
				cpt++;
				s_cpt_name = std::to_string(cpt_name);
				name = *name_obj + "(" + s_cpt_name + ")";
			}
			
			object = object->nextobj;
		}//fin while

		if (cpt == 0)
			*name_obj = name;
		else{
			CheckingName(name_obj, cpt_name);
		}
	}
}

//On change les données de l'ICP modifiées dans l'interface
void MeshTools::SetDataICP(vtkSmartPointer<vtkFloatArray> tabOfParameters, vtkFloatArray* tab_loadsave_parameter, int modeTransformation, vtkSmartPointer<vtkFloatArray> tabDisplay, bool bool_only_matchings){
	registration->SetData(tabOfParameters, modeTransformation, tabDisplay,  tab_loadsave_parameter, bool_only_matchings);
}

//On change les données d'affichage de l'ICP modifiées dans l'interface
void MeshTools::SetDisplayMatching(vtkSmartPointer<vtkFloatArray> tabDisplay){
	registration->tab_display->DeepCopy(tabDisplay);
}

//On change la donnée couleur des correspondances (sommet) pour l'affichage de l'ICP modifiées dans l'interface
void MeshTools::SetVerticeColor(float vect_color[3]){
	registration->vect_matching_color[0] = vect_color[0];
	registration->vect_matching_color[1] = vect_color[1];
	registration->vect_matching_color[2] = vect_color[2];
}

//On sauvegarde le tableau d'erreurs de l'ICP
void MeshTools::SaveErrors(){
	Save_Errors_ICP(GetErrors_ICP());
}

//On sauvegarde le tableau d'erreurs de l'ICP dans un fichier
void MeshTools::Save_Errors_ICP(vtkFloatArray* errors){
	int ok = 1;
	string file_name;
	
	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
	fnfc.title("Save Errors ICP");
	fnfc.filter("dat\t*.{dat,DAT}");

	if (ok == 1){
		switch (fnfc.show())
		{
			case -1:
				break;
			case 1:
				break;

			default:
			{
				file_name = this->correct_filename(fnfc.filename());
				std::string DAText = ".dat";
				std::string DAText2 = ".DAT";
				std::size_t found = file_name.find(DAText);
				std::size_t found2 = file_name.find(DAText2);
				if (found == std::string::npos && found2 == std::string::npos)
				{
					file_name.append(".dat");
				}

				int number_errors = errors->GetNumberOfTuples();
				ifstream file(file_name.c_str());
				if (file)
				{
					ok = fl_ask("Overwrite?");
					file.close();

				}
				if (ok == 1)
				{
					FILE *filein = fopen(file_name.c_str(), "w");
					int i = 0;
					fprintf(filein, "ite\tmatching\tland\ttotal\n");

					int val1AfterComma = 0, val2AfterComma = 0, val3AfterComma = 0;
					int truncVal1 = 0, truncVal2 = 0, truncVal3 = 0;

					while (i < number_errors)
					{
						truncVal1 = trunc(errors->GetComponent(i, 0));
						val1AfterComma = (errors->GetComponent(i, 0) - truncVal1) * 1000000;
						truncVal2 = trunc(errors->GetComponent(i, 1));
						val2AfterComma = (errors->GetComponent(i, 1) - truncVal2) * 1000000;
						truncVal3 = trunc(errors->GetComponent(i, 2));
						val3AfterComma = (errors->GetComponent(i, 2) - truncVal3) * 1000000;
						fprintf(filein, "%d\t%d,%d\t%d,%d\t%d,%d\n", (i + 1), truncVal1, val1AfterComma, truncVal2, val2AfterComma, truncVal3, val3AfterComma);
						i++;
					}

					fclose(filein);
				}
				break;
			}
		}
	}

}

//On recupère le tableau d'erreur de l'ICP
vtkSmartPointer<vtkFloatArray> MeshTools::GetErrors_ICP(){
	return registration->errors;
}

//On sauvegarde les correspondances des sommets
void MeshTools::save_SourceTarget_Vertice(){
	int ok1 = 1,ok2=1;
	std::string filename;

	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
	fnfc.title("Save Source and Target Vertice");
	fnfc.filter("STV\t*.{stv,STV}");

	int source_landmarks_number = 0, target_landmarks_number = 0;

	//verification du même nombre de landmarks
	if (bool_ICP){
		source_landmarks_number = registration->numberOfLandmarks_Source;
		target_landmarks_number = registration->numberOfLandmarks_Source;
	}
	else{
		source_landmarks_number = Cont_Mesh.Get_Landmark_Number(0);
		target_landmarks_number = Cont_Mesh.Get_Landmark_Number(1);
		if (source_landmarks_number == 0)
		{
			fl_alert("There are currently no normal landmark to be saved");
			ok1 = 0;
		}
		if (target_landmarks_number == 0)
		{
			fl_alert("There are currently no target landmark to be saved");
			ok2 = 0;
		}
		
	}

	if (ok1 == 1  && ok2 == 1)
	{
		switch (fnfc.show())
		{
			case -1:break;  // ERROR
			case  1:break;  // CANCEL
			default:
			{
				filename = this->correct_filename(fnfc.filename());
				string STVext = ".stv";
				string STVext2 = ".STV";
				size_t found = filename.find(STVext);
				size_t found2 = filename.find(STVext2);
				if (found == std::string::npos && found2 == std::string::npos)
				{
						filename.append(".stv");
				}
			

				ifstream file(filename.c_str());
				if (file)
				{
					ok1 = fl_ask("Overwrite?");
					file.close();

				}
				if (ok1 == 1)
				{
					Write_STV(filename,source_landmarks_number,target_landmarks_number);
				
				}//overwrite?
				break;
			}
		}
	}
}

//On écrit dans le fichier STV les correspondances
void MeshTools::Write_STV(string filename, int source_landmarks_number, int target_landmarks_number){
	FILE *filein = fopen(filename.c_str(), "w");
	int landmark_mode;
	
	if (bool_ICP){
		landmark_mode = -1;
		int i = 0;
		double *vect, *vectNor;
		vtkSmartPointer<vtkFloatArray> vertice = vtkSmartPointer<vtkFloatArray>::New();
		vtkSmartPointer<vtkFloatArray> vertice_norms = vtkSmartPointer<vtkFloatArray>::New();
		cout << "Save ICP Matching (en cours) :: ";
		int dec_reg = (registration->dec);
		int dec_tab = (registration->tab_display->GetTuple1(2));
		int new_Source_number = (int)(source_landmarks_number / dec_tab + 1);
		int dim = registration->dim_points;
		
		int i1 = 0;
		while (i <= source_landmarks_number && landmark_mode < 2){
			if (i == 0){
				landmark_mode++;
				if (landmark_mode < 2){//On initialise les listes 
					fprintf(filein, "%d %d\n", landmark_mode, new_Source_number);
					if (landmark_mode == 0){//Source
						vertice = vtkSmartPointer<vtkFloatArray>::New();
						vertice->SetNumberOfComponents(dim);
						vertice->SetNumberOfTuples(source_landmarks_number);
						vertice->DeepCopy(registration->list_landmarks_dimN_source);

						vertice_norms = vtkSmartPointer<vtkFloatArray>::New();
						vertice_norms->SetNumberOfTuples(registration->numberOfPoints_Source);
						vertice_norms=vtkFloatArray::SafeDownCast(registration->Source->GetPointData()->GetNormals());
						i1 = 0;
					}
					else if (landmark_mode == 1){//Cible
						vertice = vtkSmartPointer<vtkFloatArray>::New();
						vertice->SetNumberOfComponents(dim);
						vertice->SetNumberOfTuples(source_landmarks_number);
						vertice->DeepCopy(registration->list_landmarks_dimN_target);

						vertice_norms = vtkSmartPointer<vtkFloatArray>::New();
						vertice_norms->SetNumberOfTuples(registration->Target->GetNumberOfPoints());
						vertice_norms = vtkFloatArray::SafeDownCast(registration->Target->GetPointData()->GetNormals());

						i1 = 0;
					}
				}
			}
			else{

				if ((i - 1) % (dec_tab) == 0){/// selon le diviseur donnée dans l'interface

					int ind = vertice->GetComponent(i - 1, dim - 1); // on prend son indice

					if (landmark_mode == 0){// seulement pour "list_landmarks_dimN_source" qui est un sous-ensemble de "Source". pour les points cibles, on a leur vrai indice dû au KDtree.
						ind *= dec_reg;
					}

					vect = vertice->GetTuple(i - 1);// on prend les coordonnées des points 
					vectNor = vertice_norms->GetTuple(ind);//on prend les coordonnées de leurs normales
				
					fprintf(filein, "%s%d: %f %f %f %f %f %f %d\n", "landmark", i1+1, vect[0], vect[1], vect[2], vectNor[0], vectNor[1], vectNor[2],ind);
					i1++;
				}
			}
			i++;
			if (i == source_landmarks_number+1)
				i = 0;
		}//while
		cout << "done" << endl;
	}
	else{
		cout << "Save Matching (en cours) :: " << endl;
		int cpt;
		glMatrix wc_mat;
		float vv[3], vv1[3], vvn[3], vvn1[3];
		cpt = 0;
		OBJECT_LANDMARK *k = NULL;
		int ind;
		landmark_mode = 0;
		while (landmark_mode < 2){
			ind = 0;
			if (landmark_mode == 0){
				fprintf(filein, "%d %d\n", landmark_mode, source_landmarks_number);
			}
			else{
				fprintf(filein, "%d %d\n", landmark_mode, target_landmarks_number);
			}
			
			k = Cont_Mesh.landmarkafter(ind, landmark_mode);
			while (k != NULL)
			{
				k->get_world_coordinates_matrix(wc_mat);

				vv[0] = 0;
				vv[1] = 0;
				vv[2] = 0;
				vvn[0] = 1.0;
				vvn[1] = 0;
				vvn[2] = 0;
				ApplyTransformation(vv, vv1, wc_mat);
				ApplyTransformation(vvn, vvn1, wc_mat);
				cpt++;
				ind = k->landmark_index;
				
				fprintf(filein, "%s%d: %f %f %f %f %f %f\n", "landmark", ind, vv1[0], vv1[1], vv1[2], vvn1[0], vvn1[1], vvn1[2]);
					
				k = Cont_Mesh.landmarkafter(ind, landmark_mode);
			}
			landmark_mode++;
		}//while
		cout << "done" << endl;
	}//else
	fclose(filein);
}

//On ouvre les fichier STV (les correspondances Source-Cible)
void MeshTools::Open_SourceTarget_Vertice(){
	float  param2, param3, param4, param5, param6, param7;
	float m_ve[3], m_ven[3], leng;
	char param1[50];
	FILE	*filein;// Filename To Open
	char	oneline[255];
	int landmark_mode;

	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
	fnfc.title("Load landmarks");
	fnfc.filter("Landmarks\t*.{stv,STV}");
	switch (fnfc.show())
	{
	case -1:break;  // ERROR
	case  1:break;  // CANCEL
	default:
	{
		string filename = this->correct_filename(fnfc.filename());	
		int file_exists = 1;
		ifstream file(filename.c_str());

		if (file)
		{
			file.close();
		}
		else
		{
			cout << "file:" << filename.c_str() << " does not exists." << std::endl;
			file_exists = 0;
		}

		if (file_exists == 1)
		{
			std::string STVext(".stv");
			std::string STVext2(".STV");
			
			int type = 1; //VER
			
			filein = fopen(filename.c_str(), "r");
			readstr(filein, oneline);
			feof(filein);
			std::cout << "Try open landmark file " << std::endl;
			std::cout << "feof(filein)" << feof(filein) << std::endl;
			int ind = 0;
			vtkSmartPointer<vtkFloatArray> param_list = vtkSmartPointer<vtkFloatArray>::New();
			param_list->SetNumberOfComponents(1);
			int number = 0;
			int cpt_line = 0;
			while (!feof(filein))
			{
				if (cpt_line == 0){
					sscanf(oneline, "%d %d\n", &landmark_mode, &number);
				}
				else{
					sscanf(oneline, "%s %f %f %f %f %f %f %d\n", param1, &param2, &param3, &param4, &param5, &param6, &param7,&ind);
					param_list->InsertNextTuple1(param2);
					param_list->InsertNextTuple1(param3);
					param_list->InsertNextTuple1(param4);
					param_list->InsertNextTuple1(param5);
					param_list->InsertNextTuple1(param6);
					param_list->InsertNextTuple1(param7);
					
					create_landmarks(landmark_mode, param_list, type);

					param_list = vtkSmartPointer<vtkFloatArray>::New();
					param_list->SetNumberOfComponents(1);

				}
				readstr(filein, oneline); //read next line
				cpt_line++;

				if (cpt_line == number + 1 && landmark_mode == 0){
					cpt_line=0;
					landmark_mode++;
				}
			}//While scanff...
			fclose(filein);
			break;
		}//if file exists
	}//default	
	}//switch		

	this->redraw();
}

//On sauvegarde les sommets Source.
void MeshTools::Save_original_source_verticeICP(){
	if (bool_ICP){
		int ok = 0;
		std::string filename;
		cout << "Save ICP vertice (en cours) :: ";
		Fl_Native_File_Chooser fnfc;
		fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
		fnfc.title("Save the Original Source Vertice");
		fnfc.filter("VER\t*.{ver,VER}");

		int source_vertice_number = registration->numberOfPoints_Source;
		int dec = (int)(registration->dec * registration->tab_display->GetTuple1(2));
		//(dec+decdisplay)

		if (registration->Source != NULL && source_vertice_number != 0){
			ok = 1;
		}

		if (ok == 1)
		{
			switch (fnfc.show())
			{
			case -1:break;  // ERROR
			case  1:break;  // CANCEL
			default:
			{
				filename = this->correct_filename(fnfc.filename());
				string VERext = ".ver";
				string VERext2 = ".VER";
				size_t found = filename.find(VERext);
				size_t found2 = filename.find(VERext2);
				if (found == std::string::npos && found2 == std::string::npos)
				{
					filename.append(".ver");
				}


				ifstream file(filename.c_str());
				if (file)
				{
					ok = fl_ask("Overwrite?");
					file.close();

				}
				if (ok == 1)
				{
					FILE *filein = fopen(filename.c_str(), "w");
					
					vtkSmartPointer<vtkFloatArray> vertice = vtkSmartPointer<vtkFloatArray>::New();
					vertice->SetNumberOfComponents(registration->list_vertice_init_source->GetNumberOfComponents());
					vertice->DeepCopy(registration->list_vertice_init_source);
					
					double *vect;
					int i = 0,i1=0;
					while (i < source_vertice_number){
						if ((i) % dec == 0){
							vect = vertice->GetTuple(i);
			
							fprintf(filein, "%s%d: %f %f %f %f %f %f %f\n", "landmark", (i1 + 1), vect[0], vect[1], vect[2], vect[3], vect[4], vect[5],vect[6]);
							i1++;
						}
						i++;
					}//while
					fclose(filein);
				}
				cout << "done" << endl;

				}//overwrite?
				break;
			}
		}
	}
}

//On sauvegarde les parametres de l'ICP
void MeshTools::SaveParameter(){
	Save_ICP_parameter(GetParameterList());
}

//On sauvegarde les parametres de l'ICP
void MeshTools::Save_ICP_parameter(vtkFloatArray* parameter_list){
	int ok = 1;
	string file_name;

	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
	fnfc.title("Save Errors ICP");
	fnfc.filter("txt\t*.{txt,TXT}");

	
	if (ok == 1){
		switch (fnfc.show())
		{
		case -1:
			break;
		case 1:
			break;

		default:
		{
			file_name = this->correct_filename(fnfc.filename());
			std::string TXText = ".txt";
			std::string TXText2 = ".TXT";
			std::size_t found = file_name.find(TXText);
			std::size_t found2 = file_name.find(TXText2);
			if (found == std::string::npos && found2 == std::string::npos)
			{
				file_name.append(".txt");
			}

		
			ifstream file(file_name.c_str());
			if (file)
			{
				ok = fl_ask("Overwrite?");
				file.close();

			}
			if (ok == 1)
			{
				FILE *filein = fopen(file_name.c_str(), "w");
				cout << "Save ICP parameters (en cours) :: ";

				const char *source_name = (registration->Source->name).c_str(), *target_name = (registration->Target->name).c_str();

				fprintf(filein, "Source: %s\nTarget: %s\n",source_name,target_name);

				int numberOfComponents = parameter_list->GetNumberOfComponents();
				int size = parameter_list->GetNumberOfTuples();

				
				int val1AfterComma = 0, val2AfterComma = 0, val3AfterComma = 0;
				int truncVal1 = 0, truncVal2 = 0, truncVal3 = 0;
				int j = 0;
				int i = 0;
				while (i < size)
				{
					for (j = 0; j < numberOfComponents; j++){
						switch (j){
							case 0: /// 0 si le mode n'a pas de parametre et 1 si il en a.(bool_mode) 
								fprintf(filein, "%d\n", (int)parameter_list->GetComponent(i, 0)); break;
							case 1:
								fprintf(filein, "Mode: %s\n", registration->GetMode(i)); break;
							case 2:
								fprintf(filein, "Normal_weight: %f\n", parameter_list->GetComponent(i,2)); break;
							case 3:
								fprintf(filein, "Curvature_weight: %f\n", parameter_list->GetComponent(i, 3)); break;
							case 4:
								fprintf(filein, "Color_weight: %f\n", parameter_list->GetComponent(i, 4)); break;
							case 5:
								fprintf(filein, "AddLandmark_weight: %f\n", parameter_list->GetComponent(i, 5)); break;
							case 6:
								fprintf(filein, "Divisor_OfTheSourceVertice_Number: %f\n", parameter_list->GetComponent(i, 6)); break;
							case 7:
								fprintf(filein, "Percentage_Of_filter_Radius: %f\n", parameter_list->GetComponent(i, 7)); break;
							case 8:
								fprintf(filein, "MaximumNumberIteration: %f\n", parameter_list->GetComponent(i, 8)); break;
							case 9:
								fprintf(filein, "Error_Threshold: %f\n", parameter_list->GetComponent(i, 9)); break;
							case 10:
								fprintf(filein, "Percentage_Of_Sphere_radius_LA: %f\n", parameter_list->GetComponent(i, 10)); break;
							case 11:
								fprintf(filein, "Percentage_Remove_curvature: %f\n", parameter_list->GetComponent(i, 11)); break;
							default:
								fl_alert("ERROR: in Save parameters"); break;
						}
						
					}
					cout << endl;
					i++;
				}

				fclose(filein);
				cout << "done" << endl;
			}
			break;
		}
		}
	}



}

//On ouvre le fichier contenant les paramètres
void MeshTools::Open_ICP_parameter(){
	float  param2, param3, param4, param5, param6, param7;
	float m_ve[3], m_ven[3], leng;
	char param1[50];
	FILE	*filein;// Filename To Open
	char	oneline[255];
	int landmark_mode;

	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
	fnfc.title("Load parameters");
	fnfc.filter("Parameters\t*.{txt,TXT}");
	switch (fnfc.show())
	{
	case -1:{break; }  // ERROR
	case  1:{break; } // CANCEL
	default:
	{
		string filename = this->correct_filename(fnfc.filename());
		int file_exists = 1;
		ifstream file(filename.c_str());

		if (file)
		{
			file.close();
		}
		else
		{
			cout << "file:" << filename.c_str() << " does not exists." << std::endl;
			file_exists = 0;
		}

		if (file_exists == 1)
		{
			std::string TXText(".txt");
			std::string TXText2(".TXT");

			filein = fopen(filename.c_str(), "r");
			readstr(filein, oneline);
			feof(filein);
			std::cout << "Try open parameter file " << std::endl;
			std::cout << "feof(filein)" << feof(filein) << std::endl;
			cout << "Open parameter file (en cours) :: ";
			int nbcomponent = 12;
			vtkSmartPointer<vtkFloatArray> param_list = vtkSmartPointer<vtkFloatArray>::New();
			param_list->SetNumberOfComponents(nbcomponent);
			
			float tuple[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			float number = 0;
			int indice = 0;
			int cpt_line = 0,cpt_mode=-1;
			char sentence[20];
			char mode[20];
			readstr(filein, oneline);
			readstr(filein, oneline);
			while (!feof(filein))
			{
				indice = cpt_line%nbcomponent;
				if (indice == 0){
					if (cpt_mode != -1){
						param_list->InsertNextTuple(tuple);
					}
					sscanf(oneline, "%f\n", &number);
					tuple[indice] = number;
					cpt_mode++;
				}
				else if (indice == 1){
					sscanf(oneline, "%s %s\n", &sentence, &mode);
					tuple[indice] =(float) registration->GetModeInv(mode);
				}
				else{
					sscanf(oneline, "%s %f\n", &sentence, &number);
					tuple[indice] = number;
				}
				readstr(filein, oneline); //read next line
				cpt_line++;
			}//While scanff...
			fclose(filein);

			// on copie la liste des parametres de chaque mode
			loaded_parameter_list = vtkSmartPointer < vtkFloatArray > ::New();
			loaded_parameter_list->SetNumberOfComponents(param_list->GetNumberOfComponents());
			loaded_parameter_list->DeepCopy(param_list);
			cout << "done" << endl;
			break;
		}//if file exists
	}//default	
	}//switch
}

//Donne la liste de parametres pour chaque mode de transformation
vtkSmartPointer<vtkFloatArray> MeshTools::GetParameterList(){
	return registration->parameter_list;
}

//Donne dans l'interface, dans quel mode de transformation de l'ICP on se trouve
void MeshTools::Mesh_draw_ICPMode_info(int x, int y)
{
	if (bool_ICP){
		char info1[100], modeTextICP[50];
		if (!registration->bool_onlyMatching){
			strcpy(info1, "Mode of ICP Transformation::");
			strcpy(modeTextICP, registration->GetMode());
			strcat(info1, modeTextICP);
			
		}
		else{
			strcpy(info1, "Only Matchings Of ICP");
		}
		AfficheTexte(x, h() - y, info1, blanc);
	}
}

// On change le mode d'affichage des objets (VBO)
void MeshTools::drawDispMode(){
	//dispmode:
	//0-- Wire
	//1--normal surface
	//2--flat
	//3-- wire and flat

	if (this->disp_cull_face == 0)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	if (dispmode == 0 || dispmode == 1){
		glShadeModel(GL_SMOOTH);
	}
	else{
		glShadeModel(GL_FLAT);
	}
	// offset the wireframe 
	
	if (dispmode == 0 ){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	}
	if (dispmode != 0 && dispmode != 3){
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	if (dispmode == 1 || dispmode == 4 || dispmode == 5){
		if (g_enable_2sides == 1)
			{
				glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
			}
			else
			{
				glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
			}
	}
	bool_change_mode = 0;
}


// On active la couleur initiale pour tous les objets selectionné
void MeshTools::Active_InitRGB(){

	Cont_Mesh.Active_InitRGB();
}

//On change la valeur du bool_vbo
void MeshTools::VBO_activated(bool bool_vbo){
	this->bool_vbo = bool_vbo;
}


void MeshTools::FileCurv(OBJECT_MESH * object){
	int ok = 1;
	string file_name;

	Fl_Native_File_Chooser fnfc;
	fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
	fnfc.title("Save Errors ICP");
	fnfc.filter("txt\t*.{txt,TXT}");


	if (ok == 1){
		switch (fnfc.show())
		{
		case -1:
			break;
		case 1:
			break;

		default:
		{
			file_name = this->correct_filename(fnfc.filename());
			std::string TXText = ".txt";
			std::string TXText2 = ".TXT";
			std::size_t found = file_name.find(TXText);
			std::size_t found2 = file_name.find(TXText2);
			if (found == std::string::npos && found2 == std::string::npos)
			{
				file_name.append(".txt");
			}


			ifstream file(file_name.c_str());
			if (file)
			{
				ok = fl_ask("Overwrite?");
				file.close();

			}
			if (ok == 1)
			{
				FILE *filein = fopen(file_name.c_str(), "w");
				cout << "Save ICP curv (en cours) :: ";

				vtkFloatArray *currentCurv1 = vtkFloatArray::New();
				currentCurv1->SetNumberOfComponents(1);

				vtkFloatArray *currentCurv2 = vtkFloatArray::New();
				currentCurv2->SetNumberOfComponents(1);

				currentCurv1->DeepCopy(vtkFloatArray::SafeDownCast(object->GetPointData()->GetScalars("Maximum_Curvature")));
				currentCurv2->DeepCopy(vtkFloatArray::SafeDownCast(object->GetPointData()->GetScalars("Minimum_Curvature")));

				if (vtkFloatArray::SafeDownCast(object->GetPointData()->GetScalars("Maximum_Curvature")) != NULL && vtkFloatArray::SafeDownCast(object->GetPointData()->GetScalars("Minimum_Curvature")) != NULL){

					vtkSmartPointer<vtkFloatArray> tabCurv1 = vtkSmartPointer<vtkFloatArray>::New();
					tabCurv1->SetNumberOfComponents(2);

					set<double> SetCurv1;

					int numberOfComponents = currentCurv1->GetNumberOfComponents();
					int size = currentCurv1->GetNumberOfTuples();
					int j = 0, compt = 0;
					for (int i = 0; i < size; i++){
						int sizeSet = SetCurv1.size();
						//cout << " " << sizeSet;
						double val = trunc(currentCurv1->GetComponent(i, 0));
						SetCurv1.insert((double)val);
						//cout << "_" << SetCurv1.size() << " ";
						if (sizeSet == SetCurv1.size()){
							for (int k = 0; k < tabCurv1->GetNumberOfTuples(); k++){
								if (tabCurv1->GetComponent(k, 0) == val){
									j = k;
								}
							}
							compt = tabCurv1->GetComponent(j, 1) + 1;
							tabCurv1->SetComponent(j, 1, compt);// 1 = compt, 0 = val
						}
						else{
							//cout << "la";
							tabCurv1->InsertNextTuple2(val, 1);
						}

					}

					cout << "size " << tabCurv1->GetNumberOfTuples();

					/// fichier 
					int i = 0;
					while (i < tabCurv1->GetNumberOfTuples())
					{
						fprintf(filein, "%f\t%d\n", tabCurv1->GetComponent(i, 0), (int)tabCurv1->GetComponent(i, 1));
						i++;
					}
				}
				fclose(filein);
				cout << "done" << endl;
			}
			break;
		}
		}
	}

}



//
// End of "$Id: MeshTools.cxx,v 1.4.2.4.2.5 2004/04/11 04:39:00 easysw Exp $".
//
