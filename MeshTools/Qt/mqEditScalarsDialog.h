/*=========================================================================

   Program: MeshTools
   

=========================================================================*/
#ifndef _mqEditScalarsDialog_h
#define _mqEditScalarsDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"
#include <vtkSmartPointer.h>
#include <QDialog>

namespace Ui
{
class mqEditScalarsDialog;
}




/**
* mqEditScalarsDialog is the dialog used by MeshTools to save CUR files.
*/
class  mqEditScalarsDialog : public QDialog
{
  Q_OBJECT

public:
  mqEditScalarsDialog(QWidget* Parent);
  
  ~mqEditScalarsDialog();
 
  void UpdateUI();
  int SomeThingHasChanged();
 void RefreshDialog();
  public slots:
  
  virtual void slotRefreshDialog();
  virtual void slotAccepted();

protected:
	
private:
	

  Q_DISABLE_COPY(mqEditScalarsDialog)
  Ui::mqEditScalarsDialog* const Ui;
 
 

  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
