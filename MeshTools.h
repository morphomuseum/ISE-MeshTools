//
// "$Id: MeshTools.h,v 1.4.2.4.2.3 2004/04/11 04:39:00 easysw Exp $"
//
// MeshTools class definitions 
//

#ifndef MeshTools_H

#define MeshTools_H 1
#include "config.h"
#include <string.h>
#include <vtkPolyData.h>
#include "Struct.h"
#include "SpecialSlider3.h"
#include "Icp.h"
#include "Container_Mesh.h"
#include "SpecialSlider2.h"
#include "SpecialSlider4.h"
#include "CDataFile.h"
#include <FL/Fl.H>
#include <FL/Fl_Output.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Color_Chooser.H>


#define MODE_NOTHING -1
#define POS_FACE 1
#define POS_BACK 2
#define POS_BENEATH 3
#define POS_ABOVE 4
#define POS_LEFT 5
#define POS_RIGHT 6

#if HAVE_GL
#  include <FL/Fl_Gl_Window.H>
#  include <FL/gl.h>
#else
#  include <FL/Fl_Box.H>
#endif /* HAVE_GL */

#include <stdlib.h>
#include "font.h"
#include <FL/Fl_File_Chooser.H>
#include <string>
#include <sstream>
#include <iostream>

typedef struct _MouseT {
  int button;
  int x;
  int y;
} MouseT;



	//extern _CameraT camera;


#if HAVE_GL
class MeshTools : public Fl_Gl_Window {
#else
class MeshTools : public Fl_Box {
#endif /* HAVE_GL */



public:
    // this value determines the scaling factor used to draw the cube.
	
	// texture stuff
	

	//_CameraT camera;
	int landmark_mode;
	int disp_cull_face ;
	int disp_curve;
	float grid_size;
	int blend_value;
	int position;
	int mode;
	
	//POLYGON_LIST poly;	
	GLfloat light_position[4];
		
    double size;
	 _MouseT mouse;				// Keeping track of mouse input 
	
	 
	 ///-----------------ICP------------///
	 ICP *registration;
	 bool bool_ICP = false;
	 vtkSmartPointer<vtkFloatArray> loaded_parameter_list;

	 ///-----------AFFICHAGE VBO--------///
	 bool bool_initGlew;
	 bool bool_change_mode;
	 bool bool_change_pos_obj;
	 bool bool_vbo;
	 //----------------//


    MeshTools(int x,int y,int w,int h,const char *l=0);

    /* Set the rotation about the vertical (y ) axis.
     *
     * This function is called by the horizontal roller in MeshToolsUI and the
     * initialize button in MeshToolsUI.
     */
  
    /* Set the rotation about the horizontal (x ) axis.
     *
     * This function is called by the vertical roller in MeshToolsUI and the
     * initialize button in MeshToolsUI.
     */

  


    /* Sets the x shift of the cube view camera.
     *
     * This function is called by the slider in MeshToolsUI and the
     * initialize button in MeshToolsUI.
     */
	void Update_RGB();
	int Get_Active_Scalar();
	int Get_Display_All();
	int Get_mode_cam_centre_of_mass();	
	void Set_Display_All(int all);
	void Remove_Scalar(int scalar);
	void Initialize_Scalar(int scalar);
	void Set_Active_Scalar(int active_scalar);
	void Set_Active_Tag(int active_tag);
	int Get_Active_Tag();
	float scalars_get_min();
	float scalars_get_max();
	void Cam_Centre_At_Landmark (int landmark_number);
	float Get_Grid_Size();
	void Set_Grid_Size(float size);
	int Get_Blend();
	void Set_Blend(int blend_value);
	float getTranslationFactor();
	int set_matrix(float m1[16], float m2[16],int mode);
	std::string get_matrix(float m1[16], float m2[16]);
	std::string get_name_of_first_selected_obj();
	void Change_Disp_Vertices_Ids();
	void Change_Scalars_Display_Mode();
	void Set_Pencil_Extension_Level(int extension_level);
	int Get_Pencil_Extension_Level();
	void Set_Grid_Plane(int grid_plane);
	void Set_Magic_Wand_Extension_Level(int extension_level);
	void Set_Magic_Wand_Override(int over);
	int Get_Magic_Wand_Extension_Level();
	void Activate_Pencil();
	void Activate_Magic_Wand();
	void Activate_Flood_Fill();


