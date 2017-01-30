


#include "ui_MeshTools.h"
#include "MeshTools.h"
#include "vtkOrientationHelperActor.h"
#include "vtkOrientationHelperWidget.h"
#include "mqMeshToolsMenuBuilders.h"
#include "vtkMTActor.h"
#include "vtkLMActor.h"
#include "mqMeshToolsCore.h"
#include "mqUndoStack.h"
#include "vtkMTInteractorStyle.h"
#include "vtkMTActorCollection.h"
//#include "vtkUndoStack.h"
//#include "vtkUndoSet.h"
//#include "vtkUndoElement.h"
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
#include <vtkQtTableView.h>
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
#include <vtkAreaPicker.h>
#include <vtkLine.h>
#include <vtkProp3DCollection.h>

#include <QTemporaryFile>
#include <QSettings>
#include <QIcon>

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

//Select meshes, landmarks and tags ... first try!
void RubberBandSelect(vtkObject* caller,
	long unsigned int vtkNotUsed(eventId),
	void* vtkNotUsed(clientData),
	void* vtkNotUsed(callData))
{
	std::cout << "Pick." << std::endl;
	vtkAreaPicker* areaPicker = static_cast<vtkAreaPicker*>(caller);
	vtkPropCollection *pcoll = areaPicker->GetPickList();
	pcoll->GetNumberOfItems();
	cout << "Pcoll Number of items:" << pcoll->GetNumberOfItems() << endl;

	vtkProp3DCollection* props = areaPicker->GetProp3Ds();
	
	//props->PrintSelf(cout, vtkIndent(2));
	
	std::string action = "Rubber band actor selection-unselection";
	int something_to_store = 0;
	if (props->GetNumberOfItems() > 0) { something_to_store = 1; }
	cout << "Number of items:" << props->GetNumberOfItems()<<endl;
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
	

	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "MorphoMuseuM", "MeshTools");
	cout<<".ini file path"<<  settings.fileName().toStdString()<<endl;
	settings.beginGroup("display_options");
	this->MeshToolsCore->Setmui_ShowGrid(settings.value("ShowGrid", 
		this->MeshToolsCore->Getmui_DefaultShowGrid()
		).toInt());
	this->MeshToolsCore->Setmui_ShowOrientationHelper(settings.value("ShowOrientationHelper", 
		this->MeshToolsCore->Getmui_DefaultShowOrientationHelper()
		).toInt());
	this->MeshToolsCore->Setmui_CameraCentreOfMassAtOrigin(settings.value("CameraCentreOfMassAtOrigin", 
		this->MeshToolsCore->Getmui_DefaultCameraCentreOfMassAtOrigin()
		).toInt());
	this->MeshToolsCore->Setmui_CameraOrtho(settings.value("CameraOrtho", 
		this->MeshToolsCore->Getmui_DefaultCameraOrtho()
		).toInt());
	settings.endGroup();
	settings.beginGroup("color_settings");
	double defaultMeshColor[4];
	double defaultBackGroundColor[3];
	double defaultBackGroundColor2[3];

	this->MeshToolsCore->Getmui_DefaultMeshColor(defaultMeshColor);
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

	if (this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
	
		this->ui->actionCameraCentreOfMassToggle->setChecked(true);
	}

	if (this->MeshToolsCore->Getmui_CameraOrtho() == 0)
	{

		this->ui->actionCameraOrthoPerspectiveToggle->setChecked(true);
	}

	

	


	mqMeshToolsMenuBuilders::buildFileMenu(*this->ui->menuFile);
	mqMeshToolsMenuBuilders::buildEditMenu(*this->ui->menuEdit);
	mqMeshToolsMenuBuilders::buildHelpMenu(*this->ui->menuHelp);
	mqMeshToolsMenuBuilders::buildToolbars(*this);
	this->OrientationHelperWidget = vtkOrientationHelperWidget::New();
	// Qt Table View
	this->TableView = vtkSmartPointer<vtkQtTableView>::New();
	

	


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
	if (this->MeshToolsCore->Getmui_Anaglyph() == 1)
	{

		this->ui->actionRendererAnaglyphToggle->setChecked(true);
	}
	

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
	this->ResetCameraOrthoPerspective();
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
	connect(this->ui->actionOpenMesh, SIGNAL(triggered()), this, SLOT(slotOpenMeshFile()));
	connect(this->ui->actionCameraFront, SIGNAL(triggered()), this, SLOT(slotCameraFront()));
	connect(this->ui->actionCameraBack, SIGNAL(triggered()), this, SLOT(slotCameraBack()));
	connect(this->ui->actionCameraLeft, SIGNAL(triggered()), this, SLOT(slotCameraLeft()));
	connect(this->ui->actionCameraRight, SIGNAL(triggered()), this, SLOT(slotCameraRight()));
	connect(this->ui->actionCameraAbove, SIGNAL(triggered()), this, SLOT(slotCameraAbove()));
	connect(this->ui->actionCameraBelow, SIGNAL(triggered()), this, SLOT(slotCameraBelow()));
	connect(this->ui->actionGridToggle, SIGNAL(triggered()), this, SLOT(slotGridToggle()));
	connect(this->ui->actionRendererAnaglyphToggle, SIGNAL(triggered()), this, SLOT(slotRendererAnaglyphToggle()));
	connect(this->ui->actionOrientationHelperToggle, SIGNAL(triggered()), this, SLOT(slotOrientationHelperToggle()));
	connect(this->ui->actionCameraCentreOfMassToggle, SIGNAL(triggered()), this, SLOT(slotCameraCentreOfMassToggle()));
	connect(this->ui->actionCameraOrthoPerspectiveToggle, SIGNAL(triggered()), this, SLOT(slotCameraOrthoPerspectiveToggle()));

	connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));


	vtkSmartPointer<vtkOrientationHelperActor> axes =
		vtkSmartPointer<vtkOrientationHelperActor>::New();

	//

	//vtkOrientationMarkerWidget* widget = vtkOrientationMarkerWidget::New();
	// Does not work with a smart pointer, can't figure out why


	this->OrientationHelperWidget->SetOutlineColor(0.9300, 0.5700, 0.1300);
	this->OrientationHelperWidget->SetOrientationMarker(axes);
	this->OrientationHelperWidget->SetDefaultRenderer(this->MeshToolsCore->getRenderer());
	this->OrientationHelperWidget->SetInteractor(this->ui->qvtkWidget->GetRenderWindow()->GetInteractor());
	this->OrientationHelperWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
	this->OrientationHelperWidget->SetEnabled(1);
	this->OrientationHelperWidget->InteractiveOff();
	this->OrientationHelperWidget->PickingManagedOn();
	double myorigin[3];
	myorigin[0] = 0;
	myorigin[1] = 0;
	myorigin[2] = 0;

	this->MeshToolsCore->getGridActor()->SetOutlineMode(this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin());
	


	//@@ rubber band selection!
	
	 vtkSmartPointer<vtkMTInteractorStyle> style =
    vtkSmartPointer<vtkMTInteractorStyle>::New();
	 style->SetActorCollection(this->MeshToolsCore->getActorCollection());
	 style->SetLandmarkCollection(this->MeshToolsCore->getLandmarkCollection());
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

	 this->AreaPicker->AddObserver(vtkCommand::EndPickEvent, pickCallback);
	
