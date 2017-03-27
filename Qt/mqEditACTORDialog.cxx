/*=========================================================================

   Program: MeshTools
   Module:    $RCS $

 
=========================================================================*/

#include "mqEditACTORDialog.h"

#include "ui_mqEditACTORDialog.h"
#include "MeshToolsVersion.h"
#include "mqMeshToolsCore.h"
#include "vtkMTActor.h"
#include "vtkMTActorCollection.h"

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
mqEditACTORDialog::mqEditACTORDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqEditACTORDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqEditACTORDialog");
	connect(mqMeshToolsCore::instance(), SIGNAL(actorSelectionChanged()), this, SLOT(slotRefreshDialog()));
	connect(this->Ui->next, SIGNAL(pressed()), this, SLOT(slotGetNextActor()));
	connect(this->Ui->prec, SIGNAL(pressed()), this, SLOT(slotGetPrecedingActor()));
	
	this->ACTOR_Coll = NULL;
	this->ACTOR = NULL;

	QString mylabel("...");
	this->Ui->ActorName->setText(mylabel);
	
	
	QColor myActorColor;

	double Actorcolor[4];

	this->Ui->ActorColorButton->setShowAlphaChannel(false);


	mqMeshToolsCore::instance()->Getmui_MeshColor(Actorcolor);

	myActorColor.setRedF(Actorcolor[0]);
	myActorColor.setGreenF(Actorcolor[1]);
	myActorColor.setBlueF(Actorcolor[2]);

	this->Ui->ActorColorButton->setChosenColor(myActorColor);
	this->Ui->ActorAlpha->setValue(Actorcolor[3]);
	this->Ui->ActorAlpha->setMinimum(0);
	this->Ui->ActorAlpha->setMaximum(1);
	this->Ui->ActorAlpha->setSingleStep(0.01);
	this->Ui->ActorAlpha->setValue(Actorcolor[4]);

	

	this->GetFirstSelectedActor();
	this->UpdateUI();
	
 
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotsaveActor()));

}




//-----------------------------------------------------------------------------
mqEditACTORDialog::~mqEditACTORDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqEditACTORDialog::saveActor()
{
	//cout << "Save ACTOR!" << endl;
	if (this->ACTOR != NULL)
	{
		QColor myActorColor = this->Ui->ActorColorButton->chosenColor();
		double Actorcolor[4];
		myActorColor.getRgbF(&Actorcolor[0], &Actorcolor[1], &Actorcolor[2], &Actorcolor[3]);
		Actorcolor[3] = this->Ui->ActorAlpha->value();
		this->ACTOR->SetmColor(Actorcolor);
		this->ACTOR->SetName(this->Ui->ActorName->text().toStdString());
				
	
		this->ACTOR->Modified();
		
	}
	
}



void mqEditACTORDialog::GetFirstSelectedActor()
{
	this->ACTOR_Coll = NULL;
	this->ACTOR = NULL;


	this->ACTOR_Coll = mqMeshToolsCore::instance()->getActorCollection();
	int num_selected = 0;
	num_selected = this->ACTOR_Coll->GetNumberOfSelectedActors();
	if (num_selected > 0) {
		this->ACTOR = this->ACTOR_Coll->GetFirstSelectedActor();
		
	}
	

	if (this->ACTOR != NULL)
	{
	
		this->ACTOR_Coll->Modified();
	}

}

void mqEditACTORDialog::GetFirstActor()
{
	//cout << "Get first actor!" << endl;
	this->ACTOR_Coll = mqMeshToolsCore::instance()->getActorCollection();
	int num = 0;
	num = this->ACTOR_Coll->GetNumberOfItems();
	if (num > 0) {
		this->ACTOR_Coll->InitTraversal();
		this->ACTOR = vtkMTActor::SafeDownCast(this->ACTOR_Coll->GetNextActor());
		
	}
	

	if (this->ACTOR != NULL)
	{
		this->ACTOR->SetSelected(1);
		this->ACTOR_Coll->Modified();
	}

}

void mqEditACTORDialog::UpdateUI()
{
	if (this->ACTOR != NULL) {
		
		QString mylabel(this->ACTOR->GetName().c_str());
		this->Ui->ActorName->setText(mylabel);

	//	cout << "Update UI!" << endl;
		QColor myActorColor;
		double Actorcolor[4];

		//cout << "Name" <<mylabel.toStdString()<< endl;
		this->ACTOR->GetmColor(Actorcolor);

		myActorColor.setRedF(Actorcolor[0]);
		myActorColor.setGreenF(Actorcolor[1]);
		myActorColor.setBlueF(Actorcolor[2]);

		this->Ui->ActorColorButton->setChosenColor(myActorColor);

		this->Ui->ActorAlpha->setValue(Actorcolor[3]);
		//cout << "Color" << Actorcolor[0]<<","<< Actorcolor[1] << "," << Actorcolor[2] << "," << Actorcolor[3] << "." << endl;
	}

}


void mqEditACTORDialog::GetNextActor()
{

	if (ACTOR == NULL)
	{
		this->GetFirstActor();
	}
	else
	{
		this->ACTOR->SetSelected(0);
		this->ACTOR_Coll->Modified();
		int cpt = 0;
		
		this->ACTOR = this->ACTOR_Coll->GetActorAfter(this->ACTOR);
		if (this->ACTOR != NULL) { this->ACTOR->SetSelected(1); this->ACTOR_Coll->Modified(); }
		else
		{
			this->GetFirstActor();
				
		}

	}

}
void mqEditACTORDialog::GetPrecedingActor()
{

	if (this->ACTOR == NULL)
	{
		this->GetFirstActor();
	}
	else
	{
		this->ACTOR->SetSelected(0);
		this->ACTOR_Coll->Modified();
		
		this->ACTOR = this->ACTOR_Coll->GetActorBefore(this->ACTOR); 
		if (this->ACTOR != NULL) { this->ACTOR->SetSelected(1); }
		else
		{
			this->ACTOR= vtkMTActor::SafeDownCast(this->ACTOR_Coll->GetLastActor());
			this->ACTOR->SetSelected(1);
		}
	}

}
void mqEditACTORDialog::slotsaveActor()
{
	this->saveActor();
}
void mqEditACTORDialog::slotGetPrecedingActor()
{
	this->saveActor();
	this->GetPrecedingActor();
	this->UpdateUI();
	mqMeshToolsCore::instance()->Render();
}
void mqEditACTORDialog::slotGetNextActor()
{
	
	this->saveActor();
	this->GetNextActor();
	this->UpdateUI();
	mqMeshToolsCore::instance()->Render();
}

void mqEditACTORDialog::slotRefreshDialog()
{
	//cout << "Refresh ACTOR Dialog!" << endl;
	this->GetFirstSelectedActor();
	this->UpdateUI();
	mqMeshToolsCore::instance()->Render();

}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

