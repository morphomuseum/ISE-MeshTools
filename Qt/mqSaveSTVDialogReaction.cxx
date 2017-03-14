/*=========================================================================

   Program: MeshTools
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSaveSTVDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveSTVDialog.h"
#include "mqReaction.h"
#include "mqMeshToolsCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveSTVDialogReaction::mqSaveSTVDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{

}

void mqSaveSTVDialogReaction::onTriggered()
{
	vtkIdType num_CURNorm = 0;
	vtkIdType num_CURT = 0;
	vtkIdType num_CURNode = 0;
	vtkIdType num_CURH = 0;
	
		num_CURNorm = mqMeshToolsCore::instance()->getNormalLandmarkCollection()->GetNumberOfItems();
		num_CURH = mqMeshToolsCore::instance()->getTargetLandmarkCollection()->GetNumberOfItems();
		num_CURNode = mqMeshToolsCore::instance()->getNodeLandmarkCollection()->GetNumberOfItems();
		num_CURH = mqMeshToolsCore::instance()->getHandleLandmarkCollection()->GetNumberOfItems();
	
	
		if (num_CURNorm==0&& num_CURT&&num_CURNode&&num_CURH == 0) {
			QMessageBox msgBox;
			msgBox.setText("No landmark nor curve element  exist...");
			msgBox.exec();
			return;
		}
		
		cout << "Save STV Dialog Triggered!" << endl;


		

		/*if (QFile::exists(fileName))
		{
		QMessageBox msgBox;
		msgBox.setText("Overwrite?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		int ret = msgBox.exec();
		if (ret == QMessageBox::No) { return; }



		}*/

		mqSaveSTVDialogReaction::showSaveSTVDialog();
		
		/*QString fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
			tr("Save STV files"), QDir::currentPath(),
			tr("STV file (*.lmk *.ver)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);


		cout << fileName.toStdString();
		if (fileName.isEmpty()) return;*/
		
}

//-----------------------------------------------------------------------------
void mqSaveSTVDialogReaction::showSaveSTVDialog()
{
	// fonction statique. C'est à dire que lorsqu'on clique sur SAVE STV, on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqSaveSTVDialog SaveSTV_dialog(mqCoreUtilities::mainWidget());
  SaveSTV_dialog.exec();
}
