/*=========================================================================

   Program: MeshTools
   Module:    mqEditLMKDialogReaction.cxx


========================================================================*/
#include "mqEditLMKDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqEditLMKDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqEditLMKDialogReaction::mqEditLMKDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
}

//-----------------------------------------------------------------------------
void mqEditLMKDialogReaction::showEditLMKDialog()
{
	// fonction statique. C'est � dire que lorsqu'on clique sur "Color", on cr�e un nouvel objet.
	// la r�action, elle, est bien instanci�e � la cr�ation du menu, mais pas la fen�tre. 

  mqEditLMKDialog color_dialog(mqCoreUtilities::mainWidget());
  color_dialog.exec();
}
