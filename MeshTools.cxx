


#include "ui_MeshTools.h"
#include "MeshTools.h"
#include "vtkOrientationHelperActor.h"
#include "vtkOrientationHelperWidget.h"

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

#include <vtkAxesActor.h>
//#include <vtkOrientationMarkerWidget.h>

#include <vtkPolyDataNormals.h>
#include <vtkPLYReader.h>
#include <vtkSTLReader.h>
#include <vtkCleanPolyData.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
#include <vtkLine.h>




#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

// Constructor
MeshTools::MeshTools()
{
  this->ui = new Ui_MeshTools;
  this->ui->setupUi(this);

  // Qt Table View
  this->TableView = vtkSmartPointer<vtkQtTableView>::New();
  this->ActorCollection = vtkSmartPointer<vtkActorCollection>::New();
  this->Renderer = vtkSmartPointer<vtkRenderer>::New();
  this->Grid = vtkSmartPointer<vtkStructuredGrid>::New();

  // Place the table view in the designer form
  this->ui->tableFrame->layout()->addWidget(this->TableView->GetWidget());

  
  
  // Geometry
  
 
  

 
 

  this->ui->qvtkWidget->GetRenderWindow()->SetAlphaBitPlanes(1);
  this->ui->qvtkWidget->GetRenderWindow()->SetMultiSamples(0);
  

  this->Renderer->SetUseDepthPeeling(1);
  this->Renderer->SetMaximumNumberOfPeels(100);
  this->Renderer->SetOcclusionRatio(0.1);
  

  // VTK/Qt wedded


  this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(this->Renderer);
  /*

  int gridSize = 10;
  double gridSpacing = 0.1; //0.1 mm
  vtkSmartPointer<vtkImageData> imageData =
	  vtkSmartPointer<vtkImageData>::New();
  GridData(imageData, gridSize, gridSpacing);

  

  vtkSmartPointer<vtkDataSetMapper> imageDataMapper =
	  vtkSmartPointer<vtkDataSetMapper>::New();


imageDataMapper->SetInputData(imageData);
imageDataMapper->ScalarVisibilityOff();

vtkSmartPointer<vtkActor> imageDataActor =
vtkSmartPointer<vtkActor>::New();
imageDataActor->GetProperty()->SetRepresentationToWireframe();
imageDataActor->SetMapper(imageDataMapper);
imageDataActor->GetProperty()->SetColor(1, 1, 1);
imageDataActor->GetProperty()->SetLineWidth(1);
this->Renderer->AddActor(imageDataActor);


vtkSmartPointer<vtkImageData> imageData2 =
vtkSmartPointer<vtkImageData>::New();
GridData(imageData2, 2, 0.5);


vtkSmartPointer<vtkDataSetMapper> imageDataMapper2 =
vtkSmartPointer<vtkDataSetMapper>::New();
imageDataMapper2->SetInputData(imageData2);
imageDataMapper2->ScalarVisibilityOff();

vtkSmartPointer<vtkActor> imageDataActor2 =
vtkSmartPointer<vtkActor>::New();

imageDataActor2->GetProperty()->SetRepresentationToWireframe();
imageDataActor2->SetMapper(imageDataMapper2);
imageDataActor2->GetProperty()->SetColor(1, 0,1);
imageDataActor2->GetProperty()->SetLineWidth(2);



this->Renderer->AddActor(imageDataActor2);*/

  //cout<< "Peeling was used:"<< this->Renderer->GetLastRenderingUsedDepthPeeling();
  
  this->Camera = this->Renderer->GetActiveCamera();
  
  
  this->Camera->SetPosition(0, 0, -10);
  this->Camera->SetFocalPoint(0, 0, 0);

  this->Camera->Azimuth(90);// > Roll(-90); // Around "z" (profondeur) viewing axis!
  this->Camera->Roll(90); // around "x" (horizontal) viewing axis
  this->Camera->Elevation(180); // around "y" (vertical) viewing axis
  this->Camera->SetParallelScale(10);
  this->Camera->ParallelProjectionOn();

  /*this->Camera->SetParallelScale(10);
   this->Camera->ParallelProjectionOn();
  double a[3];
  a[0] = 0;
  a[1] = 0;
  a[2] = 1;
  this->Camera->SetViewUp(a);*/
  //this->Renderer->ResetCamera();
 

  // Just a bit of Qt interest: Culling off the
  // point data and handing it to a vtkQtTableView
 /* VTK_CREATE(vtkDataObjectToTable, toTable);
  toTable->SetInputConnection(elevation->GetOutputPort());
  toTable->SetFieldType(vtkDataObjectToTable::POINT_DATA);
  toTable->Update();
  // Here we take the end of the VTK pipeline and give it to a Qt View
  this->TableView->SetRepresentationFromInputConnection(toTable->GetOutputPort());*/

  // Set up action signals and slots
  connect(this->ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));

  /**/
  //vtkSmartPointer<vtkAxesActor> axes =
