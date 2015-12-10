
#include <math.h>
#include <vtkMatrix4x4.h>
#include <vtkMatrix3x3.h>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Double_Window.H>
#include <vtkSmartPointer.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkExtractEdges.h>
#include <vtkIdTypeArray.h>
#include <vtkMassProperties.h>
//#include <vtkPolyData.h>
#include <vtkIdList.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkDataArray.h>
#include <vtkCleanPolyData.h>
#include <set>
#include <vtkMath.h>
//#define GLEW_STATIC
#include <GL/glew.h>
#include <FL/gl.h>
#include <FL/glut.H>
#include "Object_Mesh.h"

#ifdef WIN32
#include <Mmsystem.h >// à enlever car specifique à windows
#endif
//#pragma comment (lib, "Ws2_32.lib")

 void OBJECT_MESH::init()
{
	 
  

	
}

 



void OBJECT_MESH::Mesh_init_Mat()
{
	int val = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
				val = 1;
			else
				val = 0;
			Mat1[i][j] = val;
			Mat2[i][j] = val;
		}

	mean[0] = 0;
	mean[1] = 0;
	mean[2] = 0;


}

int OBJECT_MESH::mround(float mnum)
{
	return (int)(mnum + 0.5);
}
void OBJECT_MESH::Update_RGB()
{
	//std::cout<<"Update RGB of "<<this->name<<std::endl;
				vtkSmartPointer<vtkUnsignedCharArray> newcolors =
				vtkSmartPointer<vtkUnsignedCharArray>::New();
				newcolors->SetNumberOfComponents(4);				
				newcolors->SetNumberOfTuples(this->GetNumberOfPoints());
				vtkFloatArray *currentScalars;
				currentScalars =(vtkFloatArray*)this->GetPointData()->GetScalars();  // couleur des courbure
				GLfloat cv[4];
				// now we create and populate RGB and Tags
				//std::cout<<"Disp_Scalars_Mode= "<<Disp_Scalars_Mode<<std::endl;
				//std::cout<<"Disp_Tags_Mode= "<<Disp_Tags_Mode<<std::endl;
				if (currentScalars!=NULL && (Disp_Scalars_Mode==1 ||Disp_Tags_Mode==1))
				{
				//	std::cout<<"currentScalars not null "<<std::endl;
				}
				else
				{
				//	std::cout<<"currentScalars null "<<std::endl;
				}
				for (int i=0;i<this->GetNumberOfPoints();i++)	// for each vertex 
				{	
					//@@@@@
					int nr,ng,nb,na;
					
					if (currentScalars!=NULL && (Disp_Scalars_Mode==1 ||Disp_Tags_Mode==1))
					{
						//std::cout<<"currentScalars not null "<<std::endl;
						double cur_t = currentScalars->GetTuple(i)[0];
						
						ConvertScalarToColor((float)cur_t, cv, 1);
						nr= (unsigned char)(mround(255*cv[0]));
						ng= (unsigned char)(mround(255*cv[1]));
						nb= (unsigned char)(mround(255*cv[2]));
						na= (unsigned char)(mround(255*cv[3]));
					}
					else
					{
						//std::cout<<"currentScalars null "<<std::endl;
						nr= (unsigned char)(mround(255*this->color[0]));
						ng= (unsigned char)(mround(255*this->color[1]));
						nb= (unsigned char)(mround(255*this->color[2]));
						na= (unsigned char)(mround(255*this->blend));
						

					}
					if (i<500)
					{
						//std::cout<<"nr="<<nr<<", ng="<<ng<<", nb="<<nb<<std::endl;
					}
					////newcolors->InsertTuple3(i, (unsigned char)mround(255*cv[0]),(unsigned char)mround(255*cv[1]),(unsigned char)mround(255*cv[2]));																				
					//newcolors->InsertTuple3(i, 0,0,255);			
					if(i<10)
					{
					//		std::cout<<"nr="<<nr<<", ng="<<ng<<", nb="<<nb<<", na="<<na<<std::endl;
					}
					newcolors->InsertTuple4(i, nr,ng,nb,na);		
					newcolors->InsertTuple4(i, nr,ng,nb,na);																				
				}
				newcolors->SetName("RGB");
				//std::cout<<"New colors added to My Obj! file!"<<std::endl;
				
				this->GetPointData()->RemoveArray("RGB");		
			   	this->GetPointData()->AddArray(newcolors);

}

void OBJECT_MESH::Mesh_Find_Closest_Vertex (float input[3], float *x, float*y, float *z, float *nx,float *ny,float *nz, vtkIdType *ve, int mode)
{
	// Fin closest vertex of My_Vertex in world coordinates.
	
	//mode 1 : input is a world coordinate point (then use WC_MAT)
	//mode 2 : input is a mouse x, mouse y, mouse z point (then use Screen_mat) 
	// in mode 2 we use a minimal angle of 70 degrees to avoid a selection of bad points.
	// This corresponds to a minimal value of cosinus:  min_cos=0.342

	float closest[3];	
	float nclosest[3];	
	float closests[3];
	closest[0] = 0;
	closest[1] = 0;
	closest[2] = 0;
	closests[0] = 0;
	closests[1] = 0;
	closests[2] = 0;
	nclosest[0] = 0;
	nclosest[1] = 0;
	nclosest[2] = 0;
	int iclosest =0;
	float min_cos=0.342;
	float uv[3];
    
	uv[0]=cos(3.1415926535897932384626*camera.az/180.0)*cos(3.1415926535897932384626*camera.el/180.0);
	uv[1]=sin(3.1415926535897932384626*camera.az/180.0)*cos(3.1415926535897932384626*camera.el/180.0);
	uv[2]=sin(3.1415926535897932384626*camera.el/180.0);
	float min_dist;
	float dist;
	min_dist = 1000000000;
	float vv1[3], vv[3],vvs[3];
	float vvn1[3], vvn[3];
	glMatrix wc_mat;
	glMatrix screen_mat;
		glPushMatrix();
		glLoadIdentity();
		glMultMatrixf((GLfloat*) this->Mat2);
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
	this->get_world_coordinates_matrix(wc_mat);
	this->get_screen_projection_matrix(screen_mat);
	glPopMatrix();
	double vvd[3];
	double *vvnd;
	vtkSmartPointer<vtkFloatArray> norms = 
	vtkSmartPointer<vtkFloatArray>::New();

	norms = vtkFloatArray::SafeDownCast(this->GetPointData()->GetNormals()); // recupere les normales
			
	
			
	for (int i=0;i<this->numvert;i++)	// for each vertex of this
	{							

		this->GetPoint(i, vvd);  /// recupère le point 
		
		vv1[0] =  (float) vvd[0];
		vv1[1] =  (float) vvd[1];
		vv1[2] =  (float) vvd[2];
		
		vvnd= norms->GetTuple((vtkIdType)i);

		vvn1[0] =  (float) vvnd[0];
		vvn1[1] =  (float) vvnd[1];
		vvn1[2] =  (float) vvnd[2];	
		
		
			ApplyTransformation(vv1, vv,wc_mat);//projection of vv1 into world coordinate
		
			ApplyTransformation(vv1,vvs,screen_mat);	// projection of vv1 on the screen
		

			ApplyRotation(vvn1, vvn,wc_mat);	//rotation of the normals..
		
		//sqdist=dist_squared(vv,mouse_vertex);
		if (mode ==1)
		{
			dist = (vv[0] - input[0])*(vv[0] - input[0]) + (vv[1] - input[1])*(vv[1] - input[1]) + (vv[2] - input[2])*(vv[2] - input[2]);
		}
		else if (mode ==2 && vvs[2]>-1.0 && vvs[2]<1.0 && (vvn[0]*uv[0]+vvn[1]*uv[1]+vvn[2]*uv[2])>min_cos)
		{
			dist = (vvs[0] - input[0])*(vvs[0] - input[0]) + (vvs[1] - input[1])*(vvs[1] - input[1]) + 0.001*(vvs[2] - input[2])*(vvs[2] - input[2]);
			
	
		}
		else
		{
			dist =1000000;
		}
		if (dist<min_dist)
		{
			
			closest[0]=vv[0];
			closest[1]=vv[1];
			closest[2]=vv[2];
			iclosest = i;
			
			nclosest[0]=vvn[0];
			nclosest[1]=vvn[1];
			nclosest[2]=vvn[2];
			closests[0]= vvs[0];
			closests[1]= vvs[1];
			closests[2]= vvs[2];
			float d = sqrt (nclosest[0]*nclosest[0]+ nclosest[1]*nclosest[1]+ nclosest[2]*nclosest[2]);
			if (d != 0)
			  {nclosest[0]/=d;nclosest[1]/=d;nclosest[2]/=d;}
			min_dist = dist;
		}
							
	}//for numvert		
	

	*x=closest[0];
	*y=closest[1];
	*z=closest[2];
	*nx=nclosest[0];
	*ny=nclosest[1];
	*nz=nclosest[2];
	*ve = iclosest;
}






void OBJECT_MESH::SC_calc_depth()
{
	if (this->selected ==1)
	{
		
		
	  vtkSmartPointer<vtkFloatArray> newScalars = 
      vtkSmartPointer<vtkFloatArray>::New();

	  newScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
	  newScalars->SetNumberOfTuples(this->numvert);

		newScalars->SetNumberOfTuples(this->numvert);

		glMatrix MMat;
		float vv[3]; float vv1[3]; 
		int i;
   
		glPushMatrix();		
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
		getmatrix(MMat);
		glPopMatrix();
	
		//Put in scalars z of each vertex
		double vvd[3];
		for (i=0;i<this->numvert;i++)	// for each vertex of this
		
		{		


				this->GetPoint(i, vvd);
				
				vv1[0] =  (float) vvd[0];
				vv1[1] =  (float) vvd[1];
				vv1[2] =  (float) vvd[2];	
				ApplyTransformation(vv1,vv,MMat);	// cam + (group) + world coordinate projection				
				//this->scalars_vertices[i] = vv[2]; // depth
				newScalars->InsertTuple1(i, vv[2]);
				
				
		}
		newScalars->SetName("Depth");
		// test if exists...
		//
		int exists =0;
		/*exists = this->Scalar_Exists(
		if (exists ==0)
		{
			this->GetPointData()->AddArray(newScalars);
		}
		else
		{*/
			// remove this scalar
			//this->GetPointData()->SetScalars(newScalars);
			this->GetPointData()->RemoveArray("Depth");
			this->GetPointData()->AddArray(newScalars);
			this->GetPointData()->SetActiveScalars("Depth");
			g_active_scalar =0; 
			// 0 => depth
			// 1 =>	"Maximum_Curvature"
			// 2 => "Minimum_Curvature"
			// 3 => "Gauss_Curvature"
			// 4 => "Mean_Curvature"
		/*}*/

		this->selected =0;
	}
	if (this->nextobj != NULL)
	{nextobj->SC_calc_depth();}
}

void OBJECT_MESH::Set_Active_Scalar()
{			
	std::string sc_name = this->Get_Active_Scalar_Name();  
	this->GetPointData()->SetActiveScalars(sc_name.c_str());   
   if (this->nextobj != NULL)
	{nextobj->Set_Active_Scalar();}
}

