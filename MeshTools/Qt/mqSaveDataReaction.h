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
  
  
  /* void SavePLY();
   void SaveSTL();
   void SaveVTP();*/
   void SavePOS();
   
   
   void SaveCURInfos();
   void SaveORI();
   
   
   
   
  
protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered()
  {
	  if (this->Mode == 1)
	  {
	  }
	/*else if (this->Mode == 2)
	{
		this->SavePLY();
	}*/
	else if (this->Mode == 3)
	{
		
	}
	else if (this->Mode == 4)
	{
	
	}
	else if (this->Mode == 5)
	{
	
	}
	else if (this->Mode == 6)
	{
		
	}
	else if (this->Mode == 7)
	{
		this->SaveORI();
	}	
	else if (this->Mode == 9)
	{
		
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
		
	}
	else if (this->Mode == 14)
	{
		
	}
	else if (this->Mode == 15)
	{
		
	}
	else if (this->Mode == 16)
	{
		
	}
	else if (this->Mode == 17)
	{
		this->SaveCURInfos();
	}
  }

private:
	Q_DISABLE_COPY(mqSaveDataReaction)
	QMainWindow* MainWindow;
  int Mode;
};

#endif
