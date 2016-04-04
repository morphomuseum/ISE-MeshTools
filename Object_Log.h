#ifndef OBJECT_LOG_H
#define OBJECT_LOG_H 455
#include "StdAfx.h"
#include <stdlib.h>
#include <math.h>										// Math Library Header File
#include <stdio.h>
#include <string.h>
#include "CDataFile.h"
#include <FL/gl.h>
#include <FL/Fl_File_Chooser.H>
//#include "Struct2.h"
#include "Struct.h"
#include "Object.h"

//#include "kdtree.h"
#include "Object_Landmark.h"
#include "Object_Mesh.h"

//#include "Struct.h"

//#ifndef STRUCT_H

#include <list>
#include <vtkAppendPolyData.h>

//#endif 
class OBJECT_LOG : public OBJECT
	// Structure For An Object
{
public:
int  selected; // Flag set to 1 if the current group is selected.
				// Flag should never be set to 1 for the big root.
 GLfloat color[4];


//OBJECT_MESH *Obj_Mesh; //The chain of Grouped meshes.
OBJECT_LANDMARK *Obj_Landmark;//The chain of Grouped landmarks.
OBJECT_LOG *nextobj;//GROUPS OF OBJECTS_LOG can be chained together
OBJECT_LOG *Obj_Root; // The reference to the root of every object should be kept!
 OBJECT_LOG *prevobj;
 OBJECT_MESH *OBJECTS;
 OBJECT_LOG *OBJECTS_LOG;
 OBJECT_LANDMARK *OBJECTS_LANDMARK;// the red landmarks
 OBJECT_LANDMARK *OBJECTS_LANDMARK_TARGET; // the violet landmarks
 OBJECT_LANDMARK *OBJECTS_LANDMARK_LABELS; // Tag Labels
 //Test version 10
int is_logical;
int level; //If 0, this means this object is the big_root object (Obj_Root)
			  //1 or more if is a grouped object.



float mean[3];
int nmean; // number of poinds used to compute nmean;
void Compute_Global_MinMax();


void Compute_Global_Mean(int only_selected = 1); // Compute center of mass of all selected objects (only applies to root objects)
void Compute_Global_Scalar_List();
void Compute_Name_Lists();
 void Update_RGB();
void Calculate_GlobalMean(float *x, float *y, float *z, int *nb, int only_selected); // compute center of masse of this object (should not be root)
void Calculate_Global_MinMax(float *minx, float *maxx, float *miny, float *maxy, float *minz, float *maxz);
int GetNumtri();
void MergeObjects(vtkSmartPointer<vtkAppendPolyData> mergedObjects, int mode);
void SC_calc_depth();
void SC_calc_mean();

void SC_calc_thickness(float thickness_max_distance, Fl_Double_Window* thicknessWindow);

void Mesh_init_Mat();
void Mesh_Ungroup();
void Mesh_Tri_Sort( int side);
void DeleteObjs();
void Mesh_Unselect();
void Mesh_Select();


void Landmark_Move_Down(int mode, int landmark_mode);
void Landmark_Move_Up(int mode, int landmark_mode);
void Change_Landmark_Indices(int landmark_mode);
void Reorder_Landmarks(int landmark_mode);
int Is_Landmark_List_In_Ascending_Order(int landmark_mode);
void Unselect_landmarks(int landmark_mode);

void Object_Move_Down();
void Object_Move_Up();

void Mesh_Increase_Level();
void Mesh_Decrease_Level();
float scalars_get_min();
float scalars_get_max();
void Curve_start(int direction);
void Mesh_Draw_Curve();
void Mesh_Group();
void Mesh_Group_Log(OBJECT_LOG *MyLog);
void Mesh_Group_Mesh(OBJECT_LOG *MyLog);
void Mesh_Group_Landmarks(OBJECT_LOG *MyLog, int landmark_mode);

void Mesh_DrawObj(int mode, bool bool_change_pos_obj,bool bool_vbo);
void get_screen_projection_matrix( glMatrix screenproj);
void get_world_coordinates_matrix(glMatrix mat);
void Mesh_invert(int select_mode);
void Mesh_invert_normals(int select_mode);
void Mesh_Mirror_ThroughY();
void Mesh_area_volume();

		
void SetLandmarkSize(float landmark_size);
int  Get_Landmark_Number(int landmark_mode);				
int  Get_Landmark_Selected( int landmark_mode);
OBJECT_LANDMARK* Get_Selected_Landmark(int landmark_mode);
OBJECT_LANDMARK* LandmarkAfter(int afterind, int landmark_mode);
OBJECT_LANDMARK* LandmarkBefore(int beforeind, int landmark_mode);
void Hierarchy(int depth);
void get_object_names_and_view_values(std::vector<std::string>& object_names, std::vector<int>& object_view_values);
int Mesh_Select (float xmin, float xmax, float ymin, float ymax, int select_mode);

void Add_ObjectLog(OBJECT_LOG * New_Obj);
void Add_ObjectMesh(OBJECT_MESH * New_Obj);
void Add_ObjectLandmark(OBJECT_LANDMARK * New_Obj, int landmark_mode);
OBJECT_LANDMARK * Mesh_CreateLandmarkAtMouse(float x, float y);
void rollinit_objects();
void Set_Blend(int blend_value);


OBJECT_LOG();

virtual ~OBJECT_LOG();//Destructor
};
#endif