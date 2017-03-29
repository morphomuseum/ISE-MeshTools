/*=========================================================================

   Program: MeshTools
   

=========================================================================*/
#ifndef _mqEditACTORDialog_h
#define _mqEditACTORDialog_h

#include "vtkMTActor.h"
#include "vtkMTActorCollection.h"

#include <QDialog>

namespace Ui
{
class mqEditACTORDialog;
}




/**
* mqEditACTORDialog is the dialog used by MeshTools to save CUR files.
*/
class  mqEditACTORDialog : public QDialog
{
  Q_OBJECT

public:
  mqEditACTORDialog(QWidget* Parent);
  
  ~mqEditACTORDialog();
  void saveActor();
  void GetFirstSelectedActor();
  void GetFirstActor();
  void GetNextActor();
  void GetPrecedingActor();
  void UpdateUI();
  public slots:
  
  virtual void slotRefreshDialog();
  virtual void slotGetPrecedingActor();
  virtual void slotGetNextActor();
  virtual void slotsaveActor();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqEditACTORDialog)
  Ui::mqEditACTORDialog* const Ui;
  vtkMTActor *ACTOR;
  vtkMTActorCollection *ACTOR_Coll;
  int current_coll;

  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
