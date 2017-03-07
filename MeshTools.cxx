


#include "ui_MeshTools.h"
#include "MeshTools.h"



#include "mqMeshToolsMenuBuilders.h"
#include "mqOpenDataReaction.h"
#include "vtkBezierSurfaceWidget.h"
#include "vtkBezierSurfaceSource.h"
#include "vtkMTActor.h"
#include "vtkLMActor.h"
#include "mqMeshToolsCore.h"
#include "mqUndoStack.h"
#include "vtkMTInteractorStyle.h"
#include "vtkMTActorCollection.h"
//#include "vtkUndoStack.h"
//#include "vtkUndoSet.h"
//#include "vtkUndoElement.h"
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>

#include <vtkPoints.h>
#include <vtkPlaneSource.h>
#include <vtkTextProperty.h>
#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include <vtkLookupTable.h>
#include <vtkBandedPolyDataContourFilter.h>
#include <vtkActor.h>
#include <vtkOpenGLActor.h>
#include <vtkMath.h>
#include <vtkCamera.h>
#include <vtkCenterOfMass.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataReader.h>
#include <vtkRenderWindow.h>
#include <vtkVectorText.h>
#include <vtkImageData.h>
#include <vtkCaptionActor2D.h>
#include <vtkProperty2D.h>
#include <vtkAxesActor.h>
#include <vtkPolyDataNormals.h>
#include <vtkPLYReader.h>
#include <vtkSTLReader.h>
#include <vtkCleanPolyData.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
//
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkCallbackCommand.h>
//#include <vtkAreaPicker.h>
#include <vtkRenderedAreaPicker.h>
#include <vtkLine.h>
#include <vtkProp3DCollection.h>

#include <QTemporaryFile>
#include <QSettings>
#include <QIcon>
#include <QFile>
#include <QTextStream>


#include <vtkTransform.h>
#include <vtkConeSource.h>
#include <vtkBoxWidget.h>
#include <vtkCommand.h>

//-----------------------------------------------------------------------------
//MeshTools* MeshTools::Instance = 0;

//-----------------------------------------------------------------------------
/*MeshTools* MeshTools::instance()
{
	return MeshTools::Instance;
}
int MeshTools::getTestInt()
{
	return MeshTools::testint;
}*/


class vtkMyCallback : public vtkCommand
{
public:
	static vtkMyCallback *New() { return new vtkMyCallback; }
	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{    // Here we use the vtkBoxWidget to transform the underlying coneActor   
		 // (by manipulating its transformation matrix).  
		vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
		vtkBoxWidget *widget = reinterpret_cast<vtkBoxWidget*>(caller);
		widget->GetTransform(t);    widget->GetProp3D()->SetUserTransform(t);
	}
};

//Select meshes, landmarks and tags ... first try!
void RubberBandSelect(vtkObject* caller,
	long unsigned int vtkNotUsed(eventId),
	void* vtkNotUsed(clientData),
	void* vtkNotUsed(callData))
{
	std::cout << "Pick." << std::endl;
	//vtkRenderedAreaPicker* areaPicker = static_cast<vtkRenderedAreaPicker*>(caller);
	
	vtkAreaPicker* areaPicker = static_cast<vtkAreaPicker*>(caller);
	//vtkPropCollection *pcoll = areaPicker->GetPickList();

	/*vtkSmartPointer<vtkPoints> myPoints = areaPicker->GetClipPoints();
	for (vtkIdType i = 0; i < myPoints->GetNumberOfPoints(); i++)
	{
		double pt[3];
		double ori[3] = { 0,0,1 };
		myPoints->GetPoint(i, pt);
		mqMeshToolsCore::instance()->CreateLandmark(pt, ori, 0);

	}*/
	
	//pcoll->GetNumberOfItems();
	
	vtkProp3DCollection* props = areaPicker->GetProp3Ds();
	cout << "Props Number of items:" << props->GetNumberOfItems() << endl;
	
		
	//props->PrintSelf(cout, vtkIndent(2));
	
	std::string action = "Rubber band actor selection-unselection";
	int something_to_store = 0;
	if (props->GetNumberOfItems() > 0) { something_to_store = 1; }
	
	props->InitTraversal();
	int Count = 0;

	if (something_to_store == 1)
	{
		Count = BEGIN_UNDO_SET(action);
	}
	
			
		
	
	props->InitTraversal();
	for (vtkIdType i = 0; i < props->GetNumberOfItems(); i++)
	{
		
		
		vtkProp3D *myprop3D = props->GetNextProp3D();
		std::cout << "Actor prop:  class name:" << myprop3D->GetClassName() << std::endl;

		std::string str1("vtkLMActor");
		if (str1.compare(myprop3D->GetClassName()) == 0)
		{
			vtkLMActor *myActor;
			myActor = vtkLMActor::SafeDownCast(myprop3D);
			

				if (myActor->GetSelected() == 0)
				{
					myActor->SaveState(Count);
					myActor->SetChanged(1);
					myActor->SetSelected(1);

				}
				else
				{
					myActor->SaveState(Count);
					myActor->SetChanged(1);
					myActor->SetSelected(0);

				}
			
		}
		std::string str2("vtkMTActor");
		if (str2.compare(myprop3D->GetClassName()) == 0)
		{
			vtkMTActor *myActor;
			myActor = vtkMTActor::SafeDownCast(myprop3D);
			
		
			if (myActor->IsInsideFrustum(areaPicker->GetFrustum()))
			{
				if (myActor->GetSelected() == 0)
				{
					myActor->SaveState(Count);
					myActor->SetChanged(1);
					myActor->SetSelected(1);

				}
				else
				{
					myActor->SaveState(Count);
					myActor->SetChanged(1);
					myActor->SetSelected(0);

				}
			}
		}
		
	}
	if (something_to_store == 1)
	{
		END_UNDO_SET();
	}
}



