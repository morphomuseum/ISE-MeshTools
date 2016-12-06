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
	// fonction statique. C'est � dire que lorsqu'on clique sur "about", on cr�e un nouvel objet.
	// la r�action, elle, est bien instanci�e � la cr�ation du menu, mais pas la fen�tre. 

  mqAboutDialog about_dialog(mqCoreUtilities::mainWidget());
  about_dialog.exec();
}
