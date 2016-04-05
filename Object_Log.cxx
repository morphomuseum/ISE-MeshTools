#include "Object_Log.h"
#include <vtkAppendPolyData.h>

 
 
void OBJECT_LOG::Object_Move_Down()
{	
// Objects are in the following order :
	// A
	// B * is the object we move down
	// C
	// D
OBJECT_MESH * LastOBJ =NULL;
OBJECT_MESH * NewFirst =NULL;
OBJECT_MESH * A =NULL; // the non changing preceding object
OBJECT_MESH * B =NULL; // the object we move down
OBJECT_MESH * C =NULL; // the old next object (the one we switch with B)
OBJECT_MESH * D =NULL; // the non changing object after C.





	if (this->OBJECTS!=NULL)
	{
		LastOBJ = this->OBJECTS;
		//CurrOBJ = this->OBJECTS;
		while(LastOBJ->nextobj !=NULL)
		{
			LastOBJ = LastOBJ->nextobj;
			//CurrOBJ =CurrOBJ->nextobj
		}

		B = LastOBJ;
		NewFirst=LastOBJ;
		
		// Maintenant on remonte dans la liste
		while(B!= NULL)
		{
			A = NULL;
			A = B->prevobj;
			C = NULL;
			C = B->nextobj;
			D = NULL;
			if (C != NULL && C->nextobj !=NULL)
			{
				D = C->nextobj;					
			}
			if( B->selected ==1)
			{
				

				if (C!=NULL && C->selected ==0)
				{
					if (A !=NULL)
					{
						A->nextobj =C;
					
					
					}
					B->prevobj = C;
					B->nextobj = D;

					C->prevobj = A;
					C->nextobj = B;
					if (D !=NULL)
					{
						D->prevobj = B;
					}
				}

				
			}
			if (A !=NULL) {NewFirst=A;}
			else
			{
				if (B->prevobj !=NULL){NewFirst=B->prevobj;}
			}
			
			B = A; // we proceed backward
			
		}
		
		this->OBJECTS = NewFirst;
		if (this->OBJECTS!=NULL)
		{
			LastOBJ = this->OBJECTS;
			
			//CurrOBJ = this->OBJECTS;
			LastOBJ->selected =0;
			while(LastOBJ->nextobj !=NULL)
			{
				
				LastOBJ = LastOBJ->nextobj;
				LastOBJ->selected =0;
				//CurrOBJ =CurrOBJ->nextobj
			}
		}

		 //this->OBJECTS->Object_Move_Down();
	}
}

void OBJECT_LOG::Update_RGB()
{
	if (this->OBJECTS !=NULL)
	{

		OBJECT_MESH * MyOBJ =this->OBJECTS;
		while(MyOBJ!= NULL)
		{
			MyOBJ->Update_RGB();
			MyOBJ = MyOBJ->nextobj;
		}
		
	}

}

void OBJECT_LOG::Object_Move_Up()
{		
	OBJECT_MESH * LastOBJ =NULL;
	OBJECT_MESH * NewFirst =NULL;
	OBJECT_MESH * A =NULL; // the non changing preceding object
	OBJECT_MESH * B =NULL; // the one (the one we switch with C)
	OBJECT_MESH * C =NULL; // the object we move up 
	OBJECT_MESH * D =NULL; // the non changing object after C.

	if (this->OBJECTS !=NULL)
	{ 
		

		C = this->OBJECTS;
		NewFirst =this->OBJECTS;
		if (C->nextobj != NULL && C->selected ==0 && C->nextobj->selected ==1)
		{NewFirst=C->nextobj;}
		// Maintenant on descend dans la liste
		while(C!= NULL)
		{
			B = NULL;
			B = C->prevobj;
			A = NULL;
			if (B !=NULL)
			{A = B->prevobj;}								
			D = NULL;
			D = C->nextobj;

			if( C->selected ==1)
			{				
				if (B!=NULL && B->selected ==0)
				{
					if (A !=NULL)
					{A->nextobj =C;}
					B->prevobj = C;
					B->nextobj = D;

					C->prevobj = A;
					C->nextobj = B;
					if (D !=NULL)
					{
						D->prevobj = B;
					}
				}				
			}
			C = D; // we proceed onward
			
		}
		this->OBJECTS = NewFirst;
		if (this->OBJECTS!=NULL)
		{
			LastOBJ = this->OBJECTS;
			
			//CurrOBJ = this->OBJECTS;
			LastOBJ->selected =0;
			while(LastOBJ->nextobj !=NULL)
			{
				
				LastOBJ = LastOBJ->nextobj;
				LastOBJ->selected =0;
				
				//CurrOBJ =CurrOBJ->nextobj
			}
		}
	}
}

void OBJECT_LOG::Landmark_Move_Down(int mode, int landmark_mode)
{
	// mode = 0 : user landmark move up (more than one can be selected
	// mode = 0 is followed by a renumbering of landmark indices

	// mode = 1 : called by "Reorder_Landmarks" : 
	// mode = 1 is not followed by a change of landmark indices...
	
	OBJECT_LANDMARK * LastOBJ =NULL;
	OBJECT_LANDMARK * NewFirst =NULL;
	OBJECT_LANDMARK * A =NULL; // the non changing preceding object
	OBJECT_LANDMARK * B =NULL; // the object we move down
	OBJECT_LANDMARK * C =NULL; // the old next object (the one we switch with B)
	OBJECT_LANDMARK * D =NULL; // the non changing object after C.

	if (landmark_mode ==0)
	{			
		LastOBJ =this->OBJECTS_LANDMARK;
	}
	else if (landmark_mode==1)
	{			
		LastOBJ =this->OBJECTS_LANDMARK_TARGET;		
	}
	else
	{			
		LastOBJ =this->OBJECTS_LANDMARK_LABELS;		
	}

	if (LastOBJ !=NULL)
	{
				
		while(LastOBJ->nextobj !=NULL)
		{
			LastOBJ = LastOBJ->nextobj;			
		}

		B = LastOBJ;
		NewFirst=LastOBJ;
		
		// Maintenant on remonte dans la liste
		while(B!= NULL)
		{
			A = NULL;
			A = B->prevobj;
			C = NULL;
			C = B->nextobj;
			D = NULL;
			if (C != NULL && C->nextobj !=NULL)
			{
				D = C->nextobj;					
			}
			if( B->selected ==1)
			{
				

				if (C!=NULL && C->selected ==0)
				{
					if (A !=NULL)
					{
						A->nextobj =C;
				
					
					}
					B->prevobj = C;
					B->nextobj = D;

					C->prevobj = A;
					C->nextobj = B;
					if (D !=NULL)
					{
						D->prevobj = B;
					}
				}

				
			}
			if (A !=NULL) {NewFirst=A;}
			else
			{
				if (B->prevobj !=NULL){NewFirst=B->prevobj;}
			}
			
			B = A; // we proceed backward
			
		}
				
		if (landmark_mode ==0)
		{
			this->OBJECTS_LANDMARK = NewFirst;
		}
		else if (landmark_mode==1)
		{
			this->OBJECTS_LANDMARK_TARGET= NewFirst;
		}	
		else
		{			
				this->OBJECTS_LANDMARK_LABELS= NewFirst;
		}
	
		if (NewFirst!=NULL)
		{					
			
			//CurrOBJ = this->OBJECTS;
			NewFirst->selected =0;
			while(NewFirst->nextobj !=NULL)
			{
				
				NewFirst = NewFirst->nextobj;
				NewFirst->selected =0;
				
				//CurrOBJ =CurrOBJ->nextobj
			}
		}						
		if (mode==0)
		{
			Change_Landmark_Indices(landmark_mode);
		}


	}
}

void OBJECT_LOG::Change_Landmark_Indices(int landmark_mode)
{
	// Si les landmarks sont dans l'ordre 1 2 5 6 7 3 4, les indices deviennent
	// 1 2 3 4 5 6 7 sans que les objets soient réordonnés.
	int num_landmarks = this->Get_Landmark_Number(landmark_mode);

	/* std::cout << "There are " <<num_landmarks
              << " Landmarks" << std::endl;*/

	if (num_landmarks>0)
	{
		int *ordered_indices;
		ordered_indices = new int [num_landmarks];
		OBJECT_LANDMARK * MyLM = this->LandmarkAfter(0,landmark_mode);
		int i;
		for (i=0; i<num_landmarks;i++)
		{
			ordered_indices[i]= MyLM->landmark_index;
			/*std::cout << "i= " <<i
               << std::endl;
			 std::cout << "ordered_indices[i]= " <<ordered_indices[i]
               << std::endl;*/
			MyLM = this->LandmarkAfter(ordered_indices[i], landmark_mode);

		}
		int ok =0;
		if (landmark_mode ==0)
		{
			MyLM = this->OBJECTS_LANDMARK;
		}
		else if (landmark_mode ==1)
		{
			MyLM = this->OBJECTS_LANDMARK_TARGET;
		}
		else
		{
			MyLM = this->OBJECTS_LANDMARK_LABELS;
		}
		int cpt=0;
		while (MyLM!=NULL && cpt<num_landmarks)
		{
			MyLM->landmark_index= ordered_indices[cpt];
			/*std::cout << "cpt=" <<cpt
               << std::endl;
			std::cout << "ordered_indices[cpt]= " <<ordered_indices[cpt]
               << std::endl;*/
			cpt++;
			MyLM = MyLM->nextobj;
			
		}
	
	}

}
void OBJECT_LOG::Unselect_landmarks(int landmark_mode)
{

	OBJECT_LANDMARK *MyLM =NULL;

	if (landmark_mode ==0)
	{			
		MyLM =this->OBJECTS_LANDMARK;
	}
	else if (landmark_mode ==1)
	{			
		MyLM =this->OBJECTS_LANDMARK_TARGET;		
	}
	else
	{
		MyLM =this->OBJECTS_LANDMARK_LABELS;		
	}

	if (MyLM !=NULL)
	{
		while (MyLM !=NULL)
		{
			MyLM->selected=0;
			MyLM = MyLM->nextobj;
			
		}
	}		

}


void OBJECT_LOG::Reorder_Landmarks(int landmark_mode)
{

	// Cela réordonne les objets landmarks selon leur indice (sans changer l'indice à attribuer à chaque landmark).
	// Cela appelle "Landmark_Move_Up (1, X)"
	// Le premier paramètre est à 1 pour éviter 
	// Cette fonction n'est appelée que lorsqu'on a placé un nouveau landmark.
	
	//on déselectionne tout.	
	int ok =0;
	this->Unselect_landmarks(landmark_mode);
	OBJECT_LANDMARK *MyLM =NULL;

	

	while (ok ==0)
	{
		

		ok = Is_Landmark_List_In_Ascending_Order(landmark_mode);
		/*std::cout << "Ok = " <<ok
               << std::endl;*/

		int last_indice=0;
		if (ok ==0)
		{
			if (landmark_mode ==0)
			{			
				MyLM =this->OBJECTS_LANDMARK;
			}
			else if (landmark_mode ==1)
			{			
				MyLM =this->OBJECTS_LANDMARK_TARGET;		
			}
			else
			{
				MyLM =this->OBJECTS_LANDMARK_LABELS;		
			}
			if (MyLM!=NULL)
			{
				while (MyLM !=NULL)
				{
					if (MyLM->landmark_index < last_indice)
					{
						MyLM->selected =1;
						/*std::cout << "MyLM = " <<MyLM->landmark_index<< " move up"
						<< std::endl;*/
						Landmark_Move_Up(1, landmark_mode); // Function moves only selected landmarks.
						MyLM->selected =0;
						MyLM=NULL;
					}
					else
					{
						last_indice=MyLM->landmark_index;
						MyLM=MyLM->nextobj;
					}
					
					
				}
			}
			
		}
	
	}

}

int OBJECT_LOG::Is_Landmark_List_In_Ascending_Order(int landmark_mode)
{
	// Cela dit si la liste des objets "Landmarks" est bien ordonnée en ordre ascendant.
	// Returning 0 means there is at least 1 lm in bad order.
	int ok =1;
	OBJECT_LANDMARK *MyLM =NULL;
	int last_indice =0;
	if (landmark_mode ==0)
	{			
		MyLM =this->OBJECTS_LANDMARK;
	}
	else if (landmark_mode ==1)
	{			
		MyLM =this->OBJECTS_LANDMARK_TARGET;		
	}
	else
	{
		MyLM =this->OBJECTS_LANDMARK_LABELS;		
	}


	if (MyLM !=NULL)
	{
		while (MyLM !=NULL)
		{
			if (MyLM->landmark_index < last_indice)
			{ok=0;}
			last_indice=MyLM->landmark_index;
			MyLM=MyLM->nextobj;
		}
	}	
	/*std::cout << "is_landmark_list_in... = " <<ok
               << std::endl;*/
	return ok;
}

