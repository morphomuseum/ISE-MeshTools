/*=========================================================================

   Program: MeshTools
   Module:    mqEditAllFLGDialogReaction.h


========================================================================*/
#ifndef mqEditAllFLGDialogReaction_h
#define mqEditAllFLGDialogReaction_h

#include "mqReaction.h"
#include "mqEditAllFLGDialog.h"
/**
* @ingroup Reactions
* mqEditAllFLGDialogReaction used to show the standard FLG dialog for the
* application.
*/
class  mqEditAllFLGDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqEditAllFLGDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showEditAllFLGDialog(mqEditAllFLGDialog *FLG_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqEditAllFLGDialogReaction::showEditAllFLGDialog(this->FLG_dialog); }

private:
	Q_DISABLE_COPY(mqEditAllFLGDialogReaction)
		mqEditAllFLGDialog *FLG_dialog;
};

#endif
