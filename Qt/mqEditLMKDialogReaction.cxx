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
	this->color_dialog = new mqEditLMKDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqEditLMKDialogReaction::showEditLMKDialog(mqEditLMKDialog *color_dialog)
{
	// fonction statique. C'est � dire que lorsqu'on clique sur "Color", on cr�e un nouvel objet.
	// la r�action, elle, est bien instanci�e � la cr�ation du menu, mais pas la fen�tre. 

  //mqEditLMKDialog color_dialog = new mqEditLMKDialog(mqCoreUtilities::mainWidget());
  color_dialog->show();
}