#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

// Constructor
MeshTools::MeshTools()
{
	this->MeshToolsCore =  new mqMeshToolsCore();

	//vtkUndoStack* undoStack = vtkUndoStack::New();
	this->ui = new Ui_MeshTools;
	this->ui->setupUi(this);
	this->MeshToolsCore->SetMainWindow(this);

	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "MorphoMuseuM", "MeshTools");
	cout<<".ini file path"<<  settings.fileName().toStdString()<<endl;
	settings.beginGroup("display_options");
	this->MeshToolsCore->Setmui_ShowGrid(settings.value("ShowGrid", 
		this->MeshToolsCore->Getmui_DefaultShowGrid()
		).toInt());
	this->MeshToolsCore->Setmui_ShowOrientationHelper(settings.value("ShowOrientationHelper", 
		this->MeshToolsCore->Getmui_DefaultShowOrientationHelper()
		).toInt());
	
	this->MeshToolsCore->Setmui_X1Label(settings.value("X1Label", this->MeshToolsCore->Getmui_DefaultX1Label()).toString());
	this->MeshToolsCore->Setmui_X2Label(settings.value("X2Label", this->MeshToolsCore->Getmui_DefaultX2Label()).toString());
	this->MeshToolsCore->Setmui_Y1Label(settings.value("Y1Label", this->MeshToolsCore->Getmui_DefaultY1Label()).toString());
	this->MeshToolsCore->Setmui_Y2Label(settings.value("Y2Label", this->MeshToolsCore->Getmui_DefaultY2Label()).toString());
	this->MeshToolsCore->Setmui_Z1Label(settings.value("Z1Label", this->MeshToolsCore->Getmui_DefaultZ1Label()).toString());
	this->MeshToolsCore->Setmui_Z2Label(settings.value("Z2Label", this->MeshToolsCore->Getmui_DefaultZ2Label()).toString());

	this->MeshToolsCore->Setmui_CameraCentreOfMassAtOrigin(settings.value("CameraCentreOfMassAtOrigin", 
		this->MeshToolsCore->Getmui_DefaultCameraCentreOfMassAtOrigin()
		).toInt());
	this->MeshToolsCore->Setmui_CameraOrtho(settings.value("CameraOrtho", 
		this->MeshToolsCore->Getmui_DefaultCameraOrtho()
		).toInt());
	settings.endGroup();
	settings.beginGroup("color_settings");
	double defaultMeshColor[4];
	double defaultFlagColor[3];
	double defaultBackGroundColor[3];
	double defaultBackGroundColor2[3];

	this->MeshToolsCore->Getmui_DefaultMeshColor(defaultMeshColor);
	this->MeshToolsCore->Getmui_DefaultFlagColor(defaultFlagColor);
	this->MeshToolsCore->Getmui_DefaultBackGroundColor(defaultBackGroundColor);
	this->MeshToolsCore->Getmui_DefaultBackGroundColor2(defaultBackGroundColor2);
	//cout << "defaultMeshColor[3]=" << defaultMeshColor[3] << endl;
	this->MeshToolsCore->Setmui_MeshColor(
	settings.value("MeshRed", 
		defaultMeshColor[0]
		).toDouble(),
	settings.value("MeshGreen", 
		defaultMeshColor[1]
		).toDouble(),
	settings.value("MeshBlue", 
		defaultMeshColor[2]
		).toDouble(),
	settings.value("MeshAlpha", 
		defaultMeshColor[3]
		).toDouble()
		);
	this->MeshToolsCore->Setmui_FlagColor(
		settings.value("FlagRed",
			defaultFlagColor[0]
			).toDouble(),
		settings.value("FlagGreen",
			defaultFlagColor[1]
			).toDouble(),
		settings.value("FlagBlue",
			defaultFlagColor[2]
			).toDouble()
		);
	this->MeshToolsCore->Setmui_BackGroundColor(
		settings.value("BackGroundRed", 
			defaultBackGroundColor[0]
			).toDouble(),
		settings.value("BackGroundGreen", 
			defaultBackGroundColor[1]
			).toDouble(),
		settings.value("BackGroundBlue", 
			defaultBackGroundColor[2]
			).toDouble()		
		);

	this->MeshToolsCore->Setmui_BackGroundColor2(
		settings.value("BackGroundRed2", 
			defaultBackGroundColor2[0]
			).toDouble(),
		settings.value("BackGroundGreen2", 
			defaultBackGroundColor2[1]
			).toDouble(),
		settings.value("BackGroundBlue2",
			defaultBackGroundColor2[2]
			).toDouble()
		);
	settings.endGroup();

	settings.beginGroup("landmark_settings");
	this->MeshToolsCore->Setmui_LandmarkBodyType(
		settings.value("LandmarkBodyType",
			this->MeshToolsCore->Getmui_DefaultLandmarkBodyType()
			).toInt()
			);

	this->MeshToolsCore->Setmui_LandmarkMode(
		settings.value("LandmarkMode",
			this->MeshToolsCore->Getmui_DefaultLandmarkMode()
			).toInt()
		);

	this->MeshToolsCore->Setmui_FlagRenderingSize(
		settings.value("FlagRenderingSize",
			this->MeshToolsCore->Getmui_DefaultFlagRenderingSize()
			).toDouble()
		);

	this->MeshToolsCore->Setmui_LandmarkRenderingSize(
		settings.value("LandmarkRenderingSize",
			this->MeshToolsCore->Getmui_DefaultLandmarkRenderingSize()
			).toDouble()
		);
	

	this->MeshToolsCore->Setmui_AdjustScaleFactor(
		settings.value("AdjustScaleFactor",
			this->MeshToolsCore->Getmui_DefaultAdjustScaleFactor()
			).toDouble()
		);
	cout << settings.value("AdjustScaleFactor",
		this->MeshToolsCore->Getmui_DefaultAdjustScaleFactor()
		).toDouble() << endl;
	settings.endGroup();

	

	

	


	
	
	// Place the table view in the designer form
	//this->ui->tableFrame->layout()->addWidget(this->TableView->GetWidget());

	this->MeshToolsCore->SetRenderWindow(this->ui->qvtkWidget->GetRenderWindow());


	this->ui->qvtkWidget->GetRenderWindow()->SetAlphaBitPlanes(1);
	this->ui->qvtkWidget->GetRenderWindow()->SetMultiSamples(0);
	
	//this->ui->qvtkWidget->GetRenderWindow()->SetStereoTypeToRedBlue();
	this->ui->qvtkWidget->GetRenderWindow()->SetStereoTypeToAnaglyph();
	this->ui->qvtkWidget->GetRenderWindow()->StereoCapableWindowOn();
	settings.beginGroup("renderer_settings");
	this->MeshToolsCore->Setmui_Anaglyph(settings.value("Anaglyph", "0").toInt());
	settings.endGroup();

	

	//this->ui->qvtkWidget->GetRenderWindow()->StereoRenderOn();
	

	// VTK/Qt wedded


	this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(this->MeshToolsCore->getRenderer());

	this->MeshToolsCore->getRenderer()->GradientBackgroundOn();
	
	this->MeshToolsCore->getRenderer()->SetBackground(this->MeshToolsCore->Getmui_BackGroundColor());
	this->MeshToolsCore->getRenderer()->SetBackground2(this->MeshToolsCore->Getmui_BackGroundColor2());
	//actor->SetmColor(this->MeshToolsCore->Getmui_MeshColor());
	//cout<< "Peeling was used:"<< this->MeshToolsCore->getRenderer()->GetLastRenderingUsedDepthPeeling();

	

	// 448/120 seems to be a good ratio!!! 3.73
	double multfactor = 1 / tan(this->MeshToolsCore->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);

	this->MeshToolsCore->getCamera()->SetPosition(120* multfactor, 0, 0);
	this->MeshToolsCore->getCamera()->SetFocalPoint(0, 0, 0);
	this->MeshToolsCore->getCamera()->SetViewUp(0, 0, 1);
	//double *viewup;
	//viewup= this->MeshToolsCore->getCamera()->GetViewUp();
	//cout << "Initial view up:" << viewup[0] << "," << viewup[1] << "," << viewup[2] << endl;
	/*this->MeshToolsCore->getCamera()->Azimuth(90);// > Roll(-90); // Around "z" (profondeur) viewing axis!
	this->MeshToolsCore->getCamera()->Roll(90); // around "x" (horizontal) viewing axis
	this->MeshToolsCore->getCamera()->Elevation(180); // around "y" (vertical) viewing axis
	*/
	this->MeshToolsCore->getCamera()->SetParallelScale(120); 
	this->MeshToolsCore->ResetCameraOrthoPerspective();
	//this->MeshToolsCore->getCamera()->ParallelProjectionOn();




	// Just a bit of Qt interest: Culling off the
	// point data and handing it to a vtkQtTableView
	/* VTK_CREATE(vtkDataObjectToTable, toTable);
	toTable->SetInputConnection(elevation->GetOutputPort());
	toTable->SetFieldType(vtkDataObjectToTable::POINT_DATA);
	toTable->Update();
	// Here we take the end of the VTK pipeline and give it to a Qt View
	this->TableView->SetRepresentationFromInputConnection(toTable->GetOutputPort());*/

	// Set up action signals and slots
	//connect(this->ui->actionOpenMesh, SIGNAL(triggered()), this, SLOT(slotOpenMeshFile()));
	

	
	

	
	connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));

	mqMeshToolsMenuBuilders::buildFileMenu(*this->ui->menuFile);
	mqMeshToolsMenuBuilders::buildEditMenu(*this->ui->menuEdit);
	mqMeshToolsMenuBuilders::buildHelpMenu(*this->ui->menuHelp);
	mqMeshToolsMenuBuilders::buildToolbars(*this);

	


	double myorigin[3];
	myorigin[0] = 0;
	myorigin[1] = 0;
	myorigin[2] = 0;

	this->MeshToolsCore->getGridActor()->SetOutlineMode(this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin());
	


	//@@ rubber band selection!
	
	 vtkSmartPointer<vtkMTInteractorStyle> style =
    vtkSmartPointer<vtkMTInteractorStyle>::New();
	 style->SetActorCollection(this->MeshToolsCore->getActorCollection());
	 style->SetNormalLandmarkCollection(this->MeshToolsCore->getNormalLandmarkCollection());
	 style->SetTargetLandmarkCollection(this->MeshToolsCore->getTargetLandmarkCollection());
	 style->SetNodeLandmarkCollection(this->MeshToolsCore->getNodeLandmarkCollection());
	 style->SetHandleLandmarkCollection(this->MeshToolsCore->getHandleLandmarkCollection());
	 style->SetFlagLandmarkCollection(this->MeshToolsCore->getFlagLandmarkCollection());
	/*vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New(); //like paraview*/
	//vtkSmartPointer<vtkInteractorStyleTrackballActor> style =
	//	vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();
	//vtkSmartPointer<vtkInteractorStyleSwitch> style =
	//	vtkSmartPointer<vtkInteractorStyleSwitch>::New();
	 vtkSmartPointer<vtkCallbackCommand> pickCallback =
		 vtkSmartPointer<vtkCallbackCommand>::New();

	 pickCallback->SetCallback(RubberBandSelect);
	 this->AreaPicker =
		 vtkSmartPointer<vtkAreaPicker>::New();

		  /*this->AreaPicker =
		 vtkSmartPointer<vtkRenderedAreaPicker>::New();*/
	 this->AreaPicker->AddObserver(vtkCommand::EndPickEvent, pickCallback);
	
