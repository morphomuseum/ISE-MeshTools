#include "Struct.h"

#include <iostream>
//#include <string.h>
 _CameraT camera;			
 float rotx, roty, rotz, rotx2, roty2, rotz2,tx,ty,tz, tx2, ty2, tz2;
 float g_zoom;
 int dispmode;
 float g_mean[3];
 int g_nmean;
 int Disp_Vertices_Ids;
int Disp_Triangle_Ids;
int Disp_Scalars_Mode;
int Disp_Tags_Mode;

float Min_Color;
float Max_Color;
float g_landmark_size;
int g_display_all;
int g_grid_plane;
int g_landmark_type;
int g_tag_extraction_criterion_all;
int g_move_cm;
int g_sc_show_below_min;
int g_sc_show_above_max;
int g_enable_2sides;
int g_tag_mode;
int g_pencil_extension;
int g_tag_tool;
int g_magic_wand_extension;

double g_magic_wand_extension_min_cos;
int g_magic_wand_over;
int g_mode_tool;
int g_auto_zoom;
int g_color_scale_id;
int g_auto_delete;
std::vector<std::string> g_scalar_list;
int g_active_scalar;
std::vector<std::string> g_scalar_list_selected;

std::vector<std::string> g_selected_names;

std::vector<std::string> g_distinct_selected_names;
char Version[50];
std::string g_tag_labels[25];


//int landmark_mode=0;
float g_lambda;
POLYGON_LIST poly;
GLfloat color_obj[4];
GLfloat g_tag_colors[25][4];
int g_active_tag;

char*   ftoa( double f, double sigfigs )
    {
    char a[81];
    int prec, width, front;

    front = (f==0)? 1 : (int)log10(fabs(f))+1;
    if ( sigfigs < 1.0 && sigfigs >= 0.0 )  // fit number to tolerance
        {
        double rem = fabs(f) - int(f);
        prec=0;
        int num = (int)rem;
        while ( rem*pow((float)10,prec) - num > sigfigs )
            num = int(rem*pow((float)10,++prec));
        width = front;
        sprintf_s(a, "%#*.*f", width, prec, f );
        }
    else
        {
        if ( sigfigs < 2.0 ) sigfigs = 2.0;
            
        if ( front > (int)sigfigs )
            {
            sprintf_s( a, "%#.*e", (int)sigfigs-1, f );
            }
        else
            {
            prec = (int)sigfigs - front;
            if ( f==0.0 ) width = 2;
            else width = front + prec + 1;
            sprintf_s( a, "%#*.*f", width, prec, f );
            }
        }
    return strdup(a);
    }
char* itoa(int val, int base){
		static char buf[32] = {0};
		int i = 30;
		for(; val && i ; --i, val /= base)
			buf[i] = "0123456789abcdef"[val % base];
		return &buf[i+1];
}

int  landmark_list_compare (const void* v1, const void* v2)
{
	int ind1=landmark_listptr(v1)->obj_p;	// points to first coordinate of vertex v1
    int ind2=landmark_listptr(v2)->obj_p;	// points to first coordinate of vertex v1
	//ind1 = ind2;
    return ind1 -ind2;    
}

int tri_depth_compare(const void* v1, const void* v2)
{
	float v = tri_vptr(v1)->d;
	float w = tri_vptr(v2)-> d;
	if (v>w)
		return 1;
	else if (w>v)
		return -1;
	else return 0;

}


int scalar_compare(const void* v1, const void* v2)
{
	float v = sc_vptr(v1)->v;
	float w = sc_vptr(v2)-> v;
	if (v>w)
		return 1;
	else if (w>v)
		return -1;
	else return 0;

}

