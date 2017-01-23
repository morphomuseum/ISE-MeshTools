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


#ifndef vtkMTInteractorStyle_h
#define vtkMTInteractorStyle_h
#include "vtkMTActorCollection.h"
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>


class vtkUnsignedCharArray;

class  vtkMTInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
  static vtkMTInteractorStyle *New();
  vtkTypeMacro(vtkMTInteractorStyle, vtkInteractorStyleTrackballCamera);
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
  //virtual void SaveSelectedActorPositions();
  virtual void RotateActors();
  virtual void SpinActors();
  virtual void PanActors();


  void SetActorCollection(vtkSmartPointer<vtkMTActorCollection>ActColl);
protected:
  
  vtkMTInteractorStyle();
  ~vtkMTInteractorStyle();
  virtual void Dolly(double factor);
  void Prop3DTransform(vtkProp3D *prop3D,
	  double *boxCenter,
	  int NumRotation,
	  double **rotate,
	  double *scale);

  virtual void Pick();
  void RedrawRubberBand();

  int StartPosition[2];
  int EndPosition[2];

  int Moving;
  int ActorsPositionsSaved;
  void SaveSelectedActorsPositions();
  void DeleteSelectedActors();
  int NumberOfSelectedActors;
  int getNumberOfSelectedActors();
  vtkUnsignedCharArray *PixelArray;

  int CurrentMode;
  int Ctrl;
  int L;
  //vtkProp3D *InteractionProp;
  vtkSmartPointer<vtkMTActorCollection> ActorCollection;
private:
#if VTK_MINOR_VERSION >= 1
	vtkMTInteractorStyle(const vtkMTInteractorStyle&) VTK_DELETE_FUNCTION;
	void operator=(const vtkMTInteractorStyle&) VTK_DELETE_FUNCTION;
#else
	vtkMTInteractorStyle(const vtkMTInteractorStyle&);  // Not implemented
	void operator=(const vtkMTInteractorStyle&);  // Not implemented
#endif

  
};

#endif