// style->SetCurrentRenderer(this->MeshToolsCore->getRenderer());
  this->ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetPicker(this->AreaPicker);
  this->ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
  

  this->SetGridVisibility();
  this->SetOrientationHelperVisibility();

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

/*void MeshTools::SetUndoCount(int Count)
{
	this->UndoCount = Count;

}
int MeshTools::GetUndoCount()
{
	return this->UndoCount;

}*/

void MeshTools::saveSettings()
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "MorphoMuseuM", "MeshTools");
	//cout<<"try save settings:" << m_sSettingsFile.toStdString();
	settings.beginGroup("display_options");
	settings.setValue("ShowGrid", this->MeshToolsCore->Getmui_ShowGrid());
	settings.setValue("ShowOrientationHelper", this->MeshToolsCore->Getmui_ShowOrientationHelper());
	settings.setValue("CameraCentreOfMassAtOrigin", this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin());
	settings.setValue("CameraOrtho", this->MeshToolsCore->Getmui_CameraOrtho());
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
	settings.endGroup();
	
	settings.beginGroup("renderer_settings");
	settings.setValue("Anaglyph", this->MeshToolsCore->Getmui_Anaglyph());	
	settings.endGroup();
	cout << "end save settings" << endl;
}


//On ajoute un indice au nom si le nom existe déjà.
//fonction recurente pour savoir quel indice lui donner.
std::string  MeshTools::CheckingName(std::string name_obj, int cpt_name) {
	std::string s_cpt_name = std::to_string(cpt_name);
	std::string name = name_obj + "(" + s_cpt_name + ")";
	// to do!!!

	return name_obj;
}

void MeshTools::UpdateRenderer()
{
	//remove all actors.
	//this->MeshToolsCore->getRenderer()->RemoveAllViewProps();
	//for (vtkIdType i = 0; i < this->MeshToolsCore->getActorCollection()->GetNumberOfItems(); i++)
	//{
	//vtkActor* actor = this->MeshToolsCore->getActorCollection()->GetLastActor();				
	//this->MeshToolsCore->getRenderer()->AddActor(actor);
	//}
}




