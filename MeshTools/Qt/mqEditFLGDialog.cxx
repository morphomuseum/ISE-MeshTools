/*=========================================================================

   Program: MeshTools
   Module:    $RCS $

 
=========================================================================*/

#include "mqEditFLGDialog.h"
#include "ui_mqEditFLGDialog.h"
#include "MeshToolsVersion.h"
#include "mqMeshToolsCore.h"
#include "mqUndoStack.h"

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
	QString mylabel("...");
	this->Ui->FlagLabel->setText(mylabel);
	
	double flag_rendering_size = mqMeshToolsCore::instance()->Getmui_FlagRenderingSize();
	this->Ui->FlagRenderingSizeValue->setMinimum(0);
	this->Ui->FlagRenderingSizeValue->setSingleStep(1);
	this->Ui->FlagRenderingSizeValue->setValue(flag_rendering_size);

	QColor myFlagColor;

	double flagcolor[4];

	this->Ui->FlagColorButton->setShowAlphaChannel(false);
	mqMeshToolsCore::instance()->Getmui_FlagColor(flagcolor);

	myFlagColor.setRedF(flagcolor[0]);
	myFlagColor.setGreenF(flagcolor[1]);
	myFlagColor.setBlueF(flagcolor[2]);


	this->Ui->FlagColorButton->setChosenColor(myFlagColor);

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
	
 
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotsaveFLG()));

}




//-----------------------------------------------------------------------------
mqEditFLGDialog::~mqEditFLGDialog()
{

 //depending on what is 
	
  delete this->Ui;
}

int mqEditFLGDialog::SomeThingHasChanged()
{
	int something_has_changed = 0;
	if (this->FLG != NULL)
	{
		QColor muiFLGColor = this->Ui->FlagColorButton->chosenColor();
		double uiFLGcolor[4];
		muiFLGColor.getRgbF(&uiFLGcolor[0], &uiFLGcolor[1], &uiFLGcolor[2], &uiFLGcolor[3]);
		

		QColor myFLGColor;
		double FLGcolor[4];
		this->FLG->GetmColor(FLGcolor);

		if (
			FLGcolor[0] != uiFLGcolor[0]
			|| FLGcolor[1] != uiFLGcolor[1]
			|| FLGcolor[2] != uiFLGcolor[2]
			
			)
		{
			something_has_changed = 1;
		}
		double orig[3];
		this->FLG->GetLMOrigin(orig);

		if (
			orig[0] != this->Ui->x->value() ||
			orig[1] != this->Ui->y->value() ||
			orig[2] != this->Ui->z->value()
			)
		{
			something_has_changed = 1;
		}

		QString mylabel(this->FLG->GetLMText().c_str());
		
		
		if (QString::compare(mylabel, this->Ui->FlagLabel->text(), Qt::CaseInsensitive) != 0)
		{
			something_has_changed = 1;
		}

		double flag_rendering_size = this->FLG->GetLMSize();
		if (flag_rendering_size!= this->Ui->FlagRenderingSizeValue->value())
		{
			something_has_changed = 1;
		}
	}
	return something_has_changed;
}

void mqEditFLGDialog::saveFLG()
{
	cout << "Save FLG!" << endl;
	if (this->FLG != NULL)
	{
		int something_has_changed = this->SomeThingHasChanged();
		if (something_has_changed)
		{
			std::string action = "Update flag's color, origin, length and label";

			int mCount = BEGIN_UNDO_SET(action);
			this->FLG->SaveState(mCount);
			QColor myFlagColor = this->Ui->FlagColorButton->chosenColor();
			double flagcolor[4];
			myFlagColor.getRgbF(&flagcolor[0], &flagcolor[1], &flagcolor[2], &flagcolor[3]);
			this->FLG->SetmColor(flagcolor);
			this->FLG->SetLMText(this->Ui->FlagLabel->text().toStdString().c_str());

			double flg_rendering_size = this->Ui->FlagRenderingSizeValue->value();
			this->FLG->SetLMSize(flg_rendering_size);
			this->FLG->SetLMOrigin(this->Ui->x->value(), this->Ui->y->value(), this->Ui->z->value());
			this->FLG->Modified();
			mqMeshToolsCore::instance()->UpdateLandmarkSettings(this->FLG);// to update body size!
			END_UNDO_SET();
		}
	}
	
}



void mqEditFLGDialog::GetFirstSelectedFlag()
{
	this->FLG_Coll = NULL;
	this->FLG = NULL;
	this->current_coll = -1;

	this->FLG_Coll = mqMeshToolsCore::instance()->getFlagLandmarkCollection();
	int num_selected = 0;
	num_selected = this->FLG_Coll->GetNumberOfSelectedActors();
	if (num_selected > 0) {
		this->FLG = this->FLG_Coll->GetFirstSelectedActor();
		current_coll = 4;
	}
	

	if (this->FLG != NULL)
	{
	
		this->FLG_Coll->Modified();
	}

}

void mqEditFLGDialog::GetFirstFlag()
{
	this->FLG_Coll = mqMeshToolsCore::instance()->getFlagLandmarkCollection();
	int num = 0;
	num = this->FLG_Coll->GetNumberOfItems();
	if (num > 0) {
		this->FLG_Coll->InitTraversal();
		this->FLG = vtkLMActor::SafeDownCast(this->FLG_Coll->GetNextActor());
		current_coll = 0;
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
		
		QString mylabel(this->FLG->GetLMLabelText());
		this->Ui->FlagLabel->setText(mylabel);

		double flag_rendering_size = this->FLG->GetLMSize();
		this->Ui->FlagRenderingSizeValue->setValue(flag_rendering_size);

		QColor myFlagColor;
		double flagcolor[4];

		this->FLG->GetmColor(flagcolor);

		myFlagColor.setRedF(flagcolor[0]);
		myFlagColor.setGreenF(flagcolor[1]);
		myFlagColor.setBlueF(flagcolor[2]);
		this->Ui->FlagColorButton->setChosenColor(myFlagColor);

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
		
		this->FLG = this->FLG_Coll->GetLandmarkAfter(num);
		if (this->FLG != NULL) { this->FLG->SetSelected(1); this->FLG_Coll->Modified(); }
		else
		{
			num = 0;
			this->FLG = this->FLG_Coll->GetLandmarkAfter(num);
				
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
		this->FLG = this->FLG_Coll->GetLandmarkBefore(num);
		if (this->FLG != NULL) { this->FLG->SetSelected(1); }
		else
		{
			num = this->FLG_Coll->GetNumberOfItems()+1;
			this->FLG = this->FLG_Coll->GetLandmarkBefore(num);
		}
	}

}
void mqEditFLGDialog::slotsaveFLG()
{
	this->saveFLG();
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

