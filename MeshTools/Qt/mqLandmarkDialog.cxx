/*=========================================================================

   Program: MeshTools
   Module:    $RCS $

 
=========================================================================*/

#include "mqLandmarkDialog.h"
#include "ui_mqLandmarkDialog.h"
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
mqLandmarkDialog::mqLandmarkDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqLandmarkDialog())
{
  this->Ui->setupUi(this);
  this->setObjectName("mqLandmarkDialog");
  
  QColor myFlagColor;
  
  double flagcolor[4];
  
  this->Ui->FlagColorButton->setShowAlphaChannel(false);
  mqMeshToolsCore::instance()->Getmui_FlagColor(flagcolor);
  
  myFlagColor.setRedF(flagcolor[0]);
  myFlagColor.setGreenF(flagcolor[1]);
  myFlagColor.setBlueF(flagcolor[2]);
  

  this->Ui->FlagColorButton->setChosenColor(myFlagColor);
  connect(this->Ui->FlagColorButton, SIGNAL(colorChosen()), this, SLOT(slotFlagColorChanged()));
 

  connect(this->Ui->reinitcolorsButton, SIGNAL(clicked()), this, SLOT(slotReinitializeColors()));
 
  int lmk_body_type = mqMeshToolsCore::instance()->Getmui_LandmarkBodyType();
  if (lmk_body_type == 0) { this->Ui->SpheresRadioButton->setChecked(true); 
  this->Ui->ArrowsRadioButton->setChecked(false);
  }
  else
  {
	  this->Ui->SpheresRadioButton->setChecked(false);
	  this->Ui->ArrowsRadioButton->setChecked(true);
  }
 
  int adj_lmk_size = mqMeshToolsCore::instance()->Getmui_AdjustLandmarkRenderingSize();
  cout << "adj_lmk_size" << adj_lmk_size << endl;
  if (adj_lmk_size == 1) {
	  this->Ui->AdjustLandmarkRenderingSizeCheckBox->setChecked(true);
	  this->Ui->LandmarkRenderingSizeValue->setDisabled(true);
	  this->Ui->AdjustScaleFactor->setEnabled(true);
  }
  else
  {
	  this->Ui->LandmarkRenderingSizeValue->setEnabled(true);
	  this->Ui->AdjustScaleFactor->setDisabled(true);
  }
 
   this->Ui->AdjustScaleFactor->setMinimum(0.01);
  this->Ui->AdjustScaleFactor->setMaximum(10);
  double scale_factor = mqMeshToolsCore::instance()->Getmui_AdjustScaleFactor();
  cout << "mt scale factor=" << scale_factor << endl;
  this->Ui->AdjustScaleFactor->setValue(scale_factor);

  double landmark_rendering_size = mqMeshToolsCore::instance()->Getmui_LandmarkRenderingSize();
  //cout << "LRS=" << landmark_rendering_size<< endl;
  this->Ui->LandmarkRenderingSizeValue->setMinimum(0);
  //this->Ui->LandmarkRenderingSizeValue->setMaximum(10000);
  this->Ui->LandmarkRenderingSizeValue->setSingleStep(1);
  this->Ui->LandmarkRenderingSizeValue->setValue(landmark_rendering_size);
  double flag_rendering_size = mqMeshToolsCore::instance()->Getmui_FlagRenderingSize();
  this->Ui->FlagRenderingSizeValue->setMinimum(0);
  this->Ui->FlagRenderingSizeValue->setSingleStep(1);
  this->Ui->FlagRenderingSizeValue->setValue(flag_rendering_size);
  
  this->Ui->SizeUnit1->setText(mqMeshToolsCore::instance()->Getmui_SizeUnit());
  this->Ui->SizeUnit2->setText(mqMeshToolsCore::instance()->Getmui_SizeUnit());

  connect(this->Ui->SpheresRadioButton, SIGNAL(clicked()), this, SLOT(slotLandmarkBodyTypeChanged()));
  connect(this->Ui->ArrowsRadioButton, SIGNAL(clicked()), this, SLOT(slotLandmarkBodyTypeChanged()));
  connect(this->Ui->AdjustLandmarkRenderingSizeCheckBox, SIGNAL(clicked()), this, SLOT(slotAdjustLandmarkRenderingSizeChanged()));
  connect(this->Ui->LandmarkRenderingSizeValue, SIGNAL(valueChanged(double)), this, SLOT(slotLandmarkRenderingSizeChanged()));
  connect(this->Ui->FlagRenderingSizeValue, SIGNAL(valueChanged(double)), this, SLOT(slotFlagRenderingSizeChanged()));
  connect(this->Ui->AdjustScaleFactor, SIGNAL(valueChanged(double)), this, SLOT(slotAdjustScaleFactorChanged()));

}




