/*=========================================================================

   Program: MeshTools
   Module:    mqOpenDataReaction.cxx

========================================================================*/
#include "mqOpenDataReaction.h"

#include "mqMeshToolsCore.h"
#include "mqUndoStack.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>




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
		tr("Load surface"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
		tr("surfaces (*.ply *.stl *.vtk)"));

	

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;
	
	QFileInfo fileInfo(fileName);
	mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	
	mqMeshToolsCore::instance()->OpenMesh(fileName);
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
		tr("Load landmarks"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
		tr("landmark files(*.ver *.lmk)"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());
	
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
		
		mqMeshToolsCore::instance()->OpenVER(fileName, mode);
	}
	else 
	{
		mqMeshToolsCore::instance()->OpenLMK(fileName, mode);
	}

}
void mqOpenDataReaction::OpenSTV()
{
	//mqMeshToolsCore::instance()->getUndoStack();
	cout << "Open STV!" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load meshtool landmarks/curve files"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
		tr("Landmark files(*.stv)"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;

	QFileInfo fileInfo(fileName);
	mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());

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

		mqMeshToolsCore::instance()->OpenSTV(fileName);
	}
	

}


void mqOpenDataReaction::OpenPOS(int mode)
{
	if (mode < 1) { mode = 1; }
	// mode : 1 for all selected meshes
	// mode : 2 for all selected landmarks/flags
	
	if (mode == 1)
	{
		vtkIdType num_selected_meshes = mqMeshToolsCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
		if (num_selected_meshes == 0) {
			QMessageBox msgBox;
			msgBox.setText("No surface selected. Please select at least one surface to use this option.");
			msgBox.exec();
			return;
		}
	}
	else
	{
		vtkIdType num_selected_landmarks = 0;
		num_selected_landmarks+= mqMeshToolsCore::instance()->getNormalLandmarkCollection()->GetNumberOfSelectedActors();
		num_selected_landmarks += mqMeshToolsCore::instance()->getTargetLandmarkCollection()->GetNumberOfSelectedActors();
		num_selected_landmarks += mqMeshToolsCore::instance()->getNodeLandmarkCollection()->GetNumberOfSelectedActors();
		num_selected_landmarks += mqMeshToolsCore::instance()->getHandleLandmarkCollection()->GetNumberOfSelectedActors();
		num_selected_landmarks += mqMeshToolsCore::instance()->getFlagLandmarkCollection()->GetNumberOfSelectedActors();

		if (num_selected_landmarks == 0) {
			QMessageBox msgBox;
			msgBox.setText("No landmark/flag selected. Please select at least one landmark or flag to use this option.");
			msgBox.exec();
			return;
		}
	}
	//open and applies position 
	
	cout << "Open POS" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load position file"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
		tr("position file (*.pos)"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;	
	QFileInfo fileInfo(fileName);
	mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());
	mqMeshToolsCore::instance()->OpenPOS(fileName,mode);
	
}

void mqOpenDataReaction::OpenPOSTrans(int mode)
{
	if (mode < 1) { mode = 1; }
	// mode : 1 for all selected meshes
	// mode : 2 for all selected landmarks/flags

	if (mode == 1)
	{
		vtkIdType num_selected_meshes = mqMeshToolsCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
		if (num_selected_meshes == 0) {
			QMessageBox msgBox;
			msgBox.setText("No surface selected. Please select at least one surface to use this option.");
			msgBox.exec();
			return;
		}
	}
	else
	{
		vtkIdType num_selected_landmarks = 0;
		num_selected_landmarks += mqMeshToolsCore::instance()->getNormalLandmarkCollection()->GetNumberOfSelectedActors();
		num_selected_landmarks += mqMeshToolsCore::instance()->getTargetLandmarkCollection()->GetNumberOfSelectedActors();
		num_selected_landmarks += mqMeshToolsCore::instance()->getNodeLandmarkCollection()->GetNumberOfSelectedActors();
		num_selected_landmarks += mqMeshToolsCore::instance()->getHandleLandmarkCollection()->GetNumberOfSelectedActors();
		num_selected_landmarks += mqMeshToolsCore::instance()->getFlagLandmarkCollection()->GetNumberOfSelectedActors();

		if (num_selected_landmarks == 0) {
			QMessageBox msgBox;
			msgBox.setText("No landmark/flag selected. Please select at least one landmark or flag to use this option.");
			msgBox.exec();
			return;
		}
	}


	//open and applies transpoed position 
	//mqMeshToolsCore::instance()->getUndoStack();
	cout << "Open transposed POS" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load transposed position file"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
		tr("transposed position file (*.pos)"));

	cout << fileName.toStdString() << endl;
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	mqMeshToolsCore::instance()->OpenPOSTrans(fileName, mode);

}

