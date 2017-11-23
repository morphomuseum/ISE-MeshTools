/*=========================================================================

   Program: MeshTools
   Module:    mqCameraControlsToolbar.h

 

========================================================================*/
#ifndef mqCameraControlsToolbar_h
#define mqCameraControlsToolbar_h


#include <QToolBar>
class Ui_mqCameraControlsToolbar;
class QDoubleSlider;
class QDial;
class QDoubleReleaseSlider;
class QReleaseSlider;
class QReleaseSliderValue;
/**
* mqCameraControlsToolbar is the toolbar with actions (and reactions) for the
* "Camera Controls" toolbar in MeshTools. 
*/
class  mqCameraControlsToolbar : public QToolBar
{
  Q_OBJECT
  typedef QToolBar Superclass;

public:
  mqCameraControlsToolbar(const QString& title, QWidget* parentObject = 0)
    : Superclass(title, parentObject)
  {
    this->constructor();
  }
  mqCameraControlsToolbar(QWidget* parentObject = 0)
    : Superclass(parentObject)
  {
    this->constructor();
  }
 

  public slots :
  virtual void slotClippingPlaneOnOff();
  virtual void slotBackfaceCullingOnOff();
  
private:
	Q_DISABLE_COPY(mqCameraControlsToolbar);
	//QReleaseSlider * zRot;
	QDial * zRot;
	//QDial * camzRot;
	QReleaseSliderValue *cP;
	//QDoubleSlider * cP;
	QDoubleReleaseSlider * zoom;
  Ui_mqCameraControlsToolbar *ui;
  void constructor();
};

#endif
