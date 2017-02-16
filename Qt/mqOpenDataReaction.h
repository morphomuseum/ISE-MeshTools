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
   void OpenPOS();
   void OpenLMK();
   void OpenFLG();
   void OpenCUR();
   void OpenTAG();
   void OpenORI();
   void OpenVERFile(QString fileName, int mode);
   void OpenMeshFile(QString fileName);
   void OpenPOSFile(QString fileName, int mode);
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
  }

private:
	Q_DISABLE_COPY(mqOpenDataReaction)
	QMainWindow* MainWindow;
  int Mode;
};

#endif