	void Activate_Scalars_Display_Mode();
	void Activate_Tags_Display_Mode();
	void Change_Tags_Display_Mode();
	void Desactivate_Tags_Display_Mode();
	void Desactivate_Scalars_Display_Mode();
	void Set_Tag_Mode (int tag_mode);
	void Mesh_Tag(int x, int y, int mode);
	void Merge_Tags(int source, int target);
	void Mesh_Tag_Connected_Regions();
	void Mesh_Decompose_Tag(int min_region_size, Fl_Double_Window* myWindow);
	void Mesh_Delete_All_Non_Zero_Tags();
	void Mesh_Delete_Tag (int tag_value);
	void Mesh_Delete_Active_Tag();
	void Mesh_Extract_Tag (int tag_value);
	void Mesh_Extract_Active_Tag();
	void Set_Tag_Extraction_Criterion(int all);
	void Mesh_Extract_Scalar_Range(float scalar_min, float scalar_max);
	void Mesh_Decompose(int min_region_size,Fl_Double_Window* decomposeWindow);
	void Mesh_Largest_Region();
	
	void Change_Disp_Triangle_Ids();
	void color_setobjcolor (uchar r, uchar g, uchar b);
	
	void color_setbackcolor (uchar r, uchar g, uchar b);
	void color_setgridcolor (uchar r, uchar g, uchar b);
	void set_tag_color (int tag_id, uchar r, uchar g, uchar b);
	
	void ChangeLandmarkMode();
	void color_getgridcolor (uchar *r, uchar *g, uchar *b);	
	void color_getbackcolor (uchar *r, uchar *g, uchar *b);
	void color_getobjcolor (uchar *r, uchar *g, uchar *b);
	void get_tag_color (int tag_id, uchar *r, uchar *g, uchar *b);
	std::string get_tag_label (int tag_id);
	std::string get_flag ();
	float get_flag_length ();
	float get_g_flag_length ();
	void set_all_flag (uchar r, uchar g, uchar b, float length);
	void set_flag ( std::string, float length);
	void set_flag_colour (uchar r, uchar g, uchar b);
	void set_g_flag_colour (uchar r, uchar g, uchar b);
	void set_flag_length (float length);
	void set_g_flag_length (float length);
	void get_flag_color (uchar *r, uchar *g, uchar *b);
	void get_g_flag_color (uchar *r, uchar *g, uchar *b);

	int set_name(std::string label, int mode);
	void set_tag_label (int tag_id, std::string label);
void get_orientation_labels (           std::string &z_plus,
									   std::string &z_minus,
									   std::string &y_plus,
									   std::string &y_minus,
									   std::string &x_plus,
									   std::string &x_minus
									   );


void set_orientation_labels (std::string z_plus,
									   std::string z_minus,
									   std::string y_plus,
									   std::string y_minus,
									   std::string x_plus,
									   std::string x_minus
									   );

