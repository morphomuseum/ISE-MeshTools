/*=========================================================================

   Program: MeshTools
   Module:    $RCS $

 
=========================================================================*/

#include "mqColorDialog.h"
#include "ui_mqColorDialog.h"
#include "MeshToolsVersion.h"
#include "vtkMeshToolsCore.h"


#include <GL/glew.h>
#include <QApplication>
#include <QFile>
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
mqColorDialog::mqColorDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqColorDialog())
{
  this->Ui->setupUi(this);
  this->setObjectName("mqColorDialog");
  QColor myColor;
  QColor myBGColor;
  QColor myBGColor2;
  double meshcolor[4];
  double backgroundcolor[3];
  double backgroundcolor2[3];
  this->Ui->meshColorButton->setShowAlphaChannel(true);
  vtkMeshToolsCore::instance()->Getmui_MeshColor(meshcolor);
  vtkMeshToolsCore::instance()->Getmui_BackGroundColor(backgroundcolor);
  vtkMeshToolsCore::instance()->Getmui_BackGroundColor2(backgroundcolor2);
  myColor.setRedF(meshcolor[0]);
  myColor.setGreenF(meshcolor[1]);
  myColor.setBlueF(meshcolor[2]);
  myColor.setAlphaF(meshcolor[3]);
  myBGColor.setRedF(backgroundcolor[0]);
  myBGColor.setGreenF(backgroundcolor[1]);
  myBGColor.setBlueF(backgroundcolor[2]);
  
  myBGColor2.setRedF(backgroundcolor2[0]);
  myBGColor2.setGreenF(backgroundcolor2[1]);
  myBGColor2.setBlueF(backgroundcolor2[2]);



  this->Ui->meshColorButton->setChosenColor(myColor);
  this->Ui->backgroundColorButton->setChosenColor(myBGColor);
  this->Ui->backgroundColorButton2->setChosenColor(myBGColor2);
  /*this->Ui->VersionLabel->setText(
	  QString("<html><b>Version: <i>%1</i></b></html>")
	  .arg(QString(MESHTOOLS_VERSION) + " " + QString(ENVIRONMENT) + "-bit"));*/
 
}




//-----------------------------------------------------------------------------
mqColorDialog::~mqColorDialog()
{
	QColor myColor = this->Ui->meshColorButton->chosenColor();
	double meshcolor[4];
	myColor.getRgbF(&meshcolor[0], &meshcolor[1], &meshcolor[2], &meshcolor[3]);

	QColor myBGColor = this->Ui->backgroundColorButton->chosenColor();
	double bgcolor[3];
	myBGColor.getRgbF(&bgcolor[0], &bgcolor[1], &bgcolor[2]);

	QColor myBGColor2 = this->Ui->backgroundColorButton2->chosenColor();
	double bgcolor2[3];
	myBGColor2.getRgbF(&bgcolor2[0], &bgcolor2[1], &bgcolor2[2]);

	//cout << "meshcolor[0]="<<meshcolor[0] << endl;
	//cout << "meshcolor[1]=" << meshcolor[1] << endl;
	//cout << "meshcolor[2]=" << meshcolor[2] << endl;
	//cout << "meshcolor[3]=" << meshcolor[3] << endl;
	vtkMeshToolsCore::instance()->Setmui_MeshColor(meshcolor);
	vtkMeshToolsCore::instance()->Setmui_BackGroundColor(bgcolor);
	vtkMeshToolsCore::instance()->Setmui_BackGroundColor2(bgcolor2);
	
  delete this->Ui;
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

