/*=========================================================================

   Program: MeshTools
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqColorDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqColorDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqColorDialogReaction::mqColorDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
}

//-----------------------------------------------------------------------------
void mqColorDialogReaction::showColorDialog()
{
	// fonction statique. C'est � dire que lorsqu'on clique sur "Color", on cr�e un nouvel objet.
	// la r�action, elle, est bien instanci�e � la cr�ation du menu, mais pas la fen�tre. 

  mqColorDialog color_dialog(mqCoreUtilities::mainWidget());
  color_dialog.exec();
}
