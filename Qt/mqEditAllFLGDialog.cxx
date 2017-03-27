/*=========================================================================

   Program: MeshTools
   Module:    $RCS $

 
=========================================================================*/

#include "mqEditAllFLGDialog.h"
#include "ui_mqEditAllFLGDialog.h"
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
mqEditAllFLGDialog::mqEditAllFLGDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqEditAllFLGDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqEditAllFLGDialog");			
	double flag_rendering_size = mqMeshToolsCore::instance()->Getmui_FlagRenderingSize();
	this->Ui->FlagRenderingSizeValue->setMinimum(0);
	this->Ui->FlagRenderingSizeValue->setSingleStep(1);
	this->Ui->FlagRenderingSizeValue->setValue(flag_rendering_size);

	QColor myFlagColor;

	double flagcolor[4];

	this->Ui->FlagColorButton->setShowAlphaChannel(false);
	mqMeshToolsCore::instance()->Getmui_FlagColor(flagcolor);

	myFlagColor.setRedF(flagcolor[0]);
	myFlagColor.setGreenF(flagcolor[1]);
	myFlagColor.setBlueF(flagcolor[2]);


	this->Ui->FlagColorButton->setChosenColor(myFlagColor);

  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotsaveAllFLG()));

}




//-----------------------------------------------------------------------------
mqEditAllFLGDialog::~mqEditAllFLGDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqEditAllFLGDialog::saveAllFLG()
{
	cout << "Save All FLG!" << endl;
	
		QColor myFlagColor = this->Ui->FlagColorButton->chosenColor();
		double flagcolor[4];
		myFlagColor.getRgbF(&flagcolor[0], &flagcolor[1], &flagcolor[2], &flagcolor[3]);
		
		double flg_rendering_size = this->Ui->FlagRenderingSizeValue->value();
		
		mqMeshToolsCore::instance()->UpdateAllSelectedFlags(flagcolor, flg_rendering_size);// to update body size!
	
	
}







void mqEditAllFLGDialog::slotsaveAllFLG()
{
	this->saveAllFLG();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