void OBJECT_LOG::Landmark_Move_Up(int mode, int landmark_mode)
{
	// mode = 0 : user landmark move up (more than one can be selected
	// mode = 0 is followed by a renumbering of landmark indices

	// mode = 1 : called by "Reorder_Landmarks" (only one landmark is selected): 
	// mode = 1 is not followed by a change of landmark indices...



	OBJECT_LANDMARK * LastOBJ =NULL;
	OBJECT_LANDMARK * NewFirst =NULL;
	OBJECT_LANDMARK * A =NULL; // the non changing preceding object
	OBJECT_LANDMARK * B =NULL; // the one (the one we switch with C)
	OBJECT_LANDMARK * C =NULL; // the object we move up 
	OBJECT_LANDMARK * D =NULL; // the non changing object after C.

	if (landmark_mode ==0)
	{			
		NewFirst =this->OBJECTS_LANDMARK;
	}
	else if (landmark_mode ==1)
	{			
		NewFirst =this->OBJECTS_LANDMARK_TARGET;		
	}
	else
	{
		NewFirst =this->OBJECTS_LANDMARK_LABELS;		
	}

	if (NewFirst !=NULL)
	{
		//this->OBJECTS_LANDMARKS->Landmark_Move_Up();

		
		C = NewFirst;					
		if (C->nextobj != NULL && C->selected ==0 && C->nextobj->selected ==1)
		{NewFirst=C->nextobj;}
		// Maintenant on descend dans la liste
		while(C!= NULL)
		{
			B = NULL;
			B = C->prevobj;
			A = NULL;
			if (B !=NULL)
			{A = B->prevobj;}								
			D = NULL;
			D = C->nextobj;

			if( C->selected ==1)
			{				
				if (B!=NULL && B->selected ==0)
				{
					if (A !=NULL)
					{A->nextobj =C;}
					B->prevobj = C;
					B->nextobj = D;

					C->prevobj = A;
					C->nextobj = B;
					if (D !=NULL)
					{
						D->prevobj = B;
					}
				}				
			}
			C = D; // we proceed onward
			
		}
		if (landmark_mode ==0)
		{
			this->OBJECTS_LANDMARK = NewFirst;
		}
		else if (landmark_mode ==1)
		{
			this->OBJECTS_LANDMARK_TARGET= NewFirst;
		}
		else
		{	this->OBJECTS_LANDMARK_LABELS= NewFirst;}
		
		if (NewFirst!=NULL)
		{					
			
			//CurrOBJ = this->OBJECTS;
			NewFirst->selected =0;
			while(NewFirst->nextobj !=NULL)
			{
				
				NewFirst = NewFirst->nextobj;
				NewFirst->selected =0;
				
				//CurrOBJ =CurrOBJ->nextobj
			}
		}
			
			
		if (mode==0)
		{
			Change_Landmark_Indices(landmark_mode);
		}
	}
}


void OBJECT_LOG::Mesh_init_Mat()
{
	Mat2[0][0]= 1;
	Mat2[1][1]= 1;
	Mat2[2][2]= 1;
	Mat2[3][3]= 1;
	Mat2[0][1]= 0;
	Mat2[0][2]= 0;
	Mat2[0][3]= 0;
	Mat2[1][0]= 0;
	Mat2[1][2]= 0;
	Mat2[1][3]= 0;
	Mat2[2][0]= 0;
	Mat2[2][1]= 0;
	Mat2[2][3]= 0;
	Mat2[3][0]= 0;
	Mat2[3][1]= 0;
	Mat2[3][2]= 0;



	Mat1[0][0]= 1;
	Mat1[1][1]= 1;
	Mat1[2][2]= 1;


	Mat1[3][3]= 1;
	Mat1[0][1]= 0;
	Mat1[0][2]= 0;
	Mat1[0][3]= 0;
	Mat1[1][0]= 0;
	Mat1[1][2]= 0;
	Mat1[1][3]= 0;
	Mat1[2][0]= 0;
	Mat1[2][1]= 0;
	Mat1[2][3]= 0;
	Mat1[3][0]= 0;
	Mat1[3][1]= 0;
	Mat1[3][2]= 0;
	mean[0] = 0;
	mean[1] = 0;
	mean[2] = 0;

}



void OBJECT_LOG::Mesh_DrawObj(int mode, bool bool_change_pos_obj,bool bool_vbo)
{

// If level ==0 (level 0 = root sera un unique objet impossible à supprimer...
// La camera ne sera dessinée que si on est l'objet racine
	//KEEP!!!
	
 //poupoune = 10000;
	
  //glColorMaterial(GL_FRONT, GL_DIFFUSE);
	//    glEnable(GL_COLOR_MATERIAL);
  VERTEX2 c;
  c.x = c.y = c.z = 0;
  double r = (double)1/3;
  float uvx[3], uvy[3], uvz[3];
  double PI = 3.1415926535897932384626;
  
  uvx[0]=cos(PI*camera.az/180.0)*cos(-PI*camera.el/180.0);
  uvx[1]=sin(PI*camera.az/180.0)*cos(-PI*camera.el/180.0);
  uvx[2]=-sin(-PI*camera.el/180.0);
  //"viewing" 0 1 0
  uvy[0]=cos(PI*camera.az/180.0)*sin(-PI*camera.el/180.0)*sin(-PI*camera.tw/180.0) - sin(PI*camera.az/180.0)*cos(-PI*camera.tw/180.0);
  uvy[1]=sin(PI*camera.az/180.0)*sin(-PI*camera.el/180.0)*sin(-PI*camera.tw/180.0) + cos(PI*camera.az/180.0)*cos(-PI*camera.tw/180.0);
  uvy[2]=cos(-PI*camera.el/180.0)*sin(-PI*camera.tw/180.0);
  //"viewing" 0 0 1
  uvz[0]=cos(PI*camera.az/180.0)*sin(-PI*camera.el/180.0)*cos(-PI*camera.tw/180.0) + sin(PI*camera.az/180.0)*sin(-PI*camera.tw/180.0);
  uvz[1]=sin(PI*camera.az/180.0)*sin(-PI*camera.el/180.0)*cos(-PI*camera.tw/180.0) - cos(PI*camera.az/180.0)*sin(-PI*camera.tw/180.0);
  uvz[2]=cos(-PI*camera.el/180.0)*cos(-PI*camera.tw/180.0);
	
  
  
if (this->level == 1) //Grouped of first level
		{
			if (this->selected ==1 )//Ungrouped object
			{
				//glColor4fv((GLfloat*)bone_ambuse);
				glMaterialfv(GL_FRONT,GL_AMBIENT,bone_ambuse);
				glMaterialfv(GL_FRONT,GL_DIFFUSE,bone_ambuse);
				glMaterialfv(GL_FRONT,GL_SPECULAR,bone_specular);
				glMaterialfv(GL_FRONT,GL_SHININESS,bone_shininess);
				glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);
	
			}
			else
			{
				//glColor4fv((GLfloat*)this->color);
				glMaterialfv(GL_FRONT,GL_AMBIENT,this->color);
				glMaterialfv(GL_FRONT,GL_DIFFUSE,this->color);
				glMaterialfv(GL_FRONT,GL_SPECULAR,this->color);
				glMaterialfv(GL_FRONT,GL_SHININESS,bone_shininess);
				glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);	
			}
		}	
	
 

		glPushMatrix();
			//Move according to widgets only if it's selected AND IF IT'S NOT THE BIG ROOT!
			if (this->selected == 1 && this->level ==1)//Grouped of first rank
			{
				glTranslated(tx2*uvx[0] + ty2*uvy[0]+ tz2*uvz[0],
				tx2*uvx[1] + ty2*uvy[1]+ tz2*uvz[1],
				tx2*uvx[2] + ty2*uvy[2]+ tz2*uvz[2]);
			    // Calculate new center of rotation:
		    
				//float newmean[3];
				/*ApplyTransformation(this->mean, newmean, this->Mat2);//Position
				glTranslated(
					newmean[0],
					newmean[1],
					newmean[2]
					);	
				
				glRotated(rotx2, uvx[0], uvx[1], uvx[2]);
				glRotated(roty2, uvy[0], uvy[1], uvy[2]);
				glRotated(rotz2, uvz[0], uvz[1], uvz[2]);
				
				glTranslated(
					-newmean[0],
					-newmean[1],
					-newmean[2]
					);	*/
				glTranslated(
					g_mean[0],
					g_mean[1],
					g_mean[2]
					);					
				glRotated(rotx2, uvx[0], uvx[1], uvx[2]);
				glRotated(roty2, uvy[0], uvy[1], uvy[2]);
				glRotated(rotz2, uvz[0], uvz[1], uvz[2]);			
				glTranslated(
					-g_mean[0],
					-g_mean[1],
					-g_mean[2]
					);
			
			//}
			}//Fin if selected ==1
		
			//In Any Case of course we multiply by Mat2 and Mat1!
			glMultMatrixf((GLfloat *)this->Mat2);

			glTranslated(
					this->mean[0],
					this->mean[1],
					this->mean[2]
					);	
			glMultMatrixf((GLfloat *)this->Mat1);
			glTranslated(
					-this->mean[0],
					-this->mean[1],
					-this->mean[2]
					);	

			//VERIFY
			
	if (this->OBJECTS_LANDMARK !=NULL)
	{
		//glEnable(GL_COLOR_MATERIAL); 
		this->OBJECTS_LANDMARK->Landmark_DrawObj(this->level, mode);
	}

	if (this->OBJECTS_LANDMARK_TARGET !=NULL)
	{
		//glEnable(GL_COLOR_MATERIAL); 
		//@@@@
		
		this->OBJECTS_LANDMARK_TARGET->Landmark_DrawObj(this->level, mode);
	}

	if (this->OBJECTS !=NULL)
	{
		
		this->OBJECTS->Mesh_DrawObj(this->level, bool_change_pos_obj,bool_vbo);
	}
	if (this->OBJECTS_LOG !=NULL)
	{
		
		this->OBJECTS_LOG->Mesh_DrawObj(mode, bool_change_pos_obj,bool_vbo);
	}

	if (this->OBJECTS_LANDMARK_LABELS != NULL)
	{
		//glEnable(GL_COLOR_MATERIAL); 
		//@@@@

		this->OBJECTS_LANDMARK_LABELS->Landmark_DrawObj(this->level, mode);
	}
	glPopMatrix()
		;
    //glDisable(GL_COLOR_MATERIAL); 
	if (nextobj !=NULL)
	{
		nextobj->Mesh_DrawObj(mode, bool_change_pos_obj,bool_vbo);
	}

}
void OBJECT_LOG::Mesh_Unselect()
{
  this->selected =0;
  if (this->nextobj !=NULL)
    {this->nextobj->Mesh_Unselect();}
  if (this->OBJECTS_LOG !=NULL)
    {this->OBJECTS_LOG->Mesh_Unselect();}
  if (this->OBJECTS_LANDMARK !=NULL)
    {this->OBJECTS_LANDMARK->Landmark_Unselect();}
  if (this->OBJECTS_LANDMARK_TARGET !=NULL)
    {this->OBJECTS_LANDMARK_TARGET->Landmark_Unselect();}
   if (this->OBJECTS_LANDMARK_LABELS !=NULL)
    {this->OBJECTS_LANDMARK_LABELS->Landmark_Unselect();}
  if (this->OBJECTS !=NULL)
    {this->OBJECTS->Mesh_Unselect();}
}

void OBJECT_LOG::Mesh_Select()
{
  this->selected = 1;
  if (this->nextobj !=NULL)
    {this->nextobj->Mesh_Select();}
  if (this->OBJECTS_LOG !=NULL)
    {this->OBJECTS_LOG->Mesh_Select();}
  if (this->OBJECTS_LANDMARK !=NULL)
    {this->OBJECTS_LANDMARK->Landmark_Select();}
  if (this->OBJECTS_LANDMARK_TARGET !=NULL)
    {this->OBJECTS_LANDMARK_TARGET->Landmark_Select();}
  if (this->OBJECTS_LANDMARK_LABELS !=NULL)
    {this->OBJECTS_LANDMARK_LABELS->Landmark_Select();}
  if (this->OBJECTS !=NULL)
    {this->OBJECTS->Mesh_Select();}
}


void OBJECT_LOG::Compute_Global_Scalar_List()
{ 
	
	OBJECT_MESH *MyMesh;
	
	if (this->OBJECTS !=NULL)
	{
		MyMesh = this->OBJECTS;
		MyMesh->Compute_Global_Scalar_List();		
	}
}

