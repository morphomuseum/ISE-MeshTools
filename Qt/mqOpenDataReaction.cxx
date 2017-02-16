/*=========================================================================

   Program: MeshTools
   Module:    mqOpenDataReaction.cxx

========================================================================*/
#include "mqOpenDataReaction.h"

#include "mqMeshToolsCore.h"
#include "mqUndoStack.h"

#include <QFileDialog>
#include <QTextStream>
#include <vtkPLYReader.h>
#include <vtkSTLReader.h>
#include <vtkCleanPolyData.h>
#include <vtkFloatArray.h>

#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataNormals.h>

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
//-----------------------------------------------------------------------------
mqOpenDataReaction::mqOpenDataReaction(QAction* parentObject,  int _mode)
  : Superclass(parentObject)
{
  this->Mode = _mode;
  this->MainWindow = mqMeshToolsCore::instance()->GetMainWindow();
  
}

//@@TODO
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void mqOpenDataReaction::OpenData()
{
	//@@TODO
	cout << "Open data!" << endl;
}

//-----------------------------------------------------------------------------
void mqOpenDataReaction::OpenMesh()
{
 //mqMeshToolsCore::instance()->getUndoStack();
	cout << "Open mesh!" << endl;
	std::string SfileName;
	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load surface"), QDir::currentPath(),
		tr("surfaces (*.ply *.stl *.vtk)"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;

	this->OpenMeshFile(fileName);
}
//-----------------------------------------------------------------------------
void mqOpenDataReaction::OpenNTW()
{
	cout << "Open NTW!" << endl;
	//char param1[1000];
	int i = 0;
	std::string SfileName;
	QString NTWfile = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load project"), QDir::currentPath(),
		tr("NTW (*.ntw)"));

	cout << NTWfile.toStdString() << endl;
	if (NTWfile.isEmpty()) return;
	int file_exists = 1;



	QFile file(NTWfile);
	QFileInfo fileInfo(NTWfile);
	QString onlyfilename(fileInfo.fileName());
	if (file.exists()) {
		// Message
		file.close();
	}
	else
	{
		file_exists = 0;

	}


	if (file_exists == 1)
	{
		cout << "found file!!!!" << endl;


		std::string only_filename = onlyfilename.toStdString();;
		std::string path = NTWfile.toStdString().substr(0, (NTWfile.toStdString().length() - only_filename.length()));
		cout << "only_filename" << only_filename << endl;
		cout << "path" << path << endl;
		mqMeshToolsCore::instance()->UnselectAll(-1);





		QFile inputFile(NTWfile);
		int ok = 0;

		if (inputFile.open(QIODevice::ReadOnly))
		{
			QTextStream in(&inputFile);
			while (!in.atEnd())
			{
				QString line = in.readLine();
				cout << "Line:" << line.toStdString() << endl;
				//sscanf(line.toStdString().c_str(), "%[^\n]s", param1);
				//cout << "param1" << param1 << endl;
				//std::string myline = param1;
				std::string myline = line.toStdString();
				cout << "My line:" << myline << endl;
				std::string FLGext(".flg");
				std::string FLGext2(".FLG");
				std::string VERext(".ver");
				std::string VERext2(".VER");
				std::string CURext(".cur");
				std::string CURext2(".CUR");
				std::string STVext(".stv");
				std::string STVext2(".STV");
				std::string TAGext(".tag");
				std::string TAGext2(".TAG");
				std::string ORIext(".ori");
				std::string ORIext2(".ORI");
				int lmk_file = 0;

				std::size_t found = myline.find(FLGext);
				std::size_t found2 = myline.find(FLGext2);
				if (found != std::string::npos || found2 != std::string::npos)
				{
					lmk_file = 1;
					// Now open flag file!
					QFileInfo flgfileInfo(line);
					QString onlyflgfilename(flgfileInfo.fileName());
					std::string flgfilename = onlyflgfilename.toStdString();
					if (myline.length() == flgfilename.length())
					{
						myline = path.c_str();
						myline.append(flgfilename.c_str());
					}
					std::cout << "Try to load flag file :<<" << myline.c_str() << std::endl;
					//@@TODO
					//this->Open_FLG_File(myline);

				}

				found = myline.find(VERext);
				found2 = myline.find(VERext2);
				if (found != std::string::npos || found2 != std::string::npos)
				{
					lmk_file = 1;
					int landmark_mode = 0;
					// Now open ver file!
					QFileInfo verfileInfo(line);
					QString onlyverfilename(verfileInfo.fileName());
					std::string verfilename = onlyverfilename.toStdString();
					if (myline.length() == verfilename.length())
					{
						myline = path.c_str();
						myline.append(verfilename.c_str());
					}
					std::cout << "Try to load landmark file :<<" << myline.c_str() << std::endl;
					//TODO!
					QString verfile(myline.c_str());
					this->OpenVERFile(verfile, landmark_mode);
					//this->Open_VER_File(landmark_mode, myline);
					// Now open VER file !

				}

				found = myline.find(CURext);
				found2 = myline.find(CURext2);
				if (found != std::string::npos || found2 != std::string::npos)
				{
					lmk_file = 1;
					// Now open cur file!
					QFileInfo curfileInfo(line);
					QString onlycurfilename(curfileInfo.fileName());
					std::string curfilename = onlycurfilename.toStdString();
					if (myline.length() == curfilename.length())
					{
						myline = path.c_str();
						myline.append(curfilename.c_str());
					}
					std::cout << "Try to load curve file :<<" << myline.c_str() << std::endl;
					//@@TODO!
					//this->Open_CUR_File(myline);


				}

				found = myline.find(STVext);
				found2 = myline.find(STVext2);
				if (found != std::string::npos || found2 != std::string::npos)
				{
					lmk_file = 1;
					// Now open STV file!
					QFileInfo stvfileInfo(line);
					QString onlystvfilename(stvfileInfo.fileName());
					std::string stvfilename = onlystvfilename.toStdString();
					if (myline.length() == stvfilename.length())
					{
						myline = path.c_str();
						myline.append(stvfilename.c_str());
					}
					std::cout << "Try to load curve file :<<" << myline.c_str() << std::endl;
					//@@TODO!
					//this->Open_STV_File(myline);
					// Now open CUR file !


				}
				found = myline.find(ORIext);
				found2 = myline.find(ORIext2);
				if (found != std::string::npos || found2 != std::string::npos)
				{
					lmk_file = 1;
					// Now open ORI file!
					QFileInfo orifileInfo(line);
					QString onlyorifilename(orifileInfo.fileName());
					std::string orifilename = onlyorifilename.toStdString();
					if (myline.length() == orifilename.length())
					{
						myline = path.c_str();
						myline.append(orifilename.c_str());
					}
					std::cout << "Try to load orientaiton file :<<" << myline.c_str() << std::endl;
					//@@TODO
					//this->Open_ORI_File(myline);
					// Now open CUR file !


				}

				found = myline.find(TAGext);
				found2 = myline.find(TAGext2);
				if (found != std::string::npos || found2 != std::string::npos)
				{
					lmk_file = 1;
					// Now open TAG file!
					QFileInfo tagfileInfo(line);
					QString onlytagfilename(tagfileInfo.fileName());
					std::string tagfilename = onlytagfilename.toStdString();
					if (myline.length() == tagfilename.length())
					{
						myline = path.c_str();
						myline.append(tagfilename.c_str());
					}
					std::cout << "Try to load orientaiton file :<<" << myline.c_str() << std::endl;
					//@@TODO!
					//this->Open_TAG_File(myline);
					// Now open CUR file !


				}

				//NOW THE SURFACES!!!

				if (lmk_file == 0)
				{
					if (i == 0)
					{

						//length=(int)strlen(oneline);						
						//strncpy(param1, oneline, length-1);
						std::string meshname = line.toStdString();
						QFileInfo meshfileInfo(line);
						QString onlymeshfilename(meshfileInfo.fileName());
						std::string meshfilename = onlymeshfilename.toStdString();

						if (meshname.length() == meshfilename.length())
						{
							meshname = path.c_str();
							meshname.append(meshfilename.c_str());
						}
						QString meshfile(meshname.c_str());



						this->OpenMeshFile(meshfile);
						vtkMTActor* actor = mqMeshToolsCore::instance()->GetLastActor();

						if (actor != NULL && actor->GetNumberOfPoints() > 10)
						{

							ok = 1;
							cout << "Object has more than 10 points <<" << endl;
						}
						else
						{
							ok = 0;
						}


					}
					if (i == 1)
					{
						if (ok)
						{

							//length= (int)strlen(oneline);						
							//strncpy(param1, oneline, length-1);
							std::string posfile = line.toStdString();
							// Now open TAG file!
							QFileInfo posfileInfo(line);
							QString onlyposfilename(posfileInfo.fileName());
							std::string posfilename = onlyposfilename.toStdString();

							if (posfile.length() == posfilename.length())
							{
								posfile = path.c_str();
								posfile.append(posfilename.c_str());
							}
							std::cout << "Try to load position :<<" << posfile.c_str() << std::endl;
							QString qposfile(posfile.c_str());
							this->OpenPOSFile(qposfile, 0);
							//@@TODO!
							//this->Open_POS_File(posfile, My_Obj);
							//std::cout <<"Object <<"<<My_Obj->name.c_str()<<">> position loaded"<< std::endl;
							//My_Obj->selected = 0;
						}
					}
					if (i == 2)
					{
						if (ok)
						{
							vtkMTActor *actor = mqMeshToolsCore::instance()->GetLastActor();
							double r, g, b, a;
							QTextStream myteststream(&line);
							myteststream >> r >> g >> b >> a;
							if (r > 1 || g > 1 || b > 1) {
								r /= 255; g /= 255;
								b /= 255;
							}


							actor->SetmColor(r, g, b, a);
							actor->SetChanged(1);

							mqMeshToolsCore::instance()->getActorCollection()->SetChanged(1);
							/*sscanf(oneline, "%f %f %f %f\n", &color1, &color2, &color3, &color4);
							//std::cout <<"color 1"<<color1<<",color 2"<<color3<<",color 3"<<color3<<",color 4"<<color4<< std::endl;
							My_Obj->color[0] = color1; My_Obj->color[1] = color2; My_Obj->color[2] = color3; My_Obj->color[3] = color4;
							My_Obj->blend = color4;
							My_Obj->Update_RGB();*/
						}
					}
					i++;
					if (i > 2)
					{
						i = 0;
					}
				}


			}
			inputFile.close();
		}
	}
}

