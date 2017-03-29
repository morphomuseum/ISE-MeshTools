/*=========================================================================

   Program: MeshTools
   

=========================================================================*/
#ifndef _mqEditAllFLGDialog_h
#define _mqEditAllFLGDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

#include <QDialog>

namespace Ui
{
class mqEditAllFLGDialog;
}




/**
* mqEditAllFLGDialog is the dialog used by MeshTools to save CUR files.
*/
class  mqEditAllFLGDialog : public QDialog
{
  Q_OBJECT

public:
  mqEditAllFLGDialog(QWidget* Parent);
  
  ~mqEditAllFLGDialog();
  void saveAllFLG();
  
  public slots:
  
  
  
  virtual void slotsaveAllFLG();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqEditAllFLGDialog)
  Ui::mqEditAllFLGDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
