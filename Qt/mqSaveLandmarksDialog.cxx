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
	
	if (this->Ui->VER->isChecked())
	{
		fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
			tr("Save Landmark files"), QDir::currentPath(),
			tr("Landmark file (*.ver)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);
	}
	if (this->Ui->LMK->isChecked())
	{
		fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
			tr("Save Landmark files"), QDir::currentPath(),
			tr("Landmark file (*.lmk)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);
	}
	
	if (fileName.isEmpty()) return;

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

