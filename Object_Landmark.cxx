#include "Object_Landmark.h"
#include <vtkSmartPointer.h>
#include <vtkTriangle.h>
#include <vtkCellArray.h>
#include <vtkPolyDataNormals.h>
#include <vtkCleanPolyData.h>
#include <vtkSphereSource.h>
#include <vtkFloatArray.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>


int OBJECT_LANDMARK::Is_Curve_Start()
{
	if (this->curve_start ==1 || this->landmark_index ==1)
	{return 1;}
	else 
	{return this->curve_start;}
	
}


void OBJECT_LANDMARK::Curve_start(int direction)
{
	//4 : exception preceding one was 3 (restart => close curve to next starting point)
	// in that case, this point should be a clean start... 
	// but still, next ones could be tagged as "3" (close curve)
	int next=0;
	if (direction ==4) 
	{		
		this->color[0] = 0;
		this->color[1] = 1;
		this->color[2] = 0;
		this->color[3] = 1;
		this->curve_start = 1;
		this->selected =0;		
	}
	if (this->selected ==1)
	{
		next=1;
		this->selected =0;
		this->curve_start = direction;
		if (direction ==0)
		{
			this->color[0] = 1;
			this->color[1] = 0;
			this->color[2] = 0;
			this->color[3] = 1;
		}
		else if (direction ==1)
		{
			this->color[0] = 0;
			this->color[1] = 1;
			this->color[2] = 0;
			this->color[3] = 1;
		}
		else if (direction ==2)
		{
			this->color[0] = 0;
			this->color[1] = 0;
			this->color[2] = 1;
			this->color[3] = 1;
		}
		else if (direction ==3)
		{
			this->color[0] = 0.6;
			this->color[1] = 0.2;
			this->color[2] = 0.5;
			this->color[3] = 1;
		}
	}

	if (this->nextobj !=NULL)
    {
		if (direction ==3 && next==1)
		{
			this->nextobj->Curve_start(4);
		}
		else if (direction ==4)
		{
			this->nextobj->Curve_start(3);
		}
		else
		{
			this->nextobj->Curve_start(direction);
		}
	}
}
 
void OBJECT_LANDMARK::Landmark_Unselect()
{
	this->selected =0;
	if (this->nextobj !=NULL)
    {this->nextobj->Landmark_Unselect();}

}
void OBJECT_LANDMARK::Landmark_Select()
{
	this->selected =1;
	if (this->nextobj !=NULL)
    {this->nextobj->Landmark_Select();}

}

void OBJECT_LANDMARK::Landmark_init_Mat(float landmark_size)
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


