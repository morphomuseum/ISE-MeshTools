/*=========================================================================

   Program: MeshTools
   Module:    mqEditAllFLGDialogReaction.cxx


========================================================================*/
#include "mqEditAllFLGDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqEditAllFLGDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqEditAllFLGDialogReaction::mqEditAllFLGDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->FLG_dialog = new mqEditAllFLGDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqEditAllFLGDialogReaction::showEditAllFLGDialog(mqEditAllFLGDialog *FLG_dialog)
{
	
  
  FLG_dialog->show();
}
