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
#include <vtkMath.h>
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


//-----------------------------------------------------------------------------
void mqOpenDataReaction::OpenMesh()
{
 //mqMeshToolsCore::instance()->getUndoStack();
	cout << "Open mesh!" << endl;
	
	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load surface"), QDir::currentPath(),
		tr("surfaces (*.ply *.stl *.vtk)"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;

	this->OpenMesh(fileName);
}
void mqOpenDataReaction::OpenLMK_or_VER(int mode)
{
	//mode 0=> inside normal landmarks
	// 1 => inside target landmarks
	//2 => inside node landmarks
	//3 => inside handle landmarks
	//mqMeshToolsCore::instance()->getUndoStack();
	cout << "Open LMK or VER!" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load landmarks"), QDir::currentPath(),
		tr("landmark files(*.ver *.lmk)"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;

	
	std::string VERext(".ver");
	std::string VERext2(".VER");	
	std::string LMKext(".lmk");
	std::string LMKext2(".LMK");

	int type = 0; //0 =  ver, 1 lmk
	std::size_t found = fileName.toStdString().find(VERext);
	std::size_t found2 = fileName.toStdString().find(VERext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 0; //VER
	}

	
	found = fileName.toStdString().find(LMKext);
	found2 = fileName.toStdString().find(LMKext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 1; //LMK
	}

	if (type == 0)
	{
		
		this->OpenVER(fileName, mode);
	}
	else 
	{
		this->OpenLMK(fileName, mode);
	}

}
void mqOpenDataReaction::OpenSTV()
{
	//mqMeshToolsCore::instance()->getUndoStack();
	cout << "Open STV!" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load meshtool landmarks/curve files"), QDir::currentPath(),
		tr("Landmark files(*.stv)"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;


	std::string STVext(".stv");
	std::string STVext2(".STV");
	

	int type = 0;
	std::size_t found = fileName.toStdString().find(STVext);
	std::size_t found2 = fileName.toStdString().find(STVext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 0; //STV
	}



	if (type == 0)
	{

		this->OpenSTV(fileName);
	}
	

}
void mqOpenDataReaction::OpenSTV(QString fileName)
{
	double  x, y, z, nx, ny, nz;
	QString LMKName;
	//Open a STV file!


	size_t  length;

	int type = 1;
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


			std::string STVext(".stv");
			std::string STVext2(".STV");

		
			std::size_t found = fileName.toStdString().find(STVext);
			std::size_t found2 = fileName.toStdString().find(STVext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 1;
				//STV
			}
		
			if (type == 1)			
			{
				//filein = fopen(fileName.toStdString().c_str(), "rt");
				QFile inputFile(fileName);
				int ok = 0;		
				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);
					int landmark_mode=0;
					int number = 0;
					int cpt_line = 0;
					while (!in.atEnd())
					{

						QString line = in.readLine();
						QTextStream myteststream(&line);			
						if (cpt_line==0)
						{
							myteststream >> landmark_mode >> number ;
						}
						else
						{
							// To do : type = 2 => information 
							int lmtype = -1;
							if (type == 2)// curve node!
							{
								myteststream >> LMKName >> x >> y >> z >> nx >> ny >> nz >> lmtype;
								//lmtype: 1 curve starting point
								//lmtype: 0 normal node
								//lmtype: 2 curve milestone
								//lmtype: 3 connect to preceding starting point
							}
							else
							{
								myteststream >> LMKName >> x >> y >> z >> nx >> ny >> nz;
							}
							double coord[3] = { x,y,z };
							double ncoord[3] = { nx,ny,nz };
							double ori[3];

							double length = nx*nx + ny*ny + nz*nz;
							if (length == 1)
							{
								ori[0] = ncoord[0];
								ori[1] = ncoord[1];
								ori[2] = ncoord[2];
							}
							else
							{
								vtkMath::Subtract(ncoord, coord, ori);
								vtkMath::Normalize(ori);
							}
							mqMeshToolsCore::instance()->CreateLandmark(coord, ori, landmark_mode, lmtype);
						}
						cpt_line++;
						if (cpt_line == number + 1) {
							cpt_line = 0;
							
						}
					}
					/**/

					inputFile.close();


				}
			}//fin if																		

		}//file exists...
	}	//length



	/*float  param2, param3, param4, param5, param6, param7;
	float m_ve[3], m_ven[3], leng;
	char param1[50];
	FILE	*filein;// Filename To Open
	char	oneline[255];
	int landmark_mode;


	int file_exists = 1;
	ifstream file(filename.c_str());

	if (file)
	{
		file.close();
	}
	else
	{
		cout << "file:" << filename.c_str() << " does not exists." << std::endl;
		file_exists = 0;
	}

	if (file_exists == 1)
	{
		std::string STVext(".stv");
		std::string STVext2(".STV");

		int type = 1; //VER

		filein = fopen(filename.c_str(), "r");
		readstr(filein, oneline);
		feof(filein);
		std::cout << "Try open landmark file " << std::endl;
		std::cout << "feof(filein)" << feof(filein) << std::endl;
		int ind = 0;
		vtkSmartPointer<vtkFloatArray> param_list = vtkSmartPointer<vtkFloatArray>::New();
		param_list->SetNumberOfComponents(1);
		int number = 0;
		int cpt_line = 0;
		while (!feof(filein))
		{
			if (cpt_line == 0) {
				sscanf(oneline, "%d %d\n", &landmark_mode, &number);
			}
			else {
				sscanf(oneline, "%s %f %f %f %f %f %f %d\n", param1, &param2, &param3, &param4, &param5, &param6, &param7, &ind);
				param_list->InsertNextTuple1(param2);
				param_list->InsertNextTuple1(param3);
				param_list->InsertNextTuple1(param4);
				param_list->InsertNextTuple1(param5);
				param_list->InsertNextTuple1(param6);
				param_list->InsertNextTuple1(param7);

				create_landmarks(landmark_mode, param_list, type);

				param_list = vtkSmartPointer<vtkFloatArray>::New();
				param_list->SetNumberOfComponents(1);

			}
			readstr(filein, oneline); //read next line
			cpt_line++;

			if (cpt_line == number + 1 && landmark_mode == 0) {
				cpt_line = 0;
				landmark_mode++;
			}
		}//While scanff...
		fclose(filein);
	}
	*/

}

