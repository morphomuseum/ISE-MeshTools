#ifndef CONTAINER_H
#define CONTAINER_H
#include "StdAfx.h"
#include <stdlib.h>
#include <math.h>										// Math Library Header File
//#include <stdio.h>
#include <iostream>
#include <string.h>
#include "CDataFile.h"
#include <FL/gl.h>
#include <FL/Fl_File_Chooser.H>
#include "Object.h"
//#include "Struct.h"
//#ifndef STRUCT_H

//#endif 
using namespace std;

class CONTAINER 
	// Structure For An Object
{
	class matrix{
  // NOTE: maxsize determines available memory storage, but
  // actualsize determines the actual size of the stored matrix in use
  // at a particular time.
  int maxsize;  // max number of rows (same as max number of columns)
  int actualsize;  // actual size (rows, or columns) of the stored matrix
  double* data;      // where the data contents of the matrix are stored
  void allocate()   {
    delete[] data;
    data = new double [maxsize*maxsize];
    };
  matrix() {};                  // private ctor's
  matrix(int newmaxsize) {matrix(newmaxsize,newmaxsize);};
public:
  matrix(int newmaxsize, int newactualsize)  { // the only public ctor
    if (newmaxsize <= 0) newmaxsize = 5;
    maxsize = newmaxsize; 
    if ((newactualsize <= newmaxsize)&&(newactualsize>0))
      actualsize = newactualsize;
    else 
      actualsize = newmaxsize;
    // since allocate() will first call delete[] on data:
    data = 0;
    allocate();
    };
  ~matrix() { delete[] data; };
  void comparetoidentity()  {
    int worstdiagonal = 0;
    double maxunitydeviation = 0.0;
    double currentunitydeviation;
    for ( int i = 0; i < actualsize; i++ )  {
      currentunitydeviation = data[i*maxsize+i] - 1.;
      if ( currentunitydeviation < 0.0) currentunitydeviation *= -1.;
      if ( currentunitydeviation > maxunitydeviation )  {
           maxunitydeviation = currentunitydeviation;
           worstdiagonal = i;
           }
      }
    int worstoffdiagonalrow = 0;
    int worstoffdiagonalcolumn = 0;
    double maxzerodeviation = 0.0;
    double currentzerodeviation ;
    for (int  i = 0; i < actualsize; i++ )  {
      for ( int j = 0; j < actualsize; j++ )  {
        if ( i == j ) continue;  // we look only at non-diagonal terms
        currentzerodeviation = data[i*maxsize+j];
        if ( currentzerodeviation < 0.0) currentzerodeviation *= -1.0;
        if ( currentzerodeviation > maxzerodeviation )  {
          maxzerodeviation = currentzerodeviation;
          worstoffdiagonalrow = i;
          worstoffdiagonalcolumn = j;
          }

        }
      }
    cout << "Worst diagonal value deviation from unity: " 
       << maxunitydeviation << " at row/column " << worstdiagonal << endl;
    cout << "Worst off-diagonal value deviation from zero: " 
       << maxzerodeviation << " at row = " << worstoffdiagonalrow 
       << ", column = " << worstoffdiagonalcolumn << endl;
  }
  void settoproduct(matrix& left, matrix& right)  {
    actualsize = left.getactualsize();
    if ( maxsize < left.getactualsize() )   {
      maxsize = left.getactualsize();
      allocate();
      }
    for ( int i = 0; i < actualsize; i++ )
      for ( int j = 0; j < actualsize; j++ )  {
        double sum = 0.0;
        double leftvalue, rightvalue;
        bool success;
        for (int c = 0; c < actualsize; c++)  {
            left.getvalue(i,c,leftvalue,success);
            right.getvalue(c,j,rightvalue,success);
            sum += leftvalue * rightvalue;
            }
        setvalue(i,j,sum);
        }
    }
  void copymatrix(matrix&  source)  {
    actualsize = source.getactualsize();
    if ( maxsize < source.getactualsize() )  {
      maxsize = source.getactualsize();
      allocate();
      }
    for ( int i = 0; i < actualsize; i++ )
      for ( int j = 0; j < actualsize; j++ )  {
        double value;
        bool success;
        source.getvalue(i,j,value,success);
        data[i*maxsize+j] = value;
        }
    };
  void setactualsize(int newactualsize) {
    if ( newactualsize > maxsize )
      {
        maxsize = newactualsize ; // * 2;  // wastes memory but saves
                                      // time otherwise required for
                                      // operation new[]
        allocate();
      }
    if (newactualsize >= 0) actualsize = newactualsize;
    };
  int getactualsize() { return actualsize; };
  void getvalue(int row, int column, double& returnvalue, bool& success)   {
    if ( (row>=maxsize) || (column>=maxsize) 
      || (row<0) || (column<0) )
      {  success = false;
         return;    }
    returnvalue = data[ row * maxsize + column ];
    success = true;
    };
  bool setvalue(int row, int column, double newvalue)  {
    if ( (row >= maxsize) || (column >= maxsize) 
        || (row<0) || (column<0) ) return false;
    data[ row * maxsize + column ] = newvalue;
    return true;
    };
  void invert()  {
    if (actualsize <= 0) return;  // sanity check
    if (actualsize == 1) return;  // must be of dimension >= 2
    for (int i=1; i < actualsize; i++) data[i] /= data[0]; // normalize row 0
    for (int i=1; i < actualsize; i++)  { 
      for (int j=i; j < actualsize; j++)  { // do a column of L
        double sum = 0.0;
        for (int k = 0; k < i; k++)  
            sum += data[j*maxsize+k] * data[k*maxsize+i];
        data[j*maxsize+i] -= sum;
        }
      if (i == actualsize-1) continue;
      for (int j=i+1; j < actualsize; j++)  {  // do a row of U
        double sum = 0.0;
        for (int k = 0; k < i; k++)
            sum += data[i*maxsize+k]*data[k*maxsize+j];
        data[i*maxsize+j] = 
           (data[i*maxsize+j]-sum) / data[i*maxsize+i];
        }
      }
    for (int  i = 0; i < actualsize; i++ )  // invert L
      for ( int j = i; j < actualsize; j++ )  {
        double x = 1.0;
        if ( i != j ) {
          x = 0.0;
          for ( int k = i; k < j; k++ ) 
              x -= data[j*maxsize+k]*data[k*maxsize+i];
          }
        data[j*maxsize+i] = x / data[j*maxsize+j];
        }
    for (int  i = 0; i < actualsize; i++ )   // invert U
      for ( int j = i; j < actualsize; j++ )  {
        if ( i == j ) continue;
        double sum = 0.0;
        for ( int k = i; k < j; k++ )
            sum += data[k*maxsize+j]*( (i==k) ? 1.0 : data[i*maxsize+k] );
        data[i*maxsize+j] = -sum;
        }
    for (int  i = 0; i < actualsize; i++ )   // final inversion
      for ( int j = 0; j < actualsize; j++ )  {
        double sum = 0.0;
        for ( int k = ((i>j)?i:j); k < actualsize; k++ )  
            sum += ((j==k)?1.0:data[j*maxsize+k])*data[k*maxsize+i];
        data[j*maxsize+i] = sum;
        }
    };
};

public:
float dmean; // Mean distance towards centroid of "something", which can be :
				// 1) the last opened object if "g_auto_zoom" is on
				// 2) value which will set 100 px in mm
			    // 3) if "draw" is called and dmean_ok = 0, this will be set according to field of view width 
				
int	  dmean_ok; // Mean distance towards centroid computed ?(used both in Morpho and Mesh)
//float


				//Not really relevant in "Mesh", could be remove!

  int *supr; // list of suppressed objects; Used both in Morpho and Mesh
  
 long numtri; // Dirty : used in Morpho (OHI) and a lot in Mesh (OpenSTL). 
			  // Should be placed in "object".
 
 
 int		nbOBJECTS;	

 virtual void containerallocate(int n){};
 virtual void containerreallocate(int n){};
 // Number Of Vertices For The Object

 // One Vertice (Vertex x,y & z)
//OBJECT my_obj;
// Triangle_list *OBJECTS;		
 



 




};
#endif