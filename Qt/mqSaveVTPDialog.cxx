/*=========================================================================

   Program: MeshTools
   Module:    $RCS $

 
=========================================================================*/

#include "mqSaveVTPDialog.h"
#include "ui_mqSaveVTPDialog.h"
#include "MeshToolsVersion.h"
#include "mqMeshToolsCore.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
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
mqSaveVTPDialog::mqSaveVTPDialog(QWidget* Parent, QString fileName)
  : QDialog(Parent)
  , Ui(new Ui::mqSaveVTPDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqSaveVTPDialog");
	this->m_fileName = fileName;
	// This is where we 
  //
 
 
 this->Ui->Binary->setChecked(true);
 this->Ui->PositionOriginal->setChecked(true);
 
  // Should connect...
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSaveVTPFile()));

}




//-----------------------------------------------------------------------------
mqSaveVTPDialog::~mqSaveVTPDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSaveVTPDialog::slotSaveVTPFile()
{
	cout << "VTP File Saved!" << endl;
	int position_mode = 0; // 0 Original position , 1 Modified position
	int file_type = 1; // 0 STL 1 VTK/VTP, 2 PLY
	int save_norms = 0; //0 no, 1 yes
	int write_type = 0;//0 binary LE, 1 binary BE, 2 ASCII
	if (this->Ui->Binary->isChecked()) { write_type = 0; }	
	else if (this->Ui->ASCII->isChecked()) { write_type = 2; }

	if (this->Ui->PositionModified->isChecked()) { position_mode = 1; }
	else if (this->Ui->PositionOriginal->isChecked()) { position_mode = 0; }
	


	mqMeshToolsCore::instance()->SaveSurfaceFile(this->m_fileName, write_type, position_mode, file_type, save_norms);

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

