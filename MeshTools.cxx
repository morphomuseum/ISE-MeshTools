


#include "ui_MeshTools.h"
#include "MeshTools.h"
#include "vtkOrientationHelperActor.h"
#include "vtkOrientationHelperWidget.h"

#include <vtkTextProperty.h>
#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include <vtkActor.h>
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
#include <QIcon>

//Select meshes, landmarks and tags ... first try!
void RubberBandSelect(vtkObject* caller,
	long unsigned int vtkNotUsed(eventId),
	void* vtkNotUsed(clientData),
	void* vtkNotUsed(callData))
{
	std::cout << "Pick." << std::endl;
	vtkAreaPicker* areaPicker = static_cast<vtkAreaPicker*>(caller);
	
	vtkProp3DCollection* props = areaPicker->GetProp3Ds();
	//vtkPropCollection* props = areaPicker->GetPrGetProps();

	for (vtkIdType i = 0; i < props->GetNumberOfItems(); i++)
	{
		vtkActor* prop = (vtkActor*)props->GetNextProp3D();
		//std::cout << "Picked prop: " << prop << ", class name:" << prop->GetClassName() << std::endl;
		std::cout << "Picked prop: " << prop << std::endl;
		//prop->GetProperty()->SetColor(1, 0, 0);
		std::cout << "Property: " << prop->GetProperty() << std::endl;
		/*vtkPropCollection* propcoll;
		prop->GetActors(propcoll);
		
		
		propcoll->InitTraversal();
		for (int j = 0; j < propcoll->GetNumberOfItems(); j++)
		{
			vtkProp *myprop = propcoll->GetNextProp();
			//std::cout << "Picked myprop: " << myprop<< std::endl;
			
			
			//myprop->Get

			
		}*/
	}
}



#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

// Constructor
MeshTools::MeshTools()
{
	this->ui = new Ui_MeshTools;
	this->ui->setupUi(this);
	this->mui_ShowGrid = 1;
	this->mui_ShowOrientationHelper = 1;

	this->OrientationHelperWidget = vtkOrientationHelperWidget::New();
	// Qt Table View
	this->TableView = vtkSmartPointer<vtkQtTableView>::New();
	this->ActorCollection = vtkSmartPointer<vtkActorCollection>::New();
	this->Renderer = vtkSmartPointer<vtkRenderer>::New();


	// Place the table view in the designer form
	this->ui->tableFrame->layout()->addWidget(this->TableView->GetWidget());



	this->ui->qvtkWidget->GetRenderWindow()->SetAlphaBitPlanes(1);
	this->ui->qvtkWidget->GetRenderWindow()->SetMultiSamples(0);


	this->Renderer->SetUseDepthPeeling(1);
	this->Renderer->SetMaximumNumberOfPeels(100);
	this->Renderer->SetOcclusionRatio(0.1);


	// VTK/Qt wedded


	this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(this->Renderer);

	//cout<< "Peeling was used:"<< this->Renderer->GetLastRenderingUsedDepthPeeling();

	this->Camera = this->Renderer->GetActiveCamera();


	this->Camera->SetPosition(150, 0, 0);
	this->Camera->SetFocalPoint(0, 0, 0);
	this->Camera->SetViewUp(0, 0, 1);
	//double *viewup;
	//viewup= this->Camera->GetViewUp();
	//cout << "Initial view up:" << viewup[0] << "," << viewup[1] << "," << viewup[2] << endl;
	/*this->Camera->Azimuth(90);// > Roll(-90); // Around "z" (profondeur) viewing axis!
	this->Camera->Roll(90); // around "x" (horizontal) viewing axis
	this->Camera->Elevation(180); // around "y" (vertical) viewing axis
	*/
	this->Camera->SetParallelScale(120);
	this->Camera->ParallelProjectionOn();




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
	connect(this->ui->actionOrientationHelperToggle, SIGNAL(triggered()), this, SLOT(slotOrientationHelperToggle()));
	
	connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));


	vtkSmartPointer<vtkOrientationHelperActor> axes =
		vtkSmartPointer<vtkOrientationHelperActor>::New();

	//

	//vtkOrientationMarkerWidget* widget = vtkOrientationMarkerWidget::New();
	// Does not work with a smart pointer, can't figure out why


	this->OrientationHelperWidget->SetOutlineColor(0.9300, 0.5700, 0.1300);
	this->OrientationHelperWidget->SetOrientationMarker(axes);
	this->OrientationHelperWidget->SetDefaultRenderer(this->Renderer);
	this->OrientationHelperWidget->SetInteractor(this->ui->qvtkWidget->GetRenderWindow()->GetInteractor());
	this->OrientationHelperWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
	this->OrientationHelperWidget->SetEnabled(1);
	this->OrientationHelperWidget->InteractiveOff();
	this->OrientationHelperWidget->PickingManagedOn();
	double myorigin[3];
	myorigin[0] = 0;
	myorigin[1] = 0;
	myorigin[2] = 0;

	this->GridActor = vtkSmartPointer<vtkGridActor>::New();
	this->GridActor->SetGridType(2);
	this->Renderer->AddActor(this->GridActor);


	//@@ rubber band selection!
	
	// vtkSmartPointer<vtkInteractorStyleRubberBandPick> style =
    //vtkSmartPointer<vtkInteractorStyleRubberBandPick>::New();
	/*vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New(); //like paraview*/
	//vtkSmartPointer<vtkInteractorStyleTrackballActor> style =
	//	vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();
	vtkSmartPointer<vtkInteractorStyleSwitch> style =
		vtkSmartPointer<vtkInteractorStyleSwitch>::New();
	 vtkSmartPointer<vtkCallbackCommand> pickCallback =
		 vtkSmartPointer<vtkCallbackCommand>::New();

	 pickCallback->SetCallback(RubberBandSelect);
	 this->AreaPicker =
		 vtkSmartPointer<vtkAreaPicker>::New();

	 this->AreaPicker->AddObserver(vtkCommand::EndPickEvent, pickCallback);
 
