# Set your files and resources here
SET (Helpers_SRCS
vtkOrientationHelperActor.cxx vtkGridActor.cxx vtkMTActor.cxx vtkOrientationHelperWidget.cxx vtkInteractorStyleMT.cxx )

set( Helpers_HDRS vtkOrientationHelperActor.h vtkMTActor.h vtkGridActor.h vtkOrientationHelperWidget.h vtkInteractorStyleMT.h)

ADD_LIBRARY (MeshToolsHelpers ${Helpers_SRCS} ${Helpers_HDRS})

#TARGET_LINK_LIBRARIES(MeshToolsHelpers  vtkGraphics vtkIOCore vtkCommonCore)



