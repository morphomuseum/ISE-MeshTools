/*=========================================================================

   Program: MeshTools
   Module:    Copied from Paraview pqMainControlsToolbar.cxx

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

========================================================================*/
#include "mqObjectsControlsToolbar.h"
#include "ui_mqObjectsControlsToolbar.h"

// For later!
#include "QReleaseSlider.h"
//#include "QReleaseSliderValue.h"

#include "mqSaveNTWDialogReaction.h"
#include "mqUndoRedoReaction.h"
#include "mqEditLMKDialogReaction.h"
#include "mqCreateLMKDialogReaction.h"
#include "mqEditFLGDialogReaction.h"
#include "mqEditACTORDialogReaction.h"
#include "mqMeshToolsCore.h"
#include "mqOpenDataReaction.h"
#include "mqCameraReaction.h"
#include "mqDisplayReaction.h"
#include <vtkRenderer.h>

#include <QToolButton>
#include <QHBoxLayout>

//-----------------------------------------------------------------------------
void mqObjectsControlsToolbar::constructor()
{
 // Ui::mqObjectsControlsToolbar ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqObjectsControlsToolbar;
  this->ui->setupUi(this);
  this->setStyleSheet(

	  "QSlider::groove:vertical{"
	  "border: 1px solid #999999;"
	  "width: 10px;" /* the groove expands to the size of the slider by default. by giving it a width, it has a fixed size */
	  "margin: 0px -4;"
	  "}"

	  "QSlider::handle:vertical{"
	  "background: qlineargradient(x1 : 0, y1 : 0, x2 : 1, y2 : 1, stop : 0 #b4b4b4, stop:1 #8f8f8f);"
	  "border: 1px solid #5c5c5c;"
	  "height: 10px;"
	  "margin: 0px -2;" /* handle is placed by default on the contents rect of the groove. Expand outside the groove */
	  "}"

	  "QSlider::add-page:vertical{"
	  "background:yellow;"
	  "margin: 1px;}"
	  
	  "QSlider::sub-page:vertical{"
	  "background:white;"
	 "margin: 1px;"
	  "}"


	  "QScrollBar{background-color:orange}"

	  "QScrollBar::handle:vertical{background:yellow;max-height:10px;} "

  );
  this->zTr = new QReleaseSlider;
  this->zTr->setMaximum(100);
  this->zTr->setMinimum(-100);
  this->zTr->setToolTip(QString("Translate along z viewing axis"));


  //this->zRot = new QReleaseSliderValue(Qt::Vertical, tr(""));
  this->zRot = new QReleaseSlider;
  this->zRot->setMaximum(90);
  this->zRot->setMinimum(-90);
  this->zRot->setToolTip(QString("Rotation along z viewing axis"));


  this->yTr = new QReleaseSlider;
  this->yTr->setMaximum(100);
  this->yTr->setMinimum(-100);
  this->yTr->setToolTip(QString("Translate along y viewing axis"));

  //this->yRot = new QReleaseSliderValue(Qt::Vertical, tr(""));
  this->yRot = new QReleaseSlider;
  this->yRot->setMaximum(90);
  this->yRot->setMinimum(-90);
  this->yRot->setToolTip(QString("Rotation along y viewing axis"));

  
  this->xTr = new QReleaseSlider;
  this->xTr->setMaximum(100);
  this->xTr->setMinimum(-100);
  this->xTr->setToolTip(QString("Translate along x viewing axis"));

  //this->xRot = new QReleaseSliderValue(Qt::Vertical, tr(""));
  this->xRot = new QReleaseSlider;
  this->xRot->setMaximum(90);
  this->xRot->setMinimum(-90);
  this->xRot->setToolTip(QString("Rotation along x viewing axis"));

  QHBoxLayout *zlayout = new QHBoxLayout;
  QWidget* zgrid = new QWidget();
  zlayout->setSpacing(1);
  zlayout->setMargin(5);
  zlayout->addWidget(this->zRot);
 // zlayout->addWidget(this->xTr);
  //zlayout->addWidget(this->yTr);
  zlayout->addWidget(this->zTr);

  zgrid->setLayout(zlayout);
  this->addWidget(zgrid);

  QHBoxLayout *ylayout = new QHBoxLayout;
  QWidget* ygrid = new QWidget();
 ylayout->setSpacing(1);
  ylayout->setMargin(5);
  ylayout->addWidget(this->yRot);
  ylayout->addWidget(this->yTr);

  ygrid->setLayout(ylayout);
  this->addWidget(ygrid);

  QHBoxLayout *xlayout = new QHBoxLayout;
  QWidget* xgrid = new QWidget();
  xlayout->setSpacing(1);
  xlayout->setMargin(5);
 xlayout->addWidget(this->xRot);
 xlayout->addWidget(this->xTr);

  xgrid->setLayout(xlayout);
  this->addWidget(xgrid);

  connect(this->ui->actionDelete, SIGNAL(triggered()), this, SLOT(slotDeleteObjects()));
  
  connect(zRot, SIGNAL(valueChanged(int)), this, SLOT(slotZrot(int)));
  connect(yRot, SIGNAL(valueChanged(int)), this, SLOT(slotYrot(int)));
  connect(xRot, SIGNAL(valueChanged(int)), this, SLOT(slotXrot(int)));
  
}

