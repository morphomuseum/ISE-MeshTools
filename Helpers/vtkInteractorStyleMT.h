/*=========================================================================

  Program:   MeshTools

  Copied from:    vtkInteractorStyleRubberBandPick.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/


#ifndef vtkInteractorStyleMT_h
#define vtkInteractorStyleMT_h

#include "vtkInteractorStyleTrackballCamera.h"

class vtkUnsignedCharArray;

class  vtkInteractorStyleMT : public vtkInteractorStyleTrackballCamera
{
public:
  static vtkInteractorStyleMT *New();
  vtkTypeMacro(vtkInteractorStyleMT, vtkInteractorStyleTrackballCamera);
  void PrintSelf(ostream& os, vtkIndent indent);

  void StartSelect();

  //@{
  /**
   * Event bindings
   */
  virtual void OnMouseMove();
  virtual void OnLeftButtonDown();
  virtual void OnLeftButtonUp();
  virtual void OnRightButtonDown();
  virtual void OnRightButtonUp();
  virtual void OnMiddleButtonDown();
  virtual void OnMiddleButtonUp();
  virtual void OnChar();
  virtual void RubberStart();
  virtual void RubberStop();
  virtual void OnKeyPress();
  virtual void OnKeyRelease();
  //@}

protected:
  vtkInteractorStyleMT();
  ~vtkInteractorStyleMT();

  virtual void Pick();
  void RedrawRubberBand();

  int StartPosition[2];
  int EndPosition[2];

  int Moving;

  vtkUnsignedCharArray *PixelArray;

  int CurrentMode;
  int Ctrl;
private:
  vtkInteractorStyleMT(const vtkInteractorStyleMT&) VTK_DELETE_FUNCTION;
  void operator=(const vtkInteractorStyleMT&) VTK_DELETE_FUNCTION;
};

#endif