// style->SetCurrentRenderer(this->MeshToolsCore->getRenderer());
  this->ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetPicker(this->AreaPicker);
  this->ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
  
  //EXAMPLE vtkBoxWidget
  /*
  vtkSmartPointer<vtkConeSource> coneSource =
	  vtkSmartPointer<vtkConeSource>::New();
  coneSource->SetHeight(1.5);
  vtkSmartPointer<vtkPolyDataMapper> mapper =
	  vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(coneSource->GetOutputPort());
  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  //NEW

  //NEW
  //DNEW
  this->MeshToolsCore->getRenderer()->AddActor(actor);

  vtkSmartPointer<vtkBoxWidget> boxWidget = vtkSmartPointer<vtkBoxWidget>::New();
  boxWidget->SetInteractor(this->ui->qvtkWidget->GetRenderWindow()->GetInteractor());
  boxWidget->SetProp3D(actor);
  boxWidget->SetPlaceFactor(1.25);
  boxWidget->PlaceWidget();
  vtkSmartPointer<vtkMyCallback> callback = vtkSmartPointer<vtkMyCallback>::New();
  boxWidget->AddObserver(vtkCommand::InteractionEvent, callback);
  boxWidget->On();*/


  
  

  //EXAMPLE BEZIER SOURCE + BEZIER WIDGET
  /*
  unsigned int controlPointsX = 4;
  unsigned int controlPointsY = 4;
  
  // Create an arbitrary plane that we can use
  // to initialize the control points for the Bezier surface
  vtkSmartPointer<vtkPlaneSource> planeSource =
	  vtkSmartPointer<vtkPlaneSource>::New();
  planeSource->SetOrigin(0, 0, 0);
  planeSource->SetPoint2(1, 0, 0);
  planeSource->SetPoint1(0, 0, 1);
  planeSource->SetXResolution(controlPointsX - 1);
  planeSource->SetYResolution(controlPointsY - 1);
  planeSource->Update();

  // Create a transformation to be applied to the Bezier surface
  vtkSmartPointer<vtkTransform> transform =
	  vtkSmartPointer<vtkTransform>::New();
  transform->Scale(2, 1, 1);

  // Create the Bezier surface that matches the plane created above
  // and scaled by the transformation
  vtkSmartPointer<vtkBezierSurfaceSource> bezierSource =
	  vtkSmartPointer<vtkBezierSurfaceSource>::New();
  bezierSource->SetNumberOfControlPoints(controlPointsX, controlPointsY);

  vtkPolyData* planePoly = planeSource->GetOutput();

  unsigned int controlPointCount = 0;
  for (unsigned int i = 0; i < controlPointsX; ++i)
  {
	  for (unsigned int j = 0; j < controlPointsY; ++j)
	  {
		  double point[3];
		  planePoly->GetPoint(i*controlPointsY + j, point);
		  //          std::cout << "Point " << controlPointCount << ": "
		  //                    << point[0] << " " << point[1] << " " << point[2] << std::endl;
		  bezierSource->SetControlPoint(i, j, point);
		  controlPointCount++;
	  }
  }

  std::cout << "Created " << controlPointCount << " control points." << std::endl;

  bezierSource->SetTransform(transform);
  bezierSource->Update();
  
  vtkBezierSurfaceWidget *widget = vtkBezierSurfaceWidget::New();
	  
  widget->SetInteractor(this->ui->qvtkWidget->GetRenderWindow()->GetInteractor());
  widget->SetBezierSource(bezierSource);
  widget->SetHandleSize(0.1);
  widget->On();

  vtkSmartPointer<vtkPolyDataMapper> Bmapper =
	  vtkSmartPointer<vtkPolyDataMapper>::New();
  Bmapper->SetInputConnection(bezierSource->GetOutputPort());

  vtkSmartPointer<vtkActor> Bactor =
	  vtkSmartPointer<vtkActor>::New();
  Bactor->SetMapper(Bmapper);
  
  this->MeshToolsCore->getRenderer()->AddActor(Bactor);
  */

  this->MeshToolsCore->SetGridVisibility();
  this->MeshToolsCore->InitializeOrientationHelper(); // creates orientation helper...
  this->MeshToolsCore->SetOrientationHelperVisibility();

  //@@ end rubber band selection!


};



