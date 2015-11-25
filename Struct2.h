#ifndef STRUCT2_H
#define STRUCT2_H
#include <math.h>										// Math Library Header File
#include <FL/gl.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct{
public : 
	float x, y, z; 
} POLYGON_VERTEX;

class POLYGON_LIST
{
public :
	POLYGON_VERTEX *point_list;
	int point_number;
	int statut; //statut 0 = open statut 1 =close


	POLYGON_LIST()
	{
		this->point_number = 0;
		this->statut = 0;
		
	
	}
	int POLYGON_POINT_INSIDE ( POLYGON_VERTEX P)
	{
    int    cn = 0;    // the crossing number counter

    // loop through all edges of the polygon
    for (int i=0; i<point_number -1; i++) {    // edge from V[i] to V[i+1]
       if (((point_list[i].y <= P.y) && (point_list[i+1].y > P.y))    // an upward crossing
        || ((point_list[i].y > P.y) && (point_list[i+1].y <= P.y))) { // a downward crossing
            // compute the actual edge-ray intersect x-coordinate
            float vt = (float)(P.y - point_list[i].y) / (point_list[i+1].y - point_list[i].y);
            if (P.x < point_list[i].x + vt * (point_list[i+1].x - point_list[i].x)) // P.x < intersect
                ++cn;   // a valid crossing of y=P.y right of P.x
        }
    }
	//last edge
	 if (((point_list[0].y <= P.y) && (point_list[point_number -1].y > P.y))    // an upward crossing
        || ((point_list[0].y > P.y) && (point_list[point_number -1].y <= P.y))) { // a downward crossing
            // compute the actual edge-ray intersect x-coordinate
            float vt = (float)(P.y - point_list[0].y) / (point_list[point_number -1].y - point_list[0].y);
            if (P.x < point_list[0].x + vt * (point_list[point_number -1].x - point_list[0].x)) // P.x < intersect
                ++cn;   // a valid crossing of y=P.y right of P.x
        }
    return (cn&1);    // 0 if even (out), and 1 if odd (in)

	}

	void Polygon_close()
	{

		if (Polygon_valid())
		{
			this->statut = 1;
		}
		else
		{
			
			Polygon_init();
			//error message
		}
	}
	void Polygon_init()
	{
		if (this->point_number >0)
		{free (this->point_list);}
		this->point_number = 0;
		this->statut = 0;
		
		
	}
	int Poly_cross (float x1, float y1, float x2, float y2, float u1, float u2, float v1, float v2)
	{
	
		float b1;
		float b2;
		float xi,yi;
		float a1,a2;
		if ( (x2-x1 ==0)||(u2-u1 ==0))
		{return 0;}// Cases were one edge is vertical are avoided...(A and B exceptions)

		b1 = (y2-y1)/(x2-x1); //(A) 
		b2 = (v2-v1)/(u2-u1); // (B) 

		if ( b2-b1 ==0)
		{return 0;}// Cases were two edges are parallel are avoided. (C exception)

		a1 = y1-b1*x1;
		a2 = v1-b2*u1;
		xi = - (a1-a2)/(b1-b2); //(C) 
		yi = a1+b1*xi ;
		if ( 
			((x1-xi)*(xi-x2)>=0)
			&&( (u1-xi)*(xi-u2)>=0)
			&&( (y1-yi)*(yi-y2)>=0) 
			&& ((v1-yi)*(yi-v2)>=0)
			)
		{return 1;}
		else {return 0;}
		

	}
	int Polygon_valid ()						// Allocate Memory For Each Object
	{		
		int x1,x2,y1,y2,u1,u2,v1,v2;
		// And Defines points

		int valid = 1;
	
		if (this->point_number < 3)
		{ return 0;}
		else
		{//return 1;
			if (this->point_number ==3)
			{
				x1 = (int)point_list[0].x;
				x2 = (int)point_list[1].x;
				y1 = (int)point_list[0].y;
				y2 = (int)point_list[1].y;
				u1 = (int)point_list[1].x;
				u2 = (int)point_list[2].x;
				v1 = (int)point_list[1].y;
				v2 = (int)point_list[2].y;
				//test if paralleles
			}
			else
			{	int i;
				for ( i=0; i<point_number -1; i++) 
				{ 
					x1 = (int)point_list[i].x;
					x2 = (int)point_list[i+1].x;
					y1 = (int)point_list[i].y;
					y2 = (int)point_list[i+1].y;
					for (int j = i+2; j<point_number-1 ; j++)
					{
						fprintf(stderr, "Segment %d - %d\n", i+1, j+1);
						u1 = (int)point_list[j].x;
						u2 = (int)point_list[j+1].x;
						v1 = (int)point_list[j].y;
						v2 = (int)point_list[j+1].y;
						 
						if (Poly_cross ((float)x1,(float)y1,(float)x2,(float)y2,(float)u1,(float)u2,(float)v1,(float)v2))
						{valid =0;}
					}				
				}
				//Last edge
				x1 = (int)point_list[point_number -1].x;
				x2 = (int)point_list[0].x;
				y1 = (int)point_list[point_number -1].y;
				y2 = (int)point_list[0].y;
				for (i = 1; i<point_number -2; i++)
				{
					    
						u1 = (int)point_list[i].x;
						u2 = (int)point_list[i+1].x;
						v1 = (int)point_list[i].y;
						v2 = (int)point_list[i+1].y;
						fprintf(stderr, "Segment %d - %d\n", point_number, i+1); 
						if (Poly_cross ((float)x1,(float)y1,(float)x2,(float)y2,(float)u1,(float)u2,(float)v1,(float)v2))
						{valid =0;}
				}	
				
			}
		}
		return valid;
	}
	void Polygon_add (int x, int y)
	{
		if (this->statut ==0)
		{
			this->point_number ++;
			if (this->point_number == 1)
			{
				Polygon_allocate (1);
			
			}
			else
			{
				Polygon_reallocate (this->point_number);
			}
			this->point_list[this->point_number -1].x = (float)x;
			this->point_list[this->point_number -1].y = (float)y;
		}
	}
		void Polygon_add (float x, float y, float z)
	{
		if (this->statut ==0)
		{
			this->point_number ++;
			if (this->point_number == 1)
			{
				Polygon_allocate (1);
			
			}
			else
			{
				Polygon_reallocate (this->point_number);
			}
			this->point_list[this->point_number -1].x = x;
			this->point_list[this->point_number -1].y = y;
			this->point_list[this->point_number -1].z = z;
		}
	}

	void Polygon_allocate (int n)						// Allocate Memory For Each Object
	{														// And Defines points
		//return 1;
		this->point_list = (POLYGON_VERTEX*)malloc(sizeof(POLYGON_VERTEX)*n);
	}
	void Polygon_reallocate(int n)
	{
		this->point_list = (POLYGON_VERTEX*)realloc(this->point_list, sizeof(POLYGON_VERTEX)*n);
	}
	
};

class VERTEX2
{
public :
	float	x, y, z;									// X, Y & Z Point coordinate
	float   nx, ny, nz;									// Mean Normal Vector
	int     *triangles;									// Triangle list to which the point belongs
	int     nbtriangles;								//Number of triangles in this list
	float area_percent;		//Total percentage towards mean configuration
	float area_total;
	GLfloat color[4],color2[4];
	float vx,vy,vz,vn,vx1,vy1,vz1,vx2,vy2,vz2; // Vector normal to normal vector and proportional to  landmark distance from corresponding landmark to mean form.
} ;												// Called VERTEX


typedef  GLfloat Mat[4];
typedef  Mat glMatrix[4];
	


			
#endif