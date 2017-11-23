/*=========================================================================

   Program: MeshTools
  

========================================================================*/
#include "mqCameraControlsToolbar.h"
#include "ui_mqCameraControlsToolbar.h"

// For later!

#include "mqUndoRedoReaction.h"
#include "mqMeshToolsCore.h"
#include "mqDisplayReaction.h"
#include "mqCameraReaction.h"
#include <vtkRenderer.h>
#include "QDoubleSlider.h"
#include "QReleaseSlider.h"
#include "QDoubleReleaseSlider.h"
#include "QReleaseSliderValue.h"
#include <QDial>
#include <QToolButton>
#include <QComboBox>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>


//-----------------------------------------------------------------------------
void mqCameraControlsToolbar::constructor()
{
	this->lastzoom = 0;
 // Ui::mqCameraControlsToolbar ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqCameraControlsToolbar;
  this->ui->setupUi(this);

  new mqCameraReaction(this->ui->actionCameraFront, 0); //0 = camera Front
  new mqCameraReaction(this->ui->actionCameraBack, 1); //1 = camera Back
  new mqCameraReaction(this->ui->actionCameraLeft, 2); //2 = camera Left
  new mqCameraReaction(this->ui->actionCameraRight, 3); //3 = camera Right
  new mqCameraReaction(this->ui->actionCameraAbove, 4); //4 = camera Above
  new mqCameraReaction(this->ui->actionCameraBelow, 5); //5 = camera Below

  if (mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
  {

	  this->ui->actionCameraCentreOfMassToggle->setChecked(true);
  }



  if (mqMeshToolsCore::instance()->Getmui_CameraOrtho() == 0)
  {

	  this->ui->actionCameraOrthoPerspectiveToggle->setChecked(true);
  }

  new mqCameraReaction(this->ui->actionCameraCentreOfMassToggle, 6); //6 = camera COM toggle
  new mqCameraReaction(this->ui->actionCameraOrthoPerspectiveToggle, 7); //7 = camera OrthoPerspective toggle



 /* */
  // this->cP = new QDoubleSlider;
  this->cP = new QReleaseSliderValue(Qt::Vertical, tr("")); 
  //this->cP->set
   this->cP->setToolTip(QString("Clipping plane"));
   this->cP->setMaximum(100);
   this->cP->setMinimum(-100);
  this->zoom = new QDoubleReleaseSlider;
  this->zoom->setToolTip(QString("Zoom"));
  
  this->zoom->setDoubleMinimum(-100);
  this->zoom->setDoubleMaximum(100);
  this->zoom->setDoubleValue(10);
  //this->zRot = new QReleaseSlider;
  this->zRot = new QDial;
  this->zRot->setFixedSize(30, 30);
  this->zRot->setObjectName("zRot");
 // this->setFixedWidth(200);
  //this->zRot->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"    );
  
  /*this->setStyleSheet("QDial#zRot{ background-color: blue }"
	  "QDial#zRot:pressed {"
					"background-color: rgb(224, 0, 0);"
					"border-style: inset;"
		"}"
	   );
	   */
  /*QSlider::groove:vertical{
  background: red;
  position: absolute; 
  left: 4px; right: 4px;
  }

	  QSlider::handle:vertical{
  height: 10px;
  background: green;
  margin: 0 - 4px; 
  }

	  QSlider::addpage:vertical{
  background: white;
  }

	  QSlider::sub - page : vertical{
  background: pink;
  }*/

  this->zRot->setMaximum(90);
  this->zRot->setMinimum(-90);

  this->zRot->setToolTip(QString("Rotate camera along viewing axis"));
  
  QHBoxLayout *layout = new QHBoxLayout;
  
  
  QWidget* grid = new QWidget();
 layout->addWidget(this->zRot);
 
 

  grid->setLayout(layout);
  this->addWidget(grid);
  // Add values in the combo box
  /*this->addWidget(this->zRot);
  this->addWidget(this->cP);
  this->addWidget(this->zoom);*/

 

  /*QHBoxLayout *layout2 = new QHBoxLayout;
  QWidget* grid2 = new QWidget();
  layout2->addWidget(this->cP);
  grid2->setLayout(layout2);
  this->addWidget(grid2);*/
  this->addWidget(this->cP);

  QWidget* spacer = new QWidget();
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  this->addWidget(spacer);

  QHBoxLayout *layout3 = new QHBoxLayout;
  QWidget* grid3 = new QWidget();
  layout3->addWidget(this->zoom);
  grid3->setLayout(layout3);
  this->addWidget(grid3);

  //this->setLayout(layout);
  //this->ui->actionTagEdit->setDisabled(true);
  //this->ui->actionClippingPlaneOnOff->setDisabled(true);
  //connect(mqMeshToolsCore::instance(), SIGNAL(clippingPlaneChanged(int)), this, SLOT(slotClippingPlaneChanged(int)));

  connect(this->ui->actionBackfaceCullingOnOff, SIGNAL(triggered()), this, SLOT(slotBackfaceCullingOnOff()));
  connect(this->ui->actionClippingPlaneOnOff, SIGNAL(triggered()), this, SLOT(slotClippingPlaneOnOff()));
 
  //connect(zRot, SIGNAL(valueChanged(int)), this, SLOT(slotZrot(int)));
  connect(zoom, SIGNAL(valueChanged(int)), this, SLOT(slotZoom()));
  
}

void mqCameraControlsToolbar::slotZoom()
{
	double val = this->zoom->doubleValue();
	if (val == 0) { this->lastzoom = 0;  return; }
	if (this->lastzoom>val)
	{
		mqMeshToolsCore::instance()->getCamera()->Zoom(1.1);
		this->lastzoom = val;
	}
	else if (this->lastzoom<val)
	{
		this->lastzoom = val;
		mqMeshToolsCore::instance()->getCamera()->Zoom(0.9);
	}
	mqMeshToolsCore::instance()->Render();
}

void mqCameraControlsToolbar::slotZrot(int val)
{
	if (val > 1)
	{
		mqMeshToolsCore::instance()->getCamera()->Zoom(1.1);
	}
	else
	{
		mqMeshToolsCore::instance()->getCamera()->Zoom(0.9);
	}
	mqMeshToolsCore::instance()->Render();
}

void mqCameraControlsToolbar::slotClippingPlaneOnOff()
{
	mqMeshToolsCore::instance()->getRenderer()->ResetCameraClippingRange();
	mqMeshToolsCore::instance()->ChangeClippingPlane();
	mqMeshToolsCore::instance()->Render();
	//mqMeshToolsCore::instance()->Render();
}

void mqCameraControlsToolbar::slotBackfaceCullingOnOff()
{
	
	mqMeshToolsCore::instance()->ChangeBackfaceCulling();
	mqMeshToolsCore::instance()->Render();
	//mqMeshToolsCore::instance()->Render();
}
	

	




