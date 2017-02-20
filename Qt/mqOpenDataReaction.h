/*=========================================================================

   Program: MeshTools
   Module:   mqOpenDataReaction

========================================================================*/
#ifndef mqOpenDataReaction_h
#define mqOpenDataReaction_h

#include "mqReaction.h"
#include <QMainWindow>


/**
* @ingroup Reactions
* Reaction for application undo-redo.
*/
class mqOpenDataReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  /**
  * if \c undo is set to true, then this behaves as an undo-reaction otherwise
  * as a redo-reaction.
  */
  mqOpenDataReaction(QAction* parent,  int _mode);

  //mode
   void OpenData();
   void OpenNTW();
   void OpenMesh();
   void OpenPOS(int mode);
   void OpenLMK_or_VER(int mode);
   void OpenFLG();
   void OpenCUR();
   void OpenTAG();
   void OpenORI();
   
   void OpenFLG(QString fileName);
   void OpenCUR(QString fileName);
   void OpenTAG(QString fileName);
   void OpenORI(QString fileName);
   void OpenNTW(QString fileName);
   void OpenVER(QString fileName, int mode);
   void OpenLMK(QString fileName, int mode);
   void OpenMesh(QString fileName);
   void OpenPOS(QString fileName, int mode);
protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered()
  {
    if (this->Mode ==0)
    {
      //mqOpenDataReaction::OpenData();
		this->OpenData();
    }
    else if (this->Mode == 1)
    {
     this->OpenNTW();
    }
	else if (this->Mode == 2)
	{
		this->OpenMesh();
	}
	else if (this->Mode == 3)
	{
		this->OpenLMK_or_VER(0);
	}
	else if (this->Mode == 4)
	{
		this->OpenLMK_or_VER(1);
	}
	else if (this->Mode == 5)
	{
		this->OpenCUR();
	}
	else if (this->Mode == 6)
	{
		this->OpenFLG();
	}
	else if (this->Mode == 7)
	{
		this->OpenORI();
	}
	else if (this->Mode == 8)
	{
		this->OpenPOS(0);
	}
	else if (this->Mode == 9)
	{
		this->OpenTAG();
	}
	else if (this->Mode == 10)
	{
		this->OpenPOS(2);
	}
  }

private:
	Q_DISABLE_COPY(mqOpenDataReaction)
	QMainWindow* MainWindow;
  int Mode;
};

#endif
