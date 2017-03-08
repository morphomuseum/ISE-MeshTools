/*=========================================================================

   Program: MeshTools
   Module:    mqSaveVTPDialogReaction.h


========================================================================*/
#ifndef mqSaveVTPDialogReaction_h
#define mqSaveVTPDialogReaction_h


#include "mqReaction.h"

/**
* @ingroup Reactions
* mqSaveVTPDialogReaction used to show the standard VTP Save dialog for the
* application.
*/
class  mqSaveVTPDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSaveVTPDialogReaction(QAction* parent);

  /**
  * Shows the SaveVTP dialog for the application.
  */
  static void showSaveVTPDialog(QString fileName);

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();


private:
  Q_DISABLE_COPY(mqSaveVTPDialogReaction)
};

#endif