float OBJECT_MESH::scalars_get_min()
{
	//int i;
	float my_min;
	float my_minnext;
	my_min = 1000000000;
	my_minnext = 1000000000;
	
	vtkFloatArray *currentScalars;
	//currentScalars = (vtkFloatArray*)this->GetPointData()->GetScalars("Depth");			
	std::string ScName;
	ScName = this->Get_Active_Scalar_Name();
	currentScalars = (vtkFloatArray*)this->GetPointData()->GetScalars(ScName.c_str());			
	//	(float)currentScalars->GetTuple(ve1)[0], cv[0], this->blend);

	if (currentScalars!=NULL)
		{			
			my_min = currentScalars->GetTuple(0)[0];
			float val;
			scalar_q *tmp_sc=(scalar_q*)malloc((int)(numvert)*sizeof(scalar_q));	
			for (int i =0;i<this->numvert; i++)
				{
						val = currentScalars->GetTuple(i)[0];
						
						tmp_sc[i].n = i;
						tmp_sc[i].v = val;

				}
				// sort list trm_tri		
				qsort(tmp_sc,numvert,sizeof(scalar_q),scalar_compare);			
				int iq = (int)(0.05*numvert);
				my_min= tmp_sc[iq].v;	
				free(tmp_sc);
				tmp_sc = NULL;
			
			
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
float OBJECT_MESH::scalars_get_max()
{
	
	float my_max;
	float my_maxnext;
	my_max = -1000000000;
	my_maxnext = -1000000000;

	vtkFloatArray *currentScalars;
	//currentScalars = (vtkFloatArray*)this->GetPointData()->GetScalars("Depth");	
	std::string ScName;
	ScName = this->Get_Active_Scalar_Name();
	currentScalars = (vtkFloatArray*)this->GetPointData()->GetScalars(ScName.c_str());			
	//	(float)currentScalars->GetTuple(ve1)[0], cv[0], this->blend);
			
	//if (scalars_vertices!=NULL)
	if (currentScalars!=NULL)
		{			
			my_max = currentScalars->GetTuple(0)[0];
			float val;
			scalar_q *tmp_sc=(scalar_q*)malloc((int)(numvert)*sizeof(scalar_q));	
			for (int i =0;i<this->numvert; i++)
				{
						val = currentScalars->GetTuple(i)[0];
						
						tmp_sc[i].n = i;
						tmp_sc[i].v = val;

				}
				// sort list trm_tri		
				qsort(tmp_sc,numvert,sizeof(scalar_q),scalar_compare);			
				int iQ = (int)(0.95*numvert);
				my_max= tmp_sc[iQ].v;	
				free(tmp_sc);
				tmp_sc = NULL;
			
		}
	//std::cout<<this->name.c_str()<<":My_max="<<my_max<<std::endl;
	if (this->nextobj !=NULL)
	{
		my_maxnext = this->nextobj->scalars_get_max();
		if (my_maxnext !=NULL && my_max !=NULL)
		{	
			if (my_max < my_maxnext){my_max = my_maxnext;}
			else if (my_max == NULL && my_maxnext !=NULL){my_max = my_maxnext;}
		}	
	}	
	//std::cout<<this->name.c_str()<<":My_max final ="<<my_max<<std::endl;	
	return my_max;
}

void OBJECT_MESH::Partition_Data_Delete()
{
	// delete pt_cubes and pt_vertices_per_cube...
	if (this->pt_cubes !=NULL )
	{
		for (vtkIdType ci=0;ci<this->pt_total_cube_nr;ci++)
		{
			if (this->pt_vertices_per_cube !=NULL && this->pt_vertices_per_cube[ci]>0)
			{
				delete pt_cubes[ci];
			}
			this->pt_cubes[ci]=NULL;		
		}
		delete pt_cubes;
	}	
	
	this->pt_cubes=NULL;
	if (this->pt_vertices_per_cube !=NULL)
	{
		delete this->pt_vertices_per_cube;
	}
	this->pt_vertices_per_cube=NULL;

	// delete pt_cubes_compare and pt_vertices_per_cube_compare...
	if (this->pt_cubes_compare !=NULL )
	{
		for (vtkIdType ci=0;ci<this->pt_total_cube_nr;ci++)
		{
			if (this->pt_vertices_per_cube_compare !=NULL && this->pt_vertices_per_cube_compare[ci]>0)
			{
				delete pt_cubes_compare[ci];
			}
			this->pt_cubes_compare[ci]=NULL;			
		}
		delete pt_cubes_compare;
	}
	
	this->pt_cubes_compare=NULL;
	if (this->pt_vertices_per_cube_compare !=NULL)
	{
		delete this->pt_vertices_per_cube_compare;
	}
	this->pt_vertices_per_cube_compare=NULL;

	this->pt_total_cube_nr=0;
	this->pt_cube_nr[0]=0;
	this->pt_cube_nr[1]=0;
	this->pt_cube_nr[2]=0;

}

//void OBJECT_MESH::Partition_Data (float xmin, float ymin, float zmin, float xmax, float ymax, float zmax, float pt_dist)
//{
/*
	glMatrix  wc_mat;
	vtkIdType ve;			
	double vvd[3];
	float vv1[3];
	float vv[3];
	vtkIdType ci;
	this->get_world_coordinates_matrix(wc_mat);
	
	// initialize pt_cubes and pt_vertices_per_cube...
	if (this->pt_cubes !=NULL )
	{
		for (ci=0;ci<this->pt_total_cube_nr;ci++)
		{
			if (this->pt_vertices_per_cube !=NULL && this->pt_vertices_per_cube[ci]>0)
			{
				delete pt_cubes[ci];
			}
			this->pt_cubes[ci]=NULL;
		
		}
		delete pt_cubes;
	}
	
	this->pt_cubes=NULL;
	if (this->pt_vertices_per_cube !=NULL)
	{
		delete this->pt_vertices_per_cube;
	}
	this->pt_vertices_per_cube=NULL;
			
	// we have min and max.
	// we will search in how many cubes we divides the dataset
	// numbers of cubes are:
	this->pt_done=0;
	this->pt_cube_nr[0]= (vtkIdType)((xmax-xmin)/pt_dist) + 1;
	this->pt_cube_nr[1]=(vtkIdType) ((ymax-ymin)/pt_dist) + 1;
	this->pt_cube_nr[2]=(vtkIdType) ((zmax-zmin)/pt_dist) + 1;


	// determine total number of cubes!:
	this->pt_total_cube_nr=this->pt_cube_nr[0]*
						   this->pt_cube_nr[1]*
						   this->pt_cube_nr[2];

	// we will create a  "raw" image of cube data							
	this->pt_vertices_per_cube = new vtkIdType [this->pt_total_cube_nr];

	//  we want to know how many vertices there are in each cube;
	// and we will produce a raw image of the cubes 
	// containing at each pixel the number of vertices contained
	// in this actual cube

	// we will consider that the raw list is structured like this (for a 3*3 cube for which int x,y,z are the indices of the cubes): 
	//  pt_vertices_per_cube [0] : x=0 y=0 z=0
	//  pt_vertices_per_cube [1] : x=1 y=0 z=0
	//  pt_vertices_per_cube [2] : x=2 y=0 z=0
	//  pt_vertices_per_cube [3] : x=0 y=1 z=0
	//  pt_vertices_per_cube [4] : x=1 y=1 z=0
	//  pt_vertices_per_cube [5] : x=2 y=1 z=0
	//  pt_vertices_per_cube [6] : x=0 y=2 z=0
	//  pt_vertices_per_cube [7] : x=1 y=2 z=0
	//  pt_vertices_per_cube [8] : x=2 y=2 z=0
	//  pt_vertices_per_cube [9] : x=0 y=0 z=1
	//  pt_vertices_per_cube [10] : x=1 y=0 z=1
	//  pt_vertices_per_cube [11] : x=2 y=0 z=1
	//  pt_vertices_per_cube [12] : x=0 y=1 z=1
	//  pt_vertices_per_cube [13] : x=1 y=1 z=1
	//  pt_vertices_per_cube [14] : x=2 y=1 z=1
	//  pt_vertices_per_cube [15] : x=0 y=2 z=1
	//  pt_vertices_per_cube [16] : x=1 y=2 z=1
	//  pt_vertices_per_cube [17] : x=2 y=2 z=1
	//  pt_vertices_per_cube [18] : x=0 y=0 z=2
	//  pt_vertices_per_cube [19] : x=1 y=0 z=2
	//  pt_vertices_per_cube [20] : x=2 y=0 z=2
	//  pt_vertices_per_cube [21] : x=0 y=1 z=2
	//  pt_vertices_per_cube [22] : x=1 y=1 z=2
	//  pt_vertices_per_cube [23] : x=2 y=1 z=2
	//  pt_vertices_per_cube [24] : x=0 y=2 z=2
	//  pt_vertices_per_cube [25] : x=1 y=2 z=2
	//  pt_vertices_per_cube [26] : x=2 y=2 z=2

	// generalization : 
	// pt_vertices_per_cube [x+ cube_nr_x*(y + cube_nr_y*z)] : x, y, z



	for (vtkIdType i=0;i<this->pt_total_cube_nr;i++){this->pt_vertices_per_cube[i]=0;}

	vtkIdType ve_cubecoords[3];
	// first we want to know how many vertices there are in each cube;
	for (ve=0;ve<this->numvert;ve++)
	{
		
		this->GetPoint(ve, vvd);		
	

		vv1[0] =  (float) vvd[0];
		vv1[1] =  (float) vvd[1];
		vv1[2] =  (float) vvd[2];
		ApplyTransformation(vv1,vv,wc_mat);
		// in which cube does this vertex falls ?

		ve_cubecoords[0]=floor((vv[0]-xmin)/pt_dist);  
		ve_cubecoords[1]=floor((vv[1]-ymin)/pt_dist);
		ve_cubecoords[2]=floor((vv[2]-zmin)/pt_dist);
		// we have a vertex of cube coords ve_cubecoords
		// in the raw dataset, this translates as :
		//  x+ cube_nr_x*(y + cube_nr_y*z)
		// ve_cubecoords[0] + cube_nr[0]*(ve_cubecoords[1]+ cube_nr[1]*ve_cubecoords[2]) 

		//we therefore have : 
		this->pt_vertices_per_cube[
			ve_cubecoords[0] 
			+ this->pt_cube_nr[0]*
					(ve_cubecoords[1]+ 
					this->pt_cube_nr[1]*ve_cubecoords[2]
					)
		]++;
	}

	// we define a table wich contains the cube data partition.
	// this->pt_cubes[cube i][indice i]= vertex index in original object array.
	

	this->pt_cubes = new vtkIdType  *[this->pt_total_cube_nr];
	for ( ci=0;ci<this->pt_total_cube_nr;ci++){this->pt_cubes[ci]=NULL;}	

	for ( ci=0;ci<this->pt_total_cube_nr;ci++)
	{
		if (this->pt_vertices_per_cube[ci]!=0)
		{
			this->pt_cubes[ci]=new vtkIdType [this->pt_vertices_per_cube[ci]];
		}
	}
		
	// put vertices ids into corresponding cubes:
	// num_ver_already_in_cube: will be filled little by little as we put vertices.
	vtkIdType  * num_ver_already_in_cube = new vtkIdType[this->pt_total_cube_nr];
	for ( ci=0;ci<this->pt_total_cube_nr;ci++){num_ver_already_in_cube[ci]=0;}

	vtkIdType cube_ind;
	for (ve=0;ve<this->numvert;ve++)
	{
		// same trick : convert xyz vertex coordinates into 
		// x y z cube coords, and then into the index in the raw list
		this->GetPoint(ve, vvd);			
		vv1[0] =  (float) vvd[0];
		vv1[1] =  (float) vvd[1];
		vv1[2] =  (float) vvd[2];
		ApplyTransformation(vv1,vv,wc_mat);
		// in which cube does this vertex falls ?

		ve_cubecoords[0]=floor((vv[0]-xmin)/pt_dist);  
		ve_cubecoords[1]=floor((vv[1]-ymin)/pt_dist);
		ve_cubecoords[2]=floor((vv[2]-zmin)/pt_dist);
		
		// we have a vertex of cube coords ve_cubecoords
		// in the raw dataset, this translates as :
		//  x+ cube_nr_x*(y + cube_nr_y*z)
		// ve_cubecoords[0] + cube_nr[0]*(ve_cubecoords[1]+ cube_nr[1]*ve_cubecoords[2]) 

		cube_ind=ve_cubecoords[0] + this->pt_cube_nr[0]*(ve_cubecoords[1]+ this->pt_cube_nr[1]*ve_cubecoords[2]);
		this->pt_cubes[cube_ind][num_ver_already_in_cube[cube_ind]]=ve;
		num_ver_already_in_cube[cube_ind]++;
	}
	delete num_ver_already_in_cube;

	// Now Initialise the lists of vertices to compare (27 adjacent cubes.
	// initialize pt_cubes_compare and pt_vertices_per_cube_compare...
	if (this->pt_cubes_compare !=NULL )
	{
		for ( ci=0;ci<this->pt_total_cube_nr;ci++)
		{
			if (this->pt_vertices_per_cube_compare !=NULL && this->pt_vertices_per_cube_compare[ci]>0)
			{
				delete pt_cubes_compare[ci];
			}
			this->pt_cubes_compare[ci]=NULL;			
		}
		delete pt_cubes_compare;
	}
	
	this->pt_cubes_compare=NULL;
	if (this->pt_vertices_per_cube_compare !=NULL)
	{
		delete this->pt_vertices_per_cube_compare;
	}
	this->pt_vertices_per_cube_compare=NULL;
	
	// we will fill pt_vertices_per_cube_compare and  pt_cubes_compare
	this->pt_vertices_per_cube_compare = new vtkIdType [this->pt_total_cube_nr];
	for (vtkIdType i=0;i<this->pt_total_cube_nr;i++){this->pt_vertices_per_cube_compare[i]=0;}

	//initialize pt_cubes_compare
	this->pt_cubes_compare = new vtkIdType  *[this->pt_total_cube_nr];
	for (ci=0;ci<this->pt_total_cube_nr;ci++){this->pt_cubes_compare[ci]=NULL;}	


	vtkIdType czi, cyi, cxi;
	vtkIdType cczi, ccyi, ccxi;

	for (czi=0;czi<this->pt_cube_nr[2];czi++)
	{
		for (cyi=0;cyi<this->pt_cube_nr[1];cyi++)
		{			
			for (cxi=0;cxi<this->pt_cube_nr[0];cxi++)
			{	
				// so for all cubes :
				// in the raw dataset, this translates as :
				vtkIdType actual_cube_index= cxi+ this->pt_cube_nr[0]*(cyi + this->pt_cube_nr[1]*czi);
				if (actual_cube_index>30 && actual_cube_index<255)
				{
					std::cout<<"actual_cube_index="<<actual_cube_index<<std::endl;
					std::cout<<"czi="<<czi;
					std::cout<<"cyi="<<cyi;
					std::cout<<"cxi="<<cxi<<std::endl;
					ccxi= (actual_cube_index)%this->pt_cube_nr[0];

					if (this->pt_cube_nr[0]>0)
					{
						ccyi= ((actual_cube_index-ccxi)/this->pt_cube_nr[0])%this->pt_cube_nr[1];
					}
					if (this->pt_cube_nr[1]>0)
					{
						cczi= 
							(
								(
									(
										(actual_cube_index-ccxi)/this->pt_cube_nr[0]
									)
									-ccyi
								)
								/this->pt_cube_nr[1]						
							)%this->pt_cube_nr[2];
					}

					//cczi= (actual_cube_index-ccxi-ccyi)%this->pt_cube_nr[2];
					std::cout<<"cczi="<<cczi;
					
					std::cout<<"ccyi="<<ccyi;
					
					std::cout<<"ccxi="<<ccxi<<std::endl;
				}
				//There we are in a cube!!!
				// we have to find all the vertices in this cube
				// then we have to compare with all the vertices of this cube + those of at maximum the 26 adjacent cubes
				vtkIdType cubes_to_compare[27];	// there are at maximum 26 neighbours and this block to investigate.
				vtkIdType cx,cy,cz;
				for (ci=0;ci<27;ci++)
				{
					cubes_to_compare[ci]=0;
				}

				int cubes_to_compare_nr=0;

				// Again some preparation : prepare the list of cubes we will investigate
				for (cz=(czi-1);cz<=(czi+1);cz++)
				{
					if (cz>=0 && cz<this->pt_cube_nr[2])
					{
						for (cy=(cyi-1);cy<=(cyi+1);cy++)
						{
							if (cy>=0 && cy<this->pt_cube_nr[1])
							{	
								for (cx=(cxi-1);cx<=(cxi+1);cx++)
								{
									if (cx>=0 && cx<this->pt_cube_nr[0])
									{
										//again this tranlation
										// 
										vtkIdType cubeind=cx+this->pt_cube_nr[0]*(cy + this->pt_cube_nr[1]*cz);
										cubes_to_compare[cubes_to_compare_nr]=cubeind;
										cubes_to_compare_nr++;
						
									}
								}
							}
						}

					}
				}
				// here we now for this given cube we know which cubes we have to investigate.
				for (ci=0;ci<cubes_to_compare_nr;ci++)
				{
					this->pt_vertices_per_cube_compare[actual_cube_index]+=
						this->pt_vertices_per_cube[cubes_to_compare[ci]];					
				}

				
								
				// now that we know the number of vertices to compare for actual_cube_index_, instantiate
				// the array!
				if (this->pt_vertices_per_cube_compare[actual_cube_index]!=0)
				{
					this->pt_cubes_compare[actual_cube_index]=
						new vtkIdType [this->pt_vertices_per_cube_compare[actual_cube_index]];
				}

				//Now fill pt_cubes_compare
				vtkIdType ce_cpt=0;
				for (ci=0;ci<cubes_to_compare_nr;ci++)
				{
					// if there are vertices in that adjacent cube 
					if (this->pt_vertices_per_cube[cubes_to_compare[ci]]>0)
					{
						for (int ve=0; ve<this->pt_vertices_per_cube[cubes_to_compare[ci]];ve++)
						{
							this->pt_cubes_compare[actual_cube_index][ce_cpt]=
							this->pt_cubes[cubes_to_compare[ci]][ve];
							ce_cpt++;
						}
					
					}
								
				}
				if (ce_cpt != this->pt_vertices_per_cube_compare[actual_cube_index])
				{std::cout<<"Pb in number of vertices to compare in cube index:"<<actual_cube_index<<std::endl;}
				
			}//for cxi
		}//for cyi
	}//for czi
	*/
	/*for (ci=0;ci<3;ci++)
	{
		std::cout<<"pt_vertices_per_cube["<<ci<<"]="<<pt_vertices_per_cube[ci]<<std::endl;
		for  (ve=0;ve<pt_vertices_per_cube[ci];ve++)
		{
			std::cout<<ve<<":"<<pt_cubes[ci][ve]<<"|";
		}
		std::cout<<std::endl;
		std::cout<<"pt_vertices_per_cube_compare["<<ci<<"]="<<pt_vertices_per_cube_compare[ci]<<std::endl;
		for  (ve=0;ve<pt_vertices_per_cube_compare[ci];ve++)
		{
			std::cout<<ve<<":"<<pt_cubes_compare[ci][ve]<<"|";
		}
		std::cout<<std::endl;
	
	}*/
	


//}

void OBJECT_MESH::Partition_Data (float xmin, float ymin, float zmin, float xmax, float ymax, float zmax, float pt_dist)
{

	glMatrix  wc_mat;
	vtkIdType ve;			
	double vvd[3];
	float vv1[3];
	float vv[3];
	vtkIdType ci;
	this->get_world_coordinates_matrix(wc_mat);
	
	// initialize pt_cubes and pt_vertices_per_cube...
	if (this->pt_cubes !=NULL )
	{
		for (ci=0;ci<this->pt_total_cube_nr;ci++)
		{
			if (this->pt_vertices_per_cube !=NULL && this->pt_vertices_per_cube[ci]>0)
			{
				delete pt_cubes[ci];
			}
			this->pt_cubes[ci]=NULL;
		
		}
		delete pt_cubes;
	}
	
	this->pt_cubes=NULL;
	if (this->pt_vertices_per_cube !=NULL)
	{
		delete this->pt_vertices_per_cube;
	}
	this->pt_vertices_per_cube=NULL;
			
	// we have min and max.
	// we will search in how many cubes we divides the dataset
	// numbers of cubes are:
	this->pt_done=0;
	this->pt_cube_nr[0]= (vtkIdType)((xmax-xmin)/pt_dist) + 1;
	this->pt_cube_nr[1]=(vtkIdType) ((ymax-ymin)/pt_dist) + 1;
	this->pt_cube_nr[2]=(vtkIdType) ((zmax-zmin)/pt_dist) + 1;


	// determine total number of cubes!:
	this->pt_total_cube_nr=this->pt_cube_nr[0]*
						   this->pt_cube_nr[1]*
						   this->pt_cube_nr[2];

	// we will create a  "raw" image of cube data							
	this->pt_vertices_per_cube = new vtkIdType [this->pt_total_cube_nr];

	//  we want to know how many vertices there are in each cube;
	// and we will produce a raw image of the cubes 
	// containing at each pixel the number of vertices contained
	// in this actual cube

	// we will consider that the raw list is structured like this (for a 3*3 cube for which int x,y,z are the indices of the cubes): 
	//  pt_vertices_per_cube [0] : x=0 y=0 z=0
	//  pt_vertices_per_cube [1] : x=1 y=0 z=0
	//  pt_vertices_per_cube [2] : x=2 y=0 z=0
	//  pt_vertices_per_cube [3] : x=0 y=1 z=0
	//  pt_vertices_per_cube [4] : x=1 y=1 z=0
	//  pt_vertices_per_cube [5] : x=2 y=1 z=0
	//  pt_vertices_per_cube [6] : x=0 y=2 z=0
	//  pt_vertices_per_cube [7] : x=1 y=2 z=0
	//  pt_vertices_per_cube [8] : x=2 y=2 z=0
	//  pt_vertices_per_cube [9] : x=0 y=0 z=1
	//  pt_vertices_per_cube [10] : x=1 y=0 z=1
	//  pt_vertices_per_cube [11] : x=2 y=0 z=1
	//  pt_vertices_per_cube [12] : x=0 y=1 z=1
	//  pt_vertices_per_cube [13] : x=1 y=1 z=1
	//  pt_vertices_per_cube [14] : x=2 y=1 z=1
	//  pt_vertices_per_cube [15] : x=0 y=2 z=1
	//  pt_vertices_per_cube [16] : x=1 y=2 z=1
	//  pt_vertices_per_cube [17] : x=2 y=2 z=1
	//  pt_vertices_per_cube [18] : x=0 y=0 z=2
	//  pt_vertices_per_cube [19] : x=1 y=0 z=2
	//  pt_vertices_per_cube [20] : x=2 y=0 z=2
	//  pt_vertices_per_cube [21] : x=0 y=1 z=2
	//  pt_vertices_per_cube [22] : x=1 y=1 z=2
	//  pt_vertices_per_cube [23] : x=2 y=1 z=2
	//  pt_vertices_per_cube [24] : x=0 y=2 z=2
	//  pt_vertices_per_cube [25] : x=1 y=2 z=2
	//  pt_vertices_per_cube [26] : x=2 y=2 z=2

	// generalization : 
	// pt_vertices_per_cube [x+ cube_nr_x*(y + cube_nr_y*z)] : x, y, z



	for (vtkIdType i=0;i<this->pt_total_cube_nr;i++){this->pt_vertices_per_cube[i]=0;}

	vtkIdType ve_cubecoords[3];
	// first we want to know how many vertices there are in each cube;
	for (ve=0;ve<this->numvert;ve++)
	{
		
		this->GetPoint(ve, vvd);		
	

		vv1[0] =  (float) vvd[0];
		vv1[1] =  (float) vvd[1];
		vv1[2] =  (float) vvd[2];
		ApplyTransformation(vv1,vv,wc_mat);
		// in which cube does this vertex falls ?

		ve_cubecoords[0]=floor((vv[0]-xmin)/pt_dist);  
		ve_cubecoords[1]=floor((vv[1]-ymin)/pt_dist);
		ve_cubecoords[2]=floor((vv[2]-zmin)/pt_dist);
		// we have a vertex of cube coords ve_cubecoords
		// in the raw dataset, this translates as :
		//  x+ cube_nr_x*(y + cube_nr_y*z)
		// ve_cubecoords[0] + cube_nr[0]*(ve_cubecoords[1]+ cube_nr[1]*ve_cubecoords[2]) 

		//we therefore have : 
		this->pt_vertices_per_cube[
			ve_cubecoords[0] 
			+ this->pt_cube_nr[0]*
					(ve_cubecoords[1]+ 
					this->pt_cube_nr[1]*ve_cubecoords[2]
					)
		]++;
	}

	// we define a table wich contains the cube data partition.
	// this->pt_cubes[cube i][indice i]= vertex index in original object array.
	

	this->pt_cubes = new vtkIdType  *[this->pt_total_cube_nr];
	for ( ci=0;ci<this->pt_total_cube_nr;ci++){this->pt_cubes[ci]=NULL;}	

	for ( ci=0;ci<this->pt_total_cube_nr;ci++)
	{
		if (this->pt_vertices_per_cube[ci]!=0)
		{
			this->pt_cubes[ci]=new vtkIdType [this->pt_vertices_per_cube[ci]];
		}
	}
		
	// put vertices ids into corresponding cubes:
	// num_ver_already_in_cube: will be filled little by little as we put vertices.
	vtkIdType  * num_ver_already_in_cube = new vtkIdType[this->pt_total_cube_nr];
	for ( ci=0;ci<this->pt_total_cube_nr;ci++){num_ver_already_in_cube[ci]=0;}

	vtkIdType cube_ind;
	for (ve=0;ve<this->numvert;ve++)
	{
		// same trick : convert xyz vertex coordinates into 
		// x y z cube coords, and then into the index in the raw list
		this->GetPoint(ve, vvd);			
		vv1[0] =  (float) vvd[0];
		vv1[1] =  (float) vvd[1];
		vv1[2] =  (float) vvd[2];
		ApplyTransformation(vv1,vv,wc_mat);
		// in which cube does this vertex falls ?

		ve_cubecoords[0]=floor((vv[0]-xmin)/pt_dist);  
		ve_cubecoords[1]=floor((vv[1]-ymin)/pt_dist);
		ve_cubecoords[2]=floor((vv[2]-zmin)/pt_dist);
		
		// we have a vertex of cube coords ve_cubecoords
		// in the raw dataset, this translates as :
		//  x+ cube_nr_x*(y + cube_nr_y*z)
		// ve_cubecoords[0] + cube_nr[0]*(ve_cubecoords[1]+ cube_nr[1]*ve_cubecoords[2]) 

		cube_ind=ve_cubecoords[0] + this->pt_cube_nr[0]*(ve_cubecoords[1]+ this->pt_cube_nr[1]*ve_cubecoords[2]);
		this->pt_cubes[cube_ind][num_ver_already_in_cube[cube_ind]]=ve;
		num_ver_already_in_cube[cube_ind]++;
	}
	delete num_ver_already_in_cube;

	// Now Initialise the lists of vertices to compare (27 adjacent cubes.
	// initialize pt_cubes_compare and pt_vertices_per_cube_compare...
	if (this->pt_cubes_compare !=NULL )
	{
		for ( ci=0;ci<this->pt_total_cube_nr;ci++)
		{
			if (this->pt_vertices_per_cube_compare !=NULL && this->pt_vertices_per_cube_compare[ci]>0)
			{
				delete pt_cubes_compare[ci];
			}
			this->pt_cubes_compare[ci]=NULL;			
		}
		delete pt_cubes_compare;
	}
	
	this->pt_cubes_compare=NULL;
	if (this->pt_vertices_per_cube_compare !=NULL)
	{
		delete this->pt_vertices_per_cube_compare;
	}
	this->pt_vertices_per_cube_compare=NULL;
	
	// we will fill pt_vertices_per_cube_compare and  pt_cubes_compare
	this->pt_vertices_per_cube_compare = new vtkIdType [this->pt_total_cube_nr];
	for (vtkIdType i=0;i<this->pt_total_cube_nr;i++){this->pt_vertices_per_cube_compare[i]=0;}

	//initialize pt_cubes_compare
	this->pt_cubes_compare = new vtkIdType  *[this->pt_total_cube_nr];
	for (ci=0;ci<this->pt_total_cube_nr;ci++){this->pt_cubes_compare[ci]=NULL;}	


	vtkIdType czi, cyi, cxi;
	vtkIdType C;
	for (C=0; C<this->pt_total_cube_nr;C++)
	{
		cxi= (C)%this->pt_cube_nr[0];		
		cyi= ((C-cxi)/this->pt_cube_nr[0])%this->pt_cube_nr[1];
		czi= ((((C-cxi)/this->pt_cube_nr[0])-cyi)/this->pt_cube_nr[1])%this->pt_cube_nr[2];

	
		// so for all cubes :
		// in the raw dataset, this translates as :
		
		//There we are in a cube!!!
		// we have to find all the vertices in this cube
		// then we have to compare with all the vertices of this cube + those of at maximum the 26 adjacent cubes
		vtkIdType cubes_to_compare[27];	// there are at maximum 26 neighbours and this block to investigate.
		vtkIdType cx,cy,cz;
		for (ci=0;ci<27;ci++)
		{
			cubes_to_compare[ci]=0;
		}

		int cubes_to_compare_nr=0;

		// Again some preparation : prepare the list of cubes we will investigate

		for (cz=(czi-1);cz<=(czi+1);cz++)
		{
			if (cz>=0 && cz<this->pt_cube_nr[2])
			{
				for (cy=(cyi-1);cy<=(cyi+1);cy++)
				{
					if (cy>=0 && cy<this->pt_cube_nr[1])
					{	
						for (cx=(cxi-1);cx<=(cxi+1);cx++)
						{
							if (cx>=0 && cx<this->pt_cube_nr[0])
							{
								//again this tranlation
								// 
								vtkIdType cubeind=cx+this->pt_cube_nr[0]*(cy + this->pt_cube_nr[1]*cz);
								cubes_to_compare[cubes_to_compare_nr]=cubeind;
								cubes_to_compare_nr++;
				
							}
						}
					}
				}

			}
		}
		// here we now for this given cube we know which cubes we have to investigate.
		for (ci=0;ci<cubes_to_compare_nr;ci++)
		{
			this->pt_vertices_per_cube_compare[C]+=
				this->pt_vertices_per_cube[cubes_to_compare[ci]];					
		}

		
						
		// now that we know the number of vertices to compare for C_, instantiate
		// the array!
		if (this->pt_vertices_per_cube_compare[C]!=0)
		{
			this->pt_cubes_compare[C]=
				new vtkIdType [this->pt_vertices_per_cube_compare[C]];
		}

		//Now fill pt_cubes_compare
		vtkIdType ce_cpt=0;
		for (ci=0;ci<cubes_to_compare_nr;ci++)
		{
			// if there are vertices in that adjacent cube 
			if (this->pt_vertices_per_cube[cubes_to_compare[ci]]>0)
			{
				for (int ve=0; ve<this->pt_vertices_per_cube[cubes_to_compare[ci]];ve++)
				{
					this->pt_cubes_compare[C][ce_cpt]=
					this->pt_cubes[cubes_to_compare[ci]][ve];
					ce_cpt++;
				}
			
			}
						
		}
		if (ce_cpt != this->pt_vertices_per_cube_compare[C])
		{std::cout<<"Pb in number of vertices to compare in cube index:"<<C<<std::endl;}
			
	}//forC
	
	/*for (ci=0;ci<3;ci++)
	{
		std::cout<<"pt_vertices_per_cube["<<ci<<"]="<<pt_vertices_per_cube[ci]<<std::endl;
		for  (ve=0;ve<pt_vertices_per_cube[ci];ve++)
		{
			std::cout<<ve<<":"<<pt_cubes[ci][ve]<<"|";
		}
		std::cout<<std::endl;
		std::cout<<"pt_vertices_per_cube_compare["<<ci<<"]="<<pt_vertices_per_cube_compare[ci]<<std::endl;
		for  (ve=0;ve<pt_vertices_per_cube_compare[ci];ve++)
		{
			std::cout<<ve<<":"<<pt_cubes_compare[ci][ve]<<"|";
		}
		std::cout<<std::endl;
	
	}*/
	


}
void OBJECT_MESH::Partition_Data(float pt_dist)
{
			// We first compute the upper and lower limits of the object in x, y and z directions
			//lower and upper limits of the object

			float myobj_min[3];
			float myobj_max[3];
			glMatrix  wc_mat;
			vtkIdType ve;			
			double vvd[3];
			float vv1[3];
			float vv[3];
		
			this->get_world_coordinates_matrix(wc_mat);
	
			myobj_min[0]=10000000000;
			myobj_min[1]=10000000000;
			myobj_min[2]=10000000000;
			
			myobj_max[0]=-10000000000;
			myobj_max[1]=-10000000000;
			myobj_max[2]=-10000000000;



			for ( ve=0;ve<this->numvert;ve++)
			{

				this->GetPoint(ve, vvd);		

				vv1[0] =  (float) vvd[0];
				vv1[1] =  (float) vvd[1];
				vv1[2] =  (float) vvd[2];
				ApplyTransformation(vv1,vv,wc_mat);
		
				if (vv[0]<myobj_min[0])
				{myobj_min[0]=vv[0];}

				if (vv[1]<myobj_min[1]) 
				{myobj_min[1]=vv[1];}

				if (vv[2]<myobj_min[2]) 
				{myobj_min[2]=vv[2];}

				if (vv[0]>myobj_max[0])
				{myobj_max[0]=vv[0];}

				if (vv[1]>myobj_max[1])
				{myobj_max[1]=vv[1];}

				if (vv[2]>myobj_max[2])
				{myobj_max[2]=vv[2];}
			}
			this->Partition_Data(myobj_min[0],myobj_min[1],myobj_min[2],
				myobj_max[0],myobj_max[1],myobj_max[2], pt_dist);

}

void OBJECT_MESH::SC_calc_thickness(float thickness_max_distance, Fl_Double_Window* thicknessWindow)
{
	// This function will find, for each vertex, the closest vertex in the direction opposite to the normal 
	// of the given vertex.
	// if no close vertex is found matching the criteria, the value "Max_Thickness" is given
	// a minimal cos is required.
	
	
	if (this->selected ==1)
	{												
			vtkSmartPointer<vtkFloatArray> newScalars = 
			vtkSmartPointer<vtkFloatArray>::New();
			newScalars->SetNumberOfComponents(1); 
			newScalars->SetNumberOfTuples(this->numvert);			
			vtkIdType ve;			
			double vvd[3];
			double *vvnd;
			float vvn1[3];
			float vv1[3];
			float vvn2[3];
			float vv2[3];
			float AB[3]; // vector between points 1 and 2
			float ABnorm[3];// normalized vector between points 1 and 2.

			float projected_vv2[3];  // projected point along -normal of point 1 towards point 2.
			float AC[3]; // vector between points 1 and projection of point 2
			 
				std::cout<<"Maximal  thickness limit: "<< thickness_max_distance<<std::endl;

			float curr_dist; // absolute distance
							
			vtkSmartPointer<vtkFloatArray> norms = 
			vtkSmartPointer<vtkFloatArray>::New();
			norms = vtkFloatArray::SafeDownCast(this->GetPointData()->GetNormals());																					

			// First, we need to partion vertices data : the algorithm would takes ages
			// to compute if we have to compare each vertex to all vertices! ( n² is too long)

			// Strategy : we divide the vertices into a number of cubes with a side size of thickness_max_distance mm
			// Then we will only compute the distance between a vertex belonging to 1 cube to all the vertices of the given cube
			// and also to the vertices of all adjacent cubes (9*3 - 1 = 26 other cubes). This way, we will manage to compare this
			// vertex to all possible vertices <= thickness limit in distance (+ some other a little bit further). 

			
		//std::cout<<"Try to partition Data"<<std::endl;
		this->Partition_Data (thickness_max_distance);
		//std::cout<<"Data partition done"<<std::endl;
			

		// now we will loop into the vertices of each cube...

		char cpercent[10];
		Fl::check();                               // give fltk some cpu to gray out button			
		Fl_Double_Window *w = thicknessWindow;           // access parent window				
		w->begin();    
		Fl_Progress *progress = new Fl_Progress(20,50,200,30);
		progress->minimum(0);                      // set progress range to be 0.0 ~ 1.0
		progress->maximum(1);
		progress->color(0x88888800);               // background color
		progress->selection_color(0x4444ff00);     // progress bar color
		progress->labelcolor(FL_WHITE);            // percent text color				
		w->end();				
		progress->show();
		w->redraw();
		Fl::check();  

		vtkIdType ci;
		
		for (ci=0;ci<this->pt_total_cube_nr; ci++)
		{
			
			int percent = (int)((100*ci)/this->pt_total_cube_nr);
			
			itoa( percent, cpercent, 10);
			 strcat (cpercent,"%");
			 progress->value((float)percent/100.0); 					  
			 progress->label(cpercent);  
			 Fl::check();   
			std::cout<<"progression:"<<percent<<"%\r";
			//std::cout<<"cube"<<actual_cube_raw_index<<" out of "<<total_cube_nr<<"\r";

			
	
			// end of preparation.
			// we are still in cxi, cyi, czi
			// and only here do we start to compare all the vertices of this cube!
			for (vtkIdType i=0;i<this->pt_vertices_per_cube[ci];i++)
			{

				ve = this->pt_cubes[ci][i];
				this->GetPoint(ve, vvd);			
			
				vv1[0] =  (float) vvd[0];
				vv1[1] =  (float) vvd[1];
				vv1[2] =  (float) vvd[2];
				
				vvnd= norms->GetTuple(ve);

				vvn1[0] =  (float) -vvnd[0];
				vvn1[1] =  (float) -vvnd[1];
				vvn1[2] =  (float) -vvnd[2];	
		
				
				float newscalar = 0;
				float min_dist = thickness_max_distance;
				float tmp_dist =0;
				
				float min_cos=0.342; // 70 degrees => Don't want to compute thickness using badly oriented vertices.
				float cur_cos = 1.0; // compare ve1's normal and ve2's normal
				float cur_cos2 = 1.0; //compare  ve1's normal and vector between ve1 and ve2
				//for(vtkIdType ve2 = 0; ve2 < this->numvert; ve2++)
				//{
				
				for (vtkIdType j=0;j<this->pt_vertices_per_cube_compare[ci];j++)
				{	
					vtkIdType ve2 = this->pt_cubes_compare[ci][j];
					if (ve2 != ve)
					{
						this->GetPoint(ve2, vvd);						
						vv2[0] =  (float) vvd[0];
						vv2[1] =  (float) vvd[1];
						vv2[2] =  (float) vvd[2];
						
						vvnd= norms->GetTuple(ve2);
						vvn2[0] =  (float) vvnd[0];
						vvn2[1] =  (float) vvnd[1];
						vvn2[2] =  (float) vvnd[2];	

						AB[0]=vv2[0]-vv1[0];
						AB[1]=vv2[1]-vv1[1];
						AB[2]=vv2[2]-vv1[2];
								
						// distance between points 1 and 2
						curr_dist=(float)sqrt(AB[0]*AB[0]+AB[1]*AB[1]+AB[2]*AB[2]);
						ABnorm[0]=0;ABnorm[1]=0; ABnorm[2]=0;
						if (curr_dist>0)
						{
							ABnorm[0]= AB[0]/curr_dist;
							ABnorm[1]= AB[1]/curr_dist;
							ABnorm[2]= AB[2]/curr_dist;
						}


						if (curr_dist <thickness_max_distance)
						{
							// calculate projected point along the normal of point 1 
							// towards point 2
							// seach if vv1n et vv2n sont suffisamment dans la bonne direction.
							cur_cos = vvn1[0]*vvn2[0]+vvn1[1]*vvn2[1]+vvn1[2]*vvn2[2];
							cur_cos2 = ABnorm[0]*vvn1[0]+ABnorm[1]*vvn1[1]+ABnorm[2]*vvn1[2];
							if (cur_cos> min_cos && cur_cos2>min_cos)
							{
								// we have a candidate!
								// compute projected point 2 along vvn1 !
								projected_vv2[0]  =vv1[0] + vvn1[0]*AB[0];
								projected_vv2[1]  =vv1[1] + vvn1[1]*AB[1];
								projected_vv2[2]  =vv1[2] + vvn1[2]*AB[2];
								AC[0] = projected_vv2[0]  -vv1[0];
								AC[1] = projected_vv2[1]  -vv1[1];
								AC[2] = projected_vv2[2]  -vv1[2];
								//tmp_dist = (float)sqrt(AC[0]*AC[0]+AC[1]*AC[1]+AC[2]*AC[2]);
								// well I am not sure I want this distance!!!
								tmp_dist = curr_dist;

							}
							else
							{
								tmp_dist = thickness_max_distance;
							}
						}
						else
						{
							tmp_dist = thickness_max_distance;
						}
						if (tmp_dist< min_dist){min_dist = tmp_dist;}												
					}// if ve2 !=ve
				}// for all vertices to compare														
				  
				 // if (ve<10){std::cout<<"New Scalar value at "<<ve<<"="<<newscalar<<std::endl;}				 
				if (ve<100)
				{
				//	std::cout<<"Point "<<ve<<"  thickness:"<< min_dist<<std::endl;

				}
				newScalars->InsertTuple1(ve, min_dist);
			}// for all the vertices in actual cube										
		}// for all cubes
		w->remove(progress);     
		delete(progress); 
		// ve_cubecoords[0] + cube_nr[0]*(ve_cubecoords[1]+ cube_nr[1]*ve_cubecoords[2]) 			
		//std::cout<<"New Scalar computation done "<<std::endl;
		newScalars->SetName("Thickness");
		this->GetPointData()->RemoveArray("Thickness");			
		//std::cout<<"Add array "<<std::endl;
		this->GetPointData()->AddArray(newScalars);
		//std::cout<<"Set Active scalar"<<std::endl;
		this->GetPointData()->SetActiveScalars("Thickness");							 				   
		this->selected = 0;

		this->Partition_Data_Delete ();
	}//selected
	if (this->nextobj != NULL)
	{nextobj->SC_calc_thickness(thickness_max_distance, thicknessWindow);}
	
}

void OBJECT_MESH::SC_calc_mean()
{
	// This function will "smooth" the scalars.
	// Reset active scalar => 
	// For a given vertex: find all neighbouring vertices
	// Set scalar value as the mean of current vertex scalar value and scalar values of all neighouring vertices.
	
	
	if (this->selected ==1)
	{
		
		vtkFloatArray *currentScalars;
		currentScalars = (vtkFloatArray*)this->GetPointData()->GetScalars();
		
		//std::cout<<"Scnam="<<scname;

		if (currentScalars!=NULL)
		{
			std::string scname= this->GetPointData()->GetScalars()->GetName();
			std::string sTags ("Tags");
			std::size_t found = scname.find(sTags);
			
				if (found == std::string::npos)
				{						
					vtkSmartPointer<vtkExtractEdges> extractEdges =
					vtkSmartPointer<vtkExtractEdges>::New();
					extractEdges->SetInputData((vtkPolyData*)this);
					extractEdges->Update();
				 
					vtkSmartPointer<vtkPolyData> mesh = extractEdges->GetOutput();

					 vtkSmartPointer<vtkFloatArray> newScalars = 
					vtkSmartPointer<vtkFloatArray>::New();

					newScalars->SetNumberOfComponents(1); 
					newScalars->SetNumberOfTuples(this->numvert);
					
					vtkIdType ve;
					
				
					double vn[3];
					vn[0]=0;
					vn[1]=0;
					vn[2]=1;
					double picked_value =0;
					for (ve=0;ve<this->numvert;ve++)
					{
						// get all vertices connected to this point (neighbouring vertices).
						//if (ve<10){std::cout<<"Try to find connected vertices at : "<<ve<<std::endl;}
						vtkSmartPointer<vtkIdList> connectedVertices= vtkSmartPointer<vtkIdList>::New();
						connectedVertices = GetConnectedVertices(mesh, vn, picked_value, ve,  -1); //	int tool_mode=-1; //no pencil! no magic wand!
						
						vtkSmartPointer<vtkIdTypeArray> ids =
						vtkSmartPointer<vtkIdTypeArray>::New();
						ids->SetNumberOfComponents(1);
						//if (ve<10){std::cout << "Connected vertices: ";}
						for(vtkIdType j = 0; j < connectedVertices->GetNumberOfIds(); j++)
						{
							//if (ve<10){std::cout << connectedVertices->GetId(j) << " ";}
							ids->InsertNextValue(connectedVertices->GetId(j));
						}
						float newscalar = 0;
						int n_vertices =connectedVertices->GetNumberOfIds();
						
						newscalar = 0;
						
						//on ajoute une fois la valeur actuelle.

						// get all scalars 				
						//if (ve<10){std::cout<<std::endl;}

						for (vtkIdType j=0;j<ids->GetNumberOfTuples();j++)
						{	// for all neighbouring vertices							
							vtkIdType at = ids->GetTuple(j)[0];
							//if (ve<10){std::cout<<"old scalar value at "<<at<<"=";}
							float curr_scalar = (float)(currentScalars->GetTuple(at))[0];
							//std::cout<<curr_scalar<<std::endl;
  							newscalar+=curr_scalar;
							
						}
						if(n_vertices>0)
						{ newscalar/=n_vertices;}
						  
						 // if (ve<10){std::cout<<"New Scalar value at "<<ve<<"="<<newscalar<<std::endl;}				 
						  newScalars->InsertTuple1(ve, newscalar);
						  

					}
					//std::cout<<"New Scalar computation done "<<std::endl;
					std::string sc_name = this->Get_Active_Scalar_Name(); 
					newScalars->SetName(sc_name.c_str());
					this->GetPointData()->RemoveArray(sc_name.c_str());			
					//std::cout<<"Add array "<<std::endl;
					this->GetPointData()->AddArray(newScalars);
					//std::cout<<"Set Active scalar"<<std::endl;
					this->GetPointData()->SetActiveScalars(sc_name.c_str());			
				}// not scalar "Tags
				else
				{
					std::cout<<"Cannot smooth Tags"<<std::endl;
				}
		}//scalars not null
		
	 		
		   
		this->selected = 0;
	}//selected
	if (this->nextobj != NULL)
	{nextobj->SC_calc_mean();}
	
}


vtkSmartPointer<vtkIdList> OBJECT_MESH::GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, double *vn, 
															 double sc, vtkIdType id, int tool_mode)
{  
	vtkSmartPointer<vtkIdList> connectedVertices =
    vtkSmartPointer<vtkIdList>::New();  
	connectedVertices->InsertNextId(id);
	vtkSmartPointer<vtkIdList> cellIdList =
    vtkSmartPointer<vtkIdList>::New();
	mesh->GetPointCells(id, cellIdList);
    
  for(vtkIdType i = 0; i < cellIdList->GetNumberOfIds(); i++)
    {
    vtkSmartPointer<vtkIdList> pointIdList =
      vtkSmartPointer<vtkIdList>::New();
    mesh->GetCellPoints(cellIdList->GetId(i), pointIdList);

	vtkIdType newid; 

    if(pointIdList->GetId(0) != id)
      {
		newid = pointIdList->GetId(0);
      }
    else
      {
	  newid = pointIdList->GetId(1);
      
      }
	  if(tool_mode ==-1) //  no tool (-1) 
	  {
		connectedVertices->InsertNextId(newid);
	  }
	  
	  else if (tool_mode ==2 || (tool_mode ==1 && g_magic_wand_extension==180)|| (tool_mode==0)) // flood bucket or magic wand with maximal extension or pencil
	  {
		// get tag value of current id
		  vtkFloatArray *currentTags;		
		  currentTags = (vtkFloatArray*)this->GetPointData()->GetScalars("Tags");
		  if (currentTags == NULL)
		  {connectedVertices->InsertNextId(newid);}
		  else
		  {
			double currentTag =currentTags->GetTuple(newid)[0];
			//std::cout<<"Current tag ="<<currentTag<<std::endl;
			//std::cout<<"sc ="<<sc<<std::endl;

			if (currentTag == sc || g_magic_wand_over ==1 || currentTag == 0.0) // Tag 0 is special! Overriden...
			{
				connectedVertices->InsertNextId(newid);
			}
		  }
	  }
	  else // magic wand : consider both current tag value and normal angle limit extension
	  {
	  
		vtkFloatArray *currentTags;		
		  currentTags = (vtkFloatArray*)this->GetPointData()->GetScalars("Tags");
		  int oktag =0;
		  if (g_magic_wand_over ==1)
		  {oktag=1;}
		  if (currentTags == NULL)
		  {oktag=1;}
		  else
		  {
			double currentTag =currentTags->GetTuple(newid)[0];
			if (currentTag == sc || currentTag == 0.0) // Tag 0 is special! Overriden...
			{
				oktag=1;
			}
		  }
		  if (oktag==1)
		  {
			// now check normal
			double *vn2; 
			float vvn1[3];
			float vvn[3];
		
			
			vtkFloatArray* norms = vtkFloatArray::SafeDownCast
			(this->GetPointData()->GetNormals());	
			if (norms !=NULL)
			{
				vn2=norms->GetTuple(newid);
				vvn1[0] =  (float) vn2[0];
				vvn1[1] =  (float) vn2[1];
				vvn1[2] =  (float) vn2[2];							
				glMatrix wc_mat;
				glMatrix screen_mat;
				glPushMatrix();
				glLoadIdentity();
				glMultMatrixf((GLfloat*) this->Mat2);
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
				this->get_world_coordinates_matrix(wc_mat);
				this->get_screen_projection_matrix(screen_mat);
				glPopMatrix();

				ApplyRotation(vvn1, vvn,wc_mat);	//rotation of the normals..
				

				double curr_cos = vn[0]*(double)vvn[0]+vn[1]*(double)vvn[1]+vn[2]*(double)vvn[2];
				if (curr_cos>g_magic_wand_extension_min_cos)
				{
					connectedVertices->InsertNextId(newid);
				}
				
			}
			else
			{
				std::cout<<"Norm NULL..."<<std::endl;
			}
		  
		  }
	  }

    } 
  return connectedVertices;
}


vtkSmartPointer<vtkIdList> OBJECT_MESH::GetConnectedVerticesLasso(vtkSmartPointer<vtkPolyData> mesh, double *vn, 
			double sc, vtkIdType id, int tool_mode, int lasso_x, int lasso_y,int h_screen, int w_screen)
{  
	glMatrix screen_mat, wc_mat;		
	POLYGON_VERTEX proj_screen;
	int proj_is_inside;
	int direction; //if 1, tag vertices inside polygon, else tag vertices outside...
	float vv[3]; 
	float vv1[3];
	double vvd[3];
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixf((GLfloat*) this->Mat2);
	glTranslated(
		this->mean[0],
		this->mean[1],
		this->mean[2]
		);	
	glMultMatrixf((GLfloat*) this->Mat1);
	glTranslated(
		-this->mean[0],
		-this->mean[1],
		-this->mean[2]
	);	
	this->get_screen_projection_matrix(screen_mat);
	this->get_world_coordinates_matrix(wc_mat);
	glPopMatrix();	


	vtkSmartPointer<vtkIdList> connectedVertices =
    vtkSmartPointer<vtkIdList>::New();  
	connectedVertices->InsertNextId(id);
	vtkSmartPointer<vtkIdList> cellIdList =
    vtkSmartPointer<vtkIdList>::New();
	mesh->GetPointCells(id, cellIdList);
    
  for(vtkIdType i = 0; i < cellIdList->GetNumberOfIds(); i++)
    {
    vtkSmartPointer<vtkIdList> pointIdList =
      vtkSmartPointer<vtkIdList>::New();
    mesh->GetCellPoints(cellIdList->GetId(i), pointIdList);

	vtkIdType newid; 

    if(pointIdList->GetId(0) != id)
    {
		newid = pointIdList->GetId(0);
    }
    else
    {
		newid = pointIdList->GetId(1);      
    }
    int new_id_inside =0;
	// Here code to test if new_id is inside lasso polygone!	 	
		
	proj_screen.x = lasso_x;
	proj_screen.y = lasso_y;
	direction = poly.POLYGON_POINT_INSIDE(proj_screen);											
	this->GetPoint(newid, vvd);
	vv1[0] =  (float) vvd[0];
	vv1[1] =  (float) vvd[1];
	vv1[2] =  (float) vvd[2];

	ApplyTransformation(vv1,vv,screen_mat);						
	proj_screen.x = (int) w_screen*(vv[0] +1 )/2;
	proj_screen.y = (int) h_screen*(vv[1] +1 )/2;
	proj_is_inside = poly.POLYGON_POINT_INSIDE(proj_screen);			
	if (direction ==0)
	{
		if (proj_is_inside ==0){proj_is_inside =1;}
		else
		{proj_is_inside =0;}
	}
	if ((vv[2]>-1.0) && vv[2]<1.0 && (proj_is_inside ==1))
	{
		new_id_inside =1;
	}

	// end code to test whether inside!
	if (new_id_inside ==1)
	 {
		   if (tool_mode ==2 || (tool_mode ==1 && g_magic_wand_extension==180)|| (tool_mode==0)) // flood bucket or magic wand with maximal extension or pencil
		  {
			// get tag value of current id
			  vtkFloatArray *currentTags;		
			  currentTags = (vtkFloatArray*)this->GetPointData()->GetScalars("Tags");
			  if (currentTags == NULL)
			  {connectedVertices->InsertNextId(newid);}
			  else
			  {
				double currentTag =currentTags->GetTuple(newid)[0];
				//std::cout<<"Current tag ="<<currentTag<<std::endl;
				//std::cout<<"sc ="<<sc<<std::endl;

				if (currentTag == sc || g_magic_wand_over ==1 || currentTag == 0.0) // Tag 0 is special! Overriden...
				{
					connectedVertices->InsertNextId(newid);
				}
			  }
		  }
		  else // magic wand : consider both current tag value and normal angle limit extension
		  {
		  
			vtkFloatArray *currentTags;		
			  currentTags = (vtkFloatArray*)this->GetPointData()->GetScalars("Tags");
			  int oktag =0;
			  if (g_magic_wand_over ==1)
			  {oktag=1;}
			  if (currentTags == NULL)
			  {oktag=1;}
			  else
			  {
				double currentTag =currentTags->GetTuple(newid)[0];
				if (currentTag == sc || currentTag == 0.0) // Tag 0 is special! Overriden...
				{
					oktag=1;
				}
			  }
			  if (oktag==1)
			  {
				// now check normal
				double *vn2; 
				float vvn1[3];
				float vvn[3];
			
				
				vtkFloatArray* norms = vtkFloatArray::SafeDownCast
				(this->GetPointData()->GetNormals());	
				if (norms !=NULL)
				{
					vn2=norms->GetTuple(newid);
					vvn1[0] =  (float) vn2[0];
					vvn1[1] =  (float) vn2[1];
					vvn1[2] =  (float) vn2[2];							
					glMatrix wc_mat;
					glMatrix screen_mat;
					glPushMatrix();
					glLoadIdentity();
					glMultMatrixf((GLfloat*) this->Mat2);
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
					this->get_world_coordinates_matrix(wc_mat);
					this->get_screen_projection_matrix(screen_mat);
					glPopMatrix();

					ApplyRotation(vvn1, vvn,wc_mat);	//rotation of the normals..
					

					double curr_cos = vn[0]*(double)vvn[0]+vn[1]*(double)vvn[1]+vn[2]*(double)vvn[2];
					if (curr_cos>g_magic_wand_extension_min_cos)
					{
						connectedVertices->InsertNextId(newid);
					}
					
				}//Norm not null
				else
				{
					std::cout<<"Norm NULL..."<<std::endl;
				}
			  
			  }// oktag ==1
		  }// else magic wand


	  }//new_id_inside ==1
    } // For all vertices
  return connectedVertices;
}


void OBJECT_MESH::GetConnectedVerticesLasso(vtkSmartPointer<vtkPolyData> mesh, double *vn, double sc,vtkSmartPointer<vtkIdList> ptList, vtkSmartPointer<vtkIdList> nptList,vtkSmartPointer<vtkIdList> exnList, vtkSmartPointer<vtkIdList> oldList,
									   vtkSmartPointer<vtkIdList> veryoldList, int *list_changed, int lasso_x, int lasso_y,int h_screen, int w_screen)
{      

	vtkSmartPointer<vtkIdList> nnptList = vtkSmartPointer<vtkIdList>::New();
	int bchanged =0;
	for (vtkIdType i = 0; i<nptList->GetNumberOfIds(); i++)
	{
		vtkSmartPointer<vtkIdList> connected =
		vtkSmartPointer<vtkIdList>::New();  
		connected = GetConnectedVerticesLasso(mesh, vn, sc, nptList->GetId(i), g_tag_tool, lasso_x, lasso_y, h_screen, w_screen);
		for (vtkIdType j=0; j<connected->GetNumberOfIds();j++)
		{
			int already =0;
			int already2 =0;
			int already3=0;
			int already4=0;
			int already5=0;
			already = this->Already_Listed(connected->GetId(j), exnList);
			already2 = this->Already_Listed(connected->GetId(j), nptList);
			already3 = this->Already_Listed(connected->GetId(j), oldList);
			already4 = this->Already_Listed(connected->GetId(j), veryoldList);
			already5 = this->Already_Listed(connected->GetId(j), nnptList);

			
			//if (already2 ==1) {std::cout<<"already2!"<<std::endl;}
			if (already==0 && already2==0 && already3==0 &&already4==0&&already5==0) { 
				ptList->InsertNextId(connected->GetId(j));				
				nnptList->InsertNextId(connected->GetId(j));				
				bchanged=1;
			}
		}
	}

	veryoldList->Initialize();
	veryoldList->DeepCopy(oldList);
	oldList->Initialize();
	oldList->DeepCopy(exnList);
	exnList->Initialize();
	exnList->DeepCopy(nptList);
	nptList->Initialize();
	nptList->DeepCopy (nnptList);	
	*list_changed=bchanged;
 
}

void OBJECT_MESH::GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, double *vn, double sc,vtkSmartPointer<vtkIdList> ptList, vtkSmartPointer<vtkIdList> nptList,vtkSmartPointer<vtkIdList> exnList, vtkSmartPointer<vtkIdList> oldList,
									   vtkSmartPointer<vtkIdList> veryoldList, int *list_changed)
{      

	vtkSmartPointer<vtkIdList> nnptList = vtkSmartPointer<vtkIdList>::New();
	int bchanged =0;
	for (vtkIdType i = 0; i<nptList->GetNumberOfIds(); i++)
	{
		vtkSmartPointer<vtkIdList> connected =
		vtkSmartPointer<vtkIdList>::New();  
		connected = GetConnectedVertices(mesh, vn, sc, nptList->GetId(i), g_tag_tool);
		for (vtkIdType j=0; j<connected->GetNumberOfIds();j++)
		{
			int already =0;
			int already2 =0;
			int already3=0;
			int already4=0;
			int already5=0;
			already = this->Already_Listed(connected->GetId(j), exnList);
			already2 = this->Already_Listed(connected->GetId(j), nptList);
			already3 = this->Already_Listed(connected->GetId(j), oldList);
			already4 = this->Already_Listed(connected->GetId(j), veryoldList);
			already5 = this->Already_Listed(connected->GetId(j), nnptList);

			
			//if (already2 ==1) {std::cout<<"already2!"<<std::endl;}
			if (already==0 && already2==0 && already3==0 &&already4==0&&already5==0) { 
				ptList->InsertNextId(connected->GetId(j));				
				nnptList->InsertNextId(connected->GetId(j));				
				bchanged=1;
			}
		}
	}

	veryoldList->Initialize();
	veryoldList->DeepCopy(oldList);
	oldList->Initialize();
	oldList->DeepCopy(exnList);
	exnList->Initialize();
	exnList->DeepCopy(nptList);
	nptList->Initialize();
	nptList->DeepCopy (nnptList);	
	*list_changed=bchanged;
 
}
int OBJECT_MESH::Already_Listed(vtkIdType ve, vtkSmartPointer<vtkIdList> ptList)
{
	int already = 0;
	for (vtkIdType i=0; i<ptList->GetNumberOfIds();i++)
	{
	
		if (ve== ptList->GetId(i))
		{already=1;}
	}
	return already;
}