	void set_tag_alpha (int tag_id, int val);
	int get_tag_alpha (int tag_id);
	float GetSpecular ();
	float GetAmbiant();
	float GetDiffuse();
	int Move_Selected_Handles (float movementintensity);
	void SetAmbiant (float a);
	void SetDiffuse (float d);
	void SetSpecular (float s);
	void Set_sc_min (float value);
	void Set_sc_max (float value);
	void SC_Show_Below_Min(int value);
	void SC_Show_Above_Max(int value);
	float Get_sc_min ();
	float Get_sc_max ();
void SC_calc_depth();
int selected_file_exists(std::string path, std::string ext, std::string postfix);
void SC_calc_curvature(int mode);
void SC_calc_mean();
void SC_calc_thickness(float thickness_max_distance, Fl_Double_Window* thicknessWindow);
void SC_calc_thickness_between_objects(float thickness_max_distance, 
									   int source, 
									   int target,
									   Fl_Double_Window* thicknessWindow2);
	vtkSmartPointer<vtkFloatArray> GetErrors_ICP();
	void Registration(int Source, int Target, vtkFloatArray *tabOfWeights, vtkFloatArray* tab_loadsave_parameter, vtkFloatArray *tabDisplay, int modeTransformation, bool bool_onlyMatching);
	CONTAINER_MESH getContainer(){ return Cont_Mesh; }
	void Compute_curv_ICP(OBJECT_MESH* My_Source, OBJECT_MESH*My_Target);
	void CheckingName(string *name_obj, int cpt_name = 0);
	void SetDataICP(vtkSmartPointer<vtkFloatArray> tabOfWeights, vtkFloatArray* tab_loadsave_parameter, int modeTransformation, vtkSmartPointer<vtkFloatArray> tabDisplay, bool bool_only_matchings);
	void SetDisplayMatching(vtkSmartPointer<vtkFloatArray> tabDisplay);
	void SetVerticeColor(float vect_color[3]);
	void SaveErrors();
	void Save_Errors_ICP(vtkFloatArray* errors);
	void Save_STV_File();
	void Write_STV(string filename, int source_landmarks_number, int target_landmarks_number);
	void Open_STV_File();
	void Open_STV_File(std::string filename);
	void Save_original_source_verticeICP();
	void SaveParameter();
	void Open_ICP_parameter();
	void Save_ICP_parameter(vtkFloatArray* parameter_list);
	vtkSmartPointer<vtkFloatArray> GetParameterList();
	void Mesh_draw_ICPMode_info(int x, int y);
	void Active_InitRGB();
	void VBO_activated(bool bool_vbo);
	float get_objects_centre_of_mass(int xyz);
	void FileCurv(OBJECT_MESH * object);
	void Adjust_landmark_rendering_size();
void Mesh_TPS( int basis, float percentage);
void Compute_Global_Mean(int only_selected = 1);
void Compute_Name_Lists();
void Compute_Global_Scalar_List();

void Print_Global_Scalar_List();
void Print_Selected_Names_List();
void Print_Distinct_Selected_Names_List();
std::vector<std::string> Get_Scalar_List();
std::vector<std::string> Get_Selected_Names_List();
void Mesh_Decimate(int percentage, int method);
void Mesh_Densify(int subdivision_nr);
void Mesh_Fill_Holes(int holes_size);
    void Mesh_UnselectAll();
	void Mesh_SelectAll();
    int PrintMACaddress(unsigned char MACData[]);
	
float Get_100pixels_in_mm();

	void Set_100pixels_in_mm (float val);
    char* ftoa( double f, double sigfigs );
	void Delete();
	void Delete_smaller_than (int numtri);
void Delete_volume_smaller_than(float volume);
	int Mesh_Delete();
	void Mesh_DeleteSmallVolume(float volume);
	void Mesh_DeleteSmall(int numtri);
	void Mesh_area_volume();
	void Hierachy();
	void get_object_names_and_view_values(std::vector<std::string>& object_names, std::vector<int>& object_view_values);
	void set_object_view_values( std::vector<int>&object_new_view_values);
	
	void Mesh_draw_landmark_infos(int x, int y);
	void Draw_Warning_Invertion(int x, int y);
	void Mesh_Tri_Sort(int side);

	char* GetVersion();
	
	void rollx(float x);
	void rolly(float y);
	void rollz(float z);
	void rollx2(float x);
	void rolly2(float y);
	void rollz2(float z);
	void rollinit_camera();
	void rollinit_objects();
    void panx(float x){tx=x;};
	void panx2(float x);
    /* Sets the y shift of the cube view camera.
     *
     * This function is called by the slider in MeshToolsUI and the
     * initialize button in MeshToolsUI.
     */
    void pany(float y)
	{
		ty= -y;
	};
	void pany2(float y);
	 void panz(float z);
	