void mqOpenDataReaction::OpenORI()
{
	
	cout << "Open ORI" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load orientation file"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
		tr("orientation file (*.ori)"));

	cout << fileName.toStdString() << endl;;
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	mqMeshToolsCore::instance()->OpenORI(fileName);

}
void mqOpenDataReaction::OpenFLG()
{

	cout << "Open FLG" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load flag file"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
		tr("flag file (*.flg)"));

	cout << fileName.toStdString() << endl;;
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());
	mqMeshToolsCore::instance()->OpenFLG(fileName);

}
void mqOpenDataReaction::OpenCUR()
{

	cout << "Open CUR" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load CUR file"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
		tr("curve file (*.cur)"));

	cout << fileName.toStdString()<<endl;
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());
	mqMeshToolsCore::instance()->OpenCUR(fileName);

}
void mqOpenDataReaction::OpenTAG()
{

	cout << "Open CUR" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load TAG file"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
		tr("tag file (*.tag)"));

	cout << fileName.toStdString()<<endl;
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	mqMeshToolsCore::instance()->OpenTAG(fileName);

}
void mqOpenDataReaction::OpenData()
{
	//mqMeshToolsCore::instance()->getUndoStack();
	cout << "Open Data!" << endl;
	
	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load data"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
		tr("meshtools data or project (*.ntw *.ver *.cur *.stv *.tag *.pos *.ori *.flg *.lmk *.ply *.stl *.vtk *.vtp)"));

	cout << fileName.toStdString()<<endl;
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());

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
		mqMeshToolsCore::instance()->OpenMesh(fileName);
	}
	else if (type == 3)
	{
		mqMeshToolsCore::instance()->OpenNTW(fileName);
	}
	else if (type == 4)
	{
		mqMeshToolsCore::instance()->OpenVER(fileName,0);
	}
	else if (type == 5)
	{
		mqMeshToolsCore::instance()->OpenCUR(fileName);
	}
	else if (type == 6)
	{
		mqMeshToolsCore::instance()->OpenFLG(fileName);
	}
	else if (type == 7)
	{
		mqMeshToolsCore::instance()->OpenLMK(fileName, 0);
	}
	else if (type == 8)
	{
		mqMeshToolsCore::instance()->OpenTAG(fileName);
	}
	else if (type == 9)
	{
		mqMeshToolsCore::instance()->OpenSTV(fileName);
	}
	else if (type == 10)
	{
		mqMeshToolsCore::instance()->OpenORI(fileName);
	}
	else if (type == 11)
	{
		mqMeshToolsCore::instance()->OpenPOS(fileName, 1);
	}
}

void mqOpenDataReaction::OpenNTW()
{
//char param1[1000];
	int i = 0;
	/*
	mqMeshToolsCore::instance()->Getmui_LastUsedDir()
	QFileInfo fileInfo(fileName);
	mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	*/
	QString NTWfile = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load project"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
		tr("NTW (*.ntw)"));
	
	cout << NTWfile.toStdString() << endl;
	if (NTWfile.isEmpty()) return;
	
	QFileInfo fileInfo(NTWfile);
	mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());
	

	mqMeshToolsCore::instance()->OpenNTW(NTWfile);
}




//-----------------------------------------------------------------------------