void OBJECT_LOG::Compute_Name_Lists()
{ 
	
	OBJECT_MESH *MyMesh;
	
	if (this->OBJECTS !=NULL)
	{
		MyMesh = this->OBJECTS;
		MyMesh->Compute_Name_Lists();		
	}
}
void OBJECT_LOG::Compute_Global_MinMax()
{
	float minx = 9999999999;
	float maxx = -9999999999;
	float miny = 9999999999;
	float maxy = -9999999999;
	float minz = 9999999999;
	float maxz = -9999999999;

	glPushMatrix();
	glLoadIdentity();
	this->Calculate_Global_MinMax(&minx, &maxx, &miny, &maxy, &minz, &maxz ); 

	
	g_minx = minx;
	std::cout << "new g_minx:" << g_minx << std::endl;
	g_maxx = maxx;
	g_miny = miny;
	g_maxy = maxy;
	g_minz = minz;
	g_maxz = maxz;

	glPopMatrix();
}
void OBJECT_LOG::Compute_Global_Mean(int only_selected)
{
	float meanx=0;
	float meany=0;
	float meanz =0;
	float minx = 0;
	float miny = 0;
	float minz = 0;
	float maxx = 0;
	float maxy = 0;
	float maxz = 0;

	int nmean=0;
	glPushMatrix();
	glLoadIdentity();		
	this->Calculate_GlobalMean(&meanx, &meany,&meanz, &nmean, 1); // computes only for selected things
	this->Compute_Global_MinMax();
	//this->Calculate_Global_MinMax(&minx, &maxx, &miny, &maxy, &minz, &maxz);

	if (nmean >0)
	{
		g_mean[0]=meanx/nmean;
		g_mean[1]=meany/nmean;
		g_mean[2]=meanz/nmean;
		g_nmean = nmean;
	}
	else
	{
		g_mean[1]=0;
		g_mean[2]=0;
		g_mean[0]=0;
		g_nmean=0;
	}
	if (only_selected == 0)
	{
		this->Calculate_GlobalMean(&meanx, &meany, &meanz, &nmean, 0); // computes for all objects

		if (nmean > 0)
		{
			g_mean_all[0] = meanx / nmean;
			g_mean_all[1] = meany / nmean;
			g_mean_all[2] = meanz / nmean;
			g_dmean_all = (g_maxx - g_minx + g_maxy - g_miny + g_maxz - g_minz) / 3;
			g_nmean_all = nmean;
		}
		else
		{
			g_mean_all[1] = 0;
			g_mean_all[2] = 0;
			g_mean_all[0] = 0;			
			g_dmean_all = 0;
		}
	}
	glPopMatrix();
	/*std::cout << "g_mean[0]:"<<g_mean[0]<< std::endl;
    std::cout << "g_mean[1]:"<<g_mean[1]<< std::endl;
	std::cout << "g_mean[2]:"<<g_mean[2]<< std::endl;
	std::cout << "g_nmean:"<<g_nmean<< std::endl<<"-----------------------"<< std::endl;*/


}
void OBJECT_LOG::Calculate_GlobalMean(float *x, float *y, float *z,  int *nb, int only_selected)
{
	// selected = 0 : computes for all objects inside this log object
	// selected = 1 : computes only for selected objects.

	// Qu'est ce qu'on fait de Mat1 et Mat2 ????
	// Si c'est l'Objet basal, on s'en fiche
	// Si c'est un autre, on multiplie par Mat1 et Mat2, sinon ça ne fonctionnera plus!

	

	//std::cout<<"Calculate_GlobalMean level: "<<level<<" selected="<<selected<< std::endl;
	float meanx=0,meany=0, meanz=0;
	int nbverts=0;
	float tmpx, tmpy, tmpz;
	int nbtmp =0;
	OBJECT_MESH *MyMesh;
	OBJECT_LANDMARK *MyLandmark;
	OBJECT_LOG *MyLog;
	//Mean of the Object stack
	glMatrix  wc_mat2;
	//this->get_world_coordinates_matrix(wc_mat2);
	//getmatrix(wc_mat2);
	
	

	glPushMatrix();
	if (level==0){glLoadIdentity();}
	else
	{
	  		glMultMatrixf((GLfloat*) this->Mat2);
			if (level==1)
			{
				glTranslated(
					this->mean[0],
					this->mean[1],
					this->mean[2]
					);	
			}
			glMultMatrixf((GLfloat*) this->Mat1); //Aspect
			if (level==1)
			{	
				glTranslated(
						-this->mean[0],
						-this->mean[1],
						-this->mean[2]
				);
			}
	}

	if (this->OBJECTS !=NULL)
	{
		MyMesh = this->OBJECTS;
		if (level==0) { // Objet basal : on veut calculer le centre de masse des objets ... sélectionnés ou pas
			
			MyMesh->Calculate_GlobalMean (&tmpx, &tmpy, &tmpz, &nbtmp, only_selected);
			
			/*if (nbtmp>0)
			{
				std::cout<<"This objects of level 0"<<std::endl<<"tmpx="<<tmpx/nbtmp<<std::endl<<"tmpy="<<tmpy/nbtmp<<std::endl<<"tmpz="<<tmpz/nbtmp<<"nbtmp="<<nbtmp<<std::endl;
			}
			else
			{
				std::cout<<"This objects of level 0 : no vertices "<<std::endl;
			}*/
		} 
		else if ((level ==1 && this->selected==1) || (level>1) ||only_selected==0)
		{
			// Cas où on se situe dans un objet groupé de niveau 1 sélectionné
			// ou un niveau >1 : on calcule tout ce qu'il y a dedans

			MyMesh->Calculate_GlobalMean (&tmpx, &tmpy, &tmpz, &nbtmp, 0);
				
		
			if (nbtmp>0)
			{
				float vv1[3], vv[3];
				vv1[0] =  tmpx/nbtmp;
				vv1[1] =  tmpy/nbtmp;
				vv1[2] =  tmpz/nbtmp;
			
				getmatrix(wc_mat2);
				
				//std::cout<<"This objets of level "<<level<<" before WCmat"<<std::endl<<"tmpx="<<tmpx/nbtmp<<std::endl<<"tmpy="<<tmpy/nbtmp<<std::endl<<"tmpz="<<tmpz/nbtmp<<std::endl;	
				/*std::cout<<"wc_mat|"<<wc_mat2[0][0]<<"|"<<wc_mat2[0][1]<<"|"<<wc_mat2[0][2]<<"|"<<wc_mat2[0][3]<<std::endl;	
				std::cout<<"wc_mat|"<<wc_mat2[1][0]<<"|"<<wc_mat2[1][1]<<"|"<<wc_mat2[1][2]<<"|"<<wc_mat2[1][3]<<std::endl;	
				std::cout<<"wc_mat|"<<wc_mat2[2][0]<<"|"<<wc_mat2[2][1]<<"|"<<wc_mat2[2][2]<<"|"<<wc_mat2[2][3]<<std::endl;	
				std::cout<<"wc_mat|"<<wc_mat2[3][0]<<"|"<<wc_mat2[3][1]<<"|"<<wc_mat2[3][2]<<"|"<<wc_mat2[3][3]<<std::endl;	*/

				ApplyTransformation(vv1,vv,wc_mat2);
				//std::cout<<vv1[0]<<","<<vv1[1]<<","<<vv1[2]<<std::endl;
				//std::cout<<vv[0]<<","<<vv[1]<<","<<vv[2]<<std::endl;
				tmpx = vv[0]*nbtmp;
				tmpy = vv[1]*nbtmp;
				tmpz = vv[2]*nbtmp;			
			}
			

				//std::cout<<"mean[0]"<<this->mean[0]<<std::endl<<"mean[1]"<<this->mean[1]<<std::endl<<"mean[2]"<<this->mean[2]<<std::endl;	
				/*if (nbtmp>0)
				{
					std::cout<<"This objects of level "<<level<<std::endl<<"tmpx="<<tmpx/nbtmp<<std::endl<<"tmpy="<<tmpy/nbtmp<<std::endl<<"tmpz="<<tmpz/nbtmp<<"nbtmp="<<nbtmp<<std::endl;
				}
				else
				{
					std::cout<<"This objects of level "<<level<<": no vertices "<<std::endl;
				}*/
		}	
		else
		{
			//std::cout<<"This objects of level "<<level<<" : no computation "<<nbtmp<<std::endl;
		}
			meanx += tmpx;
			meany += tmpy;
			meanz += tmpz;				
			nbverts+=nbtmp;
		
		
	}
	// Normalement on ne groupe plus 
	if (this->OBJECTS_LANDMARK !=NULL)
	{
		MyLandmark = this->OBJECTS_LANDMARK;
		if (level==0) 
		{ // Objet basal : on veut calculer le centre de masse des objets sélectionnés
			MyLandmark->Calculate_GlobalMean (&tmpx, &tmpy, &tmpz, &nbtmp, 1);
		}
		else
		{
			// Cas où on se situe dans un objet groupé : on calcule tout ce qu'il y a dedans
			MyLandmark->Calculate_GlobalMean (&tmpx, &tmpy, &tmpz, &nbtmp, 0);
		}
		meanx += tmpx;
		meany += tmpy;
		meanz += tmpz;	
		nbverts+=nbtmp;
		
		
	}
	if (this->OBJECTS_LANDMARK_TARGET !=NULL)
	{
		MyLandmark = this->OBJECTS_LANDMARK_TARGET;
		if (level==0) 
		{ // Objet basal : on veut calculer le centre de masse des objets sélectionnés
			MyLandmark->Calculate_GlobalMean (&tmpx, &tmpy, &tmpz, &nbtmp, 1);
		}
		else if ((level ==1 && this->selected==1) || (level>1))
		{
			// Cas où on se situe dans un objet groupé : on calcule tout ce qu'il y a dedans
			MyLandmark->Calculate_GlobalMean (&tmpx, &tmpy, &tmpz, &nbtmp, 0);
		}
		meanx += tmpx;
		meany += tmpy;
		meanz += tmpz;	
		nbverts+=nbtmp;
		
		
	}

	if (this->OBJECTS_LANDMARK_LABELS !=NULL)
	{
		MyLandmark = this->OBJECTS_LANDMARK_LABELS;
		if (level==0) 
		{ // Objet basal : on veut calculer le centre de masse des objets sélectionnés
			MyLandmark->Calculate_GlobalMean (&tmpx, &tmpy, &tmpz, &nbtmp, 1);
		}		
		if ((level ==1 && this->selected==1) || (level>1))
		{
			// Cas où on se situe dans un objet groupé d'un niveau >1 : on calcule tout ce qu'il y a dedans
			// puisque cette fonction n'a été appelée que lorsque l'objet de niveau 1 était sélectionné!
			MyLandmark->Calculate_GlobalMean (&tmpx, &tmpy, &tmpz, &nbtmp, 0);
		}
		meanx += tmpx;
		meany += tmpy;
		meanz += tmpz;	
		nbverts+=nbtmp;
		
		
	}
	//Mean of the logical object stack
	if (this->OBJECTS_LOG !=NULL)
	{
		MyLog = this->OBJECTS_LOG;
		if (level==0) 
		{ // Objet basal : on veut calculer le centre de masse des objets sélectionnés
			MyLog->Calculate_GlobalMean (&tmpx, &tmpy, &tmpz, &nbtmp, 1);
			/*if (nbtmp>0)
			{
				std::cout<<"This Logs of level 0,"<<"tmpx="<<tmpx/nbtmp<<"tmpy="<<tmpy/nbtmp<<"tmpz="<<tmpz/nbtmp<<"nbtmp="<<nbtmp<<std::endl;
			}
			else
			{
				std::cout<<"This Logs of level 0 : no vertices "<<std::endl;
			}*/
		}
		else if ((level ==1 && this->selected==1) || (level>1) || only_selected==0)
		{
			// Cas où on se situe dans un objet groupé : on calcule tout ce qu'il y a dedans
			//Là, surtout pas de GLLoadIdentity!
			MyLog->Calculate_GlobalMean (&tmpx, &tmpy, &tmpz, &nbtmp, 0);

			/*if (nbtmp>0)
			{
				std::cout<<"This Logs of level "<<level<<","<<"tmpx="<<tmpx/nbtmp<<"tmpy="<<tmpy/nbtmp<<"tmpz="<<tmpz/nbtmp<<"nbtmp="<<nbtmp<<std::endl;
			}
			else
			{
				std::cout<<"This Logs of level "<<level<<" : no vertices "<<std::endl;
			}*/
		}
		
		meanx += tmpx;
		meany += tmpy;
		meanz += tmpz;
		nbverts+=nbtmp;
					

	}
	glPopMatrix();	

	if (this->nextobj !=NULL)
	{this->nextobj->Calculate_GlobalMean(&tmpx, &tmpy, &tmpz, &nbtmp, only_selected);
	 meanx += tmpx;
	 meany += tmpy;
	 meanz += tmpz;
	 nbverts+=nbtmp;
	}
	*x = meanx;
	*y = meany;
	*z = meanz;
	*nb = nbverts;
	

}


void OBJECT_LOG::Calculate_Global_MinMax(float *minx, float *maxx, float *miny, float *maxy, float *minz, float *maxz)
{

	


	//std::cout<<"Calculate_GlobalMean min max: "<<level<<" selected="<<selected<< std::endl;
	float inx = 999999999, axx =-999999999, iny = 999999999, axy = -999999999, inz = 999999999, axz = -999999999;	
	float tmpminx = 999999999, tmpmaxx = -999999999, tmpminy = 999999999, tmpmaxy = -999999999, tmpminz = 999999999, tmpmaxz = -999999999;
	int nbtmp = 0;
	OBJECT_MESH *MyMesh;
	
	//Mean of the Object stack
	glMatrix  wc_mat2;
	//this->get_world_coordinates_matrix(wc_mat2);
	//getmatrix(wc_mat2);



	glPushMatrix();
	if (level == 0) { glLoadIdentity(); }
	else
	{
		glMultMatrixf((GLfloat*) this->Mat2);
		if (level == 1)
		{
			glTranslated(
				this->mean[0],
				this->mean[1],
				this->mean[2]
				);
		}
		glMultMatrixf((GLfloat*) this->Mat1); //Aspect
		if (level == 1)
		{
			glTranslated(
				-this->mean[0],
				-this->mean[1],
				-this->mean[2]
				);
		}
	}

	if (this->OBJECTS != NULL)
	{
		MyMesh = this->OBJECTS;
		
			MyMesh->Calculate_Global_MinMax(&tmpminx, &tmpmaxx, &tmpminy, &tmpmaxy, &tmpminz, &tmpmaxz);
			//std::cout << "tmpminx=" << tmpminx << std::endl;
			
		
		


	}
	
	glPopMatrix();	
	if (tmpminx < inx) { inx = tmpminx; 
	//std::cout << "new inx in obj log:" << inx << std::endl;
	}
	if (tmpminy < iny) { iny = tmpminy; }
	if (tmpminz < inz) { inz = tmpminz; }

	if (tmpmaxx > axx) { axx = tmpmaxx; }
	if (tmpmaxy > axy) { axy = tmpmaxy; }
	if (tmpmaxz > axz) { axz = tmpmaxz; }

	*minx = inx;
	*maxx = axx;
	*miny = iny;
	*maxy = axy;
	*minz = inz;
	*maxz = axz;
	std::cout << "inx *minx calc obj log:" << *minx << std::endl;
}

