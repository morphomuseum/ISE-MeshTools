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
	// fonction statique. C'est à dire que lorsqu'on clique sur "Color", on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqEditLMKDialog color_dialog(mqCoreUtilities::mainWidget());
  color_dialog.exec();
}
