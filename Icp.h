#ifndef ICP_H
#define ICP_H 1213
#include <stdlib.h>
#include <math.h>										// Math Library Header File
#include <stdio.h>
#include <string.h>
#include <FL/gl.h>
#include "Object.h"
#include "Object_Mesh.h"
#include "Object_Landmark.h"
#include "Container_Mesh.h"
#include "KDtree.h"
#include "kdtree1.h"

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkMatrix4x4.h>
#include <vtkFloatArray.h>
#include <vtkKdTree.h>
#include <vtkLandmarkTransform.h>
#include <vtkTransform.h>


using namespace std;
/// meme nombre que pour vtkLandmarkTransform
#define RIGID VTK_LANDMARK_RIGIDBODY // rigid
#define SIMILITUDE VTK_LANDMARK_SIMILARITY // similitude
#define AFFINE VTK_LANDMARK_AFFINE // affine

#define LOC_AFFINE 20 // locally affine

#define MAX_ERROR_DISTANCE 0.05
#define MAX_ERROR_MATRIX 0.5
#define MAX_ITERATION_PER_MODE 20
#define MAX_NUMBER_OF_POINTS 100000
#define MAX_ANGLE 30  //degrees
#define MAX_N_NEIGHBORD 10 // to find the N closest points


typedef struct sphere_Set;
typedef struct matrix;


struct sphere_Set{
	vtkSmartPointer<vtkFloatArray> list;
};

struct matriX{
	vtkSmartPointer<vtkMatrix4x4> matrix;
};



class ICP
{
public:
	/////////////////////////// Attributes
	OBJECT_MESH* Source;
	OBJECT_MESH* Target;
	CONTAINER_MESH Cont_Mesh;

	vtkSmartPointer<vtkPoints> list_landmarks_source_test;
	vtkSmartPointer<vtkPoints> list_landmarks_source;
	vtkSmartPointer<vtkPoints> list_landmarks_target;
	vtkSmartPointer<vtkPoints> list_landmarks_init_source;
	vtkSmartPointer<vtkPoints> list_landmarks_init_target;
	vtkSmartPointer<vtkFloatArray> list_vertice_init_source;
	vtkSmartPointer<vtkFloatArray> list_landmarks_dimN_source;
	vtkSmartPointer<vtkFloatArray> list_landmarks_dimN_target;
	vtkSmartPointer<vtkFloatArray> IndexSphere_i_Source;
	vtkSmartPointer<vtkFloatArray> list_landmarks_dimN_target_LA;
	vtkSmartPointer<vtkFloatArray> weights;
	vtkSmartPointer<vtkFloatArray> tab_weights;
	vtkSmartPointer<vtkFloatArray> tab_display;
	vtkSmartPointer<vtkFloatArray> errors;
	vtkSmartPointer<vtkFloatArray> time;
	vtkSmartPointer<vtkFloatArray> parameter_list;
	
	int numberOfPoints_Source;
	int numberOfLandmarks_Source;
	int numberOfLandmarks_init;
	int number_errors;
	int icp_cpt_mode;
	int problem = 0;
	int mode_transformation;
	int dec;
	int dec_user;
	int cpt_mode;
	int dim_points;
	int redo;
	bool bool_onlyMatching;
	bool bool_landmarks_add;
	bool existColor;
	bool bool_save_parameters;
	bool bool_loaded_parameters;
	bool bool_LA_redo=0;
	bool bool_tags = 0;
	bool bool_kdtree_LA =0;// mettre a 1
	float weight_vertice_addland;
	float weight_land_addland;
	float vect_matching_color[3];
	float percentRadius_filter;
	float *threshold;
	float percentageRadius_of_sphere_LA;
	float percentageCurv;
	double *pointSourceKDtree;
	double *pointTargetKDtree;
	double alpha[8];
	KDtree *tree;
	
	//// teste 1 draw à enlever
	//vtkSmartPointer<vtkFloatArray>Index;
	//vtkSmartPointer<vtkFloatArray>point;

	KDtree1 *test;

