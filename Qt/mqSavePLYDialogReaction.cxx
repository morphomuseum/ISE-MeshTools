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
	// fonction statique. C'est à dire que lorsqu'on clique sur SAVE PLY, on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqSavePLYDialog SavePLY_dialog(mqCoreUtilities::mainWidget());
  SavePLY_dialog.exec();
}