void mqOpenDataReaction::OpenPOS(int mode)
{
	if (mode < 1) { mode = 1; }
	// mode : 1 for all selected meshes
	// mode : 2 for all selected landmarks/flags
	

	//open and applies position 
	//mqMeshToolsCore::instance()->getUndoStack();
	cout << "Open POS" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load position file"), QDir::currentPath(),
		tr("position file (*.pos)"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;	
	this->OpenPOS(fileName,mode);
	
}

void mqOpenDataReaction::OpenPOSTrans(int mode)
{
	if (mode < 1) { mode = 1; }
	// mode : 1 for all selected meshes
	// mode : 2 for all selected landmarks/flags


	//open and applies transpoed position 
	//mqMeshToolsCore::instance()->getUndoStack();
	cout << "Open transposed POS" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load transposed position file"), QDir::currentPath(),
		tr("transposed position file (*.pos)"));

	cout << fileName.toStdString() << endl;
	if (fileName.isEmpty()) return;
	this->OpenPOSTrans(fileName, mode);

}

void mqOpenDataReaction::OpenORI()
{
	
	cout << "Open ORI" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load orientation file"), QDir::currentPath(),
		tr("orientation file (*.ori)"));

	cout << fileName.toStdString() << endl;;
	if (fileName.isEmpty()) return;
	this->OpenORI(fileName);

}
void mqOpenDataReaction::OpenFLG()
{

	cout << "Open FLG" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load flag file"), QDir::currentPath(),
		tr("flag file (*.flg)"));

	cout << fileName.toStdString() << endl;;
	if (fileName.isEmpty()) return;
	this->OpenFLG(fileName);

}
void mqOpenDataReaction::OpenCUR()
{

	cout << "Open CUR" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load CUR file"), QDir::currentPath(),
		tr("curve file (*.cur)"));

	cout << fileName.toStdString()<<endl;
	if (fileName.isEmpty()) return;
	this->OpenCUR(fileName);

}
void mqOpenDataReaction::OpenTAG()
{

	cout << "Open CUR" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load TAG file"), QDir::currentPath(),
		tr("tag file (*.tag)"));

	cout << fileName.toStdString()<<endl;
	if (fileName.isEmpty()) return;
	this->OpenTAG(fileName);

}
void mqOpenDataReaction::OpenData()
{
	//mqMeshToolsCore::instance()->getUndoStack();
	cout << "Open Data!" << endl;
	
	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load data"), QDir::currentPath(),
		tr("meshtools data or project (*.ntw *.ver *.cur *.stv *.tag *.pos *.ori *.flg *.lmk *.ply *.stl *.vtk *.vtp)"));

	cout << fileName.toStdString()<<endl;
	if (fileName.isEmpty()) return;

	std::string STLext(".stl");
	std::string STLext2(".STL");
	std::string VTKext(".vtk");
	std::string VTKext2(".VTK");
	std::string VTKext3(".vtp");
	std::string VTKext4(".VTP");
	std::string PLYext(".ply");
	std::string PLYext2(".PLY");
	std::string NTWext(".ntw");
	std::string NTWext2(".NTW");
	std::string VERext(".ver");
	std::string VERext2(".VER");
	std::string CURext(".cur");
	std::string CURext2(".CUR");
	std::string FLGext(".flg");
	std::string FLGext2(".FLG");
	std::string LMKext(".lmk");
	std::string LMKext2(".LMK");
	std::string TAGext(".tag");
	std::string TAGext2(".TAG");
	std::string STVext(".stv");
	std::string STVext2(".STV");
	std::string ORIext(".ori");
	std::string ORIext2(".ORI");
	std::string POSext(".pos");
	std::string POSext2(".POS");

	int type = 0; //0 = stl, 1 = vtk,  2 = ply, 3 = ntw, 4 ver, 5 cur, 6 flg, 7 lmk, 8 tag, 9 stv, 10 ori, 11 pos
	std::size_t found = fileName.toStdString().find(STLext);
	std::size_t found2 = fileName.toStdString().find(STLext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 0;
		//STL
	}

	found = fileName.toStdString().find(VTKext);
	found2 = fileName.toStdString().find(VTKext2);
	std::size_t found3 = fileName.toStdString().find(VTKext3);
	std::size_t found4 = fileName.toStdString().find(VTKext4);
	if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)
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
	
	
	//0 = stl, 1 = vtk,  2 = ply, 3 = ntw, 4 ver, 5 cur, 6 flg, 7 lmk
	found = fileName.toStdString().find(NTWext);
	found2 = fileName.toStdString().find(NTWext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 3; //NTW
	}

	//4 ver, 5 cur, 6 flg, 7 lmk
	found = fileName.toStdString().find(VERext);
	found2 = fileName.toStdString().find(VERext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 4; //VER
	}
	
	found = fileName.toStdString().find(CURext);
	found2 = fileName.toStdString().find(CURext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 5; //CUR
	}
	found = fileName.toStdString().find(FLGext);
	found2 = fileName.toStdString().find(FLGext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 6; //FLG
	}
	found = fileName.toStdString().find(LMKext);
	found2 = fileName.toStdString().find(LMKext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 7; //LMK
	}
	found = fileName.toStdString().find(TAGext);
	found2 = fileName.toStdString().find(TAGext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 8; //TAG
	}
	found = fileName.toStdString().find(STVext);
	found2 = fileName.toStdString().find(STVext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 9; //STV
	}
	//8 tag, 9 stv, 10 ori, 11 pos
	found = fileName.toStdString().find(ORIext);
	found2 = fileName.toStdString().find(ORIext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 10; //ORI
	}
	found = fileName.toStdString().find(POSext);
	found2 = fileName.toStdString().find(POSext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 11; //POS
	}


	if (type < 3)
	{
		this->OpenMesh(fileName);
	}
	else if (type == 3)
	{
		this->OpenNTW(fileName);
	}
	else if (type == 4)
	{
		this->OpenVER(fileName,0);
	}
	else if (type == 5)
	{
		this->OpenCUR(fileName);
	}
	else if (type == 6)
	{
		this->OpenFLG(fileName);
	}
	else if (type == 7)
	{
		this->OpenLMK(fileName, 0);
	}
	else if (type == 8)
	{
		this->OpenTAG(fileName);
	}
	else if (type == 9)
	{
		this->OpenSTV(fileName);
	}
	else if (type == 10)
	{
		this->OpenORI(fileName);
	}
	else if (type == 11)
	{
		this->OpenPOS(fileName, 1);
	}
}