// style->SetCurrentRenderer(this->Renderer);
  //this->ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetPicker(this->AreaPicker);
  this->ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
  



  //@@ end rubber band selection!


};



MeshTools::~MeshTools()
{
	// The smart pointers should clean up for up
	//this->OrientationHelperWidget->Delete();
}





//On ajoute un indice au nom si le nom existe d�j�.
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
	//this->Renderer->RemoveAllViewProps();
	//for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	//{
	//vtkActor* actor = this->ActorCollection->GetLastActor();				
	//this->Renderer->AddActor(actor);
	//}
}

//Get BoundingBoxLenght of all actors in this->ActorCollection
void MeshTools::GetGlobalCenterOfMass(double center[3])
{
	//double globalcm[3];
	center[0] = 0;
	center[1] = 0;
	center[2] = 0;

	vtkIdType globalvn = 0;
	vtkSmartPointer<vtkCenterOfMass> centerOfMassFilter =
		vtkSmartPointer<vtkCenterOfMass>::New();
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkActor* actor = this->ActorCollection->GetNextActor();

		centerOfMassFilter->SetInputData(actor->GetMapper()->GetInput());
		centerOfMassFilter->SetUseScalarsAsWeights(false);
		double acenter[3];
		centerOfMassFilter->Update();
		centerOfMassFilter->GetCenter(acenter);
		vtkIdType vertnum = actor->GetMapper()->GetInput()->GetNumberOfPoints();
		center[0] += acenter[0] * vertnum;
		center[1] += acenter[1] * vertnum;
		center[2] += acenter[2] * vertnum;
		globalvn += vertnum;
		cout << "Total VN = " << globalvn << endl;
	}
	cout << "Result: Total VN = " << globalvn << endl;
	if (globalvn > 0)
	{
		center[0] /= globalvn;
		center[1] /= globalvn;
		center[2] /= globalvn;

	}
	this->GridActor->SetGridOrigin(center);


}
double MeshTools::GetGlobalBoundingBoxLength()
{
	double largestbounds[6];
	largestbounds[0] = DBL_MAX; 
	largestbounds[1] = -DBL_MAX;
	largestbounds[2] = DBL_MAX;
	largestbounds[3] = -DBL_MAX;
	largestbounds[4] = DBL_MAX;
	largestbounds[5] = -DBL_MAX;

	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkActor* actor = this->ActorCollection->GetNextActor();
		if (i == 0) { actor->GetBounds(largestbounds); }
		else
		{
			double bounds[6];
			actor->GetBounds(bounds);
			if (bounds[0] < largestbounds[0]) { largestbounds[0] = bounds[0]; }
			if (bounds[1] > largestbounds[1]) { largestbounds[1] = bounds[1]; }
			if (bounds[2] < largestbounds[2]) { largestbounds[2] = bounds[2]; }
			if (bounds[3] > largestbounds[3]) { largestbounds[3] = bounds[3]; }
			if (bounds[4] < largestbounds[4]) { largestbounds[4] = bounds[4]; }
			if (bounds[5] > largestbounds[5]) { largestbounds[5] = bounds[5]; }

		}

	}
	double A[3];//min
	double B[3];//max

	A[0] = largestbounds[0];
	A[1] = largestbounds[2];
	A[2] = largestbounds[4];
	B[0] = largestbounds[1];
	B[1] = largestbounds[3];
	B[2] = largestbounds[5];
	//cout << "A:" << A[0] << "," << A[1] << "," << A[2] << endl;
	//cout << "B:" << B[0] << "," << B[1] << "," << B[2] << endl;
	double diag[3];
	diag[0] = B[0] - A[0];
	diag[1] = B[1] - A[1];
	diag[2] = B[2] - A[2];
	double lengthxyz = sqrt((diag[0])*(diag[0]) + (diag[1])*(diag[1]) + (diag[2])*(diag[2]));

	return lengthxyz;

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



			// Mapper
			VTK_CREATE(vtkPolyDataMapper, mapper);
			mapper->ImmediateModeRenderingOn();
			mapper->SetInputData(MyPolyData);
			VTK_CREATE(vtkActor, actor);
			actor->GetProperty()->SetColor(0.5, 1, 0.5);
			actor->GetProperty()->SetOpacity(0.5);
			actor->SetMapper(mapper);
			this->Renderer->AddActor(actor);
			this->ActorCollection->AddItem(actor);

			double globalcenterofmass[3];
			this->GetGlobalCenterOfMass(globalcenterofmass);
			cout << "Center of mass of all opened mesh is " << globalcenterofmass[0] << " " << globalcenterofmass[1] << " " << globalcenterofmass[2] << endl;

			//double BoundingBoxLength = MyPolyData->GetLength();
			double GlobalBoundingBoxLength = this->GetGlobalBoundingBoxLength();
			cout << "Global Bounding Box length is " << GlobalBoundingBoxLength << " mm" << endl;

			double campos[3];
			this->Camera->GetPosition(campos);
			double camfocalpoint[3];
			this->Camera->GetFocalPoint(camfocalpoint);
			double camscale = this->Camera->GetParallelScale();

			double movex, movey, movez;
			movex = (campos[0] - camfocalpoint[0])*GlobalBoundingBoxLength / camscale;
			movey = (campos[1] - camfocalpoint[1])*GlobalBoundingBoxLength / camscale;
			movez = (campos[2] - camfocalpoint[2])*GlobalBoundingBoxLength / camscale;
			this->Camera->SetPosition
				(globalcenterofmass[0] + movex,
					globalcenterofmass[1] + movey,
					globalcenterofmass[2] + movez);
			//this->Camera->SetPosition(center[0] + GlobalBoundingBoxLength, center[1], center[2]);
			this->Camera->SetFocalPoint(globalcenterofmass[0], globalcenterofmass[1], globalcenterofmass[2]);
			this->Camera->SetParallelScale(GlobalBoundingBoxLength);
			//this->Camera->ParallelProjectionOn();


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

