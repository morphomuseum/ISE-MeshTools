/*=========================================================================

   Program: MeshTools
   

=========================================================================*/
#ifndef _mqSaveVTPDialog_h
#define _mqSaveVTPDialog_h

#include <QDialog>

namespace Ui
{
class mqSaveVTPDialog;
}




/**
* mqSaveVTPDialog is the dialog used by MeshTools to save VTP files.
*/
class  mqSaveVTPDialog : public QDialog
{
  Q_OBJECT

public:
  mqSaveVTPDialog(QWidget* Parent, QString fileName);
  
  ~mqSaveVTPDialog();

  public slots:
  virtual void slotSaveVTPFile();
  
  

protected:
	
private:
	QString m_fileName;
  Q_DISABLE_COPY(mqSaveVTPDialog)
  Ui::mqSaveVTPDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif // !_mqLandmarkDialog_h
