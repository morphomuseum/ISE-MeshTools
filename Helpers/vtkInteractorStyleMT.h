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

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>
#include <vtkActorCollection.h>

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

  virtual void RotateActors();
  virtual void SpinActors();
  virtual void PanActors();
  virtual void DollyActors();
  void SetActorCollection(vtkSmartPointer<vtkActorCollection>ActColl);
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
  //vtkProp3D *InteractionProp;
  vtkSmartPointer<vtkActorCollection> ActorCollection;
private:
#if VTK_MINOR_VERSION >= 1
	vtkInteractorStyleMT(const vtkInteractorStyleMT&) VTK_DELETE_FUNCTION;
	void operator=(const vtkInteractorStyleMT&) VTK_DELETE_FUNCTION;
#else
	vtkInteractorStyleMT(const vtkInteractorStyleMT&);  // Not implemented
	void operator=(const vtkInteractorStyleMT&);  // Not implemented
#endif

  
};

#endif
