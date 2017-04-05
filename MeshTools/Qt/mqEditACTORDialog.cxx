/*=========================================================================

   Program: MeshTools
   Module:    $RCS $

 
=========================================================================*/

#include "mqEditACTORDialog.h"

#include "ui_mqEditACTORDialog.h"
#include "MeshToolsVersion.h"
#include "mqMeshToolsCore.h"
#include "mqUndoStack.h"
#include "vtkMTActor.h"
#include "vtkMTActorCollection.h"
#include <vtkMatrix4x4.h>

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
	/*this->Ui->M00->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M01->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M02->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M03->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M10->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M11->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M12->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M13->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M20->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M21->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M22->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M23->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M30->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M31->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M32->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M33->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));*/

	this->Ui->M00->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M01->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M02->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M03->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M10->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M11->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M12->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M13->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M20->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M21->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M22->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M23->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M30->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M31->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M32->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M33->setButtonSymbols(QAbstractSpinBox::NoButtons);

	this->Ui->M00->setMinimum(-DBL_MAX);
	this->Ui->M01->setMinimum(-DBL_MAX);
	this->Ui->M02->setMinimum(-DBL_MAX);
	this->Ui->M03->setMinimum(-DBL_MAX);
	this->Ui->M10->setMinimum(-DBL_MAX);
	this->Ui->M11->setMinimum(-DBL_MAX);
	this->Ui->M12->setMinimum(-DBL_MAX);
	this->Ui->M13->setMinimum(-DBL_MAX);
	this->Ui->M20->setMinimum(-DBL_MAX);
	this->Ui->M21->setMinimum(-DBL_MAX);
	this->Ui->M22->setMinimum(-DBL_MAX);
	this->Ui->M23->setMinimum(-DBL_MAX);
	this->Ui->M30->setMinimum(-DBL_MAX);
	this->Ui->M31->setMinimum(-DBL_MAX);
	this->Ui->M32->setMinimum(-DBL_MAX);
	this->Ui->M33->setMinimum(-DBL_MAX);

	this->Ui->M00->setMaximum(DBL_MAX);
	this->Ui->M01->setMaximum(DBL_MAX);
	this->Ui->M02->setMaximum(DBL_MAX);
	this->Ui->M03->setMaximum(DBL_MAX);
	this->Ui->M10->setMaximum(DBL_MAX);
	this->Ui->M11->setMaximum(DBL_MAX);
	this->Ui->M12->setMaximum(DBL_MAX);
	this->Ui->M13->setMaximum(DBL_MAX);
	this->Ui->M20->setMaximum(DBL_MAX);
	this->Ui->M21->setMaximum(DBL_MAX);
	this->Ui->M22->setMaximum(DBL_MAX);
	this->Ui->M23->setMaximum(DBL_MAX);
	this->Ui->M30->setMaximum(DBL_MAX);
	this->Ui->M31->setMaximum(DBL_MAX);
	this->Ui->M32->setMaximum(DBL_MAX);
	this->Ui->M33->setMaximum(DBL_MAX);

	this->Ui->M00->setDecimals(7);
	this->Ui->M01->setDecimals(7);
	this->Ui->M02->setDecimals(7);
	this->Ui->M03->setDecimals(7);
	this->Ui->M10->setDecimals(7);
	this->Ui->M11->setDecimals(7);
	this->Ui->M12->setDecimals(7);
	this->Ui->M13->setDecimals(7);
	this->Ui->M20->setDecimals(7);
	this->Ui->M21->setDecimals(7);
	this->Ui->M22->setDecimals(7);
	this->Ui->M23->setDecimals(7);
	this->Ui->M30->setDecimals(7);
	this->Ui->M31->setDecimals(7);
	this->Ui->M32->setDecimals(7);
	this->Ui->M33->setDecimals(7);

	
	this->Ui->M00->setValue(1);
	this->Ui->M01->setValue(0);
	this->Ui->M02->setValue(0);
	this->Ui->M03->setValue(0);
	this->Ui->M10->setValue(0);
	this->Ui->M11->setValue(1);
	this->Ui->M12->setValue(0);
	this->Ui->M13->setValue(0);
	this->Ui->M20->setValue(0);
	this->Ui->M21->setValue(0);
	this->Ui->M22->setValue(1);
	this->Ui->M23->setValue(0);
	this->Ui->M30->setValue(0);
	this->Ui->M31->setValue(0);
	this->Ui->M32->setValue(0);
	this->Ui->M33->setValue(1);
	this->UpdateUI();
	
 
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotsaveActor()));
 connect(this->Ui->Reinit, SIGNAL(pressed()), this, SLOT(slotReinitMatrix()));

}




