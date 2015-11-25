#include "kdtree1.h"
#include "Struct.h"

#include <vtkMath.h>

typedef pointNdimAxe1* point;

////////////////////////////////////////////////   KDTREE  //////////////////////////////////////////////////////

//construit le KDtree
void KDtree1::Build(const  vtkSmartPointer<vtkFloatArray> list, bool bool_color){
	this->nbComponents = list->GetNumberOfComponents();
	nbComponents--;// on enlève les indices
	int n = list->GetNumberOfTuples();
	if (bool_color)
		nbComponents--;// on enlève la donnée couleur
	this->bool_color = bool_color;

	/// on a un tableau contenant les points (coordonnées 3D, normale, courbure)
	/// et un deuxième contenant des informations ne jouant aucun role dans le KDtree (indice et couleur)
	vtkSmartPointer<vtkFloatArray> indCollist = vtkSmartPointer<vtkFloatArray>::New();
	indCollist->SetNumberOfComponents(2);
	indCollist->SetNumberOfTuples(n);
	vtkSmartPointer<vtkFloatArray> ptlist = vtkSmartPointer<vtkFloatArray>::New();
	ptlist->SetNumberOfComponents(nbComponents);
	ptlist->SetNumberOfTuples(n);

	vtkSmartPointer<vtkFloatArray> templist = vtkSmartPointer<vtkFloatArray>::New();
	templist->SetNumberOfComponents(list->GetNumberOfComponents());
	templist->SetNumberOfTuples(n);
	templist->DeepCopy(list);
	// creation de ses deux tableaux
	for (int i = 0; i < n; i++){
		if (bool_color)// tableau des informations
			indCollist->SetTuple2(i, list->GetComponent(i, nbComponents + 1), list->GetComponent(i, nbComponents));
		else
			indCollist->SetTuple2(i, list->GetComponent(i, nbComponents), -1);

		for (int j = 0; j < nbComponents; j++){// tableau des points
			ptlist->SetComponent(i, j, templist->GetComponent(i, j));
		}
	}
	cout << "KDTree1 (en cours) ::  ";
	root = new Node1(ptlist, indCollist, 0, nbComponents);// construction des noeuds
	cout << " (nbComponents=" << nbComponents << ")  done" << endl;
}

// Delete a KDtree
KDtree1::~KDtree1()
{
	root = NULL;
}


// Return the closest point in the KD tree to p
vtkSmartPointer<vtkFloatArray> KDtree1::Closest_to_pt(const double *point, float *maxdist2, vtkSmartPointer<vtkFloatArray> tab_weight, double alpha[8])
{
	double *closestPoint;
	float mindist;

	int Dim = this->nbComponents;
	Dim++; // pour l'indice
	if (bool_color == 1)// pour la couleur
		Dim++;

	vtkSmartPointer<vtkFloatArray> candidatPoint = vtkSmartPointer<vtkFloatArray>::New();
	candidatPoint->SetNumberOfComponents(Dim);
	candidatPoint->SetNumberOfTuples(1);

	*maxdist2 = INFINITY;
	root->find_closest_to_pt(point, 0, maxdist2, candidatPoint, tab_weight, alpha);
	closestPoint = candidatPoint->GetTuple(0);
	
	return candidatPoint;
}

// Return the points list that are in the radius
vtkFloatArray* KDtree1::PointsInRadius(double *point, float R){

	double *closestPoint;
	float mindist;

	vtkFloatArray *KcandidatPoint = vtkFloatArray::New();
	KcandidatPoint->SetNumberOfComponents(1);

	// on prends que les points 3d pour avoir que les voisins
	vtkFloatArray *tabW = vtkFloatArray::New();
	tabW->SetNumberOfComponents(1);
	tabW->SetNumberOfTuples(2);
	tabW->SetTuple1(0, 0);
	tabW->SetTuple1(1, 0);

	double alpha[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };
	root->PointsInRadius(point, 0, R, KcandidatPoint, tabW, alpha);

	return KcandidatPoint;  // retourne la liste des indices des points
}


///////////////////////////////////////////////////////////     NODES     ////////////////////////////////////////////////////////

