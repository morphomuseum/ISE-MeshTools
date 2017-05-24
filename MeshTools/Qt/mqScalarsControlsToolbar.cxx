/*=========================================================================

   Program: MeshTools
  

========================================================================*/
#include "mqScalarsControlsToolbar.h"
#include "ui_mqScalarsControlsToolbar.h"

// For later!
#include "mqEditScalarsDialogReaction.h"
#include "mqUndoRedoReaction.h"
#include "mqMeshToolsCore.h"
#include "mqDisplayReaction.h"

#include <QToolButton>
#include <QComboBox>


//-----------------------------------------------------------------------------
void mqScalarsControlsToolbar::constructor()
{
 // Ui::mqScalarsControlsToolbar ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqScalarsControlsToolbar;
  this->ui->setupUi(this);
  this->comboActiveScalars = new QComboBox;
  this->comboActiveScalars->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  //@@ to do : populate combo box according to found scalars
  // Create a list with all possible scalars.
  //this->comboActiveScalars->addItems({ "Initial RGB", "Tags", "Thickness", "Curvature"});
  this->comboActiveScalars->addItems({ "none" });
  // Add values in the combo box
  this->addWidget(this->comboActiveScalars);
  if (mqMeshToolsCore::instance()->Getmui_ScalarVisibility() == 1)
  {

	  this->ui->actionScalarVisibility->setChecked(true);
  }
  else
  {
	  this->comboActiveScalars->setDisabled(true);
  }
  this->ui->actionTagEdit->setDisabled(true);
  this->ui->actionColorScaleEdit->setDisabled(true);
  connect(mqMeshToolsCore::instance(), SIGNAL(existingScalarsChanged()), this, SLOT(slotRefreshComboScalars()));

  connect(this->ui->actionScalarVisibility, SIGNAL(triggered()), this, SLOT(slotScalarVisitiliby()));
  connect(mqMeshToolsCore::instance(), SIGNAL(activeScalarChanged()), this, SLOT(slotRefreshComboScalars()));
  connect(this->comboActiveScalars, SIGNAL(activated(int)), this, SLOT(slotActiveScalarChanged(int)));
  new mqEditScalarsDialogReaction(this->ui->actionColorScaleEdit);
  
}

void mqScalarsControlsToolbar::slotActiveScalarChanged(int idx)
{
	cout << "looks like active scalar has changed!:: " << idx << endl;
	QString NewActiveScalarName = this->comboActiveScalars->currentText();
	for (int i = 0; i < mqMeshToolsCore::instance()->Getmui_ExistingScalars()->Stack.size(); i++)
	{
		QString myExisingScalarName = mqMeshToolsCore::instance()->Getmui_ExistingScalars()->Stack.at(i).Name;
		if (NewActiveScalarName == myExisingScalarName)
		{
		
			mqMeshToolsCore::instance()->Setmui_ActiveScalarsAndRender(NewActiveScalarName,
				mqMeshToolsCore::instance()->Getmui_ExistingScalars()->Stack.at(i).DataType, 
				mqMeshToolsCore::instance()->Getmui_ExistingScalars()->Stack.at(i).NumComp
			);
			this->RefreshEditButtons();

		}
	}
	
	
}

void mqScalarsControlsToolbar::RefreshEditButtons()
{

	this->ui->actionTagEdit->setEnabled(false);
	this->ui->actionColorScaleEdit->setEnabled(false);
	if ((mqMeshToolsCore::instance()->Getmui_ActiveScalars()->DataType == VTK_INT ||
		mqMeshToolsCore::instance()->Getmui_ActiveScalars()->DataType == VTK_UNSIGNED_INT)
		&& mqMeshToolsCore::instance()->Getmui_ActiveScalars()->NumComp == 1
		)
	{
		this->ui->actionTagEdit->setEnabled(true);
	}
	if ((mqMeshToolsCore::instance()->Getmui_ActiveScalars()->DataType == VTK_FLOAT ||
		mqMeshToolsCore::instance()->Getmui_ActiveScalars()->DataType == VTK_DOUBLE)
		&& mqMeshToolsCore::instance()->Getmui_ActiveScalars()->NumComp == 1
		)
	{
		this->ui->actionColorScaleEdit->setEnabled(true);
	}
}

void mqScalarsControlsToolbar::slotRefreshComboScalars()
{
	//cout << "Refresh combo "<< endl;
	this->comboActiveScalars->clear();
	ExistingScalars *MyList = mqMeshToolsCore::instance()->Getmui_ExistingScalars();
	for (int i = 0; i < MyList->Stack.size(); i++)
	{
		this->comboActiveScalars->addItem(MyList->Stack.at(i).Name);
		
	}
	QString myActiveScalars = mqMeshToolsCore::instance()->Getmui_ActiveScalars()->Name;
	//cout << "myActiveScalars " << myActiveScalars.toStdString()<< endl;
	int exists = -1;
	for (int i = 0; i < MyList->Stack.size(); i++)
	{
		QString myScalar = MyList->Stack.at(i).Name;
		if (myScalar == myActiveScalars)
		{
			//cout << "found in list!!!!! " << myScalar.toStdString() << endl;
			exists = i;

		}

	}
	if (exists > -1) { 
		//cout << "Now current index of combo box is " << exists << endl;
		this->comboActiveScalars->setCurrentIndex(exists); 
		this->RefreshEditButtons();
		
	}
	
	
}
void mqScalarsControlsToolbar::slotScalarVisitiliby()
{

	if (this->ui->actionScalarVisibility->isChecked())
	{
		mqMeshToolsCore::instance()->Setmui_ScalarVisibility(1);
		this->comboActiveScalars->setDisabled(false);
	}
	else
	{
		mqMeshToolsCore::instance()->Setmui_ScalarVisibility(0);
		this->comboActiveScalars->setDisabled(true);
	}
	
	mqMeshToolsCore::instance()->Render();

	

}


