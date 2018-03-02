/*=========================================================================

   Program: MeshTools
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSaveSTLDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveSTLDialog.h"
#include "mqReaction.h"
#include "mqMeshToolsCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveSTLDialogReaction::mqSaveSTLDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
}

void mqSaveSTLDialogReaction::onTriggered()
{
	
		vtkIdType num_selected_meshes = mqMeshToolsCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
		if (num_selected_meshes == 0) {
			QMessageBox msgBox;
			msgBox.setText("No surface selected. Please select at least one surface to use this option.");
			msgBox.exec();
			return;
		}
		else if (num_selected_meshes > 1)
		{
			QMessageBox msgBox;
			msgBox.setText("More than one surface are currently selected. Do you want to save all selected surfaces into on single surface file?");
			msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Save);
			int ret = msgBox.exec();
			if (ret == QMessageBox::Cancel) { return; }

		}
		cout << "Save STL Dialog Triggered!" << endl;
		QString fileName;
		if (num_selected_meshes == 1)
		{
			mqMeshToolsCore::instance()->ComputeSelectedNamesLists();

			fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
				tr("Save STL files"), mqMeshToolsCore::instance()->Getmui_LastUsedDir() + QDir::separator() + mqMeshToolsCore::instance()->g_distinct_selected_names.at(0).c_str(),
				tr("STL file (*.stl)"), NULL
				//, QFileDialog::DontConfirmOverwrite
			);
		}
		else
		{
			fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
				tr("Save STL files"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
				tr("STL file (*.stl)"), NULL
				//, QFileDialog::DontConfirmOverwrite
			);

		}

		cout << fileName.toStdString();
		if (fileName.isEmpty()) return;
		QFileInfo fileInfo(fileName);
		mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());

		/*if (QFile::exists(fileName))
		{
		QMessageBox msgBox;
		msgBox.setText("Overwrite?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		int ret = msgBox.exec();
		if (ret == QMessageBox::No) { return; }



		}*/

		mqSaveSTLDialogReaction::showSaveSTLDialog(fileName);
}

//-----------------------------------------------------------------------------
void mqSaveSTLDialogReaction::showSaveSTLDialog(QString fileName)
{
	// fonction statique. C'est � dire que lorsqu'on clique sur SAVE STL, on cr�e un nouvel objet.
	// la r�action, elle, est bien instanci�e � la cr�ation du menu, mais pas la fen�tre. 

  mqSaveSTLDialog SaveSTL_dialog(mqCoreUtilities::mainWidget(), fileName);
  SaveSTL_dialog.exec();
}