//	  vtkSmartPointer<vtkAxesActor>::New();

  vtkSmartPointer<vtkOrientationHelperActor> axes =
	  vtkSmartPointer<vtkOrientationHelperActor>::New();
  //axes->Print(std::cout); 

  /**/
//  VTK_CREATE(vtkOrientationMarkerWidget, widget);
  //
  vtkOrientationHelperWidget* widget = vtkOrientationHelperWidget::New();
  //vtkOrientationMarkerWidget* widget = vtkOrientationMarkerWidget::New();
  // ça ne marche pas avec class widget!!!!

  
  widget->SetOutlineColor(0.9300, 0.5700, 0.1300);
  widget->SetOrientationMarker(axes);
  widget->SetDefaultRenderer(this->Renderer);
  widget->SetInteractor(this->ui->qvtkWidget->GetRenderWindow()->GetInteractor());
  widget->SetViewport(0.0, 0.0, 0.2, 0.2);
  widget->SetEnabled(1);
  widget->InteractiveOff();
  widget->PickingManagedOn();
  double myorigin[3];
  myorigin[0] = 0; 
  myorigin[1] = 0;
  myorigin[2] = 0;
  vtkActor *gridactor1 = CreateGridOutline(0, 0, 10, 0.1, myorigin);
  gridactor1->GetProperty()->SetOpacity(0.5);
  this->Renderer->AddActor(gridactor1);
  vtkActor *gridactor2 = CreateGridOutline(0, 1, 10, 0.1, myorigin);
  this->Renderer->AddActor(gridactor2);
  gridactor2->GetProperty()->SetOpacity(0.5);
  vtkActor *gridactor3 = CreateGridOutline(0, 2, 10, 0.1, myorigin);
  gridactor3->GetProperty()->SetOpacity(0.5);
  this->Renderer->AddActor(gridactor3);
  
};

MeshTools::~MeshTools()
{
  // The smart pointers should clean up for up

}

