/*=========================================================================

   Program: MeshTools
   Module:    $RCS $

 
=========================================================================*/

#include "mqEditFLGDialog.h"
#include "ui_mqEditFLGDialog.h"
#include "MeshToolsVersion.h"
#include "mqMeshToolsCore.h"
#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QHeaderView>


#include <sstream>

#define NORMAL_NODE 0
#define STARTING_NODE 1
#define MILESTONE_NODE 2
#define CONNECT_NODE 3


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
mqEditFLGDialog::mqEditFLGDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqEditFLGDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqEditFLGDialog");
	connect(mqMeshToolsCore::instance(), SIGNAL(lmSelectionChanged()), this, SLOT(slotRefreshDialog()));
	connect(this->Ui->next, SIGNAL(pressed()), this, SLOT(slotGetNextFlag()));
	connect(this->Ui->prec, SIGNAL(pressed()), this, SLOT(slotGetPrecedingFlag()));
	
	this->FLG_Coll = NULL;
	this->FLG = NULL;
	current_coll = -1;
	this->Ui->x->setDecimals(10);
	this->Ui->y->setDecimals(10);
	this->Ui->z->setDecimals(10);
	this->Ui->x->setMinimum(-DBL_MAX);
	this->Ui->y->setMinimum(-DBL_MAX);
	this->Ui->z->setMinimum(-DBL_MAX);

	this->Ui->x->setMaximum(DBL_MAX);
	this->Ui->y->setMaximum(DBL_MAX);
	this->Ui->z->setMaximum(DBL_MAX);
	

	this->GetFirstSelectedFlag();
	this->UpdateUI();
	
 
  
 //connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotEditFLG()));

}




//-----------------------------------------------------------------------------
mqEditFLGDialog::~mqEditFLGDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqEditFLGDialog::saveFLG()
{
	cout << "Save FLG!" << endl;
	if (this->FLG != NULL)
	{
		this->FLG->SetLMOrigin(this->Ui->x->value(), this->Ui->y->value(), this->Ui->z->value());
		this->FLG->Modified();
	}
	
}



void mqEditFLGDialog::GetFirstSelectedFlag()
{
	this->FLG_Coll = NULL;
	this->FLG = NULL;
	this->current_coll = -1;

	this->FLG_Coll = mqMeshToolsCore::instance()->getNodeLandmarkCollection();
	int num_selected = 0;
	num_selected = this->FLG_Coll->GetNumberOfSelectedActors();
	if (num_selected > 0) {
		this->FLG = this->FLG_Coll->GetFirstSelectedActor();
		current_coll = 0;
	}
	else
	{
		this->FLG_Coll = mqMeshToolsCore::instance()->getTargetLandmarkCollection();
		num_selected = this->FLG_Coll->GetNumberOfSelectedActors();
		if (num_selected > 0) {
			this->FLG = this->FLG_Coll->GetFirstSelectedActor();
			current_coll = 1;
		}
		else
		{
			this->FLG_Coll = mqMeshToolsCore::instance()->getNodeLandmarkCollection();
			num_selected = this->FLG_Coll->GetNumberOfSelectedActors();
			if (num_selected > 0) {
				this->FLG = this->FLG_Coll->GetFirstSelectedActor();
				current_coll = 2;
			}
			else
			{
				this->FLG_Coll = mqMeshToolsCore::instance()->getHandleLandmarkCollection();
				num_selected = this->FLG_Coll->GetNumberOfSelectedActors();
				if (num_selected > 0) {
					this->FLG = this->FLG_Coll->GetFirstSelectedActor();
					current_coll = 3;
				}
			}

		}
	}

	if (this->FLG != NULL)
	{
	
		this->FLG_Coll->Modified();
	}

}

void mqEditFLGDialog::GetFirstFlag()
{
	this->FLG_Coll = mqMeshToolsCore::instance()->getNodeLandmarkCollection();
	int num = 0;
	num = this->FLG_Coll->GetNumberOfItems();
	if (num > 0) {
		this->FLG_Coll->InitTraversal();
		this->FLG = vtkLMActor::SafeDownCast(this->FLG_Coll->GetNextActor());
		current_coll = 0;
	}
	else
	{
		this->FLG_Coll = mqMeshToolsCore::instance()->getTargetLandmarkCollection();
		num = 0;
		num = this->FLG_Coll->GetNumberOfItems();
		if (num > 0) {
			this->FLG_Coll->InitTraversal();
			this->FLG = vtkLMActor::SafeDownCast(this->FLG_Coll->GetNextActor());
			current_coll = 1;
		}
		else
		{
			this->FLG_Coll = mqMeshToolsCore::instance()->getNodeLandmarkCollection();
			num = 0;
			num = this->FLG_Coll->GetNumberOfItems();
			if (num > 0) {
				this->FLG_Coll->InitTraversal();
				this->FLG = vtkLMActor::SafeDownCast(this->FLG_Coll->GetNextActor());
				current_coll = 2;
			}
			else
			{
				this->FLG_Coll = mqMeshToolsCore::instance()->getHandleLandmarkCollection();
				num = 0;
				num = this->FLG_Coll->GetNumberOfItems();
				if (num > 0) {
					this->FLG_Coll->InitTraversal();
					this->FLG = vtkLMActor::SafeDownCast(this->FLG_Coll->GetNextActor());
					current_coll = 3;
				}
			}

		}
	}

	if (this->FLG != NULL)
	{
		this->FLG->SetSelected(1);
		this->FLG_Coll->Modified();
	}

}

