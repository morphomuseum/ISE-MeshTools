/*=========================================================================

   Program: MeshTools
   

=========================================================================*/
#ifndef _mqColorDialog_h
#define _mqColorDialog_h

#include <QDialog>

namespace Ui
{
class mqColorDialog;
}




/**
* mqColorDialog is the Color dialog used by MeshTools.
* It sets mesh default color and background color
*/
class  mqColorDialog : public QDialog
{
  Q_OBJECT

public:
  mqColorDialog(QWidget* Parent);
  
  ~mqColorDialog();

  public slots:
  virtual void slotMeshColorChanged();
  virtual void slotBackGroundColorChanged();
  virtual void slotBackGroundColorChanged2();

protected:
	
private:
  Q_DISABLE_COPY(mqColorDialog)
  Ui::mqColorDialog* const Ui;
};

#endif // !_mqColorDialog_h