void mqOpenDataReaction::OpenNTW()
{
	cout << "Open NTW!" << endl;
	//char param1[1000];
	int i = 0;
	
	QString NTWfile = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load project"), QDir::currentPath(),
		tr("NTW (*.ntw)"));

	cout << NTWfile.toStdString() << endl;
	if (NTWfile.isEmpty()) return;
	this->OpenNTW(NTWfile);
}

void mqOpenDataReaction::OpenFLG(QString fileName)
{
	cout << "OpenFLG " << fileName.toStdString() << endl;
	double  x, y, z, nx, ny, nz, flength, r,g,b;

	QString FLGName;

	//Open a landmark file!


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

			std::string FLGext(".flg");
			std::string FLGext2(".FLG");
			

			int type = 0; // 0 = .POS Ascii File //1 = .MAT binary File or simple .MAT file

			std::size_t found = fileName.toStdString().find(FLGext);
			std::size_t found2 = fileName.toStdString().find(FLGext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 1;
				//FLG
			}



			if (type == 1)
			{

				
				//filein = fopen(fileName.toStdString().c_str(), "rt");
				QFile inputFile(fileName);
				int ok = 0;

				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);
					int cpt = 1;
					while (!in.atEnd())
					{
						QString line = in.readLine();
						QTextStream myteststream(&line);
						if (cpt % 2 == 0)
						{
							
							myteststream >> x >> y >> z >> nx >> ny >> nz>>flength>>r>>g>>b;
							double coord[3] = { x,y,z };
							double ncoord[3] = { nx,ny,nz };
							double ori[3];

							double length = nx*nx + ny*ny + nz*nz;
							if (length == 1)
							{
								ori[0] = ncoord[0];
								ori[1] = ncoord[1];
								ori[2] = ncoord[2];
							}
							else
							{
								vtkMath::Subtract(ncoord, coord, ori);
								vtkMath::Normalize(ori);
							}
							mqMeshToolsCore::instance()->CreateLandmark(coord, ori, 4);
							vtkLMActor *myLastFlag = mqMeshToolsCore::instance()->GetLastLandmark(4);
							myLastFlag->SetmColor(r, g, b, 1);
							myLastFlag->SetLMText(FLGName.toStdString());
							myLastFlag->SetLMSize(flength);
							myLastFlag->SetChanged(1);
						}
						else
						{
							FLGName= line;
						}
						cpt++;

					}
					/**/

					inputFile.close();


				}
			}//fin if																		

		}//file exists...
	}

}
void mqOpenDataReaction::OpenCUR(QString fileName)

