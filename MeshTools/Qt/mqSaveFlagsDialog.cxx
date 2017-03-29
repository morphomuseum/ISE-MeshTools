/*=========================================================================

   Program: MeshTools
   Module:    $RCS $

 
=========================================================================*/

#include "mqSaveFlagsDialog.h"
#include "ui_mqSaveFlagsDialog.h"
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
mqSaveFlagsDialog::mqSaveFlagsDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqSaveFlagsDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqSaveFlagsDialog");
	
	
	// This is where we 
  //
 
 this->Ui->SaveAll->setChecked(true);
 
  // Should connect...
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSaveFlagFile()));

}




//-----------------------------------------------------------------------------
mqSaveFlagsDialog::~mqSaveFlagsDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSaveFlagsDialog::slotSaveFlagFile()
{
	cout << "Flag File Saved!" << endl;
	
	QString fileName;
	
	
		fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
			tr("Save Flag files"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
			tr("Flag file (*.flg)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);
	
	
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());


	int save_only_selected = 0; //0 no= save all landmark, 1 yes, save only selected landmarks
	
	

	if (this->Ui->SaveOnlySelected->isChecked()) { save_only_selected = 1; }
	else if (this->Ui->SaveAll->isChecked()) { save_only_selected = 0; }
	

	mqMeshToolsCore::instance()->SaveFlagFile(fileName, save_only_selected);

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

