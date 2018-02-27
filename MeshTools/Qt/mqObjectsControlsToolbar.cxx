/*=========================================================================

   Program: MeshTools
   Module:    Copied from Paraview pqMainControlsToolbar.cxx

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

========================================================================*/
#include "mqObjectsControlsToolbar.h"
#include "ui_mqObjectsControlsToolbar.h"

// For later!
#include "QReleaseSlider.h"
#include "QReleaseSliderValue.h"

#include "mqSaveNTWDialogReaction.h"
#include "mqUndoRedoReaction.h"
#include "mqEditLMKDialogReaction.h"
#include "mqCreateLMKDialogReaction.h"
#include "mqEditFLGDialogReaction.h"
#include "mqEditACTORDialogReaction.h"
#include "mqMeshToolsCore.h"
#include "mqOpenDataReaction.h"
#include "mqCameraReaction.h"
#include "mqDisplayReaction.h"
#include <vtkRenderer.h>

#include <QToolButton>
#include <QHBoxLayout>

//-----------------------------------------------------------------------------
void mqObjectsControlsToolbar::constructor()
{
 // Ui::mqObjectsControlsToolbar ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqObjectsControlsToolbar;
  this->ui->setupUi(this);
  this->setStyleSheet(

	  "QSlider::groove:vertical{"
	  "border: 1px solid #999999;"
	  "width: 10px;" /* the groove expands to the size of the slider by default. by giving it a width, it has a fixed size */
	  "margin: 0px -4;"
	  "}"

	  "QSlider::handle:vertical{"
	  "background: qlineargradient(x1 : 0, y1 : 0, x2 : 1, y2 : 1, stop : 0 #b4b4b4, stop:1 #8f8f8f);"
	  "border: 1px solid #5c5c5c;"
	  "height: 10px;"
	  "margin: 0px -2;" /* handle is placed by default on the contents rect of the groove. Expand outside the groove */
	  "}"

	  "QSlider::add-page:vertical{"
	  "background:yellow;"
	  "margin: 1px;}"
	  
	  "QSlider::sub-page:vertical{"
	  "background:white;"
	 "margin: 1px;"
	  "}"


	  "QScrollBar{background-color:orange}"

	  "QScrollBar::handle:vertical{background:yellow;max-height:10px;} "

  );
  this->zTr = new QReleaseSlider;
  this->zTr->setMaximum(100);
  this->zTr->setMinimum(-100);
  this->zTr->setToolTip(QString("Translate along z viewing axis"));


  this->zRot = new QReleaseSliderValue(Qt::Vertical, tr(""));
  this->zRot->setMaximum(90);
  this->zRot->setMinimum(-90);
  this->zRot->setToolTip(QString("Rotation along z viewing axis"));


  this->yTr = new QReleaseSlider;
  this->yTr->setMaximum(100);
  this->yTr->setMinimum(-100);
  this->yTr->setToolTip(QString("Translate along y viewing axis"));

  this->yRot = new QReleaseSliderValue(Qt::Vertical, tr(""));
  this->yRot->setMaximum(90);
  this->yRot->setMinimum(-90);
  this->yRot->setToolTip(QString("Rotation along y viewing axis"));

  
  this->xTr = new QReleaseSlider;
  this->xTr->setMaximum(100);
  this->xTr->setMinimum(-100);
  this->xTr->setToolTip(QString("Translate along x viewing axis"));

  this->xRot = new QReleaseSliderValue(Qt::Vertical, tr(""));
  this->xRot->setMaximum(90);
  this->xRot->setMinimum(-90);
  this->xRot->setToolTip(QString("Rotation along x viewing axis"));

  QHBoxLayout *zlayout = new QHBoxLayout;
  QWidget* zgrid = new QWidget();
  zlayout->addWidget(this->zRot);
  zlayout->addWidget(this->zTr);
  zlayout->setSpacing(1);
  zlayout->setMargin(1);
  zgrid->setLayout(zlayout);
  this->addWidget(zgrid);

  QHBoxLayout *ylayout = new QHBoxLayout;
  QWidget* ygrid = new QWidget();
  ylayout->addWidget(this->yRot);
  ylayout->addWidget(this->yTr);
  ylayout->setSpacing(1);
  ylayout->setMargin(1);
  ygrid->setLayout(ylayout);
  this->addWidget(ygrid);

  QHBoxLayout *xlayout = new QHBoxLayout;
  QWidget* xgrid = new QWidget();
  xlayout->addWidget(this->xRot);
  xlayout->addWidget(this->xTr);
  xlayout->setSpacing(1);
  xlayout->setMargin(1);
  xgrid->setLayout(xlayout);
  this->addWidget(xgrid);

  connect(this->ui->actionDelete, SIGNAL(triggered()), this, SLOT(slotDeleteObjects()));
  
 
  
}


void mqObjectsControlsToolbar::slotDeleteObjects()
{
	mqMeshToolsCore::instance()->DeleteSelectedActors();
	mqMeshToolsCore::instance()->Render();


	
	
	
}