{
	double  xn, yn, zn, xh, yh, zh;// coordinates of curve nodes and curve handles
	int node_type;
	QString LMKName;
	//Open a landmark file!


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

			std::string CURext(".cur");
			std::string CURext2(".CUR");
			

			int type = 0;

			std::size_t found = fileName.toStdString().find(CURext);
			std::size_t found2 = fileName.toStdString().find(CURext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 1;
				//CUR
			}

			

			if (type == 1)
			{

			//filein = fopen(fileName.toStdString().c_str(), "rt");
				QFile inputFile(fileName);
				int ok = 0;

				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);

					while (!in.atEnd())
					{

						QString line = in.readLine();
						QTextStream myteststream(&line);
						myteststream >> LMKName >> xn >> yn >> zn >> xh >> yh >> zh>> node_type;
						double coordn[3] = { xn,yn,zn };
						double coordh[3] = { xh,yh,zh };
						double ori[3];

						
							ori[0] = 0;
							ori[1] = 0;
							ori[2] = 1;
						
						mqMeshToolsCore::instance()->CreateLandmark(coordn, ori, 2, node_type);
						mqMeshToolsCore::instance()->CreateLandmark(coordh, ori, 3);

					}
					/**/

					inputFile.close();


				}
			}//fin if																		

		}//file exists...
	}	//length

	/*
		float  param2, param3, param4, param5, param6, param7;
		int param8;
		float m_ved[3], m_veh[3], m_ven[3], leng; //ved is first landmarks // veh is handles
		char param1[10];
		FILE	*filein;									// Filename To Open
		OBJECT_LANDMARK * My_LandmarkD;
		OBJECT_LANDMARK * My_LandmarkH;
		char	oneline[255];
		int start;

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

				//filein = fopen(szFile, "rt");
				filein = fopen(filename.c_str(), "rt");
				readstr(filein, oneline);
				while (!feof(filein))
				{
					//sscanf_s(oneline, "%s %f %f %f %f %f %f %d\n", &param1, &param2, &param3, &param4, &param5, &param6, &param7, &param8);
					sscanf(oneline, "%s %f %f %f %f %f %f %d\n", &param1, &param2, &param3, &param4, &param5, &param6, &param7, &param8);
					m_ved[0] = param2; m_ved[1] = param3; m_ved[2] = param4;
					m_veh[0] = param5; m_veh[1] = param6; m_veh[2] = param7;
					start = param8;
					//if (start !=1){start=0;}
					m_ven[0] = 0; m_ven[1] = 0; m_ven[2] = 1;
					leng = sqrt(m_ven[0] * m_ven[0] + m_ven[1] * m_ven[1] + m_ven[2] * m_ven[2]);
					if (leng != 0)
					{
						m_ven[0] /= leng;
						m_ven[1] /= leng;
						m_ven[2] /= leng;
					}
					//m_ven[0] = param5;m_ven[1] = param6;m_ven[2] = param7;
					My_LandmarkD = new OBJECT_LANDMARK;
					My_LandmarkH = new OBJECT_LANDMARK;
					glMatrix M1, M2;
					glPushMatrix();
					glLoadIdentity();
					//getMatrix((GLfloat*) M1);
					getmatrix(M2);
					getmatrix(M1);
					glPopMatrix();
					M1[0][0] = M1[1][1] = M1[2][2] = g_landmark_size;
					// transl. components:
					M2[3][0] = m_ved[0]; M2[3][1] = m_ved[1]; M2[3][2] = m_ved[2];
					// rot. components:
					// assume rot. about x and z axes:
					// -> values sinx,cosx,sinz,cosz
					float cosx = m_ven[2];
					float sinx = sqrt(1.0 - (cosx*cosx));
					float cosz, sinz;
					if (sinx == 0.0)
					{
						cosz = 1.0; sinz = 0.0;	// no rotation around z
					}
					else
					{
						cosz = -m_ven[1] / sinx;
						sinz = m_ven[0] / sinx;
					}
					M2[0][0] = cosz; M2[0][1] = sinz; M2[0][2] = 0;
					M2[1][0] = -cosx*sinz; M2[1][1] = cosx*cosz; M2[1][2] = sinx;
					M2[2][0] = m_ven[0]; M2[2][1] = m_ven[1]; M2[2][2] = m_ven[2];
					My_LandmarkD->curve_start = start;
					My_LandmarkD->Mat1[0][0] = M1[0][0];
					My_LandmarkD->Mat1[1][1] = M1[1][1];
					My_LandmarkD->Mat1[2][2] = M1[2][2];

					My_LandmarkD->Mat2[0][0] = M2[0][0];
					My_LandmarkD->Mat2[0][1] = M2[0][1];
					My_LandmarkD->Mat2[0][2] = M2[0][2];
					My_LandmarkD->Mat2[0][3] = M2[0][3];
					My_LandmarkD->Mat2[1][0] = M2[1][0];
					My_LandmarkD->Mat2[1][1] = M2[1][1];
					My_LandmarkD->Mat2[1][2] = M2[1][2];
					My_LandmarkD->Mat2[1][3] = M2[1][3];
					My_LandmarkD->Mat2[2][0] = M2[2][0];
					My_LandmarkD->Mat2[2][1] = M2[2][1];
					My_LandmarkD->Mat2[2][2] = M2[2][2];
					My_LandmarkD->Mat2[2][3] = M2[2][3];
					My_LandmarkD->Mat2[3][0] = M2[3][0];
					My_LandmarkD->Mat2[3][1] = M2[3][1];
					My_LandmarkD->Mat2[3][2] = M2[3][2];
					My_LandmarkD->Mat2[3][3] = M2[3][3];


					if (start == 0)
					{
						My_LandmarkD->color[0] = 1;
						My_LandmarkD->color[1] = 0;
						My_LandmarkD->color[2] = 0;
						My_LandmarkD->color[3] = 1;
					}
					else if (start == 1)
					{
						My_LandmarkD->color[0] = 0;
						My_LandmarkD->color[1] = 1;
						My_LandmarkD->color[2] = 0;
						My_LandmarkD->color[3] = 1;
					}
					else if (start == 2)
					{
						My_LandmarkD->color[0] = 0;
						My_LandmarkD->color[1] = 0;
						My_LandmarkD->color[2] = 1;
						My_LandmarkD->color[3] = 1;
					}
					else if (start == 3)
					{
						My_LandmarkD->color[0] = 0.6;
						My_LandmarkD->color[1] = 0.2;
						My_LandmarkD->color[2] = 0.5;
						My_LandmarkD->color[3] = 1;
					}

					M2[3][0] = m_veh[0]; M2[3][1] = m_veh[1]; M2[3][2] = m_veh[2];
					My_LandmarkH->Mat1[0][0] = 1.5*M1[0][0];
					My_LandmarkH->Mat1[1][1] = 1.5*M1[1][1];
					My_LandmarkH->Mat1[2][2] = 1.5*M1[2][2];

					My_LandmarkH->Mat2[0][0] = M2[0][0];
					My_LandmarkH->Mat2[0][1] = M2[0][1];
					My_LandmarkH->Mat2[0][2] = M2[0][2];
					My_LandmarkH->Mat2[0][3] = M2[0][3];
					My_LandmarkH->Mat2[1][0] = M2[1][0];
					My_LandmarkH->Mat2[1][1] = M2[1][1];
					My_LandmarkH->Mat2[1][2] = M2[1][2];
					My_LandmarkH->Mat2[1][3] = M2[1][3];
					My_LandmarkH->Mat2[2][0] = M2[2][0];
					My_LandmarkH->Mat2[2][1] = M2[2][1];
					My_LandmarkH->Mat2[2][2] = M2[2][2];
					My_LandmarkH->Mat2[2][3] = M2[2][3];
					My_LandmarkH->Mat2[3][0] = M2[3][0];
					My_LandmarkH->Mat2[3][1] = M2[3][1];
					My_LandmarkH->Mat2[3][2] = M2[3][2];
					My_LandmarkH->Mat2[3][3] = M2[3][3];


					My_LandmarkH->color[0] = 1.0;
					My_LandmarkH->color[1] = 0.7;
					My_LandmarkH->color[2] = 0.2;
					My_LandmarkH->color[3] = 0.5;


					Cont_Mesh.Add_Landmark(My_LandmarkD, 0);
					Cont_Mesh.Add_Landmark(My_LandmarkH, 1);
					readstr(filein, oneline); //read next line

				}//While scanff...		
				fclose(filein);
			}//if file exists
	*/
}
void mqOpenDataReaction::OpenTAG(QString fileName) {}
void mqOpenDataReaction::OpenORI(QString fileName) 
{

	QString X1,X2,Y1,Y2,Z1,Z2;

	
	

	
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

			std::string ORIext(".ori");
			std::string ORIext2(".ORI");
			

			int type = 0; // 0 = .POS Ascii File //1 = .MAT binary File or simple .MAT file

			std::size_t found = fileName.toStdString().find(ORIext);
			std::size_t found2 = fileName.toStdString().find(ORIext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 1;
				//ORI
			}

			int cpt = 0;


			if (type == 1)
			{
				
				//filein = fopen(fileName.toStdString().c_str(), "rt");
				QFile inputFile(fileName);
				int ok = 0;

				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);
					
					while (!in.atEnd())
					{

						QString line = in.readLine();
						if (cpt == 0) { Z1 = line; }
						if (cpt == 1) { Z2 = line; }
						if (cpt == 2) { Y1 = line; }
						if (cpt == 3) { Y2 = line; }
						if (cpt == 4) { X1 = line; }
						if (cpt == 5) { X2 = line; }
						cpt++;						
						
					}


					inputFile.close();
					mqMeshToolsCore::instance()->Setmui_X1Label(X1);
					mqMeshToolsCore::instance()->Setmui_X2Label(X2);
					mqMeshToolsCore::instance()->Setmui_Y1Label(Y1);
					mqMeshToolsCore::instance()->Setmui_Y2Label(Y2);
					mqMeshToolsCore::instance()->Setmui_Z1Label(Z1);
					mqMeshToolsCore::instance()->Setmui_Z2Label(Z2);
					mqMeshToolsCore::instance()->SetOrientationHelperLabels(X1.toStdString(), X2.toStdString(), Y1.toStdString(), Y2.toStdString(), Z1.toStdString(), Z2.toStdString());
					

				}
			}//fin if																		

		}//file exists...
	}	//length*/
}