void OBJECT_LOG::rollinit_objects()
{

	
	//glMatrix NewMat;
	OBJECT_MESH * My_Obj;
	OBJECT_LANDMARK * My_Landmark;
	OBJECT_LOG * My_Log;
	float uvx[3], uvy[3], uvz[3];
	double PI = 3.1415926535897932384626;
	// "viewing" 1 0 0
	uvx[0]=cos(PI*camera.az/180.0)*cos(-PI*camera.el/180.0);
	uvx[1]=sin(PI*camera.az/180.0)*cos(-PI*camera.el/180.0);
	uvx[2]=-sin(-PI*camera.el/180.0);
	//"viewing" 0 1 0
	uvy[0]=cos(PI*camera.az/180.0)*sin(-PI*camera.el/180.0)*sin(-PI*camera.tw/180.0) - sin(PI*camera.az/180.0)*cos(-PI*camera.tw/180.0);
	uvy[1]=sin(PI*camera.az/180.0)*sin(-PI*camera.el/180.0)*sin(-PI*camera.tw/180.0) + cos(PI*camera.az/180.0)*cos(-PI*camera.tw/180.0);
	uvy[2]=cos(-PI*camera.el/180.0)*sin(-PI*camera.tw/180.0);
	//"viewing" 0 0 1
	uvz[0]=cos(PI*camera.az/180.0)*sin(-PI*camera.el/180.0)*cos(-PI*camera.tw/180.0) + sin(PI*camera.az/180.0)*sin(-PI*camera.tw/180.0);
	uvz[1]=sin(PI*camera.az/180.0)*sin(-PI*camera.el/180.0)*cos(-PI*camera.tw/180.0) - cos(PI*camera.az/180.0)*sin(-PI*camera.tw/180.0);
	uvz[2]=cos(-PI*camera.el/180.0)*cos(-PI*camera.tw/180.0);
	int Ok = 1;
	//Implement Groups
	if (level ==0)//Root, donc cas habituel
	{
		if ( this->OBJECTS != NULL)
		{
			My_Obj = this->OBJECTS;		
		}
		else
		{	
			Ok = 0;
		}
		while (Ok !=0)
		{
		
			if (My_Obj->selected == 1)
			{	
				My_Obj->bool_changed_matrix = 1;
				glPushMatrix();
				glLoadIdentity();			
				glTranslated(tx2*uvx[0] + ty2*uvy[0]+ tz2*uvz[0],
				tx2*uvx[1] + ty2*uvy[1]+ tz2*uvz[1],
				tx2*uvx[2] + ty2*uvy[2]+ tz2*uvz[2]);
					
				//float newmean1[3],newmean[3];
				// Initialement, avant existence d'un barycentre de tous les objets sélectionnés.
				 /*ApplyTransformation(My_Obj->mean, newmean, My_Obj->Mat2); //Position
					glTranslated(
					newmean[0],
					newmean[1],
					newmean[2]
					);					
				glRotated(rotx2, uvx[0], uvx[1], uvx[2]);
				glRotated(roty2, uvy[0], uvy[1], uvy[2]);
				glRotated(rotz2, uvz[0], uvz[1], uvz[2]);			
				glTranslated(
					-newmean[0],
					-newmean[1],
					-newmean[2]
					);	*/

				// Maintenant 
				//Rotation direct autour du centre de masse de l'échantillon entier!
					//Ne fonctionne pas ApplyTransformation(g_mean, newmean, My_Obj->Mat2); //Position
					
				glTranslated(
					g_mean[0],
					g_mean[1],
					g_mean[2]
					);					
				glRotated(rotx2, uvx[0], uvx[1], uvx[2]);
				glRotated(roty2, uvy[0], uvy[1], uvy[2]);
				glRotated(rotz2, uvz[0], uvz[1], uvz[2]);			
				glTranslated(
					-g_mean[0],
					-g_mean[1],
					-g_mean[2]
					);

					
										

				glMultMatrixf((GLfloat *)My_Obj->Mat2); // Position
				getmatrix(My_Obj->Mat2);
				glPopMatrix();						
			}//Fin if
			if (My_Obj->nextobj!=NULL)
				{My_Obj = My_Obj->nextobj;}
			else
				{Ok = 0;}
		}//Fin while Normal!

		//Now the landmarks
		Ok=1;
		if ( this->OBJECTS_LANDMARK != NULL)
		{
			My_Landmark = this->OBJECTS_LANDMARK;		
		}
		else
		{	
			Ok = 0;
		}
		while (Ok !=0)
		{
		
			if (My_Landmark->selected == 1)
			{	
				glPushMatrix();
				glLoadIdentity();			
				glTranslated(tx2*uvx[0] + ty2*uvy[0]+ tz2*uvz[0],
				tx2*uvx[1] + ty2*uvy[1]+ tz2*uvz[1],
				tx2*uvx[2] + ty2*uvy[2]+ tz2*uvz[2]);
					
				//float newmean[3];
				/*ApplyTransformation(My_Landmark->mean, newmean, My_Landmark->Mat2);
				glTranslated(
					newmean[0],
					newmean[1],
					newmean[2]
					);					
				glRotated(rotx2, uvx[0], uvx[1], uvx[2]);
				glRotated(roty2, uvy[0], uvy[1], uvy[2]);
				glRotated(rotz2, uvz[0], uvz[1], uvz[2]);			
				glTranslated(
					-newmean[0],
					-newmean[1],
					-newmean[2]
					);*/
				glTranslated(
					g_mean[0],
					g_mean[1],
					g_mean[2]
					);					
				glRotated(rotx2, uvx[0], uvx[1], uvx[2]);
				glRotated(roty2, uvy[0], uvy[1], uvy[2]);
				glRotated(rotz2, uvz[0], uvz[1], uvz[2]);			
				glTranslated(
					-g_mean[0],
					-g_mean[1],
					-g_mean[2]
					);

				glMultMatrixf((GLfloat *)My_Landmark->Mat2);
				getmatrix(My_Landmark->Mat2);
				glPopMatrix();						
			}//Fin if
			if (My_Landmark->nextobj!=NULL)
				{My_Landmark = My_Landmark->nextobj;}
			else
				{Ok = 0;}
		}//Fin while Landmark Normal!

		Ok=1;
		if ( this->OBJECTS_LANDMARK_TARGET != NULL)
		{
			My_Landmark = this->OBJECTS_LANDMARK_TARGET;		
		}
		else
		{	
			Ok = 0;
		}
		while (Ok !=0)
		{
		
			if (My_Landmark->selected == 1)
			{	
				glPushMatrix();
				glLoadIdentity();			
				glTranslated(tx2*uvx[0] + ty2*uvy[0]+ tz2*uvz[0],
				tx2*uvx[1] + ty2*uvy[1]+ tz2*uvz[1],
				tx2*uvx[2] + ty2*uvy[2]+ tz2*uvz[2]);
					
				//float newmean[3];
				/*ApplyTransformation(My_Landmark->mean, newmean, My_Landmark->Mat2);
				glTranslated(
					newmean[0],
					newmean[1],
					newmean[2]
					);					
				glRotated(rotx2, uvx[0], uvx[1], uvx[2]);
				glRotated(roty2, uvy[0], uvy[1], uvy[2]);
				glRotated(rotz2, uvz[0], uvz[1], uvz[2]);			
				glTranslated(
					-newmean[0],
					-newmean[1],
					-newmean[2]
					);	*/
				glTranslated(
					g_mean[0],
					g_mean[1],
					g_mean[2]
					);					
				glRotated(rotx2, uvx[0], uvx[1], uvx[2]);
				glRotated(roty2, uvy[0], uvy[1], uvy[2]);
				glRotated(rotz2, uvz[0], uvz[1], uvz[2]);			
				glTranslated(
					-g_mean[0],
					-g_mean[1],
					-g_mean[2]
					);


				glMultMatrixf((GLfloat *)My_Landmark->Mat2);
				getmatrix(My_Landmark->Mat2);
				glPopMatrix();						
			}//Fin if
			if (My_Landmark->nextobj!=NULL)
				{My_Landmark = My_Landmark->nextobj;}
			else
				{Ok = 0;}
		}//Fin while Landmark target!

		Ok=1;
		if ( this->OBJECTS_LANDMARK_LABELS != NULL)
		{
			My_Landmark = this->OBJECTS_LANDMARK_LABELS;		
		}
		else
		{	
			Ok = 0;
		}
		while (Ok !=0)
		{
		
			if (My_Landmark->selected == 1)
			{	
				glPushMatrix();
				glLoadIdentity();			
				glTranslated(tx2*uvx[0] + ty2*uvy[0]+ tz2*uvz[0],
				tx2*uvx[1] + ty2*uvy[1]+ tz2*uvz[1],
				tx2*uvx[2] + ty2*uvy[2]+ tz2*uvz[2]);
					
				
				glTranslated(
					g_mean[0],
					g_mean[1],
					g_mean[2]
					);					
				glRotated(rotx2, uvx[0], uvx[1], uvx[2]);
				glRotated(roty2, uvy[0], uvy[1], uvy[2]);
				glRotated(rotz2, uvz[0], uvz[1], uvz[2]);			
				glTranslated(
					-g_mean[0],
					-g_mean[1],
					-g_mean[2]
					);


				glMultMatrixf((GLfloat *)My_Landmark->Mat2);
				getmatrix(My_Landmark->Mat2);
				glPopMatrix();						
			}//Fin if
			if (My_Landmark->nextobj!=NULL)
				{My_Landmark = My_Landmark->nextobj;}
			else
				{Ok = 0;}
		}//Fin while Landmark flags!

		//Maintenant, les objets LOG du premier niveau!
		if (this->OBJECTS_LOG !=NULL)
		{
			Ok = 1;
			My_Log = this->OBJECTS_LOG;		
		}
		else
		{	
			Ok = 0;
		}
		while (Ok !=0)
		{
		
			if (My_Log->selected == 1)
			{	
				glPushMatrix();
				glLoadIdentity();			
				glTranslated(tx2*uvx[0] + ty2*uvy[0]+ tz2*uvz[0],
				tx2*uvx[1] + ty2*uvy[1]+ tz2*uvz[1],
				tx2*uvx[2] + ty2*uvy[2]+ tz2*uvz[2]);
					
				//float newmean[3];
				/*ApplyTransformation(My_Log->mean, newmean, My_Log->Mat2); //Position
				glTranslated(
					newmean[0],
					newmean[1],
					newmean[2]
					);					
				glRotated(rotx2, uvx[0], uvx[1], uvx[2]);
				glRotated(roty2, uvy[0], uvy[1], uvy[2]);
				glRotated(rotz2, uvz[0], uvz[1], uvz[2]);			
				glTranslated(
					-newmean[0],
					-newmean[1],
					-newmean[2]
					);		
					*/
				glTranslated(
					g_mean[0],
					g_mean[1],
					g_mean[2]
					);					
				glRotated(rotx2, uvx[0], uvx[1], uvx[2]);
				glRotated(roty2, uvy[0], uvy[1], uvy[2]);
				glRotated(rotz2, uvz[0], uvz[1], uvz[2]);			
				glTranslated(
					-g_mean[0],
					-g_mean[1],
					-g_mean[2]
					);
				glMultMatrixf((GLfloat *)My_Log->Mat2);
				getmatrix(My_Log->Mat2);
				glPopMatrix();						
			}//Fin if
			if (My_Log->nextobj!=NULL)
				{My_Log = My_Log->nextobj;}
			else
				{Ok = 0;}
		}//Fin while LOG
	}//Fin du cas habituel
	else // Si on est dans un objet log qui n'est pas la racine...
	{ //RIEN DU TOUT!!!!!
	}
	//Now center of Mass of selected objects has changed (rotation + translations applied)
	this->Compute_Global_Mean();

}



void OBJECT_LOG::get_screen_projection_matrix( glMatrix screenproj)
{
	// If level ==0 (root sera un unique objet impossible à supprimer...
	//CHANGE ????
	glMatrix cam_mat;
	get_projection_matrix(screenproj);
	get_camera_transform_matrix(cam_mat);
	glPushMatrix();
	glLoadIdentity();
		//glLoadMatrixf((GLfloat*) screenproj);
	glMultMatrixf((GLfloat*) screenproj);
		glMultMatrixf((GLfloat*) cam_mat);
		glMultMatrixf((GLfloat*) Mat2);
		glMultMatrixf((GLfloat*) Mat1);
		getmatrix(screenproj);
	glPopMatrix ();
}
void OBJECT_LOG::DeleteObjs()
{
	//Method implemented to delete every thing in this 
	
	OBJECT_MESH * Parent;
	OBJECT_MESH * Next;
	
		
	    int Ok = 1;
		int Root_found = 0;
		Parent = Next = NULL;
		
		if (this->OBJECTS == NULL)
		{Ok = 0;}
		else
		{
			Parent = this->OBJECTS;
			
		}				
		//First : find new root object;
		while (Ok!=0)
		{	
			
			if (Parent->nextobj !=NULL)
			{
				Next = Parent->nextobj;
				delete Parent;
				Parent = Next;								
			}
			else 
			{				
				delete Parent;
				Parent = NULL;
				Next = NULL;
				this->OBJECTS = NULL;
				Ok =0;
			}			 		
					
		}
		
		OBJECT_LOG * ParentLog;
		OBJECT_LOG * NextLog;
		
		Ok = 1;
		Root_found = 0;
		ParentLog = NextLog = NULL;
		
		if (this->OBJECTS_LOG == NULL)
		{Ok = 0;}
		else
		{
			ParentLog = this->OBJECTS_LOG;			
		}				
		//First : find new root object;
		while (Ok!=0)
		{	if (ParentLog->nextobj !=NULL)
			{
				NextLog = ParentLog->nextobj;
				//ParentLog->DeleteObjs();
				delete ParentLog;				
				ParentLog = NextLog;	
				ParentLog->prevobj = NULL;
			}
			else 
			{				
				//ParentLog->DeleteObjs();
				delete ParentLog;
				ParentLog = NULL;
				NextLog = NULL;
				this->OBJECTS_LOG = NULL;
				Ok =0;
			}			 		
		
		}
		
		
}
void OBJECT_LOG::get_world_coordinates_matrix(glMatrix mat)
{
	//Implement
	// If level ==0 ( root sera un unique objet impossible à supprimer...

	if (this->level==0)
	{
		//glMatrix M2;

	
		glPushMatrix();
		glLoadIdentity();

		
	
		glMultMatrixf((GLfloat*)this->Mat2);

		glMultMatrixf((GLfloat*)this->Mat1);

		getmatrix(mat);
		/*std::cout<<"mat|"<<mat[0][0]<<"|"<<mat[0][1]<<"|"<<mat[0][2]<<"|"<<mat[0][3]<<std::endl;	
	std::cout<<"mat|"<<mat[1][0]<<"|"<<mat[1][1]<<"|"<<mat[1][2]<<"|"<<mat[1][3]<<std::endl;	
	std::cout<<"mat|"<<mat[2][0]<<"|"<<mat[2][1]<<"|"<<mat[2][2]<<"|"<<mat[2][3]<<std::endl;	
	std::cout<<"mat|"<<mat[3][0]<<"|"<<mat[3][1]<<"|"<<mat[3][2]<<"|"<<mat[3][3]<<std::endl;	*/
		glPopMatrix();
	}
	else
	{

		//glMatrix M2;

		//get_camera_transform_matrix (M2);
		glPushMatrix();
		glLoadIdentity();
		glMultMatrixf((GLfloat*)Mat2);
		glTranslated(
						this->mean[0],
						this->mean[1],
						this->mean[2]
						);	
		glMultMatrixf((GLfloat *)this->Mat1);
			glTranslated(
						-this->mean[0],
						-this->mean[1],
						-this->mean[2]
						);	
		//glMultMatrixf((GLfloat*)Mat1);
		getmatrix(mat);
		glPopMatrix();
	}
}



