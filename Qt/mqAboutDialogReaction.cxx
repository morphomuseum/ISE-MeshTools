/*=========================================================================

   Program: MeshTools
   Module:    mqAboutDialogReaction.cxx


========================================================================*/
#include "mqAboutDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqAboutDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqAboutDialogReaction::mqAboutDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
}

//-----------------------------------------------------------------------------
void mqAboutDialogReaction::showAboutDialog()
{
  mqAboutDialog about_dialog(mqCoreUtilities::mainWidget());
  about_dialog.exec();
}
