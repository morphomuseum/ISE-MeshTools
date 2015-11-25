#ifndef OBJECT_LANDMARK_H
#define OBJECT_LANDMARK_H 46
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
//#include "Struct.h"
//#ifndef STRUCT_H

//#endif 
class OBJECT_LANDMARK : public OBJECT
	// Structure For An Object
{
public:
int  selected; 
float flag_length; // flag length;
int curve_start;
GLfloat color[4];

std::string flag_label;
int is_flag; // flag or normal landmark
 
 
 /*
 long  *tr;
 float *trn;	    // pointer to triangle normals 
 float *ve;
		    
 
  long  *tr2;
 float *trn2;	    // pointer to triangle normals 
 float *ve2;
		    */
 vtkPolyData *Sphere;
 vtkPolyData *Needle;
//OBJECT_MESH *My_Obj;

int  landmark_index; // The current index in the landmark list of this object
				  //-1 if not a landmark!
 long numtri; // number of triangles of this object...
 float mean[3];
 int numvert;

 long numtri2; // number of triangles of this object...
 float mean2[3];
 int numvert2;

 OBJECT_LANDMARK *nextobj;
OBJECT_LANDMARK *prevobj;
void Add_Landmark(OBJECT_LANDMARK * New_Obj);
 
 int Is_Curve_Start();
 void Curve_start(int direction);
 void Landmark_init_Mat(float landmark_size);
void Landmark_init_Mat_Landmark(float landmark_size, int landmark_mode);
void Calculate_GlobalMean (float *meanx, float *meany, float *meanz,  int *nb, int only_selected);														
 void Landmark_DrawObj(int level, int mode);
void Hierarchy (int depth);
void get_screen_projection_matrix( glMatrix screenproj);
void get_world_coordinates_matrix(glMatrix mat);
void get_world_coordinates_matrix2(glMatrix mat);
void Landmark_Mirror_ThroughY();
/*void Landmark_drawSphere(VERTEX c,double r,int n);
void Landmark_drawSphere2 (VERTEX c, double r, int n);*/
void Landmark_Unselect();
void Landmark_Select();
int Landmark_Select(float xmin, float xmax, float ymin, float ymax);
void SetLandmarkSize(float landmark_size, int landmark_mode);
OBJECT_LANDMARK();
~OBJECT_LANDMARK();
};
#endif