void OBJECT_LOG::MergeObjects(vtkSmartPointer<vtkAppendPolyData> mergedObjects, int mode)
{
// mode = 0 : keep original position
// mode = 1 : update original position
	double v[3];
	float ve[3];
	float vepos[3];           
	glMatrix themat;
	OBJECT_MESH * My_Obj;
	if (this->selected ==1 ||this->level >1)
	  {
		// if level > 1 and we are here : this means that the corresponding level 1 object was selected => We save it!
		  
		  glPushMatrix();
		  glMultMatrixf((GLfloat*) this->Mat2);
		  glMultMatrixf((GLfloat*) this->Mat1);

		  //getmatrix(themat);
		   
			
		  if (this->OBJECTS !=NULL)
		  {
			int Ok =1;
			My_Obj = this->OBJECTS;
		    while (Ok!=0)
			  {			
				
					if (mode ==0)
					{
						mergedObjects->AddInputData ((vtkPolyData*)My_Obj);
					}
					else
					{
						glPushMatrix();							
						glMultMatrixf((GLfloat*) My_Obj->Mat2);
						glMultMatrixf((GLfloat*) My_Obj->Mat1);				
						getmatrix(themat);
						vtkSmartPointer<vtkPolyData> toSave = vtkSmartPointer<vtkPolyData>::New();
						toSave->DeepCopy((vtkPolyData*)My_Obj); 
						for (int i=0;i<toSave->GetNumberOfPoints();i++) {
							// for every triangle 
							toSave->GetPoint((vtkIdType)i, v);
							ve[0]=(float)v[0];
							ve[1]=(float)v[1];
							ve[2]=(float)v[2];
							
							ApplyTransformation(ve, vepos, themat);	
							toSave->GetPoints()->SetPoint((vtkIdType) i, vepos);
						}
						std::cout << "\n Add object log =>"<<My_Obj->name;
						mergedObjects->AddInputData (toSave);
						mergedObjects->Update();
						glPopMatrix();
					}
				
				
				if (My_Obj->nextobj != NULL)
				{
					My_Obj = My_Obj->nextobj;
					std::cout << "\n Object log => Call Next:"<<My_Obj->name;
				}
				else
				{Ok=0;}
			  }
			
			
		  }

		  if (this->OBJECTS_LOG !=NULL)
		  {
			this->OBJECTS_LOG->MergeObjects(mergedObjects, mode);			
		  }		  

		  glPopMatrix();
	}//fin if selected
    if (this->nextobj !=NULL)
	{
		this->nextobj->MergeObjects(mergedObjects, mode);
	}	 



}


void OBJECT_LOG::Mesh_Increase_Level()
{	
	this->level++;
	if (this->nextobj !=NULL)
	{this->nextobj->Mesh_Increase_Level();}
	if (this->OBJECTS_LOG !=NULL)
	{this->OBJECTS_LOG->Mesh_Increase_Level();}
}
void OBJECT_LOG::Mesh_Decrease_Level()
{
	this->level--;
	if (this->nextobj !=NULL)
	{this->nextobj->Mesh_Decrease_Level();}
	if (this->OBJECTS_LOG !=NULL)
	{this->OBJECTS_LOG->Mesh_Decrease_Level();}
}
void OBJECT_LOG::Mesh_Ungroup()
{
	
		int cpt=0;
	int Ok = 0, Ok2 =0;
	int root_exists;
	//OBJECT_MESH * My_Mesh_Level1; 
	OBJECT_MESH *MyMesh_Level2;
	OBJECT_MESH *MyMesh_Level2_next;
	
	OBJECT_LOG *MyLog_Level2;
	OBJECT_LOG *MyLog_Level2_next;
	OBJECT_LOG  *MyLog_Level1;
	OBJECT_LOG  *MyLog_Level1_Root;
	OBJECT_LOG  *MyLog_Level1_Last;
	OBJECT_LOG  *MyLog_Level1_next;
	root_exists = 0;
	// Il faut savoir virer le group_ level1 proprement.
	if (level ==0) // Dans un root;
	{   // Something is only done at the root level.
		if (this->OBJECTS_LOG !=NULL) // If level 1 groups exist !
		{
			Ok = 1;
			MyLog_Level1 = this->OBJECTS_LOG;
			MyLog_Level1_Root = this->OBJECTS_LOG; // the first level1 group!
		}		
		while (Ok)
		{
			//Here this begins!!!
			MyLog_Level1_next = MyLog_Level1->nextobj;
			if (MyLog_Level1->selected ==1)
			{
				// we remove this group with its  objets!
				// Put its mesh into root!
				if(MyLog_Level1->OBJECTS != NULL)
				{MyMesh_Level2 = MyLog_Level1->OBJECTS;Ok2 = 1;}
				while (Ok2)
				{
					MyMesh_Level2_next = MyMesh_Level2->nextobj;
					// Create new matrix M1 for MyMesh_Level2
					glPushMatrix();
					glLoadIdentity();

/*glMultMatrixf((GLfloat*)Mat2);
		glTranslated(
						this->mean[0],
						this->mean[1],
						this->mean[2]
						);	
		glMultMatrixf((GLfloat *)this->Mat1);
			glTranslated(
						-this->mean[0],
						-this->mean[1],
						-this->mean[2]
						);	*/

					glMultMatrixf((GLfloat*)MyLog_Level1->Mat2);
					glTranslated(
						MyLog_Level1->mean[0],
						MyLog_Level1->mean[1],
						MyLog_Level1->mean[2]
						);	
				
					glMultMatrixf((GLfloat*)MyLog_Level1->Mat1);
					glTranslated(
						-MyLog_Level1->mean[0],
						-MyLog_Level1->mean[1],
						-MyLog_Level1->mean[2]
						);	
					glMultMatrixf((GLfloat*)MyMesh_Level2->Mat2);
					getmatrix(MyMesh_Level2->Mat2);
					glPopMatrix();
					MyMesh_Level2->selected = 0;
					

					//Put this in root->OBJECTS
					if (this->OBJECTS !=NULL)
					{   MyMesh_Level2->nextobj =NULL;
						this->OBJECTS->Add_Object(MyMesh_Level2);}
					else
					{this->OBJECTS = MyMesh_Level2;}

					//Check for next object
					if (MyMesh_Level2_next !=NULL)
					{
						MyMesh_Level2 =MyMesh_Level2_next ;
					}
					else{Ok2 = 0;}
				}

				MyLog_Level1->OBJECTS =NULL;
				
				

				//Now the logical objects!
				// Put its groups into root!
				Ok2=0;
				if(MyLog_Level1->OBJECTS_LOG != NULL)
				{MyLog_Level2 = MyLog_Level1->OBJECTS_LOG;Ok2 = 1;}
				while (Ok2)
				{
					MyLog_Level2_next = MyLog_Level2->nextobj;
					// Create new matrix M1 for MyLog_Level2
					glPushMatrix();
					glLoadIdentity();
					glMultMatrixf((GLfloat*)MyLog_Level1->Mat2);
					glTranslated(
						MyLog_Level1->mean[0],
						MyLog_Level1->mean[1],
						MyLog_Level1->mean[2]
						);	
					glMultMatrixf((GLfloat*)MyLog_Level1->Mat1);
					glTranslated(
						-MyLog_Level1->mean[0],
						-MyLog_Level1->mean[1],
						-MyLog_Level1->mean[2]
						);	
					glMultMatrixf((GLfloat*)MyLog_Level2->Mat2);
					getmatrix(MyLog_Level2->Mat2);
					glPopMatrix();
					MyLog_Level2->selected = 0;
					
					//Put this in root->OBJECTS
					if (this->OBJECTS_LOG !=NULL)
					{ MyLog_Level2->nextobj =NULL;
						this->OBJECTS_LOG->Add_ObjectLog(MyLog_Level2);}
					else
					{this->OBJECTS_LOG = MyLog_Level2;}
					if (root_exists ==0)
					{	root_exists =1;
						MyLog_Level1_Root = MyLog_Level2;
						MyLog_Level1_Last = MyLog_Level2;
						MyLog_Level1_Last->nextobj = NULL;
					}
					else 
					{
						MyLog_Level1_Last->nextobj = MyLog_Level2;
						MyLog_Level1_Last = MyLog_Level2;
						MyLog_Level1_Last->nextobj = NULL;
					}
					MyLog_Level2->Mesh_Decrease_Level();
					//Check for next object
					if (MyLog_Level2_next !=NULL)
					{
						MyLog_Level2 =MyLog_Level2_next ;
					}
					else{Ok2 = 0;}
				}				

				MyLog_Level1->OBJECTS_LOG =NULL;

				//Put its groups into root!
				//Remove it from the list!
				//Delete it!
				delete MyLog_Level1;
			}//If MyLog_Level
			else // Si ce group n'est pas à supprimer!
			{
				if (root_exists ==0)
				{root_exists =1;
				 MyLog_Level1_Root = MyLog_Level1;
				 MyLog_Level1_Last = MyLog_Level1;
				 MyLog_Level1_Last->nextobj = NULL;
				}
				else 
				{
					MyLog_Level1_Last->nextobj = MyLog_Level1;
					MyLog_Level1_Last = MyLog_Level1;
					MyLog_Level1_Last->nextobj = NULL;
				}
			}

			if (MyLog_Level1_next == NULL){Ok =0;}
			else{MyLog_Level1 = MyLog_Level1_next;}			
		}// While level1 containers
		if (root_exists ==0)
		{this->OBJECTS_LOG =NULL;}
		else
		{this->OBJECTS_LOG = MyLog_Level1_Root;}
	}// if level ==0 (root)
}

