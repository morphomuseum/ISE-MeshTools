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

#include <vtkSmartPointer.h>    // Required for smart pointer internal ivars.
#include <vtkActorCollection.h>
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
  
  vtkSmartPointer<vtkActorCollection> ActorCollection;
  vtkSmartPointer<vtkRenderer> Renderer;
  vtkSmartPointer<vtkCamera> Camera;
  vtkSmartPointer<vtkGridActor> GridActor;

public slots:

  virtual void slotOpenMeshFile();
  virtual void slotCameraFront();
  virtual void slotCameraBack();
  virtual void slotCameraLeft();
  virtual void slotCameraRight();
  virtual void slotCameraAbove();
  virtual void slotCameraBelow();

  virtual void slotExit();

protected:

protected slots:

private:
	
	void UpdateRenderer();
	double GetGlobalBoundingBoxLength();
	void GetGlobalCenterOfMass(double center[3]);
	std::string CheckingName(std::string name_obj, int cpt_name);

  vtkSmartPointer<vtkQtTableView>         TableView;

  // Designer form
  Ui_MeshTools *ui;
};

#endif // MeshTools_H
