/*=========================================================================

   Program: MeshTools
   Module:    mqReaction.cxx

   

========================================================================*/
#include "mqReaction.h"

//-----------------------------------------------------------------------------
//mqReaction::mqReaction(QAction* parentObject, Qt::ConnectionType type, QMainWindow* mainWindow = 0)
mqReaction::mqReaction(QAction* parentObject, Qt::ConnectionType type)
  : Superclass(parentObject)
{
  Q_ASSERT(parentObject != NULL);

  QObject::connect(parentObject, SIGNAL(triggered(bool)), this, SLOT(onTriggered()), type);

  // Deal with master/slave enable/disable

  /*QObject::connect(mainWindow, SIGNAL(updateMasterEnableState(bool)), this,
    SLOT(updateMasterEnableState(bool)));*/

  this->IsMaster = true;
}

//-----------------------------------------------------------------------------
mqReaction::~mqReaction()
{
}

//-----------------------------------------------------------------------------
void mqReaction::updateMasterEnableState(bool isMaster)
{
  this->IsMaster = isMaster;
  updateEnableState();
}
