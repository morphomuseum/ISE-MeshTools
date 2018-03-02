/*=========================================================================

   Program: MeshTools
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSaveVTPDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveVTPDialog.h"
#include "mqReaction.h"
#include "mqMeshToolsCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveVTPDialogReaction::mqSaveVTPDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
}

void mqSaveVTPDialogReaction::onTriggered()
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
		cout << "Save VTP Dialog Triggered!" << endl;
		QString fileName;
		if (num_selected_meshes == 1)
		{
			mqMeshToolsCore::instance()->ComputeSelectedNamesLists();
			//+QDir::separator() + mqMeshToolsCore::instance()->g_distinct_selected_names.at(0).c_str(),
			 fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
				tr("Save VTK PolyData files"), mqMeshToolsCore::instance()->Getmui_LastUsedDir() + QDir::separator() + mqMeshToolsCore::instance()->g_distinct_selected_names.at(0).c_str(),
				tr("VTK PolyData file (*.vtk *.vtp)"), NULL
				//, QFileDialog::DontConfirmOverwrite
			);
		}
		else
		{
			 fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
				tr("Save VTK PolyData files"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
				tr("VTK PolyData file (*.vtk *.vtp)"), NULL
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

		mqSaveVTPDialogReaction::showSaveVTPDialog(fileName);
}

//-----------------------------------------------------------------------------
void mqSaveVTPDialogReaction::showSaveVTPDialog(QString fileName)
{
	// fonction statique. C'est à dire que lorsqu'on clique sur SAVE VTP, on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqSaveVTPDialog SaveVTP_dialog(mqCoreUtilities::mainWidget(), fileName);
  SaveVTP_dialog.exec();
}