vtkActor* MeshTools::CreateGridOutline(const int type, const int plane, const int gridSize,  const double gridSpacing, const double origin[3])
{
	// plane : 0 : xy plane (z=0)
	// plane : 1 : xz plane (y=0)
	// plane : 2 : yz plane (x=0)

	//type : 0 = regular lines
	//type : 1 = bold colored outline
    // gridSize is the number of spaces between lines for each quadrant
	
	// gridSpacing : space between 2 lines in mm.

	
	if (type == 0)
	{
		// We need 8* (gridSize -1) points.
		// Then we need "only" 4 * (gridSize -1) lines (lines should be written only once: A=>B, not B=>A).
		
		vtkSmartPointer<vtkPoints> pts =
		vtkSmartPointer<vtkPoints>::New();
		// create points.
		double coord[3];
		double coord2[3];
		double coord3[3];
		for (int i = (-gridSize+1); i < gridSize	; i++)
		{
			if (i != 0)
			{
				if (plane == 0)
				{
					coord[0] = i*gridSpacing;
					coord[1] = gridSize*gridSpacing;
					coord[2] = 0;

					coord2[0] = coord[0];
					coord2[1] = 0;
					coord2[2] = coord[2];

					coord3[0] = coord[0];
					coord3[1] = -coord[1];
					coord3[2] = coord[2];
				}
				if (plane == 1)
				{
					coord[0] = i*gridSpacing;
					coord[1] = 0;
					coord[2] = gridSize*gridSpacing;

					coord2[0] = coord[0];
					coord2[1] = coord[1];
					coord2[2] = 0;

					coord3[0] = coord[0];
					coord3[1] = coord[1];
					coord3[2] = -coord[2];
				}
				if (plane == 2)
				{
					coord[0] = 0;
					coord[1] = i*gridSpacing;
					coord[2] = gridSize*gridSpacing;

					coord2[0] = coord[0];
					coord2[1] = coord[1];
					coord2[2] = 0;

					coord3[0] = coord[0];
					coord3[1] = coord[1];
					coord3[2] = -coord[2];
				}
				vtkMath::Add(coord, origin, coord);
				vtkMath::Add(coord2, origin, coord2);
				vtkMath::Add(coord3, origin, coord3);
				//cout << "point 1:" << coord[0] << "," << coord[1] << "," << coord[2] << endl;
				//cout << "point 2:" << coord2[0] << "," << coord2[1] << "," << coord2[2] << endl;
				pts->InsertNextPoint(coord);
				pts->InsertNextPoint(coord2);
				pts->InsertNextPoint(coord3);

			}

		}
		for (int i = (-gridSize+1); i < gridSize	; i++)
		{
			if (i != 0)
			{
				if (plane == 0)
				{
					coord[0] = gridSize*gridSpacing;
					coord[1] = i*gridSpacing;
					coord[2] = 0;

					coord2[0] = 0;
					coord2[1] = coord[1];
					coord2[2] = coord[2];


					coord3[0] = -coord[0];
					coord3[1] = coord[1];
					coord3[2] = coord[2];
				}
				if (plane == 1)
				{
					coord[0] = gridSize*gridSpacing;
					coord[1] = 0;
					coord[2] = i*gridSpacing;

					coord2[0] = 0;
					coord2[1] = coord[1];
					coord2[2] = coord[2];

					coord3[0] = -coord[0];
					coord3[1] = coord[1];
					coord3[2] = coord[2];
				}
				if (plane == 2)
				{
					coord[0] = 0;
					coord[1] = gridSize*gridSpacing;
					coord[2] = i*gridSpacing;

					coord2[0] = coord[0];
					coord2[1] = 0;
					coord2[2] = coord[2];

					coord3[0] = coord[0];
					coord3[1] = -coord[1];
					coord3[2] = coord[2];
				}
				vtkMath::Add(coord, origin, coord);
				vtkMath::Add(coord2, origin, coord2);
				vtkMath::Add(coord3, origin, coord3);
				//cout << "2point 1:" << coord[0] << "," << coord[1] << "," << coord[2] << endl;
				//cout << "2point 2:" << coord2[0] << "," << coord2[1] << "," << coord2[2] << endl;
				pts->InsertNextPoint(coord);
				pts->InsertNextPoint(coord2);
				pts->InsertNextPoint(coord3);

			}

		}
		vtkSmartPointer<vtkPolyData> linesPolyData =
			vtkSmartPointer<vtkPolyData>::New();
		// Add the points to the polydata container
		linesPolyData->SetPoints(pts);
		// We have 4* (gridSize -1) lines. *2

		vtkSmartPointer<vtkCellArray> lines =
		vtkSmartPointer<vtkCellArray>::New();

		for (int i = 0; i < (4 * (gridSize - 1)); i++)
		{
			vtkSmartPointer<vtkLine> line =
				vtkSmartPointer<vtkLine>::New();
			line->GetPointIds()->SetId(0, 3*i); 
			line->GetPointIds()->SetId(1, 3*i+1); 
			lines->InsertNextCell(line);
			
			
		}
		for (int i = 0; i < (4 * (gridSize - 1)); i++)
		{
			vtkSmartPointer<vtkLine> line2 =
				vtkSmartPointer<vtkLine>::New();
			line2->GetPointIds()->SetId(0, 3 * i + 1);
			line2->GetPointIds()->SetId(1, 3 * i + 2);
			lines->InsertNextCell(line2);


		}

		
		linesPolyData->SetLines(lines);


		// Create six colors - one for each line
		unsigned char red[3] = { 255, 0, 0 };
		unsigned char green[3] = { 0, 255, 0 };
		unsigned char blue[3] = { 0, 0, 255 };
		unsigned char yellow[3] = { 255, 255, 0 };
		unsigned char cyann[3] = { 0, 255, 255 };
		unsigned char purple[3] = { 255, 0, 255 };

		// Create a vtkUnsignedCharArray container and store the colors in it
		vtkSmartPointer<vtkUnsignedCharArray> colors =
			vtkSmartPointer<vtkUnsignedCharArray>::New();
		colors->SetNumberOfComponents(3);
		for (int i = 0; i < (2*(gridSize - 1)); i++)
		{
			if (plane == 0)
			{
				colors->InsertNextTupleValue(green);
			}
			else if (plane == 1)
			{
				colors->InsertNextTupleValue(blue);
			}
			else 
			{
				colors->InsertNextTupleValue(blue);
			}

		}
		for (int i = 0; i < (2*(gridSize - 1)); i++)
		{
			if (plane == 0)
			{
				colors->InsertNextTupleValue(red);
			}
			else if (plane == 1)
			{
				colors->InsertNextTupleValue(red);
			}
			else
			{
				colors->InsertNextTupleValue(green);
			}
		}
		for (int i = 0; i < (2*(gridSize - 1)); i++)
		{
			if (plane == 0)
			{
				colors->InsertNextTupleValue(cyann);
			}
			else if (plane == 1)
			{
				colors->InsertNextTupleValue(purple);
			}
			else
			{
				colors->InsertNextTupleValue(purple);
			}
		}
		
		
		for (int i = 0; i < (2*(gridSize - 1)); i++)
		{
			if (plane == 0)
			{
				colors->InsertNextTupleValue(yellow);
			}
			else if (plane == 1)
			{
				colors->InsertNextTupleValue(yellow);
			}
			else
			{
				colors->InsertNextTupleValue(cyann);
			}
			
		}
		


	
		linesPolyData->GetCellData()->SetScalars(colors);

		// Setup the visualization pipeline
		vtkSmartPointer<vtkPolyDataMapper> mapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputData(linesPolyData);

		vtkActor *actor =	vtkActor::New();
		actor->SetMapper(mapper);
		return actor;

		


	}
	else
	{
		// we need 8 points + origin = 9 points
		// we need to draw 12 bold lines.
		

	}

	

	



	

									   // Create a vtkCellArray container and store the lines in it
	
	



	
}