//Constructeur
Node1::Node1(vtkSmartPointer<vtkFloatArray> ptlist, vtkSmartPointer<vtkFloatArray> indCollist, int depth, int nbComponents){
	int n = ptlist->GetNumberOfTuples();
	int d = nbComponents;
	this->nbComponents = nbComponents;
	int v = MAX_PTS_PER_NODE;
	//if (d == 2 || d==3)   //avec test KDTREE
	//	v = 1;

	if (n <= v) {
		this->IsLeaf = true;
		this->structure.L.pointList = vtkFloatArray::New();
		this->structure.L.pointList->SetNumberOfComponents(d);
		this->structure.L.indexColList = vtkFloatArray::New();
		this->structure.L.indexColList->SetNumberOfComponents(2);
		structure.L.pointList->DeepCopy(ptlist);
		structure.L.indexColList->DeepCopy(indCollist);
	}
	else{
		// Else, interior nodes
		this->IsLeaf = false;
		// definir l'axe
		structure.N.splitaxis = depth % d;

		vtkSmartPointer<vtkFloatArray> minmaxlist = vtkSmartPointer<vtkFloatArray>::New();
		minmaxlist->SetNumberOfComponents(1);
		minmaxlist->SetNumberOfTuples(2 * d);
		// Find bbox
		for (int j = 0; j < d; j++){
			minmaxlist->SetTuple1(2 * j, ptlist->GetComponent(0, j));
			minmaxlist->SetTuple1(2 * j + 1, ptlist->GetComponent(0, j));
		}


		for (size_t i = 1; i < n; i++) {
			for (int j = 0; j < d; j++){
				if (ptlist->GetComponent(i, j) < minmaxlist->GetTuple1(2 * j))//min
					minmaxlist->SetTuple1(2 * j, ptlist->GetComponent(i, j));
				if (ptlist->GetComponent(i, j) > minmaxlist->GetTuple1(2 * j + 1))//max
					minmaxlist->SetTuple1(2 * j + 1, ptlist->GetComponent(i, j));
			}
		}



		// Find node center and size
		structure.N.center = vtkFloatArray::New();
		structure.N.center->SetNumberOfComponents(d);
		structure.N.center->SetNumberOfTuples(1);

		for (int i = 0; i < d; i++){
			structure.N.center->SetComponent(0, i, 0.5f*(minmaxlist->GetTuple1(2 * structure.N.splitaxis) + minmaxlist->GetTuple1(2 * structure.N.splitaxis + 1)));
		}
		
		float mediane = structure.N.center->GetComponent(0, structure.N.splitaxis);


		vtkSmartPointer<vtkFloatArray> list = vtkSmartPointer<vtkFloatArray>::New();
		list->SetNumberOfComponents(d);
		list->SetNumberOfTuples(n);
		list->DeepCopy(ptlist);

		vtkSmartPointer<vtkFloatArray> infolist = vtkSmartPointer<vtkFloatArray>::New();
		infolist->SetNumberOfComponents(2);
		infolist->SetNumberOfTuples(n);
		infolist->DeepCopy(indCollist);

		// le tri des points selon lors axe:
		Sorted1(structure.N.splitaxis, list, infolist);
		//Sort1(list, infolist, structure.N.splitaxis); 
		
		////constructions des listes gauche et droite
		vtkSmartPointer<vtkFloatArray> left = vtkSmartPointer<vtkFloatArray>::New();
		left->SetNumberOfComponents(d);
		vtkSmartPointer<vtkFloatArray> right = vtkSmartPointer<vtkFloatArray>::New();
		right->SetNumberOfComponents(d);
		vtkSmartPointer<vtkFloatArray> infoleft = vtkSmartPointer<vtkFloatArray>::New();
		infoleft->SetNumberOfComponents(2);
		vtkSmartPointer<vtkFloatArray> inforight = vtkSmartPointer<vtkFloatArray>::New();
		inforight->SetNumberOfComponents(2);

		for (int i = 0; i < n; i++){
			if (list->GetTuple(i)[structure.N.splitaxis] <= mediane){
				left->InsertNextTuple(list->GetTuple(i));
				infoleft->InsertNextTuple(infolist->GetTuple(i));
			}
			if (list->GetTuple(i)[structure.N.splitaxis] > mediane){
				right->InsertNextTuple(list->GetTuple(i));
				inforight->InsertNextTuple(infolist->GetTuple(i));
			}
		}

		////constructions des noeuds gauche et droit
		structure.N.left_child = new Node1(left, infoleft, depth + 1, nbComponents);
		structure.N.right_child = new Node1(right, inforight, depth + 1, nbComponents);
	}
}