if (this->landmark_index ==-1)
{
	Mat1[0][0]= 1;
	Mat1[1][1]= 1;
	Mat1[2][2]= 1;
}
else 
{
	Mat1[0][0]= landmark_size;
	Mat1[1][1]= landmark_size;
	Mat1[2][2]= landmark_size;
}
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
void OBJECT_LANDMARK::Landmark_init_Mat_Landmark(float landmark_size, int landmark_mode)
{
	if (landmark_mode ==0 || landmark_mode ==2)
	{
		Mat1[0][0]= landmark_size;
		Mat1[1][1]= landmark_size;
		Mat1[2][2]= landmark_size;
	}
	else
	{
		Mat1[0][0]= 1.5*landmark_size;
		Mat1[1][1]= 1.5*landmark_size;
		Mat1[2][2]= 1.5*landmark_size;
	}

}
void OBJECT_LANDMARK::Landmark_DrawObj(int level, int mode)
{

	//std::cout << "Draw landmark " <<this->landmark_index<< std::endl;
	 
	//method : 0 for landmarks
	//method : 1 for free form objects
 //poupoune = 10000;
		float tx,ty,tz;
  
	   //glDisable(GL_COLOR_MATERIAL);

	   glEnable(GL_LIGHTING);
	     glDisable(GL_COLOR_MATERIAL); // Activé : couleur = glcolor
									 // Désactivé : couleur = glmaterial


  VERTEX2 c;
  c.x =0; c.y =0; c.z = 16.9;
  double r = (double)3;
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
	
  
  
		if (level == 0) //Only if basal objects... otherwise they are grouped.
		{
	
			if (this->selected ==1)
			{
				glMaterialfv(GL_FRONT,GL_AMBIENT,bone_ambuse);
				glMaterialfv(GL_FRONT,GL_DIFFUSE,bone_ambuse);
				glMaterialfv(GL_FRONT,GL_SPECULAR,bone_ambuse);
				glMaterialfv(GL_FRONT,GL_SHININESS,bone_ambuse);
				glMaterialfv(GL_FRONT,GL_EMISSION,bone_ambuse);
				glColor4fv((GLfloat*)bone_ambuse);
		
			}
			else
			{
				

				float c[4];

				if (color[0] > 1 || color[1] > 1 || color[2] > 1 || color[3] > 1)
				{
					for (int j = 0; j < 4; j++)
						c[j] = color[j] / 255;
				}
				else {
					for (int j = 0; j < 4; j++)
						c[j] = color[j];
				}
				

				glMaterialfv(GL_FRONT, GL_AMBIENT, c);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
				glMaterialfv(GL_FRONT, GL_SPECULAR, c);
				glMaterialfv(GL_FRONT, GL_SHININESS, bone_shininess);
				glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

				glMaterialfv(GL_BACK, GL_AMBIENT, rose);
				glMaterialfv(GL_BACK, GL_DIFFUSE, rose);
				glMaterialfv(GL_BACK, GL_SPECULAR, bone_specular);
				glMaterialfv(GL_BACK, GL_SHININESS, rose);
				glMaterialfv(GL_BACK, GL_EMISSION, no_mat);

				
			}
		}
		glPushMatrix();
		if (this->selected == 1 && level ==0)
		{
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
			
			
		}//Fin if selected ==1
		
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
		    if (this->is_flag ==0)
			{
				if (this->landmark_index != -1)
				{

					if (g_landmark_type ==0)
					{
						tx = 1;
					}
					else
					{	tx = 0.6;}
					ty = 0;
					tz = 0;
					
					if (mode==1)
					{
						

				/*	glRasterPos3f(tx, ty, tz);
					printString(itoa( this->landmark_index,  10 )); */
					glRasterPos3f(tx, ty, tz);
					printString(itoa(this->landmark_index, 10));
					glRasterPos3f(tx, ty, tz);
					printString(itoa(this->landmark_index, 10));
					// I do not know why, but if we dont call 3 times "glRasterPos and printstring,
					// the first target landmark has a strange colour...
					 
					 
					}
					

					//Landmark_drawSphere2 (c, r, 30);

				}
			}
			else
			{
				

					if (this->Mat1[0][0]>0)
					{
						tx = flag_length/this->Mat1[0][0];					
					}
					else
					{
						tx = flag_length;
					}

					ty = 0;
					tz = 0;																

					//std::string chastd::string(" ");
					printString((char*)std::string(" ").c_str()); 					 
					printString((char*)std::string(" ").c_str()); 					 
					printString((char*)std::string(" ").c_str()); 	
					printString((char*)std::string(" ").c_str()); 	

					glRasterPos3f(tx, ty, tz);
					printString((char*)this->flag_label.c_str()); 					 
					glRasterPos3f(tx, ty, tz);
					printString((char*)this->flag_label.c_str()); 					 
					glRasterPos3f(tx, ty, tz);
					printString((char*)this->flag_label.c_str());
					// I do not know why, but if we dont call 3 times "glRasterPos and printstring,
					// the first flag has no label... 
					//glRasterPos3f(tx, ty, tz);
					//printString((char*)this->flag_label.c_str()); 																		
			
					 double v1[3],v2[3];
					 v1[0]=0;v1[1]=0; v1[2]=0;					 
					 v2[0]=tx;v2[1]=0; v2[2]=0;
					 glBegin(GL_LINES);													
					 glVertex3dv((GLdouble*)&v1); // point1
				     glVertex3dv((GLdouble*)&v2);  // point2													
					 glEnd();

			}


			// COMMENCEMENT DE L'AFFICHAGE PROPREMENT DIT
			if (dispmode != 0)//AFFICHAGE DES TRIANGLES
			{

			  vtkPolyData *what;
			  if (g_landmark_type ==0)
				{
					what = this->Needle;
				}
				else
				{
					what = this->Sphere;
				}
			  glBegin(GL_TRIANGLES);
			  int ve1, ve2, ve3;
			  
			  vtkFloatArray* norms = vtkFloatArray::SafeDownCast(what->GetCellData()->GetNormals());		
			  
			 /* std::cout << "\n norms n of tuples:"<<norms->GetNumberOfTuples()
				  <<"\n norms n of compt:"<<norms->GetNumberOfComponents();*/
			  vtkSmartPointer<vtkIdList> points =vtkSmartPointer<vtkIdList>::New();
			  double v1[3],v2[3],v3[3];
			  double *vn;
			  double vn2[3];

			//std::cout << "\n Needle number of cells :"<<this->Needle->GetNumberOfCells()
			//<<"\n Needle number of points :"<<this->Needle->GetNumberOfPoints();
			  
			  for (int i=0;i<what->GetNumberOfCells();i++) {
				  //std::cout << "\n i:"<<i;
				  //std::cout << "\n i:"<<i;
			// for every triangle 			  
				what->GetCellPoints (i, points);
		
				ve1=(int)points->GetId(0);
				ve2=(int)points->GetId(1);
				ve3=(int)points->GetId(2);						
				what->GetPoint((vtkIdType)ve1, v1);
				what->GetPoint((vtkIdType)ve2, v2);
				what->GetPoint((vtkIdType)ve3, v3);								
				vn=norms->GetTuple((vtkIdType)i);
		
				vn2[0] = vn[0];
				vn2[1] = vn[1];
				vn2[2] = vn[2];
				glNormal3dv(
					(GLdouble*)&vn2);
		
	
				glVertex3dv((GLdouble*)&v3); // point								
				glVertex3dv((GLdouble*)&v2); // point							
				glVertex3dv((GLdouble*)&v1); // point		
			}
      glEnd();
				
			}

			if (dispmode ==0 || dispmode ==3) //WIREFRAME ONLY : 0  WIREFRAME + FLAT TRIANGLES : 3
			{
			  vtkPolyData *what;
			  if (g_landmark_type ==0)
				{
					what = this->Needle;
				}
				else
				{
					what = this->Sphere;
				}
				
				int ve1, ve2, ve3;
			//float coo[3], coo2[3], bari[3];
			vtkFloatArray* norms = vtkFloatArray::SafeDownCast
			(what->GetPointData()->GetNormals());			

			vtkSmartPointer<vtkIdList> points =vtkSmartPointer<vtkIdList>::New();
			double v1[3],v2[3],v3[3];
			double *vn;
			double vn2[3];

			glLineWidth(2);
			for (int i=0;i<what->GetNumberOfCells();i++) {
			// for every triangle 

			what->GetCellPoints (i, points);
			
			ve1=(int)points->GetId(0);
			ve2=(int)points->GetId(1);
			ve3=(int)points->GetId(2);				  			
			what->GetPoint((vtkIdType)ve1, v1);
			what->GetPoint((vtkIdType)ve2, v2);
			what->GetPoint((vtkIdType)ve3, v3);							
			glBegin(GL_LINES);													
			glVertex3dv((GLdouble*)&v1); // point1
			glVertex3dv((GLdouble*)&v2);  // point2
			
			vn=norms->GetTuple((vtkIdType)ve1);
			vn2[0] = -vn[0];
			vn2[1] = -vn[1];
			vn2[2] = -vn[2];

			glNormal3dv((GLdouble*)&vn2); //  normal gouraud shading
			glEnd();

			glBegin(GL_LINES);
			glVertex3dv((GLdouble*)&v2);  // point2
			glVertex3dv((GLdouble*)&v3);  // point3
			vn=norms->GetTuple((vtkIdType)ve2);
			vn2[0] = -vn[0];
			vn2[1] = -vn[1];
			vn2[2] = -vn[2];

			glNormal3dv((GLdouble*)&vn2); //  normal gouraud shading
			glEnd();
			glBegin(GL_LINES);
			glVertex3dv((GLdouble*)&v3);  // point3
			glVertex3dv((GLdouble*)&v1);  // point1
			vn=norms->GetTuple((vtkIdType)ve3);
			vn2[0] = -vn[0];
			vn2[1] = -vn[1];
			vn2[2] = -vn[2];

			glNormal3dv((GLdouble*)&vn2); //  normal gouraud shading
			glEnd();
		}								    
		}
		glPopMatrix();			 
	if (this->nextobj !=NULL)
	{this->nextobj->Landmark_DrawObj(level, mode);}

}
void OBJECT_LANDMARK::Add_Landmark(OBJECT_LANDMARK * New_Obj)
{
	int Ok;
	Ok = 1;
	OBJECT_LANDMARK * parent;
	parent = this;
	while (Ok !=0)
	{
		if (parent->nextobj != NULL)
		{parent = parent->nextobj;}
		else
		{ Ok = 0;}
	}
	parent->nextobj = New_Obj;
	New_Obj->nextobj = NULL;
	New_Obj->prevobj = parent;
}

