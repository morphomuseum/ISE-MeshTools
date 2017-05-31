/*=========================================================================

   Program: MeshTools
  

========================================================================*/
#include "mqCameraControlsToolbar.h"
#include "ui_mqCameraControlsToolbar.h"

// For later!

#include "mqUndoRedoReaction.h"
#include "mqMeshToolsCore.h"
#include "mqDisplayReaction.h"

#include <vtkRenderer.h>
#include "QDoubleSlider.h"
#include <QToolButton>
#include <QComboBox>
#include <QSlider>


//-----------------------------------------------------------------------------
void mqCameraControlsToolbar::constructor()
{
 // Ui::mqCameraControlsToolbar ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqCameraControlsToolbar;
  this->ui->setupUi(this);
  this->cP = new QDoubleSlider;
  this->zoom = new QDoubleSlider;
  this->zRot = new QDoubleSlider;
  
  
  
  // Add values in the combo box
  this->addWidget(this->zRot);
   this->addWidget(this->cP);
  this->addWidget(this->zoom);
 
  //this->ui->actionTagEdit->setDisabled(true);
  //this->ui->actionClippingPlaneOnOff->setDisabled(true);
  //connect(mqMeshToolsCore::instance(), SIGNAL(clippingPlaneChanged(int)), this, SLOT(slotClippingPlaneChanged(int)));

  connect(this->ui->actionBackfaceCullingOnOff, SIGNAL(triggered()), this, SLOT(slotBackfaceCullingOnOff()));
  connect(this->ui->actionClippingPlaneOnOff, SIGNAL(triggered()), this, SLOT(slotClippingPlaneOnOff()));
 
  
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
	

	