//-----------------------------------------------------------------------------
void mqOpenDataReaction::OpenNTW(QString fileName)
{
	
	
	int file_exists = 1;
	int i = 0;
	QFile file(fileName);
	QFileInfo fileInfo(fileName);
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
		std::string path = fileName.toStdString().substr(0, (fileName.toStdString().length() - only_filename.length()));
		cout << "only_filename" << only_filename << endl;
		cout << "path" << path << endl;
		mqMeshToolsCore::instance()->UnselectAll(-1);





		QFile inputFile(fileName);
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
					
					QString flgfile(myline.c_str());
					this->OpenFLG(flgfile);

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
					
					QString verfile(myline.c_str());
					this->OpenVER(verfile, landmark_mode);
					

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
					std::cout << "Try to load CUR curve file :<<" << myline.c_str() << std::endl;
					QString curfile(myline.c_str());
					this->OpenCUR(curfile);					

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
					std::cout << "Try to load STV curve file :<<" << myline.c_str() << std::endl;					
					QString stvfile(myline.c_str());
					this->OpenSTV(stvfile);					


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
					QString orifile(myline.c_str());
					this->OpenORI(orifile);					

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
					std::cout << "Try to load tag file :<<" << myline.c_str() << std::endl;
					QString tagfile(myline.c_str());
					this->OpenTAG(tagfile);


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



						this->OpenMesh(meshfile);
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
							this->OpenPOS(qposfile, 0);
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
							actor->SetSelected(0);
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

void mqOpenDataReaction::OpenPOS(QString fileName, int mode)
{
	// mode : 0 for last inserted mesh
	// mode : 1 for all selected meshes
	// mode : 2 for all selected landmarks/flags


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
						//Mat1[j][i] = u.f;
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
					inputFile.close();

				}
			}//fin if	
			


			//cout << "call meshtools apply mat" << &Mat << endl;
			mqMeshToolsCore::instance()->ApplyMatrix(Mat, mode);
		}//file exists...
	}	//length

}

