/*=========================================================================

   Program: MeshTools
   

=========================================================================*/
#ifndef _mqEditFLGDialog_h
#define _mqEditFLGDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

#include <QDialog>

namespace Ui
{
class mqEditFLGDialog;
}




/**
* mqEditFLGDialog is the dialog used by MeshTools to save CUR files.
*/
class  mqEditFLGDialog : public QDialog
{
  Q_OBJECT

public:
  mqEditFLGDialog(QWidget* Parent);
  
  ~mqEditFLGDialog();
  void saveFLG();
  void GetFirstSelectedFlag();
  void GetFirstFlag();
  void GetNextFlag();
  void GetPrecedingFlag();
  void UpdateUI();
  int SomeThingHasChanged();
  public slots:
  
  virtual void slotRefreshDialog();
  virtual void slotGetPrecedingFlag();
  virtual void slotGetNextFlag();
  virtual void slotsaveFLG();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqEditFLGDialog)
  Ui::mqEditFLGDialog* const Ui;
  vtkLMActor *FLG;
  vtkLMActorCollection *FLG_Coll;
  int current_coll;

  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
