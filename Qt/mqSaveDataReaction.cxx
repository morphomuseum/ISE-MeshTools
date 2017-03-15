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

		mqMeshToolsCore::instance()->SavePOS(Mat, fileName);
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
	mqMeshToolsCore::instance()->SaveORI(fileName);
	

}



//-----------------------------------------------------------------------------






