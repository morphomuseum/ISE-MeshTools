/*=========================================================================

   Program: MeshTools
   Module:    $RCS $

 
=========================================================================*/

#include "mqEditScalarsDialog.h"
#include "ui_mqEditScalarsDialog.h"
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
mqEditScalarsDialog::mqEditScalarsDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqEditScalarsDialog())
{

	//1 populate active scalar combo box, and check which scalar is the active one!
	this->Ui->setupUi(this);
	this->setObjectName("mqEditScalarsDialog");
	this->Ui->comboActiveScalar->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	
	this->Ui->comboColorMap->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	connect(mqMeshToolsCore::instance(), SIGNAL(existingScalarsChanged()), this, SLOT(slotRefreshComboScalars()));
	connect(mqMeshToolsCore::instance(), SIGNAL(activeScalarChanged()), this, SLOT(slotRefreshComboScalars()));
	connect(this->Ui->comboActiveScalar, SIGNAL(activated(int)), this, SLOT(slotActiveScalarChanged(int)));
	connect(this->Ui->comboColorMap, SIGNAL(activated(int)), this, SLOT(slotActiveColorMapChanged(int)));
	
	this->Ui->currentMin->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->currentMax->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->suggestedMin->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->suggestedMax->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->currentMin->setMinimum(-DBL_MAX);
	this->Ui->currentMax->setMinimum(-DBL_MAX);
	this->Ui->suggestedMin->setMinimum(-DBL_MAX);
	this->Ui->suggestedMax->setMinimum(-DBL_MAX);

	this->Ui->currentMin->setMaximum(DBL_MAX);
	this->Ui->currentMax->setMaximum(DBL_MAX);
	this->Ui->suggestedMin->setMaximum(DBL_MAX);
	this->Ui->suggestedMax->setMaximum(DBL_MAX);

	connect(this->Ui->pushScalarSuggestedMax, SIGNAL(pressed()), this, SLOT(slotacceptSuggestedMax()));
	connect(this->Ui->pushScalarSuggestedMin, SIGNAL(pressed()), this, SLOT(slotacceptSuggestedMin()));
	/*
	sc_show:
scWindow->show();



sliderMin->value(MT->Get_sc_min());
sliderMax->value(MT->Get_sc_max());
currentMax->value(MT->Get_sc_max());
currentMin->value(MT->Get_sc_min());
suggestedMax->value(MT->scalars_get_max());
suggestedMin->value(MT->scalars_get_min());
Refresh();
	*/

	/*connect(mqMeshToolsCore::instance(), SIGNAL(lmSelectionChanged()), this, SLOT(slotRefreshDialog()));
	connect(this->Ui->next, SIGNAL(pressed()), this, SLOT(slotGetNextFlag()));
	connect(this->Ui->prec, SIGNAL(pressed()), this, SLOT(slotGetPrecedingFlag()));
	
	this->FLG_Coll = NULL;
	this->FLG = NULL;
	
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
 */

}




//-----------------------------------------------------------------------------
mqEditScalarsDialog::~mqEditScalarsDialog()
{

 //depending on what is 
	
  delete this->Ui;
}

int mqEditScalarsDialog::SomeThingHasChanged()
{
	int something_has_changed = 0;
	
	return something_has_changed;
}




void mqEditScalarsDialog::UpdateUI()
{
	/*int color_scale_id = MT->GetColorScaleId();
	
	currentMax->value(MT->Get_sc_max());
	currentMin->value(MT->Get_sc_min());
*/

	//1 populate comboActiveScalar

	this->RefreshComboScalars();

	// 2 populate comboColorMap
	this->RefreshComboColorMaps();

	this->RefreshSuggestedRange();
	this->RefreshSliders();
	
}