// Destroy a KD tree node
Node1::~Node1()
{
	/*if (!this->IsLeaf) {
	structure.N.center->Delete();
	delete structure.N.left_child;
	delete structure.N.right_child;
	}
	else{
	structure.L.pointList->Delete();
	}*/
}

// Récupère le point et ses infos
void Node1::Candidat(double* point, double* infopoint, vtkSmartPointer<vtkFloatArray> candidatPoint){
	int d = nbComponents;
	int dcandi = candidatPoint->GetNumberOfComponents();

	vtkSmartPointer<vtkFloatArray> temp = vtkSmartPointer<vtkFloatArray>::New();
	temp->SetNumberOfComponents(dcandi);
	temp->SetNumberOfTuples(1);

	//double *p = this->structure.L.pointList->GetTuple(i);

	for (int j = 0; j < d; j++){
		temp->SetComponent(0, j, point[j]);
	}
	temp->SetComponent(0, dcandi - 1, infopoint[0]);
	if (infopoint[1] != -1){
		temp->SetComponent(0, dcandi - 2, infopoint[1]);
	}
	/*affiche(point, d);
	affiche(infopoint, 2);*/

	candidatPoint->DeepCopy(temp);
	//affiche(candidatPoint->GetTuple(0), dcandi);
}

// Crawl the KD tree  http://andrewd.ces.clemson.edu/courses/cpsc805/references/nearest_search.pdf
// On cherche le point le plus proche du point en paramètre 
void Node1::find_closest_to_pt(const double *point, int depth, float *mindist, vtkSmartPointer<vtkFloatArray> candidatPoint, vtkSmartPointer<vtkFloatArray> tab_weight, double alpha[8])
{
	/*if (depth == 0){
		Candidat(this->structure.N.center, this->structure.N.indCol, 0, candidatPoint);
	}*/
	
	if (this->IsLeaf) {// cherche dans la feuille atteint
		double *pleaf;
		//cout << "f_";
		for (int i = 0; i < structure.L.pointList->GetNumberOfTuples(); i++) {
			pleaf = structure.L.pointList->GetTuple(i);
			float myd2 = Dist2pointDimNweight(pleaf, point, tab_weight, alpha, this->nbComponents);

			if (myd2 < *mindist) {
				*mindist = myd2;
				//cout << "\n   "<< myd2 << "  ";
				this->Candidat(pleaf, structure.L.indexColList->GetTuple(i), candidatPoint);
			}
		}
		//cout << "_f" << endl;;
	}
	else{// teste et recherche dans les noeuds
		float component_axis_center = (float) structure.N.center->GetComponent(0, structure.N.splitaxis);
		float component_axis_point = (float) point[structure.N.splitaxis];
		
		//cout << "nn_ "; 
		//cout << "   " << structure.N.splitaxis << "   "; affiche(structure.N.center->GetTuple(0), nbComponents); cout << "_nn" << endl;

		if (component_axis_point <= component_axis_center){
			if (component_axis_point - *mindist <= component_axis_center){
				structure.N.left_child->find_closest_to_pt(point, depth + 1, mindist, candidatPoint, tab_weight, alpha);
			}
			if (component_axis_point + *mindist > component_axis_center){
				structure.N.right_child->find_closest_to_pt(point, depth + 1, mindist, candidatPoint, tab_weight, alpha);
			}
		}
		else{
			if (component_axis_point + *mindist > component_axis_center){
				structure.N.right_child->find_closest_to_pt(point, depth + 1, mindist, candidatPoint, tab_weight, alpha);
			}
			if (component_axis_point - *mindist <= component_axis_center){
				structure.N.left_child->find_closest_to_pt(point, depth + 1, mindist, candidatPoint, tab_weight, alpha);
			}
		}
	}
}

