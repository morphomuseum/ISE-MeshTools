#include "Icp.h"

#include <time.h>
#include <string.h>
#include <FL/Fl_Gl_Window.H> 
#include <FL/gl.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkHomogeneousTransform.h>
#include <vtkLandmarkTransform.h>
#include <vtkPointData.h>
#include <vtkDataSetAttributes.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyData.h>
#include <vtkMath.h>
#include <vtkIdList.h>
#include <vtkSortDataArray.h>
#include <vtkArrayDataWriter.h>
#include <vtkArrayWriter.h>
#include <vtkDenseArray.h>
#include <queue>

#include "Struct.h"
#include "KDtree.h"
#include "LandmarksTransform.h"



#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vtkVertexGlyphFilter.h>


inline void vtkMatrix4x4MultiplyPoint(const double elem[16], const double in[4], double out[4])
{
	double v1 = in[0];
	double v2 = in[1];
	double v3 = in[2];
	double v4 = in[3];

	out[0] = v1*elem[0] + v2*elem[1] + v3*elem[2] + v4*elem[3];
	out[1] = v1*elem[4] + v2*elem[5] + v3*elem[6] + v4*elem[7];
	out[2] = v1*elem[8] + v2*elem[9] + v3*elem[10] + v4*elem[11];
	out[3] = v1*elem[12] + v2*elem[13] + v3*elem[14] + v4*elem[15];
}

inline void Transpose(const double inElements[16],
	double outElements[16])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = i; j < 4; j++)
		{
			double temp = inElements[4 * i + j];
			outElements[4 * i + j] = inElements[4 * j + i];
			outElements[4 * j + i] = temp;
		}
	}
}

inline void MultiplyPoint(const double elements[16],
	const double in[4], double result[4])
{
	::vtkMatrix4x4MultiplyPoint(elements, in, result);
}


inline void PointMultiply(const double elements[16],
	const double in[4], double result[4])
{
	double newElements[16];
	::Transpose(elements, newElements);
	::MultiplyPoint(newElements, in, result);
}



 ///si trop de parametre faire une structure icp pour stocker tous les parametres-----> idée!
ICP::ICP(OBJECT_MESH* SourceData, OBJECT_MESH* TargetData, CONTAINER_MESH Cont_Mesh, vtkFloatArray*loaded_parameter_list, vtkFloatArray* tabOfParameters, vtkFloatArray* tab_loadsave_parameter, vtkFloatArray *tabDisplay, int modeTransformation, bool existColor, bool bool_onlyMatching)
{
	////////////////////////////////   RECUPERATION DES MAILLAGES SOURCE ET CIBLE
	vtkSmartPointer<vtkPolyData> S = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPolyData> T = vtkSmartPointer<vtkPolyData>::New();
	S->DeepCopy(SourceData);
	T->DeepCopy(TargetData);
	Cont_Mesh.Recover_PositionObject_in_PolyData(SourceData, S);
	Cont_Mesh.Recover_PositionObject_in_PolyData(TargetData, T);

	this->Source = SourceData;
	this->Target = TargetData;
	this->Cont_Mesh = Cont_Mesh;
			// on reinitialise leurs deux matrices et leurs centres
	Source->Mesh_init_Mat();
	Target->Mesh_init_Mat();
	Source->Compute_mean();
	Target->Compute_mean();


	/////////////  RECUPERATION DES LANDMARKS
	list_landmarks_source_test = vtkSmartPointer<vtkPoints>::New();
	list_landmarks_source = vtkSmartPointer<vtkPoints>::New();
	list_landmarks_target = vtkSmartPointer<vtkPoints>::New();
	list_landmarks_init_source = vtkSmartPointer<vtkPoints>::New();
	list_landmarks_init_target = vtkSmartPointer<vtkPoints>::New();
	

	this->bool_onlyMatching = bool_onlyMatching;
	if (!this->bool_onlyMatching){//mode recalage
		Cont_Mesh.RecoverLandmarks(list_landmarks_init_source, list_landmarks_init_target);
		numberOfLandmarks_Source = list_landmarks_init_target->GetNumberOfPoints();///  attention test 
		numberOfLandmarks_init = list_landmarks_init_target->GetNumberOfPoints();
	
		vtkSmartPointer<vtkPoints> list_land_source = vtkSmartPointer<vtkPoints>::New();
		for (int i = 0; i < numberOfLandmarks_init; i++){
			list_land_source->InsertNextPoint(list_landmarks_init_source->GetPoint(i));
		}
		list_landmarks_init_source->DeepCopy(list_land_source);
	}
	else{
		numberOfLandmarks_Source = 0;
		numberOfLandmarks_init = 0;
	}
	
	/////////////////////////////////////////////// CONSTRUCTION DES ATTRIBUTS

	////---------INITIALISATION DES LISTES DES POINTS
	Target->Update(T->GetPoints());
	Source->Update(S->GetPoints());
	numberOfPoints_Source = Source->GetPoints()->GetNumberOfPoints();

			/// creation de la liste des sommets source d'origine (point+normale)
	list_vertice_init_source = vtkSmartPointer<vtkFloatArray>::New();
	list_vertice_init_source->SetNumberOfComponents(7);
	vtkSmartPointer<vtkFloatArray> vertice_norms = vtkSmartPointer<vtkFloatArray>::New();
	vertice_norms = vtkFloatArray::SafeDownCast(Source->GetPointData()->GetNormals());
	double tuple[7];
	for (int i = 0; i < Source->GetNumberOfPoints(); i++){
		for (int j = 0; j < 3; j++){
			tuple[j] = Source->GetPoint(i)[j];
			tuple[j + 3] = vertice_norms->GetComponent(i, j);
		}
		tuple[6] = i;
		list_vertice_init_source->InsertNextTuple(tuple);
	}
		///---------------------

	
	///----- INITIALISATION DE LA LISTE DES PARAMETRES
	this->bool_save_parameters = tab_loadsave_parameter->GetTuple1(0);//bool pour enregistrer les paramètres
	this->bool_loaded_parameters = tab_loadsave_parameter->GetTuple1(1);//bool pour enregistrer les paramètres
	parameter_list = vtkSmartPointer<vtkFloatArray>::New();
	parameter_list->SetNumberOfComponents(12);
	parameter_list->SetNumberOfTuples(4);
	vtkSmartPointer<vtkIdList> modeList_toDo = vtkSmartPointer<vtkIdList>::New();
	modeList_toDo->DeepCopy(SetOfLoadedParameters(loaded_parameter_list, tabOfParameters, modeTransformation));

	

	
	/////--- NETTOYAGE DES COURBURES: MODIFIE LES VALEURS TROP GRANDES OU TROP PETITES
	percentageCurv = tabOfParameters->GetTuple1(9);// pourcentage pour les courbures
	Target->Sort_Curv_ICP("Maximum_Curvature", percentageCurv / 2.0f);
	Target->Sort_Curv_ICP("Minimum_Curvature", percentageCurv / 2.0f);
	Source->Sort_Curv_ICP("Maximum_Curvature", percentageCurv / 2.0f);
	Source->Sort_Curv_ICP("Minimum_Curvature", percentageCurv / 2.0f);
	
	//--------- CONSTRUCTION DES ALPHA POUR LA RECHERCHE DANS LE KDTREE
	double Pointmin_max[6];
	double norPointmin_max[6];
	double curvMinMax[8];// taille = 8 car la fonction "find_minMax_CoordofCurv" est generale pour toutes les courbures
	/// on cherche les maximums et minimums de chaque coordonnée
	Target->Find_minMax_CoordofPoint_Normal(Pointmin_max, norPointmin_max);
	Target->Find_minMax_CoordofCurv(curvMinMax);


	///---- CONSTRUCTION DES LISTES DES ERREURS ET DU TEMPS DE CALCUL ET LE SEUIL DES ERREURS
	errors = vtkSmartPointer<vtkFloatArray>::New();
	errors->SetNumberOfComponents(5);
	number_errors = 0;
	time = vtkSmartPointer<vtkFloatArray>::New();
	time->SetNumberOfComponents(4);
	threshold = new float[2];
	threshold[0] = tabOfParameters->GetTuple1(6);//ite_max
	threshold[1] = tabOfParameters->GetTuple1(7);// seuil pourcentage 

	tab_weights = vtkSmartPointer<vtkFloatArray>::New();
	IndexSphere_i_Source = vtkSmartPointer<vtkFloatArray>::New();
	list_landmarks_dimN_target_LA = vtkSmartPointer<vtkFloatArray>::New();

	

	///----- INITIALISATION DES PARAMETRES
	mode_transformation = -1;//En mode 'step by step', on commence par rigide par défaut
	weight_vertice_addland = -1;
	weight_land_addland = -1;
	dec = 1; //dec par defaut
	cpt_mode = 0;  // 0 si utilisation des landmarks et 1 si utilisation des points source et du kdtree
	dim_points = 4; /// (coord points + indice)
	percentageRadius_of_sphere_LA = tabOfParameters->GetTuple1(8);
	this->existColor = existColor;// les tags existent sur les deux maillages
	bool_tags = (tabOfParameters->GetTuple1(2) != -1 && existColor);
	Compute_dim_of_points(tabOfParameters);  /// calcule les parametres et les dimension des points
	Init_alpha_tab(Pointmin_max, norPointmin_max, curvMinMax);// construction des alpha pour le KDtree


	///----- INITIALISATION DE LA COULEUR ET TABLEAU D'AFFICHAGE DES CORRESPONDANCES
	tab_display = vtkSmartPointer<vtkFloatArray>::New();
	tab_display->SetNumberOfComponents(1);
	tab_display->SetNumberOfTuples(4);
	tab_display->DeepCopy(tabDisplay);
	vect_matching_color[0] = 0.f; // couleur des correspondances par défaut 
	vect_matching_color[1] = 0.5f;
	vect_matching_color[2] = 1.f;
	
	

	///--------INITIALISATION DES POIDS POUR LES CORRESPONDANCES
	weights = vtkSmartPointer<vtkFloatArray>::New();
	weights->SetNumberOfComponents(1);
	weights->SetNumberOfValues(numberOfLandmarks_Source);

	
	//-------- CONSTRUCTION DES POINTS A N DIMENSIONS

	list_landmarks_dimN_source = vtkSmartPointer<vtkFloatArray>::New();
	list_landmarks_dimN_source->SetNumberOfComponents(dim_points);
	list_landmarks_dimN_source->SetNumberOfTuples(Source->GetNumberOfPoints());
	list_landmarks_dimN_target = vtkSmartPointer<vtkFloatArray>::New();
	list_landmarks_dimN_target->SetNumberOfComponents(dim_points);
	list_landmarks_dimN_target->SetNumberOfTuples(Target->GetNumberOfPoints());
	
	Initialisation_of_points_ofDimN(Source, list_landmarks_dimN_source);
	Initialisation_of_points_ofDimN(Target, list_landmarks_dimN_target);
	
	

	/////-------- CONSTRUCTION DU KDTREE POUR DES POINTS A N DIMENSIONS
	cout << "  color? :: " << bool_tags << endl;
	clock_t debut;
	debut = clock();
	tree = new KDtree(list_landmarks_dimN_target, bool_tags);// on met comme indice la couleur si elle existe pour ne pas l'avoir dans la construction du kdtree (qu'elle est un axe) 
	cout << "-------   Temps consomme : " << ((double)(clock() - debut) / CLOCKS_PER_SEC) << endl;
	
	/*debut = clock();   // a enlever
	test = new KDtree1(list_landmarks_dimN_target, bool_tags);
	cout << "-------   Temps consomme : " << ((double)(clock() - debut) / CLOCKS_PER_SEC) << endl;
	*/


	//------- DEMARRAGE DE L'ICP
	if (!this->bool_onlyMatching){
		BeginICP(tabOfParameters, tab_loadsave_parameter, modeTransformation, tabDisplay, modeList_toDo);
	}
	else{
		OnlyMatching();
	}
}

// on calcule seulement les correspondances entre source et cible
void ICP::OnlyMatching(){
	list_landmarks_source->Reset();
	for (int i = 0; i < numberOfPoints_Source; i++){
		if (i%dec == 0){
			list_landmarks_source->InsertNextPoint(Source->GetPoint(i));
		}
	}
	numberOfLandmarks_Source = list_landmarks_source->GetNumberOfPoints();

	list_landmarks_dimN_source = vtkSmartPointer<vtkFloatArray>::New();
	list_landmarks_dimN_source->SetNumberOfComponents(dim_points);
	list_landmarks_dimN_source->SetNumberOfTuples(numberOfLandmarks_Source);
	Initialisation_of_points_ofDimN(Source, list_landmarks_dimN_source);

	list_landmarks_dimN_target = vtkSmartPointer<vtkFloatArray>::New();
	list_landmarks_dimN_target->SetNumberOfComponents(dim_points);
	list_landmarks_dimN_target->SetNumberOfTuples(list_landmarks_dimN_source->GetNumberOfTuples());
	
	Pairs_matching(list_landmarks_dimN_source, list_landmarks_dimN_target);

}