void mqOpenDataReaction::OpenPOSTrans(QString fileName, int mode)
{
	// mode : 0 for last inserted mesh
	// mode : 1 for all selected meshes
	// mode : 2 for all selected landmarks/flags


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
						Mat->SetElement(i, j, double(u.f));

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

						Mat->SetElement(i,0, n1);
						Mat->SetElement(i,1, n2);
						Mat->SetElement(i,2, n3);
						Mat->SetElement(i,3, n4);


					}
					inputFile.close();

				}
			}//fin if		
			
			 double N1, N2, N3;
			 N1 = -(Mat->GetElement(3,0) * Mat->GetElement(0, 0) +
			 Mat->GetElement(3, 1) * Mat->GetElement(0, 1)
			 + Mat->GetElement(3, 2) * Mat->GetElement(0, 2));
			 			 

			 
			 Mat->SetElement(0, 3, N1);



			 N2 =-(Mat->GetElement(3, 0) * Mat->GetElement(1, 0) +
			 Mat->GetElement(3, 1) * Mat->GetElement(1, 1)
			 + Mat->GetElement(3, 2) * Mat->GetElement(1, 2));
			
			 Mat->SetElement(1, 3, N2);

			 N3 = -(Mat->GetElement(3, 0) * Mat->GetElement(2, 0) +
			 Mat->GetElement(3, 1) * Mat->GetElement(2, 1)
			 + Mat->GetElement(3, 2) * Mat->GetElement(2, 2));

			 Mat->SetElement(2, 3, N3);


			 Mat->SetElement(3,0, 0);
			 Mat->SetElement(3, 1, 0);
			 Mat->SetElement(3, 2, 0);
			
			 //cout << "call meshtools apply mat" << &Mat << endl;
			mqMeshToolsCore::instance()->ApplyMatrix(Mat, mode);
		}//file exists...
	}	//length

}