//-----------------------------------------------------------------------------
mqEditACTORDialog::~mqEditACTORDialog()
{

 //depending on what is 
	
  delete this->Ui;
}

int mqEditACTORDialog::SomeThingHasChanged()
{
	int something_has_changed = 0;
	if (this->ACTOR != NULL)
	{
		QColor muiActorColor = this->Ui->ActorColorButton->chosenColor();
		double uiActorcolor[4];
		muiActorColor.getRgbF(&uiActorcolor[0], &uiActorcolor[1], &uiActorcolor[2], &uiActorcolor[3]);
		uiActorcolor[3] = this->Ui->ActorAlpha->value();

		QColor myActorColor;
		double Actorcolor[4];		
		this->ACTOR->GetmColor(Actorcolor);

		if (
			Actorcolor[0] != uiActorcolor[0]
			|| Actorcolor[1] != uiActorcolor[1]
			|| Actorcolor[2] != uiActorcolor[2]
			|| Actorcolor[3] != uiActorcolor[3]
			)
		{
			something_has_changed = 1;
		}
		vtkSmartPointer<vtkMatrix4x4> Mat = this->ACTOR->GetMatrix();

		if (
			Mat->GetElement(0, 0) != this->Ui->M00->value() ||
			Mat->GetElement(0, 1) != this->Ui->M01->value() ||
			Mat->GetElement(0, 2) != this->Ui->M02->value() ||
			Mat->GetElement(0, 3) != this->Ui->M03->value() ||
			Mat->GetElement(1, 0) != this->Ui->M10->value() ||
			Mat->GetElement(1, 1) != this->Ui->M11->value() ||
			Mat->GetElement(1, 2) != this->Ui->M12->value() ||
			Mat->GetElement(1, 3) != this->Ui->M13->value() ||
			Mat->GetElement(2, 0) != this->Ui->M20->value() ||
			Mat->GetElement(2, 1) != this->Ui->M21->value() ||
			Mat->GetElement(2, 2) != this->Ui->M22->value() ||
			Mat->GetElement(2, 3) != this->Ui->M23->value() ||
			Mat->GetElement(3, 0) != this->Ui->M30->value() ||
			Mat->GetElement(3, 1) != this->Ui->M31->value() ||
			Mat->GetElement(3, 2) != this->Ui->M32->value() ||
			Mat->GetElement(3, 3) != this->Ui->M33->value()
			)
		{
			something_has_changed = 1;
		}

		QString myname(this->ACTOR->GetName().c_str());
		
		if (QString::compare(myname, this->Ui->ActorName->text(), Qt::CaseInsensitive)!=0)
		{
			something_has_changed = 1;
			
		}
	
	}

	return something_has_changed;
}
// This dialog is non modal, and actors can have been removed from the collection in the meantime... so before saving actors, we should check whether they are still
//inside the collection.
int mqEditACTORDialog::CurrentActorInCollection()
{
	int actor_found = 0;
	vtkMTActor * Act;
	if (this->ACTOR != NULL && this->ACTOR_Coll != NULL)
	{
		this->ACTOR_Coll->InitTraversal();

		for (vtkIdType i = 0; i < this->ACTOR_Coll->GetNumberOfItems(); i++)
		{
			Act = vtkMTActor::SafeDownCast(this->ACTOR_Coll->GetNextActor());
			if (actor_found == 1) { return actor_found; }
			if (Act == this->ACTOR)
			{
				actor_found = 1;
			}
		}
	}
	return actor_found;
}
void mqEditACTORDialog::saveActor()
{
	//cout << "Save ACTOR!" << endl;
	cout << "Save actor: in collection?:" << this->CurrentActorInCollection() << endl;
	if (this->ACTOR != NULL && this->CurrentActorInCollection())
	{
		int something_has_changed = this->SomeThingHasChanged();
		if (something_has_changed)
		{
			std::string action = "Update actor's color and Matrix";

			int mCount = BEGIN_UNDO_SET(action);
			this->ACTOR->SaveState(mCount);
			QColor myActorColor = this->Ui->ActorColorButton->chosenColor();
			double Actorcolor[4];
			myActorColor.getRgbF(&Actorcolor[0], &Actorcolor[1], &Actorcolor[2], &Actorcolor[3]);
			Actorcolor[3] = this->Ui->ActorAlpha->value();
			this->ACTOR->SetmColor(Actorcolor);
			this->ACTOR->SetName(this->Ui->ActorName->text().toStdString());
			vtkSmartPointer<vtkMatrix4x4> Mat = this->ACTOR->GetMatrix();
			Mat->SetElement(0, 0, this->Ui->M00->value());
			Mat->SetElement(0, 1, this->Ui->M01->value());
			Mat->SetElement(0, 2, this->Ui->M02->value());
			Mat->SetElement(0, 3, this->Ui->M03->value());
			Mat->SetElement(1, 0, this->Ui->M10->value());
			Mat->SetElement(1, 1, this->Ui->M11->value());
			Mat->SetElement(1, 2, this->Ui->M12->value());
			Mat->SetElement(1, 3, this->Ui->M13->value());
			Mat->SetElement(2, 0, this->Ui->M20->value());
			Mat->SetElement(2, 1, this->Ui->M21->value());
			Mat->SetElement(2, 2, this->Ui->M22->value());
			Mat->SetElement(2, 3, this->Ui->M23->value());
			Mat->SetElement(3, 0, this->Ui->M30->value());
			Mat->SetElement(3, 1, this->Ui->M31->value());
			Mat->SetElement(3, 2, this->Ui->M32->value());
			Mat->SetElement(3, 3, this->Ui->M33->value());
			this->ACTOR->ApplyMatrix(Mat);
			this->ACTOR->Modified();
			END_UNDO_SET();
		}
		
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

		vtkSmartPointer<vtkMatrix4x4> Mat = this->ACTOR->GetMatrix();
		this->Ui->M00->setValue(Mat->GetElement(0, 0));
		this->Ui->M01->setValue(Mat->GetElement(0, 1));
		this->Ui->M02->setValue(Mat->GetElement(0, 2));
		this->Ui->M03->setValue(Mat->GetElement(0, 3));
		this->Ui->M10->setValue(Mat->GetElement(1, 0));
		this->Ui->M11->setValue(Mat->GetElement(1, 1));
		this->Ui->M12->setValue(Mat->GetElement(1, 2));
		this->Ui->M13->setValue(Mat->GetElement(1, 3));
		this->Ui->M20->setValue(Mat->GetElement(2, 0));
		this->Ui->M21->setValue(Mat->GetElement(2, 1));
		this->Ui->M22->setValue(Mat->GetElement(2, 2));
		this->Ui->M23->setValue(Mat->GetElement(2, 3));
		this->Ui->M30->setValue(Mat->GetElement(3, 0));
		this->Ui->M31->setValue(Mat->GetElement(3, 1));
		this->Ui->M32->setValue(Mat->GetElement(3, 2));
		this->Ui->M33->setValue(Mat->GetElement(3, 3));
		/*stream << Mat->GetElement(0, 0) << " " << Mat->GetElement(1, 0) << " " << Mat->GetElement(2, 0) << " " << Mat->GetElement(3, 0) << endl;
		stream << Mat->GetElement(0, 1) << " " << Mat->GetElement(1, 1) << " " << Mat->GetElement(2, 1) << " " << Mat->GetElement(3, 1) << endl;
		stream << Mat->GetElement(0, 2) << " " << Mat->GetElement(1, 2) << " " << Mat->GetElement(2, 2) << " " << Mat->GetElement(3, 2) << endl;
		stream << Mat->GetElement(0, 3) << " " << Mat->GetElement(1, 3) << " " << Mat->GetElement(2, 3) << " " << Mat->GetElement(3, 3) << endl;*/
		
		//this->Ui->M00->SetText()
		

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
	if (this->ACTOR != NULL)
	{
		this->ACTOR->SetSelected(0);
		this->ACTOR->Modified();
	}
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

void mqEditACTORDialog::RefreshDialog()
{
	
	this->GetFirstSelectedActor();
	
	this->UpdateUI();
	
	mqMeshToolsCore::instance()->Render();
	
}
void mqEditACTORDialog::slotRefreshDialog()
{
	
	this->RefreshDialog();

}

void mqEditACTORDialog::slotReinitMatrix()
{
	this->Ui->M00->setValue(1);
	this->Ui->M01->setValue(0);
	this->Ui->M02->setValue(0);
	this->Ui->M03->setValue(0);
	this->Ui->M10->setValue(0);
	this->Ui->M11->setValue(1);
	this->Ui->M12->setValue(0);
	this->Ui->M13->setValue(0);
	this->Ui->M20->setValue(0);
	this->Ui->M21->setValue(0);
	this->Ui->M22->setValue(1);
	this->Ui->M23->setValue(0);
	this->Ui->M30->setValue(0);
	this->Ui->M31->setValue(0);
	this->Ui->M32->setValue(0);
	this->Ui->M33->setValue(1);
}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