// debut de l'ICP
void ICP::BeginICP(vtkSmartPointer<vtkFloatArray> tabOfParameters, vtkFloatArray* tab_loadsave_parameter, int modeTransformation, vtkSmartPointer<vtkFloatArray> tabDisplay, vtkSmartPointer<vtkIdList> modeList_toDo){
	cout << "+++++++  DEBUT ICP  ++++++" << endl;
	// on recupere les parametres si on a charge un fichier sinon c'est ceux de l'interface
	if (!bool_loaded_parameters)
		SetData(tabOfParameters, modeTransformation, tabDisplay, tab_loadsave_parameter);
	else{
		for (int i = 0; i <modeList_toDo->GetNumberOfIds(); i++){
			SetData(tabOfParameters, modeList_toDo->GetId(i), tabDisplay, tab_loadsave_parameter);
		}
		bool_loaded_parameters = false;
	}
}



vtkSmartPointer<vtkIdList> ICP::SetOfLoadedParameters(vtkSmartPointer<vtkFloatArray> loaded_parameter_list, vtkSmartPointer<vtkFloatArray> tabOfParameters, int modeTransformation){
	vtkSmartPointer<vtkIdList> modeList_toDo = vtkSmartPointer<vtkIdList>::New();
	if (!bool_loaded_parameters){// si on n'a pas chargé les parametres d'un fichier
		for (int i = 0; i < 4; i++){
			float tuple_parameter[12] = { 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
			parameter_list->SetTuple(i, tuple_parameter);
		}
	}
	else{/// si on a chargé des parametres d'un fichier
		parameter_list->DeepCopy(loaded_parameter_list);
		Analyse_Loaded_Parameter_list(modeList_toDo);
		modeTransformation = modeList_toDo->GetId(0);
		SetTabOfParameter(tabOfParameters, modeList_toDo->GetId(0));//Mise en jour de tabofparameters
	}
	return modeList_toDo;
}




void ICP::Analyse_Loaded_Parameter_list(vtkIdList* modeList_toDo){
	//cette fonction permet de savoir les modes qui ont été fait 
	// et de désigner le mode final à executer sachant que 
	//similitude , affine , localement affine sont dépendant les uns des autres;
	//Excepté pour le mode rigide.
	if (1 == parameter_list->GetComponent(0, 0))
		modeList_toDo->InsertNextId(-1);
	if (1 == parameter_list->GetComponent(1, 0) && 1 != parameter_list->GetComponent(2, 0) && 1 != parameter_list->GetComponent(3, 0))
		modeList_toDo->InsertNextId(SIMILITUDE);
	if (1 == parameter_list->GetComponent(1, 0) && 1 == parameter_list->GetComponent(2, 0) && 1!=parameter_list->GetComponent(3, 0))
		modeList_toDo->InsertNextId(AFFINE);
	if (1 == parameter_list->GetComponent(1, 0) && 1 == parameter_list->GetComponent(2, 0) && 1 == parameter_list->GetComponent(3, 0))
		modeList_toDo->InsertNextId(LOC_AFFINE);


}

void ICP::SetTabOfParameter(vtkSmartPointer<vtkFloatArray>tabOfParameters, int modeTransformation){
	// on recupère pour chaque mode leurs parametres du fichier chargé dans la variable "tabOfParameters"

	int index = 0;
	if (modeTransformation == -1)
		index = 0;
	else if (modeTransformation == SIMILITUDE)
		index = 1;
	else if (modeTransformation == AFFINE)
		index = 2;
	else if (modeTransformation == LOC_AFFINE)
		index = 3;

	double *tuple = parameter_list->GetTuple(index);
	for (int i = 0; i < parameter_list->GetNumberOfComponents(); i++){
		tabOfParameters->SetTuple1(i, tuple[i + 2]);
	}
	//affiche(tuple,11);
}

void ICP::StoreParameters(){
	//Stocke les parametres de chaque mode
	if (bool_save_parameters){
		int index;
		if (mode_transformation == RIGID)
			index = 0;
		else if (mode_transformation == SIMILITUDE)
			index = 1;
		else if (mode_transformation == AFFINE)
			index = 2;
		else if (mode_transformation == LOC_AFFINE)
			index = 3;
		cout << "Store parameters " << index << endl;
		double tuple[12] = { 1, mode_transformation, tab_weights->GetTuple1(0), tab_weights->GetTuple1(1), tab_weights->GetTuple1(2), tab_weights->GetTuple1(3), dec, percentRadius_filter, threshold[0], threshold[1], percentageRadius_of_sphere_LA, percentageCurv };
		parameter_list->SetTuple(index, tuple);
		//affiche(tuple, 12);
	}
}



void ICP::Init_alpha_tab(double Pointmin_max[6], double norPointmin_max[6], double curvMinMax[8]){
// Calcule les alpha permettant à normaliser les coordonnées des points dans le kdtree
	Init_alpha_of_coord(Pointmin_max,norPointmin_max);
	Init_alpha_of_curv(curvMinMax);
	
	if (tab_weights->GetTuple1(1) != -1 && tab_weights->GetTuple1(0) == -1)
	//si dim_points a coordonnée du points et les courbures mais pas les coordonnées des normales
	{
		alpha[3] = alpha[6];
		alpha[4] = alpha[7];
	}

	/*cout << endl;
	affiche(alpha, 8);*/
}

void ICP::Init_alpha_of_coord(double Pointmin_max[6], double norPointmin_max[6]){
	// cette fonction calcule les alpha des coordonnées des points et des normales pour le kdtree
	double PXmax = Pointmin_max[3];
	double PXmin = Pointmin_max[0];
	double PYmax =  Pointmin_max[4];
	double PYmin = Pointmin_max[1];
	double PZmax = Pointmin_max[5];
	double PZmin = Pointmin_max[2];
	double NorXPmax = norPointmin_max[3];
	double NorXPmin = norPointmin_max[0];
	double NorYPmax = norPointmin_max[4];
	double NorYPmin =norPointmin_max[1];
	double NorZPmax = norPointmin_max[5];
	double NorZPmin = norPointmin_max[2];

	float coorp = max(max((PXmax - PXmin), (PYmax - PYmin)), (PZmax - PZmin));
	float norcoodp = max(max((NorXPmax - NorXPmin), (NorYPmax - NorYPmin)), (NorZPmax - NorZPmin));
	alpha[0] = 1.f / coorp;
	alpha[1] = 1.f / coorp ;// point
	alpha[2] = 1.f / coorp ;//point
	alpha[3] = 1.f / norcoodp;
	alpha[4] = 1.f / norcoodp;//normale
	alpha[5] = 1.f / norcoodp;//normale

	
	////// premiere idée
	//// on cherche le min des minimums entre chaque coordonnée du point puis celle de la normale
	//// de même pour la maximum
	//double Pmax = max(max(Pointmin_max[3], Pointmin_max[4]), Pointmin_max[5]);
	//double Pmin = min(min(Pointmin_max[0], Pointmin_max[1]), Pointmin_max[2]);
	//double NorPmax = max(max(norPointmin_max[3], norPointmin_max[4]), norPointmin_max[5]);
	//double NorPmin = min(min(norPointmin_max[0], norPointmin_max[1]), norPointmin_max[2]);
	//
	//alpha[0] = 1.f/(Pmax - Pmin);
	//alpha[3] = 1.f/(NorPmax - NorPmin);
	//alpha[1] = alpha[0];// point
	//alpha[2] = alpha[0];//point
	//alpha[4] = alpha[3];//normale
	//alpha[5] = alpha[3];//normale

	/////idée de l'article Fedmar
	//double PXmax = Pointmin_max[3];
	//double PXmin = Pointmin_max[0];
	//double PYmax =  Pointmin_max[4];
	//double PYmin = Pointmin_max[1];
	//double PZmax = Pointmin_max[5];
	//double PZmin = Pointmin_max[2];
	//double NorXPmax = norPointmin_max[3];
	//double NorXPmin = norPointmin_max[0];
	//double NorYPmax = norPointmin_max[4];
	//double NorYPmin =norPointmin_max[1];
	//double NorZPmax = norPointmin_max[5];
	//double NorZPmin = norPointmin_max[2];
	//
	//alpha[0] = 1.f / (PXmax - PXmin);
	//alpha[1] = 1.f / (PYmax - PYmin);// point
	//alpha[2] = 1.f / (PZmax - PZmin);//point
	//alpha[3] = 1.f / (NorXPmax - NorXPmin);
	//alpha[4] = 1.f / (NorYPmax - NorYPmin);//normale
	//alpha[5] = 1.f / (NorZPmax - NorZPmin);//normale
	
}

void ICP::Init_alpha_of_curv(double curvMinMax[8]){
	// on cherche les min et les max des deux coubures principales
	// curv1 = courbure du maximum et curv2 = coubure du minimum
	int index1 = 0, index2 = 0;
	bool bool_curv = (tab_weights->GetTuple1(1) != -1);
	double curv1Min, curv2Min;
	double curv1Max, curv2Max;
	double curv1_val = -1;
	double curv2_val = -1;
	if (bool_curv){
		
		index1 = 0; // index1 pour la courbure min
		index2 = 1;// index2 pour la courbure max
		// les autres index sont pour les autres courbures(moyenne et gaussienne)	
		
		curv1Min = curvMinMax[index1];
		curv1Max = curvMinMax[index1 + 4];
			
		curv2Min = curvMinMax[index2];
		curv2Max = curvMinMax[index2 + 4];

		curv1_val = 1.f / (curv1Max - curv1Min);
		curv2_val = 1.f / (curv2Max - curv2Min);
	}
	alpha[6] = curv1_val;
	alpha[7] = curv2_val;
}


void ICP::Compute_dim_of_points(vtkFloatArray* tabOfParameters){
	// cette fonction va recupèrer et/ou calculer pour la plus part des elements 
	//du tableau (la dimension des points, le diviseur du nombre de points, le pourcentage du 
	//rayon pour le filtre,
	// le poids des landmarks si ajouté avec les vertices).


	///---CONSTRUCTION DU TABLEAU DES POIDS CONSTANTS pour le kdtree (recuperer de l'interface)
	tab_weights->Reset();
	tab_weights->SetNumberOfComponents(1);
	dim_points = 4; /// par defaut on a au début que les coordonnées des points  + son indice
	dec_user = tabOfParameters->GetTuple(4)[0]; // le diviseur "dec" choisi par utilisateur
	percentRadius_filter = tabOfParameters->GetTuple(5)[0];// le pourcentage du rayon pour le filtre

	// Normale
	float weight_normale = -1;
	if (tabOfParameters->GetTuple1(0) != -1){
		weight_normale = tabOfParameters->GetTuple1(0);
		dim_points += 3;
	}


	//Courbure
	float weight_curv = -1;
	if (tabOfParameters->GetTuple1(1) != -1) {
		dim_points += 2;
		weight_curv = tabOfParameters->GetTuple1(1);
	}

	//Couleur
	float weight_color = -1;
	if (bool_tags) {
		dim_points+=1;
		weight_color=tabOfParameters->GetTuple1(2);
	}
	
	// Si on a les landmarks en plus des vertices
	bool_landmarks_add = false;
	weight_vertice_addland = -1;
	weight_land_addland = -1;

	if (tabOfParameters->GetTuple1(3) != -1){
		bool_landmarks_add = true;
		int div;
		if (dec_user != -1)
			div = dec_user;
		else
			div = numberOfPoints_Source / MAX_NUMBER_OF_POINTS;
		// on calcule les poids des landmarks ajoutés
		int numberVertice = numberOfPoints_Source / div;
		weight_land_addland = tabOfParameters->GetTuple1(3) * numberVertice;
		weight_vertice_addland = 1;
		if (weight_land_addland == numberVertice){ 
			// si on a le même poids (1) alors on prends que les landmarks.
			weight_vertice_addland = 0;
			weight_land_addland = 1;
		}
	}

	tab_weights->InsertNextTuple1(weight_normale); /// 0 --- normale
	tab_weights->InsertNextTuple1(weight_curv); /// 1 --- courbure
	tab_weights->InsertNextTuple1(weight_color); /// 2 --- couleur
	tab_weights->InsertNextTuple1(weight_land_addland); /// 3 --- landmarks

	/*cout << " tab_weights" << endl;
	for (int i = 0; i < 4; i++){
		cout << tab_weights->GetTuple1(i)<<"  ";
	}
	cout << endl;*/
}


void ICP::Initialisation_of_points_ofDimN(OBJECT_MESH *object, vtkSmartPointer<vtkFloatArray> list_landmarks_dimN_object){
	// ici on recupere les normales, courbures, et la couleur pour chaque point de l'object
	
	///------------ RECUPERATION DES NORMALES
	vtkSmartPointer<vtkFloatArray> object_norms = vtkSmartPointer<vtkFloatArray>::New();
	bool bool_norm = (tab_weights->GetTuple1(0) != -1);
	if (bool_norm){
		object_norms->DeepCopy(vtkFloatArray::SafeDownCast(object->GetPointData()->GetNormals()));
	}
	///------------- RECUPERATION COURBURE
	vtkFloatArray *currentCurv1= vtkFloatArray::New();
	currentCurv1->SetNumberOfComponents(1);
	
	vtkFloatArray *currentCurv2 = vtkFloatArray::New();
	currentCurv2->SetNumberOfComponents(1);
	bool bool_curv = (tab_weights->GetTuple1(1) != -1);
	if (bool_curv){
		currentCurv1->DeepCopy(vtkFloatArray::SafeDownCast(object->GetPointData()->GetScalars("Maximum_Curvature")));
		currentCurv2->DeepCopy(vtkFloatArray::SafeDownCast(object->GetPointData()->GetScalars("Minimum_Curvature")));
	}
	
	///------------- RECUPERATION COULEUR
	vtkFloatArray *currentTags = vtkFloatArray::New();
	currentTags->SetNumberOfComponents(1);
	if (bool_tags){
		currentTags->DeepCopy((vtkFloatArray*)(object->GetPointData()->GetScalars("Tags")));
	}

	int i1 = 0;
	float val = 0;
	list_landmarks_dimN_object->Reset();
	list_landmarks_dimN_object->SetNumberOfComponents(dim_points);
	list_landmarks_dimN_object->SetNumberOfTuples((int)(object->GetNumberOfPoints() / dec));
	for (int i = 0; i < object->GetNumberOfPoints(); i++){
		if (i%dec == 0){
			
				for (int j = 0; j < dim_points-1; j++){

					if (j < 3){// Point
						val = object->GetPoint(i)[j];
						
					}
					else if (3 <= j && j < 6){// Normale
						if (bool_norm)
							val = object_norms->GetComponent(i, j - 3);
						else if (!bool_norm && (j == 3 || j == 4)){
							if (j == 3){
								if (bool_curv){
									val = currentCurv1->GetComponent(i, 0);// Courbure max
								}
								else if (bool_tags && !bool_curv)
									val = currentTags->GetComponent(i, 0); // Couleur
							}
							else if (j == 4){
								val = currentCurv2->GetComponent(i, 0);// Coubure min
							}
						}
						else if (!bool_norm && bool_curv && j == 5){// Couleur
							if (bool_tags)
								val = currentTags->GetComponent(i, 0);
						}
					}
					else if (j == 6 || j == 7){// Courbure min et max
						if (j == 6){
							if (bool_curv){
								val = currentCurv1->GetComponent(i, 0);// Courbure max
							}
							else if (bool_tags && !bool_curv)
								val = currentTags->GetComponent(i, 0); // Couleur
						}
						else if (j == 7){
							val = currentCurv2->GetComponent(i, 0);// Coubure min
						}
					}
					else if (j == 8){// Couleur
						if (bool_tags)
							val = currentTags->GetComponent(i, 0);
					}
					list_landmarks_dimN_object->SetComponent(i1, j, val);
				}//for
				list_landmarks_dimN_object->SetComponent(i1, dim_points - 1, i1);
				i1++;
		}//if
	}//for

	currentCurv1->Delete();
	currentCurv2->Delete();
	currentTags->Delete();
}

//Destructeur
ICP::~ICP(){
	
}

// ICP iteration
void ICP::Iteration()
{
	// On a deux sortes d'itération : l'une pour Rigide, Similitude et Affine (R_S_A)
	// et l'autre pour le Localement Affine (LA)

	if (mode_transformation != LOC_AFFINE)
		IterationFor_R_S_A();
	else{
		IterationFor_LA();
	}
}

// ICP iteration pour Rigide, Similitude et Affine
void ICP::IterationFor_R_S_A()
{
	icp_cpt_mode = 0;
	float error_trans = 1;
	float err = 0, cpt_err_mode = 0;
	int MaxIte = threshold[0];
	float MaxErrorPercentage = threshold[1];
	vtkSmartPointer<vtkPoints> list_filter_source = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkPoints> list_filter_target = vtkSmartPointer<vtkPoints>::New();

	while ((abs(error_trans) > MaxErrorPercentage) && icp_cpt_mode < MaxIte && problem == 0){
		cout << " \n\t*********** ITE **** NUMBER ********     " << icp_cpt_mode << endl;
		
		weights->Reset();
		weights->SetNumberOfComponents(1);
		weights->SetNumberOfTuples(numberOfLandmarks_Source);
		list_filter_source->Reset();
		list_filter_target->Reset();

		//on cherche à construire les correspondances
		Pairs_matching(list_landmarks_dimN_source,list_landmarks_dimN_target);
		//on filtre les correspondances trouvées à partir des données que l'utilisateur a selectionné
		Filter(list_filter_source, list_filter_target);
		if (problem == 1) //on arrete la boucle si le filtre ne donne pas de correspondances
			break;
		//on calcule la transformation à partir des correspondances et mise à jour de la source
		Transformation(list_filter_source, list_filter_target);

		// recuperation des erreurs
		if (number_errors != 0){
			//on prends le dernier indice 
			vtkIdType lastId = number_errors - 1;
			cpt_err_mode++; /// à voir si on peut mettre "icp_cpt_mode" à la place de "cpt_err_mode"
			cout << "erreur " << errors->GetComponent(lastId, 0) << "  " << errors->GetComponent(lastId, 1) << "  " << errors->GetComponent(lastId, 2) << "  " << errors->GetComponent(lastId, 3) << endl;
			
			if (cpt_err_mode == 1){//pour la premiere iteration on prend l'erreur lui meme
				error_trans = errors->GetComponent(lastId, 1);// on prend sur les erreurs de la liste des correspondance source (landmark/vertice)
				cout << "erreur1 de distance entre les points ::::: " << error_trans << endl;
			}
			else if (cpt_err_mode >= 2){//sinon on calcule un pourcentage d'erreur entre l'erreur precedante et courante
				error_trans = abs(errors->GetComponent(lastId, 1) - errors->GetComponent(lastId - 1, 1) ) / errors->GetComponent(lastId - 1, 1);
				cout << "erreur de distance pred  ::::: " << error_trans << endl;
			}
		}
		icp_cpt_mode++;
	}
	//Mise a jour des positions des landmarks
	Cont_Mesh.Set_List_of_SourceLandmarks_Coordinates(list_landmarks_init_source);

}


// ICP iteration pour Localement affine
void ICP::IterationFor_LA(int radius){

	sphere_Set* sphereList = new sphere_Set[numberOfPoints_Source];
	IndexSphere_i_Source->Reset();
	IndexSphere_i_Source->SetNumberOfComponents(1);

	vtkSmartPointer<vtkPoints> new_list_source = vtkSmartPointer<vtkPoints>::New();
	new_list_source->SetNumberOfPoints(numberOfPoints_Source);

	UpdateCurvObject(Source, Source->GetPoints());

	icp_cpt_mode = 0;
	float error_trans = 1;
	float cpt_err_mode = 0;

	// calcule le rayon
	float R = 1.;
	float diameter = 2 * Source->object_radius;
	if (percentageRadius_of_sphere_LA <= 0)
		percentageRadius_of_sphere_LA = ((float)radius);
	R = percentageRadius_of_sphere_LA / 100. *diameter;
	cout << "rrayon " << R << endl;
	//recupère les seuils d'arret
	int MaxIte = threshold[0];
	float MaxErrorPercentage = threshold[1];// *2 * Target->object_radius / 100.;

	/// pretraitement des spheres pour chaque point Mk
	numberOfLandmarks_Source = numberOfPoints_Source;
	dec = 1;
	Initialisation_of_points_ofDimN(Source, list_landmarks_dimN_source);
	list_landmarks_source->Reset();
	list_landmarks_source->SetNumberOfPoints(numberOfPoints_Source);
	list_landmarks_target->Reset();
	list_landmarks_target->SetNumberOfPoints(numberOfPoints_Source);

	Build_SphereSetKDtree(sphereList, R);


	matriX* matrix_list = new matriX[numberOfPoints_Source];
	float err = 0;

	/// LES ITERATIONS 
	while ((abs(error_trans) > MaxErrorPercentage) && icp_cpt_mode < MaxIte && problem == 0){//MaxIte
		cout << " \n\t*********** ITE **** NUMBER ********     " << icp_cpt_mode << endl;

		// reinitialisation du KDtree source
		clock_t debut;
		debut = clock();
		err = 0;

		//---- ON CALCULE LES POINTS PLUS PROCHE ET LEUR TRANSFORMATION RIGIDE
		list_landmarks_dimN_target_LA->Reset();
		list_landmarks_dimN_target_LA->SetNumberOfComponents(list_landmarks_dimN_source->GetNumberOfComponents());
		list_landmarks_dimN_target_LA->SetNumberOfTuples(list_landmarks_dimN_source->GetNumberOfTuples());

	
		Pairs_matching(list_landmarks_dimN_source, list_landmarks_dimN_target_LA);
		/// filtrer ici selon la couleur si besoin

		clock_t debutMatrice;
		debutMatrice = clock();
		// liste des matrices
		Build_matrixlist(matrix_list, sphereList);

		// liste des nouveaux points
		new_list_source->Reset();
		Build_LA_transformation(matrix_list, sphereList, new_list_source, &err);

		double res_timeMatrice = (double)(clock() - debutMatrice) / CLOCKS_PER_SEC;
		cout << "Temps ::: Compute Matrice Affine  :: " << res_timeMatrice << " sec. " << endl;
		cout << "yyy"<<problem;
		if (problem==0)
			list_landmarks_source->DeepCopy(new_list_source);

		if (problem != 0){
			fl_alert("ERROR : Radius is too small %f", R);
			break;
		}

		// time
		double res_time = (double)(clock() - debut) / CLOCKS_PER_SEC;
		cout << "Temps iteration LA : " << res_time << " " << endl;
		if (problem == 0){
			//Calcule l'erreur de l'iteration pour le même mode
			error_trans = err / (float)(numberOfPoints_Source);
			double errors_tuple[5] = { mode_transformation, error_trans, -1, -1, cpt_mode };// a voir pour  les  landmarks et total
			errors->InsertNextTuple(errors_tuple);

			number_errors++;
			vtkIdType lastId = number_errors - 1;
			cpt_err_mode++;
			cout << "erreur " << errors->GetComponent(lastId, 0) << "  " << errors->GetComponent(lastId, 1) << "  " << errors->GetComponent(lastId, 2) << "  " << errors->GetComponent(lastId, 3) << endl;

			if (cpt_err_mode == 1){
				error_trans = errors->GetComponent(lastId, 1);// on prend sur les erreurs de la liste des correspondance source (landmark/vertice)
				cout << "erreur1 de distance entre les points ::::: " << error_trans << endl;
			}
			else if (cpt_err_mode >= 2){
				error_trans = abs(errors->GetComponent(lastId, 1) - errors->GetComponent(lastId - 1, 1)) / errors->GetComponent(lastId - 1, 1);
				cout << "erreur de distance pred  ::::: " << error_trans << endl;
			}

			
			UpdateCurvObject(Source, new_list_source);
			Initialisation_of_points_ofDimN(Source, list_landmarks_dimN_source);
		}
		icp_cpt_mode++;
	}// fin while

	if (problem == 0){// on fait les correspondances 
		list_landmarks_dimN_target_LA->Reset();
		list_landmarks_dimN_target_LA->SetNumberOfComponents(dim_points);
		list_landmarks_dimN_target_LA->SetNumberOfTuples(list_landmarks_dimN_source->GetNumberOfTuples());
		
		Pairs_matching(list_landmarks_dimN_source, list_landmarks_dimN_target_LA);// dim N
		// filtrer ici aussi 
	}
	cout << "mmmm" << endl;
	//free(matrix_list);
	cout << "Finish  LA " << endl;
}

//void ICP::Build_SphereSet(sphere_Set* sphereList, float R){
//	if (bool_kdtree_LA){ // a mettre dans constructeur et fichier des paramètres et updata 
//		Build_SphereSetKDtree(sphereList,R);
//	}
//	else{
//		Build_SphereSet_Neighbord(sphereList, R);
//	}
//}


// Construit les ensembles de sphere(Mk)
void ICP::Build_SphereSetKDtree(sphere_Set* sphereList, float R){

	cout << "construction du kdtree et de l'ensemble de sphere  en cours" << endl;
	vtkSmartPointer<vtkFloatArray>  list_landmarks_dimN_source_addIndex = vtkSmartPointer<vtkFloatArray>::New();
	list_landmarks_dimN_source_addIndex->SetNumberOfComponents(4);// point 3D + 1 pour l'indice
	list_landmarks_dimN_source_addIndex->SetNumberOfTuples(numberOfPoints_Source);
	
	for (int i = 0; i < numberOfPoints_Source; i++){
		double tuple[4] = { Source->GetPoint(i)[0], Source->GetPoint(i)[1], Source->GetPoint(i)[2], i };
		list_landmarks_dimN_source_addIndex->SetTuple(i, tuple);
		list_landmarks_source->SetPoint(i, tuple[0], tuple[1], tuple[2]);
	}

	clock_t debutKDt;
	debutKDt = clock();
	///-----------KDTREE
	cout << "KDtree Source" << endl;
	KDtree sourceTree(list_landmarks_dimN_source_addIndex, 0); // kdtree avec des points (coordonnée 3D + indice)
	cout << "fait" << endl;
	//----------
	// time
	double res_timeKDt = (double)(clock() - debutKDt) / CLOCKS_PER_SEC;
	cout << "Temps KDtree ::: " << res_timeKDt << " " << endl;


	clock_t debutSPhere;
	debutSPhere = clock();
	//----------------SPHERE
	cout << "Construction des Spheres: ";
	for (int i = 0; i < numberOfPoints_Source && problem == 0; i++){
		double point_3dim[3] = { Source->GetPoint(i)[0], Source->GetPoint(i)[1], Source->GetPoint(i)[2] };
		// on recupère tous les indices des points Ml dans un rayon R
		IndexSphere_i_Source->Reset();// = vtkSmartPointer<vtkFloatArray>::New();
		IndexSphere_i_Source->SetNumberOfComponents(1);
		IndexSphere_i_Source = sourceTree.PointsInRadius(point_3dim, R);
		if (IndexSphere_i_Source->GetNumberOfTuples() == 0){
		//	cout << "ii " << i << endl;
			problem += 2; //si on le remet à mettre dans parametre
			fl_alert("ERROR: the sphere radius for the transformation mode LOC_AFFINE\n is too small , radius = %f (chosen percentage : %f %)", R, percentageRadius_of_sphere_LA);
			break;// à tester par sure que cela marche
		}
		else{
			sphereList[i].list = vtkSmartPointer<vtkFloatArray>::New();
			sphereList[i].list->SetNumberOfComponents(1);
			sphereList[i].list->DeepCopy(IndexSphere_i_Source);
		}
	}
	cout << "fait" << endl;
	double res_timeSphere = (double)(clock() - debutSPhere) / CLOCKS_PER_SEC;
	cout << "Temps:: Sphere :: " << res_timeSphere << " " << endl;
}

//void ICP::Build_SphereSet_Neighbord(sphere_Set* sphereList, float R){
//	cout << "Neighbord" << endl;
//	Source->Find_Tab_of_neighborhood();
//	cout << "l" << endl;
//	Source->Compute_mean_of_edge();
//	cout << "mean  " << Source->mean_arr << endl;
//	cout << "yyynei" << endl;
//
//	queue<pair<float, float>> to_do;
//	cout << "rrr" << endl;
//
//	vtkSmartPointer<vtkPoints> list = vtkSmartPointer<vtkPoints>::New();
//	list->DeepCopy(Source->GetPoints());
//	vtkSmartPointer<vtkPoints> list1 = vtkSmartPointer<vtkPoints>::New();
//	list1->DeepCopy(Source->GetPoints());
//
//	//int i = 0;
//	for (int i = 0; i < numberOfPoints_Source  && problem==0; i++){
//		vtkSmartPointer<vtkFloatArray> done_list = vtkSmartPointer<vtkFloatArray>::New();
//		done_list->SetNumberOfComponents(2);
//		//cout << "eee" << endl;
//		pair<float, float> ref = { i, 0 };
//		//cout << "eee" << endl;
//		to_do.push(ref);
//		//cout << "eee" << endl;
//		int indexRef = i;
//		//cout << "eee" << endl;
//		double * pRef = Source->GetPoint(indexRef);
//		//cout << "eee" << endl;
//		while (!to_do.empty()){
//			int index_to_do = (to_do.front()).first;
//			float dist_to_do = (to_do.front()).second;
//			to_do.pop();
//			done_list->InsertNextTuple2(index_to_do, dist_to_do);
//			
//			double * pS = list->GetPoint(index_to_do);
//
//			set<int> temp = Source->set_neighborbhood[index_to_do];
//
//			//if (i == 0){
//			//	//cout << "   " << temp.size() << endl;
//			//}
//
//			//for (int j = 0; j <temp.size(); j++){/// on regarde ces voisins
//			std::set<int>::iterator it;
//				// verification qu'ils ont les mêmes couleurs 
//				//si une nouvelle couleur apparait la taille va changer
//			for (it = temp.begin(); it != temp.end(); ++it){
//				int indexN = *it;
//				
//				
//				//int indexN = *temp.begin();// pb
//				
//				double * pNeighbord = list1->GetPoint(indexN);
//				/*if (i == 0){
//					cout << " o " << indexN << endl; affiche(pNeighbord, 3); cout << endl;
//					cout << "  b  " << !IsEqual(pS, pNeighbord, 3) << "   " << !IsEqual(pRef, pNeighbord, 3) << "   " << !AppartientI(index_to_do, done_list) << endl;
//				}*/
//				if (!IsEqual(pS, pNeighbord, 3)&& !IsEqual(pRef,pNeighbord,3) && !AppartientI(indexN,done_list) ){
//					float distN = dist_to_do + sqrt(vtkMath::Distance2BetweenPoints(pS, pNeighbord));
//					/*if (i == 0)
//						cout << distN << endl;*/
//					if (distN<=(Source->mean_arr*2 )){
//						//cout << "la";
//						pair<float, float> val = {indexN,distN};
//						to_do.push(val);
//					}
//				}
//
//			}//end for
//
//		}//end while
//
//		if (done_list->GetNumberOfTuples() == 0){
//			//	cout << "ii " << i << endl;
//			problem += 2; //si on le remet à mettre dans parametre
//			fl_alert("ERROR: the sphere radius for the transformation mode LOC_AFFINE\n is too small , radius = %f (chosen percentage : %f %)", R, percentageRadius_of_sphere_LA);
//			break;// à tester par sure que cela marche
//		}
//		else{
//			vtkSmartPointer<vtkFloatArray> done_list_temp = vtkSmartPointer<vtkFloatArray>::New();
//			done_list_temp->SetNumberOfComponents(1);
//			for (int u = 0; u < done_list->GetNumberOfTuples(); u++){
//				done_list_temp->InsertNextTuple1(done_list->GetComponent(u, 0));
//			}
//
//
//			sphereList[i].list = vtkSmartPointer<vtkFloatArray>::New();
//			sphereList[i].list->SetNumberOfComponents(1);
//			sphereList[i].list->DeepCopy(done_list_temp);
//		}
//
//
//	}//end for
//		cout << sphereList[0].list->GetNumberOfTuples() << endl;
//		cout << "done" << endl;
//
//}


//Si un element Appartient à une liste, retourne vrai sinon faux
bool AppartientI(int axe, vtkFloatArray*listAxe){
	for (int i = 0; i < listAxe->GetNumberOfTuples(); i++){
		if (axe == listAxe->GetComponent(i,0)){
			return true;
		}
	}
	return false;
}

// construction des matrices de la transformation rigide (R,t) pour chaque point appartennant à la Source.
void ICP::Build_matrixlist(matriX* matrix_list, sphere_Set* sphereList){
	cout << "construction de la liste des matrices pour chaque point  en cours" << endl;
	vtkSmartPointer<vtkPoints> source_list = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkPoints> target_list = vtkSmartPointer<vtkPoints>::New();
	
	
	for (int i = 0; i < numberOfPoints_Source  && problem==0; i++){
		IndexSphere_i_Source->Reset();
		IndexSphere_i_Source->SetNumberOfComponents(1);
		IndexSphere_i_Source->SetNumberOfTuples(sphereList[i].list->GetNumberOfTuples());
		IndexSphere_i_Source->DeepCopy(sphereList[i].list);

		for (int k = 0; k < IndexSphere_i_Source->GetNumberOfTuples() && problem == 0; k++)
		{
			int index = IndexSphere_i_Source->GetTuple1(k);
			double sourcepoint[3] = { list_landmarks_dimN_source->GetTuple(index)[0],
				list_landmarks_dimN_source->GetTuple(index)[1],
				list_landmarks_dimN_source->GetTuple(index)[2] };
			double targetpoint[3] = { list_landmarks_dimN_target_LA->GetTuple(index)[0],
				list_landmarks_dimN_target_LA->GetTuple(index)[1],
				list_landmarks_dimN_target_LA->GetTuple(index)[2] };

			source_list->InsertNextPoint(sourcepoint);
			target_list->InsertNextPoint(targetpoint);
		}

		if (i % ((int)((float)numberOfPoints_Source/10.)) == 0 || i == numberOfPoints_Source - 1){
			if (i == 0)
				cout << "nb de matrix calcule : ";
			cout << i << "  (" << source_list->GetNumberOfPoints()<<")  -";
		}

		matrix_list[i].matrix = vtkSmartPointer<vtkMatrix4x4>::New();
		matrix_list[i].matrix->DeepCopy(MatrixTransformationForOnePoint(source_list, target_list));
		source_list->Reset();
		target_list->Reset();
	}
	
}

// Construction des matrices de la transformation affine (A,b)  pour chaque point appartennant à la Source.
void ICP::Build_LA_transformation(matriX* matrix_list, sphere_Set* sphereList, vtkSmartPointer<vtkPoints> new_list_source, float *err){

	cout << "construction pour chaque point la transformationLA   en cours" << endl;
	vtkSmartPointer<vtkFloatArray> Sphere_i_Source = vtkSmartPointer<vtkFloatArray>::New();
	Sphere_i_Source->SetNumberOfComponents(dim_points);
	vtkSmartPointer<vtkFloatArray> Sphere_i_Target = vtkSmartPointer<vtkFloatArray>::New();
	Sphere_i_Target->SetNumberOfComponents(dim_points);

	int indexPointiInSphere = 0;
	int i1 = 0;
	cout << list_landmarks_dimN_source->GetNumberOfComponents() << endl;
	///////pour chaque point de la source
	for (int i = 0; i < numberOfPoints_Source && problem==0; i++){
		// POUR CHAQUE POINT, ON RECUPERE SA SPHERE ET CALCULE LA TRANSFORMATION AFFINE LOCALE
		//cout << "tttt";
		// recupere un point de la source
		double *point_Ndim, Newpoint[3] = { 0, 0, 0 }, point_3dim[3] = { 0, 0, 0 };
		point_Ndim = list_landmarks_dimN_source->GetTuple(i);
		point_3dim[0] = point_Ndim[0];
		point_3dim[1] = point_Ndim[1];
		point_3dim[2] = point_Ndim[2];

		// on recupère tous les points de dim N dans un rayon R pour la sphere i
		IndexSphere_i_Source->Reset();
		IndexSphere_i_Source->SetNumberOfComponents(1);
		IndexSphere_i_Source->DeepCopy(sphereList[i].list);

		Sphere_i_Source->Reset();
		Sphere_i_Source->SetNumberOfComponents(dim_points + 1);

		// on recupère leurs points le plus proche
		Sphere_i_Target->Reset();
		Sphere_i_Target->SetNumberOfComponents(dim_points);
		
		for (int k = 0; k < IndexSphere_i_Source->GetNumberOfTuples(); k++)
		{
			int index = IndexSphere_i_Source->GetTuple1(k);
			double *tupleS = list_landmarks_dimN_source->GetTuple(index);
			//tupleS[dim_points] = index;
			double *tupleT = list_landmarks_dimN_target_LA->GetTuple(index);
			Sphere_i_Source->InsertNextTuple(tupleS);
			Sphere_i_Target->InsertNextTuple(tupleT);
		}

		ComputeNewPoint_LA(point_3dim, &indexPointiInSphere, Sphere_i_Source, Sphere_i_Target, Newpoint, matrix_list);
		//cout << "eee" << endl;
		if (problem == 0){
			list_landmarks_source->SetPoint(i, Newpoint);

			new_list_source->InsertNextPoint(Newpoint);
			///-------


			//--- CALCULE DES ERREURS
			double *targetPoint_i = Sphere_i_Target->GetTuple(indexPointiInSphere);
			double Tpoint[3] = { targetPoint_i[0], targetPoint_i[1], targetPoint_i[2] };

			*err += sqrt(vtkMath::Distance2BetweenPoints(Newpoint, Tpoint));
		}

		if (i % ((int)((float)numberOfPoints_Source / 10.)) == 0 || i == numberOfPoints_Source - 1){
			if (i == 0)
				cout << "nbde transformation LA calcule : ";
			cout << i1 << " / 10    -    ";
			i1++;
		}
		//-----

	}// end  for

	cout << (new_list_source->GetNumberOfPoints() == numberOfPoints_Source) << endl;
}

// Calcule les nouveaux points sources à l'aide de la matrice de la transformation affine.
void ICP::ComputeNewPoint_LA(double point_i[3], int *indexPoint_i, vtkSmartPointer<vtkFloatArray> Sphere_i_Source, vtkSmartPointer<vtkFloatArray> Sphere_i_Target, double transformedPointRef[3], matriX* matrix_list){
	int N = Sphere_i_Source->GetNumberOfTuples();
	vtkSmartPointer<vtkFloatArray> distance_list = vtkSmartPointer<vtkFloatArray>::New();
	distance_list->SetNumberOfComponents(1);
	distance_list->SetNumberOfTuples(N);
	vtkSmartPointer<vtkPoints> transformedpoint_list = vtkSmartPointer<vtkPoints>::New();
	transformedpoint_list->SetNumberOfPoints(N);

	float sumdis = 0;
	double sourcepoint[3], targetpoint[3];
	// on calcule les données (matrices, distance,somme)
	int indexMl = 0;
	for (int j = 0; j < N && problem == 0; j++){
		
		for (int k = 0; k < 3; k++){
			sourcepoint[k] = Sphere_i_Source->GetComponent(j, k);
			targetpoint[k] = Sphere_i_Target->GetComponent(j, k);
		}

		indexMl = Sphere_i_Source->GetComponent(j, dim_points-1);

		float dist = sqrt(vtkMath::Distance2BetweenPoints(point_i, sourcepoint));
		distance_list->SetTuple1(j, dist);
		double transformedPoint[3] = { 0, 0, 0 };
		transformPoint(matrix_list[indexMl].matrix, point_i, transformedPoint);

		if (transformedPoint[0] != transformedPoint[0])// on verifie si IND ou pas 
			problem = 1;

		if (problem == 0){
			transformedpoint_list->SetPoint(j, transformedPoint);
			sumdis += 1. - (distance_list->GetTuple1(j) / (Source->object_radius * 2));
		}
	}
	//cout << "uuu";
	float beta = 0; double *trans;
	// on calcule la transformation affine de point_ref  
	// N-1 car on a enlever le point source
	for (int j = 0; j < N && problem == 0 ; j++){
		beta = (1 - distance_list->GetTuple1(j) / (Source->object_radius * 2)) / sumdis;
		trans = transformedpoint_list->GetPoint(j);
		transformedPointRef[0] += beta*trans[0];
		transformedPointRef[1] += beta*trans[1];
		transformedPointRef[2] += beta*trans[2];
		
	}
}


//On calcule la transformation entre deux listes de points.
vtkMatrix4x4* ICP::MatrixTransformationForOnePoint(vtkSmartPointer<vtkPoints> sourcepoint, vtkSmartPointer<vtkPoints> targetpoint){

	vtkSmartPointer<LandmarksTransform> transformation_landmarks = vtkSmartPointer<LandmarksTransform>::New();
	transformation_landmarks->SetSourceLandmarks(sourcepoint);
	transformation_landmarks->SetTargetLandmarks(targetpoint);
	transformation_landmarks->SetMode(RIGID);
	transformation_landmarks->Update();

	return transformation_landmarks->GetMatrix();
}




// On cherche les correspondances entre les listes des points source et cible
void ICP::Pairs_matching(vtkSmartPointer<vtkFloatArray>list_landmarks_dimN_source, vtkSmartPointer<vtkFloatArray>list_landmarks_dimN_target){

	if (weight_land_addland != 1){ //si on traite les points de la source
		if (Mode_using_Source_points() || mode_transformation == LOC_AFFINE || bool_onlyMatching){

			vtkIdType indexClosestPoint = 0;
			float dist;
			double* closestPoint=NULL;
			double* closestPoint1=NULL;
			float err = 0;

			double val = 0;

			cout << " pairs matching () en cours : ";
			clock_t debutPair;
			debutPair = clock();
			list_landmarks_dimN_target->Reset();
			list_landmarks_dimN_target->SetNumberOfComponents(dim_points);
			list_landmarks_dimN_target->SetNumberOfTuples(list_landmarks_dimN_source->GetNumberOfTuples());

			list_landmarks_target->Reset();
			list_landmarks_target->SetNumberOfPoints(list_landmarks_dimN_source->GetNumberOfTuples());

			vtkSmartPointer<vtkFloatArray> pointc = vtkSmartPointer<vtkFloatArray>::New();
			pointc->SetNumberOfComponents(dim_points);
			pointc->SetNumberOfTuples(1);

			
			for (vtkIdType i = 0; i < list_landmarks_dimN_source->GetNumberOfTuples(); i++){
				//	//On cherche dans la kd tree de la cible le point le plus proche de chaque point de la liste Source 
				
				vtkSmartPointer<vtkFloatArray> pointSource = vtkSmartPointer<vtkFloatArray>::New();
				pointSource->SetNumberOfComponents(dim_points - 1);
				pointSource->SetNumberOfTuples(1);
				double *Point;
				//on copie le point en ne prennant pas l'indice
				for (vtkIdType j = 0; j < dim_points - 1; j++){
					val = list_landmarks_dimN_source->GetComponent(i, j);
					pointSource->SetComponent(0, j, val);
				}
				Point = pointSource->GetTuple(0); 

				//recherche du point le plus proche
				pointc ->DeepCopy( tree->Closest_to_pt(Point, &dist, tab_weights, alpha));
				list_landmarks_dimN_target->SetTuple(i, pointc->GetTuple(0));
				list_landmarks_target->SetPoint(i, pointc->GetComponent(0, 0), pointc->GetComponent(0, 1), pointc->GetComponent(0, 2));

			}//for
			
			cout << " done." << endl;
			double res_timePair = (double)(clock() - debutPair) / CLOCKS_PER_SEC;
			cout << "Temps ::: Pairs_matching  :: " << res_timePair << " sec. " << endl;
		}
	}
}

//on filtre et on donne un poids aux correspondances
void ICP::Filter(vtkSmartPointer<vtkPoints> list_filter_source, vtkSmartPointer<vtkPoints> list_filter_target){
	cout << " filter () en cours :";
	vtkSmartPointer<vtkFloatArray> newWeights = vtkSmartPointer<vtkFloatArray>::New();
	
	if (Mode_using_Source_points()){
		if (weight_land_addland != 1){// si on a des vertices 
			// fonctions qui augmentent le poids d'une correspondance 
			Sort_matchings_for_color(list_filter_source, list_filter_target);//si les deux points ont la même couleur 
			Sort_matchings_distances(list_filter_source, list_filter_target);// si la distance dans un correspondance est < seuil

			for (int i = 0; i < numberOfLandmarks_Source; i++){// on enlève les poids de correspondances qui sont à 0
				if (weights->GetTuple1(i) != 0){
					list_filter_source->InsertNextPoint(list_landmarks_source->GetPoint(i));
					list_filter_target->InsertNextPoint(list_landmarks_target->GetPoint(i));
					newWeights->InsertNextValue(weights->GetTuple1(i));
				}
			}
			
		}
		if (bool_landmarks_add && weight_land_addland!=-1){// si en plus on a des landmarks, on les rajoute
			for (int i = 0; i < numberOfLandmarks_init; i++){
				list_filter_source->InsertNextPoint(list_landmarks_init_source->GetPoint(i));
				list_filter_target->InsertNextPoint(list_landmarks_init_target->GetPoint(i));
				newWeights->InsertNextValue(weight_land_addland);
			}
		}
	}
	else{//si rigide et similitude land 
		for (int i = 0; i < numberOfLandmarks_init; i++){
			newWeights->InsertNextValue(1);
		}
	}
	weights->Reset();
	weights->DeepCopy(newWeights);
	if (weights->GetNumberOfTuples() == 0){
		fl_alert("ERROR:: filter() not Matching :: %d %", percentRadius_filter*100);
		problem = 1;
	}
	cout << "fait" << endl;
}

//Donne un poids aux correspondances si leurs distances sont inférieur à un seuil défini dans l'interface
void ICP::Sort_matchings_distances(vtkSmartPointer<vtkPoints> list_filter_source, vtkSmartPointer<vtkPoints> list_filter_target){
	float radius_min = 0;
	// on prends le rayon le plus petit pour 
	radius_min = min(Target->object_radius, Source->object_radius);
	float per_radius = percentRadius_filter * radius_min;

	float dist = 0;
	for (int i = 0; i < numberOfLandmarks_Source; i++){
		dist = vtkMath::Distance2BetweenPoints(list_landmarks_source->GetPoint(i),list_landmarks_target->GetPoint(i));
		if (dist < per_radius){
			if (tab_weights->GetTuple1(2) != -1)//la couleur est pris en compte
				weights->SetTuple1(i, weights->GetTuple1(i)+ 1);
			else
				weights->SetTuple1(i, 1);
		}
		else{
			weights->SetTuple1(i, 0);
		}
		
	}
}

//Donne un poids aux correspondances si point source te point cible ont la même couleur
void ICP::Sort_matchings_for_color(vtkSmartPointer<vtkPoints> list_filter_source, vtkSmartPointer<vtkPoints> list_filter_target){
	if (bool_tags){// si on a un poids pour la couleur 
		int color = 7;
		for (int i = 0; i < numberOfLandmarks_Source; i++){// on ne prend pas les landmarks initiaux
			//on augmente le poids avec celle de la couleur défini dans l'interface
			if (list_landmarks_dimN_source->GetTuple(i)[color] == list_landmarks_dimN_target->GetTuple(i)[color])
			{
				weights->SetValue(i,tab_weights->GetTuple(2)[0]);
			}
			else{
				weights->SetValue(i, 0);
			}
		}//for end
	}
}

// Calcule la transformation et donne le nouveau maillage source
void ICP::Transformation(vtkSmartPointer<vtkPoints> list_filter_source, vtkSmartPointer<vtkPoints> list_filter_target){
	float erreurTotal = 0;
	float erreurLand = 0;

	vtkSmartPointer<vtkPoints> new_list_source = vtkSmartPointer<vtkPoints>::New();

	cout << " Transformation() en cours :";
	vtkSmartPointer<vtkPoints> list_landSource = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkPoints> list_landTarget = vtkSmartPointer<vtkPoints>::New();
	if (Mode_using_Source_points() && numberOfLandmarks_init!=numberOfLandmarks_Source){
		list_landSource->DeepCopy(list_filter_source);
		list_landTarget->DeepCopy(list_filter_target);
	}
	else{
		list_landSource->DeepCopy(list_landmarks_init_source);
		list_landTarget->DeepCopy(list_landmarks_init_target);
	}

	bool deformableAndAddLandmarks = (bool_landmarks_add && Mode_using_Source_points());

	//calcule la transformation selon le mode de transformation
	vtkSmartPointer<LandmarksTransform> transformation_landmarks = vtkSmartPointer<LandmarksTransform>::New();
	transformation_landmarks->SetSourceLandmarks(list_landSource);
	transformation_landmarks->SetTargetLandmarks(list_landTarget);
	transformation_landmarks->SetLandmarksWeight(weights);
	transformation_landmarks->SetMode(mode_transformation);
	transformation_landmarks->Update();

	vtkSmartPointer<vtkMatrix4x4> matrix = transformation_landmarks->GetMatrix();

	double pointNew[3]; int i1 = 0, cpt_ele = 0;
	for (int i = 0; i < numberOfPoints_Source; i++){
		
		if (i < numberOfLandmarks_init){// modification des landmarks 
			transformPoint(matrix, list_landmarks_init_source->GetPoint(i), pointNew);
			list_landmarks_init_source->SetPoint(i, pointNew);
			erreurLand += sqrt(vtkMath::Distance2BetweenPoints(pointNew, list_landmarks_init_target->GetPoint(i)));
			// mise à jour des landmarks se trouvant dans la list_landmark_source  ( vertice + landmarks)
			if (deformableAndAddLandmarks && numberOfLandmarks_init != numberOfLandmarks_Source){
				list_landmarks_source->SetPoint(i + numberOfLandmarks_Source, pointNew);
				erreurTotal += sqrt(vtkMath::Distance2BetweenPoints(pointNew, list_landmarks_init_target->GetPoint(i)));
				cpt_ele++;
			}
			// mise à jour des landmarks se trouvant dans la list_landmark_source (utilisation seulement des landmarks)
			if (!Mode_using_Source_points() || numberOfLandmarks_init == numberOfLandmarks_Source){
				list_landmarks_source->SetPoint(i, pointNew);
				erreurTotal += sqrt(vtkMath::Distance2BetweenPoints(pointNew, list_landmarks_target->GetPoint(i)));
				cpt_ele++;
			}
		}
		transformPoint(matrix, Source->GetPoint(i), pointNew);
		new_list_source->InsertNextPoint(pointNew);
		if (Mode_using_Source_points() && numberOfLandmarks_init != numberOfLandmarks_Source){
			if (i%dec == 0){ // Update of the landmark list for Source
				list_landmarks_source->SetPoint(i1, pointNew);
				erreurTotal += sqrt( vtkMath::Distance2BetweenPoints(pointNew, list_landmarks_target->GetPoint(i1)));
				i1++;
				cpt_ele++;
			}
		}
	}
	cout << " fait." << endl;
	// mise a jour de la source.
	UpdateCurvObject(Source,new_list_source);
	if (Mode_using_Source_points()){
		Initialisation_of_points_ofDimN(Source, list_landmarks_dimN_source);
	}
	////compute the distance error between corresponding points.
	erreurLand /= (float)(numberOfLandmarks_init);
	erreurTotal /= (float)(cpt_ele);
	number_errors++;
	double errors_tuple[5] = { mode_transformation, erreurTotal, erreurLand, (erreurTotal + erreurLand), cpt_mode };
	errors->InsertNextTuple(errors_tuple);

	
}

//Mise à jour de l'objet avec ses courbures
void ICP::UpdateCurvObject(OBJECT_MESH *object, vtkSmartPointer<vtkPoints> newlist){
	if (newlist != NULL && newlist->GetNumberOfPoints()==Source->GetNumberOfPoints()){
		object->Update(newlist);

		/// A METTRE DANS UNE FONCTION ET A PLACER AUSSI DANS SETDATA SI ON CHANGE LES PARAMETRES 
		if (*(tab_weights->GetTuple(1)) != -1){//courbure
			object->selected = 1;
			Cont_Mesh.SC_calc_curv(1);/// Max
			Cont_Mesh.SC_calc_curv(2);/// Min

			object->selected = 1;
			object->SC_calc_mean();// on fait 2 lissages
			object->SC_calc_mean();
			object->selected = 0;
			object->Sort_Curv_ICP("Maximum_Curvature", (percentageCurv/2.f));
			object->Sort_Curv_ICP("Minimum_Curvature", (percentageCurv/2.f));
		}
	}
}

// calcule la transformation d'un point grace à une matrice (4*4)
void ICP::transformPoint(vtkMatrix4x4* matrix, double pointin[3], double pointout[3]){
	double pointPred[4]; double pointNew[4] = { 0, 0, 0, 0 };
	pointPred[0] = pointin[0];
	pointPred[1] = pointin[1];
	pointPred[2] = pointin[2];
	pointPred[3] = 1;
	
	matrix->MultiplyPoint(pointPred, pointNew);
	pointout[0] = pointNew[0];
	pointout[1] = pointNew[1];
	pointout[2] = pointNew[2];
}

// booléen définissant l'urilisation des points source
bool ICP::Mode_using_Source_points(){
	bool b = false;
	if ((mode_transformation == RIGID && cpt_mode == 1) || (mode_transformation == SIMILITUDE && cpt_mode == 1) || mode_transformation == AFFINE /*|| mode_transformation == LOC_AFFINE*/)
		b = true;
	return b;
}

//boolen permettant de comparer deux tableaux si ils sont identiques true sinon false
bool TabAreEqual(vtkSmartPointer<vtkFloatArray> t_new, vtkSmartPointer<vtkFloatArray> t){
	bool b = true;
	int n = t->GetNumberOfComponents();
	for (int i = 0; i < t->GetNumberOfTuples(); i++){
		affiche(t_new->GetTuple(i),n);
		affiche(t->GetTuple(i), n);
		b &= IsEqual(t_new->GetTuple(i), t->GetTuple(i), n);
		cout << "b : " << b << endl;
	}
	return b;
}

//booleen permettant de savoir si on doit refaire le kdtree ou pas (en fonction du changement de la dimension des points) 
bool ICP::RedoTree(vtkSmartPointer<vtkFloatArray> tabOfParametre, vtkSmartPointer<vtkFloatArray> tab_weights){
	
	vtkSmartPointer<vtkFloatArray> tabPOut = vtkSmartPointer<vtkFloatArray>::New();
	tabPOut->SetNumberOfComponents(tab_weights->GetNumberOfComponents());
	tabPOut->SetNumberOfTuples(tab_weights->GetNumberOfTuples());
	vtkSmartPointer<vtkFloatArray> tabWOut = vtkSmartPointer<vtkFloatArray>::New();
	tabWOut->SetNumberOfComponents(tab_weights->GetNumberOfComponents());
	tabWOut->SetNumberOfTuples(tab_weights->GetNumberOfTuples());

	AnalyseTab(tabOfParametre,  tabPOut);
	AnalyseTab(tab_weights, tabWOut);
	if (!TabAreEqual(tabPOut, tabWOut)){
		return 1;
	}
	return 0;
}

//on regarde si les elements du tab existent si oui on met 1 dans le tabOut sinon 0.
void ICP::AnalyseTab(vtkSmartPointer<vtkFloatArray> tab, vtkSmartPointer<vtkFloatArray> tabOut){

	for (int i = 0; i < tabOut->GetNumberOfTuples(); i++){
		if (tab->GetTuple1(i) != -1){
			tabOut->SetTuple1(i, 1);
		}
		else{
			tabOut->SetTuple1(i, 0);
			cout << "res   :: anatab   " << tab->GetTuple1(i) << endl;
		}
	}

}

//Mise à jour des paramètres de l'interface
int ICP::UpdateParameters(vtkSmartPointer<vtkFloatArray> tabOfParameters, vtkSmartPointer<vtkFloatArray> tabDisplay){
	int cpt_reinit_Kdtree = 0, cpt_redo = 0;
	problem = 0;

	if (tabOfParameters != NULL){
		
		// le diviseur des points 
		if (dec_user != tabOfParameters->GetTuple(4)[0]){
			dec_user = tabOfParameters->GetTuple(4)[0];
			cpt_redo += 1;
			Initialise_dec();
		}

		// le pourcentage du filtre des distances
		if (percentRadius_filter != tabOfParameters->GetTuple(5)[0]){
			percentRadius_filter = tabOfParameters->GetTuple(5)[0];
			cpt_redo += 2;
		}

		// les seuils d'arret
		if ((threshold[0] != tabOfParameters->GetTuple1(6) || threshold[1] != tabOfParameters->GetTuple1(7))){
			threshold[0] = tabOfParameters->GetTuple1(6);//ite_max
			threshold[1] = tabOfParameters->GetTuple1(7);// seuil pourcentage 
			cpt_redo += 4;
		}

		// le pourcentage du rayon pour la sphere dans le mode localement affine
		if (percentageRadius_of_sphere_LA != tabOfParameters->GetTuple1(8)){
			percentageRadius_of_sphere_LA = tabOfParameters->GetTuple1(8);
			cpt_redo += 7;
		}

		// le pourcentage pour modifier les courbures extrèmes
		if (percentageCurv != tabOfParameters->GetTuple1(9)){
			percentageCurv = tabOfParameters->GetTuple1(9);

			UpdateCurvObject(Target);
			UpdateCurvObject(Source);
			Target->Sort_Curv_ICP("Maximum_Curvature", percentageCurv / 2.0f);
			Target->Sort_Curv_ICP("Minimum_Curvature", percentageCurv / 2.0f);
			Source->Sort_Curv_ICP("Maximum_Curvature", percentageCurv / 2.0f);
			Source->Sort_Curv_ICP("Minimum_Curvature", percentageCurv / 2.0f);

			cpt_redo += 15;
			cpt_reinit_Kdtree += 1;
		}




		//Mise à jour des poids
		
		// Si la couleur n'existe pas et que l'on a mis un poids à la couleur 
		// c'est une erreur de l'utilisateur alors on met -1 au poids couleur pour qu'il ne refasse pas
		// tous les calculs du kdtree
		if (existColor == false && tabOfParameters->GetTuple1(2) != -1){
			problem = 3;
			fl_alert("ERROR: the color data do not existe for the meshes.");// a voir 
			tabOfParameters->SetTuple1(2, -1);
		}

		// si l'utilisateur à changer la dimension des points
		if (!TabAreEqual(tabOfParameters, tab_weights) || cpt_reinit_Kdtree != 0){
			  
			if ((tabOfParameters->GetTuple1(1) == -1 && -1 != tab_weights->GetTuple1(1)) || (tabOfParameters->GetTuple1(1) != -1 && -1 == tab_weights->GetTuple1(1)))
				UpdateCurvObject(Source);
			
			bool redoTree = (RedoTree(tabOfParameters, tab_weights) || cpt_reinit_Kdtree!=0);
			cout <<"redo KDTREE :  "<< redoTree << endl;

			Compute_dim_of_points(tabOfParameters);
			cpt_redo += 14;


			for (int i = 0; i < 4; i++){
				affiche(tab_weights->GetTuple(i), 1);
			}

			if (redoTree){
				cpt_reinit_Kdtree += 14;

				list_landmarks_dimN_source->Reset();
				list_landmarks_dimN_source->SetNumberOfComponents(dim_points);
				list_landmarks_dimN_source->SetNumberOfTuples(Source->GetNumberOfPoints());
				list_landmarks_dimN_target->Reset();
				list_landmarks_dimN_target->SetNumberOfComponents(dim_points);
				list_landmarks_dimN_target->SetNumberOfTuples(Target->GetNumberOfPoints());


				// par defaut --> change dans reinit() quand change de mode
				Initialisation_of_points_ofDimN(Source, list_landmarks_dimN_source);

				// comme on reconstruit le KDtree on doit avoir tous les points de la cible.
				int dec_tmp = dec;
				dec = 1;
				Initialisation_of_points_ofDimN(Target, list_landmarks_dimN_target);
				dec = dec_tmp;
				cout << "KDtree :: ";
				tree = new KDtree(list_landmarks_dimN_target, bool_tags);
				cout << "fait " << endl;


				//////   tab alpha 
				double Pointmin_max[6];
				double norPointmin_max[6];
				double curvMinMax[8];// taille = 8 car la fonction "find_minMax_CoordofCurv" est generale pour toutes les courbures
				/// on cherche les maximums et minimums de chaque coordonnée
				Target->Find_minMax_CoordofPoint_Normal(Pointmin_max, norPointmin_max);
				Target->Find_minMax_CoordofCurv(curvMinMax);

				Init_alpha_tab(Pointmin_max,norPointmin_max,curvMinMax);
				
			}

		}
	}

	// Mise en jour du tableau d'affichage
	if (tabDisplay != NULL && !TabAreEqual(tabDisplay, tab_display)){
		tab_display->DeepCopy(tabDisplay);
	}

	return cpt_reinit_Kdtree +cpt_redo;

}

//Modification des données : les paramètres et le mode de transformation
void ICP::SetData(vtkSmartPointer<vtkFloatArray> tabOfParameters, int modeTransformation, vtkSmartPointer<vtkFloatArray> tabDisplay, vtkFloatArray* tab_loadsave_parameter, bool bool_only_matchings){
	
	if (tab_loadsave_parameter != NULL){/// On charge ou sauvegarde les parametres
		// on sauvegarde les paramètres
		if (bool_save_parameters != tab_loadsave_parameter->GetTuple1(0)){
			bool_save_parameters = tab_loadsave_parameter->GetTuple1(0);
		}
	}


	if (bool_loaded_parameters) // si un fichier de paramètres a été chargé, on prends cela 
		SetTabOfParameter(tabOfParameters, modeTransformation);

	redo = UpdateParameters(tabOfParameters, tabDisplay);

	cout <<"bool_onlyMatching :: "<< bool_onlyMatching << endl;
	bool_onlyMatching = bool_only_matchings;
	if (!bool_onlyMatching){// ICP
		UpdateMode(modeTransformation);
	}
	else{
		mode_transformation = 0;
		OnlyMatching();
	}
}

// Mise a jour du mode 
void ICP::UpdateMode(int modeTransformation){

	// reitération du mode LA
	bool_LA_redo = (modeTransformation == LOC_AFFINE) && (modeTransformation == mode_transformation);

	// verification de modeTransformation == mode_transformation
	if (redo != 0 && problem == 0){// si on a changé les parametres
		// on fait le mode precedent
		if (modeTransformation == RIGID){
			mode_transformation = -1;
		}
		else if (modeTransformation == SIMILITUDE){
			mode_transformation = RIGID;
		}
		else if (modeTransformation == AFFINE){
			mode_transformation = RIGID;
		}
		else if (modeTransformation == LOC_AFFINE){
			mode_transformation = AFFINE;
			
		}
	}


	if (modeTransformation != -2 && modeTransformation != mode_transformation && problem == 0){
		if (modeTransformation == RIGID){
			mode_transformation = -1;
			cpt_mode = 0;
			SetMode();//rigide 0(landmark) et 1(vertice)
		}
		else if (modeTransformation == SIMILITUDE){
			mode_transformation = RIGID;
			cpt_mode = 0;
			SetMode();//similitude 0 et 1
			cpt_mode = 0;
		}
		else if (modeTransformation == AFFINE){
			cpt_mode = 0;
			mode_transformation = RIGID;
			SetMode();//similitude 0 et 1
			SetMode();// affine
			cpt_mode = 0;
		}
		else if (modeTransformation == LOC_AFFINE){
			mode_transformation = AFFINE;
			if (bool_LA_redo == false){
				mode_transformation = RIGID;
				SetMode();//similitude 0 et 1
				SetMode();// affine
				bool_LA_redo = true;
			}
			else if (bool_LA_redo == true){
				mode_transformation = AFFINE;
			}
			SetMode();// loc_affine
		}
	}
	else if (modeTransformation == -2 && problem == 0){// mode stepbystep
		SetMode();// rigide
	}
}

// Donne le nom du mode en fonction de sa valeur
char* ICP::GetMode(int mode){
	if (mode == -1)
		mode = mode_transformation;

	if (mode == RIGID || mode == 0)
		return "RIGID";
	else if (mode == SIMILITUDE || mode == 1)
		return "SIMILITUDE";
	else if (mode == AFFINE || mode == 2)
		return "AFFINE";
	else if (mode == LOC_AFFINE || mode == 3){
		return "LOCALLY_AFFINE";
	}
	else
		return "";
}

// donne sa valeur en fonction du nom
int ICP::GetModeInv(char *mode){
	if (strcmp(mode,"RIGID")==0)
		return RIGID;
	else if (strcmp(mode,"SIMILITUDE") == 0)
		return SIMILITUDE;
	else if (strcmp(mode,"AFFINE") == 0)
		return AFFINE;
	else if (strcmp(mode, "LOCALLY_AFFINE") == 0){
		return LOC_AFFINE;
	}
}

// Modification du mode et lance l'ICP
void ICP::SetMode(){
	if (mode_transformation != LOC_AFFINE){
		clock_t debut;
		debut = clock();
		string name_mode = "";
		if (mode_transformation == -1){
			name_mode = "RIGID";
			mode_transformation = RIGID;
		}
		else if (mode_transformation == RIGID){
			name_mode = "SIMILITUDE";
			mode_transformation = SIMILITUDE;
		}
		else if (mode_transformation == SIMILITUDE){
			name_mode = "AFFINE";
			mode_transformation = AFFINE;
			cpt_mode = 0;
		}
		else if(mode_transformation == AFFINE){
			name_mode = "LOC_AFFINE";
			mode_transformation = LOC_AFFINE;
		}
	
		cout << "\t\t\tMode ICP :: " << name_mode << "   cpt_mode ::   "<< cpt_mode << endl;
		Reinit();
		Iteration();
		double res_time = (double)(clock() - debut) / CLOCKS_PER_SEC;
		time->InsertNextTuple4(mode_transformation, res_time, icp_cpt_mode,cpt_mode);
		cout << "-------   Temps consomme : " << res_time << "  nb_ite:: " << icp_cpt_mode << "\n" << endl;;
		
		if (cpt_mode == 0)
			StoreParameters();

		if (mode_transformation == RIGID && cpt_mode == 0){// ICP avec les points source
			cpt_mode = 1;
			mode_transformation = -1;
			SetMode();
			cpt_mode = 0;
		}
		
		if (mode_transformation == SIMILITUDE && cpt_mode == 0){// ICP avec les points source
			cpt_mode = 1;
			mode_transformation = RIGID;
			SetMode();
		}
	}
	else{
		fl_alert(" Registration is finished! ");
	}
}

// reinitialise les tableaux
void ICP::Reinit()
{
	if (Mode_using_Source_points()){
		cout << " reinit() en cours :  ";
		// on vide les listes pour faire de nouvelles correspondances
		weights->Reset();
		list_landmarks_target->Reset();
		list_landmarks_source->Reset();
		
		list_landmarks_dimN_source->Reset();
		list_landmarks_dimN_source->SetNumberOfComponents(dim_points);
		list_landmarks_dimN_target->Reset();
		list_landmarks_dimN_target->SetNumberOfComponents(dim_points);


		if (weight_land_addland != 1){
			if (numberOfPoints_Source < MAX_NUMBER_OF_POINTS){
				// ON PREND TOUS LES POINTS DE LA SOURCE
				numberOfLandmarks_Source = numberOfPoints_Source;
				list_landmarks_source->DeepCopy(Source->GetPoints());
				list_landmarks_source_test->DeepCopy(list_landmarks_source);
				list_landmarks_target->SetNumberOfPoints(numberOfLandmarks_Source);
				list_landmarks_dimN_target->SetNumberOfTuples(numberOfLandmarks_Source);
				list_landmarks_dimN_source->SetNumberOfTuples(numberOfLandmarks_Source);
				dec = 1;/// juste pour etre sure de prendre que les points de la source
				Initialisation_of_points_ofDimN(Source, list_landmarks_dimN_source);

			}
			else{ // on prends toujours les mêmes points 
				Initialise_dec();
				//initialisation des liste landmarks source et INDEX 
				for (vtkIdType i = 0; i < numberOfPoints_Source; i++){
					if (i%dec == 0){
						list_landmarks_source->InsertNextPoint(Source->GetPoint(i));
					}
				}
				list_landmarks_source_test->DeepCopy(list_landmarks_source);
				numberOfLandmarks_Source = list_landmarks_source->GetNumberOfPoints();
				list_landmarks_target->SetNumberOfPoints(numberOfLandmarks_Source);
				list_landmarks_dimN_target->SetNumberOfTuples(numberOfLandmarks_Source);
				list_landmarks_dimN_source->SetNumberOfTuples(numberOfLandmarks_Source);
				Initialisation_of_points_ofDimN(Source, list_landmarks_dimN_source);
			}
		}
		else{ // cas: utilisation des landmarks avec les points. mais dont le poids des points est nul. 
			list_landmarks_source->DeepCopy(list_landmarks_init_source);
			list_landmarks_target->DeepCopy(list_landmarks_init_target);
			numberOfLandmarks_Source = numberOfLandmarks_init;
		}

		
		cout << " fait." << endl;
		cout << "source     " << Source->GetNumberOfPoints() << endl;
	}
	else{// cas : uitlisation des landmarks seulement 
		list_landmarks_source->DeepCopy(list_landmarks_init_source);
		list_landmarks_target->DeepCopy(list_landmarks_init_target);
		numberOfLandmarks_Source = numberOfLandmarks_init;
	}
	
}

//initialise le diviseur dec des points source
void ICP::Initialise_dec(){
	if (dec_user != -1)
		dec = dec_user;
	else
		dec = numberOfPoints_Source / MAX_NUMBER_OF_POINTS;
}

// Dessine les correspondances
void ICP::Draw_matching(){
	
	if (Cont_Mesh.ExistAlwaysMeshLandmarkICP(bool_onlyMatching) && problem == 0){
		vtkSmartPointer<vtkPoints>list_landmarks_source_toto = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkPoints>list_landmarks_target_toto = vtkSmartPointer<vtkPoints>::New();
		bool land=Cont_Mesh.RecoverLandmarks(list_landmarks_source_toto, list_landmarks_target_toto);
		if (!land){
			Source->bool_Source_used_ICP = false;
			Target->bool_Target_used_ICP = false;
		}


		glDisable(GL_LIGHTING);
		glLineWidth(tab_display->GetTuple1(3));

		double* sourcePoint, *targetPoint, *sourcePointTest;
		
		if (tab_display->GetTuple1(0) == 1){// Landmarks
			for (vtkIdType i = 0; i < list_landmarks_target_toto->GetNumberOfPoints(); i++){
				sourcePoint = list_landmarks_source_toto->GetPoint(i);
				targetPoint = list_landmarks_target_toto->GetPoint(i);
				
					glBegin(GL_LINES);
					glColor3f(1.f, 0.f, 0.f);
					glVertex3dv(sourcePoint);
					glVertex3dv(targetPoint);
					glEnd();
				
			}
		}
		if (tab_display->GetTuple1(1) == 1 ){// Vertice
			
			double M1s[16]; double M2s[16];//pour les points
			double M1t[16]; double M2t[16];//pour les points
			double M1n[9];//pour les normales
			double M2n[9];//pour les normales
			double sourceP[4] = { 0, 0, 0, 1 }, targetP[4] = { 0, 0, 0, 1 };

			Source->Convert_GLMatrix_ToDouble(M1s, M2s, M1n, M2n);
			Target->Convert_GLMatrix_ToDouble(M1t, M2t, M1n, M2n);

			int display_dec = abs(tab_display->GetTuple1(2));
			for (vtkIdType i = 0; i < list_landmarks_target->GetNumberOfPoints(); i++){
				
					
				if (i % display_dec == 0){
					list_landmarks_source->GetPoint(i, sourceP);
					::PointMultiply(M1s, sourceP, sourceP);
					::PointMultiply(M2s, sourceP, sourceP);


					list_landmarks_target->GetPoint(i, targetP);
					::PointMultiply(M1t, targetP, targetP);
					::PointMultiply(M2t, targetP, targetP);

					glBegin(GL_LINES); 
					glColor3fv(vect_matching_color);
					glVertex3dv(sourceP);
					glVertex3dv(targetP);
					glEnd();


					glBegin(GL_POINTS);
					glLineWidth(0.1);
					glColor3f(0.f, 0.4f, 0.9f);
					glVertex3d(sourceP[0], sourceP[1], sourceP[2]);
					glColor3f(1.f, 0.7f, 0.5f);
					glVertex3d(targetP[0], targetP[1], targetP[2]);
					glEnd();
					
				}
			}
		}
		glEnable(GL_LIGHTING);
	}
	
}

//dessin en vert les correspondances les plus pres sinon en rouge
void ICP::Draw_looking_like(float val){
	cout << "Compute Distance source et target   ::: " << endl;
	if (mode_transformation == LOC_AFFINE || (bool_onlyMatching)){
		vtkSmartPointer<vtkUnsignedCharArray> colors =
			vtkSmartPointer<vtkUnsignedCharArray>::New();
		colors->SetNumberOfComponents(4);
		colors->SetNumberOfTuples(Source->numvert);

		float green[4] = { 0., 255., 0., Source->blend };
		float red[4] = { 255., 0., 0., Source->blend };

		if ((vtkUnsignedCharArray*)Source->GetPointData()->GetScalars("ColorICP") == NULL){
			for (int i = 0; i < numberOfPoints_Source; i++){
				float dist = sqrt(vtkMath::Distance2BetweenPoints(list_landmarks_source->GetPoint(i), list_landmarks_target->GetPoint(i)));

				if (dist < val){//green
					colors->SetTuple(i, green);
				}
				else{//red
					colors->SetTuple(i, red);
				}
			}
			colors->SetName("ColorICP");
			Source->GetPointData()->AddArray(colors);
			
		}

		if ((vtkUnsignedCharArray*)Source->GetPointData()->GetScalars("ColorICP") != NULL){
			colors->DeepCopy((vtkUnsignedCharArray*)Source->GetPointData()->GetScalars("ColorICP"));
		}
		cout << "done" << endl;
		colors->SetName("RGB");
		Source->GetPointData()->RemoveArray("RGB");
		Source->GetPointData()->AddArray(colors);
		Source->selected = 0;
		Source->bool_changed_rgb_color = 1;
	}
}



////// FONCTIONs DE TEST /////

void testICP(ICP *r1, ICP *r2){
	Node *t1 = r1->tree->GetRoot();
	Node *t2 = r2->tree->GetRoot();
	int cptN = 0, cptL = 0, cptLElem=0,cptNOk=0;
	cout<<" arbres identiques? : "<<testDiffKDTreeDiff(t1, t2,&cptNOk, &cptN, &cptL, &cptLElem)<<endl;
	cout << "cptNok : " << cptNOk << "\ncptN : " << cptN << "\ncptL : " << cptL << "\ncptLElem : " << cptLElem << endl;
	TestMatchingLand(r1, r2);
	TestShapeSource(r1, r2);
	if (r2->mode_transformation == SIMILITUDE && r2->cpt_mode==1)
		TestMatchingSommet(r1, r2);
	
}

//on regarde si les arbres sont identiques
bool testDiffKDTreeDiff(Node *node1, Node* node2,int *cptNOk, int *cptN, int *cptL, int *cptLElem){
	bool b = true;
	double *center1 = node1->GetNode_Center()->GetTuple(0);
	double *center2 = node2->GetNode_Center()->GetTuple(0);
	
	int size = node1->GetNode_Center()->GetNumberOfComponents();
	if (IsEqual(center1, center2,size))
	{
		b &= true;
		*cptNOk+=1;
	}
	else{
		b &= false;
		*cptN+=1;
		if (*cptN <= 6 && *cptN >= 1){
			cout << "erreur " << endl;
			cout << "  c1 " << center1[0] << " " << center1[1] << " " << center1[2] << "\n " << center1[3] << " " << center1[4] << " " << center1[5] << "\n " << center1[6] << " " << center1[7] << " " << endl;
			cout << "  c2 " << center2[0] << " " << center2[1] << " " << center2[2] << "\n " << center2[3] << " " << center2[4] << " " << center2[5] << "\n " << center2[6] << " " << center2[7] << " " << endl;
			cout << "  cptNok : " << *cptNOk << "  cptN : " << *cptN << "\n  cptL : " << *cptL << "  cptLElem : " << *cptLElem << endl;
			cout << "axis   node1: " << node1->GetNode_Splitaxis() << "   node2: " << node1->GetNode_Splitaxis() << endl;


		}
	}

	if (!node1->GetIsLeaf() && !node2->GetIsLeaf())
	{
		b &= testDiffKDTreeDiff(node1->GetNode_LeftChild(), node2->GetNode_LeftChild(),cptNOk, cptN, cptL,cptLElem);
		b &= testDiffKDTreeDiff(node1->GetNode_RightChild(), node2->GetNode_RightChild(),cptNOk, cptN, cptL, cptLElem);	
	}
	else{
		if (node1->GetIsLeaf() && node2->GetIsLeaf()){
			for (int i = 0; i < node1->GetLeaf()->GetNumberOfTuples(); i++){
				b &= (IsEqual(node1->GetLeaf()->GetTuple(i), node2->GetLeaf()->GetTuple(i)));
			}
		}
		else{
			*cptL += 1;
			b &= false;
		}
	}

	*cptLElem += 1;

	return b;
}


void TestMatchingLand(ICP *i1,ICP *i2){
	bool b1 = true, b2 = true;
	for (int i = 0; i < i1->numberOfLandmarks_Source; i++){
		b1 &= IsEqual(i1->list_landmarks_source->GetPoint(i), i2->list_landmarks_source->GetPoint(i));
		b2 &= IsEqual(i1->list_landmarks_target->GetPoint(i), i2->list_landmarks_target->GetPoint(i));
	}
	cout << " Correspondance les memes landmarks ? == \n\tsource ? : " << b1 << "\n\ttarget ? : " << b2 << endl;

	
}

void TestMatchingSommet(ICP *i1, ICP *i2){
	bool b1 = true, b2 = true;
	for (int i = 0; i < i1->numberOfLandmarks_Source; i++){
		b1 &= IsEqual(i1->list_landmarks_source_test->GetPoint(i), i2->list_landmarks_source_test->GetPoint(i));
		b2 &= IsEqual(i1->list_landmarks_target->GetPoint(i), i2->list_landmarks_target->GetPoint(i));
	}
	cout << " Correspondance les memes Sommets ? == \n\tsource ? : " << b1 << "\n\ttarget ? : " << b2 << endl;


}

void TestShapeSource(ICP *i1, ICP *i2){
	bool b1 = true;
	for (int i = 0; i < i1->numberOfPoints_Source; i++){
		b1 &= IsEqual(i1->Source->GetPoint(i), i2->Source->GetPoint(i));
	}
	cout << " les memes Points Source? : " << b1 << endl;


}


//void ICP::Test_radius(){
//	cout << "KD-TREE " << endl;
//	point = vtkSmartPointer<vtkFloatArray>::New();
//	point->SetNumberOfComponents(3);
//	point->SetNumberOfTuples(10 * 10);
//	
//	double ind = 0;
//	for (int i = 0; i < 10; i++)
//		for (int k = 0; k < 10;k++)
//		{
//			double tuple[3] = { i, k, ind };
//			point->SetTuple(ind, tuple);
//			ind++;
//		}
//
//	float R = 3;
//	cout << "hhjkhk"<<point->GetNumberOfTuples()<<" "<<point->GetNumberOfComponents() << endl;
//	KDtree T(point,1);
//	double pointref[2] = { 4, 4};
//	Index = vtkSmartPointer<vtkFloatArray>::New();
//	Index->SetNumberOfComponents(1);
//	Index = T.PointsInRadius(pointref, R);  
//	cout << Index->GetNumberOfTuples();
//	DrawKDtree(&T);
//}


//test les correspondances les points cibles
int TestCorrespondances(vtkPoints *targetList, vtkPoints *targetList1){
	if (targetList->GetNumberOfPoints() == targetList1->GetNumberOfPoints()){
		int cptErreur = 0;
		int nbDePointsTests = 1000; // attention nbDePointsTests doit etre inférieur à targetList->GetNumberOfPoints()
		int i = 0;
		int indice = 0;

		while (i < nbDePointsTests){
			indice = vtkMath::Random(0, targetList->GetNumberOfPoints());// on prend au hasard 1000 points.
			if (!IsEqual(targetList->GetPoint(indice), targetList1->GetPoint(indice))){
				cptErreur++;
			}
			i++;
		}
		cout << "Found errors ::  " << cptErreur << endl;
		return cptErreur;
	}
	else{
		cout << "ERROR:: not same number" << endl;
		return -1;
	}

}

//on regarde si les arbres sont identiques
bool testDiffKDTreeDiff(Node1 *node1, Node1* node2, int *cptNOk, int *cptN, int *cptL, int *cptLElem){
	bool b = true;
	double *center1 = node1->GetNode_Center()->GetTuple(0);
	double *center2 = node2->GetNode_Center()->GetTuple(0);

	int size = node1->GetNode_Center()->GetNumberOfComponents();
	if (IsEqual(center1, center2, size))
	{
		b &= true;
		*cptNOk += 1;
	}
	else{
		b &= false;
		*cptN += 1;

		if (*cptN <= 6 && *cptN >= 1){
			cout << "erreur " << endl;
			cout << "  c1 " << center1[0] << " " << center1[1] << " " << center1[2] << "\n " << center1[3] << " " << center1[4] << " " << center1[5] << "\n " << center1[6] << " " << center1[7] << " " << endl;
			cout << "  c2 " << center2[0] << " " << center2[1] << " " << center2[2] << "\n " << center2[3] << " " << center2[4] << " " << center2[5] << "\n " << center2[6] << " " << center2[7] << " " << endl;
			cout << "  cptNok : " << *cptNOk << "  cptN : " << *cptN << "\n  cptL : " << *cptL << "  cptLElem : " << *cptLElem << endl;
			cout << "axis   node1: " << node1->GetNode_Splitaxis() << "   node2: " << node1->GetNode_Splitaxis() << endl;

		}
	}

	if (!node1->GetIsLeaf() && !node2->GetIsLeaf())
	{
		b &= testDiffKDTreeDiff(node1->GetNode_LeftChild(), node2->GetNode_LeftChild(), cptNOk, cptN, cptL, cptLElem);
		b &= testDiffKDTreeDiff(node1->GetNode_RightChild(), node2->GetNode_RightChild(), cptNOk, cptN, cptL, cptLElem);
	}
	else{
		if (node1->GetIsLeaf() && node2->GetIsLeaf()){
			for (int i = 0; i < node1->GetLeaf()->GetNumberOfTuples(); i++){
				b &= (IsEqual(node1->GetLeaf()->GetTuple(i), node2->GetLeaf()->GetTuple(i)));
			}
		}
		else{
			*cptL += 1;
			b &= false;
		}
	}

	*cptLElem += 1;

	return b;
}


//on regarde si les arbres sont identiques
bool testDiffKDTreeDiff(Node1 *node1, Node* node2, int *cptNOk, int *cptN, int *cptL, int cptLElem){
	bool b = true;

	if (!node1->GetIsLeaf() && !node2->GetIsLeaf())
	{

		double *center1 = node1->GetNode_Center()->GetTuple(0);
		double *center2 = node2->GetNode_Center()->GetTuple(0);

		int size = node1->GetNode_Center()->GetNumberOfComponents();
		if (IsEqual(center1, center2, size))
		{
			b &= true;
			*cptNOk += 1;
		}
		else{
			b &= false;
			*cptN += 1;

			if (*cptN <= 6 && *cptN >= 1){
				cout << "erreur " << endl;

				cout << "  c1 "; affiche(center1, size);
				cout << "  c2 "; affiche(center2, size);
				cout << "  cptNok : " << *cptNOk << "  cptN : " << *cptN << "\n  cptL : " << *cptL << "  cptLElem : " << cptLElem << endl;
				cout << "axis   node1: " << node1->GetNode_Splitaxis() << "   node2: " << node1->GetNode_Splitaxis() << endl;

			}
		}


		b &= testDiffKDTreeDiff(node1->GetNode_LeftChild(), node2->GetNode_LeftChild(), cptNOk, cptN, cptL, cptLElem + 1);
		b &= testDiffKDTreeDiff(node1->GetNode_RightChild(), node2->GetNode_RightChild(), cptNOk, cptN, cptL, cptLElem + 1);
	}
	else{

		if (node1->GetIsLeaf() && node2->GetIsLeaf()){
			bool b1 = true;
			for (int i = 0; i < node1->GetLeaf()->GetNumberOfTuples(); i++){
				b1 &= (IsEqual(node1->GetLeaf()->GetTuple(i), node2->GetLeaf()->GetTuple(i)));
			}
			b &= b1;
			if (!b1){
				*cptL += 1;
			}

		}
		else{
			b &= false;
		}
	}
	return b;
}

//test les correspondances les points cibles
int TestCorrespondances(vtkFloatArray *targetList, vtkFloatArray *targetList1, vtkFloatArray* sourceList, int dimpoints, vtkSmartPointer<vtkFloatArray> tab_weight, double alpha[6]){
	cout << "test  ::: " << endl;
	if (targetList->GetNumberOfTuples() == targetList1->GetNumberOfTuples()){
		int cptErreur = 0;
		int nbDePointsTests = sourceList->GetNumberOfTuples(); // attention nbDePointsTests doit etre inférieur à targetList->GetNumberOfPoints()
		int i = 0;
		int indice = 0;
		int cptErreurinf = 0;
		while (i < nbDePointsTests){
			indice = i;// vtkMath::Random(0, targetList->GetNumberOfPoints());// on prend au hasard 1000 points.

			
			
			if (!IsEqual(targetList->GetTuple(indice), targetList1->GetTuple(indice), dimpoints-1)){
				cptErreur++;
				if (cptErreur <= 10){
					cout << "indice : " << indice << endl;
					affiche(targetList->GetTuple(indice), dimpoints);
					cout << endl;
					affiche(targetList1->GetTuple(indice), dimpoints);
					cout << endl;
					affiche(sourceList->GetTuple(indice), dimpoints);
					cout << endl;
				}
				double *pt, *pt1, *so;
				pt = targetList->GetTuple(indice);
				pt1 = targetList1->GetTuple(indice);
				so = sourceList->GetTuple(indice);

				pt[dimpoints - 1] = 0;
				pt1[dimpoints - 1] = 0;
				so[dimpoints - 1] = 0;

				int t = Dist2pointDimNweight(pt, so, tab_weight, alpha,dimpoints - 1);
				int t1 = Dist2pointDimNweight(pt1, so, tab_weight, alpha, dimpoints - 1);

				if (t < t1)
					cptErreurinf++;

			}
			i++;
		}
		cout << "Found errors ::  " << cptErreur << endl;
		cout << "targetlist < targetlist1  " << cptErreurinf << "  inverse  " << (cptErreur - cptErreurinf) << endl;
		return cptErreur;
	}
	else{
		cout << "ERROR:: not same number" << endl;
		return -1;
	}

}
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