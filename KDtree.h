#ifndef KDTREE_H
#define KDTREE_H
/*
Szymon Rusinkiewicz
Princeton University

KDtree.h
A K-D tree for points, with limited capabilities (find nearest point to 
a given point, or to a ray). 

Note that in order to be generic, this *doesn't* use Vecs and the like...
*/
#include <vtkSmartPointer.h>
#include <vtkFloatArray.h>
#include <vector>
#include <vtkPoints.h>


class Node;
class KDtree {
private:
	
	Node *root;
	void Build(const  vtkSmartPointer<vtkFloatArray> ptlist, bool bool_indice);
	bool bool_color = 0;
	int nbComponents;
public:
	
	KDtree(const vtkSmartPointer<vtkFloatArray> &list, bool bool_indice=0){ Build(list,bool_indice);}
	~KDtree();
	vtkSmartPointer<vtkFloatArray> Closest_to_pt(const double *p, float *maxdist2, vtkSmartPointer<vtkFloatArray> tab_weight, double alpha[6]);
	vtkFloatArray* PointsInRadius(double *point, float R);
	Node* GetRoot(){ return root; }
};


///-------------------------  CLASS  NODE  -------------///

typedef struct PointNdim;
typedef struct node;
typedef struct leaf;
typedef struct pointNdimAxe;

struct node{
	vtkFloatArray *center;
	int splitaxis;
	Node *left_child;
	Node *right_child;
};

struct leaf{
	vtkFloatArray *pointList;
	vtkFloatArray *indexColList;// index et eventuelle la couleur
};

struct pointNdimAxe{
	float axepoint;
	int index;
};

struct PointNdim{
	vtkFloatArray *point;
	int index;
};



class Node
{
private:
	bool IsLeaf; // If this is false, intermediate node.  If true, leaf.
	enum { MAX_PTS_PER_NODE = 1 }; 
	union {
		struct node N; 
		struct leaf L;
	}structure; 

public:
	int nbComponents;
	Node(vtkSmartPointer<vtkFloatArray> ptlist, vtkSmartPointer<vtkFloatArray> indCollist, int depth, int nbComponents);
	~Node();
	void Candidat(double* point, double* infopoint, vtkSmartPointer<vtkFloatArray> candidatPoint);////*vtkSmartPointer<vtkFloatArray> point, vtkSmartPointer<vtkFloatArray> infopoint,*/ int i, vtkSmartPointer<vtkFloatArray> candidatPoint);
	void find_closest_to_pt(const double *point, int depth, float *maxdist2, vtkSmartPointer<vtkFloatArray> candidatPoint, vtkSmartPointer<vtkFloatArray> tab_weight, double alpha[8]);
	void PointsInRadius(double *point, int depth, float R, vtkFloatArray *KcandidatPoint,vtkSmartPointer<vtkFloatArray> tab_weight, double alpha[8]);
	
	bool GetIsLeaf(){ return IsLeaf; }
	vtkFloatArray* GetLeaf(){ return structure.L.pointList; }
	vtkFloatArray* GetInfoLeaf(){ return structure.L.indexColList; }
	vtkFloatArray *GetNode_Center(){ return structure.N.center; }
	Node * GetNode_LeftChild(){ return structure.N.left_child; }
	Node * GetNode_RightChild(){ return structure.N.right_child; }
	int GetNode_Splitaxis(){ return structure.N.splitaxis; }

};

static float Sqr(float x);
static float Dist2pointDimNweight(const double *x, const double *y, vtkSmartPointer<vtkFloatArray> tab_weight, double alpha[8], int nbComponents);
int CompareAxe(const void*p1, const void*p2);
void Sort(vtkSmartPointer<vtkFloatArray> list, vtkSmartPointer<vtkFloatArray> infolist, int axe);
void Sorted(int firstAxe, vtkFloatArray* ptlist, vtkSmartPointer<vtkFloatArray> infolist);
void TestKDtree();
bool Appartient(int j, vtkFloatArray*listAxe);
bool IsEqualAccordingToAxe(double *point1, double *point2, int dim, vtkFloatArray*listAxe);
void DrawKDtree(KDtree *t);
void DrawTree(Node *n, int depth); 

#endif