/*=========================================================================

   Program: MeshTools
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSaveLandmarksDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveLandmarksDialog.h"
#include "mqReaction.h"
#include "mqMeshToolsCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveLandmarksDialogReaction::mqSaveLandmarksDialogReaction(QAction* parentObject, int lmktype)
  : Superclass(parentObject)
{
	this->m_lmktype = lmktype;
}

void mqSaveLandmarksDialogReaction::onTriggered()
{
	
	vtkIdType num_landmarks = 0;
	if (this->m_lmktype==0)// Normal landmarks
	{
		num_landmarks = mqMeshToolsCore::instance()->getNormalLandmarkCollection()->GetNumberOfItems();
	}
	else if (this->m_lmktype == 1) // Target landmarks
	{
		num_landmarks = mqMeshToolsCore::instance()->getTargetLandmarkCollection()->GetNumberOfItems();

	}
	else if (this->m_lmktype == 2) // Target landmarks
	{
		num_landmarks = mqMeshToolsCore::instance()->getNodeLandmarkCollection()->GetNumberOfItems();
	}
	else // Handle landmarks
	{
		num_landmarks = mqMeshToolsCore::instance()->getHandleLandmarkCollection()->GetNumberOfItems();
	}
	
		if (num_landmarks == 0) {
			QMessageBox msgBox;
			msgBox.setText("No landmark of this type exist...");
			msgBox.exec();
			return;
		}
		
		cout << "Save Landmark Dialog Triggered!" << endl;


		

		/*if (QFile::exists(fileName))
		{
		QMessageBox msgBox;
		msgBox.setText("Overwrite?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		int ret = msgBox.exec();
		if (ret == QMessageBox::No) { return; }



		}*/

		mqSaveLandmarksDialogReaction::showSaveLandmarksDialog(this->m_lmktype);
		
		/*QString fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
			tr("Save Landmark files"), QDir::currentPath(),
			tr("Landmark file (*.lmk *.ver)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);


		cout << fileName.toStdString();
		if (fileName.isEmpty()) return;*/
		
}

//-----------------------------------------------------------------------------
void mqSaveLandmarksDialogReaction::showSaveLandmarksDialog(int lmktype)
{
	// fonction statique. C'est à dire que lorsqu'on clique sur SAVE Landmark, on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqSaveLandmarksDialog SaveLandmark_dialog(mqCoreUtilities::mainWidget(), lmktype);
  SaveLandmark_dialog.exec();
}
