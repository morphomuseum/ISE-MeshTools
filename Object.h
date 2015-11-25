#ifndef OBJECT_H
#define OBJECT_H 465

#include "StdAfx.h"
#include <stdlib.h>
#include <math.h>										// Math Library Header File
#include <stdio.h>
#include <string.h>
#include "CDataFile.h"
#include <FL/gl.h>
#include <FL/Fl_File_Chooser.H>
#include "Struct2.h"
#include <vtkPolyData.h>
//#include "Struct.h"
//#ifndef STRUCT_H

//#endif 

class OBJECT : public vtkPolyData
	// Structure For An Object
{
public:
 
	std::string name;
 glMatrix Mat2;
 glMatrix Mat1;
 
													// Keeps Track Of Highest Number Of Vertices Used In Any Of The
 
 OBJECT(int n);
 OBJECT();
};
#endif