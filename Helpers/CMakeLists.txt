# Set your files and resources here
SET (Helpers_SRCS
vtkOrientationHelperActor.cxx vtkOrientationHelperWidget.cxx)

set( Helpers_HDRS vtkOrientationHelperActor.h vtkOrientationHelperWidget.h)

ADD_LIBRARY (MeshToolsHelpers ${Helpers_SRCS} ${Helpers_HDRS})

TARGET_LINK_LIBRARIES(MeshToolsHelpers  vtkGraphics vtkIO vtkCommon)

IF (VTK_USE_PARALLEL)
  TARGET_LINK_LIBRARIES (MeshToolsHelpers vtkParallel)
ENDIF (VTK_USE_PARALLEL)

IF (VTK_USE_RENDERING)
  TARGET_LINK_LIBRARIES (MeshToolsHelpers vtkRendering)
  TARGET_LINK_LIBRARIES (MeshToolsHelpers  vtkVolumeRendering)
  TARGET_LINK_LIBRARIES (MeshToolsHelpers  vtkHybrid)
  TARGET_LINK_LIBRARIES (MeshToolsHelpers  vtkWidgets)
ENDIF (VTK_USE_RENDERING)