void OBJECT_LOG::Mesh_Group()
{
	int cpt=0;
	int Ok = 0;
	OBJECT_MESH *MyMesh;
	//OBJECT_LANDMARK *MyLandmark; // We do not group landmarks any more for the moment...
	OBJECT_LOG  *MyLog;
	if (level ==0) // Dans un root;
	{
		//	One logical object SHOULD hold a landmark list *OBJECT_LANDMARK and a *OBJECT_MESH!
		//	I HAVE TO MOVE THE PLACE WHERE LANDMARKS ARE SORTED		    
	
		//	 Quand on groupe :
		//					   0 cpt de tous les objets selectionnés... + que 1 ?
		//					   1 recherche tous les OBJECT_MESH selectionnés (ceux là sont "free")
		//				
		//					   1' recherche de tous les OBJECT_LANDMARK Selectionnés.	
		//					   1'' recherche de tous les OBJECT_LOGICAL selectionnés
		
		//					   2 creation d'un OBJECT_LOGICAL pour lequel ON met dans 
		//								*OBJECT_MESH les Mesh de 1
		//						        *OBJECT_LANDMARK les landmarks de 1'
		//								*OBJECT_LOGICAL les objets logiques sélectionnés.
		//					   3  on met ce nouvel objet logique dans la pile de l'objet BASAL!

		// 0 Count every selected object
		if (this->OBJECTS !=NULL)
		{
			Ok = 1;
			MyMesh = this->OBJECTS;
		}
		while (Ok !=0)
		{
			if (MyMesh->selected ==1)
			{cpt++;}
			if (MyMesh->nextobj != NULL)
			{MyMesh = MyMesh->nextobj;}
			else
			{Ok = 0;}
		}
		if (this->OBJECTS_LOG !=NULL)
		{
			Ok = 1;
			MyLog = this->OBJECTS_LOG;
		}
		while (Ok!=0)
		{
			if (MyLog->selected ==1)
			{cpt++;}
			if (MyLog->nextobj != NULL)
			{MyLog = MyLog->nextobj;}
			else
			{Ok = 0;}
		
		}
		
	//1 then group all
		if (cpt >0)
		{

			//Create a new logical object !
			MyLog = new OBJECT_LOG;
			MyLog->color[0] = 0.509803922;
			MyLog->color[1] = 0.305882353;
			MyLog->color[2] = 0.184313725;
				



			MyLog->level =1; // We create a logical object of level 1 (subject to rotation, translation!)
			MyLog->name = "Group";
			
			//A : group all the selected meshes into MyLog
			this->Mesh_Group_Mesh (MyLog);
		
			//B : group all the selected logical objects into MyLog
			this->Mesh_Group_Log(MyLog);
			
			//C : group all the selected landmarks into MyLog
			//this->Mesh_Group_Landmarks (MyLog, 0);

			//D : group all the selected target landmarks into MyLog
			//this->Mesh_Group_Landmarks (MyLog, 1);

			//C Calculate new center of rotation
			MyLog->mean[0] = 0;
			MyLog->mean[1] = 0;
			MyLog->mean[2] = 0;
			int nbtmp = 0;
			glPushMatrix();
			glLoadIdentity();
			MyLog->Calculate_GlobalMean(&MyLog->mean[0], &MyLog->mean[1], &MyLog->mean[2], &nbtmp, 0);
			glPopMatrix();
			//MyLog->mean[0] *= -1;
			//MyLog->mean[1] *= -1;
			//MyLog->mean[2] *= -1;
			
			if (nbtmp !=0)
			{
				MyLog->mean[0] /= nbtmp;
				MyLog->mean[1] /= nbtmp;
				MyLog->mean[2] /= nbtmp;
			}
			std::cout<<"-----------------------------------"<<std::endl;
			std::cout<<"New object LOG: Mean[0]="<<MyLog->mean[0]<<", Mean[1]"<<MyLog->mean[1]<<", Mean[2]="<<MyLog->mean[2]<<"nbtmp="<<nbtmp<<std::endl;
			std::cout<<"-----------------------------------"<<std::endl;
			MyLog->nmean=nbtmp;
			//D Add MyLog into this
			this->Add_ObjectLog (MyLog);
			MyLog->level =1;
		}
		else
		{printf("You must select at least one object to create a group.\n");}

	}// if level ==0 (root)
}//end function
void OBJECT_LOG::Mesh_Group_Log(OBJECT_LOG *MyLog)
{

	OBJECT_LOG * Parent;
	OBJECT_LOG * Child;
	OBJECT_LOG * SubChild;
		
	    int Ok = 1;
		int Root_found = 0;
		Parent = Child = SubChild = NULL;
		
		if (this->OBJECTS_LOG == NULL)
		{Ok = 0;}
		else
		{
			Parent = this->OBJECTS_LOG;
			
		}				
		//First : find new root of the OBJECTS pile;
		while (Ok!=0)
		{	if (Parent->selected==1)
			{
				if (Parent->nextobj !=NULL)
				{
					Child = Parent->nextobj;
					MyLog->Add_ObjectLog(Parent);
					Parent->Mesh_Increase_Level();

					//Parent = NULL;
					Parent = Child;	
					Parent->prevobj = NULL;
				}
				else 
				{				
					//delete Parent;
					MyLog->Add_ObjectLog(Parent);
					Parent->Mesh_Increase_Level();
					//Parent = NULL;
					this->OBJECTS_LOG = NULL;
					Ok =0;
				}			 		
			}
			else //Root exists!
			{Ok = 0;
			this->OBJECTS_LOG = Parent;
			Root_found = 1;
			}
		}
	
		// Now delete every selected child;
		if (this->OBJECTS_LOG == NULL)
		{Ok = 0;}
		else
		{
			Ok = 1;
			Parent = this->OBJECTS_LOG;
			Child = Parent->nextobj;
			if (Child !=NULL)
			{SubChild = Child->nextobj;}
			
		}	
		//Parent can never be selected
		while (Ok !=0)
		{			
			Ok = Ok;						
			if (Child != NULL)
			{
				if (Child->selected == 1)
				{	//delete Child;
					MyLog->Add_ObjectLog(Child);
					Child->Mesh_Increase_Level();

					Child = NULL;
					Parent->nextobj = NULL;
					if (SubChild == NULL)
					{
					
						Ok = 0; //fini
					}
					else
					{						

						Parent->nextobj = SubChild;
						SubChild->prevobj = Parent;
						Child = SubChild;
						if (SubChild !=NULL)
						{SubChild = SubChild->nextobj;}
					}
				}
				else
				{
					Parent = Child;
					Child = SubChild;
					if (SubChild !=NULL)
					{SubChild = SubChild->nextobj;}
				}
			}
			else
			{Ok = 0;}																							
		}//while


}
void OBJECT_LOG::Mesh_Group_Mesh(OBJECT_LOG *MyLog)
{
	OBJECT_MESH * Parent;
	OBJECT_MESH * Child;
	OBJECT_MESH * SubChild;
		
	    int Ok = 1;
		int Root_found = 0;
		Parent = Child = SubChild = NULL;
		
		if (this->OBJECTS == NULL)
		{Ok = 0;}
		else
		{
			Parent = this->OBJECTS;
			
		}				
		//First : find new root of the OBJECTS pile;
		while (Ok!=0)
		{	if (Parent->selected==1)
			{
				if (Parent->nextobj !=NULL)
				{
					Child = Parent->nextobj;
					MyLog->Add_ObjectMesh(Parent);
					//Parent = NULL;
					Parent = Child;	
					Parent->prevobj = NULL;
				}
				else 
				{				
					//delete Parent;
					MyLog->Add_ObjectMesh(Parent);
					//Parent = NULL;
					this->OBJECTS = NULL;
					Ok =0;
				}			 		
			}
			else //Root exists!
			{Ok = 0;
			this->OBJECTS = Parent;
			Root_found = 1;
			}
		}
		// Now delete every selected child;
		if (this->OBJECTS == NULL)
		{Ok = 0;}
		else
		{
			Ok = 1;
			Parent = this->OBJECTS;
			Child = Parent->nextobj;
			if (Child !=NULL)
			{SubChild = Child->nextobj;}
			
		}	
		//Parent can never be selected
		while (Ok !=0)
		{			
			Ok = Ok;						
			if (Child != NULL)
			{
				if (Child->selected == 1)
				{
					//delete Child;
					MyLog->Add_ObjectMesh(Child);

					Child = NULL;
					Parent->nextobj = NULL;
					if (SubChild == NULL)
					{
						Ok = 0; //fini
					}
					else
					{						

						Parent->nextobj = SubChild;
						SubChild->prevobj = Parent;
						Child = SubChild;
						if (SubChild !=NULL)
						{SubChild = SubChild->nextobj;}
					}
				}
				else
				{
					Parent = Child;
					Child = SubChild;
					if (SubChild !=NULL)
					{SubChild = SubChild->nextobj;}
				}
			}
			else
			{
				Ok = 0;
			}																							
		}//while
		

}
void OBJECT_LOG::Mesh_Group_Landmarks(OBJECT_LOG *MyLog, int landmark_mode)
{
	OBJECT_LANDMARK * Parent;
	OBJECT_LANDMARK * Child;
	OBJECT_LANDMARK * SubChild;
		
	    int Ok = 1;
		int Root_found = 0;
		Parent = Child = SubChild = NULL;
		if (landmark_mode ==0)
		{
			if (this->OBJECTS_LANDMARK == NULL)
			{Ok = 0;}
			else
			{
				Parent = this->OBJECTS_LANDMARK;
				
			}
		}
		else if (landmark_mode ==1)
		{
			if (this->OBJECTS_LANDMARK_TARGET == NULL)
			{Ok = 0;}
			else
			{
				Parent = this->OBJECTS_LANDMARK_TARGET;
				
			}
		}
		else 
		{
			if (this->OBJECTS_LANDMARK_LABELS == NULL)
			{Ok = 0;}
			else
			{
				Parent = this->OBJECTS_LANDMARK_LABELS;
				
			}
		}

		//First : find new root of the OBJECTS pile;
		while (Ok!=0)
		{	if (Parent->selected==1)
			{
				if (Parent->nextobj !=NULL)
				{
					Child = Parent->nextobj;
					//printf("Delete parent 1 \n");
					MyLog->Add_ObjectLandmark(Parent, landmark_mode);
					//Parent = NULL;
					Parent = Child;	
					Parent->prevobj = NULL;
				}
				else 
				{				
					//delete Parent;
					MyLog->Add_ObjectLandmark(Parent, landmark_mode);	
					if (landmark_mode ==0)
					{
						this->OBJECTS_LANDMARK = NULL;
					}
					else if (landmark_mode ==1)
					{
						this->OBJECTS_LANDMARK_TARGET = NULL;
					}
					else
					{
						this->OBJECTS_LANDMARK_LABELS = NULL;
					}
					Ok =0;
				}			 		
			}
			else //Root exists!
			{Ok = 0;
			if (landmark_mode ==0)
			{
				this->OBJECTS_LANDMARK = Parent;
			}
			else if (landmark_mode ==1)
			{
				this->OBJECTS_LANDMARK_TARGET = Parent;
			}
			else 
			{
				this->OBJECTS_LANDMARK_LABELS = Parent;
			}
			Root_found = 1;
			}
		}
	
		// Now delete every selected child;
		if (landmark_mode ==0)
		{
			if (this->OBJECTS_LANDMARK == NULL)
			{Ok = 0;}
			else
			{
				Ok = 1;
				Parent = this->OBJECTS_LANDMARK;
				Child = Parent->nextobj;
				if (Child !=NULL)
				{SubChild = Child->nextobj;}
				
			}	
		}
		else if (landmark_mode ==1)
		{
			if (this->OBJECTS_LANDMARK_TARGET == NULL)
			{Ok = 0;}
			else
			{
				Ok = 1;
				Parent = this->OBJECTS_LANDMARK_TARGET;
				Child = Parent->nextobj;
				if (Child !=NULL)
				{SubChild = Child->nextobj;}
				
			}	
		}
		else
		{
			if (this->OBJECTS_LANDMARK_LABELS == NULL)
			{Ok = 0;}
			else
			{
				Ok = 1;
				Parent = this->OBJECTS_LANDMARK_LABELS;
				Child = Parent->nextobj;
				if (Child !=NULL)
				{SubChild = Child->nextobj;}
				
			}	
		}
		//Parent can never be selected
		while (Ok !=0)
		{			
			Ok = Ok;						
			if (Child != NULL)
			{
				if (Child->selected == 1)
				{	
					//delete Child;
					MyLog->Add_ObjectLandmark(Child, landmark_mode);

					Child = NULL;
					Parent->nextobj = NULL;
					if (SubChild == NULL)
					{
						Ok = 0; //fini
					}
					else
					{						

						Parent->nextobj = SubChild;
						SubChild->prevobj = Parent;
						Child = SubChild;
						if (SubChild !=NULL)
						{SubChild = SubChild->nextobj;}
					}
				}
				else
				{
					Parent = Child;
					Child = SubChild;
					if (SubChild !=NULL)
					{SubChild = SubChild->nextobj;}
				}
			}
			else
			{
				Ok = 0;
			}																							
		}//while
		

}
int OBJECT_LOG::Mesh_Select(float xmin, float xmax, float ymin, float ymax, int select_mode)
{
	//To be changed
	OBJECT_MESH * MyObj;
	int bchanged= 0;
	int bchanged2 = 0;
	int bchanged3 = 0;
	int bchanged4 = 0;
	int bchanged5 = 0;
	// If level ==0 (root sera un unique objet impossible à supprimer...
	if (this->level == 0) // Normal way to select
	{
			//normal way to select
			if (this->OBJECTS !=NULL && select_mode !=3)
			{
				MyObj = this->OBJECTS;
				int bchanged2 =0;
				while (MyObj !=NULL)
				{
					bchanged2 = MyObj->Mesh_Select(xmin, xmax, ymin, ymax);
					if (bchanged2==1 && bchanged ==0)
					{bchanged = 1;}
					MyObj = MyObj->nextobj;
				}
			}
			// Grouped Objects changed
			if (this->OBJECTS_LOG !=NULL && select_mode !=3)
			{
				bchanged3 = this->OBJECTS_LOG->Mesh_Select(xmin, xmax, ymin, ymax, select_mode);
			}
			if (this->OBJECTS_LANDMARK !=NULL)
			{
				bchanged2 = this->OBJECTS_LANDMARK->Landmark_Select(xmin,xmax,ymin,ymax);
				printf ("Normal level 0 landmark selection: %d\n", bchanged2);
			}
			if (this->OBJECTS_LANDMARK_TARGET !=NULL)
			{
				bchanged4 = this->OBJECTS_LANDMARK_TARGET->Landmark_Select(xmin,xmax,ymin,ymax);
			}
			if (this->OBJECTS_LANDMARK_LABELS !=NULL)
			{
				bchanged5 = this->OBJECTS_LANDMARK_LABELS->Landmark_Select(xmin,xmax,ymin,ymax);
			}
			if (bchanged3 ==1){bchanged =1;}
			if (bchanged2 ==1){bchanged =1;}
			if (bchanged4 ==1){bchanged =1;}
			if (bchanged5 ==1){bchanged =1;}
			//bchanged = bchanged;
	}
	else
	{
			//select into a group!
			// What to we do about Mat1 and Mat2 of this ????
			glPushMatrix();
	  		glMultMatrixf((GLfloat*) this->Mat2);
			
			glTranslated(
					this->mean[0],
					this->mean[1],
					this->mean[2]
					);	
			glMultMatrixf((GLfloat*) this->Mat1); //Aspect
			glTranslated(
					-this->mean[0],
					-this->mean[1],
					-this->mean[2]
			);	

			if (this->OBJECTS_LANDMARK !=NULL && select_mode !=3)
			{
				bchanged2 = this->OBJECTS_LANDMARK->Landmark_Select(xmin,xmax,ymin,ymax);
			
			}
			if (this->OBJECTS_LANDMARK_TARGET !=NULL && select_mode !=3)
			{
				bchanged4 = this->OBJECTS_LANDMARK_TARGET->Landmark_Select(xmin,xmax,ymin,ymax);
			}
			if (this->OBJECTS_LANDMARK_LABELS !=NULL && select_mode !=3)
			{
				bchanged5 = this->OBJECTS_LANDMARK_LABELS->Landmark_Select(xmin,xmax,ymin,ymax);
			}
			if (this->OBJECTS !=NULL && select_mode !=3)
			{
				//this->OBJECTS->Mesh_Select(xmin, xmax, ymin, ymax);
				MyObj = this->OBJECTS;
				int bchanged2 =0;
				while (MyObj !=NULL)
				{
					bchanged2 = MyObj->Mesh_Select(xmin, xmax, ymin, ymax);
					if (bchanged2==1 && bchanged ==0)
					{bchanged = 1;}
					MyObj = MyObj->nextobj;
				}
				
			}
			// Grouped Objects changed
			if (this->OBJECTS_LOG !=NULL && select_mode !=3)
			{
				bchanged3 = this->OBJECTS_LOG->Mesh_Select(xmin, xmax, ymin, ymax, select_mode);
			}			
			
			if (bchanged3 ==1){bchanged =1;}
			if (bchanged4 ==1){bchanged =1;}
			if (bchanged5 ==1){bchanged =1;}
			if (bchanged2 ==1){bchanged =1;}
			//bchanged = bchanged;
			// Normal Objects changed ?
		
			glPopMatrix();
	}
	
	if (bchanged)
	{
		if( this->level>0)
		{
			if (this->selected ==0)
			{this->selected =1;}
			else
			{this->selected =0;}
		}
	}
	if (this->nextobj !=NULL)
	{bchanged2 = this->nextobj->Mesh_Select(xmin, xmax, ymin, ymax, select_mode);}
	if (bchanged2 ==1){bchanged =1;}
	
	return bchanged;
}

