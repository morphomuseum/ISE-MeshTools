#ifndef KDTREE1_H
#define KDTREE1_H
/*
Szymon Rusinkiewicz
Princeton University

KDtree1.h
A K-D tree for points, with limited capabilities (find nearest point to
a given point, or to a ray).

Note that in order to be generic, this *doesn't* use Vecs and the like...
*/
#include <vtkSmartPointer.h>
#include <vtkFloatArray.h>
#include <vector>
#include <vtkPoints.h>


class Node1;
class KDtree1 {
private:

	Node1 *root;
	void Build(const  vtkSmartPointer<vtkFloatArray> ptlist, bool bool_indice);
	bool bool_color = 0;
	int nbComponents;
public:

	KDtree1(const vtkSmartPointer<vtkFloatArray> &list, bool bool_indice = 0){ Build(list, bool_indice); }
	~KDtree1();
	vtkSmartPointer<vtkFloatArray> Closest_to_pt(const double *p, float *maxdist2, vtkSmartPointer<vtkFloatArray> tab_weight, double alpha[6]);
	vtkFloatArray* PointsInRadius(double *point, float R);
	Node1* GetRoot(){ return root; }
};


///-------------------------  CLASS  Node1  -------------///

typedef struct PointNdim1;
typedef struct node1;
typedef struct leaf1;
typedef struct pointNdimAxe1;

struct node1{
	vtkFloatArray *center;
	int splitaxis;
	Node1 *left_child;
	Node1 *right_child;

};

struct leaf1{
	vtkFloatArray *pointList;
	vtkFloatArray *indexColList;// index et eventuelle la couleur
};

struct pointNdimAxe1{
	float axepoint;
	int index;
};

struct PointNdim1{
	vtkFloatArray *point;
	int index;
};



class Node1
{
private:
	bool IsLeaf; // If this is false, intermediate Node1.  If true, leaf.
	enum { MAX_PTS_PER_NODE = 1 };
	union {
		struct node1 N;
		struct leaf1 L;
	}structure;

public:
	int nbComponents;
	Node1(vtkSmartPointer<vtkFloatArray> ptlist, vtkSmartPointer<vtkFloatArray> indCollist, int depth, int nbComponents);
	~Node1();
	void Candidat(double* point, double* infopoint, vtkSmartPointer<vtkFloatArray> candidatPoint);////*vtkSmartPointer<vtkFloatArray> point, vtkSmartPointer<vtkFloatArray> infopoint,*/ int i, vtkSmartPointer<vtkFloatArray> candidatPoint);
	void find_closest_to_pt(const double *point, int depth, float *maxdist2, vtkSmartPointer<vtkFloatArray> candidatPoint, vtkSmartPointer<vtkFloatArray> tab_weight, double alpha[8]);
	void PointsInRadius(double *point, int depth, float R, vtkFloatArray *KcandidatPoint, vtkSmartPointer<vtkFloatArray> tab_weight, double alpha[8]);

	bool GetIsLeaf(){ return IsLeaf; }
	vtkFloatArray* GetLeaf(){ return structure.L.pointList; }
	vtkFloatArray* GetInfoLeaf(){ return structure.L.indexColList; }
	vtkFloatArray *GetNode_Center(){ return structure.N.center; }
	Node1 * GetNode_LeftChild(){ return structure.N.left_child; }
	Node1 * GetNode_RightChild(){ return structure.N.right_child; }
	int GetNode_Splitaxis(){ return structure.N.splitaxis; }

};

static float Sqr(float x);
static float Dist2pointDimNweight(const double *x, const double *y, vtkSmartPointer<vtkFloatArray> tab_weight, double alpha[8], int nbComponents);
int CompareAxe1(const void*p1, const void*p2);
void Sort1(vtkSmartPointer<vtkFloatArray> list, vtkSmartPointer<vtkFloatArray> infolist, int axe);
void Sorted1(int firstAxe, vtkFloatArray* ptlist, vtkSmartPointer<vtkFloatArray> infolist);
void TestKDtree1();
bool Appartient1(int j, vtkFloatArray*listAxe);
bool IsEqualAccordingToAxe1(double *point1, double *point2, int dim, vtkFloatArray*listAxe);
void DrawKDtree(KDtree1 *t);
void DrawTree(Node1 *n, int depth);

#endif