/*

void OBJECT_MESH::GetConnectedVertices(vtkSmartPointer<vtkPolyData>
mesh, int seed, vtkSmartPointer<vtkIdList> connectedVertices)
{

  //get all cells that vertex 'seed' is a part of
  vtkSmartPointer<vtkIdList> cellIdList =
      vtkSmartPointer<vtkIdList>::New();
  mesh->GetPointCells(seed, cellIdList);

  //cout << "There are " << cellIdList->GetNumberOfIds() << " cells
	//that use point " << seed << endl;

  //loop through all the cells that use the seed point
  for(vtkIdType i = 0; i < cellIdList->GetNumberOfIds(); i++)
    {

    vtkCell* cell = mesh->GetCell(cellIdList->GetId(i));
    //cout << "The cell has " << cell->GetNumberOfEdges() << " edges." << endl;

    //if the cell doesn't have any edges, it is a line
    if(cell->GetNumberOfEdges() <= 0)
      {
      continue;
      }

    for(vtkIdType e = 0; e < cell->GetNumberOfEdges(); e++)
      {
      vtkCell* edge = cell->GetEdge(e);

      vtkIdList* pointIdList = edge->GetPointIds();
      //cout << "This cell uses " << pointIdList->GetNumberOfIds() <<
		//"	 points" << endl;
      
      if(pointIdList->GetId(0) == seed || pointIdList->GetId(1) == seed)
        {
        if(pointIdList->GetId(0) == seed)
          {
          connectedVertices->InsertNextId(pointIdList->GetId(1));
          }
        else
          {
          connectedVertices->InsertNextId(pointIdList->GetId(0));
          }
        }
      }


    }

    //cout << "There are " << connectedVertices->GetNumberOfIds() << "
points connected to point " << seed << endl;
}

*/