void mqOpenDataReaction::OpenPOSFile(QString fileName, int mode)
{
	// mode : 0 for last inserted mesh
	// mode : 1 for all selected meshes
	// mode : 2 for all selected landmarks/flags
	// mode : 3 for all selected landmarks/flags and meshes

	//Open a position file!

	int i, j, l;
	size_t  length;


	length = fileName.toStdString().length();

	union {
		float f;
		char c[4];
	} u; // holds one float or 4 characters (bytes)



	int done = 0;
	if (length>0)
	{
		int file_exists = 1;
		ifstream file(fileName.toStdString().c_str());
		if (file)
		{
			//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
			file.close();
		}
		else
		{

			std::cout << "file:" << fileName.toStdString().c_str() << " does not exists." << std::endl;
			file_exists = 0;
		}

		if (file_exists == 1)
		{

			std::string MAText(".mat");
			std::string MAText2(".MAT");
			std::string POSext(".pos");
			std::string POSext2(".POS");

			int type = 0; // 0 = .POS Ascii File //1 = .MAT binary File or simple .MAT file

			std::size_t found = fileName.toStdString().find(MAText);
			std::size_t found2 = fileName.toStdString().find(MAText2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 1;
				//MAT
			}

			found = fileName.toStdString().find(POSext);
			found2 = fileName.toStdString().find(POSext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 0; //POS
			}




			int Ok = 1;
			vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();



			if (type == 1)
			{
				FILE	*filein;									// Filename To Open


				filein = fopen(fileName.toStdString().c_str(), "rb");
				for (i = 0; i<4; i++)
					for (j = 0; j<4; j++)
					{

						for (l = 3; l >= 0; l--)
						{
							u.c[l] = fgetc(filein);
						}
						//My_Obj->Mat1[i][j] = u.f;
					}


				for (i = 0; i<4; i++)
					for (j = 0; j<4; j++)
					{
						for (l = 3; l >= 0; l--)
						{
							u.c[l] = fgetc(filein);
						}
						Mat->SetElement(j, i, double(u.f));

					}

			}
			else
			{
				//filein = fopen(fileName.toStdString().c_str(), "rt");
				QFile inputFile(fileName);
				int ok = 0;

				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);

					// first matrix is useless (for the moment)	
					for (i = 0; i < 4; i++)
					{
						QString line = in.readLine();

					}
					//
					for (i = 0; i < 4; i++)
					{
						QString line = in.readLine();
						double n1, n2, n3, n4;
						QTextStream myteststream(&line);
						myteststream >> n1 >> n2 >> n3 >> n4;

						Mat->SetElement(0, i, n1);
						Mat->SetElement(1, i, n2);
						Mat->SetElement(2, i, n3);
						Mat->SetElement(3, i, n4);


					}

				}
			}//fin if																		
			mqMeshToolsCore::instance()->ApplyMatrix(Mat, mode);
		}//file exists...
	}	//length

}
void mqOpenDataReaction::OpenVERFile(QString fileName, int mode)
{// mode : 0 for normal landmarks
	// mode : 1 for target landmarks
	double  x, y, z, nx, ny, nz;
	QString LMKName;
	//Open a landmark file!

	int i;
	size_t  length;


	length = fileName.toStdString().length();

	int done = 0;
	if (length>0)
	{
		int file_exists = 1;
		ifstream file(fileName.toStdString().c_str());
		if (file)
		{
			//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
			file.close();
		}
		else
		{

			std::cout << "file:" << fileName.toStdString().c_str() << " does not exists." << std::endl;
			file_exists = 0;
		}

		if (file_exists == 1)
		{

			std::string VERext(".ver");
			std::string VERext2(".VER");
			std::string LMKext(".LMK");
			std::string LMKext2(".LMK");

			int type = 0; // 0 = .POS Ascii File //1 = .MAT binary File or simple .MAT file

			std::size_t found = fileName.toStdString().find(LMKext);
			std::size_t found2 = fileName.toStdString().find(LMKext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 1;
				//LMK
			}

			found = fileName.toStdString().find(VERext);
			found2 = fileName.toStdString().find(VERext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 0; //VER
			}



			if (type == 1)
			{
				

			}
			else
			{
				//filein = fopen(fileName.toStdString().c_str(), "rt");
				QFile inputFile(fileName);
				int ok = 0;

				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);

					// first matrix is useless (for the moment)	
					for (i = 0; i < 4; i++)
					{
						QString line = in.readLine();

					}
					//
					for (i = 0; i < 4; i++)
					{
						QString line = in.readLine();
						QTextStream myteststream(&line);
						myteststream >> LMKName >>  x >> y >> z >> nx >> ny >> nz;

						/**/


					}

				}
			}//fin if																		
			
		}//file exists...
	}	//length

	/*double  x, y, z, nx, ny, nz;
	std::string LMKName;
	FILE	*filein;									// Filename To Open
														//	OBJECT_LANDMARK * My_Landmark;
	char	oneline[255];

	std::string VERext(".ver");
	std::string VERext2(".VER");
	std::string LMKext(".lmk");
	std::string LMKext2(".LMK");

	//char *szFile = fl_file_chooser("Load landmarks", "*.{ver,VER,lmk,LMK}", "");	
	int file_exists = 1;
	ifstream file(filename.c_str());
	if (file)
	{
		//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
		file.close();
	}
	else
	{

		std::cout << "file:" << filename.c_str() << " does not exists." << std::endl;
		file_exists = 0;
	}

	if (file_exists == 1)
	{

		int type = 0; //0 = lmk, 1 = ver
		std::size_t found = filename.find(VERext);
		std::size_t found2 = filename.find(VERext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 1; //VER

		}
		else
		{
			type = 0;
		}

		//filein = fopen(szFile, "rt");
		filein = fopen(filename.c_str(), "r");
		readstr(filein, oneline);
		feof(filein);
		std::cout << "Try open landmark file " << std::endl;
		std::cout << "feof(filein)" << feof(filein) << std::endl;

		vtkSmartPointer<vtkFloatArray> param_list = vtkSmartPointer<vtkFloatArray>::New();
		param_list->SetNumberOfComponents(1);

		while (!feof(filein))
		{
			//std::cout << "Try sscanf_s "  <<oneline<< std::endl;
			//sscanf(oneline, "%s%f %f %f %f %f %f\n", param1, &param2, &param3, &param4, &param5, &param6, &param7, _countof(param1));
			if (type == 1)
			{
				sscanf(oneline, "%s %f %f %f %f %f %f\n", param1, &param2, &param3, &param4, &param5, &param6, &param7);
				param_list->InsertNextTuple1(param2);
				param_list->InsertNextTuple1(param3);
				param_list->InsertNextTuple1(param4);
				param_list->InsertNextTuple1(param5);
				param_list->InsertNextTuple1(param6);
				param_list->InsertNextTuple1(param7);
			}
			else
			{
				sscanf(oneline, "%s %f %f %f\n", param1, &param2, &param3, &param4);
				param_list->InsertNextTuple1(param2);
				param_list->InsertNextTuple1(param3);
				param_list->InsertNextTuple1(param4);
			}
			create_landmarks(landmark_mode, param_list, type);

			param_list = vtkSmartPointer<vtkFloatArray>::New();
			param_list->SetNumberOfComponents(1);


			readstr(filein, oneline); //read next line

		}//While scanff...
		fclose(filein);

	}//if file exists

	*/

}

