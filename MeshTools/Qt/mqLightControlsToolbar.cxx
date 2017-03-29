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
#include "mqLightControlsToolbar.h"
#include "ui_mqLightControlsToolbar.h"

// For later!
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

#include <QToolButton>


//-----------------------------------------------------------------------------
void mqLightControlsToolbar::constructor()
{
 // Ui::mqLightControlsToolbar ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqLightControlsToolbar;
  this->ui->setupUi(this);
 
  connect(this->ui->actionFrontLight, SIGNAL(triggered()), this, SLOT(slotFrontLight()));
  connect(this->ui->actionBackLight, SIGNAL(triggered()), this, SLOT(slotBackLight()));
  connect(this->ui->actionAboveLight, SIGNAL(triggered()), this, SLOT(slotAboveLight()));
  connect(this->ui->actionBelowLight, SIGNAL(triggered()), this, SLOT(slotBelowLight()));
  connect(this->ui->actionLeftLight, SIGNAL(triggered()), this, SLOT(slotLeftLight()));
  connect(this->ui->actionRightLight, SIGNAL(triggered()), this, SLOT(slotRightLight()));
 
  
}


void mqLightControlsToolbar::slotFrontLight()
{

	
	/*this->ui->actionLandmarksModeTarget->setChecked(false);
	this->ui->actionLandmarksModeNode->setChecked(false);
	this->ui->actionLandmarksModeHandle->setChecked(false);
	this->ui->actionLandmarksModeFlag->setChecked(false);
	mqMeshToolsCore::instance()->Setmui_LandmarkMode(0);*/
	
}
void mqLightControlsToolbar::slotAboveLight()
{


	/*this->ui->actionLandmarksModeTarget->setChecked(false);
	this->ui->actionLandmarksModeNode->setChecked(false);
	this->ui->actionLandmarksModeHandle->setChecked(false);
	this->ui->actionLandmarksModeFlag->setChecked(false);
	mqMeshToolsCore::instance()->Setmui_LandmarkMode(0);*/

}
void mqLightControlsToolbar::slotBelowLight()
{


	/*this->ui->actionLandmarksModeTarget->setChecked(false);
	this->ui->actionLandmarksModeNode->setChecked(false);
	this->ui->actionLandmarksModeHandle->setChecked(false);
	this->ui->actionLandmarksModeFlag->setChecked(false);
	mqMeshToolsCore::instance()->Setmui_LandmarkMode(0);*/

}
void mqLightControlsToolbar::slotBackLight()
{


	/*this->ui->actionLandmarksModeTarget->setChecked(false);
	this->ui->actionLandmarksModeNode->setChecked(false);
	this->ui->actionLandmarksModeHandle->setChecked(false);
	this->ui->actionLandmarksModeFlag->setChecked(false);
	mqMeshToolsCore::instance()->Setmui_LandmarkMode(0);*/

}
void mqLightControlsToolbar::slotLeftLight()
{


	/*this->ui->actionLandmarksModeTarget->setChecked(false);
	this->ui->actionLandmarksModeNode->setChecked(false);
	this->ui->actionLandmarksModeHandle->setChecked(false);
	this->ui->actionLandmarksModeFlag->setChecked(false);
	mqMeshToolsCore::instance()->Setmui_LandmarkMode(0);*/

}
void mqLightControlsToolbar::slotRightLight()
{


	/*this->ui->actionLandmarksModeTarget->setChecked(false);
	this->ui->actionLandmarksModeNode->setChecked(false);
	this->ui->actionLandmarksModeHandle->setChecked(false);
	this->ui->actionLandmarksModeFlag->setChecked(false);
	mqMeshToolsCore::instance()->Setmui_LandmarkMode(0);*/

}