void MeshTools::slotExit() {
	//maybe we should save the .ini files!
	qApp->exit();
}
// Action to be taken upon camera front side
void MeshTools::slotCameraFront()
{
	
	

	this->Camera->SetPosition(150, 0, 0);
	this->Camera->SetFocalPoint(0, 0, 0);
	this->Camera->SetViewUp(0, 0, 1);
	//this->Camera->SetParallelScale(120);

	//this->Camera->Modified();
	this->GridActor->SetGridType(2);
	//this->ui->actionCameraFront->setDisabled(true);	

	this->ui->qvtkWidget->update(); // update main window!
}
// Action to be taken upon camera back side
void MeshTools::slotCameraBack()
{
	this->Camera->SetPosition(-150, 0, 0);
	this->Camera->SetFocalPoint(0, 0, 0);
	this->Camera->SetViewUp(0, 0, 1);
	

	this->GridActor->SetGridType(2);
	this->ui->qvtkWidget->update();

}
// Action to be taken upon camera left side
void MeshTools::slotCameraLeft()
{
	this->Camera->SetPosition(0, 150, 0);
	this->Camera->SetFocalPoint(0, 0, 0);
	this->Camera->SetViewUp(0,0, 1);
	this->GridActor->SetGridType(1);
	this->ui->qvtkWidget->update(); // update main window!

}
// Action to be taken upon camera right side
void MeshTools::slotCameraRight()
{
	this->Camera->SetPosition(0, -150, 0);
	this->Camera->SetFocalPoint(0, 0, 0);
	this->Camera->SetViewUp(0, 0, 1);
	this->GridActor->SetGridType(1);
	this->ui->qvtkWidget->update(); // update main window!
}
// Action to be taken upon camera underneath side
void MeshTools::slotCameraBelow()
{
	this->Camera->SetPosition(0, 0, -150);
	this->Camera->SetFocalPoint(0, 0, 0);
	this->Camera->SetViewUp(1, 0, 0);
	this->Camera->SetParallelScale(120);
	this->GridActor->SetGridType(0);
	this->ui->qvtkWidget->update(); // update main window!

}
// Action to be taken upon camera upper side
void MeshTools::slotCameraAbove()
{
	this->Camera->SetPosition(0, 0, 150);
	this->Camera->SetFocalPoint(0, 0, 0);
	this->Camera->SetViewUp(-1, 0, 0);
	this->GridActor->SetGridType(0);
	this->ui->qvtkWidget->update(); // update main window!

}
// show or hide grid actor
void MeshTools::slotGridToggle()
{
	if (this->mui_ShowGrid==1)
	{
		this->mui_ShowGrid = 0;
	}
	else
	{
		this->mui_ShowGrid = 1;
	}

	vtkPropCollection* props = this->Renderer->GetViewProps(); //iterate through and set each visibility to 0
	props->InitTraversal();
	std::string str1("vtkGridActor");
	for (int i = 0; i < props->GetNumberOfItems(); i++)
	{
		vtkProp *myprop = props->GetNextProp();
		if (str1.compare(myprop->GetClassName()) == 0)
		{
			if (this->mui_ShowGrid==1)
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
// show or hide the orientation helper actor
void MeshTools::slotOrientationHelperToggle()
{
	if (this->mui_ShowOrientationHelper == 1)
	{
		this->mui_ShowOrientationHelper = 0;
	}
	else
	{
		this->mui_ShowOrientationHelper = 1;
	}

	
	//std::string str1("vtkOrientationHelperActor");
			
		
			if (this->mui_ShowOrientationHelper == 1)
			{
				
				this->OrientationHelperWidget->GetOrientationMarker()->VisibilityOn();
				
			}
			else
			{
				
				this->OrientationHelperWidget->GetOrientationMarker()->VisibilityOff();
			}
		

	

	this->ui->qvtkWidget->update(); // update main window!

}