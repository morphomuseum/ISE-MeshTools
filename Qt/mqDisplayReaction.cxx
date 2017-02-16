/*=========================================================================

   Program: MeshTools
   Module:    mqDisplayReaction.cxx

========================================================================*/
#include "mqDisplayReaction.h"

#include "mqMeshToolsCore.h"



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
mqDisplayReaction::mqDisplayReaction(QAction* parentObject, int _mode)
	: Superclass(parentObject)
{
	this->Mode = _mode;
	this->MainWindow = mqMeshToolsCore::instance()->GetMainWindow();

}

void mqDisplayReaction::GridToggle() {

	if (mqMeshToolsCore::instance()->Getmui_ShowGrid() == 1)
	{
		mqMeshToolsCore::instance()->Setmui_ShowGrid(0);
	}
	else
	{
		mqMeshToolsCore::instance()->Setmui_ShowGrid(1);
	}
	mqMeshToolsCore::instance()->SetGridVisibility();
}//0
void mqDisplayReaction::OrientationHelperToggle() {
	if (mqMeshToolsCore::instance()->Getmui_ShowOrientationHelper() == 1)
	{
		mqMeshToolsCore::instance()->Setmui_ShowOrientationHelper(0);
	}
	else
	{
		mqMeshToolsCore::instance()->Setmui_ShowOrientationHelper(1);
	}
	mqMeshToolsCore::instance()->SetOrientationHelperVisibility();

}//1
void mqDisplayReaction::RendererAnaglyphToggle() {

	if (mqMeshToolsCore::instance()->Getmui_Anaglyph() == 1)
	{
		mqMeshToolsCore::instance()->Setmui_Anaglyph(0);
	}
	else
	{
		mqMeshToolsCore::instance()->Setmui_Anaglyph(1);
	}

}//2