void mqEditFLGDialog::UpdateUI()
{
	if (this->FLG != NULL) {
		this->Ui->FLGNumber->setValue(this->FLG->GetLMNumber());
		double orig[3];
		this->FLG->GetLMOrigin(orig);
		cout << "orig[0]" << orig[0] << ", orig[1]" << orig[1] << ", orig[2]" << orig[2] << endl;
		this->Ui->x->setValue(orig[0]);
		this->Ui->x->update();
		this->Ui->y->setValue(orig[1]);
		this->Ui->y->update();
		this->Ui->z->setValue(orig[2]);
		this->Ui->z->update();
	}

}


void mqEditFLGDialog::GetNextFlag()
{

	if (FLG == NULL)
	{
		this->GetFirstFlag();
	}
	else
	{
		this->FLG->SetSelected(0);
		this->FLG_Coll->Modified();
		int num = this->FLG->GetLMNumber();
		int cpt = 0;
		while (cpt < 4)
		{
			cpt++;
			this->FLG = this->FLG_Coll->GetLandmarkAfter(num);
			if (this->FLG != NULL) { this->FLG->SetSelected(1); this->FLG_Coll->Modified(); break; }
			else
			{

				num = 0;
				if (this->current_coll == 0 )
				{
					this->FLG_Coll = mqMeshToolsCore::instance()->getTargetLandmarkCollection();
					this->current_coll = 1;
				}
				else if (this->current_coll == 1)
				{
					this->FLG_Coll = mqMeshToolsCore::instance()->getNodeLandmarkCollection();
					this->current_coll = 2;
				}
				else if (this->current_coll == 2)
				{
					this->FLG_Coll = mqMeshToolsCore::instance()->getHandleLandmarkCollection();
					this->current_coll = 3;
				}
				else if (this->current_coll == 3)
				{
					this->FLG_Coll = mqMeshToolsCore::instance()->getNormalLandmarkCollection();
					this->current_coll = 0;
				}

			}

		}


	}

}
void mqEditFLGDialog::GetPrecedingFlag()
{

	if (this->FLG == NULL)
	{
		this->GetFirstFlag();
	}
	else
	{
		this->FLG->SetSelected(0);
		this->FLG_Coll->Modified();
		int num = this->FLG->GetLMNumber();
		cout << "num=" << num << endl;
		int cpt = 0;
		while (cpt < 4)
		{
			cpt++;
			this->FLG = this->FLG_Coll->GetLandmarkBefore(num);
			this->FLG_Coll->Modified();
			if (this->FLG != NULL) { this->FLG->SetSelected(1); break; }
			else
			{

				
				if (this->current_coll == 0)
				{
					this->FLG_Coll = mqMeshToolsCore::instance()->getHandleLandmarkCollection();
					num = this->FLG_Coll->GetNumberOfItems()+1;
					this->current_coll = 3;
				}
				else if (this->current_coll == 1)
				{
					this->FLG_Coll = mqMeshToolsCore::instance()->getNormalLandmarkCollection();
					num = this->FLG_Coll->GetNumberOfItems() + 1;
					this->current_coll = 0;
				}
				else if (this->current_coll == 2)
				{
					this->FLG_Coll = mqMeshToolsCore::instance()->getTargetLandmarkCollection();
					num = this->FLG_Coll->GetNumberOfItems() + 1;
					
					this->current_coll = 1;
				}
				else if (this->current_coll == 3)
				{
					this->FLG_Coll = mqMeshToolsCore::instance()->getNodeLandmarkCollection();
					num = this->FLG_Coll->GetNumberOfItems() + 1;
					this->current_coll = 2;
				}

			}

		}


	}

}

void mqEditFLGDialog::slotGetPrecedingFlag()
{
	this->saveFLG();
	this->GetPrecedingFlag();
	this->UpdateUI();
	mqMeshToolsCore::instance()->Render();
}
void mqEditFLGDialog::slotGetNextFlag()
{
	
	this->saveFLG();
	this->GetNextFlag();
	this->UpdateUI();
	mqMeshToolsCore::instance()->Render();
}

void mqEditFLGDialog::slotRefreshDialog()
{
	cout << "Refresh FLG Dialog!" << endl;
	this->GetFirstSelectedFlag();
	this->UpdateUI();
	mqMeshToolsCore::instance()->Render();

}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