// Pour ça on utilisera VTK

void OBJECT_MESH::get_screen_projection_matrix( glMatrix screenproj)
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




void OBJECT_MESH::get_world_coordinates_matrix(glMatrix mat)
{
	//glMatrix M2;

	//get_camera_transform_matrix (M2);
	glPushMatrix();
	glLoadIdentity();
	//glMultMatrixf(M2);
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

std::string OBJECT_MESH::Get_Scalar_Name(int scalar)
{

	std::string Depth ("Depth");	
	std::string Max ("Maximum_Curvature");	
	std::string Min ("Minimum_Curvature");	
	std::string Gauss ("Gauss_Curvature");	
	std::string MeanC ("Mean_Curvature");	
	std::string Tags ("Tags");
	std::string Thickness ("Thickness");
	std::string NoneN ("None");
	if (scalar ==0)
	{return Depth;}
	else if (scalar ==1)
	{
		return Max;
	}
	else if (scalar ==2)
	{
		return Min;
	}
	else if (scalar ==3)
	{
		return Gauss;
	}
	else if (scalar ==4)
	{
		return MeanC;
	}
	else if (scalar ==-1)
	{
		return Tags;
	}
	else if (scalar ==5)
	{
		return Thickness;
	}
	else
	{
		return NoneN;
	}
}

std::string OBJECT_MESH::Get_Active_Scalar_Name()
{

	return this->Get_Scalar_Name(g_active_scalar);
}


vtkSmartPointer<vtkIdTypeArray>  OBJECT_MESH::Mesh_Get_Tag_Region_Sizes()
{
	// Constructs an array containing number of vertices held by each tag.
	vtkSmartPointer<vtkIdTypeArray> region_sizes = vtkSmartPointer<vtkIdTypeArray>::New();
	// get min and max ranges.
	vtkIdType Tag_Min = 999999999;
	vtkIdType Tag_Max = 0;
	//double current_tag;
	vtkIdType current_tag;
	vtkIdType act_ve_nr;
	vtkFloatArray *currentTags;		
	currentTags = (vtkFloatArray*)this->GetPointData()->GetScalars("Tags");	
	if (currentTags!=NULL)
	{
		for (vtkIdType i=0;i<this->numvert;i++)	// for each vertex 
		{								
			current_tag=(vtkIdType)currentTags->GetTuple(i)[0];
			if (current_tag > Tag_Max ){Tag_Max = current_tag;}
			if (current_tag < Tag_Min ){Tag_Min = current_tag;}
		}
		region_sizes->SetNumberOfComponents(1);		
		region_sizes->SetNumberOfValues(Tag_Max+1);
		region_sizes->SetNumberOfTuples(Tag_Max+1);
		for (vtkIdType i=0;i<region_sizes->GetNumberOfTuples();i++)	// for each vertex 
		{
			region_sizes->SetValue(i, 0);
		}

		for (vtkIdType i=0;i<this->numvert;i++)	// for each vertex 
		{
			current_tag=(vtkIdType)currentTags->GetTuple(i)[0];
			if (current_tag>=0)
			{
				act_ve_nr = region_sizes->GetValue(current_tag);
				act_ve_nr++;
				region_sizes->SetValue(current_tag, act_ve_nr);
			}

		}		
	}
	return region_sizes;

}

void OBJECT_MESH::Test_Scalar(std::string myScalar)
{

	vtkFloatArray *currentScalars;
	currentScalars = (vtkFloatArray*)this->GetPointData()->GetScalars(myScalar.c_str());
	
	if (currentScalars != NULL)
	{
		//std::cout<<"Not Null! "<<std::endl;
		int already =0;
												
		for (int i=0; i<g_scalar_list.size(); i++)
		{
			//std::cout<<"i"<<i<<std::endl;
			std::size_t found = g_scalar_list.at(i).find(myScalar);			
			if (found != std::string::npos)
			{
				already=1; 					
			}		
		}
		if (already==0)
		{
			//std::cout<<"Try to puch back "<<Depth<<std::endl;
			g_scalar_list.push_back(myScalar);
			//std::cout<<"Depth pushed back"<<g_scalar_list.size()<<std::endl;
			g_scalar_list.size();
		}
	}

}

void OBJECT_MESH::Test_Scalar_Selected(std::string myScalar)
{

	vtkFloatArray *currentScalars;
	currentScalars = (vtkFloatArray*)this->GetPointData()->GetScalars(myScalar.c_str());
	
	if (currentScalars != NULL)
	{
		//std::cout<<"Not Null! "<<std::endl;
		int already =0;
												
		for (int i=0; i<g_scalar_list_selected.size(); i++)
		{
			//std::cout<<"i"<<i<<std::endl;
			std::size_t found = g_scalar_list_selected.at(i).find(myScalar);			
			if (found != std::string::npos)
			{
				already=1; 					
			}		
		}
		if (already==0)
		{
			//std::cout<<"Try to puch back "<<Depth<<std::endl;
			g_scalar_list_selected.push_back(myScalar);
			//std::cout<<"Depth pushed back"<<g_scalar_list.size()<<std::endl;
			g_scalar_list_selected.size();
		}
	}

}

void OBJECT_MESH::Compute_Global_Scalar_List()
{ 
	
	
	std::string Depth ("Depth");	
	std::string Max ("Maximum_Curvature");	
	std::string Min ("Minimum_Curvature");	
	std::string Gauss ("Gauss_Curvature");	
	std::string MeanC ("Mean_Curvature");	
	std::string Colour ("RGB");	
	
	this->Test_Scalar(Depth);
	this->Test_Scalar(Max);	
	this->Test_Scalar(Min);	
	this->Test_Scalar(Gauss);	
	this->Test_Scalar(MeanC);
	this->Test_Scalar(Colour);
		
	if (this->selected==1)
	{
		this->Test_Scalar_Selected(Depth);
		this->Test_Scalar_Selected(Max);
		this->Test_Scalar_Selected(Min);
		this->Test_Scalar_Selected(Gauss);
		this->Test_Scalar_Selected(MeanC);
		this->Test_Scalar_Selected(Colour);

	}

	if (this->nextobj !=NULL)
	{
		this->nextobj->Compute_Global_Scalar_List();		
	}

}


void OBJECT_MESH::Update_Distinct_Selected_Names(std::string Name)
{
	
		int already =0;
												
		for (int i=0; i<g_distinct_selected_names.size(); i++)
		{
			//std::cout<<"i"<<i<<std::endl;
			std::size_t found = g_distinct_selected_names.at(i).find(Name);			
			int length1 = Name.length();
			int length2 = g_distinct_selected_names.at(i).length();
			if (length1 == length2 && found != std::string::npos)
			{
				already=1; 					
			}		
		}
		if (already==0)
		{			
			g_distinct_selected_names.push_back(Name);			
		}
	

}

void OBJECT_MESH::Compute_Name_Lists()
{ 
				
		
	if (this->selected==1)
	{
		
		std::string Name = this->name.c_str();
		g_selected_names.push_back(Name);
		this->Update_Distinct_Selected_Names(Name);		

	}

	if (this->nextobj !=NULL)
	{
		this->nextobj->Compute_Name_Lists();		
	}
}
void OBJECT_MESH::Calculate_Global_MinMax(float *minx, float *maxx, float *miny, float *maxy, float *minz, float *maxz)
{



	//std::cout << " *minx initial value=:" << *minx << std::endl;
	//std::cout<<"Calculate_GlobalMean min max: "<<level<<" selected="<<selected<< std::endl;
	float inx = 999999999, axx = -999999999, iny = 999999999, axy = -999999999, inz = 999999999, axz = -999999999;
	float vv1[3], vv[3];

	float tmpminx, tmpmaxx, tmpminy, tmpmaxy, tmpminz, tmpmaxz;
	int nbverts = 0;

	glMatrix  wc_mat;
	this->get_world_coordinates_matrix(wc_mat);
	
	nbverts = this->numvert;
	double vvd[3];

	for (int i = 0; i<this->numvert; i++)	// for each vertex of this
	{
		this->GetPoint(i, vvd);
		vv1[0] = (float)vvd[0];
		vv1[1] = (float)vvd[1];
		vv1[2] = (float)vvd[2];
		ApplyTransformation(vv1, vv, wc_mat);
		tmpminx = vv[0];
		//std::cout
		tmpmaxx = vv[0];
		tmpminy = vv[1];
		tmpmaxy = vv[1];
		tmpminz = vv[2];
		tmpmaxz = vv[2];


		if (tmpminx < inx) { inx = tmpminx; 
		//	std::cout << "new inx:" << inx << std::endl; 
		}
		if (tmpminy < iny) { iny = tmpminy; }
		if (tmpminz < inz) { inz = tmpminz; }

		if (tmpmaxx > axx) { axx = tmpmaxx; }
		if (tmpmaxy > axy) { axy = tmpmaxy; }
		if (tmpmaxz > axz) { axz = tmpmaxz; }

	}
	
	if (this->nextobj != NULL)
	{
		this->nextobj->Calculate_Global_MinMax(&inx, &axx, &iny, &axy, &inz, &axz);
	}
	
	//std::cout << "inx after a nextobj:" << inx << std::endl;
	//std::cout << "*minx compared:" << *minx << std::endl;
	if (*minx > inx) {
		*minx = inx; 
		//std::cout << "new *minx:" << *minx << std::endl;
	}
	if (*miny > iny) { *miny = iny; }
	if (*minz > inz) { *minz = inz; }

	if (*maxx < axx) { *maxx = axx; }
	if (*maxy < axy) { *maxy = axy; }
	if (*maxz < axz) { *maxz = axz; }

	
	//std::cout << " *minx set to:" << *minx << std::endl;

}
void OBJECT_MESH::Calculate_GlobalMean (float *meanx, float *meany, float *meanz,  int *nb, int only_selected)
{
	// Que faire quand on n'est pas l'objet basal ????
	float tmpx= 0, tmpy= 0,tmpz = 0;int nbverts = 0;
	float tmpx2= 0, tmpy2= 0,tmpz2 = 0;int nbverts2 = 0;
	float vv1[3], vv[3];
	glMatrix  wc_mat;
	this->get_world_coordinates_matrix(wc_mat);
	if (only_selected ==0 || this->selected ==1)
	{
		nbverts= this->numvert;
		double vvd[3];

		for (int i=0;i<this->numvert;i++)	// for each vertex of this
		{
			this->GetPoint(i, vvd);
			vv1[0] =  (float) vvd[0];
			vv1[1] =  (float) vvd[1];
			vv1[2] =  (float) vvd[2];
			ApplyTransformation(vv1,vv,wc_mat);
			tmpx+= vv[0];
			tmpy+= vv[1];
			tmpz+= vv[2];
		}
	}

	if (this->nextobj !=NULL)
	{this->nextobj->Calculate_GlobalMean (&tmpx2, &tmpy2, &tmpz2, &nbverts2, only_selected);}

	*meanx = tmpx + tmpx2;
	*meany = tmpy + tmpy2;
	*meanz = tmpz + tmpz2;
	*nb = nbverts +nbverts2;

}

int OBJECT_MESH::Mesh_Select(float xmin, float xmax, float ymin, float ymax)
{

	//std::cout<<"Select "<<this->name<<std::endl;
	//@modif
	//int bchange, bchange2;
	int bchange;
	bchange = 0;
	float vv1[3], vv[3];
	glMatrix screen_mat, wc_mat;

	glPushMatrix();
	glMultMatrixf((GLfloat*) this->Mat2); // Position
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
	this->get_screen_projection_matrix(screen_mat);
	this->get_world_coordinates_matrix(wc_mat);
	glPopMatrix();
	double vvd[3];
	//std::cout<<"Select "<<this->name<<" browse vertices"<<std::endl;
	for (int i=0;i<this->numvert;i++)	// for each vertex of this
	{		
		this->GetPoint(i, vvd);	
		vv1[0] =  (float) vvd[0];
		vv1[1] =  (float) vvd[1];
		vv1[2] =  (float) vvd[2];			
		ApplyTransformation(vv1,vv,screen_mat);	

		//
		if ((vv[2]>-1.0) && vv[2]<1.0 && (vv[0] >=xmin)&& (vv[0] <=xmax)&& 	(vv[1] >=ymin)&& (vv[1] <=ymax))
			{
				bchange = 1;					
			}		
	}//for numvert
	//std::cout<<"Select "<<this->name<<" change select status"<<std::endl;

	if (bchange ==1)
	{
		// we only select/unselect by hand what is visible... otherwise this is not good!
		if (this->view==1)
		{
			if (this->selected ==0){this->selected =1;}
			else {this->selected = 0;}
		}
			
	}//if bchange
	//if (this->nextobj !=NULL)
	//{
		//@modif
		//bchange2 = nextobj->Mesh_Select(xmin, xmax, ymin, ymax);
	//	nextobj->Mesh_Select(xmin, xmax, ymin, ymax);
	//}
	//@modif
	//if (bchange2 ==1){bchange = 1;}
	//std::cout<<"Select "<<this->name<<" done"<<std::endl;
	//@modif
	return bchange;
}

void OBJECT_MESH::Mesh_invert_normals(int select_mode)
{
	if (this->selected == 1 ||select_mode ==3)
	{
	
	  vtkSmartPointer<vtkFloatArray> normalsArray = 
      vtkSmartPointer<vtkFloatArray>::New();

  normalsArray->SetNumberOfComponents(3); //3d normals (ie x,y,z)
  normalsArray->SetNumberOfTuples(this->GetNumberOfPoints());
  double *vn;
  float vn2[3];
  vtkFloatArray* norms = vtkFloatArray::SafeDownCast
  (this->GetPointData()->GetNormals());
  //std::cout << "\n old norms :"<<norms->GetNumberOfTuples()
	//  <<"\n Current object  number of points:"<<this->GetNumberOfPoints();

	for (int i=0;i<this->GetNumberOfPoints();i++) {
		vn=norms->GetTuple((vtkIdType)i);
		vn2[0]=(float)(-vn[0]);vn2[1]=(float)(-vn[1]);vn2[2]=(float)(-vn[2]);
		normalsArray->SetTuple(i, vn2) ;
		//std::cout << "\n i:"<<i<<"|vn2[0]"<<vn2[0]<<"|vn2[1]"<<vn2[1]<<"|vn2[2]"<<vn2[2];
	}
 
  //add the normals to the cells in the polydata
	//std::cout << "\n Set Normals? Reference count:"<<normalsArray->GetReferenceCount();
	//normalsArray->GetReferenceCount();
	this->GetPointData()->SetNormals(normalsArray);
	//std::cout << "\n Set Normals done! Reference count:"<<normalsArray->GetReferenceCount();
	}
	if (this->nextobj!=NULL)
	{
		this->nextobj->Mesh_invert_normals(select_mode);
	}
}

void OBJECT_MESH::Mesh_invert(int select_mode)
{	
	if (this->selected == 1 ||select_mode ==3)
	{
				this->Mesh_invert_normals(select_mode);
				int ve1, ve2, ve3;								
				vtkSmartPointer<vtkIdList> oldpoints =vtkSmartPointer<vtkIdList>::New();
				vtkSmartPointer<vtkIdList> newpoints =vtkSmartPointer<vtkIdList>::New();
			
				//std::cout << "\n Mesh invert";
				for (int i=0;i<this->numtri;i++) {
				this->GetCellPoints (i, oldpoints);				
				ve1=(int)oldpoints->GetId(0);
				ve2=(int)oldpoints->GetId(1);
				ve3=(int)oldpoints->GetId(2);								
				/*if (i==0 || i==(this->numtri -1))
				{  std::cout << "\n i:"<<i<<"BEFORE: ve1="<<ve1<<"|ve2="<<ve2<<"|ve3="<<ve3;
				
				}*/				
				
				this->ReverseCell((vtkIdType)i);						

				this->GetCellPoints (i, newpoints);								
				ve1=(int)newpoints->GetId(0);
				ve2=(int)newpoints->GetId(1);
				ve3=(int)newpoints->GetId(2);
				/*if (i==0 || i==(this->numtri -1))
				{  std::cout << "\n i:"<<i<<"AFTER: ve1="<<ve1<<"|ve2="<<ve2<<"|ve3="<<ve3;
				
				}*/
				}
				// for every triangle 				

	}
	if (this->nextobj!=NULL)
	{
		this->nextobj->Mesh_invert(select_mode);
	}
}

float OBJECT_MESH:: get_area()
{
	double area;
	vtkSmartPointer<vtkMassProperties> mass =vtkSmartPointer<vtkMassProperties>::New();
	mass->SetInputData ((vtkPolyData*) this);
	mass->Update();
	area = mass->GetSurfaceArea(); 
	return area;
}


float OBJECT_MESH:: get_volume()
{
	double volume;
	vtkSmartPointer<vtkMassProperties> mass =vtkSmartPointer<vtkMassProperties>::New();
	  mass->SetInputData ((vtkPolyData*) this);
	  mass->Update();
	  volume = mass->GetVolume();
	 
		return volume;
}
void OBJECT_MESH::Mesh_area_volume()
{
  
  

  double area, volume;
  
  //    float *vvv;

  if (this->selected == 1)
  {
  
	  vtkSmartPointer<vtkMassProperties> mass =vtkSmartPointer<vtkMassProperties>::New();
	  mass->SetInputData ((vtkPolyData*) this);
	  mass->Update();
	  volume = mass->GetVolume();
	  area = mass->GetSurfaceArea();
	
	  std::cout<<"Object area (without deformation matrix): "<<area<<" mm2; "<<std::endl;
	std::cout<<"Object volume (without deformation matrix + beware, object must be closed!): "<<volume<<" mm3\n";

  }//Fin if selected
  if (this->nextobj !=NULL)
  {nextobj->Mesh_area_volume();}

}

void OBJECT_MESH::Add_Object(OBJECT_MESH * New_Obj)
{
	int Ok;
	Ok = 1;
	OBJECT_MESH * parent;
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


 
void OBJECT_MESH::Hierarchy (int depth)
{
	for (int i = 1;i<=depth; i++)
	{
		std::cout<<"   ";
		if (this->selected ==1){std::cout<<"*";}
	}
	std::cout<<this->name<<"|nv:"<<this->numvert<<"|nt:"<<this->numtri<<std::endl;
	
	if (this->nextobj !=NULL)
	{this->nextobj->Hierarchy(depth);}
}

void OBJECT_MESH::get_object_names_and_view_values(std::vector<std::string>& object_names, std::vector<int>& object_view_values)
{
	object_names.push_back(this->name);
	object_view_values.push_back(this->view);
	
	if (this->nextobj !=NULL)
	{this->nextobj->get_object_names_and_view_values(object_names, object_view_values);}
	
}
void OBJECT_MESH::Set_Blend(int blend_value)
{
	if (this->selected ==1)
	{
		this->color[3] = (float)blend_value/100;
		this->selected =0;
		this->blend = (float)blend_value/100;
	}	
	if (this->nextobj != NULL)
	{nextobj->Set_Blend(blend_value);}
}
void OBJECT_MESH::Mesh_Unselect()
{
	this->selected =0;
	if (this->nextobj !=NULL)
    {this->nextobj->Mesh_Unselect();}

}
void OBJECT_MESH::Mesh_Select()
{
	this->selected =1;
	this->view=1;
	if (this->nextobj !=NULL)
    {this->nextobj->Mesh_Select();}

}


OBJECT_MESH::OBJECT_MESH() : OBJECT() // Constructor
{														// And Defines points
	//	this->points=(VERTEX*)malloc(sizeof(VERTEX)*n);		// Sets points Equal To VERTEX * Number Of Vertices
		//Morpho_only!	

	//normals =vtkSmartPointer<vtkPolyDataNormals>::New();	
	/*this->tx=0;
	this->ty=0;*/
	this->texture=0;
	this->numtri = 0;
	this->numvert = 0;
	this->is_logical = 0;

	this->pt_cube_nr[0]=0;
	this->pt_cube_nr[1]=0;
	this->pt_cube_nr[2]=0;
	this->pt_total_cube_nr=0;
	this->pt_vertices_per_cube = NULL;
	this->pt_cubes=NULL;
	this->pt_vertices_per_cube_compare = NULL;
	this->pt_cubes_compare=NULL;
	
	blend = 1;
   nextobj = NULL;
   prevobj = NULL;
	tr_sort = NULL;
	this->Mesh_init_Mat();
	this->mean[0] = 0;this->mean[1] = 0;this->mean[2] = 0;
	this->selected = 0;
	this->view=1;
	this->color[0] = 1;this->color[1] = 1;this->color[2] = 1; this->color[3] = 1; 
  
	this->bool_init_buf = 0;

	modifTab = vtkSmartPointer<vtkFloatArray>::New();
	modifTab->SetNumberOfComponents(2);
	modifTab->SetNumberOfTuples(4);
	//la premier colonne du tableau indique le mode actuel de l'objet
	// la deuxieme colonne du tableau indique si un changement a eu lieu
	modifTab->SetTuple2(0, selected, 0);
	modifTab->SetTuple2(1, g_tag_mode, 0);
	modifTab->SetTuple2(2, dispmode, 0);
	modifTab->SetTuple2(3, bool_changed_matrix, 0);
	
}

void OBJECT_MESH::Initialize_Scalar(int scalar)
{
// initialize active scalar tags
	if (this->selected ==1)
	{
	vtkSmartPointer<vtkFloatArray> newScalars = 
    vtkSmartPointer<vtkFloatArray>::New();
	newScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
	newScalars->SetNumberOfTuples(this->numvert);
	newScalars->SetNumberOfTuples(this->numvert);
    double v = 0.0;
		for (vtkIdType i=0;i<this->numvert;i++)	// for each vertex of this		
		{						
			newScalars->InsertTuple1(i, v);								
		}
		std::string sc_name = this->Get_Scalar_Name(scalar);
		newScalars->SetName(sc_name.c_str());		
			this->GetPointData()->RemoveArray(sc_name.c_str());
			this->GetPointData()->AddArray(newScalars);
			this->GetPointData()->SetActiveScalars(sc_name.c_str());
			this->selected=0;
	}

	if (nextobj !=NULL)
	{
		nextobj->Initialize_Scalar(scalar);
	}
		
}
void OBJECT_MESH::Remove_Scalar(int scalar)
{
// initialize active scalar tags
	if (this->selected ==1)
	{
	
		std::string sc_name = this->Get_Scalar_Name(scalar);		
		this->GetPointData()->RemoveArray(sc_name.c_str());
		this->selected=0;
	}

	if (nextobj !=NULL)
	{
		nextobj->Remove_Scalar(scalar);
	}
		
}
void OBJECT_MESH::Initialize_Tags()
{
	
	this->Initialize_Scalar(5);
				
}
void OBJECT_MESH::Mesh_Tri_Sort(int side)
{
	
	// Side 0: diplays the triangles from front to back (real transparency)
	// Side 1: dispalys the triangles from back to front. (not real transparency)
	glMatrix MMat;
	float vv[3]; float vv1[3]; 
	int ve1, ve2, ve3;
	vtkSmartPointer<vtkIdList> points =vtkSmartPointer<vtkIdList>::New();
	double v1[3],v2[3], v3[3];
	int i;
    
	
	if (tr_sort ==NULL)
	{
		this->tr_sort = (int *) malloc((this->numtri)*sizeof(int));
	}

	glPushMatrix();		
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
	getmatrix(MMat);
	glPopMatrix();

	
	/*if (this->blend<1)
	{*/
		tri_depth *tmp_tri=(tri_depth*)malloc((int)(numtri)*sizeof(tri_depth));	
		for (i =0;i<this->numtri; i++)
		{
				this->GetCellPoints (i, points);

				ve1=(int)points->GetId(0);
				ve2=(int)points->GetId(1);
				ve3=(int)points->GetId(2);				
			
				this->GetPoint((vtkIdType)ve1, v1);
				this->GetPoint((vtkIdType)ve2, v2);
				this->GetPoint((vtkIdType)ve3, v3);
							
				vv1[0] = (float)(v1[0] + v2[0]+v3[0])/3;
				vv1[1] = (float)(v1[1] + v2[1]+v3[1])/3;;
				vv1[2] = (float)(v1[2] + v2[2]+v3[2])/3;
				
				ApplyTransformation(vv1,vv,MMat);				
				tmp_tri[i].tn = i;
				tmp_tri[i].d = vv[2];

		}
		// sort list trm_tri
		
		qsort(tmp_tri,numtri,sizeof(tri_depth),tri_depth_compare);
		
		// construct tr_sort
		if (side ==0)
		{
			for (i =0;i<this->numtri; i++)
			{
				this->tr_sort[i] = tmp_tri[i].tn;
					
			}

		}
		else
		{
			for (i =0;i<this->numtri; i++)
			{
				this->tr_sort[i] = tmp_tri[this->numtri-1-i].tn;		
			}
		}
		

		free(tmp_tri);
		tmp_tri = NULL;
	//}
	/*else
	{
		for (i =0;i<this->numtri; i++)
		{
			this->tr_sort[i] = i;	
		}
	}*/	
	if (nextobj !=NULL)
	{
		nextobj->Mesh_Tri_Sort(side);
	}
}
OBJECT_MESH::OBJECT_MESH(const OBJECT_MESH & mobj):OBJECT() 
{
	//normals =vtkSmartPointer<vtkPolyDataNormals>::New();
	
	this->texture=0;
	this->numtri = 0;
	this->numvert = 0;
	this->is_logical = 0;
	blend = 1;
   nextobj = NULL;
   prevobj = NULL;
	tr_sort = NULL;
	this->Mesh_init_Mat();
	this->mean[0] = 0;this->mean[1] = 0;this->mean[2] = 0;
	this->selected = 0;
	this->color[0] = 1;this->color[1] = 1;this->color[2] = 1; this->color[3] = 1;     
  
  int i, j;
  
  
  mean[0] = mobj.mean[0];
  mean[1] = mobj.mean[1];
  mean[2] = mobj.mean[2];
				      
  color[0] = color_obj[0];
  color[1] = color_obj[1];
  color[2] = color_obj[2];
  color[3] = color_obj[3]; 
  this->name = "n_";
  this->name.append(mobj.name.c_str());

  
  
 numvert=mobj.numvert;

  numtri=mobj.numtri;
  
  // duplicate matrices:
  for ( i=0;i<4;i++)
    for ( j=0;j<4;j++)
      {
		this->Mat1[i][j]=mobj.Mat1[i][j];
		this->Mat2[i][j]=mobj.Mat2[i][j];
      }
  
    
  

  this->bool_init_buf = 0;
    

  modifTab = vtkSmartPointer<vtkFloatArray>::New();
  modifTab->SetNumberOfComponents(2);
  modifTab->SetNumberOfTuples(4);

  modifTab->DeepCopy(mobj.modifTab);
        
}

OBJECT_MESH::~OBJECT_MESH()//Destructor
{
	//std::cout << "\nTry Destructor called"<<this->GetReferenceCount();
  if (this->tr_sort !=NULL)free(this->tr_sort);

	// delete pt_cubes and pt_vertices_per_cube...
	if (this->pt_cubes !=NULL )
	{
		for (vtkIdType ci=0;ci<this->pt_total_cube_nr;ci++)
		{
			if (this->pt_vertices_per_cube !=NULL && this->pt_vertices_per_cube[ci]>0)
			{
				delete pt_cubes[ci];
			}
			this->pt_cubes[ci]=NULL;		
		}
		delete pt_cubes;
	}	
	
	this->pt_cubes=NULL;
	if (this->pt_vertices_per_cube !=NULL)
	{
		delete this->pt_vertices_per_cube;
	}
	this->pt_vertices_per_cube=NULL;

	// delete pt_cubes_compare and pt_vertices_per_cube_compare...
	if (this->pt_cubes_compare !=NULL )
	{
		for (vtkIdType ci=0;ci<this->pt_total_cube_nr;ci++)
		{
			if (this->pt_vertices_per_cube_compare !=NULL && this->pt_vertices_per_cube_compare[ci]>0)
			{
				delete pt_cubes_compare[ci];
			}
			this->pt_cubes_compare[ci]=NULL;			
		}
		delete pt_cubes_compare;
	}
	
	this->pt_cubes_compare=NULL;
	if (this->pt_vertices_per_cube_compare !=NULL)
	{
		delete this->pt_vertices_per_cube_compare;
	}
	if (bool_init_buf==1)
		Mesh_DrawObj_delete();
}

// Mise à jour de l'objet à partir de ses nouveaux points
void OBJECT_MESH::Update(vtkPoints* Points){
	cout << "Update" << endl;
	
	this->SetPoints(Points);
	vtkSmartPointer<vtkPolyDataNormals> ObjNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
	ObjNormals->SetInputData(this);
	ObjNormals->ComputePointNormalsOn();
	ObjNormals->ComputeCellNormalsOn();
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
	this->DeepCopy(cleanPolyDataFilter->GetOutput());
	cout << "sourceClean " << this->GetNumberOfPoints() << endl;

	std::cout << "\nVtkConnectivity new Number of points:" << this->GetNumberOfPoints() << std::endl;
	std::cout << "VtkConnectivity new Number of cells:" << this->GetNumberOfCells() << std::endl;

	this->Mesh_init_Mat();
	this->Compute_mean();
	this->Compute_radius();
	modifTab->SetComponent(3, 1, 1);// indique que l'on a modifié ses points pour le buffer 
}

// on calcule le centre de l'object
void OBJECT_MESH::Compute_mean(){
	double*ve;
	
	float mean[3];
	mean[0] = 0;
	mean[1] = 0;
	mean[2] = 0;
	int i;
	for (i = 0; i<numvert; i++)
	{
		ve = this->GetPoint(i);
		mean[0] += (float)ve[0];
		mean[1] += (float)ve[1];
		mean[2] += (float)ve[2];
	}

	mean[0] /= numvert;
	mean[1] /= numvert;
	mean[2] /= numvert;

	this->mean[0] = mean[0];
	this->mean[1] = mean[1];
	this->mean[2] = mean[2];

}

//On calcule le rayon maximum de l'objet
void OBJECT_MESH::Compute_radius(){
	object_radius=0;
	float dist = 0;
	double center[3] = { mean[0], mean[1], mean[2] };
	for (int i = 0; i < numvert; i++){
		dist = sqrt(dist2(center, this->GetPoint(i)));
		if (object_radius <  dist){
			object_radius = dist;
		}
	}
}

//on vérifie que les deux objets sont taggés avec le même nombre de couleur
void OBJECT_MESH::CheckSameColor(OBJECT_MESH *object, bool *bool_exist_color, bool *bool_same_color){
	bool b = false;
	if (this->GetPointData()->GetScalars("Tags") != NULL && object->GetPointData()->GetScalars("Tags") != NULL){
		*bool_exist_color &= true;
		int cptColor = 0;
		int numPointmin, numPointmax;
		double valColor = 0;
		set<double> colorMin, colorMax;
		OBJECT_MESH *objectmin, *objectmax;
		
		// on cherche lequel des deux objects a le plus petit nombre de points
		// pour faire qu'une boucle.
		if (numvert < object->numvert){
			objectmin = this;
			objectmax = object;
			numPointmin = numvert;
			numPointmax = object->numvert;
		}
		else{
			objectmin = object;
			objectmax = this;
			numPointmin = object->numvert;
			numPointmax = numvert;
		}
		

		// on met leurs valeurs dans un ensemble (les nombres ne sont pas répétés plusieurs fois)
		for (int i = 0; i < numPointmax; i++){
			valColor = *(objectmax->GetPointData()->GetScalars("Tags")->GetTuple(i));
			colorMax.insert(valColor);
			if (i < numPointmin){
				valColor = *(objectmin->GetPointData()->GetScalars("Tags")->GetTuple(i));
				colorMin.insert(valColor);
			}
		}
		
		// verification qu'ils ont le même nombre de tags
		b = colorMax.size() == colorMin.size();
		if (b){
			cptColor = colorMax.size();
			std::set<double>::iterator it;
			// verification qu'ils ont les mêmes couleurs 
			//si une nouvelle couleur apparait la taille va changer
			for (it = colorMin.begin(); it != colorMin.end(); ++it){
				colorMax.insert(*it);
			}

			b = cptColor == colorMax.size();
		}
		*bool_same_color &= b;
	}
	else{
		*bool_exist_color &= false;
		*bool_same_color &= false;
	}
}

//On cherche le minimum et le maximum des coordonnées des points et ses normales dans l'objet
void OBJECT_MESH::Find_minMax_CoordofPoint_Normal(double Pointmin_max[6], double norPointmin_max[6]){
	double*ve, *nve;
	for (int i = 0; i<numvert; i++)
	{
		ve = this->GetPoint(i);
		nve = this->GetPointData()->GetNormals()->GetTuple(i);
		 
		for (int j = 0; j<3; j++){
			//point
			if (Pointmin_max[j] > ve[j]) // min
				Pointmin_max[j] = ve[j];
			if (Pointmin_max[j + 3] < ve[j]) // max
				Pointmin_max[j + 3] = ve[j];
			//normale
			if (norPointmin_max[j]>nve[j]) // min
				norPointmin_max[j] = nve[j];
			if (norPointmin_max[j + 3] < nve[j]) // max
				norPointmin_max[j + 3] = nve[j];
		}
	}
}

//On cherche le minimum et le maximum des courbures de l'objet
void OBJECT_MESH::Find_minMax_CoordofCurv(double tab_minMax_curv[8]){
	double curv_data;
	
	const char * curv[4];
	curv[0] = "Maximum_Curvature";
	curv[1] = "Minimum_Curvature";
	curv[2] = "Mean_Curvature";
	curv[3] = "Gauss_Curvature";
	  
	for (int i = 0; i<numvert; i++) 
	{
		for (int j = 0; j<4; j++){
			if (this->GetPointData()->GetScalars(curv[j]) != NULL){
				curv_data = this->GetPointData()->GetScalars(curv[j])->GetTuple1(i);

				if (tab_minMax_curv[j]>curv_data)//min
					tab_minMax_curv[j] = curv_data;
				if (tab_minMax_curv[j + 4] < curv_data)//max
					tab_minMax_curv[j + 4] = curv_data;
			}
		}
	}
}


void OBJECT_MESH::Sort_Curv_ICP(char* curv, float percentage){
	if (vtkFloatArray::SafeDownCast(this->GetPointData()->GetScalars(curv)) != NULL && percentage != 0){

		vtkFloatArray *currentCurv1 = vtkFloatArray::New();
		currentCurv1->SetNumberOfComponents(1);
		currentCurv1->DeepCopy(vtkFloatArray::SafeDownCast(this->GetPointData()->GetScalars(curv)));

		//Tri des valeurs de la courbure 'curv' de manière croissante
		tri_depth *tmp_vert = (tri_depth*)malloc((int)(numvert)*sizeof(tri_depth));
		for (int i = 0; i < numvert; i++){
			tmp_vert[i].d = currentCurv1->GetComponent(i, 0);
			tmp_vert[i].tn = i;
		}
		qsort(tmp_vert, numvert, sizeof(tri_depth), tri_depth_compare);

		// Calcule du pourcentage
		int p = percentage*numvert;
		float valmin = tmp_vert[p - 1].d;
		float valmax = tmp_vert[numvert-1-p].d;
		cout << "indice: " << (p - 1) << "     " << (numvert - 1 - p) << " numvert : " << numvert << endl;
		cout << "  curv minmax :  " << valmin << "  " << valmax << "   p  :" << p << endl;

		// Remplacement de certaines valeurs par valmin ou valmax
		for (int i = 0; i < numvert; i++){
			if (currentCurv1->GetComponent(i, 0) < valmin){
				currentCurv1->SetComponent(i, 0, valmin);
			}
			if (currentCurv1->GetComponent(i, 0) > valmax){
				currentCurv1->SetComponent(i, 0, valmax);
			}
		}

		this->GetPointData()->GetScalars(curv)->DeepCopy(currentCurv1);
		
		free(tmp_vert);
	}
}

// 
void OBJECT_MESH::Find_Tab_of_neighborhood(){

	cout << "eee" << endl;
	set_neighborbhood = new set<int>[numvert];
	cout << "rrrr" << endl;
	vtkSmartPointer<vtkIdList> points = vtkSmartPointer<vtkIdList>::New();
	int t1 = 0, t2 = 0, t3=0;
	cout << "ttttt" << endl;
	
	//for (int i = 0; i < numvert; i++){
		for (int t = 0; t < numtri; t++){

			this->GetCellPoints(t, points);
			t1 = (int)points->GetId(0);
			t2 = (int)points->GetId(1);
			t3 = (int)points->GetId(2);
			

			//if (i==t1){
				set_neighborbhood[t1].insert(t2);
				set_neighborbhood[t1].insert(t3);
				set_neighborbhood[t2].insert(t1);
				set_neighborbhood[t2].insert(t3);
				set_neighborbhood[t3].insert(t1);
				set_neighborbhood[t3].insert(t2);
			//}
			//if(i== t2){
				//set_neighborbhood[t2].insert(t1);
				//set_neighborbhood[t2].insert(t3);
				/*
				set_neighborbhood[t1]->insert(t2);
				set_neighborbhood[t1]->insert(t3);
				set_neighborbhood[t3]->insert(t1);
				set_neighborbhood[t3]->insert(t2);*/
			//}
			//if (i==t3){
			//	set_neighborbhood[t3].insert(t1);
			//	set_neighborbhood[t3].insert(t2);
			/*
				set_neighborbhood[t2]->insert(t1);
				set_neighborbhood[t2]->insert(t3);
				set_neighborbhood[t1]->insert(t3);
				set_neighborbhood[t1]->insert(t2);*/
			//}
		}
	//}
		cout << set_neighborbhood[0].size() << endl;

}

// calcule la moyenne des aretes
void OBJECT_MESH::Compute_mean_of_edge(){
	float cpt = 0;

	vtkSmartPointer<vtkPoints> list = vtkSmartPointer<vtkPoints>::New();
	list->DeepCopy(this->GetPoints());
	vtkSmartPointer<vtkPoints> list1 = vtkSmartPointer<vtkPoints>::New();
	list1->DeepCopy(this->GetPoints());


	vtkSmartPointer<vtkIdList> points = vtkSmartPointer<vtkIdList>::New();
	int t1 = 0, t2 = 0, t3 = 0;
	mean_arr = 0;
	for (int t = 0; t < numtri; t++){
		this->GetCellPoints(t, points);
		t1 = (int)points->GetId(0);
		t2 = (int)points->GetId(1);
		t3 = (int)points->GetId(2);

		double *p1 = this->GetPoint(t1);
		double *p2 = list->GetPoint(t2);
		double *p3 = list1->GetPoint(t3);

		if (t == 0){
			cout << t1 << "  " << t2 << "   " << t3 << endl;
			affiche(p1, 3); affiche(p2, 3); affiche(p3, 3);
			mean_arr = sqrt((p1[0] - p2[0])*(p1[0] - p2[0])
				+ (p1[1] - p2[1])*(p1[1] - p2[1])
				+ (p1[2] - p2[2])*(p1[2] - p2[2]));
			cout << "tetet " << mean_arr<< endl;
			cout << sqrt(vtkMath::Distance2BetweenPoints(p1, p2)) << "   "
				<< vtkMath::Distance2BetweenPoints(p1, p2) << endl;
			
		}
		mean_arr += sqrt(vtkMath::Distance2BetweenPoints(p1, p2));
		mean_arr += sqrt(vtkMath::Distance2BetweenPoints(p1, p3));
		mean_arr += sqrt(vtkMath::Distance2BetweenPoints(p3, p2));
		cpt += 3;
	}
	cout << mean_arr << "   avt "<<cpt<<"  "<<numtri<<" cpt/2 "<< cpt/2. << endl;
	mean_arr /= (cpt/2.);
}




//Converti les matrices Mat1 et Mat2 en tableau de double
void OBJECT_MESH::Convert_GLMatrix_ToDouble(double M1[16], double M2[16], double M1n[9], double M2n[9]){

	int j = -1; int k = 0;
	for (int i = 0; i < 16; i++){
		if (i % 4 == 0)
			j++;
		M1[i] = Mat1[j][i % 4];
		M2[i] = Mat2[j][i % 4];

		if (k<9 && i % 4 <= 2){
			M1n[k] = Mat1[j][i % 4];
			M2n[k] = Mat2[j][i % 4];
			k++;
		}
	}
}

// Initialisation du buffer
void OBJECT_MESH::Mesh_DrawObj_init(){
	this->Mesh_DrawObj_Building_list();
	this->Mesh_DrawObj_Building_Buff();
	bool_init_buf = 1;
}

//création des listes pour le buffer
void OBJECT_MESH::Mesh_DrawObj_Building_list(){
	glColor3f(0.0f, 0.0f, 0.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

	N_VERTS = this->numvert;
	D_SIZE = 3;
	C_SIZE = 4;
	T_TRI = this->numtri;
	vtkSmartPointer<vtkIdList> pointId = vtkSmartPointer<vtkIdList>::New();
	vtkSmartPointer<vtkFloatArray> norms = vtkSmartPointer<vtkFloatArray>::New();
	norms = vtkFloatArray::SafeDownCast(this->GetPointData()->GetNormals());
	
	size_info = N_VERTS*(C_SIZE + 2*D_SIZE);// nb_de_point *( dimension couleur + dimension normale + dimension point)
	ObjectInfoArray = new GLfloat[size_info];
	ObjectIndiceArray = new GLuint[T_TRI*D_SIZE];

	
	// recuperation de la couleur courante
	vtkSmartPointer<vtkUnsignedCharArray> colors =
		vtkSmartPointer<vtkUnsignedCharArray>::New();
	colors->SetNumberOfComponents(4);				
	colors = (vtkUnsignedCharArray*)this->GetPointData()->GetScalars("RGB");

	bool bool_tags_activated = selected && g_tag_mode;

	bool bool_selected = (selected && (g_tag_mode == 0)) || (selected==0 && g_tag_mode);

	// recupération des matrices dans deux tableaux
	double M1[16]; double M2[16];//tableaux pour les points
	double M1n[9]; double M2n[9];//tableaux pour les normales
	Convert_GLMatrix_ToDouble(M1, M2, M1n, M2n);

	
	double p[4],n[4],c[4];
	if (!bool_selected){
		if (color[0] > 1 || color[2] > 1 || color[1] > 1){
			for (int u = 0; u < 4; u++){
				c[u] = color[u];
			}
		}
		else{
			for (int u = 0; u < 4; u++){
				c[u] = color[u]*255.;
			}
		}
	}
	else {
		for (int u = 0; u < 4; u++){
			c[u] = bone_ambuse[u]*255.;
		}
		c[3] = this->blend;
	}
	
	
	int ind=0;
	for (int i = 0; i<N_VERTS; i++){
		//Par défaut  la couleur sélectionnée 
		// Liste des couleurs
		if (bool_tags_activated){
			colors->GetTuple(i, c);
		}
		ObjectInfoArray[ind] = c[0]/255.; // red
		ObjectInfoArray[ind + 1] = c[1]/255.; // green
		ObjectInfoArray[ind + 2] = c[2]/255.; // blue
		ObjectInfoArray[ind + 3] = this->blend;// alpha

		//liste des normales
		norms->GetTuple(i, n);
		vtkMatrix3x3::PointMultiply(M1n, n, n);
		vtkMatrix3x3::PointMultiply(M2n, n, n);
		ObjectInfoArray[ind + 4] = n[0];
		ObjectInfoArray[ind + 5] = n[1];
		ObjectInfoArray[ind + 6] = n[2];

		//liste des points
		this->GetPoint(i, p);
		p[3] = 1;
		vtkMatrix4x4 ::PointMultiply(M1, p, p);
		vtkMatrix4x4 ::PointMultiply(M2, p, p);
		ObjectInfoArray[ind + 7] = p[0];
		ObjectInfoArray[ind + 8] = p[1];
		ObjectInfoArray[ind + 9] = p[2];

		ind += C_SIZE + 2*D_SIZE;
	}
	
	// Liste des triangles
	int ve[3], ind1 = 0;
	for (int i = 0; i<T_TRI; i++) {
		this->GetCellPoints(i, pointId);
		ve[0] = (int)pointId->GetId(0);
		ve[1] = (int)pointId->GetId(1);
		ve[2] = (int)pointId->GetId(2);
		ObjectIndiceArray[ind1] = (vtkIdType)ve[0];
		ObjectIndiceArray[ind1 + 1] = (vtkIdType)ve[1];
		ObjectIndiceArray[ind1 + 2] = (vtkIdType)ve[2];

		ind1 += 3;
	}
	size_indice = T_TRI * 3;
}

//construction du buffer
void OBJECT_MESH::Mesh_DrawObj_Building_Buff(){
	// Buffer d'informations de vertex
	glGenBuffers(1, &buf_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, buf_vertex);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(GLfloat) * size_info), ObjectInfoArray, GL_STATIC_DRAW);// si c'est un pointeur noté la taille du tableau directement.
	
	// Buffer d'indices
	glGenBuffers(1, &buf_indice);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_indice);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * size_indice, ObjectIndiceArray, GL_STATIC_DRAW);

}


