
#ifndef __Landmark_Transform_h
#define __Landmark_Transform_h

#include <vtkCommonTransformsModule.h> // For export macro
#include "vtkLinearTransform.h"
#include "vtkFloatArray.h"
#include <vtkSystemIncludes.h> // m
#include <vtksys/SystemTools.hxx>  //m
#include "vtkLandmarkTransform.h"

#define VTK_LANDMARK_RIGIDBODY 6
#define VTK_LANDMARK_SIMILARITY 7
#define VTK_LANDMARK_AFFINE 12


class vtkMatrix4x4; // m
class vtkObjectFactory; // m
class vtkCommonTransformsModule; // m
class vtkLinearTransform; // m
class vtkFloatArray; // m


class Landmark_Transform : public vtkLandmarkTransform
{
public:
	static Landmark_Transform *New();

	vtkTypeMacro(Landmark_Transform, vtkLandmarkTransform);
	
	void SetLandmarksWeight(vtkFloatArray *weight);

	// idee: ajouter affine si ça marche

	
	// Description:
	// Make another transform of the same type.
//	vtkAbstractTransform *MakeTransform();

protected:
	Landmark_Transform();
	~Landmark_Transform();

	// Update the matrix from the quaternion.
	void InternalUpdate();

	// Description:
	// This method does no type checking, use DeepCopy instead.
//	void InternalDeepCopy(vtkAbstractTransform *transform);
	
	vtkFloatArray* LandmarksWeight;

private:
	Landmark_Transform(const Landmark_Transform&);  // Not implemented.
	void operator=(const Landmark_Transform&);  // Not implemented.
};
#endif