	   void panz2(float z);

void Ungroup();
void Group();
float Get_Optimal_FOV_Depth();
	 
	 

	

void writeFloat4b(float value, FILE *filein)

{	   
	  char vc[4], *c;
	  union { float f;
	  char c[4];
      } u;
	  u.f= value; 
	  c = vc;	  
	  for (int k=3;k>=0;k--)
	  {*c++ = u.c[k];}
	  fwrite(vc, sizeof(vc[0]),4,filein);

};
	void cam_validate(double near1, double far1, double az, double el, double tw, double tx, double ty, double tz);
	double cam_getnear();
	double cam_getfar();
	double cam_gettx();
	double cam_getty();
	double cam_gettz();
	double cam_getaz();
	double cam_getel();
	double cam_gettw();

	;
	void Mesh_Select(int x1, int x2, int y1, int y2, int select_mode);
	void Mesh_Select(int x, int y);
	void Mesh_MoveLandmarkAtMouse(int x, int y);
	void Mesh_CreateLandmarkAtMouse(int x, int y);
	void Mesh_Add_landmark();
	
	void Mesh_invert();
	void Mesh_invert_normals();
	void Mesh_Smooth (int iterationnumber, double relaxation_factor);
	void Mesh_Mirror_ThroughY();
	void Mesh_Draw_Polygon();
	void AfficheTexte(int x, int y, char *chaine,  GLfloat couleur[4] );
	void AfficheRectangle(int x1, int x2, int y1, int y2,  GLfloat couleur[4]);
	//void AfficheRectangle2(int x1, int x2, int y1, int y2,  GLfloat couleur[4],GLdouble width, GLdouble height );
	void AfficheRectangle2(int x1, int x2, int y1, int y2,  GLfloat couleur[4]);
	
	void resize(int x, int y, int w, int h);


#if HAVE_GL
    /*The widget class draw() override.
     *
     *The draw() function initialize Gl for another round o f drawing
     * then calls specialized functions for drawing each of the
     * entities displayed in the cube view.
     *
     */

	
  
	void drawDispMode();
    void draw();
	void Mesh_draw_Obj(int mode, bool bool_change_pos_obj);
	void Mesh_draw_Curve();
    void DrawColorScale (int x, int y, int w1, int h1);
	//void drawColorScale2 (int x, int y, int w1, int h1);
	
	void DrawOrientations(int x, int y, float size);
	//void drawSphere(VERTEX c,double r,int n);
	void Change_Cull_Face();
	
	int handle(int event); 

#endif /* HAVE_GL */
	 void Open_Mesh_File();  
	std::string correct_filename(std::string init_filename);
void Reset_Camera();
void Set_far(float farv);
void Convert_RGB_To_Tags(int exact);
void Mesh_Tag_Within_Lasso(int x, int y, int lasso_x, int lasso_y, int mode);
void Fill_Holes (int tag_id, int max_size,  Fl_Double_Window* fillholesWindow2);

void Open_Landmarks(int landmark_mode);
void Open_VER_File(int landmark_mode, std::string filename);
void create_landmarks(int landmark_mode, vtkFloatArray* param_list, int type);
void Open_TAG_File();
void Open_TAG_File(std::string filename);
void Open_ORI_File();
void Open_ORI_File(std::string filename);
void Open_FLG_File();
void Open_FLG_File(std::string filename);
void Open_CUR_File(); 
void Open_CUR_File(std::string filename);
void Selected_Landmarks_Change_Orientation();

void Open_POS_File(std::string filename,OBJECT_MESH * My_Obj);
void Open_POS_File();
void Open_POS_File_Inv();
//void Open_STL_File2(); 
void Open_NTW_File();

void batch (int argc, char **argv);


void Landmark_Move_Down();
void Landmark_Move_Up();

void Object_Move_Down();
void Object_Move_Up();
int Check_Selected_Object_Names();


void Change_Draw_Curve ();
void Curve_start(int direction);
void Stick_Selected_Landmarks_On_Surfaces();
void Save_NTW_File();
void save_ini_param();
void initialize_ini_file();
int Save_POS_File();
void Save_Infos();
int Save_POS_File(OBJECT_MESH *My_Obj, std::string filename);

void Save_Mesh_File(int type, int mode, int file_type, int save_norms =0) ;

void Save_Mesh_File(OBJECT_MESH *My_Obj, std::string filename, int file_type, int save_norms);
int Save_CUR_LMK_File(int file_type,int decimation);
int Save_CUR_Infos();
int Save_Landmarks(int file_type, int landmark_mode, int only_selected);
void Select_Landmark(int landmark_mode, int lmk_ind);
void Select_Landmark_Range(int landmark_mode, int lmk_ind_start, int lmk_ind_end);
int Save_FLG_File();
int Save_TAG_File();
int Save_ORI_File();
int Save_CUR_File();
void Mesh_Set_Select_Mode(int Mode);
void Landmark_Mode(int mode);
void SetPosition(int pos);
void SetDisplayMode(int mode);
void SetMode(int mode);
void SetModeTool(int mode);
void SetLightPosition(int pos);
void SetLightPosition2();
void lightroll1(float pos);
void lightroll2(float pos);
void SetZoom(float zoom);
float GetZoom();
void Mesh_SetLandmarkSize(float landmark_size2);
float Mesh_GetLandmarkSize();
void set_g_landmark_auto_rendering_size(int render_mode);
int get_g_landmark_auto_rendering_size();


void Set_fov_adapt(int fov_adapt);
int Get_fov_adapt();
int Mesh_GetLandmarkType();
void Mesh_SetLandmarkType(int landmark_type2);

void  Mesh_SetOpeningBehaviour (int move_cm);

void Mesh_SetZoomBehaviour (int auto_zoom);

void  SetColorScaleId (int scale_id);
int GetColorScaleId();

void Mesh_SetAutoDelete (int auto_delete);
int Mesh_GetAutoDelete();

int  Mesh_GetOpeningBehaviour ();
int  Mesh_GetZoomBehaviour ();

void  Mesh_Set2SidesLightning (int sides);
int  Mesh_Get2SidesLightning ();

void  ShowGrid();
void  ShowOrientation();
bool GetPointNormals(vtkPolyData* polydata);
bool GetCellNormals(vtkPolyData* polydata);
void TestCellNormals(vtkPolyData* polydata);
void TestPointNormals(vtkPolyData* polydata);



private:
	
