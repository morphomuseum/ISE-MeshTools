/*=========================================================================

   Program: MeshTools
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSaveCURasVERDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveCURasVERDialog.h"
#include "mqReaction.h"
#include "mqMeshToolsCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveCURasVERDialogReaction::mqSaveCURasVERDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{

}

void mqSaveCURasVERDialogReaction::onTriggered()
{
	
	vtkIdType num_CURN = 0;
	vtkIdType num_CURH = 0;
	
		num_CURN = mqMeshToolsCore::instance()->getNodeLandmarkCollection()->GetNumberOfItems();
		num_CURH = mqMeshToolsCore::instance()->getHandleLandmarkCollection()->GetNumberOfItems();
	
	
		if (num_CURN == 0) {
			QMessageBox msgBox;
			msgBox.setText("No Curve Node exist...");
			msgBox.exec();
			return;
		}
		if (num_CURH == 0) {
			QMessageBox msgBox;
			msgBox.setText("No Curve Handle exist...");
			msgBox.exec();
			return;
		}
		if (num_CURH != num_CURN) {
			QMessageBox msgBox;
			msgBox.setText("Number of curve nodes differs from number of curve handles... impossible to export curves as landmark file!");
			msgBox.exec();
			return;
		}
		cout << "Export CUR as VER Dialog Triggered!" << endl;


		
		mqSaveCURasVERDialogReaction::showSaveCURasVERDialog();
		
	
		
}

//-----------------------------------------------------------------------------
void mqSaveCURasVERDialogReaction::showSaveCURasVERDialog()
{
	// fonction statique. C'est � dire que lorsqu'on clique sur SAVE CUR, on cr�e un nouvel objet.
	// la r�action, elle, est bien instanci�e � la cr�ation du menu, mais pas la fen�tre. 

  mqSaveCURasVERDialog SaveCURasVER_dialog(mqCoreUtilities::mainWidget());
  SaveCURasVER_dialog.exec();
}
