/*=========================================================================
*
* Copyright David Doria 2011 daviddoria@gmail.com
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0.txt
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*=========================================================================*/

/** vtkOBJWriter - Write a Wavefront OBJ (ASCII) file. The core of this class
* was taken from vtkOBJExporter. The goal is to make it follow the standard
* "writer" syntax that is present for the rest of the VTK writers (vtkPLYWriter, etc).
* It only saves the geometry (vertices, lines, polylines, strips) with normals and tcoords (if any).
* It does not write triangle strips properly.
*/

#ifndef __vtkOBJWriter_h
#define __vtkOBJWriter_h



#include "vtkIOGeometryModule.h" // For export macro
#include "vtkWriter.h"


class vtkCellArray;
class vtkPoints;
class vtkPolyData;

class  vtkOBJWriter : public vtkWriter
{
public:
  static vtkOBJWriter *New();
  vtkTypeMacro(vtkOBJWriter,vtkWriter);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Specify the name of the file to write out.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);
 
// Description:
  // Get the input to this writer.
  vtkPolyData* GetInput();
  vtkPolyData* GetInput(int port);


  
protected:



  vtkOBJWriter();
  ~vtkOBJWriter()	
    {
    delete[] this->FileName;   
    }

  void WriteData();
 void WriteOBJ(vtkPolyData *pd);
  char* FileName;
  
  virtual int FillInputPortInformation(int port, vtkInformation *info);
  /////////////////////////////

private:
  vtkOBJWriter(const vtkOBJWriter&); // Not implemented.
  void operator=(const vtkOBJWriter&); // Not implemented.

  
};

#endif