void ConvertScalarToColor(float value, GLfloat color[4], float alpha)
{
	


	
	float valcol;
	if (g_color_scale_id ==0)
	{
		// Five color scale
		float Min1, Min2, Min3;
		Min2 = (Min_Color + Max_Color)/2;
		Min1 = (Min_Color + Min2)/2;
		Min3 = (Max_Color + Min2 )/2;

		//Blue is below
		if (value < Min_Color)
		{color[0] = 0;	color[1] = 0;	color[2] = 1;	
		
			if (g_sc_show_below_min == 1)
			{
				color[3] = alpha;
			}
			else
			{
				color[3] = 0.0;
			}
		}
		// Beween blue and cyan
		else if (value <Min1)
		{valcol = (value -Min_Color)/(Min1 - Min_Color);
		 color[0] = 0;	color[1] = valcol;	color[2] = 1;	color[3] = alpha;
		}
		// Between cyan and green
		else if (value <Min2)
		{valcol = (value -Min1)/(Min2 - Min1);
		 color[0] = 0;	color[1] = 1;	color[2] = 1-valcol;	color[3] = alpha;
		}
		//between green and yellow
		else if (value <Min3)
		{valcol = (value -Min2)/(Min3 - Min2);
		 color[0] = valcol;	color[1] = 1;	color[2] = 0;	color[3] = alpha;
		}
		//between yellow and red
		else if (value <Max_Color)
		{valcol = (value -Min3)/(Max_Color - Min3);
		 color[0] = 1;	color[1] = 1-valcol;	color[2] = 0;	color[3] = alpha;
		}
		//red
		else //if (value > Max_Color)
		{color[0] = 1;	color[1] = 0;	color[2] = 0;	
			if (g_sc_show_above_max == 1)
			{	
				color[3] = alpha;
			}
			else
			{
				color[3] = 0.0;
			}
		
		}
		//color[
	}
	else if (g_color_scale_id==-1)
	{ // tags color scale id!

		// Min is 0.0
		// Max is 24.0
		float Min =0;
		float Max =24;
		int act_color=0;
		if (value>Max)
		{
			value = (float)  ((int)value%25);
		}

		if (value < Min)
		{
			color[0] = g_tag_colors[0][0];	
			color[1] = g_tag_colors[0][1];	
			color[2] = g_tag_colors[0][2];	
			color[3] = g_tag_colors[0][3];
		}
		else if (value>Max)
		{
			color[0] = g_tag_colors[24][0];	
			color[1] = g_tag_colors[24][1];	
			color[2] = g_tag_colors[24][2];	
			color[3] = g_tag_colors[24][3];
		}
		else
		{
			// dans ce cas on est bien entre min et max : on 
			// cherche où value se place dans l'échelle des
			// tags.
			act_color = (int)(24*value/(Max-Min));
			if (act_color>=0 && act_color<25)
			{

				color[0] = g_tag_colors[act_color][0];	
				color[1] = g_tag_colors[act_color][1];	
				color[2] = g_tag_colors[act_color][2];	
				color[3] = g_tag_colors[act_color][3];;
			}
			else
			{
				color[0] = 0.5;	
				color[1] = 0.5;	
				color[2] = 0.5;	
				color[3] = 0.5;
			}
		
		}
	}
	else if (g_color_scale_id ==1)
	{

			// Four color scale
		float Min2;
		float Min1;
		Min1 = Min_Color + (Max_Color-Min_Color)/3;
		Min2 = Min_Color + 2*(Max_Color - Min_Color)/3;
		
		if (value < Min_Color)
		{color[0] = 0.5;	color[1] = 0;	color[2] = 0;	color[3] = 0.0;}
		else if (value <Min1)
		{valcol = (value -Min_Color)/(Min1 - Min_Color);

		 color[0] = 0.5 +0.5*valcol;	color[1] = 0;	color[2] = 0;	color[3] = valcol/3;
		}
		else if (value <Min2)
		{valcol = (value -Min1)/(Min2 - Min1);
		 color[0] = 1;	color[1] = valcol;	color[2] = 0;	color[3] = 0.33 + valcol/3;
		}
		else if (value <Max_Color)
		{valcol = (value -Min2)/(Max_Color - Min2);
		 color[0] = 1;	color[1] = 1;	color[2] = 0.66*valcol;	color[3] = 0.66+ valcol/3;
		}
		else //if (value > Max_Color)
		{color[0] = 1;	color[1] = 1;	color[2] = 0.66;	
			
				color[3] = 1.0;
			
		
		}
		//color[
	}
	else
	{
				color[0] = 0.5;	
				color[1] = 0.5;	
				color[2] = 0.5;	
				color[3] = 0.5;
	}
	
}
int  vertex_compare (const void* v1, const void* v2)
{
	float x1,y1,z1;
	float x2,y2,z2;

	x1 = vertexpointer(v1)->x;
	y1 = vertexpointer(v1)->y;
	z1 = vertexpointer(v1)->z;

	x2 = vertexpointer(v2)->x;
	y2 = vertexpointer(v2)->y;
	z2 = vertexpointer(v2)->z;
	  

  if (z1>z2)
  {  return 1;}
  else if (z1<z2)
  {return -1;}
  else 
  {
	  if (y1>y2)
	  {return 1;}
	  else if (y1<y2)
	  {return -1;}
	  else
	  {
		  if (x1>x2)
		  {return 1;}
		  else if (x1<x2)
		  {return -1;}
		  else{ return 0;}
	  }
  }	
}


