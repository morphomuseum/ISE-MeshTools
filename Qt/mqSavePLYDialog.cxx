/*=========================================================================

   Program: MeshTools
   Module:    $RCS $

 
=========================================================================*/

#include "mqSavePLYDialog.h"
#include "ui_mqSavePLYDialog.h"
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
mqSavePLYDialog::mqSavePLYDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqSavePLYDialog())
{

	// This is where we 
  //connect(this->Ui->reinitcolorsButton, SIGNAL(clicked()), this, SLOT(slotReinitializeColors()));
 
 
 //this->Ui->SpheresRadioButton->setChecked(true);
  // Should connect...
  

}




//-----------------------------------------------------------------------------
mqSavePLYDialog::~mqSavePLYDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSavePLYDialog::slotPLYFileTypeChanged()
{
	

}

void mqSavePLYDialog::slotPLYPositionChanged()
{


}
void mqSavePLYDialog::slotPLYNormalsChanged()
{


}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

