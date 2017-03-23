/*=========================================================================

   Program: MeshTools
   Module:    mqEditLMKDialogReaction.h


========================================================================*/
#ifndef mqEditLMKDialogReaction_h
#define mqEditLMKDialogReaction_h

#include "mqReaction.h"
#include "mqEditLMKDialog.h"
/**
* @ingroup Reactions
* mqEditLMKDialogReaction used to show the standard Color dialog for the
* application.
*/
class  mqEditLMKDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqEditLMKDialogReaction(QAction* parent);

  /**
  * Shows the Color dialog for the application.
  */
  static void showEditLMKDialog(mqEditLMKDialog *color_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqEditLMKDialogReaction::showEditLMKDialog(this->color_dialog); }

private:
	Q_DISABLE_COPY(mqEditLMKDialogReaction)
		mqEditLMKDialog *color_dialog;
};

#endif
