# Set your files and resources here
SET (Helpers_SRCS
vtkOrientationHelperActor.cxx vtkGridActor.cxx vtkMTActor.cxx vtkOrientationHelperWidget.cxx )

set( Helpers_HDRS vtkOrientationHelperActor.h vtkMTActor.h vtkGridActor.h vtkOrientationHelperWidget.h )

ADD_LIBRARY (MeshToolsHelpers ${Helpers_SRCS} ${Helpers_HDRS})

#TARGET_LINK_LIBRARIES(MeshToolsHelpers  vtkGraphics vtkIOCore vtkCommonCore)