void OBJECT_LANDMARK::get_screen_projection_matrix( glMatrix screenproj)
{	
	glMatrix cam_mat;
	glMatrix themat;
	getmatrix(themat);
	get_projection_matrix(screenproj);
	get_camera_transform_matrix(cam_mat);
	glPushMatrix();
	glLoadIdentity();
		//glLoadMatrixf((GLfloat*) screenproj);	
	glMultMatrixf((GLfloat*) screenproj);
	glMultMatrixf((GLfloat*) cam_mat);
	glMultMatrixf((GLfloat*)themat); //The mat should look
	//glMultMatrixf((GLfloat*) this->Mat2);
	//	glMultMatrixf((GLfloat*) this->Mat1);
	getmatrix(screenproj);
	glPopMatrix ();
}
void OBJECT_LANDMARK::Hierarchy (int depth)
{
	for (int i = 1;i<=depth; i++)
	{std::cout<<"   ";}
	std::cout<<"Landmark number "<<this->landmark_index<<std::endl;
	
	if (this->nextobj !=NULL)
	{this->nextobj->Hierarchy(depth);}
}
void OBJECT_LANDMARK::get_world_coordinates_matrix2 (glMatrix mat)
{
	// take into account whether the landmark is actually moving!
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
	glPushMatrix();
	glLoadIdentity();
	if (this->selected == 1)
			{	
						
				glTranslated(tx2*uvx[0] + ty2*uvy[0]+ tz2*uvz[0],
				tx2*uvx[1] + ty2*uvy[1]+ tz2*uvz[1],
				tx2*uvx[2] + ty2*uvy[2]+ tz2*uvz[2]);
					
				float newmean[3];
				ApplyTransformation(this->mean, newmean, this->Mat2);
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
			}
			glMultMatrixf((GLfloat *)this->Mat2);
			glMultMatrixf((GLfloat*)this->Mat1);
			getmatrix(mat);
			glPopMatrix();
}

