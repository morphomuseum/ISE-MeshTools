/*=========================================================================

   Program: MeshTools
   Module:    mqEditLMKDialogReaction.h


========================================================================*/
#ifndef mqEditLMKDialogReaction_h
#define mqEditLMKDialogReaction_h

#include "mqReaction.h"

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
  static void showEditLMKDialog();

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqEditLMKDialogReaction::showEditLMKDialog(); }

private:
  Q_DISABLE_COPY(mqEditLMKDialogReaction)
};

#endif