// Action to be taken upon file open
void MeshTools::slotOpenMeshFile()
{

	std::string SfileName;
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Load surface"), QDir::currentPath(),
		tr("surfaces (*.ply *.stl *.vtk)"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;
	int file_exists = 1;


	QFile file(fileName);
	QString name = "";
	if (file.exists()) {
		// Message
		name = file.fileName(); // Return only a file name		
		file.close();
	}
	else
	{
		file_exists = 0;

	}


	if (file_exists == 1)
	{
		std::string STLext(".stl");
		std::string STLext2(".STL");
		std::string VTKext(".vtk");
		std::string VTKext2(".VTK");
		std::string OBJext(".obj");
		std::string OBJext2(".OBJ");
		std::string PLYext(".ply");
		std::string PLYext2(".PLY");

		int type = 0; //0 = stl, 1 = vtk, 2 = obj, 3 = ply
		std::size_t found = fileName.toStdString().find(STLext);
		std::size_t found2 = fileName.toStdString().find(STLext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 0;
			//STL
		}

		//std::cout << "0Type= " <<type<< std::endl;
		found = fileName.toStdString().find(VTKext);
		found2 = fileName.toStdString().find(VTKext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 1; //VTK
		}

		//std::cout << "2Type= " <<type<< std::endl;
		found = fileName.toStdString().find(PLYext);
		found2 = fileName.toStdString().find(PLYext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 2; //PLY
		}

		// Read and display for verification

		vtkSmartPointer<vtkPolyData> MyPolyData = vtkSmartPointer<vtkPolyData>::New();

		if (type == 0)
		{

			vtkSmartPointer<vtkSTLReader> reader =
				vtkSmartPointer<vtkSTLReader>::New();

			reader->SetFileName(fileName.toStdString().c_str());
			reader->Update();
			MyPolyData = reader->GetOutput();
		}

		else if (type == 1)
		{

			vtkSmartPointer<vtkPolyDataReader> reader =
				vtkSmartPointer<vtkPolyDataReader>::New();
			reader->SetFileName(fileName.toStdString().c_str());
			reader->Update();
			MyPolyData = reader->GetOutput();
		}
		else
		{

			vtkSmartPointer<vtkPLYReader> reader =
				vtkSmartPointer<vtkPLYReader>::New();
			reader->SetFileName(fileName.toStdString().c_str());
			reader->Update();
			MyPolyData = reader->GetOutput();
		}
		std::cout << "\nNumber of points 1:" << MyPolyData->GetNumberOfPoints() << std::endl;
		std::cout << "\nNumber of cells 1:" << MyPolyData->GetNumberOfCells() << std::endl;


		vtkSmartPointer<vtkPolyDataNormals> ObjNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
		ObjNormals->SetInputData(MyPolyData);
		ObjNormals->ComputePointNormalsOn();
		ObjNormals->ComputeCellNormalsOn();
		//ObjNormals->AutoOrientNormalsOff();
		ObjNormals->ConsistencyOff();

		ObjNormals->Update();

		vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
		cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());
		cleanPolyDataFilter->PieceInvariantOff();
		cleanPolyDataFilter->ConvertLinesToPointsOff();
		cleanPolyDataFilter->ConvertPolysToLinesOff();
		cleanPolyDataFilter->ConvertStripsToPolysOff();
		cleanPolyDataFilter->PointMergingOn();
		cleanPolyDataFilter->Update();

		MyPolyData = cleanPolyDataFilter->GetOutput();

		cout << "\nNumber of points:" << MyPolyData->GetNumberOfPoints() << std::endl;
		cout << "\nNumber of cells:" << MyPolyData->GetNumberOfCells() << std::endl;

		MyPolyData->GetCellData();

		vtkFloatArray* norms = vtkFloatArray::SafeDownCast(MyPolyData->GetCellData()->GetNormals());
		cout << "Safe cell downcast done ! " << endl;
		if (norms)
		{

			cout << "There are here " << norms->GetNumberOfTuples()
				<< " Float Cell normals in norms" << endl;
		}
		else
		{
			cout << "FloatNorms CELL is null " << endl;
		}

		norms = vtkFloatArray::SafeDownCast
			(MyPolyData->GetPointData()->GetNormals());
		cout << "Safe point downcast done ! " << endl;
		if (norms)
		{

			cout << "There are  " << norms->GetNumberOfTuples()
				<< " Float POINT normals in norms" << endl;
		}
		else
		{
			cout << "FloatNorms POINTS is null " << endl;
		}

		if (MyPolyData->GetNumberOfPoints() > 10)
		{


			std::string newname = name.toStdString();

			int nPos = newname.find_first_of(".");
			if (nPos > -1)
			{
				newname = newname.substr(0, nPos);
			}
			// to change... 
			newname = CheckingName(newname, 0);
			if ((vtkUnsignedCharArray*)MyPolyData->GetPointData()->GetScalars("RGB") != NULL)
			{
				MyPolyData->GetPointData()->SetScalars(NULL);
				cout << "found RGB colours! "<<endl;
			}
			/*
			vtkSmartPointer<vtkUnsignedCharArray> newcolors =
			vtkSmartPointer<vtkUnsignedCharArray>::New();
			newcolors->SetNumberOfComponents(4);
			newcolors->SetNumberOfTuples(numpoints);
			//ici init_RGB ou RGB_i
			if ((vtkUnsignedCharArray*)MyObj->GetPointData()->GetScalars("RGB") != NULL) {
			newcolors->DeepCopy((vtkUnsignedCharArray*)MyObj->GetPointData()->GetScalars("RGB"));

			for (int i = 0; i < numpoints; i++)
			{
			if (i < 100)
			{
			cout << newcolors->GetComponent(i, 0) << "," << newcolors->GetComponent(i, 1)
			<< "," << newcolors->GetComponent(i, 2) << std::endl;
			}
			//newcolors->SetComponent(i, 3, 255.);

			}

			cout << "found RGB colours: ";
			newcolors->SetName("Init_RGB");
			My_Obj->GetPointData()->AddArray(newcolors);
			}
			
			*/

			// Mapper
			VTK_CREATE(vtkPolyDataMapper, mapper);
			mapper->ImmediateModeRenderingOn();
			//mapper->SetColorModeToDirectScalars();
			mapper->SetColorModeToDefault();
			mapper->SetInputData(MyPolyData);
			//VTK_CREATE(vtkActor, actor);
			//cout << "create LM2 Actor" << endl;
			//VTK_CREATE(vtkLM2Actor, actor);
			VTK_CREATE(vtkMTActor, actor);
			int num = 2;
			/*actor->SetLMOrigin(0, 0, 0);
			cout << "create LM origin" << endl;
			actor->SetLMOrientation(0, 0, 1);
			cout << "create LM orientation" << endl;
			actor->SetLMSize(0.1);
			cout << "create LM Size" << endl;
			actor->SetLMType(1);
			cout << "create LM Type" << endl;
			actor->SetLMNumber(num);
			cout << "create LM Actor" << endl;
			actor->SetLMBodyType(0);*/


			//VTK_CREATE(vtkOpenGLActor, actor);
			actor->SetmColor(this->MeshToolsCore->Getmui_MeshColor());
			//actor->GetProperty()->SetColor(this->MeshToolsCore->Getmui_MeshColor()[0], this->MeshToolsCore->Getmui_MeshColor()[1], this->MeshToolsCore->Getmui_MeshColor()[2]);
			//actor->GetProperty()->SetOpacity(this->MeshToolsCore->Getmui_MeshColor()[3]);
			actor->SetSelected(0);
			actor->SetMapper(mapper);
			//Now the add actor to renderer is managed inside the collection
			//this->MeshToolsCore->getRenderer()->AddActor(actor);
			this->MeshToolsCore->getActorCollection()->AddItem(actor);
			cout << "actor added to collection" << endl;
			this->MeshToolsCore->getActorCollection()->SetChanged(1);
			cout << "actor collection changed" << endl;

			//double BoundingBoxLength = MyPolyData->GetLength();
			this->AdjustCameraAndGrid();
			cout << "camera and grid adjusted" << endl;
			/*
			double bounds[6];
			MyPolyData->GetBounds(bounds);
			vtkSmartPointer<vtkElevationFilter> elevation =
				vtkSmartPointer<vtkElevationFilter>::New();
			elevation->SetInputData(MyPolyData);
			elevation->SetLowPoint(0, bounds[2], 0);
			elevation->SetHighPoint(0, bounds[3], 0);
			elevation->Update();
			vtkSmartPointer<vtkBandedPolyDataContourFilter> bcf =
				vtkSmartPointer<vtkBandedPolyDataContourFilter>::New();
			bcf->SetInputConnection(elevation->GetOutputPort());
			bcf->SetScalarModeToValue();
			bcf->GenerateContourEdgesOn();
			
			bcf->GenerateValues(10, elevation->GetScalarRange());

			bcf->Update();
			//bcf->GetNumberOfContours();
			vtkSmartPointer<vtkPolyDataMapper> contourLineMapper =
				vtkSmartPointer<vtkPolyDataMapper>::New();
			contourLineMapper->SetInputData(bcf->GetContourEdgesOutput());
			
			cout<<"Number of contours:"<< bcf->GetNumberOfContours();
			
			contourLineMapper->SetScalarRange(elevation->GetScalarRange());
			contourLineMapper->SetResolveCoincidentTopologyToPolygonOffset();
			contourLineMapper->Update();
			
			vtkSmartPointer<vtkMTActor> contourLineActor =
				vtkSmartPointer<vtkMTActor>::New();
			contourLineActor->SetMapper(contourLineMapper);
			contourLineActor->GetProperty()->SetColor(0.5, 0.5, 1.0);
			this->MeshToolsCore->getRenderer()->AddActor(contourLineActor);

			vtkSmartPointer<vtkPolyDataMapper> mapper2 =
				vtkSmartPointer<vtkPolyDataMapper>::New();
			mapper2->SetInputConnection(bcf->GetOutputPort());
			mapper2->SetScalarModeToUseCellData();
			//mapper2->SetScalarRange(0, 1);
			vtkSmartPointer<vtkMTActor> actor2=
				vtkSmartPointer<vtkMTActor>::New();
			actor2->SetMapper(mapper2);
			this->MeshToolsCore->getRenderer()->AddActor(actor2);
			*/
			/*if (this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin() == 0)
			{
				double globalcenterofmass[3];
				this->GetGlobalCenterOfMass(globalcenterofmass);
				cout << "Center of mass of all opened mesh is " << globalcenterofmass[0] << " " << globalcenterofmass[1] << " " << globalcenterofmass[2] << endl;

				double GlobalBoundingBoxLength = this->GetGlobalBoundingBoxLength();
				cout << "Global Bounding Box length is " << GlobalBoundingBoxLength << " mm" << endl;

				double campos[3];
				this->MeshToolsCore->getCamera()->GetPosition(campos);
				double camfocalpoint[3];
				this->MeshToolsCore->getCamera()->GetFocalPoint(camfocalpoint);
				double camscale = this->MeshToolsCore->getCamera()->GetParallelScale();

				double movex, movey, movez;
				movex = (campos[0] - camfocalpoint[0])*GlobalBoundingBoxLength / camscale;
				movey = (campos[1] - camfocalpoint[1])*GlobalBoundingBoxLength / camscale;
				movez = (campos[2] - camfocalpoint[2])*GlobalBoundingBoxLength / camscale;
				this->MeshToolsCore->getCamera()->SetPosition
					(globalcenterofmass[0] + movex,
						globalcenterofmass[1] + movey,
						globalcenterofmass[2] + movez);
				//this->MeshToolsCore->getCamera()->SetPosition(center[0] + GlobalBoundingBoxLength, center[1], center[2]);
				this->MeshToolsCore->getCamera()->SetFocalPoint(globalcenterofmass[0], globalcenterofmass[1], globalcenterofmass[2]);
				this->MeshToolsCore->getCamera()->SetParallelScale(GlobalBoundingBoxLength);
			}*/
			//this->MeshToolsCore->getCamera()->ParallelProjectionOn();


			//this->UpdateRenderer();

			//My_Obj = Cont_Mesh.Mesh_PDcontainerload(MyObj, (char*)newname.c_str());

			/*My_Obj->Set_Active_Scalar();
			int numpoints = My_Obj->GetNumberOfPoints();
			int numtriangles = My_Obj->GetNumberOfCells();
			std::cout << "Number of points:" << numpoints << std::endl;
			std::cout << "Number of cells:" << numtriangles << std::endl;

			//std::cout << "2 Mean x:"<<My_Obj->mean[0]<< "Mean y:"<<My_Obj->mean[1]<< "Mean z:"<<My_Obj->mean[2]<< std::endl;

			//std::cout << "3 Mean x:"<<My_Obj->mean[0]<< "Mean y:"<<My_Obj->mean[1]<< "Mean z:"<<My_Obj->mean[2]<< std::endl;

			My_Obj->selected = 1;


			cout << "color init: ";
			vtkSmartPointer<vtkUnsignedCharArray> newcolors =
			vtkSmartPointer<vtkUnsignedCharArray>::New();
			newcolors->SetNumberOfComponents(4);
			newcolors->SetNumberOfTuples(numpoints);
			//ici init_RGB ou RGB_i
			if ((vtkUnsignedCharArray*)MyObj->GetPointData()->GetScalars("RGB") != NULL) {
			newcolors->DeepCopy((vtkUnsignedCharArray*)MyObj->GetPointData()->GetScalars("RGB"));

			for (int i = 0; i < numpoints; i++)
			{
			if (i < 100)
			{
			cout << newcolors->GetComponent(i, 0) << "," << newcolors->GetComponent(i, 1)
			<< "," << newcolors->GetComponent(i, 2) << std::endl;
			}
			//newcolors->SetComponent(i, 3, 255.);

			}

			cout << "found RGB colours: ";
			newcolors->SetName("Init_RGB");
			My_Obj->GetPointData()->AddArray(newcolors);
			}
			cout << "ok." << endl;

			My_Obj->color[0] = color_obj[0];
			My_Obj->color[1] = color_obj[1];
			My_Obj->color[2] = color_obj[2];
			My_Obj->color[3] = 1;

			My_Obj->bool_init_buf = 0;
			// Only update RGB if not exists!

			vtkUnsignedCharArray* test = (vtkUnsignedCharArray*)My_Obj->GetPointData()->GetScalars("RGB");
			if (test == NULL)
			{
			My_Obj->Update_RGB();
			}


			//std::cout << "4 Mean x:"<<My_Obj->mean[0]<< "Mean y:"<<My_Obj->mean[1]<< "Mean z:"<<My_Obj->mean[2]<< std::endl;


			//Move object at center of mass only in some cases
			if (g_move_cm == 1)
			{
			My_Obj->Mat2[3][0] = -My_Obj->mean[0];
			My_Obj->Mat2[3][1] = -My_Obj->mean[1];
			My_Obj->Mat2[3][2] = -My_Obj->mean[2];
			}

			this->Compute_Global_Mean(0);
			if (g_landmark_auto_rendering_size)
			{
			this->Adjust_landmark_rendering_size();
			}
			this->Compute_Global_Scalar_List();

			}
			cout << "Reinitialize camera" << endl;
			rollinit_camera();
			cout << "G_zoom after initialization:" << g_zoom << endl;
			this->redraw();
			cout << "G_zoom after redraw:" << g_zoom << endl;*/

		}
	}


	/*	if (fileName.isEmpty()) return;

	//if (img.loadImage(fileName.toStdString().c_str()))

	fileName = QFileDialog::getOpenFileName(this,
	tr("Open File"), "/home/jana", tr("Surface Files (*.vtk *.stl *.ply)"));
	VTK_CREATE(vtkActor, actor);
	actor->GetProperty()->SetColor(0.5, 1, 0.5);
	actor->GetProperty()->SetOpacity(0.5);*/
}

