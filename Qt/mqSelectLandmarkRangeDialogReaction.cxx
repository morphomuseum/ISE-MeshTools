/*=========================================================================

   Program: MeshTools
   Module:    mqSelectLandmarkRange.cxx


========================================================================*/
#include "mqSelectLandmarkRangeDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSelectLandmarkRangeDialog.h"
#include "mqReaction.h"
#include "mqMeshToolsCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSelectLandmarkRangeDialogReaction::mqSelectLandmarkRangeDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	
}

void mqSelectLandmarkRangeDialogReaction::onTriggered()
{
	
	

		mqSelectLandmarkRangeDialogReaction::showSelectLandmarkRangeDialog();
		
		
		
}

//-----------------------------------------------------------------------------
void mqSelectLandmarkRangeDialogReaction::showSelectLandmarkRangeDialog()
{
	// fonction statique. C'est � dire que lorsqu'on clique sur SAVE Landmark, on cr�e un nouvel objet.
	// la r�action, elle, est bien instanci�e � la cr�ation du menu, mais pas la fen�tre. 

  mqSelectLandmarkRangeDialog SaveLandmark_dialog(mqCoreUtilities::mainWidget());
  SaveLandmark_dialog.exec();
}