void OBJECT_LOG::Mesh_Mirror_ThroughY()
{
	
	/*if (this->OBJECTS !=NULL)
	{
		//In this case
		this->OBJECTS->Mesh_Mirror_ThroughY();
	}*/
	if (this->OBJECTS_LANDMARK!=NULL)
	{
		std::cout << "Call Mirror landmarks"<< std::endl;
		this->OBJECTS_LANDMARK->Landmark_Mirror_ThroughY();
		std::cout << "Mirror landmarks Called"<< std::endl;
	}
	if (this->OBJECTS_LANDMARK_TARGET!=NULL)
	{
		this->OBJECTS_LANDMARK_TARGET->Landmark_Mirror_ThroughY();
	}
	if (this->OBJECTS_LANDMARK_LABELS!=NULL)
	{
		this->OBJECTS_LANDMARK_LABELS->Landmark_Mirror_ThroughY();
	}
	//
	if (this->nextobj !=NULL)
	{this->nextobj->Mesh_Mirror_ThroughY();}
}

void OBJECT_LOG::Mesh_invert(int select_mode)
{
	//Implement
	// If level ==0 (root sera un unique objet impossible à supprimer...
	if (this->OBJECTS !=NULL)
	{
		//In this case
		this->OBJECTS->Mesh_invert(select_mode);
	}
	
	
	if (this->nextobj !=NULL)
	{this->nextobj->Mesh_invert(select_mode);}
}

void OBJECT_LOG::Mesh_invert_normals(int select_mode)
{
	//Implement
	// If level ==0 (root sera un unique objet impossible à supprimer...
	if (this->OBJECTS !=NULL)
	{
		//In this case
		this->OBJECTS->Mesh_invert_normals(select_mode);
	}
	
	
	if (this->nextobj !=NULL)
	{this->nextobj->Mesh_invert_normals(select_mode);}
}
OBJECT_LANDMARK * OBJECT_LOG::Mesh_CreateLandmarkAtMouse(float x, float y)
{
	//Implement
	// If level ==0 (root sera un unique objet impossible à supprimer...
	OBJECT_LANDMARK *Landmarks = new OBJECT_LANDMARK;
	return Landmarks;
}
int OBJECT_LOG::GetNumtri()
{
 int my_numtri = 0;
 int Ok = 1;my_numtri = 0;
 OBJECT_MESH * My_Obj;
 OBJECT_LOG * My_ObjLog;
 if (this->OBJECTS !=NULL)
 {My_Obj = this->OBJECTS;}
 else {Ok = 0;} 
 while (Ok!=0)
  {
	my_numtri+=My_Obj->numtri;
	if (My_Obj->nextobj != NULL)
	{My_Obj = My_Obj->nextobj;}
	else
	{Ok=0;}
  }	  
  Ok =1;
  if (this->OBJECTS_LOG !=NULL)
  {My_ObjLog = this->OBJECTS_LOG;}
  else {Ok =0;}
	  
  while (Ok!=0)
  {	
	my_numtri+=My_ObjLog->GetNumtri();
	if (My_ObjLog->nextobj != NULL)
	{My_ObjLog = My_ObjLog->nextobj;}
	else
	{Ok=0;}
  }
 return my_numtri;
}
void OBJECT_LOG::Hierarchy (int depth)
{
	for (int i = 1;i<=depth; i++)
	{std::cout<<"   ";}
	
	std::cout<<this->name<<" level "<<this->level<<std::endl;
	if (this->OBJECTS !=NULL)
	{
		this->OBJECTS->Hierarchy (depth +1);
	}
	std::cout<<"Normal Landmarks"<<std::endl;
	if (this->OBJECTS_LANDMARK !=NULL)
	{this->OBJECTS_LANDMARK->Hierarchy (depth +1);}
	std::cout<<"Target Landmarks"<<std::endl;
	if (this->OBJECTS_LANDMARK_TARGET !=NULL)
	{this->OBJECTS_LANDMARK_TARGET->Hierarchy (depth +1);}
	std::cout<<"Flag landmarks"<<std::endl;
	if (this->OBJECTS_LANDMARK_LABELS !=NULL)
	{this->OBJECTS_LANDMARK_LABELS->Hierarchy (depth +1);}
	if (this->OBJECTS_LOG !=NULL)
	{this->OBJECTS_LOG->Hierarchy (depth +1);}

	if (this->nextobj !=NULL)
	{this->nextobj->Hierarchy(depth);}
}

void OBJECT_LOG::get_object_names_and_view_values(std::vector<std::string>& object_names, std::vector<int>& object_view_values)
{
	if (this->OBJECTS !=NULL)
	{
		this->OBJECTS->get_object_names_and_view_values(object_names, object_view_values);
	}

}

void OBJECT_LOG::Mesh_area_volume()
{
	// If level == 0 (root sera un unique objet impossible à supprimer...
	if (this->OBJECTS !=NULL)
	{
		this->OBJECTS->Mesh_area_volume();							
	}
  if (this->nextobj !=NULL)
  {nextobj->Mesh_area_volume();}

}
void OBJECT_LOG::Set_Blend(int blend_value)
{
	if (this->selected ==1)
	{
		this->color[3] = (float)blend_value/100;
		this->selected =0;
	}
	if (this->OBJECTS != NULL)
	{this->OBJECTS->Set_Blend(blend_value);}
	if (this->OBJECTS_LOG != NULL)
	{this->OBJECTS_LOG->Set_Blend(blend_value);}

	if (this->nextobj != NULL)
	{nextobj->Set_Blend(blend_value);}
}

void OBJECT_LOG::SC_calc_depth()
{
	if (this->OBJECTS != NULL)
	{this->OBJECTS->SC_calc_depth();}
	if (this->nextobj != NULL)
	{nextobj->SC_calc_depth();}
}
void OBJECT_LOG::SC_calc_thickness(float thickness_max_distance, Fl_Double_Window* thicknessWindow)
{
	if (this->OBJECTS != NULL)
	{this->OBJECTS->SC_calc_thickness(thickness_max_distance, thicknessWindow);}
	if (this->nextobj != NULL)
	{nextobj->SC_calc_thickness(thickness_max_distance, thicknessWindow);}
}
void OBJECT_LOG::SC_calc_mean()
{
	if (this->OBJECTS != NULL)
	{this->OBJECTS->SC_calc_mean();}
	if (this->nextobj != NULL)
	{nextobj->SC_calc_mean();}
	
}

void OBJECT_LOG::Curve_start(int direction)
{
 int Ok =1;
	if (this->OBJECTS_LANDMARK ==NULL)
		{Ok=0;}
		else
		{
			this->OBJECTS_LANDMARK->Curve_start(direction);
		}
	if (this->OBJECTS_LANDMARK_TARGET ==NULL)
		{Ok=0;}
		else
		{
			this->OBJECTS_LANDMARK_TARGET->Landmark_Unselect();
		}

}
void OBJECT_LOG::Mesh_Draw_Curve()
{
	
	
}


int OBJECT_LOG::Get_Landmark_Number(int landmark_mode)
{
	OBJECT_LANDMARK *Landmark_list;
	int cpt;
	cpt = 0;
	int Ok = 1;
	if (landmark_mode==0)
	{
		if (this->OBJECTS_LANDMARK ==NULL)
		{Ok=0;}
		else
		{
			Landmark_list = this->OBJECTS_LANDMARK;
			while (Ok)
			{
				cpt++;
				if (Landmark_list->nextobj !=NULL)
				{Landmark_list = Landmark_list->nextobj;}
				else
				{Ok=0;}
			}
		}
	}
	else if (landmark_mode ==1)
	{
		if (this->OBJECTS_LANDMARK_TARGET ==NULL)
		{Ok=0;}
		else
		{
			Landmark_list = this->OBJECTS_LANDMARK_TARGET;
			while (Ok)
			{
				cpt++;
				if (Landmark_list->nextobj !=NULL)
				{Landmark_list = Landmark_list->nextobj;}
				else
				{Ok=0;}
			}
		}
	}
	else
	{
		if (this->OBJECTS_LANDMARK_LABELS ==NULL)
		{Ok=0;}
		else
		{
			Landmark_list = this->OBJECTS_LANDMARK_LABELS;
			while (Ok)
			{
				cpt++;
				if (Landmark_list->nextobj !=NULL)
				{Landmark_list = Landmark_list->nextobj;}
				else
				{Ok=0;}
			}
		}
	}
	if (this->OBJECTS_LOG!=NULL)
	{cpt += this->OBJECTS_LOG->Get_Landmark_Number(landmark_mode);}	
	if (this->nextobj !=NULL)
	{cpt += this->nextobj->Get_Landmark_Number(landmark_mode);}
	return cpt;
}



OBJECT_LANDMARK* OBJECT_LOG::LandmarkAfter(int afterind, int landmark_mode)
{
	OBJECT_LANDMARK *MyNextLandmark;
	OBJECT_LANDMARK *MyNextLandmark2;
	OBJECT_LANDMARK *OneLandmark;
	
	MyNextLandmark = NULL;
	int min_ind = 1000000;
	if (landmark_mode ==0)
	{

		if (this->OBJECTS_LANDMARK !=NULL)
		{
			int Ok = 1;
			OneLandmark = this->OBJECTS_LANDMARK;
			while (Ok)
			{
				//If afterind<OneLandmark <min_ind
				if ((OneLandmark->landmark_index <min_ind)&&(OneLandmark->landmark_index >afterind))
				{
					min_ind = OneLandmark->landmark_index;
					MyNextLandmark = OneLandmark;

				}
				if (OneLandmark->nextobj!=NULL)
				{OneLandmark= OneLandmark->nextobj;}
				else
				{Ok = 0;}
			}
		
		}
	}
	else if (landmark_mode ==1)
	{
		if (this->OBJECTS_LANDMARK_TARGET !=NULL)
		{
			int Ok = 1;
			OneLandmark = this->OBJECTS_LANDMARK_TARGET;
			while (Ok)
			{
				//If afterind<OneLandmark <min_ind
				if ((OneLandmark->landmark_index <min_ind)&&(OneLandmark->landmark_index >afterind))
				{
					min_ind = OneLandmark->landmark_index;
					MyNextLandmark = OneLandmark;

				}
				if (OneLandmark->nextobj!=NULL)
				{OneLandmark= OneLandmark->nextobj;}
				else
				{Ok = 0;}
			}
		
		}
	}
	else
	{
		if (this->OBJECTS_LANDMARK_LABELS !=NULL)
		{
			int Ok = 1;
			OneLandmark = this->OBJECTS_LANDMARK_LABELS;
			while (Ok)
			{
				//If afterind<OneLandmark <min_ind
				if ((OneLandmark->landmark_index <min_ind)&&(OneLandmark->landmark_index >afterind))
				{
					min_ind = OneLandmark->landmark_index;
					MyNextLandmark = OneLandmark;

				}
				if (OneLandmark->nextobj!=NULL)
				{OneLandmark= OneLandmark->nextobj;}
				else
				{Ok = 0;}
			}
		
		}
	}

	if (this->nextobj !=NULL)
	{
		MyNextLandmark2 = this->nextobj->LandmarkAfter(afterind, landmark_mode);
		if (MyNextLandmark2 !=NULL)
		{
			if (MyNextLandmark2->landmark_index < min_ind)
			{
					MyNextLandmark  = MyNextLandmark2;
					min_ind = MyNextLandmark2->landmark_index;
			}
		}
	}

	if (this->OBJECTS_LOG !=NULL)
	{
		MyNextLandmark2 = this->OBJECTS_LOG->LandmarkAfter(afterind, landmark_mode);
		if (MyNextLandmark2 !=NULL)
		{
			if (MyNextLandmark2->landmark_index < min_ind)
			{
					MyNextLandmark  = MyNextLandmark2;
					min_ind = MyNextLandmark2->landmark_index;
			}
		}
	
	}
	return MyNextLandmark;
}


