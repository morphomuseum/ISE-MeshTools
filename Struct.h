
//#include "stdafx.h"
#ifndef STRUCT_H
#define STRUCT_H

//#ifndef OBJECT_H
	#include "Struct2.h"
//	#include "Object.h"
//#endif
#include <stdlib.h>
#include <math.h>										// Math Library Header File
#include <stdio.h>
#include "font.h"
#include "CDataFile.h"

#include <FL/gl.h>
#include <FL/Fl_File_Chooser.H>





#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define ShowUpvector
typedef enum { NOTALLOWED, MOUNTAIN, TREE, ISLAND, BIGMTN, STEM, LEAF, 
               MOUNTAIN_MAT, WATER_MAT, LEAF_MAT, TREE_MAT, STEMANDLEAVES,
               AXES } DisplayLists;
static double PI = 3.1415926535897932384626;
static  GLfloat mtn_ambuse[] =   { (float)0.426, (float)0.256, (float)0.108, (float)1.0 };
static GLfloat mtn_specular[] = { (float)0.394, (float)0.272, (float)0.167, (float)1.0 };
static  GLfloat mtn_shininess[] = { (float)100 };

static  GLfloat bone_ambuse[] =   { (float)0.426, (float)0.426, (float)0.426, (float)1.0 };
static GLfloat bone_specular[] = { (float)0.394, (float)0.394, (float)0.394, (float)1.0 };
static  GLfloat bone_shininess[] = { (float)100 };


static GLfloat water_ambuse[] =   { (float)0.0, (float)0.1, (float)0.5, (float)1.0 };
static GLfloat water_specular[] = { (float)0.0, (float)0.1, (float)0.5, (float)1.0 };
static GLfloat water_shininess[] = { (float)10 };

static GLfloat tree_ambuse[] =   { (float)0.4, (float)0.25, (float)0.1, (float)1.0 };
static GLfloat tree_specular[] = { (float)0.0, (float)0.0, (float)0.0, (float)1.0 };
static GLfloat tree_shininess[] = { (float)0 };

static GLfloat leaf_ambuse[] =   { (float)0.0, (float)0.8, (float)0.0, (float)1.0 };
static GLfloat leaf_specular[] = { (float)0.0, (float)0.8, (float)0.0, (float)1.0 };
static GLfloat leaf_shininess[] = { (float)10 };


static GLfloat blanc[] = { 1.0f,1.0f,1.0f,1.0f };
static GLfloat jaune[] = { 1.0f,1.0f,0.0f,1.0f };
static GLfloat orange[] = { 1.0f,0.4f,0.1f,1.0f };
static GLfloat cyan[] = { 0.0f,1.0f,1.0f,1.0f };
static GLfloat magenta[] = { 1.0f,0.0f,1.0f,1.0f };
static GLfloat rouge[] = { 1.0f,0.0f,0.0f,1.0f };
static GLfloat rouge1[] = { 1.0f,0.8f,0.8f,1.0f };
static GLfloat rouge2[] = { 1.0f,0.6f,0.6f,1.0f };
static GLfloat rouge3[] = { 1.0f,0.4f,0.4f,1.0f };
static GLfloat rouge4[] = { 1.0f,0.2f,0.2f,1.0f };
static GLfloat rouge5[] = { 1.0f,0.0f,0.0f,1.0f };
static GLfloat rouge6[] = { 0.8f,0.0f,0.0f,1.0f };
static GLfloat rouge7[] = { 0.6f,0.0f,0.0f,1.0f };
static GLfloat rouge8[] = { 0.4f,0.0f,0.0f,1.0f };
static GLfloat rouge9[] = { 0.2f,0.0f,0.0f,1.0f };

static GLfloat vert[] = { 0.0f,1.0f,0.0f,1.0f };
static GLfloat vert1[] = { 0.8f,1.0f,0.8f,1.0f };
static GLfloat vert2[] = { 0.6f,1.0f,0.6f,1.0f };
static GLfloat vert3[] = { 0.4f,1.0f,0.4f,1.0f };
static GLfloat vert4[] = { 0.2f,1.0f,0.2f,1.0f };
static GLfloat vert5[] = { 0.0f,1.0f,0.0f,1.0f };
static GLfloat vert6[] = { 0.0f,0.8f,0.0f,1.0f };
static GLfloat vert7[] = { 0.0f,0.6f,0.0f,1.0f };
static GLfloat vert8[] = { 0.0f,0.4f,0.0f,1.0f };
static GLfloat vert9[] = { 0.0f,0.2f,0.0f,1.0f };
static GLfloat bleu[] = { 0.0f,0.0f,1.0f,1.0f };
static GLfloat noir[] = { 0.0f,0.0f,0.0f,1.0f };
static GLfloat grisTresFonce[] = { 0.1f,0.1f,0.1f,1.0f };
static GLfloat grisFonce[] = { 0.25f,0.25f,0.25f,1.0f };
static GLfloat grisMoyen[] = { 0.5f,0.5f,0.5f,1.0f };
static GLfloat grisClair[] = { 0.75f,0.75f,0.75f,1.0f };
static GLfloat jauneClair[] = { 0.75f,0.75f,0.0f,1.0f };
static GLfloat jauneFonce[] = { 0.5f,0.5f,0.0f,1.0f };
static GLfloat cyanFonce[] = { 0.0f,0.5f,0.5f,1.0f };
static GLfloat magentaFonce[] = { 0.5f,0.0f,0.5f,1.0f };
static GLfloat rougeFonce[] = { 0.5f,0.0f,0.0f,1.0f };
static GLfloat vertFonce[] = { 0.0f,0.5f,0.0f,1.0f };
static GLfloat bleuFonce[] = { 0.0f,0.0f,0.5f,1.0f };
static GLfloat brun[] = { 0.8f,0.6f,0.4f,1.0f };
static GLfloat rose[] = { 1.0f,0.65f,0.65f,1.0f };
static GLfloat roseFonce[] = { 0.6f,0.25f,0.25f,1.0f };
static GLfloat bleuCiel[] = { 0.5f,0.5f,1.0f,1.0f };
static GLfloat bleuCielFonce[] = { 0.25f,0.25f,0.5f,1.0f };

  static GLfloat no_mat[] = { 0.0F,0.0F,0.0F,1.0F };
  static GLfloat mat_ambient[] = { 0.7F,0.7F,0.7F,1.0F };
  static GLfloat mat_ambient_color[] = { 0.8F,0.8F,0.2F,1.0F };
  static GLfloat mat_diffuse[] = { 0.1F,0.5F,0.8F,1.0F };
  static GLfloat mat_specular[] = { 1.0F,1.0F,1.0F,1.0F };
  static GLfloat no_shininess[] = { 0.0F };
  static GLfloat low_shininess[] = { 5.0F };
  static GLfloat high_shininess[] = { 100.0F };
  static GLfloat mat_emission[] = {0.3F,0.2F,0.2F,0.0F};