void mqOpenDataReaction::OpenMeshFile(QString fileName)
{

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

			VTK_CREATE(vtkMTActor, actor);
			std::string newname = name.toStdString();

			size_t nPos = newname.find_first_of(".");
			if (nPos > -1)
			{
				newname = newname.substr(0, nPos);
			}
			//@@TODO! 
			newname = mqMeshToolsCore::instance()->CheckingName(newname, 0);
			if ((vtkUnsignedCharArray*)MyPolyData->GetPointData()->GetScalars("RGB") != NULL)
			{
				MyPolyData->GetPointData()->SetScalars(NULL);
				cout << "found RGB colours! " << endl;
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

			int num = 2;

			actor->SetmColor(mqMeshToolsCore::instance()->Getmui_MeshColor());
			actor->SetSelected(1);
			actor->SetMapper(mapper);
			mqMeshToolsCore::instance()->getActorCollection()->AddItem(actor);
			std::string action = "Load mesh file";
			int mCount = BEGIN_UNDO_SET(action);
			mqMeshToolsCore::instance()->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();



			mqMeshToolsCore::instance()->getActorCollection()->SetChanged(1);

			//double BoundingBoxLength = MyPolyData->GetLength();
			mqMeshToolsCore::instance()->AdjustCameraAndGrid();
			cout << "camera and grid adjusted" << endl;

			if (mqMeshToolsCore::instance()->Getmui_AdjustLandmarkRenderingSize() == 1)
			{
				mqMeshToolsCore::instance()->UpdateLandmarkSettings();
			}
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
			mqMeshToolsCore::instance()->getRenderer()->AddActor(contourLineActor);

			vtkSmartPointer<vtkPolyDataMapper> mapper2 =
			vtkSmartPointer<vtkPolyDataMapper>::New();
			mapper2->SetInputConnection(bcf->GetOutputPort());
			mapper2->SetScalarModeToUseCellData();
			//mapper2->SetScalarRange(0, 1);
			vtkSmartPointer<vtkMTActor> actor2=
			vtkSmartPointer<vtkMTActor>::New();
			actor2->SetMapper(mapper2);
			mqMeshToolsCore::instance()->getRenderer()->AddActor(actor2);
			*/
			/*if (mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
			{
			double globalcenterofmass[3];
			this->GetGlobalCenterOfMass(globalcenterofmass);
			cout << "Center of mass of all opened mesh is " << globalcenterofmass[0] << " " << globalcenterofmass[1] << " " << globalcenterofmass[2] << endl;

			double GlobalBoundingBoxLength = this->GetGlobalBoundingBoxLength();
			cout << "Global Bounding Box length is " << GlobalBoundingBoxLength << " mm" << endl;

			double campos[3];
			mqMeshToolsCore::instance()->getCamera()->GetPosition(campos);
			double camfocalpoint[3];
			mqMeshToolsCore::instance()->getCamera()->GetFocalPoint(camfocalpoint);
			double camscale = mqMeshToolsCore::instance()->getCamera()->GetParallelScale();

			double movex, movey, movez;
			movex = (campos[0] - camfocalpoint[0])*GlobalBoundingBoxLength / camscale;
			movey = (campos[1] - camfocalpoint[1])*GlobalBoundingBoxLength / camscale;
			movez = (campos[2] - camfocalpoint[2])*GlobalBoundingBoxLength / camscale;
			mqMeshToolsCore::instance()->getCamera()->SetPosition
			(globalcenterofmass[0] + movex,
			globalcenterofmass[1] + movey,
			globalcenterofmass[2] + movez);
			//mqMeshToolsCore::instance()->getCamera()->SetPosition(center[0] + GlobalBoundingBoxLength, center[1], center[2]);
			mqMeshToolsCore::instance()->getCamera()->SetFocalPoint(globalcenterofmass[0], globalcenterofmass[1], globalcenterofmass[2]);
			mqMeshToolsCore::instance()->getCamera()->SetParallelScale(GlobalBoundingBoxLength);
			}*/
			//mqMeshToolsCore::instance()->getCamera()->ParallelProjectionOn();


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
	//this->MainWindow->vtkWidgetUpdate();
	mqMeshToolsCore::instance()->Render();
	

}