    /*  Draw the cube boundaries
     *
     *Draw the faces of the cube using the boxv[] vertices, using
     * GL_LINE_LOOP for the faces. The color is \#defined by CUBECOLOR.
     */




#if HAVE_GL
	    	
	void drawGrid();
#else
#endif /* HAVE_GL */
	float landmarkx, landmarky, landmarkz;
    float vAng,hAng, zAng;
	double zoom;
	unsigned baseList;
    float xshift,yshift, zshift;
	int id_affiche;
	int affiche_num;
	int affiche_triangles;
	int bcouleur;
	char		texte[256];
	int affiche_norm_triangles;
	int affiche_norm_points;
	int vertrouge, showgrid, modelandmark, showorientation;//, dispmode;
	int couleur_mode;
	float axerot[3];
	int axeangle;
	int select_mode;
	int kpressed, kpressed2;
	int affiche_rect;
	int rect_x1;
	int rect_x2;
	int rect_y1;
	int rect_y2;
	
	CONTAINER_MESH 	Cont_Mesh;
	
	//char VER_Sp_File[256];
	//char POS_Sp_File[256];
	
	
    float boxv0[3];float boxv1[3];
    float boxv2[3];float boxv3[3];
    float boxv4[3];float boxv5[3];
    float boxv6[3];float boxv7[3];
	int color_surf_range_min;
	int color_surf_range_max;
	float color_distnorm_range_min;
	float color_distnorm_range_max;
	GLfloat color_grid[4];
	GLfloat color_arrow[4];
	float lightpos1;
	float lightpos2;
	float ambiant;
	float specular;
	float diffuse;

	GLfloat color_back[4];
//	GLfloat color_obj[4];
	GLfloat color_min[4];
	GLfloat color_max[4];
	GLfloat color_min2[4];
	GLfloat color_max2[4];
	GLfloat color_scale_min[9][4];
	GLfloat color_scale_max[9][4];
	GLfloat color_scale_min2[9][4];
	GLfloat color_scale_max2[9][4];
	float color_arrow_size;
	
	
};
#endif//
// End of "$Id: MeshTools.h,v 1.4.2.4.2.3 2004/04/11 04:39:00 easysw Exp $".
//