void mqEditScalarsDialog::RefreshSuggestedRange() 
{
	this->Ui->suggestedMax->setValue(mqMeshToolsCore::instance()->GetSuggestedScalarRangeMax());
	this->Ui->suggestedMin->setValue(mqMeshToolsCore::instance()->GetSuggestedScalarRangeMin());
}
void mqEditScalarsDialog::RefreshSliders() 
{
	/*
	sliderMin->maximum((MT->Get_sc_max() + MT->Get_sc_min()) / 2);
	sliderMin->minimum((3 * MT->Get_sc_min() - MT->Get_sc_max()) / 2);
	sliderMax->minimum((MT->Get_sc_max() + MT->Get_sc_min()) / 2);
	sliderMax->maximum((3 * MT->Get_sc_max() - MT->Get_sc_min()) / 2);
	sliderMax->redraw();
	sliderMax->redraw();
	*/
}
void mqEditScalarsDialog::RefreshRange() 
{
}

void mqEditScalarsDialog::RefreshComboScalars()
{
	this->Ui->comboActiveScalar->clear();
	ExistingScalars *MyList = mqMeshToolsCore::instance()->Getmui_ExistingScalars();
	for (int i = 0; i < MyList->Stack.size(); i++)
	{
		if ((MyList->Stack.at(i).DataType == VTK_FLOAT || MyList->Stack.at(i).DataType == VTK_DOUBLE) && MyList->Stack.at(i).NumComp == 1)
		{
			this->Ui->comboActiveScalar->addItem(MyList->Stack.at(i).Name);
		}

	}
	QString myActiveScalars = mqMeshToolsCore::instance()->Getmui_ActiveScalars()->Name;
	cout << "DIAL myActiveScalars " << myActiveScalars.toStdString() << endl;
	int exists = -1;
	for (int i = 0; i < this->Ui->comboActiveScalar->count(); i++)
	{
		QString myScalar = this->Ui->comboActiveScalar->itemText(i);
		if (myScalar == myActiveScalars)
		{
			cout << "DIAL found in list!!!!! " << myScalar.toStdString() << endl;
			exists = i;

		}

	}
	if (exists > -1) {
		cout << "DIAL Now current index of combo box is " << exists << endl;
		this->Ui->comboActiveScalar->setCurrentIndex(exists);


	}
}


void mqEditScalarsDialog::RefreshComboColorMaps() 
{
	this->Ui->comboColorMap->clear();
	ExistingColorMaps *MyCM = mqMeshToolsCore::instance()->Getmui_ExistingColorMaps();
	for (int i = 0; i < MyCM->Stack.size(); i++)
	{

		this->Ui->comboColorMap->addItem(MyCM->Stack.at(i).Name);


	}
	QString myActiveCM = mqMeshToolsCore::instance()->Getmui_ActiveColorMap()->Name;
	cout << "DIAL myActiveColorMap " << myActiveCM.toStdString() << endl;
	int exists = -1;
	for (int i = 0; i < this->Ui->comboColorMap->count(); i++)
	{
		QString myCM = this->Ui->comboColorMap->itemText(i);
		if (myCM == myActiveCM)
		{
			cout << "DIAL found CM in list!!!!! " << myCM.toStdString() << endl;
			exists = i;

		}

	}
	if (exists > -1) {
		cout << "DIAL Now current index of combo box is " << exists << endl;
		this->Ui->comboColorMap->setCurrentIndex(exists);


	}
}


void mqEditScalarsDialog::slotacceptSuggestedMax()
{
	this->Ui->currentMax->setValue(this->Ui->suggestedMax->value());
}

void mqEditScalarsDialog::slotacceptSuggestedMin()
{
	
	this->Ui->currentMin->setValue(this->Ui->suggestedMin->value());
}

void mqEditScalarsDialog::slotRefreshComboScalars()
{
	this->RefreshComboScalars();


}


void mqEditScalarsDialog::RefreshDialog()
{
	this->UpdateUI();
	mqMeshToolsCore::instance()->Render();
}


