/*=========================================================================

   Program: MeshTools
   Module:    mqCameraReaction.cxx

========================================================================*/
#include "mqCameraReaction.h"

#include "mqMeshToolsCore.h"
#include <vtkRenderer.h>



//-----------------------------------------------------------------------------
mqCameraReaction::mqCameraReaction(QAction* parentObject,  int _mode)
  : Superclass(parentObject)
{
  this->Mode = _mode;
  this->MainWindow = mqMeshToolsCore::instance()->GetMainWindow();
  
}

//@@TODO
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void mqCameraReaction::CameraFront()
{
	//@@TODO
	cout << "Camera Front!" << endl;
	double cameracentre[3] = { 0, 0, 0 };
	if (mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		mqMeshToolsCore::instance()->getActorCollection()->GetCenterOfMass(cameracentre);
		mqMeshToolsCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(mqMeshToolsCore::instance()->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*mqMeshToolsCore::instance()->getCamera()->GetParallelScale();
	cout << "Camdist=" <<camdist<< endl;
	mqMeshToolsCore::instance()->getCamera()->SetPosition(camdist + cameracentre[0], cameracentre[1], cameracentre[2]);
	mqMeshToolsCore::instance()->getCamera()->SetFocalPoint(cameracentre);
	mqMeshToolsCore::instance()->getCamera()->SetViewUp(0, 0, 1);
	mqMeshToolsCore::instance()->getRenderer()->ResetCameraClippingRange();
	mqMeshToolsCore::instance()->ActivateClippingPlane();
	//this->ReplaceCamera();

	mqMeshToolsCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	mqMeshToolsCore::instance()->getGridActor()->SetOutlineMode(mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin());

	mqMeshToolsCore::instance()->getGridActor()->SetGridType(2);
	mqMeshToolsCore::instance()->Render();

	
}

void mqCameraReaction::CameraBack() {
	cout << "mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin()=" << mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin() << endl;
	//this->ReplaceCamera();
	double cameracentre[3] = { 0, 0, 0 };
	if (mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		mqMeshToolsCore::instance()->getActorCollection()->GetCenterOfMass(cameracentre);
		mqMeshToolsCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(mqMeshToolsCore::instance()->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*mqMeshToolsCore::instance()->getCamera()->GetParallelScale();
	mqMeshToolsCore::instance()->getCamera()->SetPosition(-camdist + cameracentre[0], cameracentre[1], cameracentre[2]);
	mqMeshToolsCore::instance()->getCamera()->SetFocalPoint(cameracentre);
	mqMeshToolsCore::instance()->getCamera()->SetViewUp(0, 0, 1);
	mqMeshToolsCore::instance()->getRenderer()->ResetCameraClippingRange();
	mqMeshToolsCore::instance()->ActivateClippingPlane();
	mqMeshToolsCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	mqMeshToolsCore::instance()->getGridActor()->SetOutlineMode(mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin());
	mqMeshToolsCore::instance()->getGridActor()->SetGridType(2);
	mqMeshToolsCore::instance()->Render();
}//1
void mqCameraReaction::CameraLeft() {
	cout << "mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin()=" << mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin() << endl;
	//this->ReplaceCamera
	double cameracentre[3] = { 0, 0, 0 };
	if (mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		mqMeshToolsCore::instance()->getActorCollection()->GetCenterOfMass(cameracentre);
		mqMeshToolsCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(mqMeshToolsCore::instance()->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*mqMeshToolsCore::instance()->getCamera()->GetParallelScale();
	mqMeshToolsCore::instance()->getCamera()->SetPosition(cameracentre[0], camdist + cameracentre[1], cameracentre[2]);
	mqMeshToolsCore::instance()->getCamera()->SetFocalPoint(cameracentre);
	mqMeshToolsCore::instance()->getRenderer()->ResetCameraClippingRange();
	mqMeshToolsCore::instance()->ActivateClippingPlane();

	mqMeshToolsCore::instance()->getCamera()->SetViewUp(0, 0, 1);
	mqMeshToolsCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	mqMeshToolsCore::instance()->getGridActor()->SetOutlineMode(mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin());
	mqMeshToolsCore::instance()->getGridActor()->SetGridType(1);
	mqMeshToolsCore::instance()->Render();
}//2
void mqCameraReaction::CameraRight() 
{
	cout << "mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin()=" << mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin() << endl;
	//this->ReplaceCamera();
	double cameracentre[3] = { 0, 0, 0 };
	if (mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		mqMeshToolsCore::instance()->getActorCollection()->GetCenterOfMass(cameracentre);
		mqMeshToolsCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(mqMeshToolsCore::instance()->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*mqMeshToolsCore::instance()->getCamera()->GetParallelScale();
	mqMeshToolsCore::instance()->getCamera()->SetPosition(cameracentre[0], -camdist + cameracentre[1], cameracentre[2]);
	mqMeshToolsCore::instance()->getCamera()->SetFocalPoint(cameracentre);
	mqMeshToolsCore::instance()->getCamera()->SetViewUp(0, 0, 1);
	mqMeshToolsCore::instance()->getRenderer()->ResetCameraClippingRange();
	mqMeshToolsCore::instance()->ActivateClippingPlane();

	mqMeshToolsCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	mqMeshToolsCore::instance()->getGridActor()->SetOutlineMode(mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin());
	mqMeshToolsCore::instance()->getGridActor()->SetGridType(1);
	mqMeshToolsCore::instance()->Render();
}//3
void mqCameraReaction::CameraAbove() {
	cout << "mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin()=" << mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin() << endl;
	//this->ReplaceCamera();
	double cameracentre[3] = { 0, 0, 0 };
	if (mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		mqMeshToolsCore::instance()->getActorCollection()->GetCenterOfMass(cameracentre);
		mqMeshToolsCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(mqMeshToolsCore::instance()->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*mqMeshToolsCore::instance()->getCamera()->GetParallelScale();
	mqMeshToolsCore::instance()->getCamera()->SetPosition(cameracentre[0], cameracentre[1], camdist + cameracentre[2]);
	mqMeshToolsCore::instance()->getCamera()->SetFocalPoint(cameracentre);


	mqMeshToolsCore::instance()->getCamera()->SetViewUp(-1, 0, 0);
	mqMeshToolsCore::instance()->getRenderer()->ResetCameraClippingRange();
	mqMeshToolsCore::instance()->ActivateClippingPlane();
	mqMeshToolsCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	mqMeshToolsCore::instance()->getGridActor()->SetOutlineMode(mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin());
	mqMeshToolsCore::instance()->getGridActor()->SetGridType(0);
	mqMeshToolsCore::instance()->Render();
}//4
void mqCameraReaction::CameraBelow() {
	cout << "mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin()=" << mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin() << endl;
	double cameracentre[3] = { 0, 0, 0 };
	if (mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		mqMeshToolsCore::instance()->getActorCollection()->GetCenterOfMass(cameracentre);
		mqMeshToolsCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(mqMeshToolsCore::instance()->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*mqMeshToolsCore::instance()->getCamera()->GetParallelScale();
	mqMeshToolsCore::instance()->getCamera()->SetPosition(cameracentre[0], cameracentre[1], -camdist + cameracentre[2]);
	mqMeshToolsCore::instance()->getCamera()->SetFocalPoint(cameracentre);
	mqMeshToolsCore::instance()->getCamera()->SetViewUp(1, 0, 0);
	mqMeshToolsCore::instance()->getRenderer()->ResetCameraClippingRange();
	mqMeshToolsCore::instance()->ActivateClippingPlane();
	//mqMeshToolsCore::instance()->getCamera()->SetParallelScale(120);
	mqMeshToolsCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	mqMeshToolsCore::instance()->getGridActor()->SetOutlineMode(mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin());
	mqMeshToolsCore::instance()->getGridActor()->SetGridType(0);
	mqMeshToolsCore::instance()->Render();
}//5


void mqCameraReaction::CameraCentreOfMassToggle() {
	if (mqMeshToolsCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 1)
	{
		mqMeshToolsCore::instance()->Setmui_CameraCentreOfMassAtOrigin(0);
	}
	else
	{
		mqMeshToolsCore::instance()->Setmui_CameraCentreOfMassAtOrigin(1);
	}

	mqMeshToolsCore::instance()->ReplaceCameraAndGrid();

}
void mqCameraReaction::CameraOrthoPerspectiveToggle() {
	if (mqMeshToolsCore::instance()->Getmui_CameraOrtho() == 1)
	{
		mqMeshToolsCore::instance()->Setmui_CameraOrtho(0);
	}
	else
	{
		mqMeshToolsCore::instance()->Setmui_CameraOrtho(1);

	}

	mqMeshToolsCore::instance()->ResetCameraOrthoPerspective();

}