void mqOpenDataReaction::OpenLMK(QString fileName, int mode)
{// mode : 0 for normal landmarks
 // mode : 1 for target landmarks
	double  x, y, z;
	QString LMKName;
	//Open a landmark file!


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

			std::string LMKext(".LMK");
			std::string LMKext2(".LMK");

			std::size_t found = fileName.toStdString().find(LMKext);
			std::size_t found2 = fileName.toStdString().find(LMKext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
			
				//filein = fopen(fileName.toStdString().c_str(), "rt");
				QFile inputFile(fileName);
				int ok = 0;

				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);

					while (!in.atEnd())
					{

						QString line = in.readLine();
						QTextStream myteststream(&line);
						myteststream >> LMKName >> x >> y >> z;
						double coord[3] = { x,y,z };
						double ori[3];

						
						ori[0] = 0;
						ori[1] = 0;
						ori[2] = 1;
						
						mqMeshToolsCore::instance()->CreateLandmark(coord, ori, mode);

					}
					/**/

					inputFile.close();


				}

			}//fin if																		

		}//file exists...
	}	//length


}
void mqOpenDataReaction::OpenVER(QString fileName, int mode)
{// mode : 0 for normal landmarks
 // mode : 1 for target landmarks
 // mode : 2 for curve nodes
 // mode : 3 for curve handles
	double  x, y, z, nx, ny, nz;
	QString LMKName;
	//Open a landmark file!

	
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

					while (!in.atEnd())
					{

						QString line = in.readLine();
						QTextStream myteststream(&line);
						myteststream >> LMKName >> x >> y >> z >> nx >> ny >> nz;
						double coord[3] = { x,y,z };
						double ncoord[3] = { nx,ny,nz };
						double ori[3];
						
							double length = nx*nx + ny*ny + nz*nz;
						if (length == 1)
						{
							ori[0] = ncoord[0];
							ori[1] = ncoord[1];
							ori[2] = ncoord[2];
						}
						else
						{
							vtkMath::Subtract(ncoord, coord, ori);
							vtkMath::Normalize(ori);
						}
						mqMeshToolsCore::instance()->CreateLandmark(coord, ori, mode);
						
					}
						/**/

					inputFile.close();
					

				}
			}//fin if																		
			
		}//file exists...
	}	//length
	

}

void mqOpenDataReaction::OpenMesh(QString fileName)
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
		std::string VTKext3(".vtp");
		std::string VTKext4(".VTP");
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
		std::size_t found3 = fileName.toStdString().find(VTKext3);
		std::size_t found4 = fileName.toStdString().find(VTKext4);
		if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)
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
