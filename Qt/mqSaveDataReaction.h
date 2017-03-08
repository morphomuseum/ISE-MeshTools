/*=========================================================================

   Program: MeshTools
   Module:   mqSaveDataReaction

========================================================================*/
#ifndef mqSaveDataReaction_h
#define mqSaveDataReaction_h

#include "mqReaction.h"
#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkMatrix4x4.h>
/**
* @ingroup Reactions
* Reaction for application undo-redo.
*/
class mqSaveDataReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  /**
  * if \c undo is set to true, then this behaves as an undo-reaction otherwise
  * as a redo-reaction.
  */
  mqSaveDataReaction(QAction* parent,  int _mode);

  //mode
  
   void SaveNTW();
  /* void SavePLY();
   void SaveSTL();
   void SaveVTP();*/
   void SavePOS();
   void SaveLMK_or_VER(int mode);
   
   void SaveFLG();
   void SaveCUR();
   void SaveSTV();
   void SaveTAG();
   void SaveORI();
   
   void SaveFLG(QString fileName);
   void SaveCUR(QString fileName);
   void SaveSTV(QString fileName);
   void SaveTAG(QString fileName);
   void SaveORI(QString fileName);   
   void SavePOS(vtkSmartPointer<vtkMatrix4x4> Mat, QString fileName);
  
protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered()
  {
    if (this->Mode == 1)
    {
     this->SaveNTW();
    }
	/*else if (this->Mode == 2)
	{
		this->SavePLY();
	}*/
	else if (this->Mode == 3)
	{
		this->SaveLMK_or_VER(0);
	}
	else if (this->Mode == 4)
	{
		this->SaveLMK_or_VER(1);
	}
	else if (this->Mode == 5)
	{
		this->SaveCUR();
	}
	else if (this->Mode == 6)
	{
		this->SaveFLG();
	}
	else if (this->Mode == 7)
	{
		this->SaveORI();
	}	
	else if (this->Mode == 9)
	{
		this->SaveTAG();
	}
	else if (this->Mode == 8)
	{
		this->SavePOS();
	}	
	/*else if (this->Mode == 10)
	{
		this->SaveSTL();
	}
	else if (this->Mode == 11)
	{
		this->SaveVTP();
	}*/
	else if (this->Mode == 13)
	{
		this->SaveSTV();
	}
	else if (this->Mode == 14)
	{
		this->SaveLMK_or_VER(2);
	}
	else if (this->Mode == 15)
	{
		this->SaveLMK_or_VER(3);
	}
	else if (this->Mode == 16)
	{
		this->SaveSTV();
	}
  }

private:
	Q_DISABLE_COPY(mqSaveDataReaction)
	QMainWindow* MainWindow;
  int Mode;
};

#endif
