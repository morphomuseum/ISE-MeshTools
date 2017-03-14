/*=========================================================================

   Program: MeshTools
   Module:    $RCS $

 
=========================================================================*/

#include "mqSaveSTVDialog.h"
#include "ui_mqSaveSTVDialog.h"
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
mqSaveSTVDialog::mqSaveSTVDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqSaveSTVDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqSaveSTVDialog");
	
	
	// This is where we 
  //
 
 this->Ui->SaveAll->setChecked(true);
 
  // Should connect...
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSaveSTVFile()));

}




//-----------------------------------------------------------------------------
mqSaveSTVDialog::~mqSaveSTVDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSaveSTVDialog::slotSaveSTVFile()
{
	cout << "STV File Saved!" << endl;
	
	QString fileName;
	
	
		fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
			tr("Save STV files"), QDir::currentPath(),
			tr("STV file (*.stv)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);
	
	
	if (fileName.isEmpty()) return;

	int save_only_selected = 0; //0 no= save all landmark, 1 yes, save only selected landmarks
	
	

	if (this->Ui->SaveOnlySelected->isChecked()) { save_only_selected = 1; }
	else if (this->Ui->SaveAll->isChecked()) { save_only_selected = 0; }
	

	mqMeshToolsCore::instance()->SaveSTVFile(fileName, save_only_selected);

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

