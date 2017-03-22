/*=========================================================================

   Program: MeshTools
   Module:    $RCS $

 
=========================================================================*/

#include "mqSaveCURasVERDialog.h"
#include "ui_mqSaveCURasVERDialog.h"
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
mqSaveCURasVERDialog::mqSaveCURasVERDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqSaveCURasVERDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqSaveCURasVERDialog");
	
	
	// This is where we 
  //
 
 this->Ui->CUROnly->setChecked(true);
 this->Ui->VER->setChecked(true);
 this->Ui->Decimation->setValue(20);  // Should connect...
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSaveCURasVERFile()));

}




//-----------------------------------------------------------------------------
mqSaveCURasVERDialog::~mqSaveCURasVERDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSaveCURasVERDialog::slotSaveCURasVERFile()
{
	cout << "Export Cur as Landmarks !" << endl;
	
	QString fileName;
	
	if (this->Ui->VER->isChecked())
	{
		fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
			tr("Export as .VER file"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
			tr("VER file (*.ver)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);

	}
	else
	{
		fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
			tr("Export as LMK file"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
			tr("LMK file (*.lmk)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);
	}
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());


	int save_format = 0; //0 .VER
	int save_other_lmks = 0; //0 no, 1 save also Normal and Target landmarks
	

	if (this->Ui->VER->isChecked()) { save_format = 0; }
	else if (this->Ui->LMK->isChecked()) { save_format = 1; }
	if (this->Ui->All->isChecked()) { save_other_lmks = 1; }
	int decimation = this->Ui->Decimation->value();

	mqMeshToolsCore::instance()->SaveCURasVERFile(fileName, decimation, save_format, save_other_lmks);

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

