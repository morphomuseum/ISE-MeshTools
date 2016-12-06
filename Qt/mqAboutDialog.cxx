/*=========================================================================

   Program: ParaView
   Module:    $RCS $

 
=========================================================================*/

#include "mqAboutDialog.h"
#include "ui_mqAboutDialog.h"


#include <QApplication>
#include <QFile>
#include <QHeaderView>

#include <sstream>

//-----------------------------------------------------------------------------
mqAboutDialog::mqAboutDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqAboutDialog())
{
  this->Ui->setupUi(this);
  this->setObjectName("mqAboutDialog");
    
}

//-----------------------------------------------------------------------------
mqAboutDialog::~mqAboutDialog()
{
  delete this->Ui;
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