// on cherche les points dans un certain rayon "R" de centre "point".
void Node1::PointsInRadius(double *point, int depth, float R, vtkFloatArray *KcandidatPoint, vtkSmartPointer<vtkFloatArray> tab_weight, double alpha[8]){

	if (this->IsLeaf) {//on cherche dans la feuille atteint
		for (int i = 0; i < structure.L.pointList->GetNumberOfTuples(); i++) {
			float myd2 = Dist2pointDimNweight(structure.L.pointList->GetTuple(i), point, tab_weight, alpha, this->nbComponents);
			if (myd2 <= R) {
				KcandidatPoint->InsertNextTuple1(structure.L.indexColList->GetComponent(i, 0));
			}
		}
	}
	else{// teste et recherche dans les noeuds
		float component_axis_center = structure.N.center->GetComponent(0, structure.N.splitaxis);
		float component_axis_point = point[structure.N.splitaxis];
		if (component_axis_point <= component_axis_center){
			if (component_axis_point - R <= component_axis_center){
				structure.N.left_child->PointsInRadius(point, depth + 1, R, KcandidatPoint, tab_weight, alpha);
			}
			if (component_axis_point + R > component_axis_center){
				structure.N.right_child->PointsInRadius(point, depth + 1, R, KcandidatPoint, tab_weight, alpha);
			}
		}
		else{
			if (component_axis_point + R > component_axis_center){
				structure.N.right_child->PointsInRadius(point, depth + 1, R, KcandidatPoint, tab_weight, alpha);
			}
			if (component_axis_point - R <= component_axis_center){
				structure.N.left_child->PointsInRadius(point, depth + 1, R, KcandidatPoint, tab_weight, alpha);
			}
		}
	}
}



//////////////////////////////////   FONCTION ANNEXE /////////////////////////////
// carré de x
static float Sqr(float x)
{
	return x*x;
}

//Distance euclidienne dans la recherche des points dans le KDtree
static float Dist2pointDimNweight(const double *x, const double *y, vtkSmartPointer<vtkFloatArray> tab_weight, double alpha[8], int nbComponents)
{
	float result = 0.f;
	double weight = 1.;

	float normalbeta = tab_weight->GetTuple1(0);
	float curvbeta = tab_weight->GetTuple1(1);
	bool bool_curv = tab_weight->GetTuple1(1) != -1;
	bool bool_norm = tab_weight->GetTuple1(0) != -1;
	if (!bool_norm){
		normalbeta = 0;
	}
	if (!bool_curv){
		curvbeta = 0;
	}

	float pointbeta = (1 + normalbeta + curvbeta);


	for (int i = 0; i<nbComponents; i++){//nbComponents == dimension des points

		if (i < 3)
			weight = pointbeta * alpha[i];
		if (i >= 3 && i < 6)
			weight = normalbeta * alpha[i];
		if (i >= 6)
			weight = curvbeta * alpha[i];

		result += weight*Sqr(x[i] - y[i]);

	}
	return sqrt(result);
}

//Compare l'element des points selon l'axe
int CompareAxe1(const void* p1, const void* p2){
	float axp1 = point(p1)->axepoint;
	float axp2 = point(p2)->axepoint;

	if (axp1 > axp2){
		return 1;
	}
	else if (axp1 < axp2)
		return -1;
	else
		return 0;
}

// Trie de maniere croissante selon un axe donnée la liste des points
void Sort1(vtkSmartPointer<vtkFloatArray> list, vtkSmartPointer<vtkFloatArray> infolist, int axe){
	int N = list->GetNumberOfTuples();
	pointNdimAxe1 *listPaxe = (pointNdimAxe1*)malloc(N*sizeof(pointNdimAxe1));

	//trie
	for (int i = 0; i < N; i++){
		listPaxe[i].axepoint = list->GetComponent(i, axe);
		listPaxe[i].index = i;
	}
	qsort(listPaxe, N, sizeof(pointNdimAxe1), CompareAxe1);

	vtkSmartPointer<vtkFloatArray> listtemp = vtkSmartPointer<vtkFloatArray>::New();
	listtemp->SetNumberOfComponents(list->GetNumberOfComponents());
	vtkSmartPointer<vtkFloatArray> infolisttemp = vtkSmartPointer<vtkFloatArray>::New();
	infolisttemp->SetNumberOfComponents(infolist->GetNumberOfComponents());

	//Changement de place des points dans la liste en fonction du tri
	for (int i = 0; i < N; i++){
		int index = listPaxe[i].index;
		listtemp->InsertNextTuple(list->GetTuple(index));
		infolisttemp->InsertNextTuple(infolist->GetTuple(index));
	}
	list->DeepCopy(listtemp);
	infolist->DeepCopy(infolisttemp);
	free(listPaxe);
}