void readstr(FILE *f,char *string)						// Reads A String From File (f)
{
	do													// Do This
	{
		fgets(string, 255, f);							// Gets A String Of 255 Chars Max From f (File)
	} while ((string[0] == '/') || (string[0] == '\n'));// Until End Of Line Is Reached
	return;												// Return
}


void ApplyTransformation(float input[3],float result[3], glMatrix mat)
{	//Apply transformation matrix (Rotation + Translation) to input.
	result[0] = input[0]*mat[0][0] + input[1]*mat[1][0] + input[2]*mat[2][0] + mat[3][0];	
	result[1] = input[0]*mat[0][1] + input[1]*mat[1][1] + input[2]*mat[2][1] + mat[3][1];	
	result[2] = input[0]*mat[0][2] + input[1]*mat[1][2] + input[2]*mat[2][2] + mat[3][2];	
}	

void ApplyInverseTransformation(float input[3],float result[3], glMatrix mat)
{	//Apply transformation matrix (Rotation + Translation) to input.

	float m0, m1, m2;
	m0 = -(mat[3][0]*mat[0][0] 	
		  +mat[3][1]*mat[0][1]									
		  +mat[3][2]*mat[0][2]);

	m1 = -(mat[3][0]*mat[1][0]+ 
			 mat[3][1]*mat[1][1]
			+mat[3][2]*mat[1][2]);

	m2 = -(mat[3][0]*mat[2][0] 
		  +mat[3][1]*mat[2][1]
		  +mat[3][2]*mat[2][2]);


	result[0] = input[0]*mat[0][0] + input[1]*mat[0][1] + input[2]*mat[0][2] + m0;	
	result[1] = input[0]*mat[1][0] + input[1]*mat[1][1] + input[2]*mat[1][2] + m1;	
	result[2] = input[0]*mat[2][0] + input[1]*mat[2][1] + input[2]*mat[2][2] + m2;	
}	