MeshTools::~MeshTools()
{
	/*if (MeshTools::Instance == this)
	{
		MeshTools::Instance = 0;
	}*/
	saveSettings();
	
	// The smart pointers should clean up for up
	//this->OrientationHelperWidget->Delete();
}


void MeshTools::saveSettings()
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "MorphoMuseuM", "MeshTools");
	//cout<<"try save settings:" << m_sSettingsFile.toStdString();
	settings.beginGroup("display_options");
	settings.setValue("ShowGrid", this->MeshToolsCore->Getmui_ShowGrid());
	settings.setValue("ShowOrientationHelper", this->MeshToolsCore->Getmui_ShowOrientationHelper());
	settings.setValue("CameraCentreOfMassAtOrigin", this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin());
	settings.setValue("CameraOrtho", this->MeshToolsCore->Getmui_CameraOrtho());

	cout << "X1=" << this->MeshToolsCore->Getmui_X1Label().toStdString() << endl;
	cout << "X2=" << this->MeshToolsCore->Getmui_X2Label().toStdString() << endl;
	cout << "Y1=" << this->MeshToolsCore->Getmui_Y1Label().toStdString() << endl;
	cout << "Y2=" << this->MeshToolsCore->Getmui_Y2Label().toStdString() << endl;
	cout << "Z1=" << this->MeshToolsCore->Getmui_Z1Label().toStdString() << endl;
	cout << "Z2=" << this->MeshToolsCore->Getmui_Z2Label().toStdString() << endl;
	settings.setValue("X1Label", this->MeshToolsCore->Getmui_X1Label());
	settings.setValue("X2Label", this->MeshToolsCore->Getmui_X2Label());
	settings.setValue("Y1Label", this->MeshToolsCore->Getmui_Y1Label());
	settings.setValue("Y2Label", this->MeshToolsCore->Getmui_Y2Label());
	settings.setValue("Z1Label", this->MeshToolsCore->Getmui_Z1Label());
	settings.setValue("Z2Label", this->MeshToolsCore->Getmui_Z2Label());
	settings.endGroup();	

	settings.beginGroup("color_settings");
	settings.setValue("BackGroundRed", this->MeshToolsCore->Getmui_BackGroundColor()[0]);
	settings.setValue("BackGroundGreen", this->MeshToolsCore->Getmui_BackGroundColor()[1]);
	settings.setValue("BackGroundBlue", this->MeshToolsCore->Getmui_BackGroundColor()[2]);

	settings.setValue("BackGroundRed2", this->MeshToolsCore->Getmui_BackGroundColor2()[0]);
	settings.setValue("BackGroundGreen2", this->MeshToolsCore->Getmui_BackGroundColor2()[1]);
	settings.setValue("BackGroundBlue2", this->MeshToolsCore->Getmui_BackGroundColor2()[2]);


	settings.setValue("MeshRed", this->MeshToolsCore->Getmui_MeshColor()[0]);
	settings.setValue("MeshGreen", this->MeshToolsCore->Getmui_MeshColor()[1]);
	settings.setValue("MeshBlue", this->MeshToolsCore->Getmui_MeshColor()[2]);
	settings.setValue("MeshAlpha", this->MeshToolsCore->Getmui_MeshColor()[3]);

	settings.setValue("FlagRed", this->MeshToolsCore->Getmui_FlagColor()[0]);
	settings.setValue("FlagGreen", this->MeshToolsCore->Getmui_FlagColor()[1]);
	settings.setValue("FlagBlue", this->MeshToolsCore->Getmui_FlagColor()[2]);
	

	

	settings.endGroup();
	
	settings.beginGroup("landmark_settings");
	settings.setValue("LandmarkBodyType", this->MeshToolsCore->Getmui_LandmarkBodyType());
	settings.setValue("LandmarkMode", this->MeshToolsCore->Getmui_LandmarkMode());
	settings.setValue("LandmarkRenderingSize", this->MeshToolsCore->Getmui_LandmarkRenderingSize());
	settings.setValue("FlagRenderingSize", this->MeshToolsCore->Getmui_FlagRenderingSize());
	
	settings.setValue("AdjustLandmarkRenderingSize", this->MeshToolsCore->Getmui_AdjustLandmarkRenderingSize());
	settings.setValue("AdjustScaleFactor", this->MeshToolsCore->Getmui_AdjustScaleFactor());
	
	settings.endGroup();
	settings.beginGroup("renderer_settings");
	settings.setValue("Anaglyph", this->MeshToolsCore->Getmui_Anaglyph());	
	settings.endGroup();
	cout << "end save settings" << endl;
}





// problème
// aujourd'hui, c'est cette seule fonction qui est appelée lorsqu'on ouvre un nouveau maillage
// et lorsqu'on change entre "camera



void MeshTools::slotExit() {
	//maybe we should save the .ini files!
	qApp->exit();
}