void OBJECT_LANDMARK::get_world_coordinates_matrix(glMatrix mat)
{
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixf((GLfloat*)Mat2);
	glMultMatrixf((GLfloat*)Mat1);
	getmatrix(mat);
	glPopMatrix();
}

int OBJECT_LANDMARK::Landmark_Select(float xmin, float xmax, float ymin, float ymax)
{
	int bchange, bchange2;
	bchange = 0;
	float vv1[3], vv[3];
	glMatrix screen_mat, wc_mat;


	glPushMatrix();
	glMultMatrixf((GLfloat*) this->Mat2);//position 
	glMultMatrixf((GLfloat*) this->Mat1);//aspect
	this->get_screen_projection_matrix(screen_mat);
	this->get_world_coordinates_matrix(wc_mat);
	glPopMatrix();
	vtkPolyData *what;
	if (g_landmark_type ==0)
	{
		what = this->Needle;
	}
	else
	{
		what = this->Sphere;
	}				
	double v1[3];
	
	for (int i=0;i<what->GetNumberOfPoints();i++)	// for each vertex of this
	{	
		what->GetPoint((vtkIdType)i, v1);
		vv1[0]=(float)v1[0];
		vv1[1]=(float)v1[1];
		vv1[2]=(float)v1[2];

		ApplyTransformation(vv1,vv,screen_mat);	// screen proj. of vertex in vv

			//
			if ((vv[2]>-1.0) && vv[2]<1.0 &&	// near and far clipping (14.4.97)
				(vv[0] >=xmin)&& (vv[0] <=xmax)&&
				(vv[1] >=ymin)&& (vv[1] <=ymax)
				)
				{
					bchange = 1;			
					
				}		
		}//for numvert
	
	if (bchange ==1)
	{
		
		if (this->selected ==0){this->selected =1;}
		else {this->selected = 0;}			
	}
	if (this->nextobj !=NULL)
	{
		bchange2 = nextobj->Landmark_Select(xmin, xmax, ymin, ymax);
	}
	if (bchange2 ==1)
	{
		bchange = 1;	
	}
	//
	if (bchange ==1){return 1;}
	else {return 0;}
	//return bchange;
}

