/*=========================================================================

   Program: MeshTools
   Module:    $RCS $

 
=========================================================================*/

#include "mqFillHolesDialog.h"
#include "ui_mqFillHolesDialog.h"
#include "MeshToolsVersion.h"
#include "mqMeshToolsCore.h"
#include "mqUndoStack.h"
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
mqFillHolesDialog::mqFillHolesDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqFillHolesDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqFillHolesDialog");	
	
	
  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotFillHoles()));

}




//-----------------------------------------------------------------------------
mqFillHolesDialog::~mqFillHolesDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqFillHolesDialog::FillHoles()
{
	cout << "FillHoles dialog" << endl;
	
	if (mqMeshToolsCore::instance()->getActorCollection()->GetNumberOfSelectedActors() > 0)
	{
		std::string action = "FillHoles selected actors";
		
		mqMeshToolsCore::instance()->addFillHoles(this->Ui->size->value());
		
	}
}







void mqFillHolesDialog::slotFillHoles()
{
	this->FillHoles();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