// problème
// aujourd'hui, c'est cette seule fonction qui est appelée lorsqu'on ouvre un nouveau maillage
// et lorsqu'on change entre "camera

//Called to ajust camera and grid positions (distance etc...) after opening a new mesh.
void MeshTools::AdjustCameraAndGrid()
{
	cout << "start adjust camera and grid" << endl;
	double newcamerafocalpoint[3] = { 0,0,0 };
	if (this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		cout << "this is it no ? " << endl;
		this->MeshToolsCore->getActorCollection()->GetCenterOfMass(newcamerafocalpoint);
		cout << "no obviously" << endl;
		this->MeshToolsCore->getGridActor()->SetGridOrigin(newcamerafocalpoint);


	}
	cout << "New camera focal point:" << newcamerafocalpoint[0] << " " << newcamerafocalpoint[1] << " " << newcamerafocalpoint[2] << endl;

	double multfactor = 1 / tan(this->MeshToolsCore->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	double GlobalBoundingBoxLength = this->MeshToolsCore->getActorCollection()->GetBoundingBoxLength();
	if (GlobalBoundingBoxLength == std::numeric_limits<double>::infinity() || GlobalBoundingBoxLength == 0)
	{
		GlobalBoundingBoxLength = 120;
	}

	double oldcampos[3];
	double newcampos[3];
	this->MeshToolsCore->getCamera()->GetPosition(oldcampos);
	double oldcamerafocalpoint[3];
	this->MeshToolsCore->getCamera()->GetFocalPoint(oldcamerafocalpoint);

	double dispvector[3];
	
	vtkMath::Subtract(oldcampos, oldcamerafocalpoint, dispvector);
	vtkMath::Normalize(dispvector);
	double newdist = multfactor*GlobalBoundingBoxLength;
	vtkMath::MultiplyScalar(dispvector, newdist);
	
	vtkMath::Add(newcamerafocalpoint, dispvector, newcampos);

	this->MeshToolsCore->getCamera()->SetPosition(newcampos);
	this->MeshToolsCore->getCamera()->SetFocalPoint(newcamerafocalpoint);

	// now adjust if necessary..
	if (this->MeshToolsCore->Getmui_CameraOrtho() == 1)
	{
		this->MeshToolsCore->getCamera()->SetParallelScale(GlobalBoundingBoxLength);
		this->MeshToolsCore->getRenderer()->ResetCameraClippingRange();
	}
	
	this->ui->qvtkWidget->update();


}
//Called to repplace camera and grid positions when switching from "orange grid mode" to "blue grid mode"
//= when camera focalpoint and grid center are changed between 0,0,0 and COM of all opened meshes.
void MeshTools::ReplaceCameraAndGrid()
{
	double newcamerafocalpoint[3] = { 0,0,0 };
	if (this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		this->MeshToolsCore->getActorCollection()->GetCenterOfMass(newcamerafocalpoint);				
	}
	cout << "New camera focal point:" << newcamerafocalpoint[0] << " " << newcamerafocalpoint[1] << " " << newcamerafocalpoint[2] << endl;			

	double oldcampos[3];
	double newcampos[3];
	this->MeshToolsCore->getCamera()->GetPosition(oldcampos);
	double oldcamerafocalpoint[3];
	this->MeshToolsCore->getCamera()->GetFocalPoint(oldcamerafocalpoint);

	double dispvector[3];
	vtkMath::Subtract(newcamerafocalpoint, oldcamerafocalpoint, dispvector);
	vtkMath::Add(oldcampos, dispvector, newcampos);	
	this->MeshToolsCore->getCamera()->SetPosition(newcampos);
	this->MeshToolsCore->getCamera()->SetFocalPoint(newcamerafocalpoint);

	this->MeshToolsCore->getGridActor()->SetGridOrigin(newcamerafocalpoint);
	this->MeshToolsCore->getGridActor()->SetOutlineMode(this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin());
	//this->MeshToolsCore->getGridActor()->SetGridType(gridtype);	
	this->ui->qvtkWidget->update();
		
		
}

void MeshTools::slotExit() {
	//maybe we should save the .ini files!
	qApp->exit();
}
// Action to be taken upon camera front side
void MeshTools::slotCameraFront()
{
	
	cout << "this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin()=" << this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin() << endl;
	double cameracentre[3] = { 0, 0, 0 };
	if (this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		this->MeshToolsCore->getActorCollection()->GetCenterOfMass(cameracentre);
		this->MeshToolsCore->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(this->MeshToolsCore->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*this->MeshToolsCore->getCamera()->GetParallelScale();
	this->MeshToolsCore->getCamera()->SetPosition(camdist+ cameracentre[0], cameracentre[1], cameracentre[2]);
	this->MeshToolsCore->getCamera()->SetFocalPoint(cameracentre);
	this->MeshToolsCore->getCamera()->SetViewUp(0, 0, 1);
	//this->ReplaceCamera();
	
	this->MeshToolsCore->getGridActor()->SetGridOrigin(cameracentre);
	this->MeshToolsCore->getGridActor()->SetOutlineMode(this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin());
	
	this->MeshToolsCore->getGridActor()->SetGridType(2);
	

	this->ui->qvtkWidget->update(); // update main window!
}
// Action to be taken upon camera back side
void MeshTools::slotCameraBack()
{
	cout << "this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin()=" << this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin() << endl;
	//this->ReplaceCamera();
	double cameracentre[3] = { 0, 0, 0 };
	if (this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		this->MeshToolsCore->getActorCollection()->GetCenterOfMass(cameracentre);		
		this->MeshToolsCore->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(this->MeshToolsCore->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*this->MeshToolsCore->getCamera()->GetParallelScale();
	this->MeshToolsCore->getCamera()->SetPosition(-camdist + cameracentre[0], cameracentre[1], cameracentre[2]);
	this->MeshToolsCore->getCamera()->SetFocalPoint(cameracentre);
	this->MeshToolsCore->getCamera()->SetViewUp(0, 0, 1);
	this->MeshToolsCore->getGridActor()->SetGridOrigin(cameracentre);
	this->MeshToolsCore->getGridActor()->SetOutlineMode(this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin());
	this->MeshToolsCore->getGridActor()->SetGridType(2);
	this->ui->qvtkWidget->update();

}
// Action to be taken upon camera left side
void MeshTools::slotCameraLeft()
{
	cout << "this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin()=" << this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin() << endl;
	//this->ReplaceCamera
	double cameracentre[3] = { 0, 0, 0 };
	if (this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		this->MeshToolsCore->getActorCollection()->GetCenterOfMass(cameracentre);
		this->MeshToolsCore->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(this->MeshToolsCore->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*this->MeshToolsCore->getCamera()->GetParallelScale();
	this->MeshToolsCore->getCamera()->SetPosition( cameracentre[0], camdist+cameracentre[1], cameracentre[2]);
	this->MeshToolsCore->getCamera()->SetFocalPoint(cameracentre);

	this->MeshToolsCore->getCamera()->SetViewUp(0,0, 1);
	this->MeshToolsCore->getGridActor()->SetGridOrigin(cameracentre);
	this->MeshToolsCore->getGridActor()->SetOutlineMode(this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin());
	this->MeshToolsCore->getGridActor()->SetGridType(1);
	this->ui->qvtkWidget->update(); // update main window!

}
// Action to be taken upon camera right side
void MeshTools::slotCameraRight()
{
	cout << "this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin()=" << this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin() << endl;
	//this->ReplaceCamera();
	double cameracentre[3] = { 0, 0, 0 };
	if (this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		this->MeshToolsCore->getActorCollection()->GetCenterOfMass(cameracentre);
		this->MeshToolsCore->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(this->MeshToolsCore->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*this->MeshToolsCore->getCamera()->GetParallelScale();
	this->MeshToolsCore->getCamera()->SetPosition(cameracentre[0], -camdist + cameracentre[1], cameracentre[2]);
	this->MeshToolsCore->getCamera()->SetFocalPoint(cameracentre);
	this->MeshToolsCore->getCamera()->SetViewUp(0, 0, 1);
	
	this->MeshToolsCore->getGridActor()->SetGridOrigin(cameracentre);
	this->MeshToolsCore->getGridActor()->SetOutlineMode(this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin());
	this->MeshToolsCore->getGridActor()->SetGridType(1);
	this->ui->qvtkWidget->update(); // update main window!
}
// Action to be taken upon camera underneath side
void MeshTools::slotCameraBelow()
{
	cout << "this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin()=" << this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin() << endl;
	double cameracentre[3] = { 0, 0, 0 };
	if (this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		this->MeshToolsCore->getActorCollection()->GetCenterOfMass(cameracentre);
		this->MeshToolsCore->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(this->MeshToolsCore->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*this->MeshToolsCore->getCamera()->GetParallelScale();
	this->MeshToolsCore->getCamera()->SetPosition(cameracentre[0], cameracentre[1], -camdist+cameracentre[2]);
	this->MeshToolsCore->getCamera()->SetFocalPoint(cameracentre);
	this->MeshToolsCore->getCamera()->SetViewUp(1, 0, 0);
	//this->MeshToolsCore->getCamera()->SetParallelScale(120);
	this->MeshToolsCore->getGridActor()->SetGridOrigin(cameracentre);
	this->MeshToolsCore->getGridActor()->SetOutlineMode(this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin());
	this->MeshToolsCore->getGridActor()->SetGridType(0);
	this->ui->qvtkWidget->update(); // update main window!

}
// Action to be taken upon camera upper side
void MeshTools::slotCameraAbove()
{
	cout << "this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin()=" << this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin() << endl;
	//this->ReplaceCamera();
	double cameracentre[3] = { 0, 0, 0 };
	if (this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		this->MeshToolsCore->getActorCollection()->GetCenterOfMass(cameracentre);
		this->MeshToolsCore->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(this->MeshToolsCore->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*this->MeshToolsCore->getCamera()->GetParallelScale();
	this->MeshToolsCore->getCamera()->SetPosition(cameracentre[0],  cameracentre[1], camdist+cameracentre[2]);
	this->MeshToolsCore->getCamera()->SetFocalPoint(cameracentre);


	this->MeshToolsCore->getCamera()->SetViewUp(-1, 0, 0);
	this->MeshToolsCore->getGridActor()->SetGridOrigin(cameracentre);
	this->MeshToolsCore->getGridActor()->SetOutlineMode(this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin());
	this->MeshToolsCore->getGridActor()->SetGridType(0);
	this->ui->qvtkWidget->update(); // update main window!

}

void MeshTools::SetGridVisibility()
{
vtkPropCollection* props = this->MeshToolsCore->getRenderer()->GetViewProps(); //iterate through and set each visibility to 0
props->InitTraversal();
std::string str1("vtkGridActor");
for (int i = 0; i < props->GetNumberOfItems(); i++)
{
	vtkProp *myprop = props->GetNextProp();
	if (str1.compare(myprop->GetClassName()) == 0)
	{
		if (this->MeshToolsCore->Getmui_ShowGrid() == 1)
		{
			myprop->VisibilityOn();
		}
		else
		{
			myprop->VisibilityOff();
		}
	}

}


this->ui->qvtkWidget->update(); // update main window!

}

// show or hide grid actor
void MeshTools::slotGridToggle()
{
	if (this->MeshToolsCore->Getmui_ShowGrid()==1)
	{
		this->MeshToolsCore->Setmui_ShowGrid(0);
	}
	else
	{
		this->MeshToolsCore->Setmui_ShowGrid(1);
	}
	this->SetGridVisibility();
	

}
void MeshTools::slotRendererAnaglyphToggle()
{
	if (this->MeshToolsCore->Getmui_Anaglyph() == 1)
	{
		this->MeshToolsCore->Setmui_Anaglyph(0);
	}
	else
	{
		this->MeshToolsCore->Setmui_Anaglyph(1);
	}


}

void MeshTools::SetOrientationHelperVisibility()
{

	//std::string str1("vtkOrientationHelperActor");
	if (this->MeshToolsCore->Getmui_ShowOrientationHelper() == 1)
	{
		this->OrientationHelperWidget->GetOrientationMarker()->VisibilityOn();
	}
	else
	{
		this->OrientationHelperWidget->GetOrientationMarker()->VisibilityOff();
	}
	this->ui->qvtkWidget->update(); // update main window!
}
// show or hide the orientation helper actor
void MeshTools::slotOrientationHelperToggle()
{
	if (this->MeshToolsCore->Getmui_ShowOrientationHelper() == 1)
	{
		this->MeshToolsCore->Setmui_ShowOrientationHelper(0);
	}
	else
	{
		this->MeshToolsCore->Setmui_ShowOrientationHelper(1);
	}
	this->SetOrientationHelperVisibility();	
}
void MeshTools::slotCameraCentreOfMassToggle()

{
	if (this->MeshToolsCore->Getmui_CameraCentreOfMassAtOrigin() == 1)
	{
		this->MeshToolsCore->Setmui_CameraCentreOfMassAtOrigin(0);
	}
	else
	{
		this->MeshToolsCore->Setmui_CameraCentreOfMassAtOrigin(1);
	}

	this->ReplaceCameraAndGrid();
}
void MeshTools::slotCameraOrthoPerspectiveToggle()

{
	if (this->MeshToolsCore->Getmui_CameraOrtho() == 1)
	{
		this->MeshToolsCore->Setmui_CameraOrtho(0);
	}
	else
	{
		this->MeshToolsCore->Setmui_CameraOrtho(1);
		
	}

	this->ResetCameraOrthoPerspective();
}
void MeshTools::ResetCameraOrthoPerspective()
{
	if (this->MeshToolsCore->Getmui_CameraOrtho() == 1)
	{
		this->MeshToolsCore->getCamera()->SetParallelProjection(true);
		this->DollyCameraForParallelScale();
	}
	else
	{
		
		this->MeshToolsCore->getCamera()->SetParallelProjection(false);
		this->DollyCameraForPerspectiveMode();
		
		
	}
	//cout << "Parallel scale"<<this->MeshToolsCore->getCamera()->GetParallelScale()<<endl;
	double dist = 0;

	
	double campos[3] = { 0,0,0 };
	double foc[3] = { 0,0,0 };
	this->MeshToolsCore->getCamera()->GetPosition(campos);
	//cout << "Camera Position:" << campos[0] <<","<<campos[1]<<","<<campos[2]<< endl;
	this->MeshToolsCore->getCamera()->GetFocalPoint(foc);
	//cout << "Camera Position:" << foc[0] << "," << foc[1] << "," << foc[2] << endl;
	dist = sqrt(pow((campos[0]-foc[0]),2)+ pow((campos[1] - foc[1]), 2)+ pow((campos[2] - foc[2]), 2));
	//cout << "Distance between camera and focal point:" << dist << endl;
	
	//cout << "Camera viewing angle:" << this->MeshToolsCore->getCamera()->GetViewAngle() << endl;

	this->ui->qvtkWidget->update(); // update main window!
}
/*
In perspective mode, "zoom" (dolly) in/out changes the position of the camera
("dolly" functions of vtkInteractorStyleTrackballCamera.cxx and of vtkInteractorStyleJoystickCamera )
Beware : no real "Zoom" function is applied in these styles!!!!
=> before I create  MeshTools' own interactor styles, camera's parallel scale (=ortho "zoom") should 
be updated when switching from "perspective" to "ortho" to keep track of that change... 
=> Once these styles are created, this function should be removed!

*/
void MeshTools::DollyCameraForParallelScale()
{
	double campos[3] = { 0,0,0 };
	double foc[3] = { 0,0,0 };
	
	this->MeshToolsCore->getCamera()->GetPosition(campos);
	this->MeshToolsCore->getCamera()->GetFocalPoint(foc);	
	double dist = sqrt(vtkMath::Distance2BetweenPoints(campos, foc));
	//double dist = sqrt(pow((campos[0] - foc[0]), 2) + pow((campos[1] - foc[1]), 2) + pow((campos[2] - foc[2]), 2));
	double multfactor = 1 / tan(this->MeshToolsCore->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);

	double newparallelscale = dist / multfactor;
	this->MeshToolsCore->getCamera()->SetParallelScale(newparallelscale);
	
}

/*
In parallel mode, "zoom" (dolly) in/out does not change the position of the camera
("dolly" functions of vtkInteractorStyleTrackballCamera.cxx and of vtkInteractorStyleJoystickCamera )
Beware : no real "Zoom" function is applied in these styles!!!!
=> before I create  MeshTools' own interactor styles, camera's position in perspective mode should
be updated when switching from "ortho" to "perspective" to keep track of that change...
=> Once these styles are created, this function should be removed!

*/
void MeshTools::DollyCameraForPerspectiveMode()
{
	double campos[3] = { 0,0,0 };
	double foc[3] = { 0,0,0 };
	double dispvector[3];
	this->MeshToolsCore->getCamera()->GetPosition(campos);
	this->MeshToolsCore->getCamera()->GetFocalPoint(foc);
	double multfactor = 3.73; // at 30° vtk : angle = 2*atan((h/2)/d). 
	// then 2*d  =12/tan(viewangle/2) 
	multfactor = 1 / tan(this->MeshToolsCore->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	cout << "DollyCameraForPerspectiveMode" << endl;
	cout << "multfactor" << multfactor << endl;
	cout << "Old posisition:" << campos[0] << "," << campos[1] << "," << campos[2] << endl;

	vtkMath::Subtract(campos, foc, dispvector);		
	cout<<"Disp Vector:" << dispvector[0] << ","<<dispvector[1] << "," << dispvector[2] << endl;
	vtkMath::Normalize(dispvector);
	cout << "Normalized Disp Vector:" << dispvector[0] << "," << dispvector[1] << "," << dispvector[2] << endl;
	
	double newdist = multfactor*this->MeshToolsCore->getCamera()->GetParallelScale();
	cout << "New dist:" << newdist << endl;
	vtkMath::MultiplyScalar(dispvector, newdist);
	cout << "Multiplied Disp Vector:" << dispvector[0] << "," << dispvector[1] << "," << dispvector[2] << endl;
	double newpos[3]={ 0,0,0 }; 
	vtkMath::Add(foc, dispvector, newpos);
	cout << "New pos:" << newpos[0] << "," << newpos[1] << "," << newpos[2] << endl;

	this->MeshToolsCore->getCamera()->SetPosition(newpos);
	


}