void mqEditScalarsDialog::slotActiveScalarChanged(int idx)
{
	cout << "looks like active scalar has changed!:: " << idx << endl;
	QString NewActiveScalarName = this->Ui->comboActiveScalar->currentText();
	for (int i = 0; i < mqMeshToolsCore::instance()->Getmui_ExistingScalars()->Stack.size(); i++)
	{
		QString myExisingScalarName = mqMeshToolsCore::instance()->Getmui_ExistingScalars()->Stack.at(i).Name;
		if (NewActiveScalarName == myExisingScalarName)
		{

			mqMeshToolsCore::instance()->Setmui_ActiveScalarsAndRender(NewActiveScalarName,
				mqMeshToolsCore::instance()->Getmui_ExistingScalars()->Stack.at(i).DataType,
				mqMeshToolsCore::instance()->Getmui_ExistingScalars()->Stack.at(i).NumComp
			);
			this->RefreshSuggestedRange();

		}
	}


}


void mqEditScalarsDialog::slotActiveColorMapChanged(int idx)
{
	cout << "looks like active color map has changed!:: " << idx << endl;
	QString NewActiveColorMap = this->Ui->comboColorMap->currentText();
	for (int i = 0; i < mqMeshToolsCore::instance()->Getmui_ExistingColorMaps()->Stack.size(); i++)
	{
		QString myExisingColorMapName = mqMeshToolsCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).Name;
		if (NewActiveColorMap == myExisingColorMapName)
		{
			

			mqMeshToolsCore::instance()->Setmui_ActiveColorMapAndRender(NewActiveColorMap,
				mqMeshToolsCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).ColorMap
			);
		


		}
	}


}

/*

comboActiveScalar :
MT->Set_Active_Scalar(
((Fl_Choice *)o)->value());
suggestedMin->value(MT->scalars_get_max());
suggestedMax->value(MT->scalars_get_min());
MT->Update_RGB();
MT->redraw();

pushInitScalar:
MT->Initialize_Scalar(comboActiveScalar->value());
MT->redraw();

pushRemoveScalar
MT->Remove_Scalar(comboActiveScalar->value());
MT->redraw()

sliderMin:
MT->Set_sc_min(((Fl_Slider *)o)->value());
Refresh();
MT->Update_RGB();
MT->redraw();

sliderMax:
MT->Set_sc_max(((Fl_Slider *)o)->value());
Refresh();
MT->Update_RGB();
MT->redraw();

cbBelowMin:
MT->SC_Show_Below_Min(((Fl_Check_Button *)o)->value());
MT->redraw();

cbBelowMax:
MT->SC_Show_Above_Max(((Fl_Check_Button *)o)->value());
MT->redraw();

pushScalarSuggestedMin;
currentMin->value(suggestedMin->value());

pushScalarSuggestedMax;
currentMax->value(suggestedMax->value());


comboColorMap:
MT->SetColorScaleId(
((Fl_Choice *)o)->value());

suggestedMax->value(MT->scalars_get_max());
Min_sc->value(MT->scalars_get_min());
MT->Update_RGB();
MT->redraw();




Refresh:


update_2:
suggestedMax->value(MT->scalars_get_max());
suggestedMin->value(MT->scalars_get_min());


*/

void mqEditScalarsDialog::slotAccepted()
{
	

	/*MT->Set_sc_max(currentMax->value());
	MT->Set_sc_min(currentMin->value());
	if (comboActiveScalar->value() >= 0)
	{
		MT->Set_Active_Scalar(comboActiveScalar->value());
	}

	if (comboColorMap->value() >= 0)
	{

		MT->SetColorScaleId(comboColorMap->value());
	}
	sliderMin->value(MT->Get_sc_min());
	sliderMax->value(MT->Get_sc_max());
	sliderMin->maximum((MT->Get_sc_max() + MT->Get_sc_min()) / 2);
	sliderMin->minimum((3 * MT->Get_sc_min() - MT->Get_sc_max()) / 2);
	sliderMax->minimum((MT->Get_sc_max() + MT->Get_sc_min()) / 2);
	sliderMax->maximum((3 * MT->Get_sc_max() - MT->Get_sc_min()) / 2);
	sliderMax->redraw();
	sliderMin->redraw();
	MT->Update_RGB();
	MT->redraw();*/

}
void mqEditScalarsDialog::slotRefreshDialog()
{
	
	this->RefreshDialog();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

