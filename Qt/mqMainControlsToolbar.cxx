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
#include "mqMainControlsToolbar.h"
#include "ui_mqMainControlsToolbar.h"

// For later!

#include "mqUndoRedoReaction.h"
#include "mqMeshToolsCore.h"
#include "mqOpenDataReaction.h"
#include "mqCameraReaction.h"
#include "mqDisplayReaction.h"

#include <QToolButton>


//-----------------------------------------------------------------------------
void mqMainControlsToolbar::constructor()
{
 // Ui::mqMainControlsToolbar ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqMainControlsToolbar;
  this->ui->setupUi(this);
  new mqOpenDataReaction(this->ui->actionOpenData, 0);//0= open data (generic)
  new mqCameraReaction(this->ui->actionCameraFront, 0); //0 = camera Front
  new mqCameraReaction(this->ui->actionCameraBack, 1); //1 = camera Back
  new mqCameraReaction(this->ui->actionCameraLeft, 2); //2 = camera Left
  new mqCameraReaction(this->ui->actionCameraRight, 3); //3 = camera Right
  new mqCameraReaction(this->ui->actionCameraAbove, 4); //4 = camera Above
  new mqCameraReaction(this->ui->actionCameraBelow, 5); //5 = camera Below

  //new mqSaveDataReaction(this->ui->actionSaveData);
 
  new mqUndoRedoReaction(this->ui->actionUndo, true);
  new mqUndoRedoReaction(this->ui->actionRedo, false);

  if (mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
  {

	  this->ui->actionCameraCentreOfMassToggle->setChecked(true);
  }

  if (mqMeshToolsCore::instance()->Getmui_CameraOrtho() == 0)
  {

	  this->ui->actionCameraOrthoPerspectiveToggle->setChecked(true);
  }

  if (mqMeshToolsCore::instance()->Getmui_Anaglyph() == 1)
  {

	  this->ui->actionRendererAnaglyphToggle->setChecked(true);
  }

  if (mqMeshToolsCore::instance()->Getmui_ShowGrid() == 1)
  {

	  this->ui->actionGridToggle->setChecked(true);
  }
  if (mqMeshToolsCore::instance()->Getmui_ShowOrientationHelper() == 1)
  {

	  this->ui->actionOrientationHelperToggle->setChecked(true);
  }
  new mqCameraReaction(this->ui->actionCameraCentreOfMassToggle, 6); //6 = camera COM toggle
  new mqCameraReaction(this->ui->actionCameraOrthoPerspectiveToggle, 7); //7 = camera OrthoPerspective toggle

  new mqDisplayReaction(this->ui->actionGridToggle, 0); //0 = display Grid Toggle
  new mqDisplayReaction(this->ui->actionOrientationHelperToggle, 1); //1 = display Orientation Helper Toggle
  new mqDisplayReaction(this->ui->actionRendererAnaglyphToggle, 2); //2 = display Anaglyph mode Toggle

  int landmark_mode = mqMeshToolsCore::instance()->Getmui_LandmarkMode();
  if (landmark_mode == 0) { this->ui->actionLandmarksModeNormal->setChecked(true); }
  else if (landmark_mode == 1) { this->ui->actionLandmarksModeTarget->setChecked(true); }
  else if (landmark_mode == 2) { this->ui->actionLandmarksModeNode->setChecked(true); }
  else if (landmark_mode == 3) { this->ui->actionLandmarksModeHandle->setChecked(true); }
  else { this->ui->actionLandmarksModeFlag->setChecked(true); }//4

  connect(this->ui->actionLandmarksModeNormal, SIGNAL(triggered()), this, SLOT(slotLandmarkNormalMode()));
  connect(this->ui->actionLandmarksModeTarget, SIGNAL(triggered()), this, SLOT(slotLandmarkTargetMode()));
  connect(this->ui->actionLandmarksModeHandle, SIGNAL(triggered()), this, SLOT(slotLandmarkHandleMode()));
  connect(this->ui->actionLandmarksModeNode, SIGNAL(triggered()), this, SLOT(slotLandmarkNodeMode()));
  connect(this->ui->actionLandmarksModeFlag, SIGNAL(triggered()), this, SLOT(slotFlagMode()));
}

void mqMainControlsToolbar::slotLandmarkNormalMode()
{
	cout << "Landmark setting mode: 0" << endl;
	
	this->ui->actionLandmarksModeTarget->setChecked(false);
	this->ui->actionLandmarksModeNode->setChecked(false);
	this->ui->actionLandmarksModeHandle->setChecked(false);
	this->ui->actionLandmarksModeFlag->setChecked(false);
	mqMeshToolsCore::instance()->Setmui_LandmarkMode(0);
	
}
void mqMainControlsToolbar::slotLandmarkTargetMode()
{
	cout << "Landmark setting mode: 1" << endl;
	this->ui->actionLandmarksModeNormal->setChecked(false);
	this->ui->actionLandmarksModeNode->setChecked(false);
	this->ui->actionLandmarksModeHandle->setChecked(false);
	this->ui->actionLandmarksModeFlag->setChecked(false);
	mqMeshToolsCore::instance()->Setmui_LandmarkMode(1);

}
void mqMainControlsToolbar::slotLandmarkNodeMode()
{
	cout << "Landmark setting mode: 2" << endl;
	this->ui->actionLandmarksModeNormal->setChecked(false);
	this->ui->actionLandmarksModeTarget->setChecked(false);
	this->ui->actionLandmarksModeHandle->setChecked(false);
	this->ui->actionLandmarksModeFlag->setChecked(false);
	mqMeshToolsCore::instance()->Setmui_LandmarkMode(2);
}
void mqMainControlsToolbar::slotLandmarkHandleMode()
{
	cout << "Landmark setting mode: 3" << endl;
	this->ui->actionLandmarksModeNormal->setChecked(false);
	this->ui->actionLandmarksModeTarget->setChecked(false);
	this->ui->actionLandmarksModeNode->setChecked(false);
	this->ui->actionLandmarksModeFlag->setChecked(false);
	mqMeshToolsCore::instance()->Setmui_LandmarkMode(3);
	

}
void mqMainControlsToolbar::slotFlagMode()
{
	cout << "Landmark setting mode: 4=flags" << endl;
	this->ui->actionLandmarksModeNormal->setChecked(false);
	this->ui->actionLandmarksModeTarget->setChecked(false);
	this->ui->actionLandmarksModeNode->setChecked(false);
	this->ui->actionLandmarksModeHandle->setChecked(false);
	
	mqMeshToolsCore::instance()->Setmui_LandmarkMode(4);


}