//Trie en "profondeur"
void Sorted1(int firstAxe, vtkFloatArray* ptlist, vtkSmartPointer<vtkFloatArray> infolist){
	int n = ptlist->GetNumberOfTuples();
	int d = ptlist->GetNumberOfComponents();

	vtkSmartPointer<vtkFloatArray> list = vtkSmartPointer<vtkFloatArray>::New();
	list->SetNumberOfComponents(d);
	list->SetNumberOfTuples(n);
	Sort1(ptlist, infolist, firstAxe);// premier trie
	list->DeepCopy(ptlist);

	vtkSmartPointer<vtkFloatArray> listAxe = vtkSmartPointer<vtkFloatArray>::New();
	listAxe->InsertNextValue(firstAxe);


	int debut = 0, fin = 0;

	vtkSmartPointer<vtkFloatArray> list1 = vtkSmartPointer<vtkFloatArray>::New();
	list1->SetNumberOfComponents(d);
	list1->SetNumberOfTuples(n);// pour les points,
	list1->DeepCopy(list);// il me faut une deuxieme liste sinon on ne peut prendre i et i+1

	vtkSmartPointer<vtkFloatArray> infolist1 = vtkSmartPointer<vtkFloatArray>::New();
	infolist1->SetNumberOfComponents(infolist->GetNumberOfComponents());
	infolist1->SetNumberOfTuples(n);// pour les infos des points,
	infolist1->DeepCopy(infolist);// il me faut une deuxieme liste sinon on ne peut prendre i et i+1


	vtkSmartPointer<vtkFloatArray> listTemp = vtkSmartPointer<vtkFloatArray>::New();
	listTemp->SetNumberOfComponents(d);
	vtkSmartPointer<vtkFloatArray> infolistTemp = vtkSmartPointer<vtkFloatArray>::New();
	infolistTemp->SetNumberOfComponents(infolist->GetNumberOfComponents());

	// Une fois un tri fait, si pour cet axe, on a la même valeur sur plusieurs points consécutifs
	// on retrie sur CES points sur l'axe suivant (non traité).
	for (int axe = 0; axe < d; axe++){
		if (!Appartient1(axe, listAxe)){
			for (int i = 0; i < n; i++){
				if (i + 1 < n) {
					double *point1 = list->GetTuple(i);
					double *point2 = list1->GetTuple(i + 1);
					double *info1 = infolist->GetTuple(i);
					double *info2 = infolist1->GetTuple(i + 1);

					if (IsEqualAccordingToAxe1(point1, point2, d, listAxe)){

						if (listTemp->GetNumberOfTuples() == 0){
							debut = i;
							listTemp->InsertNextTuple(point1);
							infolistTemp->InsertNextTuple(info1);
						}
						listTemp->InsertNextTuple(point2);
						infolistTemp->InsertNextTuple(info2);
					}
					else{
						if (listTemp->GetNumberOfTuples() != 0){
							fin = debut + listTemp->GetNumberOfTuples();
							//trie
							Sort1(listTemp, infolistTemp, axe);

							for (int j = debut; j < fin; j++){
								list->SetTuple(j, listTemp->GetTuple(j - debut));
								infolist->SetTuple(j, infolistTemp->GetTuple(j - debut));
							}
							list1->DeepCopy(list);

							listTemp = vtkSmartPointer<vtkFloatArray>::New();
							listTemp->SetNumberOfComponents(d);
							infolistTemp = vtkSmartPointer<vtkFloatArray>::New();
							infolistTemp->SetNumberOfComponents(infolist->GetNumberOfComponents());

							debut = 0, fin = 0;
						}
					}//else
				}
			}// for i
			listAxe->InsertNextValue(axe);
		}// if app
	}// for j
	ptlist->DeepCopy(list);
}



