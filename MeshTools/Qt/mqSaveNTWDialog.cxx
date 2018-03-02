/*=========================================================================

   Program: MeshTools
   Module:    $RCS $

 
=========================================================================*/

#include "mqSaveNTWDialog.h"
#include "ui_mqSaveNTWDialog.h"
#include "MeshToolsVersion.h"
#include "mqMeshToolsCore.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QHeaderView>


#include <sstream>

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT "64"
#else
#define ENVIRONMENT "32"
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT "64"
#else
#define ENVIRONMENT "32"
#endif
#endif

//-----------------------------------------------------------------------------
mqSaveNTWDialog::mqSaveNTWDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqSaveNTWDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqSaveNTWDialog");
	
	
	// This is where we 
  //
 
 this->Ui->DoNotSaveORI->setChecked(true);
 this->Ui->DoNotSaveTAG->setChecked(true);
 this->Ui->VTP->setChecked(true);
 this->Ui->PositionInPosFile->setChecked(true);
  // Should connect...
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSaveNTWFile()));

}




//-----------------------------------------------------------------------------
mqSaveNTWDialog::~mqSaveNTWDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSaveNTWDialog::slotSaveNTWFile()
{
	cout << "NTW File Saved!" << endl;
	
	

	QString fileName;
	
	/*std::string filename;
	int exists = 0;
	for (int i = 0; i<g_distinct_selected_names.size(); i++)
	{
		filename = path.c_str();
		filename.append(g_distinct_selected_names.at(i).c_str());*/

	cout << "SAVE NTW: number of distinct selected surfaces:" << mqMeshToolsCore::instance()->g_distinct_selected_names.size() << endl;
	if (mqMeshToolsCore::instance()->getActorCollection()->GetNumberOfSelectedActors() == 1)
	{
		mqMeshToolsCore::instance()->ComputeSelectedNamesLists();
		//cout << "preferred name:" << mqMeshToolsCore::instance()->g_distinct_selected_names.at(0).c_str();
		fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
			tr("Save NTW files"), mqMeshToolsCore::instance()->Getmui_LastUsedDir()+ QDir::separator()+ mqMeshToolsCore::instance()->g_distinct_selected_names.at(0).c_str(),
			tr("NTW file (*.ntw)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);
	}

	else
	{
		
		fileName = QFileDialog::getSaveFileName(mqMeshToolsCore::instance()->GetMainWindow(),
			tr("Save NTW files"), mqMeshToolsCore::instance()->Getmui_LastUsedDir(),
			tr("NTW file (*.ntw)"),NULL
			//, QFileDialog::DontConfirmOverwrite
		);
	}
	
	
	
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMeshToolsCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	int save_ori = 0; //0 no= save all landmark, 1 yes, save only selected landmarks
	int save_tag = 0;
	int save_surfaces_as_ply = 0;
	int apply_position_to_surfaces = 0;


	if (this->Ui->SaveORI->isChecked()) { save_ori = 1; }
	if (this->Ui->SaveTAG->isChecked()) { save_tag = 1; }
	if (this->Ui->PLY->isChecked()) { save_surfaces_as_ply = 1; }
	if (this->Ui->ApplyPositionToSurfaces->isChecked()) { apply_position_to_surfaces = 1; }

	

	mqMeshToolsCore::instance()->SaveNTWFile(fileName, save_ori, save_tag, save_surfaces_as_ply, apply_position_to_surfaces);

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