//-----------------------------------------------------------------------------
mqLandmarkDialog::~mqLandmarkDialog()
{
	QColor myFlagColor = this->Ui->FlagColorButton->chosenColor();
	double flagcolor[3];
	myFlagColor.getRgbF(&flagcolor[0], &flagcolor[1], &flagcolor[2]);

	

	mqMeshToolsCore::instance()->Setmui_FlagColor(flagcolor);
	
	
  delete this->Ui;
}
void mqLandmarkDialog::slotAdjustScaleFactorChanged()
{
	double scale_factor = 1;
	scale_factor = this->Ui->AdjustScaleFactor->value();
	mqMeshToolsCore::instance()->Setmui_AdjustScaleFactor(scale_factor);

}
void mqLandmarkDialog::slotAdjustLandmarkRenderingSizeChanged()
{
	int adj_lmk_size=0;
	if (this->Ui->AdjustLandmarkRenderingSizeCheckBox->isChecked())
	{
		adj_lmk_size = 1;
	}
	if (adj_lmk_size == 1) {		
		this->Ui->LandmarkRenderingSizeValue->setDisabled(true);
		this->Ui->AdjustScaleFactor->setEnabled(true);
	}
	else
	{
		this->Ui->LandmarkRenderingSizeValue->setEnabled(true);
		this->Ui->AdjustScaleFactor->setDisabled(true);
	}


	cout << "new adjust lmk rdr size:" << adj_lmk_size << endl;
	mqMeshToolsCore::instance()->Setmui_AdjustLandmarkRenderingSize(adj_lmk_size);
}

void mqLandmarkDialog::slotLandmarkRenderingSizeChanged()
{
	double lmk_rendering_size= this->Ui->LandmarkRenderingSizeValue->value();
	cout << "new lmk rdr size:" << lmk_rendering_size << endl;
	mqMeshToolsCore::instance()->Setmui_LandmarkRenderingSize(lmk_rendering_size);

}
void mqLandmarkDialog::slotFlagRenderingSizeChanged()
{
	double flg_rendering_size = this->Ui->FlagRenderingSizeValue->value();
	cout << "new flg rdr size:" << flg_rendering_size << endl;
	mqMeshToolsCore::instance()->Setmui_FlagRenderingSize(flg_rendering_size);
}
void mqLandmarkDialog::slotLandmarkBodyTypeChanged()
{
	int lmk_body_type = 0;
	if (this->Ui->ArrowsRadioButton->isChecked())
	{
		lmk_body_type = 1;
	}
	cout << "new lmk body type:" << lmk_body_type << endl;
	mqMeshToolsCore::instance()->Setmui_LandmarkBodyType(lmk_body_type);
}

void mqLandmarkDialog::slotFlagColorChanged()
{
	QColor myFlagColor = this->Ui->FlagColorButton->chosenColor();
	double flagcolor[4];
	myFlagColor.getRgbF(&flagcolor[0], &flagcolor[1], &flagcolor[2], &flagcolor[3]);
		
	mqMeshToolsCore::instance()->Setmui_FlagColor(flagcolor);	
	mqMeshToolsCore::instance()->Render();
	cout << "Flag color changed!" << endl;
}

void mqLandmarkDialog::slotReinitializeColors()
{
	QColor myDefaultFlagColor;
	double defaultFlagColor[3];
	int defaultAdjustLandmarkRenderingSize = mqMeshToolsCore::instance()->Getmui_DefaultAdjustLandmarkRenderingSize();
	int defaultLandmarkBodyType = mqMeshToolsCore::instance()->Getmui_DefaultLandmarkBodyType();
	double defaultLandmarkRenderingSize = mqMeshToolsCore::instance()->Getmui_DefaultLandmarkRenderingSize();
	double defaultFlagRenderingSize = mqMeshToolsCore::instance()->Getmui_DefaultFlagRenderingSize();

	mqMeshToolsCore::instance()->Setmui_AdjustLandmarkRenderingSize(defaultAdjustLandmarkRenderingSize);
	mqMeshToolsCore::instance()->Setmui_LandmarkBodyType(defaultLandmarkBodyType);
	mqMeshToolsCore::instance()->Setmui_LandmarkRenderingSize(defaultLandmarkRenderingSize);
	mqMeshToolsCore::instance()->Setmui_FlagRenderingSize(defaultFlagRenderingSize);
	
	if (defaultLandmarkBodyType == 0) {
		this->Ui->SpheresRadioButton->setChecked(true);
		this->Ui->ArrowsRadioButton->setChecked(false);
	}
	else
	{
		this->Ui->SpheresRadioButton->setChecked(false);
		this->Ui->ArrowsRadioButton->setChecked(true);
	}
	if (defaultAdjustLandmarkRenderingSize == 1) 
	{
		this->Ui->AdjustLandmarkRenderingSizeCheckBox->setChecked(true);
		this->Ui->LandmarkRenderingSizeValue->setDisabled(true);
	}
	else
	{
		this->Ui->LandmarkRenderingSizeValue->setEnabled(true);
	}	
	this->Ui->LandmarkRenderingSizeValue->setValue(defaultLandmarkRenderingSize);	
	this->Ui->FlagRenderingSizeValue->setValue(defaultFlagRenderingSize);



	mqMeshToolsCore::instance()->Getmui_DefaultFlagColor(defaultFlagColor);
	mqMeshToolsCore::instance()->Setmui_FlagColor(defaultFlagColor);
	

	myDefaultFlagColor.setRedF(defaultFlagColor[0]);
	myDefaultFlagColor.setGreenF(defaultFlagColor[1]);
	myDefaultFlagColor.setBlueF(defaultFlagColor[2]);
	


	this->Ui->FlagColorButton->setChosenColor(myDefaultFlagColor);	
	
	mqMeshToolsCore::instance()->Render();
	
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

