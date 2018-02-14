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


//-----------------------------------------------------------------------------
void mqObjectsControlsToolbar::constructor()
{
 // Ui::mqObjectsControlsToolbar ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqObjectsControlsToolbar;
  this->ui->setupUi(this);
 
  connect(this->ui->actionDelete, SIGNAL(triggered()), this, SLOT(slotDeleteObjects()));
  
 
  
}


void mqObjectsControlsToolbar::slotDeleteObjects()
{
	mqMeshToolsCore::instance()->DeleteSelectedActors();
	mqMeshToolsCore::instance()->Render();


	
	
	
}
