/*=========================================================================

  Program:   Visualization Toolkit
  Module:    MeshTools.h
  Language:  C++

  Copyright 2009 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
  license for use of this work by or on behalf of the
  U.S. Government. Redistribution and use in source and binary forms, with
  or without modification, are permitted provided that this Notice and any
  statement of authorship are reproduced on all copies.

=========================================================================*/
#ifndef MeshTools_H
#define MeshTools_H

#include "vtkGridActor.h"
#include "vtkOrientationHelperWidget.h"
#include "vtkMTActorCollection.h"

#include <vtkSmartPointer.h>    // Required for smart pointer internal ivars.
#include <vtkAreaPicker.h>   

#include <vtkRenderer.h>
#include <vtkImageData.h>
#include <vtkCamera.h>
#include <vtkStructuredGrid.h>
#include <vtkCaptionActor2D.h>
#include <QMainWindow>
#include <QFileDialog>


// Forward Qt class declarations
class Ui_MeshTools;

// Forward VTK class declarations
class vtkQtTableView;


class MeshTools : public QMainWindow
{
  Q_OBJECT

public:

  // Constructor/Destructor
  MeshTools();
  ~MeshTools();
  
  
  vtkSmartPointer<vtkMTActorCollection> ActorCollection;
  vtkSmartPointer<vtkRenderer> Renderer;
  vtkSmartPointer<vtkCamera> Camera;
  vtkSmartPointer<vtkGridActor> GridActor;
  vtkSmartPointer<vtkAreaPicker> AreaPicker;


  vtkOrientationHelperWidget* OrientationHelperWidget; 

  int mui_ShowGrid;
  int mui_ShowOrientationHelper;
  int mui_CameraCentreOfMassAtOrigin;
  int mui_CameraOrtho;
public slots:

  virtual void slotOpenMeshFile();
  virtual void slotCameraFront();
  virtual void slotCameraBack();
  virtual void slotCameraLeft();
  virtual void slotCameraRight();
  virtual void slotCameraAbove();
  virtual void slotCameraBelow();
  virtual void slotGridToggle();
  virtual void slotOrientationHelperToggle();
  virtual void slotCameraCentreOfMassToggle();
  virtual void slotCameraOrthoPerspectiveToggle();
  virtual void slotExit();
/*  void RubberBandSelect(vtkObject* caller,
	  long unsigned int vtkNotUsed(eventId),
	  void* vtkNotUsed(clientData),
	  void* vtkNotUsed(callData));*/

protected:

protected slots:

private:
	void ReplaceCameraAndGrid();
	void ResetCameraOrthoPerspective();
	void DollyCameraForParallelScale();
	void DollyCameraForPerspectiveMode();
	void SetGridVisibility();
	void SetOrientationHelperVisibility();
	
	void saveSettings();
	void UpdateRenderer();
	double GetGlobalBoundingBoxLength();
	
	std::string CheckingName(std::string name_obj, int cpt_name);

  vtkSmartPointer<vtkQtTableView>         TableView;

  // Designer form
  Ui_MeshTools *ui;
};

#endif // MeshTools_H
