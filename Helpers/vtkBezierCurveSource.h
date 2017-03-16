

#ifndef VTK_BEZIER_CURVE_SOURCE_H
#define VTK_BEZIER_CURVE_SOURCE_H

#include <vtkPolyDataAlgorithm.h>
#include "vtkLMActorCollection.h"

class vtkImageData;
class vtkTransform;

class vtkBezierCurveSource : public vtkPolyDataAlgorithm
{
public:
  static vtkBezierCurveSource *New();
  vtkTypeMacro(vtkBezierCurveSource, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  vtkPolyData* GetControlPointsOutput();
  vtkPolyData* GetBezierCurveOutput(); // same as GetOutput()

  void SetNodes(vtkSmartPointer<vtkLMActorCollection> Nodes);
  void SetHandles(vtkSmartPointer<vtkLMActorCollection> Handles);
  
protected:

  vtkBezierCurveSource();
  ~vtkBezierCurveSource();
  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
  vtkBezierCurveSource(const vtkBezierCurveSource&);  // Not implemented.
  void operator=(const vtkBezierCurveSource&);  // Not implemented.

  void UpdateControlPointsPolyData(vtkPolyData* pd);
  void UpdateBezierCurvePolyData(vtkPolyData* pd);

private:
	vtkSmartPointer<vtkLMActorCollection> Nodes;
	vtkSmartPointer<vtkLMActorCollection> Handles;
  
};

#endif
