/*=========================================================================

   Program: MeshTools
   Module:    mqSaveDataReaction.cxx

========================================================================*/
#include "mqSaveDataReaction.h"

#include "mqMeshToolsCore.h"
#include "mqUndoStack.h"
#include "mqMeshToolsCore.h"
#include "vtkMTActor.h"

#include <QFileDialog>
#include <QMessageBox>
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
mqSaveDataReaction::mqSaveDataReaction(QAction* parentObject,  int _mode)
  : Superclass(parentObject)
{
  this->Mode = _mode;
  this->MainWindow = mqMeshToolsCore::instance()->GetMainWindow();
  
}


void mqSaveDataReaction::SaveLMK_or_VER(int mode)
{
	//mode 0=> inside normal landmarks
	// 1 => inside target landmarks
	//2 => inside node landmarks
	//3 => inside handle landmarks
	//mqMeshToolsCore::instance()->getUndoStack();
	cout << "Save LMK or VER!" << endl;
	
	vtkIdType num_selected_landmarks = 0;
	/*if (mode == 0)
	{
	= mqMeshToolsCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at one single surface to use this option.");
		msgBox.exec();
		return;
	}
	else if (num_selected_meshes > 1)
	{
		QMessageBox msgBox;
		msgBox.setText("More than one surface are currently selected. Please select at one single surface to use this option.");
		int ret = msgBox.exec();
		return;

	}
	vtkMTActor *FirstSelectedActor = mqMeshToolsCore::instance()->GetFirstSelectedActor();
	if (FirstSelectedActor != NULL) {

		vtkSmartPointer<vtkMatrix4x4> Mat = FirstSelectedActor->GetMatrix();


		QString fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
			tr("Save POS files"), QDir::currentPath(),
			tr("Pos file (*.pos)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);


		cout << fileName.toStdString();
		if (fileName.isEmpty()) return;


		//Save and applies position 
		//mqMeshToolsCore::instance()->getUndoStack();
		cout << "Save POS" << endl;
		this->SavePOS(Mat, fileName);
	}*/


}
void mqSaveDataReaction::SaveSTV()
{
	//mqMeshToolsCore::instance()->getUndoStack();
	cout << "Save STV!" << endl;

	QString fileName = QFileDialog::getSaveFileName(this->MainWindow,
		tr("Save meshtool landmarks/curve files"), QDir::currentPath(),
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

		this->SaveSTV(fileName);
	}
	

}
void mqSaveDataReaction::SaveSTV(QString fileName)
{
	//@@ TODO!

}

void mqSaveDataReaction::SavePOS()
{
	
	
	vtkIdType num_selected_meshes = mqMeshToolsCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at one single surface to use this option.");
		msgBox.exec();
		return;
	}
	else if (num_selected_meshes > 1)
	{
		QMessageBox msgBox;
		msgBox.setText("More than one surface are currently selected. Please select at one single surface to use this option.");		
		int ret = msgBox.exec();
		return;

	}
	vtkMTActor *FirstSelectedActor = mqMeshToolsCore::instance()->GetFirstSelectedActor();
	if (FirstSelectedActor != NULL) {	

		vtkSmartPointer<vtkMatrix4x4> Mat = FirstSelectedActor->GetMatrix();		


		QString fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
			tr("Save POS files"), QDir::currentPath(),
			tr("Pos file (*.pos)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);


		cout << fileName.toStdString();
		if (fileName.isEmpty()) return;
	
	
		//Save and applies position 
		//mqMeshToolsCore::instance()->getUndoStack();
		cout << "Save POS" << endl;	
		this->SavePOS(Mat, fileName);
	}

	
}


void mqSaveDataReaction::SaveORI()
{
	
	cout << "Save ORI" << endl;

	QString fileName = QFileDialog::getSaveFileName(this->MainWindow,
		tr("Save orientation file"), QDir::currentPath(),
		tr("orientation file (*.ori)"));

	cout << fileName.toStdString() << endl;;
	if (fileName.isEmpty()) return;
	this->SaveORI(fileName);

}
void mqSaveDataReaction::SaveFLG()
{

	cout << "Save FLG" << endl;

	QString fileName = QFileDialog::getSaveFileName(this->MainWindow,
		tr("Save flags"), QDir::currentPath(),
		tr("flag file (*.flg)"));

	cout << fileName.toStdString() << endl;;
	if (fileName.isEmpty()) return;
	this->SaveFLG(fileName);

}
void mqSaveDataReaction::SaveCUR()
{

	cout << "Save CUR" << endl;

	QString fileName = QFileDialog::getSaveFileName(this->MainWindow,
		tr("Save CUR file"), QDir::currentPath(),
		tr("curve file (*.cur)"));

	cout << fileName.toStdString()<<endl;
	if (fileName.isEmpty()) return;
	this->SaveCUR(fileName);

}
void mqSaveDataReaction::SaveTAG()
{

	cout << "Save TAG" << endl;

	QString fileName = QFileDialog::getSaveFileName(this->MainWindow,
		tr("Save TAG file"), QDir::currentPath(),
		tr("tag file (*.tag)"));

	cout << fileName.toStdString()<<endl;
	if (fileName.isEmpty()) return;
	this->SaveTAG(fileName);

}


void mqSaveDataReaction::SaveNTW()
{
	cout << "Save NTW!" << endl;
	//@@TODO!
}

void mqSaveDataReaction::SaveFLG(QString fileName)
{
	cout << "SaveFLG " << fileName.toStdString() << endl;
	double  x, y, z, nx, ny, nz, flength, r,g,b;

	QString FLGName;
	//@@TODO!

}
void mqSaveDataReaction::SaveCUR(QString fileName)

{
	double  xn, yn, zn, xh, yh, zh;// coordinates of curve nodes and curve handles
	int node_type;
	QString LMKName;
	//Open a landmark file!


	
}
void mqSaveDataReaction::SaveTAG(QString fileName) {}
void mqSaveDataReaction::SaveORI(QString fileName) 
{

	QString X1,X2,Y1,Y2,Z1,Z2;

	
	

	
}

//-----------------------------------------------------------------------------


void mqSaveDataReaction::SavePOS(vtkSmartPointer<vtkMatrix4x4> Mat, QString fileName)
{
	
	std::string POSext = ".pos";
	std::string POSext2 = ".POS";
	std::size_t found = fileName.toStdString().find(POSext);
	std::size_t found2 = fileName.toStdString().find(POSext2);
	if (found == std::string::npos && found2 == std::string::npos)
	{
		fileName.append(".pos");
	}

	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);

		stream << "1 0 0 0" << endl;
		stream << "0 1 0 0" << endl;
		stream << "0 0 1 0" << endl;
		stream << "0 0 0 1" << endl;
	

		stream << Mat->GetElement(0,0)<<" "<< Mat->GetElement(1, 0) << " "<< Mat->GetElement(2, 0) << " "<< Mat->GetElement(3, 0) << endl;
		stream << Mat->GetElement(0, 1) << " " << Mat->GetElement(1, 1) << " " << Mat->GetElement(2, 1) << " " << Mat->GetElement(3, 1) <<  endl;
		stream << Mat->GetElement(0, 2) << " " << Mat->GetElement(1, 2) << " " << Mat->GetElement(2, 2) << " " << Mat->GetElement(3, 2) <<  endl;
		stream << Mat->GetElement(0, 3) << " " << Mat->GetElement(1, 3) << " " << Mat->GetElement(2, 3) << " " << Mat->GetElement(3, 3) <<  endl;


	}
	file.close();

	
	
	

	


}



