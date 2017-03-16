/*=========================================================================

   Program: MeshTools
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSaveNTWDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveNTWDialog.h"
#include "mqReaction.h"
#include "mqMeshToolsCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveNTWDialogReaction::mqSaveNTWDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{

}

void mqSaveNTWDialogReaction::onTriggered()
{
	vtkIdType num_sel_Actors = 0;
	vtkIdType num_sel_Fags= 0;
	vtkIdType num_sel_Normal_Landmarks = 0;
	vtkIdType num_sel_Target_Landmarks = 0;
	vtkIdType num_sel_Node_Landmarks = 0;
	vtkIdType num_sel_Handle_Landmarks = 0;
	
	vtkIdType num_Actors = 0;
	vtkIdType num_Fags = 0;
	vtkIdType num_Normal_Landmarks = 0;
	vtkIdType num_Target_Landmarks = 0;
	vtkIdType num_Node_Landmarks = 0;
	vtkIdType num_Handle_Landmarks = 0;


	num_sel_Actors = mqMeshToolsCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	num_sel_Fags = mqMeshToolsCore::instance()->getFlagLandmarkCollection()->GetNumberOfSelectedActors();
	num_sel_Normal_Landmarks = mqMeshToolsCore::instance()->getNormalLandmarkCollection()->GetNumberOfSelectedActors();
	num_sel_Target_Landmarks = mqMeshToolsCore::instance()->getTargetLandmarkCollection()->GetNumberOfSelectedActors();
	num_sel_Node_Landmarks = mqMeshToolsCore::instance()->getNodeLandmarkCollection()->GetNumberOfSelectedActors();
	num_sel_Handle_Landmarks = mqMeshToolsCore::instance()->getHandleLandmarkCollection()->GetNumberOfSelectedActors();
	
	num_Actors = mqMeshToolsCore::instance()->getActorCollection()->GetNumberOfItems();
	num_Fags = mqMeshToolsCore::instance()->getFlagLandmarkCollection()->GetNumberOfItems();
	num_Normal_Landmarks = mqMeshToolsCore::instance()->getNormalLandmarkCollection()->GetNumberOfItems();
	num_Target_Landmarks = mqMeshToolsCore::instance()->getTargetLandmarkCollection()->GetNumberOfItems();
	num_Node_Landmarks = mqMeshToolsCore::instance()->getNodeLandmarkCollection()->GetNumberOfItems();
	num_Handle_Landmarks = mqMeshToolsCore::instance()->getHandleLandmarkCollection()->GetNumberOfItems();

	
		if (num_sel_Actors ==0
			&& num_sel_Fags==0
			&& num_sel_Normal_Landmarks==0
			&& num_sel_Target_Landmarks ==0
			&& num_sel_Node_Landmarks == 0
			&& num_sel_Handle_Landmarks==0
			
			) {
			QMessageBox msgBox;
			msgBox.setText("Select at least one surface/flag/landmark/curve element to use this function!");
			msgBox.exec();
			return;
		}
		
		if (num_sel_Actors != num_Actors
			|| num_sel_Fags != num_Fags 
			|| num_sel_Normal_Landmarks != num_Normal_Landmarks
			|| num_sel_Target_Landmarks != num_Target_Landmarks
			|| num_sel_Node_Landmarks != num_Node_Landmarks
			|| num_sel_Handle_Landmarks != num_Handle_Landmarks
			

			) {
			QMessageBox msgBox;
			msgBox.setText("Some elements are unselected and will not be saved. Continue ?");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			msgBox.setDefaultButton(QMessageBox::No);
			int ret = msgBox.exec();
			if (ret == QMessageBox::No) { return; }

			
		}

		cout << "Save NTW Dialog Triggered!" << endl;


		

		

		mqSaveNTWDialogReaction::showSaveNTWDialog();
		
		
		
}

//-----------------------------------------------------------------------------
void mqSaveNTWDialogReaction::showSaveNTWDialog()
{
	// fonction statique. C'est � dire que lorsqu'on clique sur SAVE NTW, on cr�e un nouvel objet.
	// la r�action, elle, est bien instanci�e � la cr�ation du menu, mais pas la fen�tre. 

  mqSaveNTWDialog SaveNTW_dialog(mqCoreUtilities::mainWidget());
  SaveNTW_dialog.exec();
}