void ApplyRotation(float input[3],float result[3], glMatrix mat)
{
	//Apply rotation of a transformation matrix to input.	
	result[0] = input[0]*mat[0][0] + input[1]*mat[1][0] + input[2]*mat[2][0];	
	result[1] = input[0]*mat[0][1] + input[1]*mat[1][1] + input[2]*mat[2][1];	
	result[2] = input[0]*mat[0][2] + input[1]*mat[1][2] + input[2]*mat[2][2];	

}
void get_projection_matrix(glMatrix mat)
{
	 float m[16];	

   glGetFloatv(GL_PROJECTION_MATRIX, m);
   //glPopMatrix();
   //screenproj[0][0]= 1;
   
   mat[0][0]= (GLfloat)m[0];
   mat[0][1]= (GLfloat)m[1];
   mat[0][2]= (GLfloat)m[2];
   mat[0][3]= (GLfloat)m[3];
   mat[1][0]= (GLfloat)m[4];
   mat[1][1]= (GLfloat)m[5];
   mat[1][2]= (GLfloat)m[6];
   mat[1][3]= (GLfloat)m[7];
   mat[2][0]= (GLfloat)m[8];
   mat[2][1]= (GLfloat)m[9];
   mat[2][2]= (GLfloat)m[10];
   mat[2][3]= (GLfloat)m[11];
   mat[3][0]= (GLfloat)m[12];
   mat[3][1]= (GLfloat)m[13];
   mat[3][2]= (GLfloat)m[14];
   mat[3][3]= (GLfloat)m[15];
}
void getmatrix(glMatrix mat)
{    float m[16];	
   glGetFloatv(GL_MODELVIEW_MATRIX, m);
   //glPopMatrix();
   //screenproj[0][0]= 1;
   
   mat[0][0]= (GLfloat)m[0];
   mat[0][1]= (GLfloat)m[1];
   mat[0][2]= (GLfloat)m[2];
   mat[0][3]= (GLfloat)m[3];
   mat[1][0]= (GLfloat)m[4];
   mat[1][1]= (GLfloat)m[5];
   mat[1][2]= (GLfloat)m[6];
   mat[1][3]= (GLfloat)m[7];
   mat[2][0]= (GLfloat)m[8];
   mat[2][1]= (GLfloat)m[9];
   mat[2][2]= (GLfloat)m[10];
   mat[2][3]= (GLfloat)m[11];
   mat[3][0]= (GLfloat)m[12];
   mat[3][1]= (GLfloat)m[13];
   mat[3][2]= (GLfloat)m[14];
   mat[3][3]= (GLfloat)m[15];
	
}
void readInt4bs(long *my_int, FILE* filein)
	   {
			char vc[4], *c;
			union{long ii; char c[4];}u;

			fread(vc,1,4,filein);
			c=&vc[0];
			for(int k=3;k>-1;k--)
			{u.c[k]=*c++;}
			*my_int = u.ii;
	   }
void readFloat4b(float *my_float, FILE* filein)
	   {
			char vc[4], *c;
			union{float f; char c[4];}u;

			fread(vc,1,4,filein);
			c=&vc[0];
			for(int k=3;k>=0;k--)
			{u.c[k]=*c++;}
			*my_float = u.f;
	   }

void get_camera_transform_matrix (glMatrix cam)
{
	glPushMatrix();
	glLoadIdentity();
   glTranslated(tx, ty, tz);   //M2
   glTranslated(camera.tx, camera.ty, camera.tz);   //M2
   glTranslated(camera.atx, camera.aty, camera.atz); //M1
glRotated(-90, 0.0, 0.0, 1.0);
   glRotated(-90, 0.0, 1.0, 0.0);
  
	
   
   
   //glRotated(roty, 0.0, 1.0, 0.0);
   //glRotated(rotz, 0.0, 0.0, 1.0);

   glRotated(rotx, 1.0, 0.0, 0.0);
   glRotated(roty, 0.0, 1.0, 0.0);
   
   
   glRotated(camera.tw, 1.0, 0.0, 0.0);
   glRotated(camera.el, 0.0, 1.0, 0.0);
   glRotated(-camera.az, 0.0, 0.0, 1.0); 
   glRotated(rotz, 0.0, 0.0, 1.0); 

   glTranslated(-camera.atx, -camera.aty, -camera.atz);
   glScaled(camera.zoom, camera.zoom, camera.zoom);
   getmatrix(cam);
   glPopMatrix();

}

/*void VL2::get_camera_transform_matrix2 (glMatrix cam)
{
	glPushMatrix();
	glLoadIdentity();
   glTranslated(tx, ty, tz);   //M2
   glTranslated(camera.tx, camera.ty, camera.tz);   //M2
   glTranslated(camera.atx, camera.aty, camera.atz); //M1
   glRotated(-90, 0.0, 0.0, 1.0);
   glRotated(-90, 0.0, 1.0, 0.0);

   glRotated(rotx, 1.0, 0.0, 0.0);
   glRotated(roty, 0.0, 1.0, 0.0);
   glRotated(rotz, 0.0, 0.0, 1.0); 
   



   glRotated(camera.tw, 1.0, 0.0, 0.0);
   glRotated(camera.el, 0.0, 1.0, 0.0);
   glRotated(-camera.az, 0.0, 0.0, 1.0); 
   glTranslated(-camera.atx, -camera.aty, -camera.atz);
   glScaled(camera.zoom, camera.zoom, camera.zoom);
   getmatrix(cam);
   glPopMatrix();

}
*/

