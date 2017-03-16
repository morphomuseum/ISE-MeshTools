/*=========================================================================

   Program: MeshTools
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSaveFlagsDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveFlagsDialog.h"
#include "mqReaction.h"
#include "mqMeshToolsCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveFlagsDialogReaction::mqSaveFlagsDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{

}

void mqSaveFlagsDialogReaction::onTriggered()
{
	
	vtkIdType num_Flags = 0;
	
		num_Flags = mqMeshToolsCore::instance()->getFlagLandmarkCollection()->GetNumberOfItems();
	
	
		if (num_Flags == 0) {
			QMessageBox msgBox;
			msgBox.setText("No Flag exist...");
			msgBox.exec();
			return;
		}
		
		cout << "Save Flag Dialog Triggered!" << endl;


		

		/*if (QFile::exists(fileName))
		{
		QMessageBox msgBox;
		msgBox.setText("Overwrite?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		int ret = msgBox.exec();
		if (ret == QMessageBox::No) { return; }



		}*/

		mqSaveFlagsDialogReaction::showSaveFlagsDialog();
		
		
		
}

//-----------------------------------------------------------------------------
void mqSaveFlagsDialogReaction::showSaveFlagsDialog()
{
	// fonction statique. C'est � dire que lorsqu'on clique sur SAVE Flag, on cr�e un nouvel objet.
	// la r�action, elle, est bien instanci�e � la cr�ation du menu, mais pas la fen�tre. 

  mqSaveFlagsDialog SaveFlag_dialog(mqCoreUtilities::mainWidget());
  SaveFlag_dialog.exec();
}
