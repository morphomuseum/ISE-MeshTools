/*=========================================================================

   Program: MeshTools
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSaveCURDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveCURDialog.h"
#include "mqReaction.h"
#include "mqMeshToolsCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveCURDialogReaction::mqSaveCURDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{

}

void mqSaveCURDialogReaction::onTriggered()
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
			msgBox.setText("Number of curve nodes differs from number of curve handles... impossible to save .CUR file!");
			msgBox.exec();
			return;
		}
		cout << "Save CUR Dialog Triggered!" << endl;


		

		/*if (QFile::exists(fileName))
		{
		QMessageBox msgBox;
		msgBox.setText("Overwrite?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		int ret = msgBox.exec();
		if (ret == QMessageBox::No) { return; }



		}*/

		mqSaveCURDialogReaction::showSaveCURDialog();
		
		/*QString fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
			tr("Save CUR files"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
			tr("CUR file (*.lmk *.ver)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);


		cout << fileName.toStdString();
		if (fileName.isEmpty()) return;
		QFileInfo fileInfo(fileName);
	mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());

		*/
		
}

//-----------------------------------------------------------------------------
void mqSaveCURDialogReaction::showSaveCURDialog()
{
	// fonction statique. C'est � dire que lorsqu'on clique sur SAVE CUR, on cr�e un nouvel objet.
	// la r�action, elle, est bien instanci�e � la cr�ation du menu, mais pas la fen�tre. 

  mqSaveCURDialog SaveCUR_dialog(mqCoreUtilities::mainWidget());
  SaveCUR_dialog.exec();
}