//on dessine les listes du buffer
void OBJECT_MESH::Mesh_DrawObj_paintGL(){
	glBindBuffer(GL_ARRAY_BUFFER, buf_vertex);
	glVertexPointer(3, GL_FLOAT, ((int)(C_SIZE + 2 * D_SIZE)) * sizeof(float), ((float*)NULL + (C_SIZE + D_SIZE)));
	glNormalPointer(GL_FLOAT, ((int)(C_SIZE + 2 * D_SIZE)) * sizeof(float), ((float*)NULL + (C_SIZE)));
	glColorPointer(4, GL_FLOAT, ((int)(C_SIZE + 2 * D_SIZE)) * sizeof(float), 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_indice);
	
	// Activation d'utilisation des tableaux
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	// Rendu de notre géométrie
	glDrawElements(GL_TRIANGLES, size_indice, GL_UNSIGNED_INT, 0);
	
	// Desactivation des tableaux de sommets
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}


// suppression de l'objet tampon
void OBJECT_MESH::Mesh_DrawObj_delete(){
	free(ObjectInfoArray);
	free(ObjectIndiceArray);
	glDeleteBuffers(1, &buf_vertex);
	glDeleteBuffers(1, &buf_indice);

}

//On récupère les données
void OBJECT_MESH::Mesh_DrawObj_updateBuffer(){
	//VERTEX
	GLfloat * info_vbo = NULL;
	GLuint * indice_vbo = NULL;
	glBindBuffer(GL_ARRAY_BUFFER, buf_vertex);
	info_vbo = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

	if (info_vbo == NULL){
		cout << "ERROR, impossible d'acceder au données (info) du vbo!" << endl;
		return;
	}
	memcpy(info_vbo, ObjectInfoArray, size_info * sizeof*ObjectInfoArray); // copie le tableau dans le buffer 

	glUnmapBuffer(GL_ARRAY_BUFFER);
	info_vbo = NULL;



	//INDICE
	glBindBuffer(GL_ARRAY_BUFFER, buf_indice);
	indice_vbo = (GLuint *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

	if (indice_vbo == NULL){
		cout << "ERROR, impossible d'acceder au données (indice) du vbo!" << endl;
		return;
	}
	memcpy(indice_vbo, ObjectIndiceArray, size_indice* sizeof * ObjectIndiceArray);

	glUnmapBuffer(GL_ARRAY_BUFFER);
	indice_vbo = NULL;
}

//Dessine l'objet
void OBJECT_MESH::Mesh_DrawObj_VBO(bool bool_change_pos_obj)
{
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, noir);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, noir);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, noir);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.7);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat);
	glEnable(GL_COLOR_MATERIAL);// active le material noir au dessous 
	
	// initialisation du buffer
	if (bool_init_buf == 0){
		Mesh_DrawObj_init();
		bool_init_buf = 1;


		colorT = vtkSmartPointer<vtkUnsignedCharArray>::New();
		colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
	}
	
	
	if (this->view == 1){

		if (bool_change_pos_obj){// si on bouge que l'objet
			glPushMatrix();
			dmean = 1;
			c.x = c.y = c.z = 0;

			//récupérer le repère de la caméra les axes x , y, z
			uvx[0] = cos(PI*camera.az / 180.0)*cos(-PI*camera.el / 180.0);
			uvx[1] = sin(PI*camera.az / 180.0)*cos(-PI*camera.el / 180.0);
			uvx[2] = -sin(-PI*camera.el / 180.0);
			//"viewing" 0 1 0
			uvy[0] = cos(PI*camera.az / 180.0)*sin(-PI*camera.el / 180.0)*sin(-PI*camera.tw / 180.0) - sin(PI*camera.az / 180.0)*cos(-PI*camera.tw / 180.0);
			uvy[1] = sin(PI*camera.az / 180.0)*sin(-PI*camera.el / 180.0)*sin(-PI*camera.tw / 180.0) + cos(PI*camera.az / 180.0)*cos(-PI*camera.tw / 180.0);
			uvy[2] = cos(-PI*camera.el / 180.0)*sin(-PI*camera.tw / 180.0);
			//"viewing" 0 0 1
			uvz[0] = cos(PI*camera.az / 180.0)*sin(-PI*camera.el / 180.0)*cos(-PI*camera.tw / 180.0) + sin(PI*camera.az / 180.0)*sin(-PI*camera.tw / 180.0);
			uvz[1] = sin(PI*camera.az / 180.0)*sin(-PI*camera.el / 180.0)*cos(-PI*camera.tw / 180.0) - cos(PI*camera.az / 180.0)*sin(-PI*camera.tw / 180.0);
			uvz[2] = cos(-PI*camera.el / 180.0)*cos(-PI*camera.tw / 180.0);
			
			if (this->selected == 1)
			{
				glTranslated(tx2*uvx[0] + ty2*uvy[0] + tz2*uvz[0],
					tx2*uvx[1] + ty2*uvy[1] + tz2*uvz[1],
					tx2*uvx[2] + ty2*uvy[2] + tz2*uvz[2]);
				// Calculate new center of rotation:
				// Maintenant 
				//Rotation direct autour du centre de masse de l'échantillon entier!
				///rotation des objets sélectionnés par la souris -> l'objet lui meme que la camera (grille bouge pas)
				glTranslated(
					g_mean[0], // g_mean = barycentre des objets sélectionnés
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
			Mesh_DrawObj_paintGL();//on redessine l'objet
			glPopMatrix();
		}//fin bool
		else{
			
			changement =
				(modifTab->GetComponent(0, 0) != selected || modifTab->GetComponent(1, 0) != g_tag_mode || !IsEqual(color, prevColor, 4) || bool_changed_tags_color)
				|| ((dispmode == 4 && (cpt_dispmode4and5 == 1 || modifTab->GetComponent(2, 0) != dispmode)))
				|| ((dispmode == 5 && (cpt_dispmode4and5 == 1 || modifTab->GetComponent(2, 0) != dispmode)))
				|| ((dispmode == 1 && modifTab->GetComponent(2, 0) != dispmode))
				|| (modifTab->GetComponent(3, 1)==1)
				|| (bool_changed_matrix);
			
			// on fait une mise à jour de l'objet seulement si on a un changement
			if (changement)
			{
				colorT->Reset();
				colorT->SetNumberOfComponents(4);
				colorT->SetNumberOfTuples(N_VERTS);
				
				
				Mesh_DrawObj_modified_data(colorT);
				Mesh_DrawObj_updata(colorT);
				Mesh_DrawObj_updateBuffer();
			}

			//On dessine l'objet en fonction du mode d'affichage
			if (dispmode == 3){ // Wire + flat
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				Mesh_DrawObj_paintGL();
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				Mesh_DrawObj_paintGL();
			}
			else{
				Mesh_DrawObj_paintGL();
			}
		}
	}//fin if
}

//on modifie l'objet selon les données modifiées
void OBJECT_MESH::Mesh_DrawObj_modified_data(vtkSmartPointer<vtkUnsignedCharArray> colorT){

	///-------------------------- COULEUR
	if (modifTab->GetComponent(0, 0) != selected || modifTab->GetComponent(1, 0) != g_tag_mode 
		|| !IsEqual(color, prevColor, 4) || bool_changed_tags_color){
		
		modifTab->SetTuple2(0, selected,1);
		modifTab->SetTuple2(1, g_tag_mode,1);
		
		//selection ou deselection selon le mode
		if ((g_tag_mode == 0 && this->selected == 1) || (g_tag_mode == 1 && this->selected == 0) ){
			// on donne une couleur grise au "ColorT"à chaque point
			new_color[0] = bone_ambuse[0] * 255;
			new_color[1] = bone_ambuse[1] * 255;
			new_color[2] = bone_ambuse[2] * 255;
			for (int i = 0; i < colorT->GetNumberOfTuples(); i++){
				colorT->SetTuple(i, new_color);
			}
		}
		else if ((g_tag_mode == 1 && this->selected == 1)  || bool_changed_tags_color || bool_changed_init_color || (g_tag_mode == 0 && this->selected == 0 && Disp_Scalars_Mode)){
			// la couleur de normal(deselection) ou tags selon le mode(selection)
			bool_changed_tags_color = 0;
			bool_changed_init_color = 0;
			colors->Reset();
			colors->SetNumberOfComponents(4);
			colors->SetNumberOfTuples(N_VERTS);
			colors->DeepCopy( (vtkUnsignedCharArray*)this->GetPointData()->GetScalars("RGB"));
			
			for (int i = 0; i < colors->GetNumberOfTuples(); i++){
				colorT->SetTuple(i, colors->GetTuple(i));
			}
		}
		else if ((g_tag_mode == 0 && this->selected == 0 && Disp_Scalars_Mode==0) || (!IsEqual(color, prevColor, 4) && !IsEqual(color, bone_ambuse, 4) && !IsEqual(prevColor, bone_ambuse, 4))){
			// on verifie si on a changé la couleur de l'objet
			if (color[0] < 1 || color[2] < 1 || color[1] < 1){
				for (int u = 0; u < 4; u++){
					prevColor[u] = color[u];
					new_color[u] = color[u] * 255.;
				}
			}
			else{
				for (int u = 0; u < 4; u++){
					prevColor[u] = color[u];
					new_color[u] = color[u];
				}
			}

			for (int i = 0; i < colorT->GetNumberOfTuples(); i++){
				colorT->SetTuple(i, new_color);
			}
		}
	}


	///--------------------------------------TRIANGLES

	//Affichage des triangles  et change l'ordre des indices des triangles selon le mode d'affichage
	if ((dispmode == 4 && (cpt_dispmode4and5 == 1 || modifTab->GetComponent(2, 0) != dispmode)))
	{
		Mesh_Tri_Sort(0);
	}
	if ((dispmode == 5 && (cpt_dispmode4and5 == 1 || modifTab->GetComponent(2, 0) != dispmode)))
	{
		Mesh_Tri_Sort(1);
	}


	if (   ( dispmode == 4 && (cpt_dispmode4and5 == 1 || modifTab->GetComponent(2, 0) != dispmode) ) 
		|| ( dispmode == 5 && (cpt_dispmode4and5==1 || modifTab->GetComponent(2, 0) != dispmode)  )
		|| ( dispmode == 1 && modifTab->GetComponent(2, 0) != dispmode )    ){
		modifTab->SetTuple2(2, dispmode, 1);
		cpt_dispmode4and5 = 0;
		draw_tab_indice = new int[T_TRI * 3];
		for (int i = 0; i < T_TRI; i++) {
			if (this->tr_sort == NULL)
			{
				draw_tab_indice[i] = i;
			}
			else
			{
				draw_tab_indice[i] = tr_sort[i]; // triangles triés en fct de l'orientation de l'écran
			}
		}
	}

	///----------------- CHANGEMENT DES MATRICES POUR LES POINTS ou MODIFICATION DES POINTS
	if (bool_changed_matrix){
		bool_changed_matrix = 0;
		modifTab->SetComponent(3, 1, 1);
	}


}

// Mise à jour des listes du buffer
void OBJECT_MESH::Mesh_DrawObj_updata(vtkSmartPointer<vtkUnsignedCharArray> colorT){
	if (modifTab->GetComponent(0, 1) == 1){// selection a changé
		modifTab->SetComponent(0, 1, 0);
		//---------------------COULEUR
		if (modifTab->GetComponent(1, 1) == 1){// couleur a changé
			modifTab->SetComponent(1, 1, 0);
			double p[3], n[3], c[4];
			int ind = 0;
			
			for (int i = 0; i<N_VERTS; i++){
				//Par défaut  la couleur sélectionnée 
				// Liste des couleurs
				colorT->GetTuple(i, c);
				ObjectInfoArray[ind] = c[0] / 255.; // red
				ObjectInfoArray[ind + 1] =  c[1] / 255.; // green
				ObjectInfoArray[ind + 2] =  c[2] / 255.; // blue
				ObjectInfoArray[ind + 3] =  this->blend;// alpha

				ind += C_SIZE + 2 * D_SIZE;
			}
		}
	}
	
	
	///-------------------UPDATE TRIANGLE
	if (modifTab->GetComponent(2, 1) == 1){
		modifTab->SetComponent(2, 1, 0);
		vtkSmartPointer<vtkIdList> pointId = vtkSmartPointer<vtkIdList>::New();
		int ve[3], ind3 = 0;
		for (int i = 0; i<T_TRI; i++) {
			this->GetCellPoints(draw_tab_indice[i], pointId);
			ve[0] = (int)pointId->GetId(0);
			ve[1] = (int)pointId->GetId(1);
			ve[2] = (int)pointId->GetId(2);
			ObjectIndiceArray[ind3] = (vtkIdType)ve[0];
			ObjectIndiceArray[ind3 + 1] = (vtkIdType)ve[1];
			ObjectIndiceArray[ind3 + 2] = (vtkIdType)ve[2];
			pointId = vtkSmartPointer<vtkIdList>::New();
			ind3 += 3;
		}
		free(draw_tab_indice);
	}

	
	//----------------------UPDATE POINTS
	if (modifTab->GetComponent(3, 1) == 1){
		modifTab->SetComponent(3, 1, 0);
		//recupère les normales
		vtkSmartPointer<vtkFloatArray> norms = vtkSmartPointer<vtkFloatArray>::New();
		norms = vtkFloatArray::SafeDownCast(this->GetPointData()->GetNormals());

		// recupère les deux matrices   ((mettre dans une fonction ::: apparait deux fois)) // à arranger
		double M1[16]; double M2[16];//pour les points
		double M1n[9]; double M2n[9];//pour les normales
		
		Convert_GLMatrix_ToDouble(M1, M2, M1n, M2n);
		
		double p[4], n[4], c[4];
		int ind = 0;
		for (int i = 0; i<N_VERTS; i++){
			//liste des normales
			norms->GetTuple(i, n);
			vtkMatrix3x3::PointMultiply(M1n, n, n);
			vtkMatrix3x3::PointMultiply(M2n, n, n);
			ObjectInfoArray[ind + 4] = n[0];
			ObjectInfoArray[ind + 5] = n[1];
			ObjectInfoArray[ind + 6] = n[2];

			//liste des points
			this->GetPoint(i, p);
			p[3] = 1;
			vtkMatrix4x4::PointMultiply(M1, p, p);
			vtkMatrix4x4::PointMultiply(M2, p, p);
			ObjectInfoArray[ind + 7] = p[0];
			ObjectInfoArray[ind + 8] = p[1];
			ObjectInfoArray[ind + 9] = p[2];

			ind += C_SIZE + 2 * D_SIZE;
		}
	}
}



void OBJECT_MESH::Mesh_DrawObj(int level, bool bool_change_pos_obj,bool bool_vbo){
	
	if (bool_vbo){
		Mesh_DrawObj_VBO(bool_change_pos_obj);
	}
	else{
		if (bool_init_buf == 1){
			Mesh_DrawObj_delete();
			bool_init_buf = 0;
		}

		modifTab->SetTuple2(0, selected, 0);
		modifTab->SetTuple2(1, g_tag_mode, 0);
		modifTab->SetTuple2(2, dispmode, 0);
		modifTab->SetTuple2(3, bool_changed_matrix, 0);

		Mesh_DrawObj_imediateFunct(level);
	}
	
	//on passe à l'objet suivant
	if (nextobj != NULL)
	{
		nextobj->Mesh_DrawObj(level,bool_change_pos_obj,bool_vbo);
	}
}

void OBJECT_MESH::Mesh_DrawObj_imediateFunct(int level){
	if (this->view ==1)
	{
	GLfloat cv[3][4];
	double curr_scalar1,curr_scalar2,curr_scalar3;
	
	// mode 4 or 5 = real time rendering sorting!
	 if (dispmode ==4)
	 {Mesh_Tri_Sort(0);}
	if (dispmode ==5)
	 {Mesh_Tri_Sort(1);}

 
	//method : 0 for landmarks
	//method : 1 for free form objects
 		int i;
  //glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glEnable ( GL_LIGHTING ) ;
 
	    glDisable(GL_COLOR_MATERIAL); // Activé : couleur = glcolor
									// Désactivé : couleur = glmaterial


  VERTEX2 c;
  dmean = 1;
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
	    	
		 vtkSmartPointer<vtkUnsignedCharArray> colors =
				   vtkSmartPointer<vtkUnsignedCharArray>::New();
				   colors->SetNumberOfComponents(3);				
				   colors=(vtkUnsignedCharArray*)this->GetPointData()->GetScalars("RGB");      
	
			glPushMatrix();
			//float newmean1[3],newmean[3];
			if (this->selected == 1 && level == 0)
			{
				glTranslated(tx2*uvx[0] + ty2*uvy[0]+ tz2*uvz[0],
				tx2*uvx[1] + ty2*uvy[1]+ tz2*uvz[1],
				tx2*uvx[2] + ty2*uvy[2]+ tz2*uvz[2]);
			    // Calculate new center of rotation:


				/*
				Initialement (avant existence d'un g_mean = barycentre des objets sélectionnés
				ApplyTransformation(this->mean, newmean, this->Mat2); // Position												
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
				//ApplyTransformation(g_mean, newmean, this->Mat2); // Position
			// Maintenant 
				//Rotation direct autour du centre de masse de l'échantillon entier!
					
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
			
			glMultMatrixf((GLfloat *)this->Mat2); //Position
			
																		
						glTranslated(
					this->mean[0],
					this->mean[1],
					this->mean[2]
					);		
			glMultMatrixf((GLfloat *)this->Mat1); //Aspect
			glTranslated(
					-this->mean[0],
					-this->mean[1],
					-this->mean[2]
					);	
			
			
			
	//	if (this->draw_texture==0)
	//{
	
  if ((Disp_Vertices_Ids ==1)
	  //&& scalars_vertices!=NULL
	  )
    {

			
		  glMaterialfv(GL_FRONT,GL_AMBIENT,rouge);
		  glMaterialfv(GL_FRONT,GL_DIFFUSE,rouge);
		  glMaterialfv(GL_FRONT,GL_SPECULAR,rouge);
		  glMaterialfv(GL_FRONT,GL_SHININESS,rouge);
		  glMaterialfv(GL_FRONT,GL_EMISSION,rouge);
		  double v[3];
		  double *vn;
		  
		vtkFloatArray* 	norms = vtkFloatArray::SafeDownCast
				(this->GetPointData()->GetNormals());

		  for ( i=0;i<this->numvert;i++)
			{	
				this->GetPoint((vtkIdType)i, v);
				vn=norms->GetTuple((vtkIdType)i);
			  glRasterPos3f(v[0]+(vn[0]*this->dmean/100)
					, v[1]+(vn[1]*this->dmean/100)
					, v[2]+(vn[2]*this->dmean/100));
			  printString(itoa( i,  10 )); 
			}




    }
  if (Disp_Triangle_Ids ==1)
    {	
		
		  	 float mean[3];
			  int ve1, ve2, ve3;
				

			vtkSmartPointer<vtkIdList> points =vtkSmartPointer<vtkIdList>::New();
			double v1[3],v2[3],v3[3];			
			vtkFloatArray* norms = vtkFloatArray::SafeDownCast(this->GetCellData()->GetNormals());			
	
			double *vn;
		
		  glMaterialfv(GL_FRONT,GL_AMBIENT,vert);
		  glMaterialfv(GL_FRONT,GL_DIFFUSE,vert);
		  glMaterialfv(GL_FRONT,GL_SPECULAR,vert);
		 glMaterialfv(GL_FRONT,GL_SHININESS,vert);
		 glMaterialfv(GL_FRONT,GL_EMISSION,vert);
    	
		
			
			
			for ( i=0;i<this->numtri;i++) {
			// for every triangle 
			//if (i%200 ==0)
			//{
				this->GetCellPoints (i, points);
				vn=norms->GetTuple((vtkIdType)i);

				ve1=(int)points->GetId(0);
				ve2=(int)points->GetId(1);
				ve3=(int)points->GetId(2);
			
				this->GetPoint((vtkIdType)ve1, v1);
				this->GetPoint((vtkIdType)ve2, v2);
				this->GetPoint((vtkIdType)ve3, v3);	 
				  
				  // for every triangle 
				  mean[0] = (float)(v1[0]+v2[0]+v3[0])/3;
				  mean[1] = (float)(v1[1]+v2[1]+v3[1])/3;
				  mean[2] =(float)(v1[2]+v2[2]+v3[2])/3;
				  
				  glRasterPos3f(mean[0]+(vn[0]*this->dmean/100)
						, mean[1]+(vn[1]*this->dmean/100)
						, mean[2]+(vn[2]*this->dmean/100));
				  printString(itoa( i,  10 ));
			//}
		}
			
	
    }

int draw_sc=0;
//glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,blanc);
if (level == 0) //Only if basal objects... otherwise they are grouped.
		{
			
			//if (this->selected ==1)//Ungrouped object
			if ((g_tag_mode == 0 && this->selected == 1) || (g_tag_mode == 1 && this->selected == 0))//Ungrouped object
			{
				bone_ambuse[3] = this->blend;

				glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,bone_ambuse);
				glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,bone_ambuse);
				glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,bone_specular);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,bone_shininess);
				glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,no_mat);
				bool_changed_init_color = 0;
	
			}
			else
			{
				draw_sc=1;
				//glColor4fv((GLfloat*)this->color);
				float c[4];

				if (color[0] > 1 || color[1] > 1 || color[2] > 1)
				{
					for (int j = 0; j < 3; j++)
						c[j] = color[j]/255;
				}
				else{
					for (int j = 0; j < 3; j++)
						c[j] = color[j];
				}
				c[3] = this->blend;
				
				glMaterialfv(GL_FRONT,GL_AMBIENT,c);
				glMaterialfv(GL_FRONT,GL_DIFFUSE,c);
				glMaterialfv(GL_FRONT,GL_SPECULAR,c);
				glMaterialfv(GL_FRONT,GL_SHININESS,bone_shininess);
				glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);	

				glMaterialfv(GL_BACK,GL_AMBIENT,rose);
				glMaterialfv(GL_BACK,GL_DIFFUSE,rose);
				glMaterialfv(GL_BACK,GL_SPECULAR,bone_specular);
				glMaterialfv(GL_BACK,GL_SHININESS,rose);
				glMaterialfv(GL_BACK,GL_EMISSION,no_mat);
				
			}
			
				
		}


			//glShadeModel(GL_SMOOTH);


	    if ((rotx==0 && roty==0 && rotz ==0 && tx  ==0 && ty ==0 && tz==0 
			&& rotx2==0 && roty2 ==0 && rotz2 ==0 && tx2==0 && ty2==0 && tz2==0
			&& g_zoom ==0)||g_display_all ==1
			)
		{
			if (dispmode ==1 || dispmode ==4 || dispmode ==5)//AFFICHAGE "NORMAL" = triangles + normales sur les points
			{
				
				if (g_enable_2sides ==1)
				{
					glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
				}
				else
				{
					glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
				}

				
				int ve1, ve2, ve3;								

				//std::cout << "\n Draw: start";
				vtkSmartPointer<vtkFloatArray> norms = 
				vtkSmartPointer<vtkFloatArray>::New();

				norms = vtkFloatArray::SafeDownCast
					(this->GetPointData()->GetNormals());
				
				vtkSmartPointer<vtkIdList> points =vtkSmartPointer<vtkIdList>::New();
				double v[3];
				double *vn;
				double vn2[3];

				
				glBegin(GL_TRIANGLES);
				//std::cout << "\n Draw: current number of norms :"<<norms->GetNumberOfTuples()
				//<<"\n Draw current object number of points:"<<this->GetNumberOfPoints();
				//vtkFloatArray *currentScalars;
				//currentScalars = (vtkFloatArray*)this->GetPointData()->GetScalars("Depth");


				//currentScalars = (vtkFloatArray*)this->GetPointData()->GetScalars("RGB");

				for ( i=0;i<this->numtri;i++) {
					// for every triangle 
					if (this->tr_sort==NULL)
					{
						this->GetCellPoints (i, points);
					}
					else
					{
						this->GetCellPoints (tr_sort[i], points);
					}
					
					
					ve1=(int)points->GetId(0);
					ve2=(int)points->GetId(1);
					ve3=(int)points->GetId(2);
					
					if (((Disp_Scalars_Mode == 1 || Disp_Tags_Mode == 1) && /*(currentScalars != NULL) && */draw_sc == 1) || bool_changed_init_color == 1)
					{
						/*curr_scalar1=currentScalars->GetTuple(ve1)[0];
						curr_scalar2=currentScalars->GetTuple(ve2)[0];
						curr_scalar3=currentScalars->GetTuple(ve3)[0];*/
						
						//ConvertScalarToColor((float)curr_scalar1, cv[0], this->blend);		
						//ConvertScalarToColor((float)curr_scalar2, cv[1], this->blend);					
						//ConvertScalarToColor((float)curr_scalar3, cv[2], this->blend);
					    if (colors !=NULL)
						{
							cv[0][0] = colors->GetTuple(ve1)[0]/ 255;
							cv[0][1] = colors->GetTuple(ve1)[1]/ 255;
							cv[0][2] = colors->GetTuple(ve1)[2]/ 255;
							cv[0][3] = this->blend; //colors->GetTuple(ve1)[3] / 255;
							//cv[0][3] = 0.5;
							//cv[0][3] = this->blend;
							cv[1][0] = colors->GetTuple(ve2)[0]/ 255;
							cv[1][1] = colors->GetTuple(ve2)[1]/255;
							cv[1][2] = colors->GetTuple(ve2)[2]/ 255;
							cv[1][3] = this->blend; //colors->GetTuple(ve2)[3] / 255;
							//cv[1][3] = 0.5;
							//cv[1][3] = this->blend;
							cv[2][0] = colors->GetTuple(ve3)[0]/255;
							cv[2][1] = colors->GetTuple(ve3)[1]/ 255;
							cv[2][2] = colors->GetTuple(ve3)[2]/255;
							cv[2][3] = this->blend;// colors->GetTuple(ve3)[3] / 255;
							//cv[2][3] = 0.5;
							//cv[2][3] = this->blend;	
						}
						else
						{
							cv[0][0] = 0;
							cv[0][1] = 1;
							cv[0][2] = 0;
							cv[0][3] = this->blend;
							cv[1][0] = 0;
							cv[1][1] = 1;
							cv[1][2] = 0;
							cv[1][3] = this->blend;
							cv[2][0] = 0;
							cv[2][1] = 1;
							cv[2][2] = 0;
							cv[2][3] = this->blend;
						}
					}
				
						
					//Point 3
					if (((Disp_Scalars_Mode == 1 || Disp_Tags_Mode == 1) && /*(currentScalars != NULL) && */draw_sc == 1) || bool_changed_init_color == 1)
					{
						//glColor4fv((GLfloat*)cv[2]);				
						glMaterialfv(GL_FRONT,GL_AMBIENT,(GLfloat*)cv[2]);
						glMaterialfv(GL_FRONT,GL_DIFFUSE,(GLfloat*)cv[2]);
						glMaterialfv(GL_FRONT,GL_SPECULAR,(GLfloat*)cv[2]);
						glMaterialfv(GL_FRONT,GL_SHININESS,bone_shininess);
						glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);
					}
						
					this->GetPoint((vtkIdType)ve3, v);
					vn=norms->GetTuple((vtkIdType)ve3);
					vn2[0] = vn[0];
					vn2[1] = vn[1];
					vn2[2] = vn[2];
					glNormal3dv(
						(GLdouble*)&vn2);
					
					glVertex3dv(
					(GLdouble*)&v
					);
					//Point 2
					if (((Disp_Scalars_Mode == 1 || Disp_Tags_Mode == 1) /*&& (currentScalars != NULL)*/ && draw_sc == 1) || bool_changed_init_color == 1)
					{
						//glColor4fv((GLfloat*)cv[1]);
						glMaterialfv(GL_FRONT,GL_AMBIENT,(GLfloat*)cv[1]);
						glMaterialfv(GL_FRONT,GL_DIFFUSE,(GLfloat*)cv[1]);
						glMaterialfv(GL_FRONT,GL_SPECULAR,(GLfloat*)cv[1]);
						glMaterialfv(GL_FRONT,GL_SHININESS,bone_shininess);
						glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);
					}

					
					this->GetPoint((vtkIdType)ve2, v);
					vn=norms->GetTuple((vtkIdType)ve2);
					
					vn2[0] = vn[0];
					vn2[1] = vn[1];
					vn2[2] = vn[2];
					glNormal3dv(
						(GLdouble*)&vn2);
					glVertex3dv(
						(GLdouble*)&v
					); 
				
					//Point 1
					if (((Disp_Scalars_Mode == 1 || Disp_Tags_Mode == 1) /*&& (currentScalars != NULL)*/ && draw_sc == 1 ) || bool_changed_init_color == 1)
					{
						//glColor4fv((GLfloat*)cv[0]);				
						glMaterialfv(GL_FRONT,GL_AMBIENT,(GLfloat*)cv[0]);
						glMaterialfv(GL_FRONT,GL_DIFFUSE,(GLfloat*)cv[0]);
						glMaterialfv(GL_FRONT,GL_SPECULAR,(GLfloat*)cv[0]);
						glMaterialfv(GL_FRONT,GL_SHININESS,bone_shininess);
						glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);
					}
				
					vn=norms->GetTuple((vtkIdType)ve1);			
					this->GetPoint((vtkIdType)ve1, v);
						
					vn2[0] = vn[0];
					vn2[1] = vn[1];
					vn2[2] = vn[2];
					glNormal3dv(
					(GLdouble*)&vn2);
					
					
					glVertex3dv(
						(GLdouble*)&v
					); 
				}
					
				glEnd();
					//glDisable(GL_COLOR_MATERIAL);
			}

		if (dispmode ==2 || dispmode==3) // Draw Flat triangles. Mode 2 = flat triangles only
										// Mode 3 = wireframe + flat triangles
		{
			if (g_enable_2sides ==1)
			{
				glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
			}
			else
			{
				glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
			}

		  glBegin(GL_TRIANGLES);
		  int ve1, ve2, ve3;
		  vtkFloatArray* norms = vtkFloatArray::SafeDownCast(this->GetCellData()->GetNormals());			
		  vtkSmartPointer<vtkIdList> points =vtkSmartPointer<vtkIdList>::New();
		  double v1[3],v2[3],v3[3];
		  double *vn;
		  double vn2[3];


		  //vtkFloatArray *currentScalars;
		  ////currentScalars = (vtkFloatArray*)this->GetPointData()->GetScalars("Depth");
		  //currentScalars = (vtkFloatArray*)this->GetPointData()->GetScalars("RGB");

		  for ( i=0;i<this->numtri;i++) {
		// for every triangle 
		  if (this->tr_sort==NULL)
			{
				this->GetCellPoints (i, points);
			}
			else
			{
				this->GetCellPoints (tr_sort[i], points);
			}
			ve1=(int)points->GetId(0);
			ve2=(int)points->GetId(1);
			ve3=(int)points->GetId(2);		

			if (((Disp_Scalars_Mode == 1 || Disp_Tags_Mode == 1) && /*(currentScalars != NULL) &&*/ draw_sc == 1) || bool_changed_init_color == 1)
			{
				//ConvertScalarToColor((float)currentScalars->GetTuple(ve1)[0], cv[0], this->blend);
				//ConvertScalarToColor((float)currentScalars->GetTuple(ve2)[0], cv[1], this->blend);
				//ConvertScalarToColor((float)currentScalars->GetTuple(ve3)[0], cv[2], this->blend);	
				 if (colors !=NULL)
							{
								cv[0][0] = colors->GetTuple(ve1)[0]/255;
								cv[0][1] = colors->GetTuple(ve1)[1]/ 255;
								cv[0][2] = colors->GetTuple(ve1)[2]/ 255;
								cv[0][3] = this->blend;
								cv[1][0] = colors->GetTuple(ve2)[0]/ 255;
								cv[1][1] = colors->GetTuple(ve2)[1]/ 255;
								cv[1][2] = colors->GetTuple(ve2)[2]/ 255;
								cv[1][3] = this->blend;
								cv[2][0] = colors->GetTuple(ve3)[0]/ 255;
								cv[2][1] = colors->GetTuple(ve3)[1]/ 255;
								cv[2][2] = colors->GetTuple(ve3)[2]/ 255;
								cv[2][3] = this->blend;
								
							}
							else
							{
								cv[0][0] = 0;
								cv[0][1] = 1;
								cv[0][2] = 0;
								cv[0][3] = this->blend;
								cv[1][0] = 0;
								cv[1][1] = 1;
								cv[1][2] = 0;
								cv[1][3] = this->blend;
								cv[2][0] = 0;
								cv[2][1] = 1;
								cv[2][2] = 0;
								cv[2][3] = this->blend;
	
							}
			}
			

			this->GetPoint((vtkIdType)ve1, v1);
			this->GetPoint((vtkIdType)ve2, v2);
			this->GetPoint((vtkIdType)ve3, v3);				
			
			
			vn=norms->GetTuple((vtkIdType)i);
			//
			vn2[0] = vn[0];
			vn2[1] = vn[1];
			vn2[2] = vn[2];
			glNormal3dv(
				(GLdouble*)&vn2);
			if (((Disp_Scalars_Mode == 1 || Disp_Tags_Mode == 1) /*&& (currentScalars != NULL)*/ && draw_sc == 1) || bool_changed_init_color == 1)
			{
				glMaterialfv(GL_FRONT,GL_AMBIENT,(GLfloat*)cv[2]);
				glMaterialfv(GL_FRONT,GL_DIFFUSE,(GLfloat*)cv[2]);
				glMaterialfv(GL_FRONT,GL_SPECULAR,(GLfloat*)cv[2]);
				glMaterialfv(GL_FRONT,GL_SHININESS,bone_shininess);
				glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);
			}
		
			glVertex3dv((GLdouble*)&v3); // point				
			if (((Disp_Scalars_Mode == 1 || Disp_Tags_Mode == 1) /*&& (currentScalars != NULL)*/ && draw_sc == 1) || bool_changed_init_color == 1)
			{				
				glMaterialfv(GL_FRONT,GL_AMBIENT,(GLfloat*)cv[1]);
				glMaterialfv(GL_FRONT,GL_DIFFUSE,(GLfloat*)cv[1]);
				glMaterialfv(GL_FRONT,GL_SPECULAR,(GLfloat*)cv[1]);
				glMaterialfv(GL_FRONT,GL_SHININESS,bone_shininess);
				glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);
			}
		
			glVertex3dv((GLdouble*)&v2); // point				
			if (((Disp_Scalars_Mode == 1 || Disp_Tags_Mode == 1) /*&& (currentScalars != NULL) */&& draw_sc == 1) || bool_changed_init_color == 1)
			{
				glMaterialfv(GL_FRONT,GL_AMBIENT,(GLfloat*)cv[0]);
				glMaterialfv(GL_FRONT,GL_DIFFUSE,(GLfloat*)cv[0]);
				glMaterialfv(GL_FRONT,GL_SPECULAR,(GLfloat*)cv[0]);
				glMaterialfv(GL_FRONT,GL_SHININESS,bone_shininess);
				glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);
			}
		
			glVertex3dv((GLdouble*)&v1); // point
			


					


																	
		  }
		  glEnd();
		}

		//} 
		
			if (dispmode ==0 || dispmode ==3) //WIREFRAME ONLY : 0  WIREFRAME + FLAT TRIANGLES : 3
			{
				int nm=1;
				if (dispmode==3){nm=-1;}
				int ve1, ve2, ve3;
				//float coo[3], coo2[3], bari[3];
				vtkFloatArray* norms = vtkFloatArray::SafeDownCast
				(this->GetPointData()->GetNormals());			

				vtkSmartPointer<vtkIdList> points =vtkSmartPointer<vtkIdList>::New();
				double v1[3],v2[3],v3[3];
				double *vn;
				double vn2[3];

				glLineWidth(2);
				for ( i=0;i<this->numtri;i++) {
				// for every triangle 

				this->GetCellPoints (i, points);
				
				ve1=(int)points->GetId(0);
				ve2=(int)points->GetId(1);
				ve3=(int)points->GetId(2);
				
		  
				
				this->GetPoint((vtkIdType)ve1, v1);
				this->GetPoint((vtkIdType)ve2, v2);
				this->GetPoint((vtkIdType)ve3, v3);				
				
				glBegin(GL_LINES);													
				glVertex3dv((GLdouble*)&v1); // point1
				glVertex3dv((GLdouble*)&v2);  // point2
				
				vn=norms->GetTuple((vtkIdType)ve1);
				vn2[0] = vn[0]*nm;
				vn2[1] = vn[1]*nm;
				vn2[2] = vn[2]*nm;

				glNormal3dv((GLdouble*)&vn2); //  normal gouraud shading
				glEnd();

				glBegin(GL_LINES);
				glVertex3dv((GLdouble*)&v2);  // point2
				glVertex3dv((GLdouble*)&v3);  // point3
				vn=norms->GetTuple((vtkIdType)ve2);
				vn2[0] = vn[0]*nm;
				vn2[1] = vn[1]*nm;
				vn2[2] = vn[2]*nm;

				glNormal3dv((GLdouble*)&vn2); //  normal gouraud shading
				glEnd();
				glBegin(GL_LINES);
				glVertex3dv((GLdouble*)&v3);  // point3
				glVertex3dv((GLdouble*)&v1);  // point1
				vn=norms->GetTuple((vtkIdType)ve3);
				vn2[0] = vn[0]*nm;
				vn2[1] = vn[1]*nm;
				vn2[2] = vn[2]*nm;

				glNormal3dv((GLdouble*)&vn2); //  normal gouraud shading
				glEnd();
				

			}
		}
	}//rot x rot y rot z ==0!
	else // fast point rendering
	{
		 double v[3];
		 double *vn;
		double vn2[3];
		 glBegin(GL_POINTS);
		 int dec=2;
		/* if (this->numvert>200)
		  {dec=10;}
		 if (this->numvert>2000)
		  {dec=100;}
		 if (this->numvert>20000)
		  {dec=250;}
		 if (this->numvert>200000)
		  {dec=2000;}*/
		 /*if (this->numvert>1000000)
		  {dec=10000;}*/
		vtkSmartPointer<vtkFloatArray> norms = 
				vtkSmartPointer<vtkFloatArray>::New();

		norms = vtkFloatArray::SafeDownCast
			(this->GetPointData()->GetNormals());
		
		
		/*vtkFloatArray *currentScalars;				
		currentScalars = (vtkFloatArray*)this->GetPointData()->GetScalars();*/
		 
		vtkSmartPointer<vtkIdList> points =vtkSmartPointer<vtkIdList>::New();
			

		for ( i=0;i<this->numtri;i++) 
		{
			// for every triangle 
			if (this->tr_sort==NULL)
			{
					this->GetCellPoints (i, points);
			}
			else
			{
				this->GetCellPoints (tr_sort[i], points);
			}
					
					
			vtkIdType ve=(int)points->GetId(0);
			 if (ve%dec ==0)
			
			{
				if (((Disp_Scalars_Mode == 1 || Disp_Tags_Mode == 1) /*&& (currentScalars != NULL)*/ && draw_sc == 1) || bool_changed_init_color == 1)
					{
						/*curr_scalar1=currentScalars->GetTuple(ve)[0];	*/										
						//ConvertScalarToColor((float)curr_scalar1, cv[0], this->blend);
						 if (colors !=NULL)
							{
								cv[0][0] = colors->GetTuple(ve)[0]/255;
								cv[0][1] = colors->GetTuple(ve)[1]/255;
									cv[0][2] = colors->GetTuple(ve)[2]/255;
								cv[0][3] = this->blend;
								
								
							}
							else
							{
								cv[0][0] = 0;
								cv[0][1] = 1;
								cv[0][2] = 0;
								cv[0][3] = this->blend;
								
	
							}
					}				
											
				if (((Disp_Scalars_Mode == 1 || Disp_Tags_Mode == 1) /*&& (currentScalars != NULL)*/ && draw_sc == 1) || bool_changed_init_color == 1)
					{
						glColor4fv((GLfloat*)cv[0]);				
						glMaterialfv(GL_FRONT,GL_AMBIENT,(GLfloat*)cv[0]);
						glMaterialfv(GL_FRONT,GL_DIFFUSE,(GLfloat*)cv[0]);
						glMaterialfv(GL_FRONT,GL_SPECULAR,(GLfloat*)cv[0]);
						glMaterialfv(GL_FRONT,GL_SHININESS,bone_shininess);
						glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);
					}
					vn=norms->GetTuple((vtkIdType)ve);
					vn2[0] = vn[0];
					vn2[1] = vn[1];
					vn2[2] = vn[2];
					glNormal3dv(
						(GLdouble*)&vn2);

	 				this->GetPoint(ve, v);
					glVertex3dv((GLdouble*)&v);  
			 }


		}
		/* for (vtkIdType ve=0;ve<this->numvert;ve++) 
		 {		

				

			    if (ve%dec ==0)
				{


					if ((Disp_Scalars_Mode == 1 || Disp_Tags_Mode == 1)&&(currentScalars!=NULL) && draw_sc==1)
					{
						curr_scalar1=currentScalars->GetTuple(ve)[0];											
						ConvertScalarToColor((float)curr_scalar1, cv[0], this->blend);															
					}				
											
					if ((Disp_Scalars_Mode == 1 || Disp_Tags_Mode == 1)&&(currentScalars!=NULL) && draw_sc == 1 )
					{
						glColor4fv((GLfloat*)cv[0]);				
						glMaterialfv(GL_FRONT,GL_AMBIENT,(GLfloat*)cv[0]);
						glMaterialfv(GL_FRONT,GL_DIFFUSE,(GLfloat*)cv[0]);
						glMaterialfv(GL_FRONT,GL_SPECULAR,(GLfloat*)cv[0]);
						glMaterialfv(GL_FRONT,GL_SHININESS,bone_shininess);
						glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);
					}
					vn=norms->GetTuple((vtkIdType)ve);
					vn2[0] = vn[0];
					vn2[1] = vn[1];
					vn2[2] = vn[2];
					glNormal3dv(
						(GLdouble*)&vn2);

		 			this->GetPoint(ve, v);
					glVertex3dv((GLdouble*)&v);  										
				}
		 }*/
		 glEnd( );
		
	}
	

	

	glPopMatrix();
		
	
	}//if view =1
}