/////////////////////////////////////////////////////////////////////////////
// CMainWnd
// Encapsulates the main window
//
typedef struct _CameraT {
public :  
double zoom;
  double el;
  double az;
  double tw;
  double tx;
  double ty;
  double tz;
  float atx;
  float aty;
  float atz;
  double far1;
  double near1;
} CameraT;

//float poupoune;
extern	 _CameraT camera;				// Structure about camera setting
extern float rotx, roty, rotz, rotx2, roty2, rotz2,tx,ty,tz, tx2, ty2, tz2, g_zoom; 
extern int dispmode;
extern int cpt_dispmode4and5;
extern float g_landmark_size;
extern int g_display_all;
extern int g_grid_plane;
extern int g_landmark_type;
extern int g_tag_mode;
extern int g_tag_tool;
extern int g_magic_wand_extension;
//extern double PI;/// a   enle
extern double g_magic_wand_extension_min_cos;
extern int g_magic_wand_over;
extern int g_pencil_extension;
extern int g_move_cm;
extern float g_flag_length;
extern int g_fov_adapt;
extern int g_sc_show_below_min;
extern int g_sc_show_above_max;
extern int g_auto_zoom;
extern std::string g_tag_labels[25];
extern std::string g_orientation_labels[6];
extern int g_color_scale_id;
extern GLfloat g_tag_colors[25][4];	
extern int g_active_tag;
extern int g_tag_extraction_criterion_all;
extern int g_mode_tool;
extern int g_auto_delete;
extern int g_enable_2sides;
extern std::vector<std::string> g_scalar_list;
extern std::vector<std::string> g_selected_names;
extern std::vector<std::string> g_distinct_selected_names;

extern int g_active_scalar;
extern std::vector<std::string> g_scalar_list_selected;
extern GLfloat color_obj[4];
extern GLfloat g_flag_color[4];
extern POLYGON_LIST poly;
extern int landmark_mode;
extern float g_lambda; // mean of selected objects

extern float g_mean[3];
extern int g_nmean;
extern char Version[50];

extern int Disp_Vertices_Ids;
extern int Disp_Triangle_Ids;
extern int Disp_Scalars_Mode;
extern int Disp_Tags_Mode;
extern float Min_Color;
extern float Max_Color;






struct tri_depth{
	float d; //depth
	long tn; //original triangle nr	
};


struct scalar_q{
	float v; //depth
	long n; //original vertex or triangle nr	
};



//USED in sorting function when stl file is loaded.
struct vertex {
  float x,y,z;	// coordinates
  long tn;	// triangle nr. to which this vertex belongs
  int vn;	// vertex nunber triangle (1,2,or 3)
  int vi;	// vertex index in ve list!
};


typedef tri_depth* tri_vptr;


typedef scalar_q* sc_vptr;
	
typedef vertex* vertexpointer;
//itoa has to be define for gcc
char* itoa( int value, char* result, int base );
char* itoa(int val, int base);
char*   ftoa( double f, double sigfigs );
int  vertex_compare (const void* v1, const void* v2);
int tri_depth_compare(const void* v1, const void* v2);
int scalar_compare(const void* v1, const void* v2);
struct landmark_list
{
	int obj_p;	    //landmark index in container
	int obj_n;	//index object in container	
};
typedef landmark_list* landmark_listptr;
int  landmark_list_compare (const void* v1, const void* v2);
void readstr(FILE *f,char *string); //Fonction globale...

void get_projection_matrix(glMatrix mat);
void get_camera_transform_matrix (glMatrix cam);
void get_camera_transform_matrix_no_translation (glMatrix cam);
void readInt4bs(long *my_int, FILE* filein);
void readFloat4b(float *my_float, FILE* filein);
	//void get_camera_transform_matrix2 (glMatrix cam);

void ConvertScalarToColor(float value, GLfloat color[4], float alpha);
void getmatrix(glMatrix Mat);

double AngleBetweenTwoVectors(double v1[3], double v2[3]);
float sqr(float x);
float dist2(const double *x, const double *y);

void ApplyTransformation(float input[3],float result[3], glMatrix mat);
void ApplyInverseTransformation(float input[3],float result[3], glMatrix mat);
void ApplyRotation(float input[3],float result[3], glMatrix mat);

bool IsEqual(double *point, double *point1, int size = 3);
bool IsEqual(GLfloat*point, GLfloat *point1, int size=4);
void affiche(double * point, int dim);
void affiche(GLfloat* point, int dim);

#endif
