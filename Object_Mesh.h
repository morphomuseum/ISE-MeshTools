#ifndef OBJECT_MESH_H
#define OBJECT_MESH_H 4908
#include "StdAfx.h"
#include <stdlib.h>
#include <math.h>										// Math Library Header File
#include <stdio.h>
#include <string.h>
#include "CDataFile.h"
//#include "Struct2.h"
#include "Struct.h"
#include "Object.h"
#include <vtkPolyDataNormals.h>
#include <vtkSmartPointer.h>

#include <set>
#include <FL/gl.h>
#include <FL/Fl_File_Chooser.H>
//#include "kdtree.h"
#include "Object_Landmark.h"
//#include "Struct.h"
//#ifndef STRUCT_H

#include <list>



//#endif 
class OBJECT_MESH : public OBJECT
	// Structure For An Object
{
public:

	///////////////////////////////////////  ATRIBUTES //////////////////////////

	//static OBJECT_MESH *New();
	int  selected; // Flag set to 1 if the current object is selected.
	int view;

	GLfloat color[4];
	GLfloat prevColor[4];

	// data partitionning stuff!
	int pt_done; // partitionning is done
	vtkIdType pt_cube_nr[3]; // number of cubes in x, y, z
	long pt_total_cube_nr; // total number of cubes in partition

	vtkIdType *pt_vertices_per_cube; // for a given cube, how many vertices.
	vtkIdType  ** pt_cubes; // actual lists of vertices ids in each cube

	vtkIdType *pt_vertices_per_cube_compare; // for a given cube, how many vertices are in the 27 adjacent cubes.
	vtkIdType  ** pt_cubes_compare; // actual lists of vertices ids to compare for a given cube : that is for a given
	// cube, we will have the vertex indices of the vertices of the 27 adjacent cubes

	//-------variable pour l'ICP-----//
	bool bool_Source_used_ICP = false;
	bool bool_Target_used_ICP = false;
	float mean_arr = 0;//moyenne des longueur des arètes
	set<int>* set_neighborbhood;// attribut

	vtkSmartPointer<vtkFloatArray> modifTab;// tableau indiquant les modifications apparues qur l'objet pour l'affichage
	bool bool_changed_tags_color = 0;
	bool bool_changed_init_color = 0;
	bool bool_changed_matrix = 0;
	bool changement=0;//pour le VBO
	vtkSmartPointer<vtkUnsignedCharArray> colors;//pour le VBO
	vtkSmartPointer<vtkUnsignedCharArray> colorT;//tableau de color tampon pour le VBO
	GLfloat new_color[3];//pour affichage

	float object_radius;// rayon de l'objet
	float dmean;

	int *tr_sort;
	float blend;
	GLuint texture;

	//Test version 10
	int *draw_tab_indice;
	float *draw_color;

		OBJECT_MESH *nextobj;
		OBJECT_MESH *prevobj;
		//Test version 10
		int is_logical; // Only 1 if Group function is used
					//-1 if not a landmark!

		int numtri; // number of triangles of this object...
		float mean[3];
		int numvert;


		VERTEX2 c;
		double r = (double)1 / 3;
		float uvx[3], uvy[3], uvz[3];
		float newmean1[3], newmean[3];

	/////-------------AFFICHAGE----VBO--------------////
		int N_VERTS;//nombre de points
		int D_SIZE;//dimension des points
		int C_SIZE;//dimension de la couleur
		int T_TRI;//nombre de triangle

		int bool_init_buf;
		
		GLfloat *ObjectInfoArray;
		GLuint *ObjectIndiceArray;
		GLuint size_indice = 0;
		GLuint size_info = 0;

		GLuint buf_vertex, buf_indice;         /* identifiant de notre objet tampon */
		/////////----------------------------

	 ///////////////////////////////////// FUNCTIONS ///////////////////////////:

		static void init();
		void Set_Blend(int blend_value);
		std::string Get_Active_Scalar_Name();
		std::string Get_Scalar_Name(int scalar);

		int mround(float mnum);
		void Update_RGB();
		void Mesh_init_Mat();
		vtkSmartPointer<vtkIdList> GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, double *vn, double sc, vtkIdType id, 
				int tool_mode);
		void GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, double *vn, double sc,vtkSmartPointer<vtkIdList> ptList,
		vtkSmartPointer<vtkIdList> nptList, vtkSmartPointer<vtkIdList> exnList, vtkSmartPointer<vtkIdList> oldList, 
		vtkSmartPointer<vtkIdList> veryoldList,int *list_changed);
		void Partition_Data (float xmin, float ymin, float zmin, float xmax, float ymax, float zmax, float pt_dist);
		void Partition_Data(float pt_dist);
		void Partition_Data_Delete();
		// Combined with lasso
		vtkSmartPointer<vtkIdList> GetConnectedVerticesLasso(vtkSmartPointer<vtkPolyData> mesh, double *vn, double sc, vtkIdType id, 
		int tool_mode, int lasso_x, int lasso_y, int h_screen, int w_screen);

		void GetConnectedVerticesLasso(vtkSmartPointer<vtkPolyData> mesh, double *vn, double sc,vtkSmartPointer<vtkIdList> ptList,
		vtkSmartPointer<vtkIdList> nptList, vtkSmartPointer<vtkIdList> exnList, vtkSmartPointer<vtkIdList> oldList, 
		vtkSmartPointer<vtkIdList> veryoldList,int *list_changed, int lasso_x, int lasso_y, int h_screen, int w_screen);


		//void GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, int seed, vtkSmartPointer<vtkIdList> connectedVertices);
		void Initialize_Scalar(int scalar);
		void Remove_Scalar(int scalar);
		void Initialize_Tags();
		vtkSmartPointer<vtkIdTypeArray> Mesh_Get_Tag_Region_Sizes();

		void Hierarchy (int depth);
		void get_object_names_and_view_values(std::vector<std::string>& object_names, std::vector<int>& object_view_values);
		void Set_Active_Scalar();
		void SC_calc_depth();
		void SC_calc_mean();
		void SC_calc_thickness(float thickness_max_distance, Fl_Double_Window* thicknessWindow);
		void get_screen_projection_matrix( glMatrix screenproj);
		void get_world_coordinates_matrix(glMatrix mat);
		void Mesh_invert(int select_mode);
		void Mesh_invert_normals(int select_mode);


		//void Mesh_drawSphere(VERTEX c,double r,int n);
		//void Mesh_drawSphere2 (VERTEX c, double r, int n);
		float get_volume();
		float get_area();
		void Mesh_area_volume( );
		void  Test_Scalar(std::string myScalar);
		void  Test_Scalar_Selected(std::string myScalar);
		void  Update_Distinct_Selected_Names(std::string Name);
		void Calculate_GlobalMean (float *meanx, float *meany, float *meanz, int *nb, int only_selected);
		void Calculate_Global_MinMax(float *minx, float *maxx, float *miny, float *maxy, float *minz, float *maxz);
		void Compute_Global_Scalar_List();
		void Compute_Name_Lists();
		int Mesh_Select (float xmin, float xmax, float ymin, float ymax);

		void Add_Object(OBJECT_MESH * New_Obj);

		int Already_Listed(vtkIdType ve, vtkSmartPointer<vtkIdList> tagged_ids);
		void Mesh_Unselect();
		void Mesh_Select();
		float scalars_get_min();
		float scalars_get_max(); 
		void Mesh_Tri_Sort(int side);
		void Mesh_Find_Closest_Vertex (float input[3], float *x, float *y, float *z,float *nx,float *ny,float *nz, vtkIdType *ve, int mode);
		/*float Mesh_Distance_To_Plane(VERTEX * My_Vertex, VERTEX * A, VERTEX * B, VERTEX *C);*/
		  
		OBJECT_MESH();
		OBJECT_MESH(const OBJECT_MESH & mobj);
		virtual ~OBJECT_MESH();//Destructor

		void Update(vtkPoints* Points);// Modifie les normales de this à partir de nouveaux points
		void Compute_mean();
		void Compute_radius();
		void CheckSameColor(OBJECT_MESH *object, bool *bool_exist_color, bool *bool_same_color);
		void Find_minMax_CoordofPoint_Normal(double Pointmin_max[6], double norPointmin_max[6]);
		void Find_minMax_CoordofCurv(double tab_minMax_curv[8]);
		void Sort_Curv_ICP(char* curv, float percentage);
		void Convert_GLMatrix_ToDouble(double M1[16], double M2[16], double M1n[9], double M2n[9]);
		void Find_Tab_of_neighborhood();
		void Compute_mean_of_edge();

		///------------------------- AFFICHAGE
		// VBO
		void Mesh_DrawObj_Building_list();
		void Mesh_DrawObj_Building_Buff();
		void Mesh_DrawObj_init();
		void Mesh_DrawObj_paintGL();
		void Mesh_DrawObj_updateBuffer();
		void Mesh_DrawObj_delete();
		void Mesh_DrawObj_modified_data(vtkSmartPointer<vtkUnsignedCharArray> colorT);
		void Mesh_DrawObj_updata(vtkSmartPointer<vtkUnsignedCharArray> colorT);
		void Mesh_DrawObj_VBO(bool bool_change_pos_obj);

		// fonction d'avant 
		void Mesh_DrawObj_imediateFunct(int level);

		// fonction générale
		void Mesh_DrawObj(int level, bool bool_change_pos_obj,bool bool_vbo);


};

#endif