void mqObjectsControlsToolbar::RotateActors(int axis, int degrees)
{
	cout << "Rotate axis: " << axis << ", degrees:" << degrees << endl;
//axis: 0=X, 1=Y, 2=z
	/*
	double rot_center[3] = { 0,0,0 };

	mqMeshToolsCore::instance()->GetCenterOfMassOfSelectedActors(rot_center);
	//cout << "rotation center: " << rot_centerendl;
	//cout << "Rotation center: " << rot_center[0] << "," << rot_center[1] << "," << rot_center[2] << endl;
	//cout << "bb length...." << endl;
	double boundRadius = mqMeshToolsCore::instance()->GetBoundingBoxLengthOfSelectedActors();
	//cout << "Bound Radius: " << boundRadius << endl;
	if (boundRadius == std::numeric_limits<double>::infinity())
	{
		boundRadius = 60;
	}
	else
	{
		boundRadius *= 0.5;
	}
	// GetLength gets the length of the diagonal of the bounding box
	

	// Get the view up and view right vectors
	double view_up[3], view_look[3], view_right[3];

	mqMeshToolsCore::instance()->getCamera()->OrthogonalizeViewUp();
	mqMeshToolsCore::instance()->getCamera()->ComputeViewPlaneNormal();
	mqMeshToolsCore::instance()->getCamera()->GetViewUp(view_up);
	vtkMath::Normalize(view_up);
	mqMeshToolsCore::instance()->getCamera()->GetViewPlaneNormal(view_look);
	vtkMath::Cross(view_up, view_look, view_right);
	vtkMath::Normalize(view_right);

	// Get the furtherest point from object position+origin
	double outsidept[3];

	outsidept[0] = rot_center[0] + view_right[0] * boundRadius;
	outsidept[1] = rot_center[1] + view_right[1] * boundRadius;
	outsidept[2] = rot_center[2] + view_right[2] * boundRadius;

	// Convert them to display coord
	double disp_obj_center[3];

	this->ComputeWorldToDisplay(rot_center[0], rot_center[1], rot_center[2],
		disp_obj_center);

	this->ComputeWorldToDisplay(outsidept[0], outsidept[1], outsidept[2],
		outsidept);

	double radius = sqrt(vtkMath::Distance2BetweenPoints(disp_obj_center,
		outsidept));
	double nxf = (rwi->GetEventPosition()[0] - disp_obj_center[0]) / radius;

	double nyf = (rwi->GetEventPosition()[1] - disp_obj_center[1]) / radius;

	double oxf = (rwi->GetLastEventPosition()[0] - disp_obj_center[0]) / radius;

	double oyf = (rwi->GetLastEventPosition()[1] - disp_obj_center[1]) / radius;

	if (((nxf * nxf + nyf * nyf) <= 1.0) &&
		((oxf * oxf + oyf * oyf) <= 1.0))
	{
		double newXAngle = vtkMath::DegreesFromRadians(asin(nxf));
		double newYAngle = vtkMath::DegreesFromRadians(asin(nyf));
		double oldXAngle = vtkMath::DegreesFromRadians(asin(oxf));
		double oldYAngle = vtkMath::DegreesFromRadians(asin(oyf));

		double scale[3];
		scale[0] = scale[1] = scale[2] = 1.0;

		double **rotate = new double*[2];

		rotate[0] = new double[4];
		rotate[1] = new double[4];

		rotate[0][0] = newXAngle - oldXAngle;
		rotate[0][1] = view_up[0];
		rotate[0][2] = view_up[1];
		rotate[0][3] = view_up[2];

		rotate[1][0] = oldYAngle - newYAngle;
		rotate[1][1] = view_right[0];
		rotate[1][2] = view_right[1];
		rotate[1][3] = view_right[2];

		this->ActorCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				//cout << "Apply prop3Dtransform" << endl;
				for (vtkIdType j = 0; j < 2; j++)
				{
					for (vtkIdType k = 0; k < 4; k++)
					{
						//cout << "rotate["<<j<<"]"<<"["<<k<<"]="<< rotate[j][k] << endl;

					}
				}

				//cout << "scale:" << scale[0] << ","<< scale[1] << ","<< scale[2] << endl;

				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				myActor->SetChanged(1);
			}
		}
		this->NormalLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
				myActor->SetChanged(1);
			}
		}
		this->TargetLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
				myActor->SetChanged(1);
			}
		}
		this->NodeLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
				myActor->SetChanged(1);
			}
		}
		this->HandleLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
				myActor->SetChanged(1);
			}
		}
		this->FlagLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
				myActor->SetChanged(1);
			}
		}

		delete[] rotate[0];
		delete[] rotate[1];
		delete[] rotate;

		if (this->AutoAdjustCameraClippingRange)
		{
			this->CurrentRenderer->ResetCameraClippingRange();
			mqMeshToolsCore::instance()->ActivateClippingPlane();
		}

		rwi->Render();
	}*/
}
void mqObjectsControlsToolbar::slotDeleteObjects()
{
	mqMeshToolsCore::instance()->DeleteSelectedActors();
	mqMeshToolsCore::instance()->Render();


	
	
	
}
void mqObjectsControlsToolbar::slotZrot(int val)
{
	
	if (val == 0) {
		// here the actors should be updated for good!
		return; }

	this->RotateActors(2, val);

	
}
void mqObjectsControlsToolbar::slotXrot(int val)
{	
	if (val == 0) {
		// here the actors should be updated for good!
		return;
	}

	this->RotateActors(0, val);

}
void mqObjectsControlsToolbar::slotYrot(int val)
{
	if (val == 0) {
		// here the actors should be updated for good!
		return;
	}

	this->RotateActors(1, val);

}