OBJECT_LANDMARK* OBJECT_LOG::LandmarkBefore(int beforeind, int landmark_mode)
{
	OBJECT_LANDMARK *MyPrecedingLandmark;
	OBJECT_LANDMARK *MyPrecedingLandmark2;
	OBJECT_LANDMARK *OneLandmark;
	// on cherche le plus grand indice qui soit plus petit que before ind
	MyPrecedingLandmark = NULL;
	int max_ind = 0;
	if (landmark_mode ==0)
	{

		if (this->OBJECTS_LANDMARK !=NULL)
		{
			int Ok = 1;
			OneLandmark = this->OBJECTS_LANDMARK;
			while (Ok)
			{
				//If afterind<OneLandmark <max_ind
				if ((OneLandmark->landmark_index >max_ind)&&(OneLandmark->landmark_index <beforeind))
				{
					max_ind = OneLandmark->landmark_index;
					MyPrecedingLandmark = OneLandmark;

				}
				if (OneLandmark->nextobj!=NULL)
				{OneLandmark= OneLandmark->nextobj;}
				else
				{Ok = 0;}
			}
		
		}
	}
	else if (landmark_mode ==1)
	{
		if (this->OBJECTS_LANDMARK_TARGET !=NULL)
		{
			int Ok = 1;
			OneLandmark = this->OBJECTS_LANDMARK_TARGET;
			while (Ok)
			{
				//If afterind<OneLandmark <min_ind
				if ((OneLandmark->landmark_index >max_ind)&&(OneLandmark->landmark_index <beforeind))
				{
					max_ind = OneLandmark->landmark_index;
					MyPrecedingLandmark = OneLandmark;

				}
				if (OneLandmark->nextobj!=NULL)
				{OneLandmark= OneLandmark->nextobj;}
				else
				{Ok = 0;}
			}
		
		}
	}
	else 
	{
		if (this->OBJECTS_LANDMARK_LABELS !=NULL)
		{
			int Ok = 1;
			OneLandmark = this->OBJECTS_LANDMARK_LABELS;
			while (Ok)
			{
				//If afterind<OneLandmark <min_ind
				if ((OneLandmark->landmark_index >max_ind)&&(OneLandmark->landmark_index <beforeind))
				{
					max_ind = OneLandmark->landmark_index;
					MyPrecedingLandmark = OneLandmark;

				}
				if (OneLandmark->nextobj!=NULL)
				{OneLandmark= OneLandmark->nextobj;}
				else
				{Ok = 0;}
			}
		
		}
	}

	if (this->nextobj !=NULL)
	{
		MyPrecedingLandmark2 = this->nextobj->LandmarkBefore(beforeind, landmark_mode);
		if (MyPrecedingLandmark2 !=NULL)
		{
			if (MyPrecedingLandmark2->landmark_index > max_ind)
			{
					MyPrecedingLandmark  = MyPrecedingLandmark2;
					max_ind = MyPrecedingLandmark2->landmark_index;
			}
		}
	}

	if (this->OBJECTS_LOG !=NULL)
	{
		MyPrecedingLandmark2 = this->OBJECTS_LOG->LandmarkBefore(beforeind, landmark_mode);
		if (MyPrecedingLandmark2 !=NULL)
		{
			if (MyPrecedingLandmark2->landmark_index > max_ind)
			{
					MyPrecedingLandmark  = MyPrecedingLandmark2;
					max_ind = MyPrecedingLandmark2->landmark_index;
			}
		}
	
	}
	return MyPrecedingLandmark;
}

OBJECT_LANDMARK* OBJECT_LOG::Get_Selected_Landmark(int landmark_mode)
{
	OBJECT_LANDMARK *Landmark_list;
	int cpt;
	cpt = 0;
	int Ok = 1;
	if (landmark_mode ==0)
	{
		if (this->OBJECTS_LANDMARK ==NULL && this->level ==0)
		{Ok=0; return NULL;}
		else
		{
			Landmark_list = this->OBJECTS_LANDMARK;
			while (Ok)
			{
				if (Landmark_list->selected ==1)
				{
					Ok=0;
				}
				else if (Landmark_list->nextobj !=NULL)
				{Landmark_list = Landmark_list->nextobj;}
				else
				{Ok=0;return NULL;}
			}
		}
	}
	else if (landmark_mode ==1)
	{
		if (this->OBJECTS_LANDMARK_TARGET ==NULL && this->level ==0)
		{Ok=0; return NULL;}
		else
		{
			Landmark_list = this->OBJECTS_LANDMARK_TARGET;
			while (Ok)
			{
				if (Landmark_list->selected ==1)
				{
					Ok=0;
				}
				else if (Landmark_list->nextobj !=NULL)
				{Landmark_list = Landmark_list->nextobj;}
				else
				{Ok=0;return NULL;}
			}
		}
	}
	else 
	{
		if (this->OBJECTS_LANDMARK_LABELS ==NULL && this->level ==0)
		{Ok=0; return NULL;}
		else
		{
			Landmark_list = this->OBJECTS_LANDMARK_LABELS;
			while (Ok)
			{
				if (Landmark_list->selected ==1)
				{
					Ok=0;
				}
				else if (Landmark_list->nextobj !=NULL)
				{Landmark_list = Landmark_list->nextobj;}
				else
				{Ok=0;return NULL;}
			}
		}
	}
	return Landmark_list;
}
int OBJECT_LOG::Get_Landmark_Selected(int landmark_mode)
{
	//Only level 1 landmarks
	OBJECT_LANDMARK *Landmark_list;
	int cpt;
	cpt = 0;
	int Ok = 1;
	if (landmark_mode ==0)
	{
		if (this->OBJECTS_LANDMARK ==NULL && this->level ==0)
		{Ok=0;}
		else
		{
			Landmark_list = this->OBJECTS_LANDMARK;
			while (Ok)
			{
				if (Landmark_list->selected ==1)
				{
					cpt++;
				}
				if (Landmark_list->nextobj !=NULL)
				{Landmark_list = Landmark_list->nextobj;}
				else
				{Ok=0;}
			}
		}
	}
	else if (landmark_mode ==1)
	{
		if (this->OBJECTS_LANDMARK_TARGET ==NULL && this->level ==0)
		{Ok=0;}
		else
		{
			Landmark_list = this->OBJECTS_LANDMARK_TARGET;
			while (Ok)
			{
				if (Landmark_list->selected ==1)
				{
					cpt++;
				}
				if (Landmark_list->nextobj !=NULL)
				{Landmark_list = Landmark_list->nextobj;}
				else
				{Ok=0;}
			}
		}	
	}

	else 
	{
		if (this->OBJECTS_LANDMARK_LABELS ==NULL && this->level ==0)
		{Ok=0;}
		else
		{
			Landmark_list = this->OBJECTS_LANDMARK_LABELS;
			while (Ok)
			{
				if (Landmark_list->selected ==1)
				{
					cpt++;
				}
				if (Landmark_list->nextobj !=NULL)
				{Landmark_list = Landmark_list->nextobj;}
				else
				{Ok=0;}
			}
		}	
	}

	return cpt;
}

void OBJECT_LOG::SetLandmarkSize(float landmark_size)
{
			
		
		if (this->OBJECTS_LANDMARK !=NULL)
		{
			this->OBJECTS_LANDMARK->SetLandmarkSize(landmark_size, 0);
		}
		if (this->OBJECTS_LANDMARK_TARGET !=NULL)
		{
			this->OBJECTS_LANDMARK_TARGET->SetLandmarkSize(landmark_size, 1);
		}
		if (this->OBJECTS_LANDMARK_LABELS !=NULL)
		{
			this->OBJECTS_LANDMARK_LABELS->SetLandmarkSize(landmark_size, 2);
		}
		if (this->OBJECTS_LOG !=NULL)
		{
			this->OBJECTS_LOG->SetLandmarkSize(landmark_size);
		}

		if (this->nextobj !=NULL)
		{
			this->nextobj->SetLandmarkSize(landmark_size);
		}
}
		

void OBJECT_LOG::Add_ObjectLog(OBJECT_LOG * New_Obj)
{
	// Add to end of queue ?
	// Change to the beginning of the queue ?
	int Ok;
	Ok = 1;
	OBJECT_LOG * parent;
	parent = this->OBJECTS_LOG;
	if (parent ==NULL)
	{
		Ok=0;
		this->OBJECTS_LOG = New_Obj;
		New_Obj->prevobj = NULL;
	}
	else
	{
		while (Ok !=0)
		{
			if (parent->nextobj != NULL)
			{parent = parent->nextobj;}
			else
			{ Ok = 0;}
		}
		parent->nextobj = New_Obj;
		New_Obj->prevobj = parent;
	}
	
	New_Obj->nextobj = NULL;
	
}
float OBJECT_LOG::scalars_get_min()
{
	float my_min;
	float my_minlog;
	float my_minnext;
	my_min = NULL;
	my_minlog = NULL;
	my_minnext = NULL;
	
	if (this->OBJECTS != NULL)
	{
		my_min = this->OBJECTS->scalars_get_min();
	}
	if (this->OBJECTS_LOG != NULL)
	{
		my_minlog = this->OBJECTS_LOG->scalars_get_min();
		if (my_minlog !=NULL && my_min !=NULL)
		{	
			if (my_min > my_minlog){my_min = my_minlog;}
			else if (my_min == NULL && my_minlog !=NULL){my_min = my_minlog;}
		}	

	}
	if (this->nextobj !=NULL)
	{
		my_minnext = this->nextobj->scalars_get_min();
		if (my_minnext !=NULL && my_min !=NULL)
		{	
			if (my_min > my_minnext){my_min = my_minnext;}
			else if (my_min == NULL && my_minnext !=NULL){my_min = my_minnext;}
		}	
	}		
	return my_min;
}

float OBJECT_LOG::scalars_get_max()
{

	float my_max;
	float my_maxlog;
	float my_maxnext;
	my_max = NULL;
	my_maxlog = NULL;
	my_maxnext = NULL;
	
	if (this->OBJECTS != NULL)
	{
		my_max = this->OBJECTS->scalars_get_max();
	}
	if (this->OBJECTS_LOG != NULL)
	{
		my_maxlog = this->OBJECTS_LOG->scalars_get_max();
		if (my_maxlog !=NULL && my_max !=NULL)
		{	
			if (my_max < my_maxlog){my_max = my_maxlog;}
			else if (my_max == NULL && my_maxlog !=NULL){my_max = my_maxlog;}
		}	

	}
	if (this->nextobj !=NULL)
	{
		my_maxnext = this->nextobj->scalars_get_max();
		if (my_maxnext !=NULL && my_max !=NULL)
		{	
			if (my_max < my_maxnext){my_max = my_maxnext;}
			else if (my_max == NULL && my_maxnext !=NULL){my_max = my_maxnext;}
		}	
	}		
	return my_max;
}
void OBJECT_LOG::Add_ObjectMesh(OBJECT_MESH * New_Obj)
{
	// Add to end of queue ?
	// Change to the beginning of the queue ?
	int Ok;
	Ok = 1;
	OBJECT_MESH *parent;
	parent = this->OBJECTS;
	New_Obj->selected =0;
	if (parent == NULL)
	{
		Ok = 0;
		this->OBJECTS = New_Obj;
		New_Obj->prevobj = NULL;
	}
	else
	{
		while (Ok !=0)
		{
			if (parent->nextobj != NULL)
			{parent = parent->nextobj;}
			else
			{ Ok = 0;}
		}
		parent->nextobj = New_Obj;
		New_Obj->prevobj = parent;
	}
	New_Obj->selected=0;
	New_Obj->nextobj = NULL;	
}

void OBJECT_LOG::Add_ObjectLandmark(OBJECT_LANDMARK * New_Obj, int landmark_mode)
{
	// Add to end of queue ?
	// Change to the beginning of the queue ?
	int Ok;
	Ok = 1;

	OBJECT_LANDMARK *parent;
	if (landmark_mode ==0)
	{
		parent = this->OBJECTS_LANDMARK;
	}
	else if (landmark_mode ==1)
	{
		parent = this->OBJECTS_LANDMARK_TARGET;
	}
	else
	{
		parent = this->OBJECTS_LANDMARK_LABELS;
	}
	New_Obj->selected =0;
	if (parent == NULL)
	{
		Ok = 0;
		if (landmark_mode ==0)
		{
			this->OBJECTS_LANDMARK = New_Obj;
		}
		else if (landmark_mode ==1)
		{
			this->OBJECTS_LANDMARK_TARGET = New_Obj;
		}
		else
		{
			this->OBJECTS_LANDMARK_LABELS = New_Obj;
		}
		New_Obj->prevobj = NULL;
	}
	else
	{
		while (Ok !=0)
		{
			if (parent->nextobj != NULL)
			{parent = parent->nextobj;}
			else
			{ Ok = 0;}
		}
		parent->nextobj = New_Obj;
		New_Obj->prevobj = parent;
	}
	New_Obj->nextobj = NULL;	
}



void OBJECT_LOG::Mesh_Tri_Sort( int side)
{
	if (this->OBJECTS !=NULL)
	{
		this->OBJECTS->Mesh_Tri_Sort(side);
	}

	if (this->OBJECTS_LOG!=NULL)
	{
		this->OBJECTS_LOG->Mesh_Tri_Sort(side);
	}
}






OBJECT_LOG::OBJECT_LOG() : OBJECT()
{														// And Defines points
	//Constructor...
	this->level =0; // BOF!
	this->is_logical = 1;
	
	nextobj = NULL;
	prevobj = NULL;
	OBJECTS_LOG = NULL;
	OBJECTS = NULL;
	OBJECTS_LANDMARK = NULL;
	OBJECTS_LANDMARK_TARGET = NULL;
	OBJECTS_LANDMARK_LABELS = NULL;
	this->Mesh_init_Mat();
	this->mean[0] = 0;this->mean[1] = 0;this->mean[2] = 0;
	this->nmean=0;
	this->selected = 0;
	this->color[0] = 1;this->color[1] = 0;this->color[2] = 0; this->color[3] = 1;  
  
}



OBJECT_LOG::~OBJECT_LOG()//Destructor
{

	this->DeleteObjs();
  
}