	////////////////////////// Functions
	ICP(OBJECT_MESH* SourceData, OBJECT_MESH* TargetData, CONTAINER_MESH Cont_Mesh, vtkFloatArray*loaded_parameter_list, vtkFloatArray* tabOfParameters, vtkFloatArray* tab_loadsave_parameter, vtkFloatArray *tabDisplay, int modeTransformation, bool existColor, bool bool_onlyMatching);
	void Draw_matching();
	void Draw_looking_like(float val);
	void SetMode();
	void SetData(vtkSmartPointer<vtkFloatArray> tabOfParameters, int modeTransformation, vtkSmartPointer<vtkFloatArray> tabDisplay, vtkFloatArray* tab_loadsave_parameter, bool bool_only_matchings = 0);
	bool Mode_using_Source_points();
	char* GetMode(int mode=-1);
	int GetModeInv(char *mode);
	~ICP();
private:
	void Iteration(); // ICP iteration
	void Reinit();
	void Pairs_matching(vtkSmartPointer<vtkFloatArray>list_landmarks_dimN_source, vtkSmartPointer<vtkFloatArray>list_landmarks_dimN_target);
	void Transformation(vtkSmartPointer<vtkPoints> list_filter_source, vtkSmartPointer<vtkPoints> list_filter_target);
	void IterationFor_R_S_A();
	void IterationFor_LA(int radius=5);
	void Build_SphereSet(sphere_Set* sphereList, float R);
	void Build_SphereSetKDtree(sphere_Set* sphereList, float R);
	void Build_SphereSet_Neighbord(sphere_Set* sphereList, float R);
	void Build_matrixlist(matriX* matrix_list, sphere_Set* sphereList);
	void Build_LA_transformation(matriX* matrix_list, sphere_Set* sphereList, vtkSmartPointer<vtkPoints> new_list_source, float *err);
	void ComputeNewPoint_LA(double point_i[3], int *index, vtkSmartPointer<vtkFloatArray> Sphere_i_Source, vtkSmartPointer<vtkFloatArray> Sphere_i_Target, double transformedPointRef[3], matriX* matrix_list);
	vtkMatrix4x4* MatrixTransformationForOnePoint(vtkSmartPointer<vtkPoints> sourcepoint, vtkSmartPointer<vtkPoints> targetpoint);

	void Filter(vtkSmartPointer<vtkPoints> list_filter_source, vtkSmartPointer<vtkPoints> list_filter_target);
	void Sort_matchings_distances(vtkSmartPointer<vtkPoints> list_filter_source, vtkSmartPointer<vtkPoints> list_filter_target);
	void Sort_matchings_for_color(vtkSmartPointer<vtkPoints> list_filter_source, vtkSmartPointer<vtkPoints> list_filter_target);
	void Sort_matchings_for_curv();
	void transformPoint(vtkMatrix4x4* matrix, double pointin[3], double pointout[3]);
	void UpdateCurvObject(OBJECT_MESH *object, vtkSmartPointer<vtkPoints> newlist=NULL);
	
	void Initialisation_of_points_ofDimN(OBJECT_MESH *object, vtkSmartPointer<vtkFloatArray> list_landmarks_dimN_object);
	void Compute_dim_of_points(vtkFloatArray* tabOfParameters);
	void Init_alpha_tab(double Pointmin_max[6], double norPointmin_max[6], double curvMinMax[8]);

	void Init_alpha_of_coord(double Pointmin_max[6], double norPointmin_max[6]);
	void Init_alpha_of_curv(double curvMinMax[8]);
	void StoreParameters();
	vtkSmartPointer<vtkIdList> SetOfLoadedParameters(vtkSmartPointer<vtkFloatArray> loaded_parameter_list, vtkSmartPointer<vtkFloatArray> tabOfParameters, int modeTransformation);

	int UpdateParameters(vtkSmartPointer<vtkFloatArray> tabOfParameters, vtkSmartPointer<vtkFloatArray> tabDisplay);
	void Analyse_Loaded_Parameter_list(vtkIdList* modeList_toDo);
	void SetTabOfParameter(vtkSmartPointer<vtkFloatArray>tabOfParameters, int modeTransformation);
	void BeginICP(vtkSmartPointer<vtkFloatArray> tabOfParameters, vtkFloatArray* tab_loadsave_parameter, int modeTransformation, vtkSmartPointer<vtkFloatArray> tabDisplay, vtkSmartPointer<vtkIdList> modeList_toDo);
	void OnlyMatching();
	void Initialise_dec();
	void UpdateMode(int modeTransformation);

	bool RedoTree(vtkSmartPointer<vtkFloatArray> tabOfParametre, vtkSmartPointer<vtkFloatArray> tab_weights);
	void AnalyseTab(vtkSmartPointer<vtkFloatArray> tab, vtkSmartPointer<vtkFloatArray> tabout);



	void Test_radius();
};
bool AppartientI(int axe, vtkFloatArray*listAxe);
bool TabAreEqual(vtkSmartPointer<vtkFloatArray> t_new, vtkSmartPointer<vtkFloatArray> t);
void testICP(ICP *r1, ICP *r2);
bool testDiffKDTreeDiff(Node *node1, Node* node2, int *cptNOk, int *cptN, int *cptL, int *cptLElem);
void TestMatchingLand(ICP *i1, ICP *i2);
void TestMatchingSommet(ICP *i1, ICP *i2);
void TestShapeSource(ICP *i1, ICP *i2);
int TestCorrespondances(vtkPoints *targetList, vtkPoints *targetList1);
bool testDiffKDTreeDiff(Node1 *node1, Node1* node2, int *cptNOk, int *cptN, int *cptL, int *cptLElem);
bool testDiffKDTreeDiff(Node1 *node1, Node* node2, int *cptNOk, int *cptN, int *cptL, int cptLElem);
int TestCorrespondances(vtkFloatArray *targetList, vtkFloatArray *targetList1, vtkFloatArray* sourceList, int dimpoints, vtkSmartPointer<vtkFloatArray> tab_weight, double alpha[6]);
static float Sqr(float x);
static float Dist2pointDimNweight(const double *x, const double *y, vtkSmartPointer<vtkFloatArray> tab_weight, double alpha[8], int nbComponents);


#endif