// Quelques tests  du KDtree
void TestKDtree1(){

	cout << "KD-TREE " << endl;
	vtkSmartPointer<vtkFloatArray> list_points = vtkSmartPointer<vtkFloatArray>::New();
	list_points->SetNumberOfComponents(3);
	list_points->SetNumberOfTuples(10);
	list_points->SetTuple3(0, 1, 2, 0);
	list_points->SetTuple3(1, 2, 3, 1);
	list_points->SetTuple3(2, 6, 4, 2);
	list_points->SetTuple3(3, 5, 8, 3);
	list_points->SetTuple3(4, 10, 3, 4);
	list_points->SetTuple3(5, 14, 7, 5);
	list_points->SetTuple3(6, 18, 100, 6);
	list_points->SetTuple3(7, 200, 20, 7);
	list_points->SetTuple3(8, 3, 4, 8);
	list_points->SetTuple3(9, 3, 2, 9);

	double point[2];
	point[0] = 6;
	point[1] = 7;
	float dist = 0;
	float tuple = 1;
	cout << " c'est parti!!!! " << endl;
	vtkSmartPointer<vtkFloatArray> weights = vtkSmartPointer<vtkFloatArray>::New();
	weights->InsertNextTuple(&tuple);
	weights->InsertNextTuple(&tuple);
	double alpha[6] = { 1, 1, 1, 1, 1, 1 };

	KDtree1 tree(list_points);
	cout << "depart     -----" << endl;

	DrawTree(tree.GetRoot(), 0);

	//double *closest = tree.Closest_to_pt(point, &dist, weights, alpha);
	//cout << " le point le plus proche is :::: ";  affiche(closest, 3);
	//system("pause");


	cout << "KD-TREE " << endl;
	vtkSmartPointer<vtkFloatArray> list_points0 = vtkSmartPointer<vtkFloatArray>::New();
	list_points0->SetNumberOfComponents(5);
	list_points0->SetNumberOfTuples(10);
	double  tuple0[5] = { 1, 2, 12, 7, 0 };
	double  tuple1[5] = { 2, 3, 17, 9, 1 };
	double  tuple2[5] = { 6, 4, 20, 10, 2 };
	double  tuple3[5] = { 5, 8, 1, 2, 3 };
	double  tuple4[5] = { 10, 3, 5, 5, 4 };
	double  tuple5[5] = { 14, 7, 6, 8, 5 };
	double  tuple6[5] = { 18, 100, 8, 4, 6 };
	double  tuple7[5] = { 200, 20, 9, 5, 7 };
	double  tuple8[5] = { 3, 4, 10, 4, 8 };
	double  tuple9[5] = { 3, 2, 11, 16, 9 };


	list_points0->SetTuple(0, tuple0);
	list_points0->SetTuple(1, tuple1);
	list_points0->SetTuple(2, tuple2);
	list_points0->SetTuple(3, tuple3);
	list_points0->SetTuple(4, tuple4);
	list_points0->SetTuple(5, tuple5);
	list_points0->SetTuple(6, tuple6);
	list_points0->SetTuple(7, tuple7);
	list_points0->SetTuple(8, tuple8);
	list_points0->SetTuple(9, tuple9);

	double point0[4];
	point0[0] = 6;
	point0[1] = 7;
	point0[2] = 3;
	point0[3] = 1;
	float dist0 = 0;
	float tuple0111 = 1;
	cout << " c'est parti!!!! " << endl;
	vtkSmartPointer<vtkFloatArray> weights0 = vtkSmartPointer<vtkFloatArray>::New();
	weights0->InsertNextTuple(&tuple0111);
	weights0->InsertNextTuple(&tuple0111);
	double alpha0[6] = { 0.4, 0.5, 0.5, 0.5, 0.2, 0.3 };

	KDtree1 tree0(list_points0, 0);
	cout << "depart     -----" << endl;

	DrawTree(tree0.GetRoot(), 0);

	/*double *closest0 = tree0.Closest_to_pt(point0, &dist0, weights0, alpha0);
	cout << " le point le plus proche is :::: "; affiche(closest0, 5);
*/
	system("pause");




	/*cout << "KD-TREE " << endl;
	vtkSmartPointer<vtkFloatArray> list_points1 = vtkSmartPointer<vtkFloatArray>::New();
	list_points1->SetNumberOfComponents(3);
	list_points1->SetNumberOfTuples(10);
	list_points1->SetTuple3(0, 1, 2, 3);
	list_points1->SetTuple3(1, 0, 10, 5);
	list_points1->SetTuple3(2, 1, 4, 3);
	list_points1->SetTuple3(3, 2, 5, 9);
	list_points1->SetTuple3(4, 3, 7, 3);
	list_points1->SetTuple3(5, 1, 4, 6);
	list_points1->SetTuple3(6, 2, 4, 1);
	list_points1->SetTuple3(7, 2, 6, 20);
	list_points1->SetTuple3(8, 3, 6, 17);
	list_points1->SetTuple3(9, 3, 8, 100);

	double point1[3];
	point1[0] = 2;
	point1[1] = 3;
	point1[2] = 2;
	float dist1 = 0;
	float tuple1 = 1;
	cout << " c'est parti!!!! " << endl;
	vtkSmartPointer<vtkFloatArray> weights1 = vtkSmartPointer<vtkFloatArray>::New();
	weights1->InsertNextTuple(&tuple1);
	weights1->InsertNextTuple(&tuple1);
	weights1->InsertNextTuple(&tuple1);
	double alpha1[6] = { 1, 1, 1, 1, 1, 1 };

	KDtree tree1(list_points1);
	cout << "depart     -----" << endl;

	DrawTree(tree1.GetRoot(), 0);

	double *closest1 = tree1.Closest_to_pt(point1, &dist1, weights1, alpha1);
	cout << " le point le plus proche is :::: " << closest1[0] << "  " << closest1[1] << "   " << closest1[2] << endl;

	system("pause");*/

	cout << "KD-TREE " << endl;
	vtkSmartPointer<vtkFloatArray> list_points1 = vtkSmartPointer<vtkFloatArray>::New();
	list_points1->SetNumberOfComponents(3);
	list_points1->SetNumberOfTuples(17);
	list_points1->SetTuple3(0, -4, -1, 0);
	list_points1->SetTuple3(1, -3, 3, 1);
	list_points1->SetTuple3(2, -3, -2, 2);
	list_points1->SetTuple3(3, -3, -4, 3);
	list_points1->SetTuple3(4, -2, 1, 4);
	list_points1->SetTuple3(5, -1, 2, 5);
	list_points1->SetTuple3(6, -1, -2, 6);
	list_points1->SetTuple3(7, -1, -5, 7);
	list_points1->SetTuple3(8, 1, 4, 8);
	list_points1->SetTuple3(9, 1, 2, 9);
	list_points1->SetTuple3(10, 1, 0, 10);
	list_points1->SetTuple3(11, 1, -4, 11);
	list_points1->SetTuple3(12, 2, -2, 12);
	list_points1->SetTuple3(13, 3, 3, 13);
	list_points1->SetTuple3(14, 6, 1, 14);
	list_points1->SetTuple3(15, 4, -2, 15);
	list_points1->SetTuple3(16, 5, 2, 16);

	double point1[2];
	point1[0] = 1;
	point1[1] = 0;
	float dist1 = 0;
	float tuple1000 = 1;
	cout << " c'est parti!!!! " << endl;
	vtkSmartPointer<vtkFloatArray> weights1 = vtkSmartPointer<vtkFloatArray>::New();
	weights1->InsertNextTuple1(0);
	weights1->InsertNextTuple1(0);
	double alpha1[6] = { 1, 1, 1, 1, 1, 1 };
	int R = 3;
	KDtree1 tree1(list_points1);
	cout << "depart     -----" << endl;

	DrawTree(tree1.GetRoot(), 0);
	/*vtkSmartPointer<vtkFloatArray> closest1 = vtkSmartPointer<vtkFloatArray>::New();
	closest1->SetNumberOfComponents(1);*/
	/*closest1->DeepCopy(tree1.PointsInRadius(point1, R));

	for (int i = 0; i < closest1->GetNumberOfTuples(); i++){
	affiche(closest1->GetTuple(i), 1);
	}*/

	system("pause");


	cout << "KD-TREE " << endl;
	vtkSmartPointer<vtkFloatArray> list_points10 = vtkSmartPointer<vtkFloatArray>::New();
	list_points10->SetNumberOfComponents(4);
	list_points10->SetNumberOfTuples(17);
	list_points10->SetTuple4(0, -4, -1, 12, 0);
	list_points10->SetTuple4(1, -3, 3, 16, 1);
	list_points10->SetTuple4(2, -3, -2, 19, 2);
	list_points10->SetTuple4(3, -3, -4, 20, 3);
	list_points10->SetTuple4(4, -2, 1, 0, 4);
	list_points10->SetTuple4(5, -1, 2, 15, 5);
	list_points10->SetTuple4(6, -1, -2, 15, 6);
	list_points10->SetTuple4(7, -1, -5, 15, 7);
	list_points10->SetTuple4(8, 1, 4, 16, 8);
	list_points10->SetTuple4(9, 1, 2, 13, 9);
	list_points10->SetTuple4(10, 1, 0, 19, 10);
	list_points10->SetTuple4(11, 1, -4, 16, 11);
	list_points10->SetTuple4(12, 2, -2, 8, 12);
	list_points10->SetTuple4(13, 3, 3, 17, 13);
	list_points10->SetTuple4(14, 6, 1, 2, 14);
	list_points10->SetTuple4(15, 4, -2, 2, 15);
	list_points10->SetTuple4(16, 5, 2, 2, 16);

	double point10[2];
	point10[0] = 1;
	point10[1] = 0;
	float dist10 = 0;
	float tuple10 = 1;
	cout << " c'est parti!!!! " << endl;
	vtkSmartPointer<vtkFloatArray> weights10 = vtkSmartPointer<vtkFloatArray>::New();
	weights10->InsertNextTuple1(0);
	weights10->InsertNextTuple1(0);
	double alpha10[6] = { 1, 1, 1, 1, 1, 1 };
	int R0 = 3;
	KDtree1 tree10(list_points10, 1);
	cout << "depart     -----" << endl;

	DrawTree(tree10.GetRoot(), 0);
	vtkSmartPointer<vtkFloatArray> closest10 = vtkSmartPointer<vtkFloatArray>::New();
	closest10->SetNumberOfComponents(1);
	/*closest10->DeepCopy(tree10.PointsInRadius(point10, R0));
	cout <<"nnnnn : "<< closest10->GetNumberOfTuples() << endl;
	cout << "resultat" << endl;
	for (int i = 0; i < closest10->GetNumberOfTuples(); i++){
	affiche(closest10->GetTuple(i), 1);
	}
	cout << "ok" << endl;*/


	system("pause");

}

