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
  Ui::mqMainControlsToolbar ui;
  ui.setupUi(this);
  new mqOpenDataReaction(ui.actionOpenData, 0);//0= open data (generic)
  new mqCameraReaction(ui.actionCameraFront, 0); //0 = camera Front
  new mqCameraReaction(ui.actionCameraBack, 1); //1 = camera Back
  new mqCameraReaction(ui.actionCameraLeft, 2); //2 = camera Left
  new mqCameraReaction(ui.actionCameraRight, 3); //3 = camera Right
  new mqCameraReaction(ui.actionCameraAbove, 4); //4 = camera Above
  new mqCameraReaction(ui.actionCameraBelow, 5); //5 = camera Below

  //new mqSaveDataReaction(ui.actionSaveData);
 
  new mqUndoRedoReaction(ui.actionUndo, true);
  new mqUndoRedoReaction(ui.actionRedo, false);

  if (mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
  {

	  ui.actionCameraCentreOfMassToggle->setChecked(true);
  }

  if (mqMeshToolsCore::instance()->Getmui_CameraOrtho() == 0)
  {

	  ui.actionCameraOrthoPerspectiveToggle->setChecked(true);
  }

  if (mqMeshToolsCore::instance()->Getmui_Anaglyph() == 1)
  {

	  ui.actionRendererAnaglyphToggle->setChecked(true);
  }

  if (mqMeshToolsCore::instance()->Getmui_ShowGrid() == 1)
  {

	  ui.actionGridToggle->setChecked(true);
  }
  if (mqMeshToolsCore::instance()->Getmui_ShowOrientationHelper() == 1)
  {

	  ui.actionOrientationHelperToggle->setChecked(true);
  }
  new mqCameraReaction(ui.actionCameraCentreOfMassToggle, 6); //6 = camera COM toggle
  new mqCameraReaction(ui.actionCameraOrthoPerspectiveToggle, 7); //7 = camera OrthoPerspective toggle

  new mqDisplayReaction(ui.actionGridToggle, 0); //0 = display Grid Toggle
  new mqDisplayReaction(ui.actionOrientationHelperToggle, 1); //1 = display Orientation Helper Toggle
  new mqDisplayReaction(ui.actionRendererAnaglyphToggle, 2); //2 = display Anaglyph mode Toggle
}
