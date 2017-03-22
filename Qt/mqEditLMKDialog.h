/*=========================================================================

   Program: MeshTools
   

=========================================================================*/
#ifndef _mqEditLMKDialog_h
#define _mqEditLMKDialog_h

#include <QDialog>

namespace Ui
{
class mqEditLMKDialog;
}




/**
* mqEditLMKDialog is the dialog used by MeshTools to save CUR files.
*/
class  mqEditLMKDialog : public QDialog
{
  Q_OBJECT

public:
  mqEditLMKDialog(QWidget* Parent);
  
  ~mqEditLMKDialog();

  public slots:
  virtual void slotEditLMK();
  
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqEditLMKDialog)
  Ui::mqEditLMKDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
