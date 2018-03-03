/*=========================================================================

   Program: MeshTools
   Module:    $RCS $

 
=========================================================================*/

#include "mqSaveLandmarksDialog.h"
#include "ui_mqSaveLandmarksDialog.h"
#include "MeshToolsVersion.h"
#include "mqMeshToolsCore.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QHeaderView>


#include <sstream>

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT "64"
#else
#define ENVIRONMENT "32"
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT "64"
#else
#define ENVIRONMENT "32"
#endif
#endif

//-----------------------------------------------------------------------------
mqSaveLandmarksDialog::mqSaveLandmarksDialog(QWidget* Parent, int lmtype)
  : QDialog(Parent)
  , Ui(new Ui::mqSaveLandmarksDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqSaveLandmarksDialog");
	
	this->m_lmtype = lmtype;
	// This is where we 
  //
 
 
 this->Ui->VER->setChecked(true);
 this->Ui->SaveAll->setChecked(true);
 
  // Should connect...
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSaveLandmarkFile()));

}




//-----------------------------------------------------------------------------
mqSaveLandmarksDialog::~mqSaveLandmarksDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSaveLandmarksDialog::slotSaveLandmarkFile()
{
	cout << "Landmark File Saved!" << endl;

	QString fileName;
	QString proposedName = "";
	vtkIdType num_meshes = mqMeshToolsCore::instance()->getActorCollection()->GetNumberOfItems();
	if (num_meshes == 1)
	{
		mqMeshToolsCore::instance()->getActorCollection()->InitTraversal();
		vtkMTActor *myActor = vtkMTActor::SafeDownCast(mqMeshToolsCore::instance()->getActorCollection()->GetNextActor());
		proposedName += QDir::separator();
		proposedName += myActor->GetName().c_str();
	}
	else
	{
		vtkIdType num_selected_meshes = mqMeshToolsCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
		if (num_selected_meshes == 1)
		{
			mqMeshToolsCore::instance()->ComputeSelectedNamesLists();
			proposedName += QDir::separator();
			proposedName += +mqMeshToolsCore::instance()->g_distinct_selected_names.at(0).c_str();
		}
	}
	if (this->Ui->VER->isChecked())
	{
		fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
			tr("Save Landmark files"), mqMeshToolsCore::instance()->Getmui_LastUsedDir()+ proposedName,
			tr("Landmark file (*.ver)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);
	}
	if (this->Ui->LMK->isChecked())
	{
		fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
			tr("Save Landmark files"), mqMeshToolsCore::instance()->Getmui_LastUsedDir()+ proposedName,
			tr("Landmark file (*.lmk)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);
	}
	
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());


	int file_type = 0; // 0 VER 1 LMK, 2 => to implement!
	int save_only_selected = 0; //0 no= save all landmark, 1 yes, save only selected landmarks
	
	if (this->Ui->VER->isChecked()) { file_type = 0; }
	else if (this->Ui->LMK->isChecked()) { file_type = 1; }
	

	if (this->Ui->SaveOnlySelected->isChecked()) { save_only_selected = 1; }
	else if (this->Ui->SaveAll->isChecked()) { save_only_selected = 0; }
	

	mqMeshToolsCore::instance()->SaveLandmarkFile(fileName, this->m_lmtype, file_type, save_only_selected);

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