void MeshTools::InitGrid(int gridSize)
{

	/*this->Grid->SetExtent(0, gridSize - 1, 0, gridSize - 1, 0, gridSize - 1);

	vtkSmartPointer<vtkDoubleArray> xCoords =
		vtkSmartPointer<vtkDoubleArray>::New();
	xCoords->SetNumberOfComponents(1);
	vtkSmartPointer<vtkDoubleArray> yCoords =
		vtkSmartPointer<vtkDoubleArray>::New();
	yCoords->SetNumberOfComponents(1);
	vtkSmartPointer<vtkDoubleArray> zCoords =
		vtkSmartPointer<vtkDoubleArray>::New();
	zCoords->SetNumberOfComponents(1);

	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();

	for (int z = 0; z < gridSize; z++)
	{
		
			for ( int y = 0; y < gridSize; y++)
			{
				for ( int x = 0; x < gridSize; x++)
				{
					points->InsertNextPoint(x, y, z);
				}
			}
		}
	
	*/
	

	
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

	vtkIdType globalvn=0;
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


}
double MeshTools::GetGlobalBoundingBoxLength()
{
	double largestbounds[6];
	largestbounds[0] = 0;
	largestbounds[1] = 0;
	largestbounds[2] = 0;
	largestbounds[3] = 0;
	largestbounds[4] = 0;
	largestbounds[5] = 0;

	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkActor* actor = this->ActorCollection->GetNextActor();
		if (i == 0) {actor->GetBounds(largestbounds);}
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

	double diag[3];
	diag[0] = B[0] - A[0];
	diag[2] = B[1] - A[1];
	diag[3] = B[2] - A[2];
	double lengthxyz = sqrt((diag[0])*(diag[0]) + (diag[1])*(diag[1]) + (diag[2])*(diag[2]));

	return lengthxyz;

}

// Action to be taken upon file open
void MeshTools::slotOpenFile()
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

		
			/*double a[3];
			a[0] = 0;
			a[1] = 0;
			a[2] = 1;
			


			this->Camera->SetViewUp(a);*/

			
			


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
			
			/*vtkSmartPointer<vtkCenterOfMass> centerOfMassFilter =
				vtkSmartPointer<vtkCenterOfMass>::New();

			centerOfMassFilter->SetInputData(MyPolyData);
			centerOfMassFilter->SetUseScalarsAsWeights(false);
			double center[3];
			centerOfMassFilter->Update();
			centerOfMassFilter->GetCenter(center);*/
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
				globalcenterofmass[1]+ movey, 
					globalcenterofmass[2]+ movez);
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
  qApp->exit();
}