void OBJECT_LANDMARK::SetLandmarkSize(float landmark_size, int landmark_mode)
{
			
		Landmark_init_Mat_Landmark(landmark_size, landmark_mode);		
		if (this->nextobj !=NULL)
		{
			this->nextobj->SetLandmarkSize(landmark_size, landmark_mode);
		}
}


void OBJECT_LANDMARK::Landmark_Mirror_ThroughY()
{
			if (this->selected==1)
			{
				std::cout << "Try mirror Mat2 " <<Mat2[3][1]<< std::endl;
				this->Mat2[3][1]*=-1.0;
				std::cout << "After mirror Mat2 " <<Mat2[3][1]<< std::endl;
				this->Mat2[0][1]*=-1.0;
				this->Mat2[1][0]*=-1.0;
				this->Mat2[1][2]*=-1.0;
				this->Mat2[2][1]*=-1.0;   
				this->selected=0;
			}
			if (this->nextobj !=NULL)
			{this->nextobj->Landmark_Mirror_ThroughY();}
			
}

void OBJECT_LANDMARK::Calculate_GlobalMean (float *meanx, float *meany, float *meanz,  int *nb, int only_selected)
{
	// Que faire quand on n'est pas l'objet basal ????
	float tmpx= 0, tmpy= 0,tmpz = 0;int nbverts = 0;
	float tmpx2= 0, tmpy2= 0,tmpz2 = 0;int nbverts2 = 0;
	float vv1[3], vv[3];
	glMatrix  wc_mat;
	this->get_world_coordinates_matrix(wc_mat);
	if (only_selected ==0 || this->selected ==1)
	{
		nbverts= 1;
		
			vv1[0] =  0;
			vv1[1] =  0;
			vv1[2] =  0;
			ApplyTransformation(vv1,vv,wc_mat);
			tmpx+= vv[0];
			tmpy+= vv[1];
			tmpz+= vv[2];
	}
	

	if (this->nextobj !=NULL)
	{this->nextobj->Calculate_GlobalMean (&tmpx2, &tmpy2, &tmpz2, &nbverts2, only_selected);}

	*meanx = tmpx + tmpx2;
	*meany = tmpy + tmpy2;
	*meanz = tmpz + tmpz2;
	*nb = nbverts +nbverts2;

}




