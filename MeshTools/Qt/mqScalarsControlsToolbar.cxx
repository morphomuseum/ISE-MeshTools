/*=========================================================================

   Program: MeshTools
   Module:    Copied from Paraview pqScalarsControlsToolbar.cxx

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
#include "mqScalarsControlsToolbar.h"
#include "ui_mqScalarsControlsToolbar.h"

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
#include <QComboBox>


//-----------------------------------------------------------------------------
void mqScalarsControlsToolbar::constructor()
{
 // Ui::mqScalarsControlsToolbar ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqScalarsControlsToolbar;
  this->ui->setupUi(this);
  this->comboActiveScalars = new QComboBox;
  //@@ to do : populate combo box according to found scalars
  // Create a list with all possible scalars.
  //this->comboActiveScalars->addItems({ "Initial RGB", "Tags", "Thickness", "Curvature"});
  this->comboActiveScalars->addItems({ "none" });
  // Add values in the combo box
  this->addWidget(this->comboActiveScalars);
  if (mqMeshToolsCore::instance()->Getmui_ScalarVisibility() == 1)
  {

	  this->ui->actionScalarVisibility->setChecked(true);
  }
  else
  {
	  this->comboActiveScalars->setDisabled(true);
  }
  connect(mqMeshToolsCore::instance(), SIGNAL(existingScalarsChanged()), this, SLOT(slotRefreshComboScalars()));

  connect(this->ui->actionScalarVisibility, SIGNAL(triggered()), this, SLOT(slotScalarVisitiliby()));
  connect(this->comboActiveScalars, SIGNAL(currentIndexChanged(int)), this, SLOT(slotActiveScalarChanged(int)));
  
}

void mqScalarsControlsToolbar::slotActiveScalarChanged(int idx)
{
	cout << "looks like active scalar has changed!:: "<<idx << endl;
}
void mqScalarsControlsToolbar::slotRefreshComboScalars()
{
	this->comboActiveScalars->clear();
	QStringList MyList = mqMeshToolsCore::instance()->Getmui_ExistingScalars();
	this->comboActiveScalars->addItems(MyList);
	
}
void mqScalarsControlsToolbar::slotScalarVisitiliby()
{

	if (this->ui->actionScalarVisibility->isChecked())
	{
		mqMeshToolsCore::instance()->Setmui_ScalarVisibility(1);
		this->comboActiveScalars->setDisabled(false);
	}
	else
	{
		mqMeshToolsCore::instance()->Setmui_ScalarVisibility(0);
		this->comboActiveScalars->setDisabled(true);
	}
	
	mqMeshToolsCore::instance()->Render();

	

}


