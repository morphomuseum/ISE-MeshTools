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

#include "mqMeshToolsCore.h"



#include <vtkSmartPointer.h>    // Required for smart pointer internal ivars.
#include <vtkRenderedAreaPicker.h>   
#include <vtkAreaPicker.h>   

#include <vtkRenderer.h>
#include <vtkImageData.h>

#include <vtkStructuredGrid.h>
#include <vtkBillboardTextActor3D.h>
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
  
  mqMeshToolsCore* MeshToolsCore;
  
 // vtkSmartPointer<vtkRenderedAreaPicker> AreaPicker;
 vtkSmartPointer<vtkAreaPicker> AreaPicker;

  
  
  
public slots:
  //static  MeshTools* Instance;
 // static MeshTools* instance();
 // static int getTestInt();
  
  
  
  
  virtual void slotExit();
  


protected:

protected slots:
	
private:
	
	
	
	
	
	
	void saveSettings();
	
	
  vtkSmartPointer<vtkQtTableView>         TableView;
   Ui_MeshTools *ui;
};

#endif // MeshTools_H