//Constructor
OBJECT_LANDMARK::OBJECT_LANDMARK() : OBJECT()
{

	this->Sphere = vtkPolyData::New();
	this->Needle = vtkPolyData::New();
	
	// And Defines points
	this->numtri = 0;
	this->numvert = 0;
	this->numtri2 = 0;
	this->numvert2 = 0;
	this->curve_start=0;
	/*ve=NULL;
	ve2=NULL;
	tr=NULL;
	tr2=NULL;
	trn=NULL;
	trn2=NULL;
	*/
	this->flag_label="Flag";
	this->flag_length=25;
	this->is_flag=0;

	this->landmark_index = -1;
	this->Landmark_init_Mat(1);
	this->mean[0] = 0;this->mean[1] = 0;this->mean[2] = 0;
	this->mean2[0] = 0;this->mean2[1] = 0;this->mean2[2] = 0;
	this->selected = 0;
	this->color[0] = 0;this->color[1] = 1.0;this->color[2] = 0.0; this->color[3] = 0.5;  	
	
	nextobj = NULL;
	prevobj = NULL;
	this->Landmark_init_Mat(g_landmark_size);
	

	
	this->numvert = 10;
	numtri = this->numtri = 16;	
  
	//create a triangle on the three points in the polydata
	//add the triangle to the list of triangles (in this case there is only 1)     

	vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();   	
	vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();

	triangle->GetPointIds()->SetId (0,0);
	triangle->GetPointIds()->SetId (1,2);
	triangle->GetPointIds()->SetId (2,1);
	triangles->InsertNextCell (triangle );

	triangle->GetPointIds()->SetId (0,0);
	triangle->GetPointIds()->SetId (1,3);
	triangle->GetPointIds()->SetId (2,2);
	triangles->InsertNextCell (triangle );
	triangle->GetPointIds()->SetId (0,0);
	triangle->GetPointIds()->SetId (1,4);
	triangle->GetPointIds()->SetId (2,3);
	triangles->InsertNextCell ( triangle );
	triangle->GetPointIds()->SetId (0,0);
	triangle->GetPointIds()->SetId (1,5);
		triangle->GetPointIds()->SetId (2,4);
		 triangles->InsertNextCell ( triangle );

		triangle->GetPointIds()->SetId (0,0);
		triangle->GetPointIds()->SetId (1,6);
		triangle->GetPointIds()->SetId (2,5);
		triangles->InsertNextCell ( triangle );
		
		triangle->GetPointIds()->SetId (0,0);
		triangle->GetPointIds()->SetId (1,7);
		triangle->GetPointIds()->SetId (2,6);
		triangles->InsertNextCell ( triangle );
		
		triangle->GetPointIds()->SetId (0,0);
		triangle->GetPointIds()->SetId (1,8);
		triangle->GetPointIds()->SetId (2,7);
		triangles->InsertNextCell(triangle );

		triangle->GetPointIds()->SetId (0,0);
		triangle->GetPointIds()->SetId (1,1);
		triangle->GetPointIds()->SetId (2,8);
		triangles->InsertNextCell ( triangle );

		triangle->GetPointIds()->SetId (0,9);
		triangle->GetPointIds()->SetId (1,1);
		triangle->GetPointIds()->SetId (2,2);
		 triangles->InsertNextCell ( triangle );

		triangle->GetPointIds()->SetId (0,9);
		triangle->GetPointIds()->SetId (1,2);
		triangle->GetPointIds()->SetId (2,3);
		triangles->InsertNextCell ( triangle );

		triangle->GetPointIds()->SetId (0, 9);
		triangle->GetPointIds()->SetId (1,3);
		triangle->GetPointIds()->SetId (2,4);
		triangles->InsertNextCell ( triangle );

		triangle->GetPointIds()->SetId (0,9);
		triangle->GetPointIds()->SetId (1,4);
		triangle->GetPointIds()->SetId (2,5);
		triangles->InsertNextCell ( triangle );

		triangle->GetPointIds()->SetId (0,9);
		triangle->GetPointIds()->SetId (1,5);
		triangle->GetPointIds()->SetId (2,6);
		triangles->InsertNextCell ( triangle );

		triangle->GetPointIds()->SetId (0,9);
		triangle->GetPointIds()->SetId (1,6);
		triangle->GetPointIds()->SetId (2,7);
		triangles->InsertNextCell ( triangle );

		triangle->GetPointIds()->SetId (0,9);
		triangle->GetPointIds()->SetId (1,7);
		triangle->GetPointIds()->SetId (2,8);
		triangles->InsertNextCell ( triangle );

		triangle->GetPointIds()->SetId (0,9);
		triangle->GetPointIds()->SetId (1,8);
		triangle->GetPointIds()->SetId (2,1);
		triangles->InsertNextCell ( triangle );		
		

		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		points->InsertNextPoint (0.000000,0.000000, 0.000000);
		points->InsertNextPoint (16.0/20, 1.00000/20,0.000000);
		points->InsertNextPoint (16.0/20, 0.63255/20,  0.63255/20);
		points->InsertNextPoint (16.0/20, 0.0000,1.000000/20);
		points->InsertNextPoint (16.0/20, -0.63255/20,0.63255/20);
		points->InsertNextPoint (16.0/20, -1.0/20,0);
		points->InsertNextPoint (16.0/20, -0.63255/20,-0.63255/20);
		points->InsertNextPoint (16.0/20,  0.00,-1.000000/20);
		points->InsertNextPoint (16.0/20,  0.63255/20,-0.63255/20);
		points->InsertNextPoint (20.000000/20,0.000000,0.000000);
		
		vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
		polydata->SetPoints ( points );
		polydata->SetPolys ( triangles );

		vtkSmartPointer<vtkPolyDataNormals> ObjNormals =  vtkSmartPointer<vtkPolyDataNormals>::New();		
		ObjNormals->SetInputData(polydata);												
		ObjNormals->ComputePointNormalsOn();
		ObjNormals->ComputeCellNormalsOn();
		//ObjNormals->AutoOrientNormalsOff();
		ObjNormals->ConsistencyOff();
		ObjNormals->Update();
	
		vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
		cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());
		cleanPolyDataFilter->PieceInvariantOff();
		cleanPolyDataFilter->ConvertLinesToPointsOff();
		cleanPolyDataFilter->ConvertPolysToLinesOff();
		cleanPolyDataFilter->ConvertStripsToPolysOff();
		cleanPolyDataFilter->PointMergingOn();
		cleanPolyDataFilter->Update();

		this->Needle->DeepCopy(cleanPolyDataFilter->GetOutput()); // otherwise object is cleared!
	

		/*std::cout << "\n Needle points :"<<this->Needle->GetNumberOfPoints()
	  <<"\n n Needle cells:"<<this->Needle->GetNumberOfCells();*/

	vtkFloatArray* norms = vtkFloatArray::SafeDownCast(this->Needle->GetCellData()->GetNormals());		

  /*std::cout << "\n norms n of tuples:"<<norms->GetNumberOfTuples()
				  <<"\n norms n of compt:"<<norms->GetNumberOfComponents();*/
 vtkSmartPointer<vtkSphereSource> sphereSource = 
    vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->SetCenter(0.0, 0.0, 0.0);
  sphereSource->SetRadius(0.5);
  sphereSource->SetPhiResolution(30);
  sphereSource->SetThetaResolution(30);
  sphereSource->Update();
  ObjNormals->SetInputData(sphereSource->GetOutput());												
  ObjNormals->Update();	
  cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());	
  cleanPolyDataFilter->Update();
  Sphere->DeepCopy(cleanPolyDataFilter->GetOutput());
  this->numvert2 = Sphere->GetNumberOfPoints();
  numtri2 = this->numtri2 = Sphere->GetNumberOfCells();
	
  /*std::cout << "\n Sphere points :"<<this->numvert2
	  <<"\n n Sphere cells:"<<this->numtri2;*/
	


	mean[0]=0;
	mean[1]=0;
	mean[2]=0;
	/*for (int i=0;i<3*this->numvert2;i+=3)
	{
		mean[0]+=this->ve2[i];
		mean[1]+=this->ve2[i+1];
		mean[2]+=this->ve2[i+2];
	}

		mean[0]/=numvert2;
		mean[1]/=numvert2;
		mean[2]/=numvert2;
	*/
	this->selected = 0;
	
}

OBJECT_LANDMARK::~OBJECT_LANDMARK()//Destructor
{

	if (Sphere !=NULL){Sphere->Delete();}
	if (Needle !=NULL){Needle->Delete();}
  
}
