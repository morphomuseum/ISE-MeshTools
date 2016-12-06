/*=========================================================================

   Program: MeshTools
   

=========================================================================*/
#ifndef _mqAboutDialog_h
#define _mqAboutDialog_h

#include <QDialog>

namespace Ui
{
class mqAboutDialog;
}




/**
* mqAboutDialog is the about dialog used by MeshTools.
* It provides information about MeshTools.
*/
class  mqAboutDialog : public QDialog
{
  Q_OBJECT

public:
  mqAboutDialog(QWidget* Parent);
  ~mqAboutDialog();

protected:
  
private:
  Q_DISABLE_COPY(mqAboutDialog)
  Ui::mqAboutDialog* const Ui;
};

#endif // !_mqAboutDialog_h
