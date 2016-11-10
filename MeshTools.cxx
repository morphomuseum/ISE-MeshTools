


#include "ui_MeshTools.h"
#include "MeshTools.h"

#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataReader.h>
#include <vtkQtTableView.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkVectorText.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>

#include "vtkSmartPointer.h"
#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

// Constructor
MeshTools::MeshTools()
{
  this->ui = new Ui_MeshTools;
  this->ui->setupUi(this);

  // Qt Table View
  this->TableView = vtkSmartPointer<vtkQtTableView>::New();

  // Place the table view in the designer form
  this->ui->tableFrame->layout()->addWidget(this->TableView->GetWidget());

  // Geometry
  VTK_CREATE(vtkVectorText, text);
  text->SetText("VTK and Qt!");
  VTK_CREATE(vtkElevationFilter, elevation);
  elevation->SetInputConnection(text->GetOutputPort());
  elevation->SetLowPoint(0,0,0);
  elevation->SetHighPoint(10,0,0);

  // Mapper
  VTK_CREATE(vtkPolyDataMapper, mapper);
  mapper->ImmediateModeRenderingOn();
  
  VTK_CREATE(vtkPolyDataMapper, mapper2);
  mapper2->ImmediateModeRenderingOn();

  VTK_CREATE(vtkPolyDataMapper, mapper3);
  mapper3->ImmediateModeRenderingOn();

  vtkSmartPointer<vtkPolyDataReader> reader =
 vtkSmartPointer<vtkPolyDataReader>::New();
  reader->SetFileName("Cranium.vtk");
  reader->Update();
  
  vtkSmartPointer<vtkPolyDataReader> reader2 =
	  vtkSmartPointer<vtkPolyDataReader>::New();
  reader2->SetFileName("Letf_inner_ear.vtk");
  reader2->Update();

  vtkSmartPointer<vtkPolyDataReader> reader3 =
	  vtkSmartPointer<vtkPolyDataReader>::New();
  reader3->SetFileName("Right_inner_ear.vtk");
  reader3->Update();
  mapper->SetInputConnection(reader->GetOutputPort());
  mapper2->SetInputConnection(reader2->GetOutputPort());
  mapper3->SetInputConnection(reader3->GetOutputPort());

  // Actors in scene
  VTK_CREATE(vtkActor, actor);
  actor->GetProperty()->SetColor(0.5, 1, 0.5);
  actor->GetProperty()->SetOpacity(0.5);  
  actor->SetMapper(mapper);
 
  VTK_CREATE(vtkActor, actor2);
// vtkSmartPointer<vtkActor> mactor2 =
//vtkSmartPointer<vtkActor>::New();
  
 actor2->GetProperty()->SetColor(1, 0, 0);
 actor2->SetMapper(mapper2);

 VTK_CREATE(vtkActor, actor3);
 // vtkSmartPointer<vtkActor> mactor2 =
 //vtkSmartPointer<vtkActor>::New();

 actor3->GetProperty()->SetColor(0, 1, 0);
 actor3->SetMapper(mapper3);

  // VTK Renderer
  VTK_CREATE(vtkRenderer, ren);
  

  // Add Actor to renderer
  ren->AddActor(actor2);
  ren->AddActor(actor3);
  ren->AddActor(actor);
 

  this->ui->qvtkWidget->GetRenderWindow()->SetAlphaBitPlanes(1);
  this->ui->qvtkWidget->GetRenderWindow()->SetMultiSamples(0);
  
  ren->SetUseDepthPeeling(1);
  ren->SetMaximumNumberOfPeels(100);
  ren->SetOcclusionRatio(0.1);
  

  // VTK/Qt wedded


  this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(ren);
  cout<< "Peeling was used:"<< ren->GetLastRenderingUsedDepthPeeling();
  vtkSmartPointer<vtkCamera> camera =
	  vtkSmartPointer<vtkCamera>::New();
 
  camera = ren->GetActiveCamera();
  camera->SetViewUp(0, 0, -1);
  camera->SetPosition(0, 1, -100);
  camera->SetFocalPoint(0, 0, 0);
  camera->ComputeViewPlaneNormal();
  camera->Azimuth(30.0);
  camera->Elevation(30.0);
  camera->ParallelProjectionOn();
  // Just a bit of Qt interest: Culling off the
  // point data and handing it to a vtkQtTableView
  VTK_CREATE(vtkDataObjectToTable, toTable);
  toTable->SetInputConnection(elevation->GetOutputPort());
  toTable->SetFieldType(vtkDataObjectToTable::POINT_DATA);
  toTable->Update();
  // Here we take the end of the VTK pipeline and give it to a Qt View
  this->TableView->SetRepresentationFromInputConnection(toTable->GetOutputPort());

  // Set up action signals and slots
  connect(this->ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));


  vtkSmartPointer<vtkAxesActor> axes =
	  vtkSmartPointer<vtkAxesActor>::New();

//  VTK_CREATE(vtkOrientationMarkerWidget, widget);
  vtkOrientationMarkerWidget* widget = vtkOrientationMarkerWidget::New();
  // ça ne marche pas avec class widget!!!!

  
  widget->SetOutlineColor(0.9300, 0.5700, 0.1300);
  widget->SetOrientationMarker(axes);
  widget->SetDefaultRenderer(ren);
  widget->SetInteractor(this->ui->qvtkWidget->GetRenderWindow()->GetInteractor());
  widget->SetViewport(0.0, 0.0, 0.2, 0.2);
  widget->SetEnabled(1);
  widget->InteractiveOff();
  widget->PickingManagedOn();
  

};

MeshTools::~MeshTools()
{
  // The smart pointers should clean up for up

}

// Action to be taken upon file open
void MeshTools::slotOpenFile()
{

}

void MeshTools::slotExit() {
  qApp->exit();
}
