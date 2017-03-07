/*=========================================================================

   Program: MeshTools
   Module:    mqSaveDataReaction.cxx

========================================================================*/
#include "mqSaveDataReaction.h"

#include "mqMeshToolsCore.h"
#include "mqUndoStack.h"

#include "mqMeshToolsCore.h"

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

//@@TODO
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/*
void mqSaveDataReaction::SavePLY()
{


	
		vtkIdType num_selected_meshes = mqMeshToolsCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
		if (num_selected_meshes == 0) {
			QMessageBox msgBox;
			msgBox.setText("No surface selected. Please select at least one surface to use this option.");
			msgBox.exec();
			return;
		}

	

 //mqMeshToolsCore::instance()->getUndoStack();
	cout << "Save PLY!" << endl;
	//@@ TODO!	
}
void mqSaveDataReaction::SaveSTL()
{



	vtkIdType num_selected_meshes = mqMeshToolsCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}



	//mqMeshToolsCore::instance()->getUndoStack();
	cout << "Save PLY!" << endl;
	//@@ TODO!	
}
void mqSaveDataReaction::SaveVTP()
{



	vtkIdType num_selected_meshes = mqMeshToolsCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}



	//mqMeshToolsCore::instance()->getUndoStack();
	cout << "Save PLY!" << endl;
	//@@ TODO!	
}
*/
void mqSaveDataReaction::SaveLMK_or_VER(int mode)
{
	//mode 0=> inside normal landmarks
	// 1 => inside target landmarks
	//2 => inside node landmarks
	//3 => inside handle landmarks
	//mqMeshToolsCore::instance()->getUndoStack();
	cout << "Save LMK or VER!" << endl;
	

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
	
	

	//Save and applies position 
	//mqMeshToolsCore::instance()->getUndoStack();
	cout << "Save POS" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load position file"), QDir::currentPath(),
		tr("position file (*.pos)"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;	
	this->SavePOS(fileName);
	
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


void mqSaveDataReaction::SavePOS(QString fileName)
{
	

	int i, j, l;
	size_t  length;


	length = fileName.toStdString().length();


}



