/*=========================================================================

   Program: MeshTools
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSavePLYDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSavePLYDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqSavePLYDialogReaction::mqSavePLYDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
}

//-----------------------------------------------------------------------------
void mqSavePLYDialogReaction::showSavePLYDialog()
{
	// fonction statique. C'est � dire que lorsqu'on clique sur SAVE PLY, on cr�e un nouvel objet.
	// la r�action, elle, est bien instanci�e � la cr�ation du menu, mais pas la fen�tre. 

  mqSavePLYDialog SavePLY_dialog(mqCoreUtilities::mainWidget());
  SavePLY_dialog.exec();
}
