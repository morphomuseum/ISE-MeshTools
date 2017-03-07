/*=========================================================================

   Program: MeshTools
   

=========================================================================*/
#ifndef _mqSavePLYDialog_h
#define _mqSavePLYDialog_h

#include <QDialog>

namespace Ui
{
class mqSavePLYDialog;
}




/**
* mqSavePLYDialog is the dialog used by MeshTools to save PLY files.
*/
class  mqSavePLYDialog : public QDialog
{
  Q_OBJECT

public:
  mqSavePLYDialog(QWidget* Parent);
  
  ~mqSavePLYDialog();

  public slots:
  virtual void slotPLYFileTypeChanged();
  virtual void slotPLYPositionChanged();
  virtual void slotPLYNormalsChanged();
  

protected:
	
private:
  Q_DISABLE_COPY(mqSavePLYDialog)
  Ui::mqSavePLYDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif // !_mqLandmarkDialog_h
