/*=========================================================================

   Program: MeshTools
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqLandmarkDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqLandmarkDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqLandmarkDialogReaction::mqLandmarkDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
}

//-----------------------------------------------------------------------------
void mqLandmarkDialogReaction::showLandmarkDialog()
{
	// fonction statique. C'est � dire que lorsqu'on clique sur "Landmark", on cr�e un nouvel objet.
	// la r�action, elle, est bien instanci�e � la cr�ation du menu, mais pas la fen�tre. 

  mqLandmarkDialog landmark_dialog(mqCoreUtilities::mainWidget());
  landmark_dialog.exec();
}
