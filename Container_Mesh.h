#ifndef CONTAINER_MESH_H
#define CONTAINER_MESH_H 35
#include "StdAfx.h"
#include <stdlib.h>
//#include <gl/gl.h>
//#include <gl/glu.h>


#include <math.h>										// Math Library Header File
#include <stdio.h>
#include <string.h>
#include "CDataFile.h"
#include <FL/gl.h>
#include <FL/Fl_File_Chooser.H>
#include "Struct.h"
//#include "Struct2.h"
#include "Object_Mesh.h"
#include "Object_Log.h"
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
extern "C" {
//#include "nrutil.h"
//#include "svdcmp.c"

}

#include "Object.h"
//#include "matrix.h"
#include "Container.h"
#include <time.h>
//#include "Struct.h"
//#ifndef STRUCT_H
#ifndef _NO_NAMESPACE
using namespace std;
//using namespace math;
#define STD std
#else
#define STD
#endif

//#ifndef _NO_TEMPLATE
//typedef matrix<double> Matrix;
//#else
//typedef matrix Matrix;
//#endif
//#endif 
class CONTAINER_MESH : public CONTAINER  
	// Structure For An Object
{
public:
	int numvert; // Dirty : used in Mesh but should be only used in OBJECT
	//int numvert; // Dirty : used in Mesh but should be only used in OBJECT
	//OBJECT_MESH		*OBJECTS;	 //"Normal" object list
	//OBJECT_LANDMARK		*LANDMARKS; //Landmarks on normal object
	OBJECT_LOG     *OBJECTS_ROOT;

	POLYGON_LIST	vect_list;
	POLYGON_LIST	vect_dense_list;
	POLYGON_LIST	land_list;
	POLYGON_LIST	land_dense_list;
	int				vect_list_status;
	int				vect_dense_list_status;
	 float *sauv_ve;

	void Mesh_Mirror_ThroughY ();

	void Update_RGB();

	void Landmark_Move_Down();
	void Landmark_Move_Up();
	int Is_Segment_Selected (int seg);
	std::string Get_Active_Scalar_Name();
	std::string Get_Scalar_Name(int scalar);

	void Object_Move_Down();
	void Object_Move_Up();
	void Mesh_TPS( int basis, float percentage);
	void Compute_Global_Mean(int only_selected =1);
	void Compute_Global_MinMax();
	void Set_Active_Scalar();
	void Initialize_Scalar(int scalar);
	void Remove_Scalar(int scalar);
	void Compute_Global_Scalar_List();
	void Compute_Name_Lists();
	void rollinit_objects();
	void SC_calc_depth();
	void Mesh_Decimate(int percentage, int method);
	void Mesh_Densify(int subdivision_nr);
	void Mesh_Fill_Holes(int holes_size);
	void SC_calc_curv(int mode);
	void SC_calc_mean();
	void SC_calc_thickness(float thickness_max_distance,Fl_Double_Window* thicknessWindow);
	void SC_calc_thickness_between_objects(float thickness_max_distance, 
										   OBJECT_MESH *source, 
										   OBJECT_MESH *target,
										   Fl_Double_Window* thicknessWindow2);

	int Get_Curve_Segment_Number();
	float Get_Curve_Segment_Length(int seg);
	void Curve_start(int direction);
	void Mesh_Draw_Curve();
	void Mesh_Tri_Sort(int side);
	float scalars_get_min();
	float scalars_get_max();
	void Mesh_CreateLandmarkAtMouse (float x, float y, int landmark_mode);
	void Mesh_MoveLandmarkAtMouse(OBJECT_LANDMARK *p, float x, float y, int landmark_mode);
	void Mesh_Select(float x, float y);
	void Selected_Landmarks_Change_Orientation(int landmark_mode);

	void Stick_Selected_Landmarks_On_Surfaces();
	void Mesh_container_setlandmarksize(float landmark_size);
	OBJECT_LANDMARK* landmarkafter(int afterind, int landmark_mode);
	OBJECT_LANDMARK* landmarkbefore(int beforeind, int landmark_mode);
	int Mesh_container_newlandmarkindex(int landmark_mode);
	void Mesh_Draw_Obj(int mode, bool bool_change_pos_obj,bool bool_vbo);
	void Add_Landmark(OBJECT_LANDMARK * New_Landmark, int landmark_mode);
	void Mesh_containerfree(void);									// Frees The Object (Releasing The Memory)
	void Mesh_Delete(int mode, int numtri, float volume);
	//void Mesh_DeleteVolume(float volume);	// Frees The Object (Releasing The Memory)

	OBJECT_MESH* Get_Selected_Mesh(int selected_nr);

	OBJECT_MESH* GetClosestVertex(float input [3], float *x,float *y,float *z, float *nx,float *ny,float *nz, vtkIdType* ve,int mode);
	void Set_Blend (int blend_value);
	OBJECT_MESH *Mesh_PDcontainerload(vtkSmartPointer<vtkPolyData> MyObj, char *MESHname);


	//OBJECT_MESH *Mesh_PDcontainerload(OBJECT_MESH *MyObj, char *MESHname);

	OBJECT_MESH *Mesh_Create (GLuint texture, int width, int height, float scale);

	// Loads Object From File (name)
	OBJECT_LANDMARK * CreateLandmark(float x, float y, float z, float nx, float ny, float nz, int landmark_mode); 

	void Mesh_Ungroup();
	void Mesh_Group();
	void Get_Landmark_Coordinates (int landmark_number,float *x,float *y,float *z, int landmark_mode);

	int Mesh_write(std::string filename,int type, int mode, int file_type, int save_norms);


	void Mesh_Decompose_Tag(int min_region_size, Fl_Double_Window* myWindow);
	void Mesh_Delete_Scalar (double scalar_to_remove);
	void Mesh_Extract_Scalar_Range(double scalar_min, double scalar_max);


	int Mesh_write(OBJECT_MESH *My_Obj, std::string filename, int type, int file_type, int save_norms);
	void Mesh_Decompose(int min_region_size, Fl_Double_Window *mywindow);
	void Mesh_Tag_Connected_Regions();
	void Mesh_Largest_Region();
	void Mesh_UnselectAll();
	void Mesh_SelectAll();

	void Hierarchy();
	void get_object_names_and_view_values(std::vector<std::string>& object_names, std::vector<int>& object_view_values);
	void set_object_view_values( std::vector<int>&object_new_view_values);
	void Mesh_Select(float x1, float x2, float y1, float y2, int select_mode);
	int mround(float mnum);
	void Mesh_Lasso_Vertices(int h_screen, int w_screen,float mousex, float mousey, int mode);
	void Mesh_Tag(float x, float y, int mode);
	void Convert_RGB_To_Tags(int exact);
	void Mesh_Tag_Within_Lasso(float x, float y, int lasso_x, int lasso_y, int h_screen, int w_screen, int mode);
	void Fill_Holes (int tag_id, int max_size,  Fl_Double_Window* fillholesWindow2);
	void Merge_Tags(int source, int target);
	void Mesh_area_volume();
	void smooth(int iterationnumber, double relaxation_factor);
	int Get_Landmark_Number(int mymode);
	int Get_Landmark_Selected(int landmark_mode);
	OBJECT_LANDMARK* Get_Selected_Landmark(int landmark_mode);
	int nbLANDMARKS;


	void Mesh_invert( int select_mode);
	void Mesh_invert_normals ( int select_mode);  



	bool ExistAlwaysMeshLandmarkICP(bool bool_onlyMatching);
	void Set_List_of_ObjectMesh_Coordinates(vtkPoints* new_list_Coord);
	void Set_List_of_SourceLandmarks_Coordinates(vtkPoints* new_landmaks_list);
	bool RecoverLandmarks(vtkSmartPointer< vtkPoints > landmarks_list_source, vtkSmartPointer< vtkPoints > landmarks_list_target);
	void Update_Normal(vtkPolyData* My_Output);
	void Recover_PositionObject_in_PolyData(const OBJECT_MESH* My_Input, vtkSmartPointer<vtkPolyData> My_Out);
	void Active_InitRGB();

};
#endif