//Si un element Appartient à une liste, retourne vrai sinon faux
bool Appartient1(int axe, vtkFloatArray*listAxe){
	for (int i = 0; i < listAxe->GetNumberOfTuples(); i++){
		if (axe == listAxe->GetValue(i)){
			return true;
		}
	}
	return false;
}

//On regarde les égalités entre les points selon un axe
bool IsEqualAccordingToAxe1(double *point1, double *point2, int dim, vtkFloatArray*listAxe){
	int cpt = 0;
	for (int axe = 0; axe < dim; axe++){
		if (Appartient1(axe, listAxe)){
			if (point1[axe] == point2[axe]){
				cpt++;
			}
		}
	}
	if (cpt == listAxe->GetNumberOfTuples()){
		return true;
	}
	return false;
}

//On appelle la focntion pour dessiner le KDtree
void DrawKDtree(KDtree1 *t){
	DrawTree(t->GetRoot(), 0);
}

//On dessine le KDtree
void DrawTree(Node1 *n, int depth){
	if (!n->GetIsLeaf()){ // les noeuds
		cout << "-";
		double *center = n->GetNode_Center()->GetTuple(0);
		cout << "-";
		int d = n->GetNode_Center()->GetNumberOfComponents();
		cout << "-";
		affiche(center, d);
		if (depth == 0)
			cout << endl;
		cout << "g" << (depth + 1);
		DrawTree(n->GetNode_LeftChild(), depth + 1);
		cout << "\n";
		cout << "d" << (depth + 1);
		DrawTree(n->GetNode_RightChild(), depth + 1);
		cout << "     " << endl;
	}
	else {// la feuille
		int d = n->GetLeaf()->GetNumberOfComponents();
		int infd = n->GetInfoLeaf()->GetNumberOfComponents();
		cout << "\n   f[";
		for (int i = 0; i < n->GetLeaf()->GetNumberOfTuples(); i++){
			affiche(n->GetLeaf()->GetTuple(i), d);
			affiche(n->GetInfoLeaf()->GetTuple(i), infd);
		}
		cout << "]    ";
	}

}
