/*=========================================================================

   Program: MeshTools
   Module:    $RCS $

 
=========================================================================*/

#include "mqEditLMKDialog.h"
#include "ui_mqEditLMKDialog.h"
#include "MeshToolsVersion.h"
#include "mqMeshToolsCore.h"
#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QHeaderView>


#include <sstream>

#define NORMAL_NODE 0
#define STARTING_NODE 1
#define MILESTONE_NODE 2
#define CONNECT_NODE 3


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
mqEditLMKDialog::mqEditLMKDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqEditLMKDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqEditLMKDialog");
	
	
	// This is where we 
  //
 

 
  
 //connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotEditLMK()));

}




//-----------------------------------------------------------------------------
mqEditLMKDialog::~mqEditLMKDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqEditLMKDialog::slotEditLMK()
{
	cout << "Edit LMK!" << endl;
	
	
	
	
	

	

	

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

