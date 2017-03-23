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
	// fonction statique. C'est à dire que lorsqu'on clique sur "Color", on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  //mqEditLMKDialog color_dialog = new mqEditLMKDialog(mqCoreUtilities::mainWidget());
  color_dialog->show();
}
